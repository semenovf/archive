#ifndef __CWT_STRING_H__
#define __CWT_STRING_H__

#include <cwt/global.h>
#include <cwt/types.h>
#include <string.h>
/*

#define jq_strcoll  strcoll
#define jq_strcspn  strcspn
#define jq_strpbrk  strpbrk
#define jq_strrchr  strrchr
#define jq_strspn   strspn
*/
#define cwtStrError          strerror
#define cwtStrTok            strtok
#define cwtStrCat            strcat
#define cwtStrNcat           strncat
#define cwtStrChr            strchr
#define cwtStrCmp            strcmp
#define cwtStrNcmp           strncmp
#define cwtStrIcmp(s1,s2)    CWT_ISO_CPP_NAME(stricmp)(s1, s2)
#define cwtStrNicmp(s1,s2,n) CWT_ISO_CPP_NAME(strnicmp)(s1, s2, n)
#define cwtStrLen(s)         strlen(s)
#define cwtStrEq(s1,s2)      (cwtStrCmp(s1, s2) == 0)
#define cwtStrEqi(s1,s2)     (cwtStrIcmp(s1, s2) == 0)
#define cwtStrDup   CWT_ISO_CPP_NAME(strdup)
#define cwtStrStr(s,substr)  strstr(s,substr)
#define cwtStrCpy   strcpy
#define cwtStrNcpy  strncpy

#define cwtMemCpy   memcpy
#define cwtMemChr   memchr
#define cwtMemCmp   memcmp
#define cwtMemMove  memmove
#define cwtMemSet   memset
#define cwtMemZero(p,sz) memset(p, 0, sz)

extern const CHAR* CWT_CONST_EMPTYSTR;
extern const CHAR* CWT_CONST_NULLSTR;

#define CWT_STRING_OR_EMPTYSTR(s) ((s) ? (s) : CWT_CONST_EMPTYSTR)
#define CWT_STRING_OR_NULLSTR(s) ((s) ? (s) : CWT_CONST_NULLSTR)

EXTERN_C_BEGIN

DLL_API_EXPORT CHAR* cwtStrNdup(const CHAR *s, size_t n);

EXTERN_C_END

#endif /*_JQ_STRING_H_*/
