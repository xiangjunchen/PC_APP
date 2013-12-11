//////////////////////////////////////////////////////////////////////////
// 静态变量定义，此文件只能被 TPSSE2.cpp 调用，否则会造成内存浪费
//
// 创建：赵志猛 @ 2007-09-06
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "TPBufferOperateDef.h"

//////////////////////////////////////////////////////////////////////////
// 常量

static const TPSSE_DECLARE(DWORD) gc_dwMaskDLB[4] = { 0x000000ff, 0x000000ff, 0x000000ff, 0x000000ff };
static const TPSSE_DECLARE(DWORD) gc_dwMaskWLB[4] = { 0x00ff00ff, 0x00ff00ff, 0x00ff00ff, 0x00ff00ff };
static const TPSSE_DECLARE(DWORD) gc_dwMaskWHB[4] = { 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00 };
static const TPSSE_DECLARE(DWORD) gc_dwMaskRGB[4] = { 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff };
static const TPSSE_DECLARE(DWORD) gc_dwMaskWBIT[4] = { 0x00010001, 0x00010001, 0x00010001, 0x00010001 };
static const TPSSE_DECLARE(DWORD) gc_dwMaskAll[4] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };
static const TPSSE_DECLARE(DWORD) gc_dwMaskDLW[4] = { 0x0000ffff, 0x0000ffff, 0x0000ffff, 0x0000ffff };
static const TPSSE_DECLARE(DWORD) gc_dwMaskDHW[4] = { 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000 };
static const TPSSE_DECLARE(DWORD) gc_dwMaskSpecUV[4] = { 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000 };
static const TPSSE_DECLARE(DWORD) gc_dwMaskAlpha[4] = { 0xff000000, 0xff000000, 0xff000000, 0xff000000 };

static const TPSSE_DECLARE(int) gc_nOff128_5[4] = { 0x10001000, 0x10001000, 0x10001000, 0x10001000 };
static const TPSSE_DECLARE(int) gc_nOff128_6[4] = { 0x20002000, 0x20002000, 0x20002000, 0x20002000 };
static const TPSSE_DECLARE(int) gc_nOff128_7[4] = { 0x40004000, 0x40004000, 0x40004000, 0x40004000 };
static const TPSSE_DECLARE(int) gc_nRoundComp_2[4] = { 0x00020002, 0x00020002, 0x00020002, 0x00020002 };
static const TPSSE_DECLARE(int) gc_nRoundComp_3[4] = { 0x00040004, 0x00040004, 0x00040004, 0x00040004 };
static const TPSSE_DECLARE(int) gc_nRoundComp_5[4] = { 0x00100010, 0x00100010, 0x00100010, 0x00100010 };

static const TPSSE_DECLARE(float) gc_fRoundComp[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
static const TPSSE_DECLARE(float) gc_fOff128[4] = { 128.f, 128.f, 128.f, 128.f };

//////////////////////////////////////////////////////////////////////////
// 变量

// 颜色变换参数定义
// rgba bgra
static TPSSE_DECLARE(int) g_nY32_BR_HD[4];		// YbYr YbYr YbYr YbYr  （rgb）
static TPSSE_DECLARE(int) g_nY32_G_HD[4];		// 0Yg 0Yg 0Yg 0Yg  （rgb or bgr）
static TPSSE_DECLARE(int) g_nY32_RB_HD[4];		// YrYb YrYb YrYb YrYb  （bgr）

static TPSSE_DECLARE(int) g_nY32_BR_SD[4];		// YbYr YbYr YbYr YbYr  （rgb）
static TPSSE_DECLARE(int) g_nY32_G_SD[4];		// 0Yg 0Yg 0Yg 0Yg  （rgb or bgr）
static TPSSE_DECLARE(int) g_nY32_RB_SD[4];		// YrYb YrYb YrYb YrYb  （bgr）

// rgb bgr
static TPSSE_DECLARE(int) g_nY24_RGB_HD[4];		// YrYg YbYr YgYb YrYg  （rgb）
static TPSSE_DECLARE(int) g_nY24_BGR_HD[4];		// YbYg YrYb YgYr YbYg  （bgr）

static TPSSE_DECLARE(int) g_nY24_RGB_SD[4];		// YrYg YbYr YgYb YrYg  （rgb）
static TPSSE_DECLARE(int) g_nY24_BGR_SD[4];		// YbYg YrYb YgYr YbYg  （bgr）

// rgba rgb bgra bgr
static TPSSE_DECLARE(int) g_nY32_BGR_HD[4];		// 0Yb YgYr 0Yb YgYr  （rgb）
static TPSSE_DECLARE(int) g_nUV_BGR_HD[4];		// 0Vb VgVr 0Ub UgUr  （rgb）
static TPSSE_DECLARE(int) g_nU_BGR_HD[4];		// 0Ub UgUr 0Ub UgUr  （rgb）
static TPSSE_DECLARE(int) g_nV_BGR_HD[4];		// 0Vb VgVr 0Vb VgVr  （rgb）
static TPSSE_DECLARE(int) g_nY32_RGB_HD[4];		// 0Yr YgYb 0Yr YgYb  （bgr）
static TPSSE_DECLARE(int) g_nUV_RGB_HD[4];		// 0Vr VgVb 0Ur UgUb  （bgr）
static TPSSE_DECLARE(int) g_nU_RGB_HD[4];		// 0Ur UgUb 0Ur UgUb  （bgr）
static TPSSE_DECLARE(int) g_nV_RGB_HD[4];		// 0Vr VgVb 0Vr VgVb  （bgr）
static TPSSE_DECLARE(int) g_nR_V_HD[4];			// RvRv RvRv RvRv RvRv  （rgb or bgr）
static TPSSE_DECLARE(int) g_nG_U_HD[4];			// GuGu GuGu GuGu GuGu  （rgb or bgr）
static TPSSE_DECLARE(int) g_nG_V_HD[4];			// GvGv GvGv GvGv GvGv  （rgb or bgr）
static TPSSE_DECLARE(int) g_nB_U_HD[4];			// BuBu BuBu BuBu BuBu  （rgb or bgr）

static TPSSE_DECLARE(int) g_nY32_BGR_SD[4];		// 0Yb YgYr 0Yb YgYr  （rgb）
static TPSSE_DECLARE(int) g_nUV_BGR_SD[4];		// 0Vb VgVr 0Ub UgUr  （rgb）
static TPSSE_DECLARE(int) g_nU_BGR_SD[4];		// 0Ub UgUr 0Ub UgUr  （rgb）
static TPSSE_DECLARE(int) g_nV_BGR_SD[4];		// 0Vb VgVr 0Vb VgVr  （rgb）
static TPSSE_DECLARE(int) g_nY32_RGB_SD[4];		// 0Yr YgYb 0Yr YgYb  （bgr）
static TPSSE_DECLARE(int) g_nUV_RGB_SD[4];		// 0Vr VgVb 0Ur UgUb  （bgr）
static TPSSE_DECLARE(int) g_nU_RGB_SD[4];		// 0Ur UgUb 0Ur UgUb  （bgr）
static TPSSE_DECLARE(int) g_nV_RGB_SD[4];		// 0Vr VgVb 0Vr VgVb  （bgr）
static TPSSE_DECLARE(int) g_nR_V_SD[4];			// RvRv RvRv RvRv RvRv  （rgb or bgr）
static TPSSE_DECLARE(int) g_nG_U_SD[4];			// GuGu GuGu GuGu GuGu  （rgb or bgr）
static TPSSE_DECLARE(int) g_nG_V_SD[4];			// GvGv GvGv GvGv GvGv  （rgb or bgr）
static TPSSE_DECLARE(int) g_nB_U_SD[4];			// BuBu BuBu BuBu BuBu  （rgb or bgr）

// rgb bgr
static TPSSE_DECLARE(float) g_fRB_YUV_HD[4];	// By Rv Bu Ry  （rgb or bgr）
static TPSSE_DECLARE(float) g_fG_YUV_HD[4];		// 0 Gv Gu Gy  （rgb or bgr）

static TPSSE_DECLARE(float) g_fRB_YUV_SD[4];	// By Rv Bu Ry  （rgb or bgr）
static TPSSE_DECLARE(float) g_fG_YUV_SD[4];		// 0 Gv Gu Gy  （rgb or bgr）

//////////////////////////////////////////////////////////////////////////
// 文件结束