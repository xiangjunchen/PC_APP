#pragma  once

#include "sg_interface.h"
#include "sg_manual.h"
#include "sg_algorithm.h"
#include "RGBClusterInterface.h"
#include "TPAVTgaFile.h"
// �����ؼ�ֱ֡�ӵ�֡���
#define NX_GIF_GOP_SIZE 5
// һ�εõ���GOP��Ŀ
#define NX_GET_GOP_INFO_NUM_ONCE 20

typedef struct tag_NXGOPGifFrame
{
	int iFrameIndex; // GIF �ļ��ڵ�֡�������� 0 ��ʼ
	BYTE *pFrameData; // ��֡��������
	LARGE_INTEGER llFrameCursor; // ��֡���ݶ�ȡ��Ϻ��ļ�ָ��λ�ã��Ӵ�λ�ÿ�ʼ��ȡ��һ֡
	SGPalette framePalette[256]; // 256 * 3 ��֡��Ҫʹ�õĵ�ɫ��
	int iFrameImageWidth; // ��֡ͼ��Ŀ�Ⱥ͸߶ȡ�ע�����˿�����߼���Ļ�ģ������Ǵ�֡�ֲ�ͼ��Ŀ��
	int iFrameImageHeight;
	unsigned short usFrameDelayTime; // ��֡ͣ��ʱ��

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
	BYTE *pIndexImage; // �ֿ��ȡʱ����¼��ȡ������ͼ����ɫ��������
	int iFrameNum; // ��¼�� GIF ͼ�������֡��Ŀ
	LARGE_INTEGER llNXImageBeginPos; // �ļ�ͷ�Ѿ���ȡ��ϣ���ʼ��ȡ���ݣ�ÿ��ȡ֡����ʱ��ָ���ƶ������λ�� add by wgf@2010-5-26 for showtime
	unsigned short usFrameDelayTime; // �ոն�ȡ��ͼ�񲥷�ʱͣ��ʱ�䣬��λΪ 1/100s����ֵ���� 1 ��Ч add by wgf@2010-6-1
	BOOL bUseGifGop; // true - ʹ�� GOP ���ֶ� GIF ֡
	LPNXGOPGifFrame pNXGOPGifFrame; // GOP �ؼ�֡�γɵļ��ϡ�ע����һ�������� 0 ֡�����һ֡
	int iNXGOPGifFrameNum;
	NXGOPGifFrame stuNXPreGifFrame; // ��¼�ոն�ȡ��֡�������ݣ�Play ģʽʹ�ã���Ϊ����һ֡�ϳɵ�ǰһ֡
	int iAlreadyGetGOPNum; // �Ѿ��õ���GOP��Ŀ��GOP��Ϣ�Ļ�ȡ�����ö�εõ����Լ��ٵȴ�ʱ��
	CSGBufferFile *pBufferFile; // �޸�GIFͼƬʹ�û����д add by wgf@2010-11-15

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
