//////////////////////////////////////////////////
// Brief: 	从RGB中抽取256色
// Name:	WangGaofei
// Date:	2013/09/22
//////////////////////////////////////////////////
#pragma once

// 调色板数据
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
*	功能： 将送入的BGR颜色量化为256种颜色，返回调色板和调色板索引数据。																																					*
*	参数： pGray -- 调色板索引数据。																																																						*
*				stPalette -- 调色板数据。																																																							*
*				iPaletteSize -- 调色板真实大小。																																																			*
*				pBGR -- BGR数据。B在低地址，R在高地址。																																														*
*				iWidth -- BGR数据一行像素数目。																																																			*
*				iHeight -- BGR数据一列像素数目。																																																			*
*				iPitch -- BGR数据一行字节数目。iPitch >= iWidth * 3																																											*
* 返回值： TRUE -- 将BGR数据成功量化为256种颜色数据，并保存在pGray； FALSE -- 参数有误。																												*
* 【注】： 此接口不对送入的指针型参数做new/delete操作，仅适用或者填充指针指向的内存数据。																												*
**************************************************************************************************************************************************************/
BOOL NXRGBCluster(OUT BYTE *pGray, OUT NXOCPalette stPalette[256], OUT int &iPaletteSize, IN BYTE *pBGR, IN int iWidth, IN int iHeight, IN int iPitch);

// end of file