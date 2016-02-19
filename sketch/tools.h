#ifndef TOOLS_H
#define TOOLS_H

#include <Arduino.h>



// extracts a form token string from str, starting from iStart
// returns end of found token or -1
int getFormToken(const String& str, String& token, int iStart);



// extracts a url-encoded key/value pair from str to key and value, starting at iStart
// returns end of found key/value pair or -1
bool splitKVPair(const String& token, String& key, String& value);



/* Converts an integer value to its hex character*/
static inline char to_hex(char code) {
  static const char hex[] = "0123456789abcdef";
  return hex[code & 15];
}



/* Converts a hex character to its integer value */
static inline char from_hex(char ch) {
  return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

/* Converts a url-encoded string to ascii */
void urldecode(const char* src, char* dest, size_t destsize);

#endif // TOOLS_H
