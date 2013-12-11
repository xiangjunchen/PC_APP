//////////////////////////////////////////////////////////////////////////
// 描述：	各个颜色空间之间的变换公式定义
//			整个系统中要求只存在 CPU 一套变换定义，任何使用颜色变换的地方都必须
//			包含此头文件，确保整个系统的颜色的统一性。
// 路径：	TPCommonData\TPBufferOperate\TPColorSpaceDef.h
// 创建：	赵志猛 @ 2007-07-26
//////////////////////////////////////////////////////////////////////////
#pragma once

///////////////////////////////////////////////////////////////////////////////
// YUVA 与 RGBA 之间的转换

// 高清定义（HD）
static const float g_matYUVA2RGBA_HD[][4] = {
    { 1.f,	    0.f,		    1.574722f,	    0.f },
    { 1.f,	    -0.187314f,	    -0.468207f, 	0.f },
    { 1.f,	    1.855615f,	    0.f,	    	0.f },
    { 0.f,	    0.f,	    	0.f,	    	1.f }
};

static const float g_matRGBA2YUVA_HD[][4] = {
    { 0.212639f,	0.715169f,  	0.072192f,	    0.f },
    { -0.114592f,	-0.385409f,	    0.5f,		    0.f },
    { 0.5f,		    -0.454156f,	    -0.045844f,	    0.f },
    { 0.f,		    0.f,		    0.f,	    	1.f }
};

// 标清定义（SD）（索尼认为数字信号下 PAL 与 NTSC 相同）
static const float g_matYUVA2RGBA_SD[][4] = {
    { 1.f,  	0.f,		    1.401978f,	    0.f },
    { 1.f,  	-0.344116f,	    -0.714111f, 	0.f },
    { 1.f,  	1.771973f,  	0.f,		    0.f },
    { 0.f,  	0.f,		    0.f,		    1.f }
};

static const float g_matRGBA2YUVA_SD[][4] = {
    { 0.29895f, 	0.587036f,	    0.114014f,  	0.f },
    { -0.168701f,	-0.331299f,     0.5f,		    0.f },
    { 0.5f,	    	-0.418701f,	    -0.081299f, 	0.f },
    { 0.f,	    	0.f,		    0.f,		    1.f }
};


#define CS_UV_OFFSET		128.f / 255.f

#define cYUYVMode			D3DXVECTOR4(0.0f, CS_UV_OFFSET, 0.0f, CS_UV_OFFSET)
#define cYUVAMode			D3DXVECTOR4(0.0f, CS_UV_OFFSET, CS_UV_OFFSET, 0.0f)
#define CS_YUYV_OFFSET		cYUYVMode
#define CS_YUVA_OFFSET		cYUVAMode

//////////////////////////////////////////////////////////////////////////
// 文件结束