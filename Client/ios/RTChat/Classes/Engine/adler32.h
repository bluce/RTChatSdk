#ifndef _ADLER_32_
#define _ADLER_32_

#define BASE 65521 /* largest prime smaller than 65536 */
#define NMAX 5552
/* NMAX is the largest n such that 255n(n+1)/2 + (n+1)(BASE-1) <= 2^32-1 */

#define DO1(buf)  {s1 += *buf++; s2 += s1;}
#define DO2(buf)  DO1(buf); DO1(buf);
#define DO4(buf)  DO2(buf); DO2(buf);
#define DO8(buf)  DO4(buf); DO4(buf);
#define DO16(buf) DO8(buf); DO8(buf);

#define Z_NULL  0

#include <math.h>
#include <stdlib.h>

typedef unsigned long  uLong;
typedef unsigned char  Byte;
typedef unsigned int   uInt;

uLong adler32(uLong adler, Byte *buf, uInt len);

#endif /* _ADLER_32_ */
