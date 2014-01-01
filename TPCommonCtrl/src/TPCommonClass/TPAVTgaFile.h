#pragma once
typedef void* HAVTGAFILE;

typedef struct tag_tagData
{
	HAVTGAFILE hFile;
	int			iX;
	int			iY;
	int        iWidth;
	int        iHeight;
	int        iDepth;
	BYTE       * pBuffer;
	TCHAR      file_name[MAX_PATH];
	BYTE*		pCustomData;//这个不需要delete，只要hFile还在打开，即可：）
	long		lCustomDataSize;
	BOOL		bNewCustomBuffer;//是否是新New的空间ljj
	LONGLONG	llFileSize;
	BOOL		bExistFile;//是否是打开已经存在的File，add by ljj @ 2009-3-4
	tag_tagData()
	{
		hFile=NULL;
		iX = 0;
		iY = 0;
		iWidth=0;
		iHeight=0;
		iDepth=0;
		pBuffer=NULL;
		ZeroMemory (file_name, sizeof (TCHAR) * MAX_PATH);
		pCustomData = NULL;
		lCustomDataSize = 0;
		bNewCustomBuffer = FALSE;
		llFileSize = 0;
		bExistFile = FALSE;
	}
}TGADATA;
#define TP_IMG_FILE_READ  0x00000001
#define TP_IMG_FILE_WRITE 0x00000002
#define TP_IMG_FILE_CREATE 0x00000004
//#define TP_TGA_MAX_CUSTOMDATASIZE 255


int TPAVTgaFileOpen(HAVTGAFILE& hFile,TCHAR* ptszFileName,DWORD dwOpenType );
int TPAVTgaFileOpen(HAVTGAFILE& hFile,TCHAR* ptszFileName );
int TPAVTgaFileClose(HAVTGAFILE hFile);
int TPAVTgaFileGetImageInfo(HAVTGAFILE hFile,int& iX, int& iY, int& iWidth,int& iHeight,int &iDepth,BYTE*& pBuf, long& lBufSize);//add X and Y location by ljj @ 2009-2-10 
int TPAVTgaFileGetImageInfo(HAVTGAFILE hFile,int& iWidth,int& iHeight,int &iDepth);//add X and Y location by ljj @ 2009-2-10 
int TPAVTgaFileGetImageData(HAVTGAFILE hFile,BYTE* pData);
int NXAVTgaFilePrepareGetImageData(HAVTGAFILE hFile,BYTE* pData, BOOL &bLeftBottom); // 为分块读取图像数据做准备
int NXAVTgaFileGetImageData(HAVTGAFILE hFile, int iBeginRow, int iEndRow); // 分块读取图像数据
bool NXImageHaveAlpha(HAVTGAFILE hFile); // 返回此图像是否含有 alpha
int TPAVTgaFilePutImageData (HAVTGAFILE hFile, int iX, int iY, int iWidth, int iHeight, int iDepth, BYTE *pData);//add X and Y location by ljj @ 2009-2-10
int TPAVTgaFilePutImageData (HAVTGAFILE hFile, int iX, int iY, int iWidth, int iHeight, int iDepth, BYTE *pData, BYTE* pCustomData, long lCustomDataSize);//add X and Y location by ljj @ 2009-2-10
int TPAVTgaFileUpdateCustomData (HAVTGAFILE hFile,BYTE* pCustomData, long lCustomDataSize);//add by ljj @ 2009-3-4
bool NXTgaSupportBlockRead(HAVTGAFILE hFile); // 返回此图像是否可以分块读取 by ljj@2011-4-28



// 拷贝一行 Buffer
LRESULT TP_MemcpyLine(OUT void *des,		// 目标
					  IN void *src,		// 源
					  IN int nlen);		// 字节长度
// 以字节为单位设置一行 Buffer
LRESULT TP_MemsetChar(OUT void *des,		// 目标
					  IN char value,		// 字节值
					  IN int nlen);		// 字节长度

// 一行 Buffer 的双字设值操作
LRESULT TP_MemsetDWORD(OUT DWORD *des,		// 目标
					   IN DWORD value,		// 源
					   IN int nsize);		// 双字的个数


void TPCS_24_to_32_Frame(OUT BYTE* pDes32,				// 输出（存储方式 X0 X1 X2 A X0 X1 X2 A ……）
						 IN int nDesPitch,
						 IN BYTE* pSrc24,               // 24 位源输入（存储方式 X0 X1 X2 X0 X1 X2 ……）
						 IN int nSrc24Pitch,            // 源一行的字节宽度（必须为 4 的倍数）
						 IN BYTE* pSrcAlpha,            // 源 Alpha 输入（存储方式 A0 A1 A2 A3 ……）（如果输入为 NULL 则直接填 0xff）
						 IN int nSrcAlphaPitch,
						 IN int nWidth,                 // 源图大小
						 IN int nHeight,
						 IN BOOL bSrc24Revert,          // 图像是否行翻转标志
						 IN BOOL bSrcAlphaRevert);      // Alpha 是否行翻转标志
void TPCS_24_to_32_Split(OUT BYTE* pDes32First,         // 第一场输出（存储方式 X0 X1 X2 A X0 X1 X2 A ……）
						 IN int nFirstPitch,
						 OUT BYTE* pDes32Second,        // 第二场输出（存储方式 X0 X1 X2 A X0 X1 X2 A ……）
						 IN int nSecondPitch,
						 IN BYTE* pSrc24,               // 24 位源输入（存储方式 X0 X1 X2 X0 X1 X2 ……）
						 IN int nSrc24Pitch,            // 源一行的字节宽度（必须为 4 的倍数）
						 IN BYTE* pSrcAlpha,            // 源 Alpha 输入（存储方式 A0 A1 A2 A3 ……）（如果输入为 NULL 则直接填 0xff）
						 IN int nSrcAlphaPitch,
						 IN int nWidth,                 // 源图大小
						 IN int nHeight,
						 IN BOOL bSrc24Revert,          // 图像是否行翻转标志
						 IN BOOL bSrcAlphaRevert);      // Alpha 是否行翻转标志
// 翻转
BOOL TP_OverTurnSelf(IN OUT void* pBuf,    // 内存
					 IN int nPitch,        // 一行字节宽度（四字节对齐）
					 IN int nHeight)    ;   // 高度

