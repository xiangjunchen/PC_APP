
#define BITS_IN_JSAMPLE  8	/* use 8 or 12 */

#define MAX_COMPONENTS  10	/* maximum number of image components */

#if BITS_IN_JSAMPLE == 8
/* JSAMPLE should be the smallest type that will hold the values 0..255.
* You can use a signed char by having GETJSAMPLE mask it with 0xFF.
*/

#ifdef HAVE_UNSIGNED_CHAR

typedef unsigned char JSAMPLE;
#define GETJSAMPLE(value)  ((int) (value))

#else /* not HAVE_UNSIGNED_CHAR */

typedef char JSAMPLE;
#ifdef CHAR_IS_UNSIGNED
#define GETJSAMPLE(value)  ((int) (value))
#else
#define GETJSAMPLE(value)  ((int) (value) & 0xFF)
#endif /* CHAR_IS_UNSIGNED */

#endif /* HAVE_UNSIGNED_CHAR */

#define MAXJSAMPLE	255
#define CENTERJSAMPLE	128

#endif /* BITS_IN_JSAMPLE == 8 */


#if BITS_IN_JSAMPLE == 12
/* JSAMPLE should be the smallest type that will hold the values 0..4095.
* On nearly all machines "short" will do nicely.
*/

typedef short JSAMPLE;
#define GETJSAMPLE(value)  ((int) (value))

#define MAXJSAMPLE	4095
#define CENTERJSAMPLE	2048

#endif /* BITS_IN_JSAMPLE == 12 */

typedef short JCOEF;

#ifdef HAVE_UNSIGNED_CHAR

typedef unsigned char JOCTET;
#define GETJOCTET(value)  (value)

#else /* not HAVE_UNSIGNED_CHAR */

typedef char JOCTET;
#ifdef CHAR_IS_UNSIGNED
#define GETJOCTET(value)  (value)
#else
#define GETJOCTET(value)  ((value) & 0xFF)
#endif /* CHAR_IS_UNSIGNED */

#endif /* HAVE_UNSIGNED_CHAR */

/* UINT8 must hold at least the values 0..255. */

#ifdef HAVE_UNSIGNED_CHAR
typedef unsigned char UINT8;
#else /* not HAVE_UNSIGNED_CHAR */
#ifdef CHAR_IS_UNSIGNED
typedef char UINT8;
#else /* not CHAR_IS_UNSIGNED */
typedef short UINT8;
#endif /* CHAR_IS_UNSIGNED */
#endif /* HAVE_UNSIGNED_CHAR */

/* UINT16 must hold at least the values 0..65535. */

#ifdef HAVE_UNSIGNED_SHORT
typedef unsigned short UINT16;
#else /* not HAVE_UNSIGNED_SHORT */
typedef unsigned int UINT16;
#endif /* HAVE_UNSIGNED_SHORT */

#ifndef XMD_H			/* X11/xmd.h correctly defines INT16 */
typedef short INT16;
#endif


typedef unsigned int JDIMENSION;

#define JPEG_MAX_DIMENSION  65500L  /* a tad under 64K to prevent overflows */

/* a function called through method pointers: */
#define METHODDEF(type)		static type
/* a function used only in its module: */
#define LOCAL(type)		static type
/* a function referenced thru EXTERNs: */
#define GLOBAL(type)		type
/* a reference to a GLOBAL function: */
#define EXTERN(type)		extern type

#ifdef HAVE_PROTOTYPES
#define JMETHOD(type,methodname,arglist)  type (*methodname) arglist
#else
#define JMETHOD(type,methodname,arglist)  type (*methodname) ()
#endif


#ifndef FAR
#ifdef NEED_FAR_POINTERS
#define FAR  far
#else
#define FAR
#endif
#endif

#ifndef HAVE_BOOLEAN
typedef int boolean;
#endif
#ifndef FALSE			/* in case these macros already exist */
#define FALSE	0		/* values of boolean */
#endif
#ifndef TRUE
#define TRUE	1
#endif