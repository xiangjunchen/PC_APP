//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: ���ٴ����еĹ��ܺ�����ӿ���.
//
// Created: ZhaoZhimeng @ 2010-05-24
//////////////////////////////////////////////////////////////////////////
#ifndef __I_NX_VIDEO_SPEED_FUNC_H__
#define __I_NX_VIDEO_SPEED_FUNC_H__

#include ".\NXBasic.h"
#include ".\INXVideoSpeedProc.h"

//////////////////////////////////////////////////////////////////////////
// ��ӳ�䶨�壨out = left * d + right * (1 - d)��
typedef struct tag_nxspeedfieldmap
{
	double					dInFramePos;		// ӳ�����ʵλ��
	int						nLeftFrameIndex;	// ��ӳ���Դ֡��
	NXVIDEO_FIELD_PLAY		eLeftFieldIndex;	// ��ӳ����Դ֡�еĳ���
	int						nRightFrameIndex;	// �ҳ�ӳ���Դ֡��
	NXVIDEO_FIELD_PLAY		eRightFieldIndex;	// �ҳ�ӳ����Դ֡�еĳ���
	double					dLeftFieldWeight;	// ��ӳ���Ȩ��
} NXSpeedFieldMap, *LPNXSPEEDFIELDMAP;
// ֡�ڰ���ӳ�䶨��
typedef struct tag_nxspeedmapdata
{
	int					nOutFrameIndex;			// ���֡��
	NXSpeedFieldMap		stuFirstFieldMap;		// ��һ��ӳ��
	NXSpeedFieldMap		stuSecondFieldMap;		// �ڶ���ӳ��
} NXSpeedMapData, *LPNXSPEEDMAPDATA;

//////////////////////////////////////////////////////////////////////////
// �ӿ�������
interface NOVTABLE INXVideoSpeedFunc
{
	// ���ò��仯�Ĵ���������������桢���ظ�ʽ������ģʽ��������ʱ��
	// ��1��ePixelFmt ���Ϊ NXVIDEO_PIXEL_FMT_YUV422 �� NXVIDEO_PIXEL_FMT_YUV420 ��ʽ��
	// Ҫ�����������ϸ��� Y һƬ��U һƬ��V һƬ˳��洢����ͬ����������� RAW_DATA��
	// ��ʱ nPitch ��ʾ Y һ��ռ�õ��ֽڿ�ȣ��� U/V ����ռ���ֽڿ��Ϊ Y ��һ�롣
	// ��2����ͬ��Ƶ��ʽ�ĳ��������в��죬SD50i��HD59.94i��HD50i �������Ȳ����� SD59.94i �򰴵׳��Ȳ���
	// ��3��nFrameWidth��nFrameHeight ������ȫ������ʵ�ʴ��������� SetProcArea �ӿ�����
	// ����		NXSPEED_PROC_RET_SUCCEED			�ɹ�
	//			NXSPEED_PROC_RET_INVALID_PARAM		���������Ч
	virtual int SetProcParam(int nFrameWidth,
							 int nFrameHeight,
							 NXVIDEO_PIXEL_FMT ePixelFmt,
							 NXSPEED_PROC_ARITH eProcArith,
							 bool bTopFieldFirst) = 0;
	// ע���˶������������Ҫ����ʱ���壨ֻ�����㷨Ϊ MotionAdaptiveCut �� MotionAdaptiveMix ʱ��Ҫ��
	// ���ڻᱻƵ��ʹ�ã������ȷ���ã������� N ���˶�����߳��ڲ��У�����
	// pFrameCB:
	// ��1���ɱ�ͬʱ��д��֡��ʱ�������Ϊ 4 * N��
	// ��2��ÿ֡����Ĵ�СΪ nFrameWidth * nFrameHeight * nPixelBytes��
	// �������Ϊ YUV420 ��ʽ��ת��Ϊ YUV422 ��ʽ���д����� YUV422 �ֽ� Y��U��V �ֿ�����
	// ��������ָ�ʽ����ʱ�����С��Ϊ nFrameWidth * nFrameHeight��
	// pCoefCB:
	// ��1���ɱ�ͬʱ��д��ϵ����ʱ�������Ϊ N��
	// ��2��ÿ������Ĵ�СΪ nFrameWidth * nPixelBytes * sizeof(short)����ϵ��������ʱ�� short ���ʹ��ڣ�
	// �������Ϊ YUV422��YUV420 ���ַ�Ƭ��ʽ���� Y��U��V �ֿ��������С��Ϊ nFrameWidth * sizeof(short)��
	// ����		NXSPEED_PROC_RET_SUCCEED			�ɹ�
	//			NXSPEED_PROC_RET_INVALID_PARAM		���������Ч
	virtual int RegMotionCalcShareCB(INXVideoShareBufferCB *pFrameCB,		// ֡��ʱ����
									 INXVideoShareBufferCB *pCoefCB) = 0;	// ϵ����ʱ����
	// ע�� YUV420 ��ʽ����任���м���ʱ���壨ֻ������Ϊ YUV420 ʱ����Ҫ��
	// ���ڻᱻƵ��ʹ�ã������ȷ���ã������� N ���˶�����̡߳�M ������߳��ڲ��У�����
	// ��1���ɱ�ͬʱ��д�Ļ������Ϊ N * 2 + M��
	// ��2��ÿ�黺��Ĵ�СΪ nFrameWidth * nFrameHeight��
	// ����		NXSPEED_PROC_RET_SUCCEED			�ɹ�
	//			NXSPEED_PROC_RET_INVALID_PARAM		���������Ч
	virtual int RegUV22MidShareCB(INXVideoShareBufferCB *pUV22CB) = 0;

	// ��ȡ����
	virtual int GetFrameWidth() = 0;
	virtual int GetFrameHeight() = 0;
	virtual int GetFieldHeight() = 0;
	virtual NXVIDEO_PIXEL_FMT GetPixelFmt() = 0;
	virtual NXSPEED_PROC_ARITH GetProcArith() = 0;
	// ���״̬
	virtual bool IsTopFieldFirst() = 0;		// �ж��Ƿ�Ϊ�����Ȳ�
	virtual bool IsStillArith() = 0;		// �ж��Ƿ�Ϊ��֡�㷨
	virtual bool IsNeedMotion() = 0;		// �ж��Ƿ���Ҫ�˶�����㷨
	virtual bool IsFieldArith() = 0;		// �ж��Ƿ�Ϊ���������㷨
	virtual bool IsReverseSpeed(double dSpeed) = 0;		// ����
	virtual bool IsSpeedValid(double dSpeed) = 0;	// �ж��ٶ��Ƿ���Ч

	//////////////////////////////////////////////////////////////////////////
	// ӳ�����
	// ����		NXSPEED_PROC_RET_SUCCEED			�ɹ�
	//			NXSPEED_PROC_RET_INVALID_PARAM		���������Ч
	//////////////////////////////////////////////////////////////////////////
	// �����������ӳ��
	// ��1�����ӳ��ṹ��ͬʱ��������һ֡����������ӳ����ʼ�� dInFramePosNext��
	// ��2���� bIntegralMode ����Ϊ true ʱ��ʾ���л�����ӳ����㣬ʹ�� dInFramePosStart
	// ��������ǰ�ٶ��ۼӳ���ǰ֡��ӳ�䣻Ϊ false ʱ��ֱ�Ӱ��򵥱��ٷ�ʽ���㡣
	virtual int CalcMapping(LPNXSPEEDMAPDATA pMapData,	// ӳ������ṹ
							double &dInFramePosStart,	// ��ǰ֡���ӳ���������ʼ��
							int nOutFrameIndex,			// ���֡�������
							int nInClipLength,			// ������֡��
							bool bMapField,				// ����ӳ���־
							bool bIntegralMode,			// �����ͼ���ģʽ
							double dSpeed) = 0;			// �ٶ�
	// �����ٶȵı仯����һ֡ӳ���������ʼλ�á��������֡���������µ������֡����
	// ��1���� dNewSpeed < -1e^-6 ʱ���� nOutLength = 0 && dInFramePosStart = nInClipLength ���ʾ���������֡����
	// ��2���� dNewSpeed > 1e^-6 ʱ���� nOutLength = 0 && dInFramePosStart = 0 ���ʾ���������֡����
	virtual int UpdateOutClipLength(int &nOutLength,			// �������֡��
									double dInFramePosStart,	// ��һ֡ӳ���������ʼλ��
									double dNewSpeed,			// ���ٶ�
									int nInClipLength) = 0;		// ������֡��

	//////////////////////////////////////////////////////////////////////////
	// �˶����
	// ��1��������֡�����Ҳ����֡��
	// ��2���㷨�ӿڲ���������ȫ��⡣
	// ��3���ڲ���Ҫ�Ĳ�����������ã�����桢�㷨���͡����ظ�ʽ������
	// ����		NXSPEED_PROC_RET_SUCCEED		�ɹ�
	//			NXSPEED_PROC_RET_FAILED			ʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual int MotionFrame(unsigned char *pucTopFrame,			// �������֡
							intptr_t nTopFramePitch,			// ���һ����ռ�õ��ֽ���
							unsigned char *pucBottomFrame,		// ����׳�֡
							intptr_t nBottomFramePitch,			// ���һ����ռ�õ��ֽ���
							unsigned char *pucCurrFrame,		// ���뵱ǰ֡
							intptr_t nCurrFramePitch,			// ����һ����ռ�õ��ֽ���
							unsigned char *pucPrevFrame,		// ����ǰһ֡
							intptr_t nPrevFramePitch,			// ����һ����ռ�õ��ֽ���
							int nLineProcBytes,					// һ�д�����ֽ���
							NXVIDEO_PIXEL_FMT ePixelFmtIn) = 0;	// ����֡�����ظ�ʽ

	//////////////////////////////////////////////////////////////////////////
	// ��Ⱦ���
	// ��1���㷨�ӿڲ���������ȫ��⡣
	// ��2���ڲ���Ҫ�Ĳ�����������ã�����桢�㷨���͡����ظ�ʽ������
	// ����		NXSPEED_PROC_RET_SUCCEED		�ɹ�
	//			NXSPEED_PROC_RET_FAILED			ʧ��
	//////////////////////////////////////////////////////////////////////////
	// ��֡��̬��Ⱦ��֧�� NXSPEED_PROC_ARITH_STILL_FIRST��NXSPEED_PROC_ARITH_STILL_SECOND��
	// NXSPEED_PROC_ARITH_STILL_FRAME �����㷨�������������ʧ�ܡ�
	virtual int StillArith(unsigned char *pucOutFrame,			// ���֡��ʼ
						   intptr_t nOutPitch,					// ���һ����ռ�õ��ֽ���
						   unsigned char *pucInFrame,			// ����֡��ʼ
						   intptr_t nInPitch,					// ����һ����ռ�õ��ֽ���
						   int nLineProcBytes,					// һ�д�����ֽ���
						   NXVIDEO_PIXEL_FMT ePixelFmtIn) = 0;	// ����֡�����ظ�ʽ
	// ���� FilterOff �� FieldCut ��Ⱦ
	// ��Ҫע�⣬���������ַ��Ϊ֡���ݵ���ʼ�㣬���ǳ����ݵ���ʼ��
	virtual int FilterOffFieldCut(unsigned char *pucOutFrame,	// ���֡��ʼ
								  intptr_t nOutPitch,			// ���һ����ռ�õ��ֽ���
								  bool bOutTopField,			// ��������־
								  unsigned char *pucInFrame,	// ����֡��ʼ
								  intptr_t nInPitch,			// ����һ����ռ�õ��ֽ���
								  bool bInMapTopField,			// ӳ��Ϊ���붥���ı�־
								  bool bInputShare,				// ����֡δ��ת����־
								  bool bFilterOff,				// FilterOff �㷨��־
								  int nLineProcBytes) = 0;		// һ�д�����ֽ���
	// ���� FieldMix ��Ⱦ
	// ��Ҫע�⣬���������ַ��Ϊ֡���ݵ���ʼ�㣬���ǳ����ݵ���ʼ��
	virtual int FieldMix(unsigned char *pucOutFrame,		// ���֡��ʼ
						 intptr_t nOutPitch,				// ���һ����ռ�õ��ֽ���
						 bool bOutTopField,					// ��������־
						 unsigned char *pucInLeftFrame,		// ������֡��ʼ
						 intptr_t nInLeftPitch,				// ����һ����ռ�õ��ֽ���
						 bool bInLeftTopField,				// ӳ��Ϊ���붥���ı�־
						 unsigned char *pucInRightFrame,	// ������֡��ʼ
						 intptr_t nInRightPitch,			// ����һ����ռ�õ��ֽ���
						 bool bInRightTopField,				// ӳ��Ϊ���붥���ı�־
						 bool bInputShare,					// ����֡δ��ת����־
						 double dLeftFieldWeight,			// ��Ȩ��
						 int nLineProcBytes) = 0;			// һ�д�����ֽ���
	// ��֡ FrameCut ��Ⱦ
	// ��Ҫע�⣬���������ַ��Ϊ֡���ݵ���ʼ�㣬���ǳ����ݵ���ʼ��
	// pucInPrevFrame ������ bReverseSpeed ����ֵΪ false ʱ�������ţ���Ч
	virtual int FrameCut(unsigned char *pucOutFrame,	// ���֡��ʼ
						 intptr_t nOutPitch,			// ���һ����ռ�õ��ֽ���
						 unsigned char *pucInFrame,		// ����֡��ʼ
						 intptr_t nInPitch,				// ����һ����ռ�õ��ֽ���
						 unsigned char *pucInPrevFrame,	// ����֡��ʼ
						 intptr_t nInPrevPitch,			// ����һ����ռ�õ��ֽ���
						 bool bReverseSpeed,			// ���ű�־
						 int nLineProcBytes) = 0;		// һ�д�����ֽ���
	// ����ʱ��֡ && ����ʱ���� FrameMix ��Ⱦ
	// ��Ҫע�⣬���������ַ��Ϊ֡���ݵ���ʼ�㣬���ǳ����ݵ���ʼ��
	// bOutTopField ������ bReverseSpeed ����ֵΪ false ʱ�������ţ���Ч
	virtual int FrameMix(unsigned char *pucOutFrame,		// ���֡��ʼ
						 intptr_t nOutPitch,				// ���һ����ռ�õ��ֽ���
						 bool bOutTopField,					// ��������־
						 unsigned char *pucInLeftFrame,		// ������֡��ʼ
						 intptr_t nInLeftPitch,				// ����һ����ռ�õ��ֽ���
						 unsigned char *pucInRightFrame,	// ������֡��ʼ
						 intptr_t nInRightPitch,			// ����һ����ռ�õ��ֽ���
						 bool bReverseSpeed,				// ���ű�־
						 double dLeftFieldWeight,			// ��֡Ȩ��
						 int nLineProcBytes) = 0;			// һ�д�����ֽ���
	// ���� MotionAdaptiveCut ��Ⱦ
	// ��Ҫע�⣬���������ַ��Ϊ֡���ݵ���ʼ�㣬���ǳ����ݵ���ʼ��
	virtual int MotionCut(unsigned char *pucOutFrame,	// ���֡��ʼ
						  intptr_t nOutPitch,			// ���һ����ռ�õ��ֽ���
						  bool bOutTopField,			// ��������־
						  unsigned char *pucInFrame,	// ����֡��ʼ
						  intptr_t nInPitch,			// ����һ����ռ�õ��ֽ���
						  int nLineProcBytes) = 0;		// һ�д�����ֽ���
	// ���� MotionAdaptiveMix ��Ⱦ
	// ��Ҫע�⣬���������ַ��Ϊ֡���ݵ���ʼ�㣬���ǳ����ݵ���ʼ��
	virtual int MotionMix(unsigned char *pucOutFrame,		// ���֡��ʼ
						  intptr_t nOutPitch,				// ���һ����ռ�õ��ֽ���
						  bool bOutTopField,				// ��������־
						  unsigned char *pucInLeftFrame,	// ������֡��ʼ
						  intptr_t nInLeftPitch,			// ����һ����ռ�õ��ֽ���
						  unsigned char *pucInRightFrame,	// ������֡��ʼ
						  intptr_t nInRightPitch,			// ����һ����ռ�õ��ֽ���
						  double dLeftFieldWeight,			// ��Ȩ��
						  int nLineProcBytes) = 0;			// һ�д�����ֽ���
	// ����ʵ�ʴ���������Ϣ
	// ȫ��������Ϣ�� SetProcParam �ӿ�����
	// ����		NXSPEED_PROC_RET_SUCCEED			�ɹ�
	//			NXSPEED_PROC_RET_FAILED				ʧ�ܣ�����������Ч
	virtual int SetProcArea(IN int nProcX,				// ������㣬�����ͼ�����Ͻ�
							IN int nProcY,
							IN int nProcWidth,			// ������
							IN int nProcHeight) = 0;
	virtual int GetProcArea(IN int& nProcX,				// ������㣬�����ͼ�����Ͻ�
							IN int& nProcY,
							IN int& nProcWidth,			// ������
							IN int& nProcHeight) = 0;
	// ȥ�����㷨���ͣ�Ĭ��Ϊ NXVIDEO_DEINTERLACE_TAPS_4
	virtual void SetDeinterlaceArith(NXVIDEO_DEINTERLACE_ARITH eArith) = 0;
	virtual NXVIDEO_DEINTERLACE_ARITH GetDeinterlaceArith() const = 0;
};


#endif		// #ifndef __I_NX_VIDEO_SPEED_FUNC_H__
//////////////////////////////////////////////////////////////////////////
// End of file.
