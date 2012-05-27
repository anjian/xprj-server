/* C code produced by gperf version 3.0.1 */
/* Command-line: gperf --struct-type --ignore-case -N elementLook -H GetElementHash ElementList.input  */
/* Computed positions: -k'1-2,$' */

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

1. gperf  --struct-type --ignore-case -N elementLook -H GetElementHash ElementList.input > elementListAUTO.h

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

Recommend to add a prefix to it.

 */
//struct Wap2Keyword {const UC_BYTE * name;UC_INT token;};

#define TOTAL_KEYWORDS_ELEMENT 99
#define MIN_WORD_LENGTH_ELEMENT 1
#define MAX_WORD_LENGTH_ELEMENT 13
#define MIN_HASH_VALUE_ELEMENT 1
#define MAX_HASH_VALUE_ELEMENT 204
/* maximum key range = 204, duplicates = 0 */


static unsigned int
GetElementHash (register const char *str, register unsigned int len)
{
  static unsigned char asso_values[] =
    {
      205, 205, 205, 205, 205, 205, 205, 205, 205, 205,
      205, 205, 205, 205, 205, 205, 205, 205, 205, 205,
      205, 205, 205, 205, 205, 205, 205, 205, 205, 205,
      205, 205, 205, 205, 205, 205, 205, 205, 205, 205,
      205, 205, 205, 205, 205, 205, 205, 205, 205,  30,
       40,  20,  25,  15,  15,  10, 205, 205, 205, 205,
      205, 205, 205, 205, 205,  30,   0,  75,   0,   0,
       15,  65,  70,  60,  15,  65,  30,  25,  40,  70,
       20,  80,  75,   0,   5,  15,  45,  15, 205,  85,
       55, 205, 205, 205, 205, 205, 205,  30,   0,  75,
        0,   0,  15,  65,  70,  60,  15,  65,  30,  25,
       40,  70,  20,  80,  75,   0,   5,  15,  45,  15,
      205,  85,  55, 205, 205, 205, 205, 205, 205, 205,
      205, 205, 205, 205, 205, 205, 205, 205, 205, 205,
      205, 205, 205, 205, 205, 205, 205, 205, 205, 205,
      205, 205, 205, 205, 205, 205, 205, 205, 205, 205,
      205, 205, 205, 205, 205, 205, 205, 205, 205, 205,
      205, 205, 205, 205, 205, 205, 205, 205, 205, 205,
      205, 205, 205, 205, 205, 205, 205, 205, 205, 205,
      205, 205, 205, 205, 205, 205, 205, 205, 205, 205,
      205, 205, 205, 205, 205, 205, 205, 205, 205, 205,
      205, 205, 205, 205, 205, 205, 205, 205, 205, 205,
      205, 205, 205, 205, 205, 205, 205, 205, 205, 205,
      205, 205, 205, 205, 205, 205, 205, 205, 205, 205,
      205, 205, 205, 205, 205, 205, 205, 205, 205, 205,
      205, 205, 205, 205, 205, 205, 205
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[1]+1];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

#ifdef __GNUC__
__inline
#endif
struct Wap2Keyword *
elementLook (register const char *str, register unsigned int len)
{
  static struct Wap2Keyword wordlist[] =
    {
      {""},
      {"b",             WEB_TAG_TYPE_B},
      {"dd",            WEB_TAG_TYPE_DD},
      {""},
      {"base",          WEB_TAG_TYPE_BASE},
      {""}, {""},
      {"td",            WEB_TAG_TYPE_TD},
      {""}, {""},
      {"table",         WEB_TAG_TYPE_TABLE},
      {"script",        WEB_TAG_TYPE_SCRIPT},
      {""}, {""}, {""}, {""},
      {"target",        WEB_TAG_TYPE_TARGET},
      {""}, {""}, {""},
      {"style",         WEB_TAG_TYPE_STYLE},
      {"strike",        WEB_TAG_TYPE_STRIKE},
      {"dt",            WEB_TAG_TYPE_DT},
      {"pre",           WEB_TAG_TYPE_PRE},
      {"samp",          WEB_TAG_TYPE_SAMP},
      {"title",         WEB_TAG_TYPE_TITLE},
      {"select",        WEB_TAG_TYPE_SELECT},
      {"tt",            WEB_TAG_TYPE_TT},
      {"template",      WEB_TAG_TYPE_TEMPLATE},
      {""},
      {"preference",    WEB_TAG_TYPE_PREFERENCE},
      {"u",             WEB_TAG_TYPE_U},
      {""},
      {"ucf",           WEB_TAG_TYPE_UCF},
      {""},
      {"blockquote",    WEB_TAG_TYPE_BLOCKQUOTE},
      {"access",        WEB_TAG_TYPE_ACCESS},
      {"address",       WEB_TAG_TYPE_ADDRESS},
      {""}, {""}, {""},
      {"p",             WEB_TAG_TYPE_P},
      {""},
      {"fieldset",      WEB_TAG_TYPE_FIELDSET},
      {"font",          WEB_TAG_TYPE_FONT},
      {""}, {""}, {""},
      {"sub",           WEB_TAG_TYPE_SUB},
      {"postfield",     WEB_TAG_TYPE_POSTFIELD},
      {"param",         WEB_TAG_TYPE_PARAM},
      {"legend",        WEB_TAG_TYPE_LEGEND},
      {""}, {""}, {""}, {""}, {""},
      {"dl",            WEB_TAG_TYPE_DL},
      {"textarea",      WEB_TAG_TYPE_TEXTAREA},
      {"menu",          WEB_TAG_TYPE_MENU},
      {""},
      {"a",             WEB_TAG_TYPE_A},
      {"acronym",       WEB_TAG_TYPE_ACRONYM},
      {"div",           WEB_TAG_TYPE_DIV},
      {"form",          WEB_TAG_TYPE_FORM},
      {"label",         WEB_TAG_TYPE_LABEL},
      {""},
      {"em",            WEB_TAG_TYPE_EM},
      {"sup",           WEB_TAG_TYPE_SUP},
      {"prev",          WEB_TAG_TYPE_PREV},
      {""}, {""},
      {"ul",            WEB_TAG_TYPE_UL},
      {"noscript",      WEB_TAG_TYPE_NOSCRIPT},
      {"meta",          WEB_TAG_TYPE_META},
      {"small",         WEB_TAG_TYPE_SMALL},
      {""},
      {"br",            WEB_TAG_TYPE_BR},
      {"sys_conf",      WEB_TAG_TYPE_SYS_CONF},
      {"card",          WEB_TAG_TYPE_CARD},
      {""},
      {"assign",        WEB_TAG_TYPE_ASSIGN},
      {"tr",            WEB_TAG_TYPE_TR},
      {"big",           WEB_TAG_TYPE_BIG},
      {"noop",          WEB_TAG_TYPE_NOOP},
      {""},
      {"strong",        WEB_TAG_TYPE_STRONG},
      {"sys_conf_map",  WEB_TAG_TYPE_SYS_CONF_MAP},
      {"wml",           WEB_TAG_TYPE_WML},
      {"head",          WEB_TAG_TYPE_HEAD},
      {"group",         WEB_TAG_TYPE_GROUP},
      {"button",        WEB_TAG_TYPE_BUTTON},
      {"do",            WEB_TAG_TYPE_DO},
      {"dir",           WEB_TAG_TYPE_DIR},
      {"cite",          WEB_TAG_TYPE_CITE},
      {""},
      {"setvar",        WEB_TAG_TYPE_SETVAR},
      {"h6",            WEB_TAG_TYPE_H6},
      {""},
      {"code",          WEB_TAG_TYPE_CODE},
      {"timer",         WEB_TAG_TYPE_TIMER},
      {""},
      {"h5",            WEB_TAG_TYPE_H5},
      {""},
      {"item",          WEB_TAG_TYPE_ITEM},
      {""}, {""},
      {"li",            WEB_TAG_TYPE_LI},
      {"dfn",           WEB_TAG_TYPE_DFN},
      {"body",          WEB_TAG_TYPE_BODY},
      {"urlhistory",    WEB_TAG_TYPE_URLHISTORY},
      {""},
      {"h4",            WEB_TAG_TYPE_H4},
      {"searchhistory", WEB_TAG_TYPE_SEARCHHISTORY},
      {"link",          WEB_TAG_TYPE_LINK},
      {""}, {""},
      {"h3",            WEB_TAG_TYPE_H3},
      {""},
      {"html",          WEB_TAG_TYPE_HTML},
      {""},
      {"i",             WEB_TAG_TYPE_I},
      {"caption",       WEB_TAG_TYPE_CAPTION},
      {"var",           WEB_TAG_TYPE_VAR},
      {"span",          WEB_TAG_TYPE_SPAN},
      {""}, {""},
      {"ol",            WEB_TAG_TYPE_OL},
      {""}, {""}, {""}, {""},
      {"h2",            WEB_TAG_TYPE_H2},
      {"sys_info",      WEB_TAG_TYPE_SYS_INFO},
      {""}, {""}, {""},
      {"th",            WEB_TAG_TYPE_TH},
      {""}, {""},
      {"input",         WEB_TAG_TYPE_INPUT},
      {""},
      {"h1",            WEB_TAG_TYPE_H1},
      {"kbd",           WEB_TAG_TYPE_KBD},
      {""}, {""}, {""},
      {"hr",            WEB_TAG_TYPE_HR},
      {""}, {""}, {""}, {""},
      {"onevent",       WEB_TAG_TYPE_ONEVENT},
      {""}, {""}, {""},
      {"object",        WEB_TAG_TYPE_OBJECT},
      {"go",            WEB_TAG_TYPE_GO},
      {""}, {""}, {""},
      {"q",             WEB_TAG_TYPE_Q},
      {""}, {""}, {""}, {""}, {""},
      {"refresh",       WEB_TAG_TYPE_REFRESH},
      {"img",           WEB_TAG_TYPE_IMG},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {"history",       WEB_TAG_TYPE_HISTORY},
      {"optgroup",      WEB_TAG_TYPE_OPTGROUP},
      {""}, {""},
      {"anchor",        WEB_TAG_TYPE_ANCHOR},
      {""}, {""},
      {"abbr",          WEB_TAG_TYPE_ABBR},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
      {"option",        WEB_TAG_TYPE_OPTION},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {"info",          WEB_TAG_TYPE_INFO},
      {"imagecaches", WEB_TAG_TYPE_IMAGECACHE}
    };

  if (len <= MAX_WORD_LENGTH_ELEMENT && len >= MIN_WORD_LENGTH_ELEMENT)
    {
      register int key = GetElementHash (str, len);

      if (key <= MAX_HASH_VALUE_ELEMENT && key >= 0)
        {
          register const char *s = wordlist[key].name;

          if ((((unsigned char)*str ^ (unsigned char)*s) & ~32) == 0 && !uc_strnicmp (str, s, len))
            return &wordlist[key];
        }
    }
  return 0;
}
