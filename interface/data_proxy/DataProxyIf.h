#ifndef __DATA_PROXY_IF_H_
#define __DATA_PROXY_IF_H_

#include <common/inc/SimpleVector.h>

//class IoTransIf_i;
class TlvAttrIf_i;
struct sqlite3;

// data proxy
typedef bool (*response_handler_callback)(TempSimpleVector_c<TlvAttrIf_i*>* lstResponse);

void SET_CLIENT_RESPONSE_HANDLER(response_handler_callback func);
void SEND_NET_REQUEST(int nSgId, const char* pBuf, int nLen);

// database proxy
bool OPEN_DATABASE(const char* sDb);
sqlite3 *GET_DB_CONNECTION();
bool CLOSE_DATABASE();

#endif
