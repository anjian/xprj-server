#ifndef __SERVICE_GROUP_IF_H_
#define __SERVICE_GROUP_IF_H_

class TlvAttrIf_i;

void SERVICE_GROUP_INFO_SET_MASTER();        // dispatcher owns master service group info
void SERVICE_GROUP_INFO_SET_SLAVE();         // all others are slave

void SERVICE_GROUP_INFO_ADD(int nId);
bool SERVICE_GROUP_INFO_UPDATE(TlvAttrIf_i* pAttr);
bool SERVICE_GROUP_INFO_QUERY(int nId, const char*& sHost, const char*& sPort);

// For master service group
void SERVICE_GROUP_INFO_LOAD();
void SERVICE_GROUP_INFO_SAVE();

#endif
