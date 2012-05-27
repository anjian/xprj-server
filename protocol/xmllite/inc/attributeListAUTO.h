/* C code produced by gperf version 3.0.1 */
/* Command-line: gperf --struct-type --ignore-case -N findAttribute -H getAttrHash AttributeList.input  */
/* Computed positions: -k'1,3,7' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif


#include <interface/protocol/UcWebTagInfo.h>
/*!!!!!! Important notes!!!!

1. gperf  --struct-type --ignore-case  -N findAttribute -H getAttrHash AttributeList.input > AttributeListAUTO.h

2. Please use below to replace the get method. It looks like      
  if ((((unsigned char)*str ^ (unsigned char)*s) & ~32) == 0 && !gperf_case_strcmp (str, s))
  Please replace it as below. 
  if ((((UC_BYTE)*str ^ (UC_BYTE)*s) & ~32) == 0 
			  && !WMLStrNICmp(str, s, len))
3. Remove the gperf_case_strcmp code.

4. Avoid the name conflict with the attributeListAUTO.h for 
#define TOTAL_KEYWORDS 85
#define MIN_WORD_LENGTH 1
#define MAX_WORD_LENGTH 10
#define MIN_HASH_VALUE 1
#define MAX_HASH_VALUE 217

Recommend to add a prefix to it

*/
struct Wap2Keyword {const UC_CHAR * name;UC_INT token;};

#define TOTAL_KEYWORDS 98
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 15
#define MIN_HASH_VALUE 2
#define MAX_HASH_VALUE 182
/* maximum key range = 181, duplicates = 0 */

static unsigned int
getAttrHash (register const char *str, register unsigned int len)
{
  static unsigned char asso_values[] =
    {
      183, 183, 183, 183, 183, 183, 183, 183, 183, 183,
      183, 183, 183, 183, 183, 183, 183, 183, 183, 183,
      183, 183, 183, 183, 183, 183, 183, 183, 183, 183,
      183, 183, 183, 183, 183, 183, 183, 183, 183, 183,
      183, 183, 183, 183, 183,  10, 183, 183, 183, 183,
      183, 183, 183, 183, 183, 183, 183, 183, 183, 183,
      183, 183, 183, 183, 183,   5,   5,  55,  25,  25,
       95,  65,  15,  45,   0,  10,  10,  60,   0,   0,
       35,  75,  40,  30,  15, 120,   0,  65,  30,  85,
        0, 183, 183, 183, 183, 183, 183,   5,   5,  55,
       25,  25,  95,  65,  15,  45,   0,  10,  10,  60,
        0,   0,  35,  75,  40,  30,  15, 120,   0,  65,
       30,  85,   0, 183, 183, 183, 183, 183, 183, 183,
      183, 183, 183, 183, 183, 183, 183, 183, 183, 183,
      183, 183, 183, 183, 183, 183, 183, 183, 183, 183,
      183, 183, 183, 183, 183, 183, 183, 183, 183, 183,
      183, 183, 183, 183, 183, 183, 183, 183, 183, 183,
      183, 183, 183, 183, 183, 183, 183, 183, 183, 183,
      183, 183, 183, 183, 183, 183, 183, 183, 183, 183,
      183, 183, 183, 183, 183, 183, 183, 183, 183, 183,
      183, 183, 183, 183, 183, 183, 183, 183, 183, 183,
      183, 183, 183, 183, 183, 183, 183, 183, 183, 183,
      183, 183, 183, 183, 183, 183, 183, 183, 183, 183,
      183, 183, 183, 183, 183, 183, 183, 183, 183, 183,
      183, 183, 183, 183, 183, 183, 183, 183, 183, 183,
      183, 183, 183, 183, 183, 183, 183
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[6]];
      /*FALLTHROUGH*/
      case 6:
      case 5:
      case 4:
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      /*FALLTHROUGH*/
      case 2:
      case 1:
        hval += asso_values[(unsigned char)str[0]+1];
        break;
    }
  return hval;
}

#ifdef __GNUC__
__inline
#endif
struct Wap2Keyword *
findAttribute (register const char *str, register unsigned int len)
{
  static struct Wap2Keyword wordlist[] =
    {
      {""}, {""},
      {"id",                                             WEB_ATTRIBUTE_TYPE_ID},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {"iname",                                          WEB_ATTRIBUTE_TYPE_INAME},
      {"ivalue",                                         WEB_ATTRIBUTE_TYPE_IVALUE},
      {""}, {""},
      {"abbr",                                           WEB_ATTRIBUTE_TYPE_ABBR},
      {"abbrs",                                          WEB_ATTRIBUTE_TYPE_ABBRS},
      {""}, {""}, {""},
      {"size",                                           WEB_ATTRIBUTE_TYPE_SIZE},
      {"scope",                                          WEB_ATTRIBUTE_TYPE_SCOPE},
      {"method",                                         WEB_ATTRIBUTE_TYPE_METHOD},
      {""},
      {"alt",                                            WEB_ATTRIBUTE_TYPE_ALT},
      {""},
      {"start",                                          WEB_ATTRIBUTE_TYPE_START},
      {"action",                                         WEB_ATTRIBUTE_TYPE_ACTION},
      {""},
      {"multiple",                                       WEB_ATTRIBUTE_TYPE_MULTIPLE},
      {""},
      {"media",                                          WEB_ATTRIBUTE_TYPE_MEDIA},
      {"sub_menu_id",                                    WEB_ATTRIBUTE_TYPE_SUB_MENU_ID},
      {""},
      {"rev",                                            WEB_ATTRIBUTE_TYPE_REV},
      {"char",                                           WEB_ATTRIBUTE_TYPE_CHAR},
      {"class",                                          WEB_ATTRIBUTE_TYPE_CLASS},
      {"scheme",                                         WEB_ATTRIBUTE_TYPE_SCHEME},
      {""}, {""},
      {"cols",                                           WEB_ATTRIBUTE_TYPE_COLS},
      {"color",                                          WEB_ATTRIBUTE_TYPE_COLOR},
      {""},
      {"colspan",                                        WEB_ATTRIBUTE_TYPE_COLSPAN},
      {"rel",                                            WEB_ATTRIBUTE_TYPE_REL},
      {"data",                                           WEB_ATTRIBUTE_TYPE_DATE},
      {"rules",                                          WEB_ATTRIBUTE_TYPE_RULES},
      {""},
      {"content",                                        WEB_ATTRIBUTE_TYPE_CONTENT},
      {"icon_src",                                       WEB_ATTRIBUTE_TYPE_ICON_SRC},
      {""}, {""},
      {"cellspacing",                                    WEB_ATTRIBUTE_TYPE_COLSPAN},
      {"charset",                                        WEB_ATTRIBUTE_TYPE_CHARSET},
      {"readonly",                                       WEB_ATTRIBUTE_TYPE_READONLY},
      {"axis",                                           WEB_ATTRIBUTE_TYPE_AXIS},
      {"align",                                          WEB_ATTRIBUTE_TYPE_ALIGN},
      {"sys_conf_id",                                    WEB_ATTRIBUTE_TYPE_SYS_CONF_ID},
      {"seleted",                                        WEB_ATTRIBUTE_TYPE_SELETED},
      {"selected",                                       WEB_ATTRIBUTE_TYPE_SELECTED},
      {""},
      {"width",                                          WEB_ATTRIBUTE_TYPE_WIDTH},
      {""}, {""},
      {"optional",                                       WEB_ATTRIBUTE_TYPE_OPTIONAL},
      {"name",                                           WEB_ATTRIBUTE_TYPE_NAME},
      {""},
      {"accept",                                         WEB_ATTRIBUTE_TYPE_ACCEPT},
      {""},
      {"dir",                                            WEB_ATTRIBUTE_TYPE_DIR},
      {"accessory",                                      WEB_ATTRIBUTE_TYPE_ACCESSORY},
      {"label",                                          WEB_ATTRIBUTE_TYPE_LABEL},
      {"cellpadding",                                    WEB_ATTRIBUTE_TYPE_CELLPADDING},
      {"columns",                                        WEB_ATTRIBUTE_TYPE_COLUMNS},
      {"src",                                            WEB_ATTRIBUTE_TYPE_SRC},
      {"href",                                           WEB_ATTRIBUTE_TYPE_HREF},
      {"frame",                                          WEB_ATTRIBUTE_TYPE_FRAME},
      {"onpick",                                         WEB_ATTRIBUTE_TYPE_ONPICK},
      {""},
      {"hreflang",                                       WEB_ATTRIBUTE_TYPE_HREFLANG},
      {"accesskey",                                      WEB_ATTRIBUTE_TYPE_ACCESSKEY},
      {"value",                                          WEB_ATTRIBUTE_TYPE_VALUE},
      {"valign",                                         WEB_ATTRIBUTE_TYPE_VALIGN},
      {"checked",                                        WEB_ATTRIBUTE_TYPE_CHECKED},
      {""},
      {"accept-charset",                                 WEB_ATTRIBUTE_TYPE_ACCEPT_CHARSET},
      {""},
      {"hspace",                                         WEB_ATTRIBUTE_TYPE_HSPACE},
      {"headers",                                        WEB_ATTRIBUTE_TYPE_HEADERS},
      {"disabled",                                       WEB_ATTRIBUTE_TYPE_DISABLED},
      {""},
      {"newcontext",                                     WEB_ATTRIBUTE_TYPE_NEWCONTEXT},
      {"domain",                                         WEB_ATTRIBUTE_TYPE_DOMAIN},
      {"ordered",                                        WEB_ATTRIBUTE_TYPE_ORDERED},
      {"noselect",                                       WEB_ATTRIBUTE_TYPE_NOSELECT},
      {"path",                                           WEB_ATTRIBUTE_TYPE_PATH},
      {""},
      {"height",                                         WEB_ATTRIBUTE_TYPE_HEIGHT},
      {"ontimer",                                        WEB_ATTRIBUTE_TYPE_ONTIMER},
      {"longdesc",                                       WEB_ATTRIBUTE_TYPE_LONGDESC},
      {"rows",                                           WEB_ATTRIBUTE_TYPE_ROWS},
      {"xmlns",                                          WEB_ATTRIBUTE_TYPE_XMLNS},
      {"border",                                         WEB_ATTRIBUTE_TYPE_BORDER},
      {"rowspan",                                        WEB_ATTRIBUTE_TYPE_ROWSPAN},
      {"xml:lang",                                       WEB_ATTRIBUTE_TYPE_XML_LANG},
      {"maxlength",                                      WEB_ATTRIBUTE_TYPE_MAXLENGTH},
      {"style",                                          WEB_ATTRIBUTE_TYPE_STYLE},
      {"vspace",                                         WEB_ATTRIBUTE_TYPE_VSPACE},
      {"profile",                                        WEB_ATTRIBUTE_TYPE_PROFILE},
      {"for",                                            WEB_ATTRIBUTE_TYPE_FOR},
      {"xml:space",                                      WEB_ATTRIBUTE_TYPE_XML_SPACE},
      {"forua",                                          WEB_ATTRIBUTE_TYPE_FORUA},
      {"format",                                         WEB_ATTRIBUTE_TYPE_FORMAT},
      {"version",                                        WEB_ATTRIBUTE_TYPE_VERSION},
      {""},
      {"onenterforward",                                 WEB_ATTRIBUTE_TYPE_ONENTERFORWARD},
      {"onenterbackward",                                WEB_ATTRIBUTE_TYPE_ONENTERBACKWARD},
      {""}, {""}, {""}, {""},
      {"background",                                     WEB_ATTRIBUTE_TYPE_BACKGROUND},
      {"sendreferer",                                    WEB_ATTRIBUTE_TYPE_SENDREFERER},
      {""}, {""}, {""},
      {"backgroundcolor",                                WEB_ATTRIBUTE_TYPE_BGCOLOR},
      {""},
      {"img_src",                                        WEB_ATTRIBUTE_TYPE_IMG_SRC},
      {""}, {""}, {""}, {""},
      {"charoff",                                        WEB_ATTRIBUTE_TYPE_CHAROFF},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {"title",                                          WEB_ATTRIBUTE_TYPE_TITLE},
      {""}, {""}, {""}, {""},
      {"http-equiv",                                     WEB_ATTRIBUTE_TYPE_HTTP_EQUIV},
      {""},
      {"emptyok",                                        WEB_ATTRIBUTE_TYPE_EMPTYOK},
      {"cache-control",                                  WEB_ATTRIBUTE_TYPE_CACHE_CONTROL},
      {""}, {""}, {""}, {""}, {""},
      {"link_type",                                      WEB_ATTRIBUTE_TYPE_LINK_TYPE},
      {""}, {""},
      {"bgcolor",                                        WEB_ATTRIBUTE_TYPE_BGCOLOR},
      {"tabindex",                                       WEB_ATTRIBUTE_TYPE_TABLEINDEX},
      {"type",                                           WEB_ATTRIBUTE_TYPE_TYPE},
      {""}, {""}, {""},
      {"localsrc",                                       WEB_ATTRIBUTE_TYPE_LOCALSRC},
      {""}, {""},
      {"target",                                         WEB_ATTRIBUTE_TYPE_TARGET},
      {"summary",                                        WEB_ATTRIBUTE_TYPE_SUMMARY},
      {""}, {""}, {""}, {""}, {""},
      {"event_id",                                       WEB_ATTRIBUTE_TYPE_EVENT_ID},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {"enctype",                                        WEB_ATTRIBUTE_TYPE_ENCTYPE}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = getAttrHash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = wordlist[key].name;

          if ((((unsigned char)*str ^ (unsigned char)*s) & ~32) == 0 && !uc_strnicmp(str, s, len))
            return &wordlist[key];
        }
    }
  return 0;
}
