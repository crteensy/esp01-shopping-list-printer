#ifndef SHOPPINGLIST_H
#define SHOPPINGLIST_H

#include <Arduino.h>

#include "asciitohtml.h"
#include "tools.h"

namespace format_tag
{
  typedef struct {} html_t;
  typedef struct {} ascii_t;
}


class ShoppingList
{
  public:
    typedef int ID_t;
    class Item
    {
      friend class ShoppingList;
      public:
        
        static constexpr size_t max_str_size = 128;
        Item* next() const
        {
          return m_pNext;
        }

        ID_t id() const
        {
          return m_id;
        }

        void markAsPrinted()
        {
          m_printed = true;
        }

        template <typename PrintClass> void printOn(PrintClass& printer, format_tag::html_t) const
        {
          const char* pSrc = m_str;
          while(*pSrc != 0)
          {
            char c = *pSrc;
            if(asciiToHtml[c][0] != 0)
            {
              printer.print(asciiToHtml[c]);
            }
            else
            {
              printer.print(c);
            }

//            char out[6] = "&#x  ;";
//            char c = *pSrc;
//            out[3] = to_hex(c >> 4);
//            out[4] = to_hex(c);
//            printer.print(out);

            pSrc++;
          }
        }

        template <typename PrintClass> void printOn(PrintClass& printer, format_tag::ascii_t) const
        {
          printer.print(m_str);
        }

      private:
        //destsize includes space for terminating zero
        Item(const String& urlstr, const ID_t& id, Item* next) :
          m_id(id),
          m_pNext(next),
          m_printed(false)
        {
          urldecode(urlstr.c_str(), m_str, max_str_size);
        }

        void setNext(Item* next)
        {
          m_pNext = next;
        }

        char m_str[max_str_size];
        const ID_t m_id;
        Item* m_pNext;
        bool m_printed;
    };

    ShoppingList() :
      m_pItems(nullptr),
      m_nextId(0)
    {
      
    }

    const Item* items() const
    {
      return m_pItems;
    }

    bool add(const String& str)
    {
      String trimmed(str);
      trimmed.trim();
      if(trimmed == "")
      {
        return false;
      }
      Item* newItem = new Item(trimmed, m_nextId, m_pItems);
      if(newItem == nullptr)
      {
        return false;
      }
      m_pItems = newItem;
      m_nextId++;
    }

    Item* lookupById(const ID_t& id) const
    {
      Item* pItem = m_pItems;
      while((pItem != nullptr) && (pItem->id() != id))
      {
        pItem = pItem->next();
      }
      return pItem;
    }

    bool removeById(const ID_t& id)
    {
      Item* pItem = lookupById(id);
      return remove(pItem);
    }

    bool remove(Item* item)
    {
      if(item == nullptr)
      {
        return false;
      }
      else if(m_pItems == item)
      {
        m_pItems = item->next();
        delete item;
        return true;
      }
      else
      {
        Item* pPrevious = m_pItems;
        while((pPrevious != nullptr) && (pPrevious->next() != item))
        {
          pPrevious = pPrevious->next();
        }
        if(pPrevious == nullptr)
        {
          return false;
        }
        pPrevious->setNext(item->next());
        delete item;
        return true;
      }
    }

    bool isEmpty() const
    {
      return (m_pItems == nullptr);
    }

    void clear()
    {
      while(m_pItems != nullptr)
      {
        remove(m_pItems);
      }
    }

  private:
    Item* m_pItems;
    ID_t m_nextId;
};

#endif
