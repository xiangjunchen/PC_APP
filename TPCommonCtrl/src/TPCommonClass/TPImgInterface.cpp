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
	pBuffer = JpegFile::JpegFileToBGRA(hJgpFile,&uWidth,&uHeight); // SG: 直接得到 BGRA 格式图像，buf 需要调用者释放

	//close
	CloseHandle(hJgpFile);

	return 0;
}