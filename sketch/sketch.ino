/***
 * ESP8266 shopping list printer
 * Adapt to your needs in settings.cpp! (SSID, password, device name, language, server port)
 */

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <elapsedMillis.h>

#include "shoppinglist.h"
#include "tools.h"
#include "localization.h"
#include "settings.h"

#define GPIO0 0
#define DebugSerial Serial
#define PrinterSerial Serial1

ShoppingList shoppingList;

// TCP server will respond to HTTP requests
WiFiServer server(serverHttpPort);

/* add a url-encoded item name to the shopping list */
void addItemByForm(const String& str)
{
  if(str.length() > 0)
  {
    int iStart = 0;
    int iEnd;
    while (iStart < str.length())
    {
      String token;
      iEnd = getFormToken(str, token, iStart);
      String key;
      String value;
      if(splitKVPair(token, key, value))
      {
        if((key == "itemname") && (value != ""))
        {
          shoppingList.add(value);
          break;
        }
      }
      iStart = iEnd+1;
    }
  }
}

/* delete a url-encoded item ID from the shopping list */
void deleteItemsByForm(const String& str)
{
  if(str.length() > 0)
  {
    int iStart = 0;
    int iEnd;
    while (iStart < str.length())
    {
      String token;
      iEnd = getFormToken(str, token, iStart);
      String key;
      String value;
      if(splitKVPair(token, key, value))
      {
        if((key == "id_list%5B%5D") && (value != ""))
        {
          char cstr[value.length()+1];
          urldecode(value.c_str(), cstr, value.length()+1);
          int id = String(cstr).toInt();
          DebugSerial.println("Deleting " + String(id));
          shoppingList.removeById(id);
        }
      }
      iStart = iEnd+1;
    }
  }
}

void setup(void)
{  
  DebugSerial.begin(115200);
  PrinterSerial.begin(115200);
  
  // Connect to WiFi network
  WiFi.begin(ssid, password);
  DebugSerial.println("");  
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    DebugSerial.print(".");
  }
  DebugSerial.println("");
  DebugSerial.print("Connected to ");
  DebugSerial.println(ssid);
  DebugSerial.print("IP address: ");
  DebugSerial.println(WiFi.localIP());

  // Set up mDNS responder:
  // - first argument is the domain name, in this example
  //   the fully-qualified domain name is "esp8266.local"
  // - second argument is the IP address to advertise
  //   we send our IP address on the WiFi network
  if (!MDNS.begin(devicename)) {
    DebugSerial.println("Error setting up MDNS responder!");
    while(1) { 
      delay(1000);
    }
  }
  DebugSerial.println("mDNS responder started");
  
  // Start TCP (HTTP) server
  server.begin();
  DebugSerial.println("TCP server started");
  
  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);

  // heating params
  PrinterSerial.write(0x1B);
  PrinterSerial.write(0x37);
  PrinterSerial.write(15);
  PrinterSerial.write(165);
  PrinterSerial.write(15);
  delay(500);

  // character page
  PrinterSerial.write(0x1B);
  PrinterSerial.write('t');
  PrinterSerial.write(23); // 23: ISO8859-1
  delay(500);

  // bold on
  PrinterSerial.write(0x1B);
  PrinterSerial.write('E');
  PrinterSerial.write(1);
  delay(500);

  PrinterSerial.println("initialized");
}

static const uint8_t buttonPeriod = 10;
static elapsedMillis buttonTimer;
void loop(void)
{
  if(buttonTimer >= buttonPeriod)
  {
    pinMode(GPIO0, INPUT);
    delayMicroseconds(10);
    if ((digitalRead(GPIO0) == 0) && (shoppingList.isEmpty() == false))
    {
      /* Print list */
      const ShoppingList::Item* pItem = shoppingList.items();
      PrinterSerial.print('\n');
      while(pItem != nullptr)
      {
        DebugSerial.print("Item #");DebugSerial.println(pItem->id());
        pItem->printOn(PrinterSerial, format_tag::ascii_t());
        PrinterSerial.print('\n');
        pItem->printOn(DebugSerial, format_tag::html_t());
        DebugSerial.print('\n');
        DebugSerial.print('\n');
        pItem = pItem->next();
        if(pItem != nullptr)
        {
          delay(500);
        }
      }
      shoppingList.clear();
    }
    buttonTimer = 0;
  }

  if(!shoppingList.isEmpty())
  {
    pinMode(GPIO0, OUTPUT);
    digitalWrite(GPIO0, 0);
  }

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  DebugSerial.println("");
  DebugSerial.println("New client");

  // Wait for data from client to become available
  while(client.connected() && !client.available()){
    delay(1);
  }
  
  // Read the first line of HTTP request
  String req = client.readStringUntil('\r');
  
  // First line of HTTP request looks like "GET /path HTTP/1.1"
  // Retrieve the "/path" part by finding the spaces
  int addr_start = req.indexOf(' ');
  int addr_end = req.indexOf(' ', addr_start + 1);
  if (addr_start == -1 || addr_end == -1) {
    DebugSerial.print("Invalid request: ");
    DebugSerial.println(req);
    return;
  }
  req = req.substring(addr_start + 1, addr_end);
  DebugSerial.print("Request: ");
  DebugSerial.println(req);

  String body;
  while(client.available())
  {
    String line = client.readStringUntil('\r');
    if (client.peek() == '\n')
    {
      client.read();
    }
    if ((line == "") && client.available())
    {
      body = client.readStringUntil('\r');
      break;
    }
    else
    {
      /* extra header, might be useful */
      DebugSerial.println("  skipped extra header: \"" + String(line) + "\"");
    }
  }
  /***
   * extract POST data, if any
   */
  body.trim();
  if(body.length() > 0)
  {
    int iStart = 0;
    int iEnd;
    while (iStart < body.length())
    {
      String token;
      iEnd = getFormToken(body, token, iStart);
      DebugSerial.print("token: ");DebugSerial.println(token);
      String key;
      String value;
      if(splitKVPair(token, key, value))
      {
        if(key == "action")
        {
          if(value == "add")
          {
            addItemByForm(body);
          }
          else if(value == "delete")
          {
            deleteItemsByForm(body);
          }
        }
        DebugSerial.print("    key: ");DebugSerial.println(key);
        DebugSerial.print("  value: ");DebugSerial.println(value);
      }
      iStart = iEnd+1;
    }
  }

  /***
   * discard all other data
   */
  client.flush();
  
  if (req == "/")
  {
    IPAddress ip = WiFi.localIP();
    //String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html><h1>");
    client.print(localization[iLanguage].htmlPageTitle);
    client.print("</h1>""<main role=\"main\">\r\n"
      "<form action=\"\" method=\"POST\" name=\"form\">\r\n");
    
    /** create new item */
    client.print(localization[iLanguage].htmlAddItemLabel);
    client.print("<br>\r\n"
      "<input type=\"text\" name=\"itemname\" maxlength=\"" + String(ShoppingList::Item::max_str_size) + "\">"
      "<button type=\"submit\" name=\"action\" value=\"add\" method=\"POST\">");
    client.print(localization[iLanguage].htmlAddBtnLabel);
    client.print("</button>"
      "<br>\r\n");
    
    /* existing items */
    client.print(localization[iLanguage].htmlListTitle);
    client.print(":<br>\r\n");

    const ShoppingList::Item* pItem = shoppingList.items();
    while(pItem != nullptr)
    {
      client.print("<label><input type=\"checkbox\" name=\"id_list[]\" value=\"");
      client.print(pItem->id());
      client.print("\">");
      pItem->printOn(client, format_tag::html_t());
      client.print("</label><br>\r\n");
      pItem = pItem->next();
    }

    client.print("<button type=\"submit\" name=\"action\" value=\"delete\">");
    client.print(localization[iLanguage].htmlDeleteBtnLabel);
    client.print("</button>\r\n"
         "</form>\r\n"
         "</main>\r\n"
         "</html>\r\n\r\n");
    DebugSerial.println("Sending 200");
  }
  else
  {
    client.print("HTTP/1.1 404 Not Found\r\n\r\n");
    DebugSerial.println("Sending 404");
  }
  
  DebugSerial.println("Done with client");
}

