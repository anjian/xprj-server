
#ifndef __UC_XML_PARSER_TEST_DATA_H__
#define __UC_XML_PARSER_TEST_DATA_H__

const char historyData[] =
"<history> <item TITLE=\"title\" DATE=\"123\" HREF=\"href\" LINK_TYPE=\"web\" ICON_SRC=\"icon\" > </item> </history>";

const char preferenceData[] =
"<preference> <dir TITLE=\"title\"> <item TITLE=\"title\" HREF=\"href\" LINK_TYPE=\"wap\" ICON_SRC=\"icon\" > </item> </dir> </preference>";

const char sysconfmapData[] =
"<sys_conf_map> <group TITLE=\"title\"> <item ID=\"1\"> </item> </group> </sys_conf_map>";

const char sysinfoData[] =
"<sys_info> <info ID=\"123\" TITLE=\"title\" VALUE=\"true\" > <option NAME=\"name\" VALUE=\"value\" SELECTED=\"true\" > </option> </info> </sys_info>";

const char sysconfData[] = 
"<sys_conf> <item ID=\"1\" VALUE=\"value\" > </item> </sys_conf>";

const char tabbarData[] = 
"<ucf TYPE=\"tab\"> <ucf TYPE=\"tab_item\" NAME=\"name\" VALUE=\"value\" > </ucf> </ucf>";

const char menuData[] = 
"<menu> <group ID=\"123\"> <item TITLE=\"title\" ACCESSORY=\"#8\" EVENT_ID=\"1\" TYPE=\"check\" SUB_MENU_ID=\"1\" SYS_CONF_ID=\"1\" > </item> </group> </menu>";

const char searchhistoryData[] = 
"<searchhistory> <item VALUE=\"value\"> </item> </searchhistory>";

const char urlhistoryData[] = 
"<urlhistory> <item VALUE=\"value\"> </item> </urlhistory>";

#endif

