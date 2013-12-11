#ifndef __TP_BUFFEROPERATE_INC__
#define __TP_BUFFEROPERATE_INC__

#include "TPBufferOperateDef.h"

#include "TPBufferOperateAPI.h"
#include "TPScrollScreen_BufferOperate.h"
#include "TPMaskOperateAPI.h"
#include "TPMemoryOperateAPI.h"
#include "TPBufferOperateSSE2.h"
#include "TPColorSpaceConv.h"

#ifdef _UNICODE
#ifdef _DEBUG
#pragma comment(lib,"TPBufferOperateUD.lib")
#else
#pragma comment(lib,"TPBufferOperateU.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib,"TPBufferOperateD.lib")
#else
#pragma comment(lib,"TPBufferOperate.lib")
#endif
#endif
#endif