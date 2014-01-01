#ifndef _GIF_LIB_H_
#define _GIF_LIB_H_ 1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GIF_ERROR   0
#define GIF_OK      1

#ifndef TRUE
#define TRUE        1
#endif /* TRUE */
#ifndef FALSE
#define FALSE       0
#endif /* FALSE */

#ifndef NULL
#define NULL        0
#endif /* NULL */

#define GIF_STAMP "GIFVER"          /* First chars in file - GIF stamp.  */
#define GIF_STAMP_LEN sizeof(GIF_STAMP) - 1
#define GIF_VERSION_POS 3           /* Version first character in stamp. */
#define GIF87_STAMP "GIF87a"        /* First chars in file - GIF stamp.  */
#define GIF89_STAMP "GIF89a"        /* First chars in file - GIF stamp.  */

#define HAVE_BASETSD_H 1
#define HAVE_IO_H 1
#define HAVE_FCNTL_H 1
#define WINDOWS32

#define GIF_FILE_BUFFER_SIZE 16384  /* 16 K Files uses bigger buffers than usual. */

typedef int SGBooleanType;
typedef unsigned char SGPixelType;
typedef unsigned char *SGRowType;
typedef unsigned char SGByteType;

typedef unsigned int SGPrefixType;
typedef int SGWord;


#define VoidPtr void *

typedef struct SGPalette {
    SGByteType Red;
	SGByteType Green;
	SGByteType Blue;
} SGPalette;

typedef struct SGColorMap {
    int ColorCount;
    int BitsPerPixel;
    SGPalette *Colors;    /* on malloc(3) heap */
} SGColorMap;

typedef struct SGImageDesc {
    SGWord Left;
	SGWord Top;
	SGWord Width;
	SGWord Height;   /* Current image dimensions. */
    SGWord Interlace;                    /* Sequential/Interlaced lines. */
    SGColorMap *ColorMap;       /* The local color map */
} SGImageDesc;

typedef struct SGFileType {
    SGWord SWidth;
	SGWord SHeight;        /* Screen dimensions. */
    SGWord SColorResolution;         /* How many colors can we generate? */
    SGWord SBackGroundColor;         /* I hope you understand this one... */
    SGColorMap *SColorMap;  /* NULL if not exists. */
    int ImageCount;             /* Number of current image */
    SGImageDesc Image;         /* Block describing current image */
    struct SavedImage *SavedImages; /* Use this to accumulate file state */
    VoidPtr UserData;           /* hook to attach user data (TVT) */
    VoidPtr Private;            /* Don't mess with this! */
} SGFileType;

typedef enum {
    UNDEFINED_RECORD_TYPE,
    SCREEN_DESC_RECORD_TYPE,
    IMAGE_DESC_RECORD_TYPE, /* Begin with ',' */
    EXTENSION_RECORD_TYPE,  /* Begin with '!' */
    TERMINATE_RECORD_TYPE   /* Begin with ';' */
} SGRecordType;

/* DumpScreen2SG routine constants identify type of window/screen to dump.
 * Note all values below 1000 are reserved for the IBMPC different display
 * devices (it has many!) and are compatible with the numbering TC2.0
 * (Turbo C 2.0 compiler for IBM PC) gives to these devices.
 */
//typedef enum {
//    GIF_DUMP_SGI_WINDOW = 1000,
//    GIF_DUMP_X_WINDOW = 1001
//} SGScreenDumpType;

/* func type to read gif data from arbitrary sources (TVT) */
typedef int (*InputFunc) (SGFileType *, SGByteType *, int);

/* func type to write gif data ro arbitrary targets.
 * Returns count of bytes written. (MRB)
 */
typedef int (*OutputFunc) (SGFileType *, const SGByteType *, int);



#define COMMENT_EXT_FUNC_CODE     0xfe    /* comment */
#define GRAPHICS_EXT_FUNC_CODE    0xf9    /* graphics control */
#define PLAINTEXT_EXT_FUNC_CODE   0x01    /* plaintext */
#define APPLICATION_EXT_FUNC_CODE 0xff    /* application block */

SGFileType *ESGOpenFileHandle(void * SGFileHandle, void *pFunWrite);

int ESGSpew(SGFileType * SGFile);
void ESGSetSGVersion(const char *Version);
int ESGPutScreenDesc(SGFileType * SGFile,
                      int SGWidth, int SGHeight, int SGColorRes,
                      int SGBackGround,
                      const SGColorMap * SGColorMap);
int ESGPutImageDesc(SGFileType * SGFile, int SGLeft, int SGTop,
                     int Width, int SGHeight, int SGInterlace,
                     const SGColorMap * SGColorMap);
int ESGPutLine(SGFileType * SGFile, SGPixelType * SGLine,
                int SGLineLen);
int ESGPutPixel(SGFileType * SGFile, SGPixelType SGPixel);
int ESGPutComment(SGFileType * SGFile, const char *SGComment);
int ESGPutExtensionFirst(SGFileType * SGFile, int SGExtCode,
                          int SGExtLen, const VoidPtr SGExtension);
int ESGPutExtensionNext(SGFileType * SGFile, int SGExtCode,
                         int SGExtLen, const VoidPtr SGExtension);
int ESGPutExtensionLast(SGFileType * SGFile, int SGExtCode,
                         int SGExtLen, const VoidPtr SGExtension);
int ESGPutExtension(SGFileType * SGFile, int SGExtCode, int SGExtLen,
                     const VoidPtr SGExtension);
int ESGPutCode(SGFileType * SGFile, int SGCodeSize,
                const SGByteType * SGCodeBlock);
int ESGPutCodeNext(SGFileType * SGFile,
                    const SGByteType * SGCodeBlock);
int ESGCloseFile(SGFileType * SGFile);

#define E_SG_ERR_OPEN_FAILED    1    /* And ESG possible errors. */
#define E_SG_ERR_WRITE_FAILED   2
#define E_SG_ERR_HAS_SCRN_DSCR  3
#define E_SG_ERR_HAS_IMAG_DSCR  4
#define E_SG_ERR_NO_COLOR_MAP   5
#define E_SG_ERR_DATA_TOO_BIG   6
#define E_SG_ERR_NOT_ENOUGH_MEM 7
#define E_SG_ERR_DISK_IS_FULL   8
#define E_SG_ERR_CLOSE_FAILED   9
#define E_SG_ERR_NOT_WRITEABLE  10

SGFileType *DSGOpenFileHandle(void * SGFileHandle, void * pSGFunRead);
int DSGSlurp(SGFileType * SGFile);

int DSGGetScreenDesc(SGFileType * SGFile);
int DSGGetRecordType(SGFileType * SGFile, SGRecordType * SGType);
int DSGGetImageDesc(SGFileType * SGFile);
int DSGGetLine(SGFileType * SGFile, SGPixelType * SGLine, int SGLineLen);
int DSGGetPixel(SGFileType * SGFile, SGPixelType SGPixel);
int DSGGetComment(SGFileType * SGFile, char *SGComment);
int DSGGetExtension(SGFileType * SGFile, int *SGExtCode,
                     SGByteType ** SGExtension);
int DSGGetExtensionNext(SGFileType * SGFile, SGByteType ** SGExtension);
int DSGGetCode(SGFileType * SGFile, int *SGCodeSize,
                SGByteType ** SGCodeBlock);
int DSGGetCodeNext(SGFileType * SGFile, SGByteType ** SGCodeBlock);
int DSGGetLZCodes(SGFileType * SGFile, int *SGCode);
int DSGCloseFile(SGFileType * SGFile);

#define D_GIF_ERR_OPEN_FAILED    101    /* And DSG possible errors. */
#define D_GIF_ERR_READ_FAILED    102
#define D_GIF_ERR_NOT_GIF_FILE   103
#define D_GIF_ERR_NO_SCRN_DSCR   104
#define D_GIF_ERR_NO_IMAG_DSCR   105
#define D_GIF_ERR_NO_COLOR_MAP   106
#define D_GIF_ERR_WRONG_RECORD   107
#define D_GIF_ERR_DATA_TOO_BIG   108
#define D_GIF_ERR_NOT_ENOUGH_MEM 109
#define D_GIF_ERR_CLOSE_FAILED   110
#define D_GIF_ERR_NOT_READABLE   111
#define D_GIF_ERR_IMAGE_DEFECT   112
#define D_GIF_ERR_EOF_TOO_SOON   113

/******************************************************************************
 * O.K., here are the routines from GIF_LIB file QUANTIZE.C.              
******************************************************************************/
int QuantizeBuffer(unsigned int Width, unsigned int Height,
                   int *ColorMapSize, SGByteType * RedInput,
                   SGByteType * GreenInput, SGByteType * BlueInput,
                   SGByteType * OutputBuffer,
                   SGPalette * OutputColorMap);

/******************************************************************************
 * O.K., here are the routines from GIF_LIB file QPRINTF.C.              
******************************************************************************/
extern int SGQuietPrint;

#ifdef HAVE_STDARG_H
    extern void SGQprintf(char *Format, ...);
#elif defined (HAVE_VARARGS_H)
    extern void SGQprintf();
#endif /* HAVE_STDARG_H */

/******************************************************************************
 * O.K., here are the routines from GIF_LIB file GIF_ERR.C.              
******************************************************************************/
//#ifndef _GBA_NO_FILEIO
//extern void PrintSGError(void);
//#endif /* _GBA_NO_FILEIO */
//extern int SGLastError(void);

/*****************************************************************************
 *
 * Everything below this point is new after version 1.2, supporting `slurp
 * mode' for doing I/O in two big belts with all the image-bashing in core.
 *
 *****************************************************************************/

/******************************************************************************
 * Color Map handling from ALLOCGIF.C                          
 *****************************************************************************/

extern SGColorMap *MakeMapObject(int ColorCount,
                                     const SGPalette * ColorMap);
extern void FreeMapObject(SGColorMap * Object);
extern SGColorMap *UnionColorMap(const SGColorMap * ColorIn1,
                                     const SGColorMap * ColorIn2,
                                     SGPixelType ColorTransIn2[]);
extern int BitSize(int n);

/******************************************************************************
 * Support for the in-core structures allocation (slurp mode).              
 *****************************************************************************/

/* This is the in-core version of an extension record */
typedef struct {
    int ByteCount;
    char *Bytes;    /* on malloc(3) heap */
    int Function;   /* Holds the type of the Extension block. */
} ExtensionBlock;

/* This holds an image header, its unpacked raster bits, and extensions */
typedef struct SavedImage {
    SGImageDesc ImageDesc;
    unsigned char *RasterBits;  /* on malloc(3) heap */
    int Function;   /* DEPRECATED: Use ExtensionBlocks[x].Function instead */
    int ExtensionBlockCount;
    ExtensionBlock *ExtensionBlocks;    /* on malloc(3) heap */
} SavedImage;

extern void ApplyTranslation(SavedImage * Image, SGPixelType Translation[]);
extern void MakeExtension(SavedImage * New, int Function);
extern int AddExtensionBlock(SavedImage * New, int Len,
                             unsigned char ExtData[]);
extern void FreeExtension(SavedImage * Image);
extern SavedImage *MakeSavedImage(SGFileType * SGFile,
                                  const SavedImage * CopyFrom);
extern void FreeSavedImages(SGFileType * SGFile);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _GIF_LIB_H */
