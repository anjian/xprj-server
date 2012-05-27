#ifndef __APP_EVENT_H_
#define __APP_EVENT_H_

//#include <platform/inc/ucweb_os_type.h>

class AppEventDispatchIf_i;

//
// Please NOTE that the event sequence is important, it can effect the event
// handler defintion obsEventHandler_m[] in dispatcher[]
//
// In case there is event type added/removed, please DO adjust obsEventHandler_m
// at the same time!!!
enum AppEventType_e
{
    EVENT_APP_START =0,

    // SYS
    EVENT_SYSTEM_INIT,

    // IO
    EVENT_IO_POSTPONED_DELETE,

    // User Service event
    EVENT_US_SG_CHECK,                              // check service group info

#if 0
    EVENT_WEB_DATA_UPDATE = EVENT_APP_START,
    EVENT_WEB_DATA_COMPLETED,
    EVENT_WEB_DATA_RELOADED,
    EVENT_WEB_DATA_RECEIVED,
    EVENT_WEB_DATA_REFRESH,                         
    EVENT_WEB_DATA_REFRESH_WAP10,                   // wap 1.x refresh (default with setvar)
    EVENT_WEB_DATA_PREV,                            // wap 1.x prev without setvar
    EVENT_WEB_DATA_CHANGED_PREV,                    // wap 1.x prev with setvar
    EVENT_WEB_DATA_REFRESH_DATA_RECEIVED,           // for web page refresh, when data received at the first time.
    EVENT_REFRESH,                                  // refresh (re access uri)
    EVENT_OPEN_HOME_PAGE,                           // open home page

    /*-.-.-.-.-.-.-.-.-.The following for download data-.-.-.-.-.--.-.-.-.-.-.-.-.-.-.*/
    EVENT_WEB_DOWNLOAD_TASK_RENAME,
    EVENT_WEB_DOWNLOAD_ACTION,
    EVENT_WEB_DOWNLOAD_COMPLETED,
    EVENT_WEB_DOWNLOAD_RECEIVED,
    EVENT_WEB_DOWNLOAD_FAILED,
    /*-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/

    EVENT_IO_ASSIGNED,
    EVENT_IO_STATE_UPDATE,
    EVENT_IO_DATA_SENT,
    EVENT_IO_DATA_RECEIVED,
    EVENT_IO_CLOSED,
    EVENT_IO_ERROR,
    EVENT_IO_CHECK,

    EVENT_CDDATA_UPDATE,
    
    EVENT_SYSTEM_INFO_IO,               //drive system mgr to load or save

    EVENT_IO_CONTROLLER_CLEAN,          // clean idle io controller

    // SLO event begin
    EVENT_SLO_UPLOAD_FONT,
    EVENT_SLO_UPLOAD_FONT_FINISH,
    // SLO event end
    // controller internal scheduling
    EVENT_CONTROLLER_STATE_SYS_CONF_COMPLETED,
    EVENT_CONTROLLER_STATE_DISPATCHER_COMPLETED,
    EVENT_CONTROLLER_STATE_CDDATA_COMPLETED,    
    EVENT_CONTROLLER_STATE_STARTUP_COMPLETED,
    EVENT_CONTROLLER_OPEN_LINK,

    /*-.-.-.-.-.-.-.-.-.The following for upload data-.-.-.-.-.-.--.-.-.-.-.-.-.-.-.-.*/
    EVENT_WEB_UPLOAD_COMPLETED,
    EVENT_WEB_UPLOAD_SEND,
    EVENT_WEB_UPLOAD_FAILED,
    /*-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
#endif

    // NOTE: Please add new type above this line!!!
    EVENT_APP_END,
};

// Event definition
struct AppEvent_t
{
    AppEvent_t() : 
        eEventType_m(EVENT_APP_START)
    {

    }
    // event header
    AppEventType_e eEventType_m;

#if 0
    UC_INT32 nIoControllerId_m;     // IO event

    // data
    union
    {
        UC_INT32 nContainerId_m;        // Window addition, deletion and change
        UC_INT32 nWebDataId_m;          // WEB_DATA_UPDATE

        void* pIoTrans_m;               // IO Assignment -> UcIoTransaction_c*
        UC_INT32 nIoContentLen_m;       // IO Received
        UC_CHAR*  sUcServerUri_m;   //The Us server or the dispatcher server.
        UC_INT nErrorCode_m; // general event's error code
    };

    const void*         pData_m;
    UC_INT32            nValidId_m;     // valid id, use to check the id is valid
#endif
};

////////////////////////////////////////////////////////////////////////////////
// app event send interface
////////////////////////////////////////////////////////////////////////////////
bool APP_EVENT_QUEUE_START();
void APP_EVENT_SENT(AppEvent_t& event);
void APP_EVENT_SET_DISPATCHER(AppEventDispatchIf_i* dispatcher);

#endif

