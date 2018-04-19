/*
 sha1.h: Implementation of SHA-1 Secure Hash Algorithm-1

 Based upon: NIST FIPS180-1 Secure Hash Algorithm-1
   http://www.itl.nist.gov/fipspubs/fip180-1.htm

 Non-official Japanese Translation by HIRATA Yasuyuki:
   http://yasu.asuka.net/translations/SHA-1.html

 Copyright (C) 2002 vi@nwr.jp. All rights reserved.

 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

 1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgement in the product documentation would be
    appreciated but is not required.
 2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as beging the original software.
 3. This notice may not be removed or altered from any source distribution.

 Note:
   The copyright notice above is copied from md5.h by L. Petet Deutsch
   <ghost@aladdin.com>. Thank him since I'm not a good speaker of English. :)
 */

/**
 * @file   sha1.h
 * @author see Copyright above
 * @date   Feb 21, 2013 1:14:58 PM
 *
 * @brief
 */

#ifndef __CWT_SHA1_H__
#define __CWT_SHA1_H__

#include <cwt/cwt.h>
#include <stdio.h>

typedef	unsigned int	sha1_word_t;	/* 32bits unsigned integer */
typedef unsigned char	sha1_byte_t;	/* 8bits unsigned integer */
#define	BITS		8

/* Define the state of SHA-1 algorithm */
typedef struct {
  sha1_byte_t	sha1_buf[64];	/* 512 bits */
  int		    sha1_count;	    /* How many bytes are used */
  sha1_word_t	sha1_size1;		/* Length counter Lower Word */
  sha1_word_t	sha1_size2;		/* Length counter Upper Word */
  sha1_word_t	sha1_h[5];		/* Hash output */
} sha1_state_t;
#define	SHA1_OUTPUT_SIZE	20	/* in bytes */

EXTERN_C_BEGIN
DLL_API void  sha1_init(sha1_state_t *pms); /* Initialize SHA-1 algorithm */
DLL_API void  sha1_update(sha1_state_t *pms, const sha1_byte_t *input_buffer, int length); /* Append a string to SHA-1 algorithm */
DLL_API void  sha1_finish(sha1_state_t *pms, sha1_byte_t output[SHA1_OUTPUT_SIZE]);  /* Finish the SHA-1 algorithm and return the hash */
DLL_API char* sha1_file(FILE *fp);
DLL_API char* sha1_str(const char *p);
EXTERN_C_END

#endif /* __CWT_SHA1_H__ */
