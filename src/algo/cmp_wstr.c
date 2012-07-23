#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <cwt/algo/cmp_str.h>

/* Comparison functions for strings */

DLL_API_EXPORT int wstring_equal(void *string1, void *string2)
{
	return wcscmp((wchar_t *) string1, (wchar_t *) string2) == 0;
}

DLL_API_EXPORT int wstring_compare(void *string1, void *string2)
{
	int result;

	result = wcscmp((wchar_t *) string1, (wchar_t *) string2);
	
	if (result < 0) {
		return -1;
	} else if (result > 0) {
		return 1;
	} else {
		return 0;
	}
}

/* Comparison functions for strings, which ignore the case of letters. */

DLL_API_EXPORT int wstring_nocase_equal(void *string1, void *string2)
{
	return string_nocase_compare((wchar_t *) string1, (wchar_t *) string2) == 0;
}

/* On many systems, strcasecmp or stricmp will give the same functionality
 * as this function.  However, it is non-standard and cannot be relied
 * on to be present. */

DLL_API_EXPORT int wstring_nocase_compare(void *string1, void *string2)
{
	wchar_t *p1;
	wchar_t *p2;
	wint_t c1, c2;

	/* Iterate over each character in the strings */

	p1 = (wchar_t *) string1;
	p2 = (wchar_t *) string2;

	for (;;) {

		c1 = towlower(*p1);
		c2 = towlower(*p2);

		if (c1 != c2) {

			/* Strings are different */

			if (c1 < c2) {
				return -1;
			} else {
				return 1;
			}
		}

		/* End of string */

		if (c1 == '\0')
			break;

		/* Advance to the next character */

		++p1;
		++p2;
	}

	/* Reached the end of string and no difference found */

	return 0;
}


