#ifndef _GIF_LIB_PRIVATE_H
#define _GIF_LIB_PRIVATE_H

#include "sg_interface.h"
#include "sg_algorithm.h"

#define LZ_MAX_CODE         4095    /* Biggest code possible in 12 bits. */
#define LZ_BITS             12

#define FLUSH_OUTPUT        4096    /* Impossible code, to signal flush. */
#define FIRST_CODE          4097    /* Impossible code, to signal first. */
#define NO_SUCH_CODE        4098    /* Impossible code, to signal empty. */

#define FILE_STATE_WRITE    0x01
#define FILE_STATE_SCREEN   0x02
#define FILE_STATE_IMAGE    0x04
#define FILE_STATE_READ     0x08

#define IS_READABLE(Private)    (Private->FileState & FILE_STATE_READ)
#define IS_WRITEABLE(Private)   (Private->FileState & FILE_STATE_WRITE)

typedef struct SGFilePrivateType {
    SGWord FileState,  /* Where all this data goes to! */
      BitsPerPixel,     /* Bits per pixel (Codes uses at least this + 1). */
      ClearCode,   /* The CLEAR LZ code. */
      EOFCode,     /* The EOF LZ code. */
      RunningCode, /* The next code algorithm can generate. */
      RunningBits, /* The number of bits required to represent RunningCode. */
      MaxCode1,    /* 1 bigger than max. possible code, in RunningBits bits. */
      LastCode,    /* The code before the current code. */
      CrntCode,    /* Current algorithm code. */
      StackPtr,    /* For character stack (see below). */
      CrntShiftState;    /* Number of bits in CrntShiftDWord. */
    unsigned long CrntShiftDWord;   /* For bytes decomposition into codes. */
    unsigned long PixelCount;   /* Number of pixels in image. */
    //FILE *File;    /* File as stream. */
	void *hGifFile; // �൱�� HANDLE������ʹ�� CreateFile()
    InputFunc Read;     /* function to read gif input (TVT) */
    OutputFunc Write;   /* function to write gif output (MRB) */
    SGByteType Buf[256];   /* Compressed input is buffered here. */
    SGByteType Stack[LZ_MAX_CODE]; /* Decoded pixels are stacked here. */
    SGByteType Suffix[LZ_MAX_CODE + 1];    /* So we can trace the codes. */
    SGPrefixType Prefix[LZ_MAX_CODE + 1];
    SGHashTableType *HashTable;
} SGFilePrivateType;

#endif /* _GIF_LIB_PRIVATE_H */
