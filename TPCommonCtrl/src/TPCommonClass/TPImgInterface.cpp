#include "StdAfx.h"


CTPImgDecode::CTPImgDecode()
{

}
CTPImgDecode::~CTPImgDecode()
{

}

int CTPImgDecode::Decode(CString sFileNmae, BYTE *&pBuffer, int &iWidth, int &iHeight)
{
	//open
	HANDLE hJgpFile = CreateFile(sFileNmae, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(NULL == hJgpFile)
	{
		return 0;
	}

	JpegFile::GetHeadSize(hJgpFile, (UINT *)&iWidth, (UINT *)&iHeight);

	//getimg

	UINT uWidth =0;
	UINT uHeight =0;
	pBuffer = JpegFile::JpegFileToBGRA(hJgpFile,&uWidth,&uHeight); // SG: ֱ�ӵõ� BGRA ��ʽͼ��buf ��Ҫ�������ͷ�

	//close
	CloseHandle(hJgpFile);

	return 0;
}