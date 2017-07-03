//#include "stdafx.h"								//pf_ziminski  [2003]
#include "ztime.h"
#include "utstrings.h"

int timecompare(LPSYSTEMTIME pcr, LPSYSTEMTIME pst, int precision)
{
	//вопрос: текущее время (pcr) меньше pst
	//сравниваем pst с pcr(текущем временем)
	// pcr <  pst return <0; pst ещё не наступило
	// pcr == pst return  0; равны
	// pcr >  pst return >0; pst уже прошло
	if(pcr->wYear < pst->wYear)
	{
		return -1;
	}
	else if(pcr->wYear > pst->wYear)
	{
		return 1;
	}
	if(pcr->wMonth < pst->wMonth)
	{
		return -1;
	}
	else if(pcr->wMonth > pst->wMonth)
	{
		return 1;
	}
	if(pcr->wDay < pst->wDay)
	{
		return -1;
	}
	else if(pcr->wDay > pst->wDay)
	{
		return 1;
	}
	if(	(pcr->wSecond + pcr->wMinute*60 + pcr->wHour*60*60) <
		(pst->wSecond + pst->wMinute*60 + pst->wHour*60*60))
	{
		return -1;
	}
	else if((pcr->wSecond + pcr->wMinute*60 + pcr->wHour*60*60) >
			(pst->wSecond + pst->wMinute*60 + pst->wHour*60*60))
	{
		return 1;
	}
	if(precision)
	{
		if(pcr->wMilliseconds < pst->wMilliseconds)
		{
			return -1;
		}
		else if(pcr->wMilliseconds > pst->wMilliseconds)
		{
			return 1;
		}
	}
	return 0; // если добрались до сюда - то равны ;)
}

int timecompare(unsigned long cyear, unsigned long cmonth, unsigned long cday, unsigned long year, unsigned long month, unsigned long day)
{
	//вопрос: текущее время (pcr) меньше pst
	//сравниваем pst с pcr(текущем временем)
	// pcr <  pst return <0; pst ещё не наступило
	// pcr == pst return  0; равны
	// pcr >  pst return >0; pst уже прошло
	if(cyear < year)
	{
		return -1;
	}
	else if(cyear > year)
	{
		return 1;
	}
	if(cmonth < month)
	{
		return -1;
	}
	else if(cmonth > month)
	{
		return 1;
	}
	if(cday < day)
	{
		return -1;
	}
	else if(cday > day)
	{
		return 1;
	}
	return 0; // если добрались до сюда - то равны ;)
}

int time_compare(unsigned long dd1, unsigned long dd2)
{
	if(zyear(dd1) < zyear(dd2))
	{
		return -1;
	}
	else if(zyear(dd1) > zyear(dd2))
	{
		return 1;
	}
	if(zmonth(dd1) < zmonth(dd2))
	{
		return -1;
	}
	else if(zmonth(dd1) > zmonth(dd2))
	{
		return 1;
	}
	if(zday(dd1) < zday(dd2))
	{
		return -1;
	}
	else if(zday(dd1) > zday(dd2))
	{
		return 1;
	}
	return 0;
}

unsigned long time_inc_month(unsigned long dd, unsigned long delta)
{
	if(zmonth(dd) + delta > 12)
	{
		dd = zdate(zday(dd), (zmonth(dd) + delta)%12, zyear(dd) + ((zmonth(dd) + delta)/12));
	}
	else
	{
		dd = zdate(zday(dd), zmonth(dd) + delta, zyear(dd) + ((zmonth(dd) + delta)/12));
	}

	//проверка: если сделали 31/01/2002 +1month
	//правим: с 31/02/2002 на 28/02/2002
	unsigned long temp;
	switch(zmonth(dd))
	{
		case 02: temp = ((zyear(dd)%4 == 0) && ((zyear(dd)%100 != 0) || (zyear(dd)%400 == 0)))?29:28; break;
		case 04: case 06: case  9: case 11: temp = 30; break;
		case 01: case 03: case 05:
		case 07: case  8: case 10: case 12: temp = 31; break;
	}
	if(zday(dd) > temp)
	{
		dd = zdate(temp, zmonth(dd), zyear(dd));
		//pst->wDay -= temp;
	}

	return dd;
}

void timeincmonth(LPSYSTEMTIME pst, unsigned long delta)
{
	if(pst->wMonth + delta > 12)
	{
		pst->wYear += (WORD)(pst->wMonth+delta)/12;
		pst->wMonth = (WORD)(pst->wMonth+delta)%12;
	}
	else
	{
		pst->wMonth += (WORD)delta;
	}

	//проверка: если сделали 31/01/2002 +1month
	//правим: с 31/02/2002 на 28/02/2002
	WORD temp;
	switch(pst->wMonth)
	{
		case 02: temp = ((pst->wYear%4 == 0) && ((pst->wYear%100 != 0) || (pst->wYear%400 == 0)))?29:28; break;
		case 04: case 06: case  9: case 11: temp = 30; break;
		case 01: case 03: case 05:
		case 07: case  8: case 10: case 12: temp = 31; break;
	}
	if(pst->wDay > temp)
	{
		pst->wDay = temp;
		//pst->wDay -= temp;
	}
}

void timeincday(LPSYSTEMTIME pst, unsigned long delta)
{
	WORD temp;
	while(delta > 0)
	{
		switch(pst->wMonth)
		{
			case 02: temp = ((pst->wYear%4 == 0) && ((pst->wYear%100 != 0) || (pst->wYear%400 == 0)))?29:28; break;
			case 04: case 06: case  9: case 11: temp = 30; break;
			case 01: case 03: case 05:
			case 07: case  8: case 10: case 12: temp = 31; break;
		}
		if(pst->wDay + delta > temp)
		{
			delta -= temp - pst->wDay +1;
			pst->wDay = 1;
			timeincmonth(pst, 1);
		}
		else
		{
			pst->wDay += (WORD)delta;
			delta = 0;
		}
	}
}

void timeinchour(LPSYSTEMTIME pst, unsigned long delta)
{
	if(pst->wHour + delta >= 24)
	{
		timeincday(pst, (pst->wHour+delta)/24);
		pst->wHour = (WORD)(pst->wHour+delta)%24;
	}
	else
	{
		pst->wHour += (WORD)delta;
	}
}

void timeincminutes(LPSYSTEMTIME pst, unsigned long delta)
{
	if(pst->wMinute + delta >= 60)
	{
		timeinchour(pst, (pst->wMinute+delta)/60);
		pst->wMinute = (WORD)(pst->wMinute+delta)%60;
	}
	else
	{
		pst->wMinute += (WORD)delta;
	}
}

void timeincseconds(LPSYSTEMTIME pst, unsigned long delta)
{
	if(pst->wSecond + delta >= 60)
	{
		timeincminutes(pst, (pst->wSecond+delta)/60);
		pst->wSecond = (WORD)(pst->wSecond+delta)%60;
	}
	else
	{
		pst->wSecond += (WORD)delta;
	}
}

unsigned long time_get_days_per_month(unsigned long month, unsigned long year)
{
	switch(month)
	{
		case 02: return ((year%4 == 0) && ((year%100 != 0) || (year%400 == 0)))?29:28;
		case 04: case 06: case  9: case 11: return 30;
	}

	return 31;
}

unsigned long time_get_day_of_week(unsigned long day, unsigned long month, unsigned long year)
{
	//signed long m2;
	signed long val4;
	signed long val5;
	signed long val6;
	signed long val7;
	signed long val8;
	signed long val9;
	signed long val0;

	//m2 = month;

	if(month == 1)
	{
		month = 13;
		year = year-1;
	}
	else if(month == 2)
	{
		month = 14;
		year = year-1;
	}

	val4 = ((month+1)*3)/5;
	val5 = year/4;
	val6 = year/100;
	val7 = year/400;
	val8 = day+(month*2)+val4+year+val5-val6+val7+2;
	val9 = val8/7;
	val0 = val8-(val9*7);

	return (val0 > 1)?(val0 - 2):(val0 + 5);

	// from: http://ru.wikipedia.org/wiki/%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%B2%D1%8B%D1%87%D0%B8%D1%81%D0%BB%D0%B5%D0%BD%D0%B8%D1%8F_%D0%B4%D0%BD%D1%8F_%D0%BD%D0%B5%D0%B4%D0%B5%D0%BB%D0%B8
    //signed long a = (14 - month) / 12;
    //signed long y = year - a;
    //signed long m = month + 12 * a - 2;
    //return (7000 + (day + y + y / 4 - y / 100 + y / 400 + (31 * m) / 12)) % 7;
	//Результат: 0 — воскресенье, 1 — понедельник и т. д.

	//incorrect
	//return ((((1461*(year+4800+(month-14)/12))/4+(367*(month-2-12*((month-14)/12)))/12-(3*((year+4900+(month-14)/12)/100))/4+day-32075+1)%7)+6)%7;	// monday is 0
	//return ((1461*(year+4800+(month-14)/12))/4+(367*(month-2-12*((month-14)/12)))/12-(3*((year+4900+(month-14)/12)/100))/4+day-32075+1)%7;			// sunday is 0
}

/*
__int64 timesubtime(LPSYSTEMTIME pst, LPSYSTEMTIME cst)
{
	return *(__int64 *)pst - *(__int64 *)cst;
}
*/

unsigned long timesubtime(unsigned short *h1, unsigned short *m1, unsigned short *s1, unsigned short h2, unsigned short m2, unsigned short s2)
{
	//20:20:20
	//19:20:21

	if(*s1 < s2)
	{
		*s1 += 60;
		if(*m1)
		{
			*m1 -= 1;
		}
		else
		{
			*m1 = 59;
			if(*h1)
			{
				*h1 -= 1;
			}
			else
			{
				*h1 = 23;
			}
		}
	}

	*s1 -= s2;

	if(*m1 < m2)
	{
		*m1 += 60;
		if(*h1)
		{
			*h1 -= 1;
		}
		else
		{
			*h1 = 23;
		}
	}

	*m1 -= m2;

	if(*h1 < h2)	// || ((*h1 == h2) && ((*m1 < m2) || ((*m1 == m2) && (*s1 < s2)))))
	{
		*h1 += 24;
	}

	*h1 -= h2;

	return ((*h1) * 60 + (*m1)) * 60 + (*s1);
}
