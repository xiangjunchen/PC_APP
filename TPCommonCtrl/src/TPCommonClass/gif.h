#pragma  once

#include "sg_interface.h"
#include "sg_manual.h"
#include "sg_algorithm.h"
#include "RGBClusterInterface.h"
#include "TPAVTgaFile.h"
// 两个关键帧直接的帧间隔
#define NX_GIF_GOP_SIZE 5
// 一次得到的GOP数目
#define NX_GET_GOP_INFO_NUM_ONCE 20

typedef struct tag_NXGOPGifFrame
{
	int iFrameIndex; // GIF 文件内的帧索引，从 0 开始
	BYTE *pFrameData; // 此帧索引数据
	LARGE_INTEGER llFrameCursor; // 此帧数据读取完毕后文件指针位置，从此位置开始读取下一帧
	SGPalette framePalette[256]; // 256 * 3 此帧需要使用的调色板
	int iFrameImageWidth; // 此帧图像的宽度和高度【注】：此宽高是逻辑屏幕的，并不是此帧局部图像的宽高
	int iFrameImageHeight;
	unsigned short usFrameDelayTime; // 此帧停留时间

	tag_NXGOPGifFrame()
	{
		iFrameIndex = -1;
		pFrameData = NULL;
		llFrameCursor.QuadPart = 0;
		TP_MemsetChar(framePalette, 0, sizeof(SGPalette) * 256);
		iFrameImageHeight = 0;
		iFrameImageWidth = 0;
		usFrameDelayTime = 0;
	}
}NXGOPGifFrame, *LPNXGOPGifFrame;
typedef struct tag_SGGIFFileData
{
	SGFileType* pGifFile;
	HANDLE hGifFile;
	BOOL  bEncode;
	BYTE *pIndexImage; // 分块读取时，记录读取的整个图像颜色索引数据
	int iFrameNum; // 记录此 GIF 图像包含的帧数目
	LARGE_INTEGER llNXImageBeginPos; // 文件头已经读取完毕，开始读取数据，每次取帧数据时，指针移动到这个位置 add by wgf@2010-5-26 for showtime
	unsigned short usFrameDelayTime; // 刚刚读取的图像播放时停留时间，单位为 1/100s，此值大于 1 有效 add by wgf@2010-6-1
	BOOL bUseGifGop; // true - 使用 GOP 来分段 GIF 帧
	LPNXGOPGifFrame pNXGOPGifFrame; // GOP 关键帧形成的集合【注】：一定包含第 0 帧和最后一帧
	int iNXGOPGifFrameNum;
	NXGOPGifFrame stuNXPreGifFrame; // 记录刚刚读取的帧索引数据，Play 模式使用，作为和下一帧合成的前一帧
	int iAlreadyGetGOPNum; // 已经得到的GOP数目，GOP信息的获取，采用多次得到，以减少等待时间
	CSGBufferFile *pBufferFile; // 修改GIF图片使用缓冲读写 add by wgf@2010-11-15

	tag_SGGIFFileData(){
		pGifFile = NULL;
		bEncode = FALSE;
		hGifFile = NULL;
		pIndexImage = NULL;
		iFrameNum = 0;
		llNXImageBeginPos.QuadPart = 0;
		usFrameDelayTime = 1;
		bUseGifGop = FALSE;
		pNXGOPGifFrame= NULL;
		iNXGOPGifFrameNum = 0;
		iAlreadyGetGOPNum = 0;
		pBufferFile = NULL;
	}
}SGGIFFileData;
