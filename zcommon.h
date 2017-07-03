#ifndef _ZCOMMOM_H_
#define _ZCOMMON_H_


#define not 	!
#define and 	&&
#define or		||

#define IsBit(flags, flag)				((flags) & (flag))
#define SetBit(flags, flag, bset)		flags = (bset)?((flags) | (flag)):((flags) & ~(flag))

#define ARRAYCOUNT(a)					(sizeof(a)/sizeof((a)[0]))


#endif //_ZCOMMOM_H_