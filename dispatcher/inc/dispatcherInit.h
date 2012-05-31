#ifndef __DISPATCHER_INIT_H_
#define __DISPATCHER_INIT_H_

struct sqlite3;

class DispatcherInit_c
{
    public:
        ~DispatcherInit_c();
        static DispatcherInit_c* getInstance();

        bool initialize();

    protected:
        DispatcherInit_c();

        bool loadDataFromDB();
        bool loadUserInfo(sqlite3 *pConn);
        bool loadUserAccount(sqlite3 *pConn);
        bool loadSgInfo(sqlite3 *pConn);

    private:
};

#endif
