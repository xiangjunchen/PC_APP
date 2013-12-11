//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: ��Ƶ������ص���ӿ�������.
//
// Created: ZhaoZhimeng @ 2008-02-16
//////////////////////////////////////////////////////////////////////////
#ifndef __I_NX_VIDEO_SHARE_BUFFER_CB_H__
#define __I_NX_VIDEO_SHARE_BUFFER_CB_H__


//////////////////////////////////////////////////////////////////////////
// ��������

#ifndef IN
#define IN
#endif		// #ifndef IN
#ifndef OUT
#define OUT
#endif		// #ifndef OUT


//////////////////////////////////////////////////////////////////////////
// ����ֵ����
enum NXVIDEO_SHARE_RET
{
	NXVIDEO_SHARE_RET_SUCCEED	= 1,
	NXVIDEO_SHARE_RET_FAILED	= 0,

	NXVIDEO_SHARE_RET_INPUT_END = 3,		// �� LockReadUnit �з��ظ�ֵ�����������
};


//////////////////////////////////////////////////////////////////////////
// ������ص��ӿ�������
// ��1��Ϊ��ʹ�ûص��ߵ�ʵ�ַ��㣬����ȡ�ӿڶ���д��ӿڶԷ���һ���ص��ӿ����н���������
// ��һ������£�һ��������ص��ӿڶ���ֻ����һ�Խӿڱ�ʹ�õ���
// ��2����ȡ�ӿڶ���д��ӿڶ����໥���������Խӿڣ�����֮��û���κε���ϵ��
interface INXVideoShareBufferCB
{
    // ������Ҫ�����Ŀɶ�ȡ���嵥Ԫ����
	// ��1��������ص��ڴ��������»ᱻ˳���ʹ�ã���������������ȳ������ԡ�
	// ��2���ڶ�ȡĳ֡ nFrameIndex ����ʱ����ǰ��Ҫ�������� (nFrameIndex - nReserved) ֡���壬
	// ��ǰ�Ļ�������Ա�������
	// ��3���� nReserved = -1 ʱ����ȡ�����֡�Ž���������ġ�
	// ����		NXVIDEO_SHARE_RET_SUCCEED		�ɹ�
	//			NXVIDEO_SHARE_RET_FAILED		ʧ��
    virtual int SetReadReservedUnits(IN int nReserved) = 0;
	// ��ȡ�ɶ�ȡ��ĳ֡����
	// ����		NXVIDEO_SHARE_RET_SUCCEED		�ɹ�
	//			NXVIDEO_SHARE_RET_FAILED		ʧ��
	//			NXVIDEO_SHARE_RET_INPUT_END		�������
    virtual int LockReadUnit(OUT unsigned char* &pucBuf,	// ֡������ʼ��ַ
                             OUT int &nPitch,				// ֡����һ����ռ�õ��ֽ���
                             IN int nFrameIndex) = 0;		// ֡��
    virtual int UnlockReadUnit(IN unsigned char *pucBuf,	// ֡������ʼ��ַ
                               IN int nFrameIndex) = 0;		// ֡��
	// ��ȡ��д���ĳ֡����
	// ��1��LockWriteUnit �ӿ��е� nFrameIndex < 0 ʱ����ʾ��ȡ����һ����д��Ļ��塣
	// �� nFrameIndex >= 0 ʱ���ص�ʵ���߿�����������������Ƿ�ʹ�øò�����
	// ��2��UnlockWriteUnit �ӿ��е� nFrameIndex < 0 ʱ����ʾд��֡��Ч��
	// ����		NXVIDEO_SHARE_RET_SUCCEED		�ɹ�
	//			NXVIDEO_SHARE_RET_FAILED		ʧ��
    virtual int LockWriteUnit(OUT unsigned char* &pucBuf,	// ֡������ʼ��ַ
							  OUT int &nPitch,				// ֡����һ����ռ�õ��ֽ���
                              IN int nFrameIndex) = 0;		// ֡��
    virtual int UnlockWriteUnit(IN unsigned char *pucBuf,	// ֡������ʼ��ַ
                                IN int nFrameIndex) = 0;	// ֡��
};


#endif		// #ifndef __I_NX_VIDEO_SHARE_BUFFER_CB_H__
//////////////////////////////////////////////////////////////////////////
// End of file.