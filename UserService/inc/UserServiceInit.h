#ifndef __USER_SERVICE_INIT_H_
#define __USER_SERVICE_INIT_H_

struct sqlite3;

class UserServiceInit_c
{
    public:
        ~UserServiceInit_c();

        static UserServiceInit_c* getInstance();

        bool initialize();

    protected:
        UserServiceInit_c();

        bool loadUserData();
        bool loadUserInfo(sqlite3 *pConn);
        bool loadUserBuddies(sqlite3 *pConn);

    private:
};

#endif
