//////////////////////////////////////////////////////////////////////////
// �������ڴ�����ӿ� SSE �Ż�
// �����ߣ���־��
//////////////////////////////////////////////////////////////////////////
#ifndef __TP_BUFFER_OPERATE_SSE_INC__
#define __TP_BUFFER_OPERATE_SSE_INC__

//////////////////////////////////////////////////////////////////////////
// BGRA 32 λɫ Buffer Copy �ӿ�
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_Copy_SSE(OUT DWORD* pDest,			// Ŀ�� Buffer
					 IN int nDestX,				// Ŀ����ʼ��
					 IN int nDestY,
					 IN int nDestWidth,			// Ŀ�� Mix ��С
					 IN int nDestHeight,
					 IN int nDestBufferWidth,	// Ŀ�� Buffer ��С
					 IN int nDestBufferHeight,
					 IN DWORD* pSrc,			// Դ Buffer
					 IN int nSrcX,				// Դ��ʼ��
					 IN int nSrcY,
					 IN int nSrcWidth,			// Դ Mix ��С
					 IN int nSrcHeight,
					 IN int nSrcBufferWidth,	// Դ Buffer ��С
					 IN int nSrcBufferHeight,
					 IN int nParity);			// ��ż������

//////////////////////////////////////////////////////////////////////////
// BGRA 32 λɫ Buffer Mix �ӿ�
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_Mix_SSE(IN OUT DWORD* pDest,		// Ŀ�� Buffer
					IN int nDestX,				// Ŀ����ʼ��
					IN int nDestY,
					IN int nDestWidth,			// Ŀ�� Mix ��С
					IN int nDestHeight,
					IN int nDestBufferWidth,	// Ŀ�� Buffer ��С
					IN int nDestBufferHeight,
					IN DWORD* pSrc,				// Դ Buffer
					IN int nSrcX,				// Դ��ʼ��
					IN int nSrcY,
					IN int nSrcWidth,			// Դ Mix ��С
					IN int nSrcHeight,
					IN int nSrcBufferWidth,		// Դ Buffer ��С
					IN int nSrcBufferHeight,
					IN int nParity);			// ��ż������

//////////////////////////////////////////////////////////////////////////
// BGRA 32 λɫ Buffer NotMix �ӿ�
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_NotMix_SSE(IN OUT DWORD* pDest,			// Ŀ�� Buffer
					   IN int nDestX,				// Ŀ����ʼ��
					   IN int nDestY,
					   IN int nDestWidth,			// Ŀ�� Mix ��С
					   IN int nDestHeight,
					   IN int nDestBufferWidth,		// Ŀ�� Buffer ��С
					   IN int nDestBufferHeight,
					   IN DWORD* pSrc,				// Դ Buffer
					   IN int nSrcX,				// Դ��ʼ��
					   IN int nSrcY,
					   IN int nSrcWidth,			// Դ Mix ��С
					   IN int nSrcHeight,
					   IN int nSrcBufferWidth,		// Դ Buffer ��С
					   IN int nSrcBufferHeight,
					   IN int nParity);				// ��ż������

//////////////////////////////////////////////////////////////////////////
// BGRA 32 λɫ Buffer ˫���Բ�ֵ���ſ���
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_BLStretchCopy_SSE(OUT DWORD* pDest,			// Ŀ�� Buffer
							  IN int nDestX,			// Ŀ�����
							  IN int nDestY,
							  IN int nDestWidth,		// Ŀ����������
							  IN int nDestHeight,
							  IN int nDestBufferWidth,	// Ŀ�� Buffer ��С
							  IN int nDestBufferHeight,
							  IN DWORD* pSrc,			// Դ Buffer
							  IN int nSrcX,				// Դ���
							  IN int nSrcY,
							  IN int nSrcWidth,			// Դ��������
							  IN int nSrcHeight,
							  IN int nSrcBufferWidth,	// Դ Buffer ��С
							  IN int nSrcBufferHeight,
							  IN int nParity);			// ��ż������

//////////////////////////////////////////////////////////////////////////
// BGRA 32 λɫ Buffer ��Ȩƽ����ֵ���ſ���
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_MeanStretchCopy_SSE(OUT DWORD* pDest,			// Ŀ�� Buffer
								IN int nDestX,				// Ŀ�����
								IN int nDestY,
								IN int nDestWidth,			// Ŀ����������
								IN int nDestHeight,
								IN int nDestBufferWidth,	// Ŀ�� Buffer ��С
								IN int nDestBufferHeight,
								IN DWORD* pSrc,				// Դ Buffer
								IN int nSrcX,				// Դ���
								IN int nSrcY,
								IN int nSrcWidth,			// Դ��������
								IN int nSrcHeight,
								IN int nSrcBufferWidth,		// Դ Buffer ��С
								IN int nSrcBufferHeight,
								IN int nParity);			// ��ż������

//////////////////////////////////////////////////////////////////////////
// BGRA 32 λɫ Buffer ˫���Բ�ֵ���� Mix
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_BLStretchMix_SSE(OUT DWORD* pDest,			// Ŀ�� Buffer
							 IN int nDestX,				// Ŀ�����
							 IN int nDestY,
							 IN int nDestWidth,			// Ŀ����������
							 IN int nDestHeight,
							 IN int nDestBufferWidth,	// Ŀ�� Buffer ��С
							 IN int nDestBufferHeight,
							 IN DWORD* pSrc,			// Դ Buffer
							 IN int nSrcX,				// Դ���
							 IN int nSrcY,
							 IN int nSrcWidth,			// Դ��������
							 IN int nSrcHeight,
							 IN int nSrcBufferWidth,	// Դ Buffer ��С
							 IN int nSrcBufferHeight,
							 IN int nParity);			// ��ż������

//////////////////////////////////////////////////////////////////////////
// BGRA 32 λɫ Buffer ˫���Բ�ֵ���� NotMix
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_BLStretchNotMix_SSE(OUT DWORD* pDest,			// Ŀ�� Buffer
								IN int nDestX,				// Ŀ�����
								IN int nDestY,
								IN int nDestWidth,			// Ŀ����������
								IN int nDestHeight,
								IN int nDestBufferWidth,	// Ŀ�� Buffer ��С
								IN int nDestBufferHeight,
								IN DWORD* pSrc,				// Դ Buffer
								IN int nSrcX,				// Դ���
								IN int nSrcY,
								IN int nSrcWidth,			// Դ��������
								IN int nSrcHeight,
								IN int nSrcBufferWidth,		// Դ Buffer ��С
								IN int nSrcBufferHeight,
								IN int nParity);			// ��ż������

//////////////////////////////////////////////////////////////////////////
// BGRA 32 λɫ Buffer ��Ȩƽ����ֵ���� Mix
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_MeanStretchMix_SSE(OUT DWORD* pDest,			// Ŀ�� Buffer
							   IN int nDestX,				// Ŀ�����
							   IN int nDestY,
							   IN int nDestWidth,			// Ŀ����������
							   IN int nDestHeight,
							   IN int nDestBufferWidth,		// Ŀ�� Buffer ��С
							   IN int nDestBufferHeight,
							   IN DWORD* pSrc,				// Դ Buffer
							   IN int nSrcX,				// Դ���
							   IN int nSrcY,
							   IN int nSrcWidth,			// Դ��������
							   IN int nSrcHeight,
							   IN int nSrcBufferWidth,		// Դ Buffer ��С
							   IN int nSrcBufferHeight,
							   IN int nParity);				// ��ż������

//////////////////////////////////////////////////////////////////////////
// BGRA 32 λɫ Buffer ��Ȩƽ����ֵ���� Mix
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_MeanStretchNotMix_SSE(OUT DWORD* pDest,				// Ŀ�� Buffer
								  IN int nDestX,				// Ŀ�����
								  IN int nDestY,
								  IN int nDestWidth,			// Ŀ����������
								  IN int nDestHeight,
								  IN int nDestBufferWidth,		// Ŀ�� Buffer ��С
								  IN int nDestBufferHeight,
								  IN DWORD* pSrc,				// Դ Buffer
								  IN int nSrcX,					// Դ���
								  IN int nSrcY,
								  IN int nSrcWidth,				// Դ��������
								  IN int nSrcHeight,
								  IN int nSrcBufferWidth,		// Դ Buffer ��С
								  IN int nSrcBufferHeight,
								  IN int nParity);				// ��ż������

#endif
//////////////////////////////////////////////////////////////////////////
// �ļ�����