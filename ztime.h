#ifndef _ZTIME_H_
#define _ZTIME_H_

#include <windows.h>

int timecompare(LPSYSTEMTIME pcr, LPSYSTEMTIME pst, int precision);
int timecompare(unsigned long cyear, unsigned long cmonth, unsigned long cday, unsigned long year, unsigned long month, unsigned long day);
int time_compare(unsigned long dd1, unsigned long dd2);
unsigned long time_inc_month(unsigned long dd, unsigned long delta);
void timeincmonth(LPSYSTEMTIME pst, unsigned long delta);
void timeincday(LPSYSTEMTIME pst, unsigned long delta);
void timeinchour(LPSYSTEMTIME pst, unsigned long delta);
void timeincminutes(LPSYSTEMTIME pst, unsigned long delta);
void timeincseconds(LPSYSTEMTIME pst, unsigned long delta);
unsigned long time_get_days_per_month(unsigned long month, unsigned long year);
unsigned long time_get_day_of_week(unsigned long day, unsigned long month, unsigned long year);
unsigned long timesubtime(unsigned short *h1, unsigned short *m1, unsigned short *s1, unsigned short h2, unsigned short m2, unsigned short s2);

inline void GetLocalTimeRUS(LPSYSTEMTIME lpSystemTime)
{
	GetLocalTime(lpSystemTime);
	if(lpSystemTime->wDayOfWeek == 0) lpSystemTime->wDayOfWeek = 7;
}


#endif //_ZTIME_H_