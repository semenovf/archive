/*
 * pcre.c
 *
 *  Created on: 10.08.2012
 *      Author: wladt
 */


#include <cwt/pcre/pcre.h>
/*
#include <cwt/string.h>
#include <cwt/txtcodec.h>
#include <cwt/unistd.h>
#include <cwt/logger.h>
#include <cwt/bytearr.h>
#include <cwt/string.h>
#include <cwt/strlist.h>
#include <cwt/io/channel.h>
#include <cwt/csv/csv.h>
#include <cwt/hashtab.h>
*/

extern pcre*            pcre_compile                 (const char*, int, const char**, int*, const unsigned char*);
extern pcre*            pcre_compile2                (const char*, int, int*, const char**, int*, const unsigned char*);
extern int              pcre_config                  (int, void*);
extern int              pcre_copy_named_substring    (const pcre*, const char*, int*, int, const char*, char*, int);
extern int              pcre_copy_substring          (const char*, int*, int, int, char*, int);
extern int              pcre_dfa_exec                (const pcre*, const pcre_extra*, const char*, int, int, int, int*, int , int*, int);
extern int              pcre_exec                    (const pcre*, const pcre_extra*, PCRE_SPTR, int, int, int, int*, int);
extern void             pcre_free_substring          (const char*);
extern void             pcre_free_substring_list     (const char**);
extern int              pcre_fullinfo                (const pcre*, const pcre_extra*, int, void*);
extern int              pcre_get_named_substring     (const pcre*, const char*, int*, int, const char*, const char**);
extern int              pcre_get_stringnumber        (const pcre*, const char *);
extern int              pcre_get_stringtable_entries (const pcre*, const char*, char**, char**);
extern int              pcre_get_substring           (const char*, int*, int, int, const char**);
extern int              pcre_get_substring_list      (const char*, int*, int, const char***);
extern int              pcre_info                    (const pcre*, int*, int*);
extern const UCHAR*     pcre_maketables              (void);
extern int              pcre_refcount                (pcre*, int);
extern pcre_extra*      pcre_study                   (const pcre*, int, const char**);
extern const CWT_CHAR*  pcre_version                 (void);

extern size_t           pcre_regerror(int errcode, const regex_t*, char*, size_t);
extern void             pcre_regfree(regex_t *preg);
extern int              pcre_regcomp(regex_t *preg, const char *pattern, int cflags);
extern int              pcre_regexec(const regex_t *preg, const char *string, size_t nmatch, regmatch_t pmatch[], int eflags);
/*extern int              pcre_callout                 (pcre_callout_block *);*/


static CwtPCRENS __cwtPCRENS = {
	  pcre_compile
	, pcre_compile2
	, pcre_config
	, pcre_copy_named_substring
	, pcre_copy_substring
	, pcre_dfa_exec
	, pcre_exec
	, pcre_free_substring
	, pcre_free_substring_list
	, pcre_fullinfo
	, pcre_get_named_substring
	, pcre_get_stringnumber
	, pcre_get_stringtable_entries
	, pcre_get_substring
	, pcre_get_substring_list
	, pcre_info
	, pcre_maketables
	, pcre_refcount
	, pcre_study
	, pcre_version

	, pcre_regerror
	, pcre_regfree
	, pcre_regcomp
	, pcre_regexec
	, NULL
};


DLL_API_EXPORT CwtPCRENS* cwtPCRENS(void)
{
	return &__cwtPCRENS;
}
