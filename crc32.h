#ifndef _CRC32_H_
#define _CRC32_H_

unsigned long crc32(const char *pBuf);
unsigned long crc32(unsigned long &crc, const char *pBuf);
unsigned long crc32(unsigned long &crc, const char *pBuf, unsigned long dwBufSize);
unsigned long file_crc32(const char *filename);

#endif //_CRC32_H_
