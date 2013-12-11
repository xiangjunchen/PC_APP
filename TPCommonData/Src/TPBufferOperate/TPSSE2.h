// 采用 SSE2 优化算法

#pragma once

//////////////////////////////////////////////////////////////////////////
// 初始化参数
void InitSSEStaticCoef();

//////////////////////////////////////////////////////////////////////////
// 拷贝一行 Buffer
//////////////////////////////////////////////////////////////////////////
void memcpy_line(OUT void *des,		// 目标
				 IN void *src,		// 源
				 IN int na16,		// 宽度除以 16 的商
				 IN int nm16);		// 宽度除以 16 的余数

//////////////////////////////////////////////////////////////////////////
// 以字节为单位设置一行 Buffer
//////////////////////////////////////////////////////////////////////////
void memset_char_line(OUT void *des,	// 目标
					  IN char value,	// 设置值
					  IN int na16,		// 宽度除以 16 的商
					  IN int nm16);		// 宽度除以 16 的余数

//////////////////////////////////////////////////////////////////////////
// 以双字为单位设置一行 Buffer
//////////////////////////////////////////////////////////////////////////
void memset_dword_line(OUT void *des,	// 目标
					   IN DWORD value,	// 设置值
					   IN int na16,		// 宽度除以 16 的商
					   IN int nm16);	// 宽度除以 16 的余数

//////////////////////////////////////////////////////////////////////////
// 分裂字节
// 分裂函数命名规则：第一个 n 表示分裂对象的字节长，第二个 n 表示分裂起始点，
//		第三个 n 表示分裂字节数，都是 n 时表示任意分裂
//////////////////////////////////////////////////////////////////////////
void memsplit_n_n_n(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount,		// 分裂对象的个数
					IN int nUnitBytes,		// 分裂对象的字节长
					IN int nCellStart,		// 分裂起始点
					IN int nCellCount);		// 分裂字节数
// 2
void memsplit_2_0_1(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount);		// 分裂对象的个数
void memsplit_2_1_1(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount);		// 分裂对象的个数
// 4
void memsplit_4_0_1(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount);		// 分裂对象的个数
void memsplit_4_0_2(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount);		// 分裂对象的个数
void memsplit_4_0_3(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount);		// 分裂对象的个数
void memsplit_4_1_1(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount);		// 分裂对象的个数
void memsplit_4_1_2(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount);		// 分裂对象的个数
void memsplit_4_1_3(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount);		// 分裂对象的个数
void memsplit_4_2_1(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount);		// 分裂对象的个数
void memsplit_4_2_2(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount);		// 分裂对象的个数
void memsplit_4_3_1(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount);		// 分裂对象的个数
// 8
void memsplit_8_0_4(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount);		// 分裂对象的个数
void memsplit_8_4_4(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount);		// 分裂对象的个数
//////////////////////////////////////////////////////////////////////////
// 合成字节
// 合成函数命名规则：第一个 n 表示合成对象的字节长，第二个 n 表示合成起始点，
//		第三个 n 表示合成字节数，都是 n 时表示任意合成
//////////////////////////////////////////////////////////////////////////
void memcompose_n_n_n(OUT void *des,			// 目标
					  IN void *src,				// 源
					  IN int nUnitCount,		// 合成对象的个数
					  IN int nUnitBytes,		// 合成对象的字节长
					  IN int nCellStart,		// 合成起始点
					  IN int nCellCount);		// 合成字节数
// 2
void memcompose_2_0_1(OUT void *des,			// 目标
					  IN void *src,				// 源
					  IN int nUnitCount);		// 合成对象的个数
void memcompose_2_1_1(OUT void *des,			// 目标
					  IN void *src,				// 源
					  IN int nUnitCount);		// 合成对象的个数
// 4
void memcompose_4_0_1(OUT void *des,			// 目标
					  IN void *src,				// 源
					  IN int nUnitCount);		// 合成对象的个数
void memcompose_4_0_2(OUT void *des,			// 目标
					  IN void *src,				// 源
					  IN int nUnitCount);		// 合成对象的个数
void memcompose_4_1_1(OUT void *des,			// 目标
					  IN void *src,				// 源
					  IN int nUnitCount);		// 合成对象的个数
void memcompose_4_2_1(OUT void *des,			// 目标
					  IN void *src,				// 源
					  IN int nUnitCount);		// 合成对象的个数
void memcompose_4_2_2(OUT void *des,			// 目标
					  IN void *src,				// 源
					  IN int nUnitCount);		// 合成对象的个数
void memcompose_4_3_1(OUT void *des,			// 目标
					  IN void *src,				// 源
					  IN int nUnitCount);		// 合成对象的个数
// 8
void memcompose_8_0_4(OUT void *des,			// 目标
					  IN void *src,				// 源
					  IN int nUnitCount);		// 合成对象的个数
void memcompose_8_4_4(OUT void *des,			// 目标
					  IN void *src,				// 源
					  IN int nUnitCount);		// 合成对象的个数

//////////////////////////////////////////////////////////////////////////
// 对一行 BGRA32 Buffer 进行 Alpha 混色
// 优化计算透明度 MA = SA + DA - SA * DA / 255 = SA + DA - (DA * SA + 255) >> 8
// 查表得到增益 KA = SA / MA = SA * 255 / (SA * 255 + DA * 255 - SA * DA)
// 优化计算颜色值 MC = (SC * SA + DC * (1 - SA / 255) * DA) / MA = (SC - DC) * KA + DC
// 增益表的偏移地址 Offset = (SA * 256 + DA) * 4
//////////////////////////////////////////////////////////////////////////
void bgra32_line_mix(IN OUT void *des,		// 目标
					 IN void *src,			// 源
					 IN int na16,			// 宽度除以 16 的商
					 IN int nm16,			// 宽度除以 16 的余数
					 IN void *ka_table);	// 256 * 256 * sizeof(float) 的 KA 表

//////////////////////////////////////////////////////////////////////////
// 对一行 BGRA32 Buffer 进行 Alpha 反混色
// 优化计算透明度 MA = (255 - SA) + DA - (255 - SA) * DA / 255 = 255 - SA + (DA * SA + 255) >> 8
// 查表得到增益 KA = (255 - SA) / MA = (65535 - SA * 255) / (65535 - SA * 255 + SA * DA)
// 优化计算颜色值 MC = (SC * (255 - SA) + DC * (1 - (255 - SA) / 255) * DA) / MA = (SC - DC) * KA + DC
// 增益表的偏移地址 Offset = ((255 - SA) * 256 + DA) * 4
//////////////////////////////////////////////////////////////////////////
void bgra32_line_notmix(IN OUT void *des,		// 目标
						IN void *src,			// 源
						IN int na16,			// 宽度除以 16 的商
						IN int nm16,			// 宽度除以 16 的余数
						IN void *ka_table);		// 256 * 256 * sizeof(float) 的 KA 表

//////////////////////////////////////////////////////////////////////////
// 采用双线性插值法对 BGRA32 Buffer 进行缩放，使用权重与索引映射表进行逐行逐个像素处理
// 通过索引映射表得到目标行像素映射到源行的起始位置，利用以它为首的四个相邻像素进行加权插值
// 计算公式		S00 * x * y				S10 * (1 - x) * y
//				S01 * x * (1 - y)		S11 * (1 - x) * (1 - y)
// D = S11 + (S00 - S10 - S01 + S11) * x * y + (S01 - S11) * x + (S10 - S11) * y
//////////////////////////////////////////////////////////////////////////
void bgra32_bl_copy_line(OUT void* des,		// 目标拷贝行
						 IN int dcw,		// 目标拷贝行的宽度
						 IN void* src,		// 源拷贝行
						 IN int src_pitch,	// 源图一行的字节数
						 IN void* pPowerX,	// 目标映射到源的 X 方向上的列权重
						 IN void* pMapX,	// 目标映射到源的 X 方向上的列索引
						 IN DWORD powerY);	// 目标行映射到源行的行权重

//////////////////////////////////////////////////////////////////////////
// 采用加权均值滤波法对 BGRA32 Buffer 进行缩放，使用权重与索引映射表进行逐行逐个像素处理
//////////////////////////////////////////////////////////////////////////
// 对垂直方向上目标行映射到源的行进行加权（垂直方向任意缩放倍数）
void bgra32_mean_copy_add_y(OUT void* des,			// 临时缓存行（是源行拷贝宽度的四倍）
							IN void* src,			// 源拷贝起始行
							IN int scw,				// 源拷贝宽度
							IN int src_pitch,		// 源行字节数
							IN void* pPowerY,		// 目标某行映射的垂直方向上的权重表
							IN void* pCoefH,		// 垂直方向上的缩放系数
							IN int nMapY);			// 目标一行映射到源的行数
// （垂直方向缩放倍数小于255的高效算法）
void bgra32_mean_copy_add_y_quick(OUT void* des,		// 临时缓存行（是源行拷贝宽度的四倍）
								  IN void* src,			// 源拷贝起始行
								  IN int scw,			// 源拷贝宽度
								  IN int src_pitch,		// 源行字节数
								  IN void* pPowerY,		// 目标某行映射的垂直方向上的权重表
								  IN void* pCoefH,		// 垂直方向上的缩放系数
								  IN int nMapY);		// 目标一行映射到源的行数
// 对水平方向上目标列映射到源的列进行加权
void bgra32_mean_copy_add_x(OUT void* des,			// 目标拷贝行
							IN int dcw,				// 目标拷贝宽度
							IN void* src,			// 临时缓存行（由行加权得到的结果）
							IN void* pPowerX,		// 水平方向上的权重表
							IN void* pMapX,			// 水平方向上的映射表
							IN void* pCoefW);		// 水平方向上的缩放系数


//////////////////////////////////////////////////////////////////////////

// 颜色变换并拆场（不做边界判断）

void color32_to_yuyva_frame(OUT BYTE* pDesYUYV,         // YUYV 输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                            IN int nYUYVPitch,
                            OUT BYTE* pDesAlpha,        // Alpha 输出（存储方式 A0 A1 A2 A3 A4 ……）
                            IN int nAlphaPitch,
                            IN BYTE* pSrc32,            // 源 *g@a 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
                            IN int nSrcPitch,           // 源一行的字节宽度
                            IN int nWidth,		        // 源图大小
                            IN int nHeight,
                            IN BOOL bHD,                // 高标清标志
                            IN BOOL bSrcRevert,         // 图像是否行翻转标志
                            IN BOOL bBGRA);             // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
void color32_to_yuyva_split(OUT BYTE* pDesYUYVF,        // YUYV 第一场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                            IN int nYUYVPitchF,
                            OUT BYTE* pDesAlphaF,       // Alpha 第一场输出（存储方式 A0 A1 A2 A3 A4 ……）
                            IN int nAlphaPitchF,
                            OUT BYTE* pDesYUYVS,        // YUYV 第二场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                            IN int nYUYVPitchS,
                            OUT BYTE* pDesAlphaS,       // Alpha 第二场输出（存储方式 A0 A1 A2 A3 A4 ……）
                            IN int nAlphaPitchS,
                            IN BYTE* pSrc32,            // 源 *g@a 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
                            IN int nSrcPitch,           // 源一行的字节宽度
                            IN int nWidth,              // 源图大小
                            IN int nHeight,
                            IN BOOL bHD,                // 高标清标志
                            IN BOOL bSrcRevert,         // 图像是否行翻转标志
                            IN BOOL bBGRA);             // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位

void color32_to_yuyv_frame(OUT BYTE* pDesYUYV,         // YUYV 输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                           IN int nYUYVPitch,
                           IN BYTE* pSrc32,            // 源 *g@a 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
                           IN int nSrcPitch,           // 源一行的字节宽度
                           IN int nWidth,		        // 源图大小
                           IN int nHeight,
                           IN BOOL bHD,                // 高标清标志
                           IN BOOL bSrcRevert,         // 图像是否行翻转标志
                           IN BOOL bBGRA);             // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
void color32_to_yuyv_split(OUT BYTE* pDesYUYVF,        // YUYV 第一场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                           IN int nYUYVPitchF,
                           OUT BYTE* pDesYUYVS,        // YUYV 第二场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                           IN int nYUYVPitchS,
                           IN BYTE* pSrc32,            // 源 *g@a 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
                           IN int nSrcPitch,           // 源一行的字节宽度
                           IN int nWidth,              // 源图大小
                           IN int nHeight,
                           IN BOOL bHD,                // 高标清标志
                           IN BOOL bSrcRevert,         // 图像是否行翻转标志
                           IN BOOL bBGRA);             // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位

void yuyv_to_color32_compose(OUT BYTE* pDes32,          // *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）（Alpha 直接填 0xff）
                             IN int nDesPitch,
                             IN BYTE* pSrcYUYVF,        // YUYV 第一场输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                             IN int nFirstPitch,
                             IN BYTE* pSrcYUYVS,        // YUYV 第二场输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                             IN int nSecondPitch,
                             IN int nWidth,             // 目标大小
                             IN int nHeight,
                             IN BOOL bHD,               // 高标清标志
                             IN BOOL bBGRA,            // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
							 IN BOOL bDstRevert);		// 图像是否行翻转标志

void yuyv_to_color32_frame(OUT BYTE* pDes32,          // *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）（Alpha 直接填 0xff）
                             IN int nDesPitch,
                             IN BYTE* pSrcYUYV,        // YUYV 输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                             IN int nSrcPitch,
                             IN int nWidth,             // 目标大小
                             IN int nHeight,
                             IN BOOL bHD,               // 高标清标志
                             IN BOOL bBGRA,            // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
							 IN BOOL bDstRevert);		// 图像是否行翻转标志

void color24_to_yuyv_frame(OUT BYTE* pDesYUYV,         // YUYV 输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                           IN int nYUYVPitch,
                           IN BYTE* pSrc24,            // *G@ 输入，存放方式 *0 G0 @0 *1 G1 @1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
                           IN int nSrcPitch,           // 源一行的字节宽度
                           IN int nWidth,		       // 源图大小
                           IN int nHeight,
                           IN BOOL bHD,                // 高标清标志
                           IN BOOL bSrcRevert,         // 图像是否行翻转标志
                           IN BOOL bBGR);              // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
void color24_to_yuyv_split(OUT BYTE* pDesFirst,        // 第一场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                           IN int nFirstPitch,
                           OUT BYTE* pDesSecond,       // 第二场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                           IN int nSecondPitch,
                           IN BYTE* pSrc24,            // *G@ 输入，存放方式 *0 G0 @0 *1 G1 @1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
                           IN int nSrcPitch,           // 源一行的字节宽度（必须为 4 的倍数）
                           IN int nWidth,			   // 源图大小
                           IN int nHeight,
                           IN BOOL bHD,                // 高标清标志
                           IN BOOL bSrcRevert,         // 图像是否行翻转标志
                           IN BOOL bBGR);              // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位

void yuyv_to_color24_frame(OUT BYTE* pDes24,			// *G@ 缓存，存放方式 *0 G0 @0  *1 G1 @1  ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
						   IN int nDesPitch,
						   IN BYTE* pSrcYUYV,			// YUYV 输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
						   IN int nSrcPitch,
						   IN int nWidth,		        // 目标大小
						   IN int nHeight,
						   IN BOOL bHD,			        // 高标清标志
						   IN BOOL bBGRA,				// 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
						   IN BOOL bDstRevert);			// 图像是否行翻转标志

void yuv420_to_color32_frame(OUT BYTE* pDes32,           // *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）（Alpha 直接填 0xff）
                             IN int nDesPitch,
                             IN BYTE* pSrcY,             // Y 输入（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
                             IN int nSrcYPitch,
                             IN BYTE* pSrcU,             // U 输入（存储方式 UU0 UU2 UU4 ……）（两行共用一个）
                             IN int nSrcUPitch,
                             IN BYTE* pSrcV,             // V 输入（存储方式 VV0 VV2 VV4 ……）（两行共用一个）
                             IN int nSrcVPitch,
                             IN int nWidth,              // 目标大小
                             IN int nHeight,
                             IN BOOL bHD,                // 高标清标志
                             IN BOOL bDesRevert,         // 图像是否行翻转标志
                             IN BOOL bBGRA);             // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
void yuv420_to_color32_split(OUT BYTE* pDesFirst,        // 第一场 *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）（Alpha 直接填 0xff）
                             IN int nFirstPitch,
                             OUT BYTE* pDesSecond,       // 第二场 *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）（Alpha 直接填 0xff）
                             IN int nSecondPitch,
                             IN BYTE* pSrcY,             // Y 输入（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
                             IN int nSrcYPitch,
                             IN BYTE* pSrcU,             // U 输入（存储方式 UU0 UU2 UU4 ……）（两行共用一个）
                             IN int nSrcUPitch,
                             IN BYTE* pSrcV,             // V 输入（存储方式 VV0 VV2 VV4 ……）（两行共用一个）
                             IN int nSrcVPitch,
                             IN int nWidth,              // 目标大小
                             IN int nHeight,
                             IN BOOL bHD,                // 高标清标志
                             IN BOOL bDesRevert,         // 图像是否行翻转标志
                             IN BOOL bBGRA);             // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位

void yuv422_to_color32_frame(OUT BYTE* pDes32,           // *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）（Alpha 直接填 0xff）
                             IN int nDesPitch,
                             IN BYTE* pSrcY,             // Y 输入（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
                             IN int nSrcYPitch,
                             IN BYTE* pSrcU,             // U 输入（存储方式 U0 U2 U4 ……）
                             IN int nSrcUPitch,
                             IN BYTE* pSrcV,             // V 输入（存储方式 V0 V2 V4 ……）
                             IN int nSrcVPitch,
                             IN int nWidth,              // 目标大小
                             IN int nHeight,
                             IN BOOL bHD,                // 高标清标志
                             IN BOOL bDesRevert,         // 图像是否行翻转标志
                             IN BOOL bBGRA);             // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
void yuv422_to_color32_split(OUT BYTE* pDesFirst,        // 第一场 *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）（Alpha 直接填 0xff）
                             IN int nFirstPitch,
                             OUT BYTE* pDesSecond,       // 第二场 *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）（Alpha 直接填 0xff）
                             IN int nSecondPitch,
                             IN BYTE* pSrcY,             // Y 输入（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
                             IN int nSrcYPitch,
                             IN BYTE* pSrcU,             // U 输入（存储方式 U0 U2 U4 ……）
                             IN int nSrcUPitch,
                             IN BYTE* pSrcV,             // V 输入（存储方式 V0 V2 V4 ……）
                             IN int nSrcVPitch,
                             IN int nWidth,              // 目标大小
                             IN int nHeight,
                             IN BOOL bHD,                // 高标清标志
                             IN BOOL bDesRevert,         // 图像是否行翻转标志
                             IN BOOL bBGRA);             // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位

// 重组数据

// 需要按场处理，在场内隔行插值，每次输出两行
// 前两行
void yuv420_to_yuyv_line_2_F(OUT BYTE* yuyv,        // yuyv 输出，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
                             IN int yuyv_p,
                             IN BYTE* y,            // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
                             IN int y_p_2,
                             IN BYTE* u,            // u 输入，存放方式 UU0 UU2 UU4 UU6 ……
                             IN int u_p_2,
                             IN BYTE* v,            // v 输入，存放方式 VV0 VV2 VV4 VV6 ……
                             IN int v_p_2,
                             IN int na16,           // 一行像素宽度除以 16 的商与余数（注意目标为 16 位的缓存）
                             IN int nm16);
// 中间行
void yuv420_to_yuyv_line_2(OUT BYTE* yuyv,        // yuyv 输出，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
                           IN int yuyv_p,
                           IN BYTE* y,            // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
                           IN int y_p_2,
                           IN BYTE* u,            // u 输入，存放方式 UU0 UU2 UU4 UU6 ……
                           IN int u_p_2,
                           IN BYTE* v,            // v 输入，存放方式 VV0 VV2 VV4 VV6 ……
                           IN int v_p_2,
                           IN int na16,           // 一行像素宽度除以 16 的商与余数（注意目标为 16 位的缓存）
                           IN int nm16);
// 最后两行
void yuv420_to_yuyv_line_2_L(OUT BYTE* yuyv,        // yuyv 输出，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
                             IN int yuyv_p,
                             IN BYTE* y,            // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
                             IN int y_p_2,
                             IN BYTE* u,            // u 输入，存放方式 UU0 UU2 UU4 UU6 ……
                             IN int u_p_2,
                             IN BYTE* v,            // v 输入，存放方式 VV0 VV2 VV4 VV6 ……
                             IN int v_p_2,
                             IN int na16,           // 一行像素宽度除以 16 的商与余数（注意目标为 16 位的缓存）
                             IN int nm16);
// 最后一行
void yuv420_to_yuyv_line_1_L(OUT BYTE* yuyv,        // yuyv 输出，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
                             IN BYTE* y,            // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
                             IN BYTE* u,            // u 输入，存放方式 UU0 UU2 UU4 UU6 ……
                             IN int u_p_2,
                             IN BYTE* v,            // v 输入，存放方式 VV0 VV2 VV4 VV6 ……
                             IN int v_p_2,
                             IN int na16,           // 一行像素宽度除以 16 的商与余数（注意目标为 16 位的缓存）
                             IN int nm16);

// XDCAM 插值方式：420->422 一行插值出两行（使用上下行），第一场公式为
// F1_Out0 = 0.125 * F1_Prev + 0.875 * F1_Curr    F1_Out1 = 0.625 * F1_Curr + 0.375 * F1_Next
// 第一行
void yuv420_to_yuyv_xdcam_F_line_2_F(OUT BYTE* yuyv,        // yuyv 输出，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
									 IN int yuyv_p,
									 IN BYTE* y,            // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
									 IN int y_p_2,
									 IN BYTE* u,            // u 输入，存放方式 UU0 UU2 UU4 UU6 ……
									 IN int u_p_2,
									 IN BYTE* v,            // v 输入，存放方式 VV0 VV2 VV4 VV6 ……
									 IN int v_p_2,
									 IN int na16,           // 一行像素宽度除以 16 的商与余数（注意目标为 16 位的缓存）
									 IN int nm16);
// 中间行
void yuv420_to_yuyv_xdcam_F_line_2(OUT BYTE* yuyv,        // yuyv 输出，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
								   IN int yuyv_p,
								   IN BYTE* y,            // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
								   IN int y_p_2,
								   IN BYTE* u,            // u 输入，存放方式 UU0 UU2 UU4 UU6 ……
								   IN int u_p_2,
								   IN BYTE* v,            // v 输入，存放方式 VV0 VV2 VV4 VV6 ……
								   IN int v_p_2,
								   IN int na16,           // 一行像素宽度除以 16 的商与余数（注意目标为 16 位的缓存）
								   IN int nm16);
// 最后一行
void yuv420_to_yuyv_xdcam_F_line_1_L(OUT BYTE* yuyv,        // yuyv 输出，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
									 IN BYTE* y,            // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
									 IN BYTE* u,            // u 输入，存放方式 UU0 UU2 UU4 UU6 ……
									 IN int u_p_2,
									 IN BYTE* v,            // v 输入，存放方式 VV0 VV2 VV4 VV6 ……
									 IN int v_p_2,
									 IN int na16,           // 一行像素宽度除以 16 的商与余数（注意目标为 16 位的缓存）
									 IN int nm16);
// XDCAM 插值方式：420->422 一行插值出两行（使用上下行），第二场公式为
// F2_Out0 = 0.375 * F1_Prev + 0.625 * F1_Curr    F1_Out1 = 0.875 * F1_Curr + 0.125 * F1_Next
// 第一行
void yuv420_to_yuyv_xdcam_S_line_2_F(OUT BYTE* yuyv,        // yuyv 输出，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
									 IN int yuyv_p,
									 IN BYTE* y,            // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
									 IN int y_p_2,
									 IN BYTE* u,            // u 输入，存放方式 UU0 UU2 UU4 UU6 ……
									 IN int u_p_2,
									 IN BYTE* v,            // v 输入，存放方式 VV0 VV2 VV4 VV6 ……
									 IN int v_p_2,
									 IN int na16,           // 一行像素宽度除以 16 的商与余数（注意目标为 16 位的缓存）
									 IN int nm16);
// 中间行
void yuv420_to_yuyv_xdcam_S_line_2(OUT BYTE* yuyv,        // yuyv 输出，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
								   IN int yuyv_p,
								   IN BYTE* y,            // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
								   IN int y_p_2,
								   IN BYTE* u,            // u 输入，存放方式 UU0 UU2 UU4 UU6 ……
								   IN int u_p_2,
								   IN BYTE* v,            // v 输入，存放方式 VV0 VV2 VV4 VV6 ……
								   IN int v_p_2,
								   IN int na16,           // 一行像素宽度除以 16 的商与余数（注意目标为 16 位的缓存）
								   IN int nm16);
// 最后一行
void yuv420_to_yuyv_xdcam_S_line_1_L(OUT BYTE* yuyv,        // yuyv 输出，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
									 IN BYTE* y,            // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
									 IN BYTE* u,            // u 输入，存放方式 UU0 UU2 UU4 UU6 ……
									 IN int u_p_2,
									 IN BYTE* v,            // v 输入，存放方式 VV0 VV2 VV4 VV6 ……
									 IN int v_p_2,
									 IN int na16,           // 一行像素宽度除以 16 的商与余数（注意目标为 16 位的缓存）
									 IN int nm16);
// XDCAM 插值方式：422->420 两行插值出一行，第一场公式为 F1_Out = 0.75 * F1_0 + 0.25 * F1_1
void yuyv_to_yuv420_xdcam_F_line_2(OUT BYTE* y,         // 第一行 y 输出，存放方式 Y0 Y1 Y2 Y3 ……
								   IN int y_pitch,		// 第二行 y 输出与第一行的偏移
								   OUT BYTE* u,			// 一行 u 输出，存放方式 UU0 UU2 UU4 UU6 ……
								   OUT BYTE* v,			// 一行 v 输出，存放方式 VV0 VV2 VV4 VV6 ……
								   IN BYTE* yuyv,		// 第一行 yuyv 输入，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
								   IN int yuyv_pitch,	// 第二行 yuyv 输入与第一行的偏移
								   IN int na8,          // 一行像素宽度除以 8 的商与余数（注意目标为 16 位的缓存）
								   IN int nm8);
// XDCAM 插值方式：422->420 两行插值出一行，第二场公式为 F2_Out = 0.25 * F2_0 + 3 * F2_1
void yuyv_to_yuv420_xdcam_S_line_2(OUT BYTE* y,         // 第一行 y 输出，存放方式 Y0 Y1 Y2 Y3 ……
								   IN int y_pitch,		// 第二行 y 输出与第一行的偏移
								   OUT BYTE* u,			// 一行 u 输出，存放方式 UU0 UU2 UU4 UU6 ……
								   OUT BYTE* v,			// 一行 v 输出，存放方式 VV0 VV2 VV4 VV6 ……
								   IN BYTE* yuyv,		// 第一行 yuyv 输入，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
								   IN int yuyv_pitch,	// 第二行 yuyv 输入与第一行的偏移
								   IN int na8,          // 一行像素宽度除以 8 的商与余数（注意目标为 16 位的缓存）
								   IN int nm8);

void yuyv_to_yuv420_line_2_avr(OUT BYTE* y,         // 第一行 y 输出，存放方式 Y0 Y1 Y2 Y3 ……
							   IN int y_pitch,		// 第二行 y 输出与第一行的偏移
							   OUT BYTE* u,			// 一行 u 输出，存放方式 UU0 UU2 UU4 UU6 ……
							   OUT BYTE* v,			// 一行 v 输出，存放方式 VV0 VV2 VV4 VV6 ……
							   IN BYTE* yuyv,		// 第一行 yuyv 输入，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
							   IN int yuyv_pitch,	// 第二行 yuyv 输入与第一行的偏移
							   IN int na8,          // 一行像素宽度除以 8 的商与余数（注意目标为 16 位的缓存）
							   IN int nm8);
void yuyv_to_yuv420_line_1(OUT BYTE* y,         // 一行 y 输出，存放方式 Y0 Y1 Y2 Y3 ……
						   OUT BYTE* u,			// 一行 u 输出，存放方式 UU0 UU2 UU4 UU6 ……
						   OUT BYTE* v,			// 一行 v 输出，存放方式 VV0 VV2 VV4 VV6 ……
						   IN BYTE* yuyv,		// 一行 yuyv 输入，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
						   IN int na8,			// 一行像素宽度除以 8 的商与余数（注意目标为 16 位的缓存）
						   IN int nm8);
// 采用场内四行 -1, 3, 3, -1 的系数对 UV 进行采样
// 第一行
void yuyv_to_yuv420_line_4_F(OUT BYTE* y,			// 第一行 y 输出，存放方式 Y0 Y1 Y2 Y3 ……
							 IN int y_pitch,		// 第二行 y 输出与第一行的偏移
							 OUT BYTE* u,			// 一行 u 输出，存放方式 UU0 UU2 UU4 UU6 ……
							 OUT BYTE* v,			// 一行 v 输出，存放方式 VV0 VV2 VV4 VV6 ……
							 IN BYTE* yuyv1,		// 第一行 yuyv 输入（权重 -1+3），存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
							 IN BYTE* yuyv2,		// 第二行 yuyv 输入（权重 3），存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
							 IN int yuyv_pitch,		// 两行 yuyv 输入间的偏移
							 IN int na8,			// 一行像素宽度除以 8 的商与余数（注意目标为 16 位的缓存）
							 IN int nm8);
// 中间行
void yuyv_to_yuv420_line_4(OUT BYTE* y,			// 第一行 y 输出，存放方式 Y0 Y1 Y2 Y3 ……
						   IN int y_pitch,		// 第二行 y 输出与第一行的偏移
						   OUT BYTE* u,			// 一行 u 输出，存放方式 UU0 UU2 UU4 UU6 ……
						   OUT BYTE* v,			// 一行 v 输出，存放方式 VV0 VV2 VV4 VV6 ……
						   IN BYTE* yuyv0,		// 上一行 yuyv 输入（权重 -1），存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
						   IN BYTE* yuyv2,		// 第二行 yuyv 输入（权重 3），存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
						   IN int yuyv_pitch,	// 两行 yuyv 输入间的偏移
						   IN int na8,			// 一行像素宽度除以 8 的商与余数（注意目标为 16 位的缓存）
						   IN int nm8);
// 最后一行
void yuyv_to_yuv420_line_4_L(OUT BYTE* y,			// 第一行 y 输出，存放方式 Y0 Y1 Y2 Y3 ……
							 IN int y_pitch,		// 第二行 y 输出与第一行的偏移
							 OUT BYTE* u,			// 一行 u 输出，存放方式 UU0 UU2 UU4 UU6 ……
							 OUT BYTE* v,			// 一行 v 输出，存放方式 VV0 VV2 VV4 VV6 ……
							 IN BYTE* yuyv0,		// 上一行 yuyv 输入（权重 -1），存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
							 IN BYTE* yuyv2,		// 第二行 yuyv 输入（权重 3-1），存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
							 IN int yuyv_pitch,		// 两行 yuyv 输入间的偏移
							 IN int na8,			// 一行像素宽度除以 8 的商与余数（注意目标为 16 位的缓存）
							 IN int nm8);
// 特殊的最后一行，一般不可能出现
void yuyv_to_yuv420_line_4_LS(OUT BYTE* y,			// 一行 y 输出，存放方式 Y0 Y1 Y2 Y3 ……
							  OUT BYTE* u,			// 一行 u 输出，存放方式 UU0 UU2 UU4 UU6 ……
							  OUT BYTE* v,			// 一行 v 输出，存放方式 VV0 VV2 VV4 VV6 ……
							  IN BYTE* yuyv0,		// 上一行 yuyv 输入（权重 -1-1），存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
							  IN BYTE* yuyv1,		// 第一行 yuyv 输入（权重 3+3），存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
							  IN int na8,			// 一行像素宽度除以 8 的商与余数（注意目标为 16 位的缓存）
							  IN int nm8);

void yuv422_to_yuyv_line(OUT BYTE* yuyv,        // yuyv 输出，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
						 IN BYTE* y,            // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
						 IN BYTE* u,            // u 输入，存放方式 U0 U2 U4 U6 ……
						 IN BYTE* v,            // v 输入，存放方式 V0 V2 V4 V6 ……
						 IN int na16,           // 一行像素宽度除以 16 的商与余数（注意目标为 16 位的缓存）
						 IN int nm16);

// 一行 Buffer 颜色空间转换

// 分片存储的源 YUV422 或 YUV420

// 前两行
void uv20_to_uv22_line_2_F(OUT void* u_o,         // 422 u 输出，存储方式 U0 U2 U4 U6 ……（两行）
                           IN int u_o_p,
                           OUT void* v_o,         // 422 v 输出，存储方式 V0 V2 V4 V6 ……（两行）
                           IN int v_o_p,
                           IN void* u_i,          // 420 u 输入，存储方式 UU0 UU2 UU4 UU6 ……
                           IN int u_i_p_2,
                           IN void* v_i,          // 420 v 输入，存储方式 VV0 VV2 VV4 VV6 ……
                           IN int v_i_p_2,
                           IN int na16,           // 一行像素宽度的一半（因为一行内的 UV 与 Y 为 422 的关系）除以 16 的商与余数
                           IN int nm16);
// 中间行
void uv20_to_uv22_line_2(OUT void* u_o,         // 422 u 输出，存储方式 U0 U2 U4 U6 ……（两行）
                         IN int u_o_p,
                         OUT void* v_o,         // 422 v 输出，存储方式 V0 V2 V4 V6 ……（两行）
                         IN int v_o_p,
                         IN void* u_i,          // 420 u 输入，存储方式 UU0 UU2 UU4 UU6 ……
                         IN int u_i_p_2,
                         IN void* v_i,          // 420 v 输入，存储方式 VV0 VV2 VV4 VV6 ……
                         IN int v_i_p_2,
                         IN int na16,           // 一行像素宽度的一半（因为一行内的 UV 与 Y 为 422 的关系）除以 16 的商与余数
                         IN int nm16);
// 最后两行
void uv20_to_uv22_line_2_L(OUT void* u_o,         // 422 u 输出，存储方式 U0 U2 U4 U6 ……（两行）
                           IN int u_o_p,
                           OUT void* v_o,         // 422 v 输出，存储方式 V0 V2 V4 V6 ……（两行）
                           IN int v_o_p,
                           IN void* u_i,          // 420 u 输入，存储方式 UU0 UU2 UU4 UU6 ……
                           IN int u_i_p_2,
                           IN void* v_i,          // 420 v 输入，存储方式 VV0 VV2 VV4 VV6 ……
                           IN int v_i_p_2,
                           IN int na16,           // 一行像素宽度的一半（因为一行内的 UV 与 Y 为 422 的关系）除以 16 的商与余数
                           IN int nm16);
// 最后一行
void uv20_to_uv22_line_1_L(OUT void* u_o,         // 422 u 输出，存储方式 U0 U2 U4 U6 ……（两行）
                           OUT void* v_o,         // 422 v 输出，存储方式 V0 V2 V4 V6 ……（两行）
                           IN void* u_i,          // 420 u 输入，存储方式 UU0 UU2 UU4 UU6 ……
                           IN int u_i_p_2,
                           IN void* v_i,          // 420 v 输入，存储方式 VV0 VV2 VV4 VV6 ……
                           IN int v_i_p_2,
                           IN int na16,           // 一行像素宽度的一半（因为一行内的 UV 与 Y 为 422 的关系）除以 16 的商与余数
                           IN int nm16);

void yuv422_to_color32_line(OUT void* color32,      // *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）（Alpha 直接填 0xff）
                            IN void* y,             // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
                            IN void* u,             // u 输入，存放方式 UU0 UU2 UU4 UU6 ……
                            IN void* v,             // v 输入，存放方式 VV0 VV2 VV4 VV6 ……
                            IN int na8, 		    // 源像素宽度减 1 后除以 8 的商与余数
                            IN int nm8,
                            IN int bHD,             // 高标清标志
                            IN BOOL bBGRA);         // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
void yuv422_to_color32_inner(OUT void* color32,     // *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
                             IN void* y,            // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
                             IN void* u,            // u 输入，存放方式 UU0 UU2 UU4 UU6 ……
                             IN void* v,            // v 输入，存放方式 VV0 VV2 VV4 VV6 ……
                             IN int na8,		    // 源像素宽度减 1 后除以 8 的商与余数
                             IN int nm8,
                             IN int* pnR_V,         // 系数
                             IN int* pnG_U,         // （通过交换 U 与 V 可以做到 RGBA 与 BGRA 的兼容）
                             IN int* pnG_V,         // （输入 u 与 v 交换，pnR_V 与 pnB_U 交换，pnG_V 与 pnG_U 交换即可实现）
                             IN int* pnB_U);

// 保留 Alpha 通道，直接丢弃第二个 UV
void color32_to_yuyva_line(OUT void* yuyv,		// 目标 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
                           OUT void* alpha,	    // 目标 alpha 缓存，存放方式 A0 A1 A2 A3 ……
                           IN void* color32,	// 源 *g@a 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
                           IN int na16,	    	// 源字节宽度除以 16 的商与余数
                           IN int nm16,
                           IN BOOL bHD,         // 高标清标志
                           IN BOOL bBGRA);      // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
void color32_to_yuyva_inner(OUT void* yuyv,	        // 目标 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
                            OUT void* alpha,	    // 目标 alpha 缓存，存放方式 A0 A1 A2 A3 ……
                            IN void* color32,	    // 源 *g@a 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
                            IN int na16,		    // 源字节宽度除以 16 的商与余数
                            IN int nm16,
                            IN int* pnY32_BR,       // 系数（通过变更 R 分量与 B 分量的系数位置可以做到 BGRA 与 RGBA 的兼容）
                            IN int* pnY32_G,        // BR 表示 R 在低位
                            IN int* pnY32_BGR,
                            IN int* pnUV_BGR,
                            IN int* pnU_BGR,
                            IN int* pnV_BGR);
// 丢弃 Alpha 通道，直接丢弃第二个 UV
void color32_to_yuyv_line(OUT void* yuyv,		// 目标 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
                          IN void* color32,	    // 源 *g@a 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
                          IN int na16,	    	// 源字节宽度除以 16 的商与余数
                          IN int nm16,
                          IN BOOL bHD,          // 高标清标志
                          IN BOOL bBGRA);       // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
void color32_to_yuyv_inner(OUT void* yuyv, 	    // 目标 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
                           IN void* color32,	// 源 *g@a 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
                           IN int na16,		    // 源字节宽度除以 16 的商与余数
                           IN int nm16,
                           IN int* pnY32_BR,    // 系数（通过变更 R 分量与 B 分量的系数位置可以做到 BGRA 与 RGBA 的兼容）
                           IN int* pnY32_G,     // BR 表示 R 在低位
                           IN int* pnY32_BGR,
                           IN int* pnUV_BGR,
                           IN int* pnU_BGR,
                           IN int* pnV_BGR);
// 使用 Alpha 通道，前后插值获得第二个 UV
void yuyva_to_color32_line(OUT void* color32,    // *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
                           IN void* yuyv,	 	 // 源 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
                           IN void* alpha,		 // 源 alpha 缓存，存放方式 A0 A1 A2 A3 ……
                           IN int na16,		     // 源字节宽度减 4 后除以 16 的商与余数
                           IN int nm16,
                           IN BOOL bHD,          // 高标清标志
                           IN BOOL bBGRA);        // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
void yuyva_to_rgba_inner(OUT void* rgba,        // RGBA 缓存，存放方式 R0 G0 B0 A0 R1 G1 B1 A1 ……
                         IN void* yuyv, 		// 源 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
                         IN void* alpha,		// 源 alpha 缓存，存放方式 A0 A1 A2 A3 ……
                         IN int na16,		    // 源字节宽度减 4 后除以 16 的商与余数
                         IN int nm16,
                         IN int* pnR_V,         // 系数（无法做到 RGBA 与 BGRA 的兼容）
                         IN int* pnG_U,       
                         IN int* pnG_V,
                         IN int* pnB_U);
void yuyva_to_bgra_inner(OUT void* bgra,        // BGRA 缓存，存放方式 B0 G0 R0 A0 B1 G1 R1 A1 ……
                         IN void* yuyv, 		// 源 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
                         IN void* alpha,		// 源 alpha 缓存，存放方式 A0 A1 A2 A3 ……
                         IN int na16,		    // 源字节宽度减 4 后除以 16 的商与余数
                         IN int nm16,
                         IN int* pnR_V,         // 系数（无法做到 RGBA 与 BGRA 的兼容）
                         IN int* pnG_U,       
                         IN int* pnG_V,
                         IN int* pnB_U);
// 补充 Alpha 通道为 255，前后插值获得第二个 UV
void yuyv_to_color32_line(OUT void* color32,    // *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）（Alpha 直接填 0xff）
                          IN void* yuyv,		// 源 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
                          IN int na16,			// 源字节宽度减 4 后除以 16 的商与余数
                          IN int nm16,
                          IN int bHD,           // 高标清标志
                          IN BOOL bBGRA);       // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
void yuyv_to_rgba_inner(OUT void* rgba,	        // RGBA 缓存，存放方式 R0 G0 B0 A0 R1 G1 B1 A1 ……
                          IN void* yuyv, 		// 源 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
                          IN int na16,		    // 源字节宽度减 4 后除以 16 的商与余数
                          IN int nm16,
                          IN int* pnR_V,        // 系数（无法做到 RGBA 与 BGRA 的兼容）
                          IN int* pnG_U,
                          IN int* pnG_V,
                          IN int* pnB_U);
void yuyv_to_bgra_inner(OUT void* bgra,     // BGRA 缓存，存放方式 B0 G0 R0 A0 B1 G1 R1 A1 ……
                        IN void* yuyv, 		// 源 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
                        IN int na16,		// 源字节宽度减 4 后除以 16 的商与余数
                        IN int nm16,
                        IN int* pnR_V,      // 系数（无法做到 RGBA 与 BGRA 的兼容）
                        IN int* pnG_U,
                        IN int* pnG_V,
                        IN int* pnB_U);
// 直接丢弃第二个 UV
void color24_to_yuyv_line(OUT void* yuyv,		// 目标 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
                          IN void* color24,		// *G@ 输入，存放方式 *0 G0 @0 *1 G1 @1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
                          IN int na12,			// 源字节宽度减 4 后除以 12 的商
                          IN int nmp,			// 做完 na12 循环后剩余的像素
                          IN int bHD,           // 高标清标志
                          IN BOOL bBGR);        // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
void color24_to_yuyv_inner(OUT void* yuyv,		// 目标 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
                           IN void* color24,	// *G@ 输入，存放方式 *0 G0 @0 *1 G1 @1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
                           IN int na12,			// 源字节宽度减 4 后除以 12 的商
                           IN int nmp,			// 做完 na12 循环后剩余的像素'
                           IN int* pnY24_RGB,   // 系数（通过变更 R 分量与 B 分量的系数位置可以做到 BGRA 与 RGBA 的兼容）
                           IN int* pnY32_BGR,
                           IN int* pnU_BGR,
                           IN int* pnV_BGR,
                           IN int* pnUV_BGR);
// 前后插值获得第二个 UV
void yuyv_to_color24_line(OUT void* color24,	// *G@ 缓存，存放方式 *0 G0 @0 *1 G1 @1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
                          IN void* yuyv,		// 源 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
                          IN int na16,			// 源字节宽度减 4 后除以 16 的商与余数
                          IN int nm16,
                          IN int bHD,           // 高标清标志
                          IN BOOL bBGR);        // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
void yuyv_to_color24_inner(OUT void* color24,   // *G@ 缓存，存放方式 *0 G0 @0 *1 G1 @1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
                           IN void* yuyv,		// 源 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
                           IN int na16,			// 源字节宽度减 4 后除以 16 的商与余数
                           IN int nm16,
                           IN int* pnR_V,       // 系数（通过变更 R 分量与 B 分量的系数位置可以做到 BGRA 与 RGBA 的兼容）
                           IN int* pnG_U,       // （此处只需要将 pnR_V 与 pnB_U 的输入交换即可实现）
                           IN int* pnG_V,
                           IN int* pnB_U);
// 补充 Alpha 通道为 0xff
void color24_to_color32(OUT void* color32,	// *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……
                        IN void* color24,	// *G@ 输入，存放方式 *0 G0 @0 *1 G1 @1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
                        IN int nap5,		// 源像素宽度除以 5 的商与余数（正好满足十六字节对齐）
                        IN int nmp5);
// 交换 RGBA 与 BGRA 的 R、B 通道
void rgb32_swaprbself(OUT void* rgb32,		// *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……
					  IN int pixelcount);	// 像素个数

//////////////////////////////////////////////////////////////////////////
// 平均插值缩放时的范围剪切
// 范围有效时返回 TRUE 无效时返回 FALSE
//////////////////////////////////////////////////////////////////////////
BOOL TPMeanStretch_CurRange(IN OUT int &nDestX,			// 目标起点
							IN OUT int &nDestY,
							IN OUT int &nDestWidth,		// 目标范围
							IN OUT int &nDestHeight,
							IN int nDestBufferWidth,	// 目标 Buffer 大小
							IN int nDestBufferHeight,
							IN OUT int &nSrcX,			// 源起点
							IN OUT int &nSrcY,
							IN OUT int &nSrcWidth,		// 源范围
							IN OUT int &nSrcHeight,
							IN int nSrcBufferWidth,		// 源 Buffer 大小
							IN int nSrcBufferHeight,
							OUT float &fScaleWidth,		// 缩放比例
							OUT float &fScaleHeight,
							OUT float &fSrcStartX,		// 源浮点起点坐标
							OUT float &fSrcStartY,
							OUT int &nDestAddWidth,		// 目标 Buffer 增量
							OUT int &nLineIndex,		// 行起始
							OUT int &nLineInc,			// 行增量
							IN int nParity);				// 奇偶场类型
//////////////////////////////////////////////////////////////////////////
// 用于加权平均缩放时计算权重的接口
//////////////////////////////////////////////////////////////////////////
// 计算水平或垂直方向上某个像素的权重个数及其首尾权重
int calc_power_pixel(OUT float* pPower,		// 首尾权重表
					 IN OUT float& fStart,	// 起始点坐标值
					 IN float fStep);		// 缩放步长
// 计算水平或垂直方向上各像素的权重个数表与首尾权重表
void strech_calc_power(OUT float* pPower,		// 权重表
					   OUT int* pMap,			// 每个点占用的权重个数表
					   IN int nCount,			// 目标拷贝长度或高度
					   IN float fStep,			// 缩放步长
					   IN float fStart = 0.0f);	// 起始点坐标值
// yuyv 到 bgra/rgba 的平均插值缩放变换，补充 Alpha 通道为 0xff
BOOL yuyv_to_color32_meanstretch(OUT BYTE* color32,		// *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）（Alpha 直接填 0xff）
								 IN int nDesPitch,
								 IN int nDesX,			// 目标起点
								 IN int nDesY,
								 IN int nDesWidth,		// 目标操作大小
								 IN int nDesHeight,
								 IN int nDesImgWidth,	// 目标图像大小
								 IN int nDesImgHeight,
								 IN BYTE* pSrcYUYV,		// YUYV 输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……）
								 IN int nSrcPitch,
								 IN int nSrcX,			// 源起点
								 IN int nSrcY,
								 IN int nSrcWidth,		// 源操作大小
								 IN int nSrcHeight,
								 IN int nSrcImgWidth,	// 源图像大小（由于源为16位，要求此宽度为偶数）
								 IN int nSrcImgHeight,
								 IN BOOL bSrcHD,		// 源信号的高标清标志（决定使用颜色变换的公式）
								 IN BOOL bBGRA,			// 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
								 IN BOOL bDesRevert);	// 图像是否行翻转标志
// 平均插值中临时缓存结果 YUYV 到 YUVY 的变换
void yuyv_mean_color32_midres(OUT void* yuvy,		// 浮点结果，存放方式 Y0 U0 V0 Y0 Y1 U1 V1 Y1 ……
							  IN void* yuyv,		// 浮点垂直叠加结果，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
							  IN int nCount);		// 像素点个数（必为偶数）
// 平均插值中水平方向加权求和，并将最终 YUV 的像素变换为 BGRA/RGBA 颜色。
void yuyv_mean_color32_add_x(OUT void* des,			// 目标 32 颜色行
							 IN int dcw,			// 目标宽度
							 IN void* src,			// 临时缓存行（由行加权得到的结果）
							 IN void* pPowerX,		// 水平方向上的权重表
							 IN void* pMapX,		// 水平方向上的映射表
							 IN void* pCoefW,		// 水平方向上的缩放系数
							 IN float* pfRB_YUV,	// YUVY 到 RB 变换的系数（Ry Bu Rv By）
							 IN float* pfG_YUV,		// YUVY 到 G 变换的系数（Gy Gu Gv 0）
							 IN BOOL bBGRA);		// BGRA/RGBA 颜色区别

//////////////////////////////////////////////////////////////////////////
//
// LaiShoubo @ 2009-11-5
//
// 按给定的A将24位转为32位
//
void color24_a_2_color32(OUT void* color32,			// 32位buffer
						 IN void* color24,			// 24位buffer
						 IN void* alpha,			// A通道
						 IN int count);				// 像素个数

//////////////////////////////////////////////////////////////////////////
// 文件结束