#include <ctype.h>

#include <cwt/algo/hash_str.h>

/* String hash function */

DLL_API_EXPORT unsigned long wstring_hash(void *string)
{
	/* This is the djb2 string hash function */

	unsigned long result = 5381;
	wchar_t *p;

	p = (wchar_t*) string;

	while (*p != L'\0') {
		result = ((result << 5) ^ result ) ^ (*p);
		++p;
	}

	return result;
}

/* The same function, with a tolower on every character so that 
 * case is ignored.  This code is duplicated for performance. */

DLL_API_EXPORT unsigned long wstring_nocase_hash(void *string)
{
	unsigned long result = 5381;
	wchar_t *p;

	p = (wchar_t *) string;

	while (*p != L'\0') {
		result = ((result << 5) ^ result ) ^ towlower(*p);
		++p;
	}
	
	return result;
}

