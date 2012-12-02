/*
 Unix snprintf implementation.
 Version 1.1
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
   
   Revision History:

   1.1:
      *  added changes from Miles Bader
      *  corrected a bug with %f
      *  added support for %#g
      *  added more comments :-)
   1.0:
      *  supporting must ANSI syntaxic_sugars(see below)
   0.0:
      *  suppot %s %c %d

    it understands:
      Integer:
        %lu %lu %u
        %hd %ld %d     decimal
        %ho %lo %o     octal
        %hx %lx %x %X  hexa
      Floating points:
        %g %G %e %E %f  double
      Strings:
        %s %c  string
        %%   %

    Formating conversion flags:
      - justify left
      + Justify right or put a plus if number
      # prefix 0x, 0X for hexa and 0 for octal
      * precision/witdth is specify as an (int) in the arguments
    ' ' leave a blank for number with no sign
      l the later should be a long
      h the later should be a short

format:
  snprintf(holder, sizeof_holder, format, ...)

Return values:
  (sizeof_holder - 1)


 THANKS(for the patches and ideas):
     Miles Bader
     Cyrille Rustom
     Jacek Slabocewiz
     Mike Parker(mouse)

Alain Magloire: alainm@rcsm.ee.mcgill.ca
*/

#include <stdarg.h>
#include <stdlib.h>    /* for atoi() */
#include <cwt/types.h>


/* 
 * For the FLOATING POINT FORMAT :
 *  the challenge was finding a way to
 *  manipulate the Real numbers without having
 *  to resort to mathematical function(it
 *  would require to link with -lm) and not
 *  going down to the bit pattern(not portable)
 *
 *  so a number, a real is:

      real = integral + fraction

      integral = ... + a(2)*10^2 + a(1)*10^1 + a(0)*10^0
      fraction = b(1)*10^-1 + b(2)*10^-2 + ...

      where:
       0 <= a(i) => 9 
       0 <= b(i) => 9 
 
    from then it was simple math
 */

/*
 * size of the buffer for the integral part
 * and the fraction part 
 */
#define MAX_INT  99 + 1 /* 1 for the null */
#define MAX_FRACT 29 + 1

/* 
 * numtoa() uses PRIVATE buffers to store the results,
 * So this function is not reentrant
 */
#define itoa(n) numtoa(n, 10, 0, (CWT_CHAR **)0)
#define otoa(n) numtoa(n, 8, 0, (CWT_CHAR **)0)
#define htoa(n) numtoa(n, 16, 0, (CWT_CHAR **)0)
#define dtoa(n, p, f) numtoa(n, 10, p, f)

#define SWAP_INT(a,b) {int t; t = (a); (a) = (b); (b) = t;}

/* this struct holds everything we need */
struct DATA {
	size_t length;
	CWT_CHAR *holder;
	int counter;
	const CWT_CHAR *pf;
	/* FLAGS */
	int width, precision;
	int justify;
	CWT_CHAR pad;
	int square, space, star_w, star_p, a_long ;
};

/* signature of the functions */
/* the floating point stuff */
static double    pow_10(int);
static int       log_10(double);
static double    integral(double, double *);
static CWT_CHAR* numtoa(double, int, int, CWT_CHAR **);

/* for the format */
static void conv_flag(CWT_CHAR *, struct DATA *);
static void floating(struct DATA *, double);
static void exponent(struct DATA *, double);
static void decimal(struct DATA *, double);
static void octal(struct DATA *, double);
static void hexa(struct DATA *, double);
static void strings(struct DATA *, CWT_CHAR *);


/* those are defines specific to snprintf to hopefully
 * make the code clearer :-)
 */
#define RIGHT 1
#define LEFT  0
#define NOT_FOUND -1
#define FOUND 1
#define MAX_FIELD 15

/* the conversion flags */
#define isflag(c) ((c) == _T('#') || (c) == _T(' ') || \
                   (c) == _T('*') || (c) == _T('+') || \
                   (c) == _T('-') || (c) == _T('.') || \
                   isdigit(c))

/* round off to the precision */
#define ROUND(d, p) \
            (d < 0.) ? \
             d - pow_10(-(p)->precision) * 0.5 : \
             d + pow_10(-(p)->precision) * 0.5

/* set default precision */
#define DEF_PREC(p) \
            if ((p)->precision == NOT_FOUND) \
              (p)->precision = 6

/* put a char */
#define PUT_CHAR(c, p) \
            if ((p)->counter < (p)->length) { \
              *(p)->holder++ = (c); \
              (p)->counter++; \
            }

#define PUT_PLUS(d, p) \
            if ((d) > 0. && (p)->justify == RIGHT) \
              PUT_CHAR(_T('+'), p)

#define PUT_SPACE(d, p) \
            if ((p)->space == FOUND && (d) > 0.) \
              PUT_CHAR(_T(' '), p)

/* pad right */ 
#define PAD_RIGHT(p) \
            if ((p)->width > 0 && (p)->justify != LEFT) \
              for (; (p)->width > 0; (p)->width--) \
                 PUT_CHAR((p)->pad, p)

/* pad left */
#define PAD_LEFT(p) \
            if ((p)->width > 0 && (p)->justify == LEFT) \
              for (; (p)->width > 0; (p)->width--) \
                 PUT_CHAR((p)->pad, p)

/* if width and prec. in the args */
#define STAR_ARGS(p)                                               \
            if ((p)->star_w == FOUND)                              \
              (p)->width = va_arg(args, int);                      \
            if ((p)->star_p == FOUND)                              \
              (p)->precision = va_arg(args, int)
