#ifndef __CWT_STR_H__
#define __CWT_STR_H__

#ifdef _STRING_H
#	error do not need to mix <cwt/str.h> and <string.h>
#endif
#include <time.h>
#include <cwt/types.h>
#include <cwt/string.h>
#include <cwt/bytearr.h>

typedef struct CwtStrNS {
	const CWT_CHAR*   (*strerror)     (int);
	size_t            (*strftime)     (CWT_CHAR *buf, size_t sz, const CWT_CHAR *format, const struct tm* ptm);
	size_t            (*strlen)       (const CWT_CHAR*);
	CWT_CHAR*         (*strcpy)       (CWT_CHAR *dest, const CWT_CHAR *src);
	CWT_CHAR*         (*strncpy)      (CWT_CHAR *dest, const CWT_CHAR *src, size_t n);
	CWT_CHAR*         (*strchr)       (CWT_CHAR *s, CWT_CHAR ch);
	CWT_CHAR*         (*strstr)       (const CWT_CHAR *s, const CWT_CHAR *substr);
	CWT_CHAR*         (*strrstr)      (const CWT_CHAR *s, const CWT_CHAR *substr);
	BOOL              (*streq)        (const CWT_CHAR *s1, const CWT_CHAR *s2);
	BOOL              (*strieq)       (const CWT_CHAR *s1, const CWT_CHAR *s2);
	int               (*strcmp)       (const CWT_CHAR *s1, const CWT_CHAR *s2);
	int               (*strncmp)      (const CWT_CHAR *s1, const CWT_CHAR *s2, size_t n);
	int 			  (*stricmp)      (const CWT_CHAR *s1, const CWT_CHAR *s2);
	int               (*strnicmp)     (const CWT_CHAR *s1, const CWT_CHAR *s2, size_t n);
	CWT_CHAR*         (*strdup)       (const CWT_CHAR *s);
	CWT_CHAR*         (*strndup)      (const CWT_CHAR *s, size_t n);
	CWT_CHAR*         (*strcat)       (CWT_CHAR *dest, const CWT_CHAR *src);
	CWT_CHAR*         (*strncat)      (CWT_CHAR *dest, const CWT_CHAR *src, size_t n);
	CWT_CHAR*		  (*strtok)       (CWT_CHAR *tok, const CWT_CHAR *delim);
	LONG              (*strtol)       (const CWT_CHAR *s, CWT_CHAR **endptr, int radix);
	ULONG             (*strtoul)      (const CWT_CHAR *s, CWT_CHAR **endptr, int radix);
	LONGLONG          (*strtoll)      (const CWT_CHAR *s, CWT_CHAR **endptr, int radix);
	ULONGLONG         (*strtoull)     (const CWT_CHAR *s, CWT_CHAR **endptr, int radix);
	int               (*isalpha)      (CWT_CHAR);

	void*             (*memcpy)       (void *to, const void *from, size_t sz);
	void*             (*memmove)      (void *to, const void *from, size_t sz);
	void*             (*memset)       (void *block, int c, size_t sz);
	void*             (*bzero)        (void *block, size_t sz);
	int               (*memcmp)       (const void *m1, const void *m2, size_t n);

	LONGLONG          (*toLONGLONG)   (const CWT_CHAR *str, int radix, BOOL *ok);
	ULONGLONG         (*toULONGLONG)  (const CWT_CHAR *str, int radix, BOOL *ok);
	LONG              (*toLONG)       (const CWT_CHAR *str, int radix, BOOL *ok);
	ULONG             (*toULONG)      (const CWT_CHAR *str, int radix, BOOL *ok);
	INT               (*toINT)        (const CWT_CHAR *str, int radix, BOOL *ok);
	UINT              (*toUINT)       (const CWT_CHAR *str, int radix, BOOL *ok);
	SHORT             (*toSHORT)      (const CWT_CHAR *str, int radix, BOOL *ok);
	USHORT            (*toUSHORT)     (const CWT_CHAR *str, int radix, BOOL *ok);
	SBYTE             (*toSBYTE)      (const CWT_CHAR *str, int radix, BOOL *ok);
	BYTE              (*toBYTE)       (const CWT_CHAR *str, int radix, BOOL *ok);

	INT8              (*toINT8)       (const CWT_CHAR *str, int radix, BOOL *ok);
	UINT8             (*toUINT8)      (const CWT_CHAR *str, int radix, BOOL *ok);
	INT16             (*toINT16)      (const CWT_CHAR *str, int radix, BOOL *ok);
	UINT16            (*toUINT16)     (const CWT_CHAR *str, int radix, BOOL *ok);
	INT32             (*toINT32)      (const CWT_CHAR *str, int radix, BOOL *ok);
	UINT32            (*toUINT32)     (const CWT_CHAR *str, int radix, BOOL *ok);
	INT64             (*toINT64)      (const CWT_CHAR *str, int radix, BOOL *ok);
	UINT64            (*toUINT64)     (const CWT_CHAR *str, int radix, BOOL *ok);

	CWT_CHAR          (*toupper)      (CWT_CHAR ch);
	CWT_CHAR          (*tolower)      (CWT_CHAR ch);
	void              (*toupperStr)   (CWT_CHAR *dest, const CWT_CHAR *src, size_t n);
	void              (*tolowerStr)   (CWT_CHAR *dest, const CWT_CHAR *src, size_t n);
	void              (*chomp)        (CWT_CHAR *s);

	const CWT_CHAR*   (*constEmptyStr)(void);
	const CWT_CHAR*   (*constNullStr) (void);
} CwtStrNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtStrNS* cwtStrNS(void);
EXTERN_C_END

#define CWT_STRING_OR_EMPTYSTR(s) ((s) ? (s) : cwtStrNS()->constEmptyStr())
#define CWT_STRING_OR_NULLSTR(s) ((s) ? (s) : cwtStrNS()->constNullStr())


#ifdef __COMMENT__
/*
#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable:4275)
#endif
*/
class DLL_API String : public base_string_t {
#ifdef _MSC_VER
#	pragma warning(pop)
#endif
public:
	static const String& emptyString();

public:
	String()
		: base_string_t() {}
	String( const Char *chars )
		: base_string_t(chars) {}
	String( const base_string_t& str )
		: base_string_t(str) {}
	String( const String& str )
		: base_string_t(str) {}
	String( const String& str, size_t pos, size_t n = npos )
		: base_string_t(str, pos, n) {}
	String( const Char* s, size_t n )
		: base_string_t(s, n) {}
	String( size_t n, Char c )
		: base_string_t(n,c) {}

#ifdef JQ_UNICODE
	String( const char* chars );
	String( const char* chars, size_t n );
#endif

	template<class InputIterator>
	String( InputIterator begin, InputIterator end )
		: base_string_t( begin, end ) {}

	String& operator=(const String& str) { assign(str); return *this; }
	String& operator=(const base_string_t& str) { assign(str); return *this; }
	String& operator=(const Char* chars) { chars ? assign(chars) : assign(_T("")); return *this; }

	void chomp();
	bool equalsTo(const jq::String& s) const { return 0 == this->compare(s); }


	// TODO http://stackoverflow.com/questions/874134/find-if-string-endswith-another-string-c
	bool endsWith( const jq::String& ending ) const
	{
		return (length() > ending.length())
			? (0 == compare(length() - ending.length(), ending.length(), ending))
			: false;
	}
	bool startsWith( const jq::String& starting ) const
	{
	    return length() > starting.length()
	    	? find(starting) == 0
	    	: false;
	}

	bool startsWith( const jq::Char& starting ) const
	{
	    return length() > 0
	    	? (*this)[0] == starting
	    	: false;
	}


	String& replaceAll(const String& from, const String& to);

    // TODO need to realize the support of 'base' feature, double long and floating point types
    String&	setNum( int n, int base = 10 )      { base=base; sprintf(_T("%d"))(n); return *this;  }
    String&	setNum( uint_t n, int base = 10 )   { base=base; sprintf(_T("%u"))(n); return *this;  }
    String&	setNum( long n, int base = 10 )     { base=base; sprintf(_T("%ld"))(n); return *this; }
    String&	setNum( ulong_t n, int base = 10 )  { base=base; sprintf(_T("%lu"))(n); return *this; }
    String&	setNum( short n, int base = 10 )    { base=base; sprintf(_T("%d"))(n); return *this;  }
    String& setNum( ushort_t n, int base = 10 ) { base=base; sprintf(_T("%u"))(n); return *this;  }

	String& fromUtf8(const char* utf8, bool* ok = NULL);
	String& fromUtf8(char utf8char, bool* ok = NULL) { char utf8chars[2] = {utf8char, '\0'}; return fromUtf8(utf8chars, ok); }
	std::string toUtf8(bool* ok = NULL) const;

	std::string toLatin1() const;
};
#endif
#endif /*__CWT_STR_H__*/
