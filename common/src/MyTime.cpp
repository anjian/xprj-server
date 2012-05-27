//#include <com/inc/UcStdLibrary.h>

#include <time.h>
#include <sys/time.h>

#include <common/inc/MyTime.h>

#if 0
//#define STR_UTC_FORMAT "Wed, 02-Nov-2099 14:36:28 GMT"
#define STR_UTC_FORMAT "%02d-%s-%4d %02d:%02d:%02d GMT"

const char* months_g[] = 
{
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec"
};
#endif

Time_c::Time_c() :
    m_years(0),
    m_months(0),
    m_days(0),
    m_hours(0),
    m_minutes(0),
    m_sec(0),
    m_millsec(0)
{
}

Time_c::Time_c(int nYear, int nMonths, int nDays, int nHours, int nMinutes, int nSeconds) :
    m_years(nYear),
    m_months(nMonths),
    m_days(nDays),
    m_hours(nHours),
    m_minutes(nMinutes),
    m_sec(nSeconds),
    m_millsec(0)
{
}

Time_c::~Time_c()
{
}

Time_c Time_c::getLocalTime()
{
#if 0
    SystemEngine * psSystemEngine = system_engine_get_instance();
    const struct SystemOp * psSystemOp = psSystemEngine->m_system_op;
    SystemDate_T sDate;
    SystemTime_T sTime;
    psSystemOp->get_local_date_time(&sDate,&sTime);
#endif

    time_t tNow;
    struct tm *ts;
    struct timeval tv;

    int nMSec = 0;

    // get current time
    time(&tNow);

    // get current million seconds
    if (0 == gettimeofday(&tv, NULL))
    {
        nMSec = tv.tv_usec;
    }

    ts = localtime(&tNow);

    Time_c sUcTime;
    sUcTime.m_years     = ts->tm_year;
    sUcTime.m_months    = ts->tm_mon;
    sUcTime.m_days      = ts->tm_mday;
    sUcTime.m_hours     = ts->tm_hour;
    sUcTime.m_minutes   = ts->tm_min;
    sUcTime.m_sec       = ts->tm_sec;
    sUcTime.m_millsec   = nMSec;

    return sUcTime;
}

//This method returns the number of seconds since 1970/01/01 00:00:00 UTC
long long Time_c::getTimestamp()
{
    return time(NULL);
}

#if 0
long long UcTime::msecsTo(const Time_c & aRhs)
{
    struct SystemDate_T sSystemDate1,sSystemDate2;
    struct SystemTime_T sSystemTime1,sSystemTime2;


    assignDateTime_T(*this,&sSystemDate1,&sSystemTime1);

    assignDateTime_T(aRhs,&sSystemDate2,&sSystemTime2);

    SystemEngine * psSystemEngine = system_engine_get_instance();
    const struct SystemOp * psSystemOp = psSystemEngine->m_system_op;

    return psSystemOp->date_time_msec_to(&sSystemDate1,&sSystemTime1,
            &sSystemDate2,&sSystemTime2);
}

unsigned long Time_c::getLocalTimestamp()
{
    SystemEngine * psSystemEngine = system_engine_get_instance();
    const struct SystemOp * psSystemOp = psSystemEngine->m_system_op;

    return psSystemOp->get_local_time_stamp();
}


unsigned long Time_c::secsToBaseTime()
{
    SystemEngine * psSystemEngine = system_engine_get_instance();
    const struct SystemOp * psSystemOp = psSystemEngine->m_system_op;
    struct SystemDate_T sDate;
    struct SystemTime_T sTime;

    assignDateTime_T(*this,&sDate,&sTime);
    return psSystemOp->secs_to_base_time(&sDate,&sTime);
}

void Time_c::setSecsToBaseTime(unsigned long nSecsToBaseTime)
{
    SystemEngine * psSystemEngine = system_engine_get_instance();
    const struct SystemOp * psSystemOp = psSystemEngine->m_system_op;
    struct SystemDate_T sDate;
    struct SystemTime_T sTime;

    psSystemOp->get_date_by_secs_to_base_time(nSecsToBaseTime, &sDate, &sTime);
    this->setYears(sDate.m_year);
    this->setMonths(sDate.m_month);
    this->setDays(sDate.m_day);
    this->setHours(sTime.m_hour);
    this->setMinutes(sTime.m_minute);
    this->setSec(sTime.m_sec);
}

void Time_c::getUtcTimeString(char* sUcTimeStr)
{
    if (NULL != sUcTimeStr)
    {
        uc_sprintf(sUcTimeStr, STR_UTC_FORMAT, m_days, months_g[m_months-1], m_years, m_hours, m_minutes, m_sec);
    }
}

void Time_c::assignDateTime_T( const UcTime & aTime,struct SystemDate_T * aSysDate, struct SystemTime_T * aSysTime)
{
    if(aSysTime)
    {
        aSysTime->m_hour = aTime.m_hours;
        aSysTime->m_minute = aTime.m_minutes;
        aSysTime->m_sec = aTime.m_sec;
        aSysTime->m_millisec = aTime.m_millsec;
    }

    if(aSysDate)
    {
        aSysDate->m_year = aTime.m_years;
        aSysDate->m_month = aTime.m_months;
        aSysDate->m_day = aTime.m_days;
    }

}
#endif

