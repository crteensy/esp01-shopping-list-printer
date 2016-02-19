#include "tools.h"



int getFormToken(const String& str, String& token, int iStart)
{
  int iEnd = str.length();
  int iAmp = str.indexOf('&', iStart);
  if (iAmp > iStart)
  {
    iEnd = iAmp;
  }
  if(iEnd > iStart)
  {
    token = str.substring(iStart, iEnd);
  }
  return iEnd;
}



bool splitKVPair(const String& token, String& key, String& value)
{
  int iEqu = token.indexOf('=', 1);
  if (iEqu < 0)
  {
    return false;
  }
  key = token.substring(0, iEqu);
  value = token.substring(iEqu+1);
  return true;
}



void urldecode(const char* src, char* dest, size_t destsize)
{
  const char* pSrc = src;
  char* pDest = dest;
  size_t written = 0;
  while((written < (destsize - 1)) && (*pSrc != 0))
  {
    if(*pSrc == '%')
    {
      if ((pSrc[1] != 0) && (pSrc[2] != 0))
      {
        *pDest = from_hex(pSrc[1]) << 4 | from_hex(pSrc[2]);
        pSrc += 2;
      }
    }
    else if(*pSrc =='+')
    {
      *pDest = ' ';
    }
    else
    {
      *pDest = *pSrc;
    }
    pSrc++;
    pDest++;
    written++;
  }
  *pDest = 0;
}

