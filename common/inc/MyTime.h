#ifndef __MY_TIME_H__
#define __MY_TIME_H__

#define STR_UTC_FORMAT_MAX_LENGTH 32

class Time_c
{
    public:
        Time_c(void);
        Time_c(int nYear, int nMonths, int nDays, int nHours, int nMinutes, int nSeconds);
        ~Time_c(void);

#if 0
        unsigned long secsToBaseTime();

        void setSecsToBaseTime(unsigned long nSecsToBaseTime);
        void getUtcTimeString(char* sUcTimeStr);
#endif

    public:
        inline unsigned int getYears() const        { return m_years; }
        inline unsigned int getMonths() const       { return m_months; }
        inline unsigned int getDays() const         { return m_days; }
        inline unsigned int getMinutes() const      { return m_minutes; }
        inline unsigned int getSec() const          { return m_sec; }
        inline unsigned int getMillsec() const      { return m_millsec; }
        inline unsigned int getHours() const        { return m_hours; }

        inline void setYears(int val)               { m_years = val; }
        inline void setMonths(int val)              { m_months = val; }
        inline void setDays(int val)                { m_days = val; }
        inline void setMinutes(int val)             { m_minutes = val; }
        inline void setSec(int val)                 { m_sec = val; }
        inline void setMillsec(int val)             { m_millsec = val; }
        inline void setHours(int val)               { m_hours = val; }

        static Time_c getLocalTime();
        //This method returns the number of seconds since 1970/01/06 00:00:00 UTC
        static long long getTimestamp();

#if 0
        long long msecsTo(const UcTime & aRhs);
#endif

    private:
        //static void assignDateTime_T(const Time_c & aTime, struct SystemDate_T * , struct SystemTime_T *);

    private:
        int m_years;
        int m_months;
        int m_days;
        int m_hours;
        int m_minutes;
        int m_sec;

        int m_millsec;
};


#endif // UcTime_h__
