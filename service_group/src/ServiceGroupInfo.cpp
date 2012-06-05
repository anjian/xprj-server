
// system library
#include <sqlite3.h>

#include <common/inc/MyStdLibrary.h>
#include <common/inc/MyDebug.h>

#include <interface/baseos/MemoryIf.h>
//#include <interface/baseos/AppEvent.h>
#include <interface/baseos/TempMemBuffer.h>
#include <interface/system/GlobalInstanceMgr.h>
#include <interface/system/SystemInfoIf.h>
//#include <interface/io/IoTransIf.h>
#include <interface/data_proxy/DataProxyIf.h>

#include <interface/protocol/TlvIf.h>
#include <interface/protocol/tlv.h>

#include <interface/service_group/ServiceGroupIf.h>

#include <service_group/inc/ServiceGroupInfo.h>

////////////////////////////////////////////////////////////////////////////////
// ServiceGroupIf
////////////////////////////////////////////////////////////////////////////////
//
void SERVICE_GROUP_INFO_SET_MASTER()
{
    ServiceGroupMgr_c::getInstance()->setMode(true);
}

void SERVICE_GROUP_INFO_SET_SLAVE()
{
    ServiceGroupMgr_c::getInstance()->setMode(false);
}

void SERVICE_GROUP_INFO_ADD(int nId)
{
    ServiceGroupMgr_c::getInstance()->addServiceGroup(nId);
}

bool SERVICE_GROUP_INFO_UPDATE(TlvAttrIf_i* pAttr)
{
    return ServiceGroupMgr_c::getInstance()->handleServiceGroupUpdate(pAttr);
}

bool SERVICE_GROUP_INFO_QUERY(int nId, const char*& sHost, const char*& sPort)
{
    ServiceGroup_c* pSg = ServiceGroupMgr_c::getInstance()->getServiceGroup(nId);
    if (NULL == pSg)
    {
        return false;
    }

    sHost = pSg->getHost();
    sPort = pSg->getPort();

    return true;
}

// For master service group
void SERVICE_GROUP_INFO_LOAD()
{
    ServiceGroupMgr_c::getInstance()->load();
}

void SERVICE_GROUP_INFO_SAVE()
{
    ServiceGroupMgr_c::getInstance()->save();
}


////////////////////////////////////////////////////////////////////////////////
// ServiceGroup_c implementation
////////////////////////////////////////////////////////////////////////////////
ServiceGroup_c::~ServiceGroup_c()
{
    if (NULL != sHost_m)
    {
        xfree(sHost_m);
    }

    if (NULL != sPort_m)
    {
        xfree(sPort_m);
    }
}

void ServiceGroup_c::setHost(const char* sHost)
{
    if (NULL == sHost)
    {
        return;
    }

    if (NULL != sHost_m)
    {
        xfree(sHost_m);
    }

    int nLen = xstrlen(sHost);
    sHost_m = (char*)xmalloc(nLen + 1);
    if (NULL == sHost_m)
    {
        return;
    }

    xmemcpy(sHost_m, sHost, nLen);
    sHost_m[nLen] = 0;
}

void ServiceGroup_c::setPort(const char* sPort)
{
    if (NULL == sPort)
    {
        return;
    }

    if (NULL != sPort_m)
    {
        xfree(sPort_m);
    }

    int nLen = xstrlen(sPort);
    sPort_m = (char*)xmalloc(nLen + 1);
    if (NULL == sPort_m)
    {
        return;
    }

    xmemcpy(sPort_m, sPort, nLen);
    sPort_m[nLen] = 0;
}

////////////////////////////////////////////////////////////////////////////////
// ServiceGroupMgr_c implementation
////////////////////////////////////////////////////////////////////////////////
ServiceGroupMgr_c::ServiceGroupMgr_c() :
    htSGInfo_m(),
    lstPendingGroup_m(NULL),
    bMasterMode_m(false)
{
}

ServiceGroupMgr_c::~ServiceGroupMgr_c()
{
    if (NULL != lstPendingGroup_m)
    {
        delete lstPendingGroup_m;
    }
}

IMPLE_OBJECT_GET_INSTANCE(ServiceGroupMgr_c)

void ServiceGroupMgr_c::addServiceGroup(int nId)
{
    // 1. check if given ID has been handled
    if (NULL != htSGInfo_m.lookup(nId))
    {
        return;
    }

    // 2. check if given id is in pending list
    if (NULL != lstPendingGroup_m)
    {
        for (int nIndex=lstPendingGroup_m->size()-1; nIndex>=0; nIndex--)
        {
            if (lstPendingGroup_m->get(nIndex) == nId)
            {
                return;
            }
        }
    }

    // 3. add into pending list, and retrieve related info late (don't depend on dispatcher service status)
    if (NULL == lstPendingGroup_m)
    {
        lstPendingGroup_m = new TempSimpleVector_c<int>(8);
        if (NULL == lstPendingGroup_m)
        {
            return;
        }
    }

    lstPendingGroup_m->append(nId);

    // 4. send APP_EVENT to check service group info
    sendSGCheckEvent(nId);
}

bool ServiceGroupMgr_c::insertGroup(ServiceGroup_c* pSG)
{
    if ((NULL == pSG) || (NULL != htSGInfo_m.lookup(pSG->getId())))
    {
        return false;
    }

    htSGInfo_m.insert(pSG->getIdAddress(), pSG);
    return true;
}

bool ServiceGroupMgr_c::updateServiceGroup(int nId, const char* sHost, const char* sPort)
{
    if (nId < 0)
    {
        return false;
    }

    ServiceGroup_c* pGroup = htSGInfo_m.lookup(nId);
    if (NULL == pGroup)
    {
        // create new group
        pGroup = new ServiceGroup_c;
        if (NULL == pGroup)
        {
            return false;
        }

        pGroup->setId(nId);
        htSGInfo_m.insert(pGroup->getIdAddress(), pGroup);
    }

    pGroup->setHost(sHost);
    pGroup->setPort(sPort);

    return true;
}

bool ServiceGroupMgr_c::load()
{
    if (!bMasterMode_m)
    {
        return false;
    }

    const char* sDB = SYS_INFO_GET_STRING(SYS_INFO_TYPE_SERVICE_DATABASE);
    if (NULL == sDB)
    {
        MSG_ERR("User Info DB is not configured\n");
        return false;
    }

    sqlite3 *pConn;
    if (sqlite3_open(sDB, &pConn))
    {
        MSG_ERR("Can not open database [%d]\n", sDB);
        return false;
    }

    bool bRet = loadSgInfo(pConn);
    if (!bRet)
    {
        MSG_ERR("load sg info error\n");
    }

    sqlite3_close(pConn);

    return bRet;
}

bool ServiceGroupMgr_c::save()
{
    if (!bMasterMode_m)
    {
        return false;
    }

    const char* sDB = SYS_INFO_GET_STRING(SYS_INFO_TYPE_SERVICE_DATABASE);
    if (NULL == sDB)
    {
        MSG_ERR("User Info DB is not configured\n");
        return false;
    }

    sqlite3 *pConn;
    if (sqlite3_open(sDB, &pConn))
    {
        MSG_ERR("Can not open database [%d]\n", sDB);
        return false;
    }

    bool bRet = saveSgInfo(pConn);
    if (bRet)
    {
        MSG_ERR("save sg info error\n");
    }

    sqlite3_close(pConn);

    return bRet;
}

bool ServiceGroupMgr_c::loadSgInfo(sqlite3 *pConn)
{
    const char* sSgQuery = "select id, host, port, type from sginfo";
    //sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &sErrMsg);
    //sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &sErrMsg);
    //
    //sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, &sErrMsg);
    //sqlite3_exec(db, "PRAGMA journal_mode = MEMORY", NULL, NULL, &sErrMsg);
    //  PRAGMA foreign_keys = ON

    sqlite3_stmt *res;
    const char *tail;

    if (SQLITE_OK != sqlite3_prepare_v2(pConn,
                sSgQuery,
                100,
                &res,
                &tail))
    {
        MSG_ERR("Could not get data from DB\n");
        return false;
    }

    //int rec_count = 0;
    while (SQLITE_ROW == sqlite3_step(res))
    {
        // 1st user
        ServiceGroup_c* pGroup = new ServiceGroup_c;
        if (NULL != pGroup)
        {
            pGroup->setId(sqlite3_column_int(res, 0));
            pGroup->setHost((const char*)sqlite3_column_text(res, 1));
            pGroup->setPort((const char*)sqlite3_column_text(res, 2));
            //pGroup->setType(sqlite3_column_int(res, 3));

            if (!insertGroup(pGroup))
            {
                delete pGroup;
            }
        }
#if 0
        printf("%s|", sqlite3_column_text(res, 0));
        printf("%s|", sqlite3_column_text(res, 1));
        printf("%s|", sqlite3_column_text(res, 2));
        printf("%u\n", sqlite3_column_int(res, 3));

        //rec_count++;
#endif
    }

    sqlite3_finalize(res);

    return true;
}

bool ServiceGroupMgr_c::saveSgInfo(sqlite3 *pConn)
{
    if ((NULL == lstPendingGroup_m) || (0 >= lstPendingGroup_m->size()))
    {
        return true;
    }

    const char* sSgInsert = "insert into sginfo values (@id, @host, @port, @type)";
    //sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &sErrMsg);
    //sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &sErrMsg);
    //
    //sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, &sErrMsg);
    //sqlite3_exec(db, "PRAGMA journal_mode = MEMORY", NULL, NULL, &sErrMsg);
    //  PRAGMA foreign_keys = ON

    sqlite3_stmt *res;
    const char *tail;

    if (SQLITE_OK != sqlite3_prepare_v2(pConn,
                sSgInsert,
                1000,
                &res,
                &tail))
    {
        MSG_ERR("Could not get data from DB\n");
        return false;
    }

    char * sErrMsg = NULL;
    sqlite3_exec(pConn, "BEGIN TRANSACTION", NULL, NULL, &sErrMsg);
    //int rec_count = 0;
    for (int nIndex=lstPendingGroup_m->size()-1; nIndex>=0; nIndex--)
    {
        ServiceGroup_c* pSg = getServiceGroup(lstPendingGroup_m->get(nIndex));
        if (NULL == pSg)
        {
            continue;
        }

        sqlite3_bind_int(res, 1, pSg->getId());
        sqlite3_bind_text(res, 2, pSg->getHost(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(res, 3, pSg->getPort(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(res, 4, 0);

        sqlite3_step(res);

        sqlite3_clear_bindings(res);
        sqlite3_reset(res);
    }

    sqlite3_exec(pConn, "END TRANSACTION", NULL, NULL, &sErrMsg);
    sqlite3_finalize(res);

    lstPendingGroup_m->clear();

    return true;
}

#if 0
void ServiceGroupMgr_c::resolveServiceGroup()
{
    if (NULL == lstPendingGroup_m) // || (0 >= lstPendingGroup_m->size()))
    {
        return;
    }

    if (0 < lstPendingGroup_m->size())
    {
        // start io transaction
        pIoTrans_m = IO_TRANSACTION_ALLOC(this, "127.0.0.1", "8081"); 

        if (NULL == pIoTrans_m)
        {
            MSG("Can not open IO transaction!!!\n");

            // TODO: how to handle error
        }
    }
    else
    {
        delete lstPendingGroup_m;
        lstPendingGroup_m = NULL;
    }
}
#endif

#if 0
bool ServiceGroupMgr_c::isUnderQuerying()
{
    return (NULL != pIoTrans_m);
}
#endif

bool ServiceGroupMgr_c::sendSGCheckEvent(int nSgId)
{
    TlvAttrIf_i* tlvSG = buildServiceGroupQuery(nSgId);
    if (NULL == tlvSG)
    {
        MSG_ERR("build service group query error\n");
        return false;
    }

    // encode message content
    TempMemBuffer_c tbBuffer;
    TlvEncoder_c tlvEncoder;
    tlvEncoder.generate(tlvSG, tbBuffer);

    delete tlvSG;

    MSG("insert app event\n");
    // TODO: remove magic number
    SEND_NET_REQUEST(-1, tbBuffer.getBuffer(), tbBuffer.getLength());

    return true;
}

#if 0
void ServiceGroupMgr_c::releaseIoTrans()
{
    if (NULL == pIoTrans_m)
    {
        return;
    }

    IO_TRANSACTION_RELEASE(pIoTrans_m);
    pIoTrans_m = NULL;
}
#endif

#if 0
////////////////////////////////////////////////////////////////////////////////
// interface IoDataIf_i
////////////////////////////////////////////////////////////////////////////////
void ServiceGroupMgr_c::generateRequest(TempMemBuffer_c& tbBuffer)
{
    if (NULL == lstPendingGroup_m)
    {
        return;
    }

    TempSimpleVector_c<TlvAttrIf_i*> lstAttrs(8);

    // TODO: should have a frame work to generate TLV attributes
    //
    // append protocol version
    TlvAttrIf_i* tlvTest = createTlvAttribute(TLV_ATTR_CLIENT_VERSION);
    tlvTest->setValue_int32(0x00010000);
    lstAttrs.append(tlvTest);

    // append service group info
    for (int nIndex=lstPendingGroup_m->size()-1; nIndex>=0; nIndex--)
    {
        // TODO:
        //  1. threshold of service group item in one message
        //  2. manager service groups which are under querying 

        // build message to SG Center
        TlvAttrIf_i* tlvSG = buildServiceGroupQuery(lstPendingGroup_m->get(nIndex));
        lstPendingGroup_m->takeLast();

        if (NULL != tlvSG)
        {
            lstAttrs.append(tlvSG);
        }
    }

    // append tail flag
    TlvAttrIf_i* tlvTail = createTlvAttribute(TLV_ATTR_TAIL_FLAG);
    tlvTail->setValue_int32(0);
    lstAttrs.append(tlvTail);

    // encode message content
    TlvEncoder_c tlvEncoder;
    tlvEncoder.generate(&lstAttrs, tbBuffer);

    // release tlv attributes
    for (int nIndex=lstAttrs.size()-1; nIndex>=0; nIndex--)
    {
        delete lstAttrs.takeLast();
    }
}

void ServiceGroupMgr_c::analyzeResponse(TempMemBuffer_c& tbBuffer)
{
    //tbBuffer.cutAsString();
    MSG("RECEIVED reponse, to analyze...\n");
    TempSimpleVector_c<TlvAttrIf_i*> lstResponseAttrs(8);

    TlvParser_c parser(&lstResponseAttrs);
    parser.parse(tbBuffer.getBuffer(), tbBuffer.getLength());

    MSG("returned %d attributes\n", lstResponseAttrs.size());

    processSGResponse(&lstResponseAttrs);

    MSG("release io trans...\n");
    releaseIoTrans();

    //to check if need to start new round checking
    if (NULL != lstPendingGroup_m) // && (lstPendingGroup_m->size() > 0))
    {
        sendSGCheckEvent();
    }
}
#endif

TlvAttrIf_i* ServiceGroupMgr_c::buildServiceGroupQuery(int nSGId)
{
    TlvAttrIf_i* tlvSGQuery = createTlvAttribute(TLV_ATTR_SERVICE_GROUP_QUERY);
    if (NULL == tlvSGQuery)
    {
        return NULL;
    }

    TlvAttrIf_i* tlvSGId = createTlvAttribute(TLV_ATTR_SERVICE_GROUP_ID);
    if (NULL == tlvSGId)
    {
        delete tlvSGQuery;
        return NULL;
    }
    tlvSGId->setValue_int32(nSGId);

    tlvSGQuery->appendAttr(tlvSGId);

    return tlvSGQuery;
}

#if 0
bool ServiceGroupMgr_c::processSGResponse(TempSimpleVector_c<TlvAttrIf_i*>* tlvAttrs)
{
    // handle content
    for (int nIndex=1; nIndex<tlvAttrs->size(); nIndex++)
    {
        TlvAttrIf_i* pAttr = tlvAttrs->get(nIndex);
        switch (pAttr->getType())
        {
            case TLV_ATTR_SERVICE_GROUP_UPDATE:
                {
                    handleServiceGroupUpdate(pAttr);
                }
                break;
            case TLV_ATTR_CLIENT_VERSION:
            case TLV_ATTR_TAIL_FLAG:
                {
                    // TODO: error case
                }
                break;
            default:;
        }
    }

    return true;
}
#endif

bool ServiceGroupMgr_c::handleServiceGroupUpdate(TlvAttrIf_i* pAttr)
{
    if ((NULL == pAttr) || (TLV_ATTR_SERVICE_GROUP_UPDATE != pAttr->getType()))
    {
        return false;
    }

    TlvAttrIterator_i* pIterator = pAttr->getIterator();
    if (NULL == pIterator)
    {
        return false;
    }

    // get service group
    {
        int nSgId = -1;
        const char* sHost = NULL;
        const char* sPort = NULL;

        TlvAttrIf_i* pItem = NULL;
        for (;(pItem = pIterator->next());)
        {
            switch (pItem->getType())
            {
                case TLV_ATTR_SERVICE_GROUP_ID:
                    {
                        nSgId = pItem->getInt32();
                        break;
                    }
                case TLV_ATTR_SERVICE_GROUP_HOST:
                    {
                        pItem->cutAsString();
                        sHost = pItem->getString();
                        break;
                    }
                case TLV_ATTR_SERVICE_GROUP_PORT:
                    {
                        pItem->cutAsString();
                        sPort = pItem->getString();
                        break;
                    }
                default:;
            }
        }
        delete pIterator;

        // find service group id
        if ((-1 != nSgId) && (NULL != sHost) && (NULL != sPort))
        {
            MSG("Got Service Group Info: %d, %s %s\n", nSgId, sHost, sPort);

            updateServiceGroup(nSgId, sHost, sPort);
        }

        // remove sg id from pending list
    }

    return true;
}
