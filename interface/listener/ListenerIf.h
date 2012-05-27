#ifndef __LISTENER_IF_H_
#define __LISTENER_IF_H_

#include <common/inc/SimpleVector.h>

class TempMemBuffer_c;
class IoTransIf_i;

// listener
//typedef bool (*data_handler_callback)(int nFd, TempMemBuffer_c& dataReceived, TempMemBuffer_c& dataToSend);
//void IO_SOCKET_SET_DATA_HANDLER(data_handler_callback func);

class TlvAttrIf_i;
class RequestItemIf_i;

typedef bool (*request_handler_callback)(RequestItemIf_i* pReqItem, TempSimpleVector_c<TlvAttrIf_i*>* lstRequest);
void SET_SERVER_REQUEST_HANDLER(request_handler_callback func);

int  IO_SOCKET_START_LISTENER(const char* sPort);

// IoTransMgr
int  IO_SOCKET_CREATE(const char* sHost, const char* sPort);
int  IO_SOCKET_WRITE(int nFd, TempMemBuffer_c& tbSend);
void IO_SOCKET_CLOSE(int nFd);
bool IO_SOCKET_TRANS_MANAGEABLE(int nFd);
IoTransIf_i* IO_SOCKET_GET_TRANS(int nFd);

#endif
