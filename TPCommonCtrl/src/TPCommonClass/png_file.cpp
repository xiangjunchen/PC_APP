#include "stdafx.h"
// SG: PNG pSGImgResource->pPrivateData สนำร

#define PNG_FILE_EXIT
#ifdef PNG_FILE_EXIT
#include "TPImgInterface.h"


#include "zlib.h"
#include "png.h"

#ifdef _M_X64
#pragma comment (lib,"zlib64.lib")
#pragma comment (lib,"png64.lib")
#else
#if _MSC_VER >= 1500
#pragma comment (lib,"zlib2008.lib")
#pragma comment (lib,"png2008.lib")
#else
#pragma comment (lib,"zlib2003.lib")
#pragma comment (lib,"png2003.lib")
#endif
#endif


#endif