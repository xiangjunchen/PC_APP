/********************************************************************
	Copyright (c) 2004, Sobey���� ����������
	All rights reserved.
	����ʱ��:	2004��3��4��   15:21
	�ļ���: 	c:\ndsrc\chenyufeng_desktop_3\devvob\sobeydesktop\src\tpcommon\tpbufferoperate\tpscrollscreen_bufferoperate.h
	��ǰ�汾��	1.0
	����:		�����
	ժҪ:		������������������
*********************************************************************/
#ifndef __TP_SCROLLSCREEN_BUFFER_OPERATE_INC__
#define __TP_SCROLLSCREEN_BUFFER_OPERATE_INC__

// ��������ģʽ
#define TP_SCROLLSCREEN_UPMOVE	0	// ����
#define TP_SCROLLSCREEN_DOWNMOVE 1// ����
#define TP_SCROLLSCREEN_LEFTMOVE 2	// ����
#define TP_SCROLLSCREEN_RIGHTMOVE 3 // ����

// �����Ϲ�
BOOL CALLBACK TPBuffer_ScrollScreen_Up(DWORD* pdwDestBuffer, 
							  int nDestX,
							  int nDestY,
							  int nDestWidth,
							  int nDestHeight,
							  int nDestBufferWidth,
							  int nDestBufferHeight,
							  DWORD* pdwSrcBuffer,
							  int nSrcX,
							  int nSrcY,
							  int nSrcWidth,
							  int nSrcHeight,
							  int nSrcBufferWidth,
							  int nSrcBufferHeight,
							  int nParity,				// ��ż��ģʽ
							  float fPower,			 // ������ռȨ��
							  int nFadeIn,
							  int nFadeOut);
// �������
BOOL CALLBACK TPBuffer_ScrollScreen_Left(DWORD* pdwDestBuffer, 
								int nDestX,
								int nDestY,
								int nDestWidth,
								int nDestHeight,
								int nDestBufferWidth,
								int nDestBufferHeight,
								DWORD* pdwSrcBuffer,
								int nSrcX,
								int nSrcY,
								int nSrcWidth,
								int nSrcHeight,
								int nSrcBufferWidth,
								int nSrcBufferHeight,
								int nParity,				// ��ż��ģʽ
								float fPower,			 // ������ռȨ��
								int nFadeIn,
								int nFadeOut);
// �����¹�
BOOL CALLBACK TPBuffer_ScrollScreen_Down(DWORD* pdwDestBuffer, 
								int nDestX,
								int nDestY,
								int nDestWidth,
								int nDestHeight,
								int nDestBufferWidth,
								int nDestBufferHeight,
								DWORD* pdwSrcBuffer,
								int nSrcX,
								int nSrcY,
								int nSrcWidth,
								int nSrcHeight,
								int nSrcBufferWidth,
								int nSrcBufferHeight,
								int nParity,				// ��ż��ģʽ
								float fPower,			 // ������ռȨ��
								int nFadeIn,
								int nFadeOut);
// �����ҹ�
BOOL CALLBACK TPBuffer_ScrollScreen_Right(DWORD* pdwDestBuffer, 
								 int nDestX,
								 int nDestY,
								 int nDestWidth,
								 int nDestHeight,
								 int nDestBufferWidth,
								 int nDestBufferHeight,
								 DWORD* pdwSrcBuffer,
								 int nSrcX,
								 int nSrcY,
								 int nSrcWidth,
								 int nSrcHeight,
								 int nSrcBufferWidth,
								 int nSrcBufferHeight,
								 int nParity,				// ��ż��ģʽ
								 float fPower,			 // ������ռȨ��
								 int nFadeIn,
								 int nFadeOut);
// ������BitBlt����
LRESULT CALLBACK TP_SCROLLSCREEN_BitBlt(
										IN OUT DWORD* pdwDestBuffer, 
										IN int nDestX,
										IN int nDestY,
										IN int nDestWidth,
										IN int nDestHeight,
										IN int nDestBufferWidth,
										IN int nDestBufferHeight,
										IN DWORD* pdwSrcBuffer,
										IN int nSrcX,
										IN int nSrcY,
										IN int nSrcWidth,
										IN int nSrcHeight,
										IN int nSrcBufferWidth,
										IN int nSrcBufferHeight,
										IN int nMix,			// ���ģʽ
										IN int nParity,		// ��ż��ģʽ
										IN int nScrollType,  // ��������
										IN float fPower,		// ������ռȨ�� 
										IN int nFadeIn,
										IN int nFadeOut);

#endif // __TP_SCROLLSCREEN_BUFFER_OPERATE_INC__