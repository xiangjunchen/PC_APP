/********************************************************************
	Copyright (c) 2004, Sobey���� ����������
	All rights reserved.
	����ʱ��:	2004��8��3��   10:11
	�ļ���: 	g:\e7\chenyufeng_desktop_cg\desktopsobey\tpcommondata\src\tpbufferoperate\tpmaskoperateapi.h
	��ǰ�汾��	1.0
	����:		�����
	ժҪ:		��ģ����������
*********************************************************************/
#ifndef __TP_MASK_OPERATE_INC__
#define __TP_MASK_OPERATE_INC__

// ��MASK���:DA=DA-SA,�������
LRESULT CALLBACK TP_SubMask(IN OUT BYTE* pbyDestMask,
							IN int nDestX,
							IN int nDestY,
							IN int nDestMaskWidth,
							IN int nDestMaskHeight,
							IN BYTE* pbySrcMask,
							IN int nSrcX,
							IN int nSrcY,
							IN int nSrcMaskWidth,
							IN int nSrcMaskHeight,
							IN int nOperateWidth,	// Ҫ�����Ŀ���
							IN int nOperateHeight,
							IN int nType);			// ���Ͷȴ�������

#endif // __TP_MASK_OPERATE_INC__