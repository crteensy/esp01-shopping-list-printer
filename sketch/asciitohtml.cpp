#include "asciitohtml.h"

const char asciiToHtml[256][9] = {
  //   0 ..  15
  "", "", "", "",   "", "", "", "",   "", "", "", "",   "", "", "", "",
  //  16 ..  31
  "", "", "", "",   "", "", "", "",   "", "", "", "",   "", "", "", "",
  //  32 ..  47
  "", "", "&quot;", "",   "", "", "&amp;", "",   "", "", "", "",   "", "", "", "",
  //  48 ..  63
  "", "", "", "",   "", "", "", "",   "", "", "", "",   "&lt;", "", "&gt;", "",
  //  64 ..  79
  "", "", "", "",   "", "", "", "",   "", "", "", "",   "", "", "", "",
  //  80 ..  95
  "", "", "", "",   "", "", "", "",   "", "", "", "",   "", "", "", "",
  //  96 .. 111
  "", "", "", "",   "", "", "", "",   "", "", "", "",   "", "", "", "",
  // 112 .. 127
  "", "", "", "",   "", "", "", "",   "", "", "", "",   "", "", "", "",
  // 128 .. 143
  "", "", "", "",   "", "", "", "",   "", "", "", "",   "", "", "", "",
  // 144 .. 159
  "", "", "", "",   "", "", "", "",   "", "", "", "",   "", "", "", "",
  // 160 .. 175
  "&nbsp;", "&iexcl;", "&cent;", "&pound;",   "&curren;", "&yen;", "&brvbar;", "&sect;",   "&uml;", "&copy;", "&ordf;", "&laquo;",   "&not;", "&shy;", "&reg;", "&macr;",
  // 176 .. 191
  "&deg;", "&plusmn;", "&sup2;", "&sup3;",   "&acute;", "&micro;", "&para;", "&middot;",   "&cedil;", "&sup1;", "&ordm;", "&raquo;",   "&frac14;", "&frac12;", "&frac34;", "&iquest;",
  // 192 .. 207
  "&Agrave;", "&Aacute;", "&Acirc;", "&Atilde;",   "&Auml;", "&Aring;", "&AElig;", "&Ccedil;",   "&Egrave;", "&Eacute;", "&Ecirc;", "&Euml;",   "&Igrave;", "&Iacute;", "&Icirc;", "&Iuml;",
  // 208 .. 223
  "&ETH;", "&Ntilde;", "&Ograve;", "&Oacute;",   "&Ocirc;", "&Otilde;", "&Ouml;", "&times;",   "&Oslash;", "&Ugrave;", "&Uacute;", "&Ucirc;",   "&Uuml;", "&Yacute;", "&THORN;", "&szlig;",
  // 224 .. 239
  "&agrave;", "&aacute;", "&acirc;", "&atilde;",   "&auml;", "&aring;", "&aelig;", "&ccedil;",   "&egrave;", "&eacute;", "&ecirc;", "&euml;",   "&igrave;", "&iacute;", "&icirc;", "&iuml;",
  // 240 .. 255
  "&eth;", "&ntilde;", "&ograve;", "&oacute;",   "&ocirc;", "&otilde;", "&ouml;", "&divide;",   "&oslash;", "&ugrave;", "&uacute;", "&ucirc;",   "&uuml;", "&yacute;", "&thorn;", "&yuml;",
};

