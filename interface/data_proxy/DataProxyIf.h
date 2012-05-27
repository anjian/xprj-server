#ifndef __DATA_PROXY_IF_H_
#define __DATA_PROXY_IF_H_

#include <common/inc/SimpleVector.h>

//class IoTransIf_i;
class TlvAttrIf_i;

// data proxy
typedef bool (*response_handler_callback)(TempSimpleVector_c<TlvAttrIf_i*>* lstResponse);

void SET_CLIENT_RESPONSE_HANDLER(response_handler_callback func);
void SEND_NET_REQUEST(int nSgId, const char* pBuf, int nLen);

#endif
