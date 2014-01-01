//////////////////////////////////////////////////
// Brief: 	��RGB�г�ȡ256ɫ
// Name:	WangGaofei
// Date:	2013/09/22
//////////////////////////////////////////////////
#pragma once

// ��ɫ������
typedef struct tag_NXOCPalette
{
	BYTE byteR;
	BYTE byteG;
	BYTE byteB;

	tag_NXOCPalette()
	{
		memset(this, 0, sizeof(*this));
	}
}NXOCPalette;

/**************************************************************************************************************************************************************
*	���ܣ� �������BGR��ɫ����Ϊ256����ɫ�����ص�ɫ��͵�ɫ���������ݡ�																																					*
*	������ pGray -- ��ɫ���������ݡ�																																																						*
*				stPalette -- ��ɫ�����ݡ�																																																							*
*				iPaletteSize -- ��ɫ����ʵ��С��																																																			*
*				pBGR -- BGR���ݡ�B�ڵ͵�ַ��R�ڸߵ�ַ��																																														*
*				iWidth -- BGR����һ��������Ŀ��																																																			*
*				iHeight -- BGR����һ��������Ŀ��																																																			*
*				iPitch -- BGR����һ���ֽ���Ŀ��iPitch >= iWidth * 3																																											*
* ����ֵ�� TRUE -- ��BGR���ݳɹ�����Ϊ256����ɫ���ݣ���������pGray�� FALSE -- ��������																												*
* ��ע���� �˽ӿڲ��������ָ���Ͳ�����new/delete�����������û������ָ��ָ����ڴ����ݡ�																												*
**************************************************************************************************************************************************************/
BOOL NXRGBCluster(OUT BYTE *pGray, OUT NXOCPalette stPalette[256], OUT int &iPaletteSize, IN BYTE *pBGR, IN int iWidth, IN int iHeight, IN int iPitch);

// end of file