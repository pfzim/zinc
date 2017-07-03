#ifndef _ZALLOC_H_
#define _ZALLOC_H_

void *zalloc(unsigned long size);
void *zrealloc(void *lpbuf, unsigned long size);
void zfree(void *lpbuf);

#endif //_ZALLOC_H_
