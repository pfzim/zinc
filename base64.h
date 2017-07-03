#ifndef _BASE64_H_
#define _BASE64_H_

#pragma once

#define base64_encode en64
#define base64_decode de64

#define alloc_base64_encode alloc_en64
#define alloc_base64_decode alloc_de64

char* en64 (const unsigned char *in, unsigned char *out, int inlen);
int de64 (const char *in, char *out, int maxlen);

unsigned char* alloc_en64 (const unsigned char *in, int inlen);
char* alloc_de64 (const char *in, int *outlen);

#endif //_BASE64_H_
