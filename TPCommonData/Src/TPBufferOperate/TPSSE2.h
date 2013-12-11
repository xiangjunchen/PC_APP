// ���� SSE2 �Ż��㷨

#pragma once

//////////////////////////////////////////////////////////////////////////
// ��ʼ������
void InitSSEStaticCoef();

//////////////////////////////////////////////////////////////////////////
// ����һ�� Buffer
//////////////////////////////////////////////////////////////////////////
void memcpy_line(OUT void *des,		// Ŀ��
				 IN void *src,		// Դ
				 IN int na16,		// ��ȳ��� 16 ����
				 IN int nm16);		// ��ȳ��� 16 ������

//////////////////////////////////////////////////////////////////////////
// ���ֽ�Ϊ��λ����һ�� Buffer
//////////////////////////////////////////////////////////////////////////
void memset_char_line(OUT void *des,	// Ŀ��
					  IN char value,	// ����ֵ
					  IN int na16,		// ��ȳ��� 16 ����
					  IN int nm16);		// ��ȳ��� 16 ������

//////////////////////////////////////////////////////////////////////////
// ��˫��Ϊ��λ����һ�� Buffer
//////////////////////////////////////////////////////////////////////////
void memset_dword_line(OUT void *des,	// Ŀ��
					   IN DWORD value,	// ����ֵ
					   IN int na16,		// ��ȳ��� 16 ����
					   IN int nm16);	// ��ȳ��� 16 ������

//////////////////////////////////////////////////////////////////////////
// �����ֽ�
// ���Ѻ����������򣺵�һ�� n ��ʾ���Ѷ�����ֽڳ����ڶ��� n ��ʾ������ʼ�㣬
//		������ n ��ʾ�����ֽ��������� n ʱ��ʾ�������
//////////////////////////////////////////////////////////////////////////
void memsplit_n_n_n(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount,		// ���Ѷ���ĸ���
					IN int nUnitBytes,		// ���Ѷ�����ֽڳ�
					IN int nCellStart,		// ������ʼ��
					IN int nCellCount);		// �����ֽ���
// 2
void memsplit_2_0_1(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount);		// ���Ѷ���ĸ���
void memsplit_2_1_1(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount);		// ���Ѷ���ĸ���
// 4
void memsplit_4_0_1(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount);		// ���Ѷ���ĸ���
void memsplit_4_0_2(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount);		// ���Ѷ���ĸ���
void memsplit_4_0_3(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount);		// ���Ѷ���ĸ���
void memsplit_4_1_1(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount);		// ���Ѷ���ĸ���
void memsplit_4_1_2(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount);		// ���Ѷ���ĸ���
void memsplit_4_1_3(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount);		// ���Ѷ���ĸ���
void memsplit_4_2_1(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount);		// ���Ѷ���ĸ���
void memsplit_4_2_2(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount);		// ���Ѷ���ĸ���
void memsplit_4_3_1(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount);		// ���Ѷ���ĸ���
// 8
void memsplit_8_0_4(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount);		// ���Ѷ���ĸ���
void memsplit_8_4_4(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount);		// ���Ѷ���ĸ���
//////////////////////////////////////////////////////////////////////////
// �ϳ��ֽ�
// �ϳɺ����������򣺵�һ�� n ��ʾ�ϳɶ�����ֽڳ����ڶ��� n ��ʾ�ϳ���ʼ�㣬
//		������ n ��ʾ�ϳ��ֽ��������� n ʱ��ʾ����ϳ�
//////////////////////////////////////////////////////////////////////////
void memcompose_n_n_n(OUT void *des,			// Ŀ��
					  IN void *src,				// Դ
					  IN int nUnitCount,		// �ϳɶ���ĸ���
					  IN int nUnitBytes,		// �ϳɶ�����ֽڳ�
					  IN int nCellStart,		// �ϳ���ʼ��
					  IN int nCellCount);		// �ϳ��ֽ���
// 2
void memcompose_2_0_1(OUT void *des,			// Ŀ��
					  IN void *src,				// Դ
					  IN int nUnitCount);		// �ϳɶ���ĸ���
void memcompose_2_1_1(OUT void *des,			// Ŀ��
					  IN void *src,				// Դ
					  IN int nUnitCount);		// �ϳɶ���ĸ���
// 4
void memcompose_4_0_1(OUT void *des,			// Ŀ��
					  IN void *src,				// Դ
					  IN int nUnitCount);		// �ϳɶ���ĸ���
void memcompose_4_0_2(OUT void *des,			// Ŀ��
					  IN void *src,				// Դ
					  IN int nUnitCount);		// �ϳɶ���ĸ���
void memcompose_4_1_1(OUT void *des,			// Ŀ��
					  IN void *src,				// Դ
					  IN int nUnitCount);		// �ϳɶ���ĸ���
void memcompose_4_2_1(OUT void *des,			// Ŀ��
					  IN void *src,				// Դ
					  IN int nUnitCount);		// �ϳɶ���ĸ���
void memcompose_4_2_2(OUT void *des,			// Ŀ��
					  IN void *src,				// Դ
					  IN int nUnitCount);		// �ϳɶ���ĸ���
void memcompose_4_3_1(OUT void *des,			// Ŀ��
					  IN void *src,				// Դ
					  IN int nUnitCount);		// �ϳɶ���ĸ���
// 8
void memcompose_8_0_4(OUT void *des,			// Ŀ��
					  IN void *src,				// Դ
					  IN int nUnitCount);		// �ϳɶ���ĸ���
void memcompose_8_4_4(OUT void *des,			// Ŀ��
					  IN void *src,				// Դ
					  IN int nUnitCount);		// �ϳɶ���ĸ���

//////////////////////////////////////////////////////////////////////////
// ��һ�� BGRA32 Buffer ���� Alpha ��ɫ
// �Ż�����͸���� MA = SA + DA - SA * DA / 255 = SA + DA - (DA * SA + 255) >> 8
// ���õ����� KA = SA / MA = SA * 255 / (SA * 255 + DA * 255 - SA * DA)
// �Ż�������ɫֵ MC = (SC * SA + DC * (1 - SA / 255) * DA) / MA = (SC - DC) * KA + DC
// ������ƫ�Ƶ�ַ Offset = (SA * 256 + DA) * 4
//////////////////////////////////////////////////////////////////////////
void bgra32_line_mix(IN OUT void *des,		// Ŀ��
					 IN void *src,			// Դ
					 IN int na16,			// ��ȳ��� 16 ����
					 IN int nm16,			// ��ȳ��� 16 ������
					 IN void *ka_table);	// 256 * 256 * sizeof(float) �� KA ��

//////////////////////////////////////////////////////////////////////////
// ��һ�� BGRA32 Buffer ���� Alpha ����ɫ
// �Ż�����͸���� MA = (255 - SA) + DA - (255 - SA) * DA / 255 = 255 - SA + (DA * SA + 255) >> 8
// ���õ����� KA = (255 - SA) / MA = (65535 - SA * 255) / (65535 - SA * 255 + SA * DA)
// �Ż�������ɫֵ MC = (SC * (255 - SA) + DC * (1 - (255 - SA) / 255) * DA) / MA = (SC - DC) * KA + DC
// ������ƫ�Ƶ�ַ Offset = ((255 - SA) * 256 + DA) * 4
//////////////////////////////////////////////////////////////////////////
void bgra32_line_notmix(IN OUT void *des,		// Ŀ��
						IN void *src,			// Դ
						IN int na16,			// ��ȳ��� 16 ����
						IN int nm16,			// ��ȳ��� 16 ������
						IN void *ka_table);		// 256 * 256 * sizeof(float) �� KA ��

//////////////////////////////////////////////////////////////////////////
// ����˫���Բ�ֵ���� BGRA32 Buffer �������ţ�ʹ��Ȩ��������ӳ����������������ش���
// ͨ������ӳ���õ�Ŀ��������ӳ�䵽Դ�е���ʼλ�ã���������Ϊ�׵��ĸ��������ؽ��м�Ȩ��ֵ
// ���㹫ʽ		S00 * x * y				S10 * (1 - x) * y
//				S01 * x * (1 - y)		S11 * (1 - x) * (1 - y)
// D = S11 + (S00 - S10 - S01 + S11) * x * y + (S01 - S11) * x + (S10 - S11) * y
//////////////////////////////////////////////////////////////////////////
void bgra32_bl_copy_line(OUT void* des,		// Ŀ�꿽����
						 IN int dcw,		// Ŀ�꿽���еĿ��
						 IN void* src,		// Դ������
						 IN int src_pitch,	// Դͼһ�е��ֽ���
						 IN void* pPowerX,	// Ŀ��ӳ�䵽Դ�� X �����ϵ���Ȩ��
						 IN void* pMapX,	// Ŀ��ӳ�䵽Դ�� X �����ϵ�������
						 IN DWORD powerY);	// Ŀ����ӳ�䵽Դ�е���Ȩ��

//////////////////////////////////////////////////////////////////////////
// ���ü�Ȩ��ֵ�˲����� BGRA32 Buffer �������ţ�ʹ��Ȩ��������ӳ����������������ش���
//////////////////////////////////////////////////////////////////////////
// �Դ�ֱ������Ŀ����ӳ�䵽Դ���н��м�Ȩ����ֱ�����������ű�����
void bgra32_mean_copy_add_y(OUT void* des,			// ��ʱ�����У���Դ�п�����ȵ��ı���
							IN void* src,			// Դ������ʼ��
							IN int scw,				// Դ�������
							IN int src_pitch,		// Դ���ֽ���
							IN void* pPowerY,		// Ŀ��ĳ��ӳ��Ĵ�ֱ�����ϵ�Ȩ�ر�
							IN void* pCoefH,		// ��ֱ�����ϵ�����ϵ��
							IN int nMapY);			// Ŀ��һ��ӳ�䵽Դ������
// ����ֱ�������ű���С��255�ĸ�Ч�㷨��
void bgra32_mean_copy_add_y_quick(OUT void* des,		// ��ʱ�����У���Դ�п�����ȵ��ı���
								  IN void* src,			// Դ������ʼ��
								  IN int scw,			// Դ�������
								  IN int src_pitch,		// Դ���ֽ���
								  IN void* pPowerY,		// Ŀ��ĳ��ӳ��Ĵ�ֱ�����ϵ�Ȩ�ر�
								  IN void* pCoefH,		// ��ֱ�����ϵ�����ϵ��
								  IN int nMapY);		// Ŀ��һ��ӳ�䵽Դ������
// ��ˮƽ������Ŀ����ӳ�䵽Դ���н��м�Ȩ
void bgra32_mean_copy_add_x(OUT void* des,			// Ŀ�꿽����
							IN int dcw,				// Ŀ�꿽�����
							IN void* src,			// ��ʱ�����У����м�Ȩ�õ��Ľ����
							IN void* pPowerX,		// ˮƽ�����ϵ�Ȩ�ر�
							IN void* pMapX,			// ˮƽ�����ϵ�ӳ���
							IN void* pCoefW);		// ˮƽ�����ϵ�����ϵ��


//////////////////////////////////////////////////////////////////////////

// ��ɫ�任���𳡣������߽��жϣ�

void color32_to_yuyva_frame(OUT BYTE* pDesYUYV,         // YUYV ������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                            IN int nYUYVPitch,
                            OUT BYTE* pDesAlpha,        // Alpha ������洢��ʽ A0 A1 A2 A3 A4 ������
                            IN int nAlphaPitch,
                            IN BYTE* pSrc32,            // Դ *g@a ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
                            IN int nSrcPitch,           // Դһ�е��ֽڿ��
                            IN int nWidth,		        // Դͼ��С
                            IN int nHeight,
                            IN BOOL bHD,                // �߱����־
                            IN BOOL bSrcRevert,         // ͼ���Ƿ��з�ת��־
                            IN BOOL bBGRA);             // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
void color32_to_yuyva_split(OUT BYTE* pDesYUYVF,        // YUYV ��һ��������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                            IN int nYUYVPitchF,
                            OUT BYTE* pDesAlphaF,       // Alpha ��һ��������洢��ʽ A0 A1 A2 A3 A4 ������
                            IN int nAlphaPitchF,
                            OUT BYTE* pDesYUYVS,        // YUYV �ڶ���������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                            IN int nYUYVPitchS,
                            OUT BYTE* pDesAlphaS,       // Alpha �ڶ���������洢��ʽ A0 A1 A2 A3 A4 ������
                            IN int nAlphaPitchS,
                            IN BYTE* pSrc32,            // Դ *g@a ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
                            IN int nSrcPitch,           // Դһ�е��ֽڿ��
                            IN int nWidth,              // Դͼ��С
                            IN int nHeight,
                            IN BOOL bHD,                // �߱����־
                            IN BOOL bSrcRevert,         // ͼ���Ƿ��з�ת��־
                            IN BOOL bBGRA);             // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ

void color32_to_yuyv_frame(OUT BYTE* pDesYUYV,         // YUYV ������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                           IN int nYUYVPitch,
                           IN BYTE* pSrc32,            // Դ *g@a ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
                           IN int nSrcPitch,           // Դһ�е��ֽڿ��
                           IN int nWidth,		        // Դͼ��С
                           IN int nHeight,
                           IN BOOL bHD,                // �߱����־
                           IN BOOL bSrcRevert,         // ͼ���Ƿ��з�ת��־
                           IN BOOL bBGRA);             // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
void color32_to_yuyv_split(OUT BYTE* pDesYUYVF,        // YUYV ��һ��������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                           IN int nYUYVPitchF,
                           OUT BYTE* pDesYUYVS,        // YUYV �ڶ���������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                           IN int nYUYVPitchS,
                           IN BYTE* pSrc32,            // Դ *g@a ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
                           IN int nSrcPitch,           // Դһ�е��ֽڿ��
                           IN int nWidth,              // Դͼ��С
                           IN int nHeight,
                           IN BOOL bHD,                // �߱����־
                           IN BOOL bSrcRevert,         // ͼ���Ƿ��з�ת��־
                           IN BOOL bBGRA);             // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ

void yuyv_to_color32_compose(OUT BYTE* pDes32,          // *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ������Alpha ֱ���� 0xff��
                             IN int nDesPitch,
                             IN BYTE* pSrcYUYVF,        // YUYV ��һ�����루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                             IN int nFirstPitch,
                             IN BYTE* pSrcYUYVS,        // YUYV �ڶ������루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                             IN int nSecondPitch,
                             IN int nWidth,             // Ŀ���С
                             IN int nHeight,
                             IN BOOL bHD,               // �߱����־
                             IN BOOL bBGRA,            // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
							 IN BOOL bDstRevert);		// ͼ���Ƿ��з�ת��־

void yuyv_to_color32_frame(OUT BYTE* pDes32,          // *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ������Alpha ֱ���� 0xff��
                             IN int nDesPitch,
                             IN BYTE* pSrcYUYV,        // YUYV ���루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                             IN int nSrcPitch,
                             IN int nWidth,             // Ŀ���С
                             IN int nHeight,
                             IN BOOL bHD,               // �߱����־
                             IN BOOL bBGRA,            // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
							 IN BOOL bDstRevert);		// ͼ���Ƿ��з�ת��־

void color24_to_yuyv_frame(OUT BYTE* pDesYUYV,         // YUYV ������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                           IN int nYUYVPitch,
                           IN BYTE* pSrc24,            // *G@ ���룬��ŷ�ʽ *0 G0 @0 *1 G1 @1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
                           IN int nSrcPitch,           // Դһ�е��ֽڿ��
                           IN int nWidth,		       // Դͼ��С
                           IN int nHeight,
                           IN BOOL bHD,                // �߱����־
                           IN BOOL bSrcRevert,         // ͼ���Ƿ��з�ת��־
                           IN BOOL bBGR);              // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
void color24_to_yuyv_split(OUT BYTE* pDesFirst,        // ��һ��������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                           IN int nFirstPitch,
                           OUT BYTE* pDesSecond,       // �ڶ���������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                           IN int nSecondPitch,
                           IN BYTE* pSrc24,            // *G@ ���룬��ŷ�ʽ *0 G0 @0 *1 G1 @1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
                           IN int nSrcPitch,           // Դһ�е��ֽڿ�ȣ�����Ϊ 4 �ı�����
                           IN int nWidth,			   // Դͼ��С
                           IN int nHeight,
                           IN BOOL bHD,                // �߱����־
                           IN BOOL bSrcRevert,         // ͼ���Ƿ��з�ת��־
                           IN BOOL bBGR);              // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ

void yuyv_to_color24_frame(OUT BYTE* pDes24,			// *G@ ���棬��ŷ�ʽ *0 G0 @0  *1 G1 @1  ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
						   IN int nDesPitch,
						   IN BYTE* pSrcYUYV,			// YUYV ���루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
						   IN int nSrcPitch,
						   IN int nWidth,		        // Ŀ���С
						   IN int nHeight,
						   IN BOOL bHD,			        // �߱����־
						   IN BOOL bBGRA,				// ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
						   IN BOOL bDstRevert);			// ͼ���Ƿ��з�ת��־

void yuv420_to_color32_frame(OUT BYTE* pDes32,           // *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ������Alpha ֱ���� 0xff��
                             IN int nDesPitch,
                             IN BYTE* pSrcY,             // Y ���루�洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
                             IN int nSrcYPitch,
                             IN BYTE* pSrcU,             // U ���루�洢��ʽ UU0 UU2 UU4 �����������й���һ����
                             IN int nSrcUPitch,
                             IN BYTE* pSrcV,             // V ���루�洢��ʽ VV0 VV2 VV4 �����������й���һ����
                             IN int nSrcVPitch,
                             IN int nWidth,              // Ŀ���С
                             IN int nHeight,
                             IN BOOL bHD,                // �߱����־
                             IN BOOL bDesRevert,         // ͼ���Ƿ��з�ת��־
                             IN BOOL bBGRA);             // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
void yuv420_to_color32_split(OUT BYTE* pDesFirst,        // ��һ�� *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ������Alpha ֱ���� 0xff��
                             IN int nFirstPitch,
                             OUT BYTE* pDesSecond,       // �ڶ��� *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ������Alpha ֱ���� 0xff��
                             IN int nSecondPitch,
                             IN BYTE* pSrcY,             // Y ���루�洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
                             IN int nSrcYPitch,
                             IN BYTE* pSrcU,             // U ���루�洢��ʽ UU0 UU2 UU4 �����������й���һ����
                             IN int nSrcUPitch,
                             IN BYTE* pSrcV,             // V ���루�洢��ʽ VV0 VV2 VV4 �����������й���һ����
                             IN int nSrcVPitch,
                             IN int nWidth,              // Ŀ���С
                             IN int nHeight,
                             IN BOOL bHD,                // �߱����־
                             IN BOOL bDesRevert,         // ͼ���Ƿ��з�ת��־
                             IN BOOL bBGRA);             // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ

void yuv422_to_color32_frame(OUT BYTE* pDes32,           // *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ������Alpha ֱ���� 0xff��
                             IN int nDesPitch,
                             IN BYTE* pSrcY,             // Y ���루�洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
                             IN int nSrcYPitch,
                             IN BYTE* pSrcU,             // U ���루�洢��ʽ U0 U2 U4 ������
                             IN int nSrcUPitch,
                             IN BYTE* pSrcV,             // V ���루�洢��ʽ V0 V2 V4 ������
                             IN int nSrcVPitch,
                             IN int nWidth,              // Ŀ���С
                             IN int nHeight,
                             IN BOOL bHD,                // �߱����־
                             IN BOOL bDesRevert,         // ͼ���Ƿ��з�ת��־
                             IN BOOL bBGRA);             // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
void yuv422_to_color32_split(OUT BYTE* pDesFirst,        // ��һ�� *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ������Alpha ֱ���� 0xff��
                             IN int nFirstPitch,
                             OUT BYTE* pDesSecond,       // �ڶ��� *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ������Alpha ֱ���� 0xff��
                             IN int nSecondPitch,
                             IN BYTE* pSrcY,             // Y ���루�洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
                             IN int nSrcYPitch,
                             IN BYTE* pSrcU,             // U ���루�洢��ʽ U0 U2 U4 ������
                             IN int nSrcUPitch,
                             IN BYTE* pSrcV,             // V ���루�洢��ʽ V0 V2 V4 ������
                             IN int nSrcVPitch,
                             IN int nWidth,              // Ŀ���С
                             IN int nHeight,
                             IN BOOL bHD,                // �߱����־
                             IN BOOL bDesRevert,         // ͼ���Ƿ��з�ת��־
                             IN BOOL bBGRA);             // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ

// ��������

// ��Ҫ���������ڳ��ڸ��в�ֵ��ÿ���������
// ǰ����
void yuv420_to_yuyv_line_2_F(OUT BYTE* yuyv,        // yuyv �������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
                             IN int yuyv_p,
                             IN BYTE* y,            // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
                             IN int y_p_2,
                             IN BYTE* u,            // u ���룬��ŷ�ʽ UU0 UU2 UU4 UU6 ����
                             IN int u_p_2,
                             IN BYTE* v,            // v ���룬��ŷ�ʽ VV0 VV2 VV4 VV6 ����
                             IN int v_p_2,
                             IN int na16,           // һ�����ؿ�ȳ��� 16 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
                             IN int nm16);
// �м���
void yuv420_to_yuyv_line_2(OUT BYTE* yuyv,        // yuyv �������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
                           IN int yuyv_p,
                           IN BYTE* y,            // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
                           IN int y_p_2,
                           IN BYTE* u,            // u ���룬��ŷ�ʽ UU0 UU2 UU4 UU6 ����
                           IN int u_p_2,
                           IN BYTE* v,            // v ���룬��ŷ�ʽ VV0 VV2 VV4 VV6 ����
                           IN int v_p_2,
                           IN int na16,           // һ�����ؿ�ȳ��� 16 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
                           IN int nm16);
// �������
void yuv420_to_yuyv_line_2_L(OUT BYTE* yuyv,        // yuyv �������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
                             IN int yuyv_p,
                             IN BYTE* y,            // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
                             IN int y_p_2,
                             IN BYTE* u,            // u ���룬��ŷ�ʽ UU0 UU2 UU4 UU6 ����
                             IN int u_p_2,
                             IN BYTE* v,            // v ���룬��ŷ�ʽ VV0 VV2 VV4 VV6 ����
                             IN int v_p_2,
                             IN int na16,           // һ�����ؿ�ȳ��� 16 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
                             IN int nm16);
// ���һ��
void yuv420_to_yuyv_line_1_L(OUT BYTE* yuyv,        // yuyv �������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
                             IN BYTE* y,            // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
                             IN BYTE* u,            // u ���룬��ŷ�ʽ UU0 UU2 UU4 UU6 ����
                             IN int u_p_2,
                             IN BYTE* v,            // v ���룬��ŷ�ʽ VV0 VV2 VV4 VV6 ����
                             IN int v_p_2,
                             IN int na16,           // һ�����ؿ�ȳ��� 16 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
                             IN int nm16);

// XDCAM ��ֵ��ʽ��420->422 һ�в�ֵ�����У�ʹ�������У�����һ����ʽΪ
// F1_Out0 = 0.125 * F1_Prev + 0.875 * F1_Curr    F1_Out1 = 0.625 * F1_Curr + 0.375 * F1_Next
// ��һ��
void yuv420_to_yuyv_xdcam_F_line_2_F(OUT BYTE* yuyv,        // yuyv �������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
									 IN int yuyv_p,
									 IN BYTE* y,            // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
									 IN int y_p_2,
									 IN BYTE* u,            // u ���룬��ŷ�ʽ UU0 UU2 UU4 UU6 ����
									 IN int u_p_2,
									 IN BYTE* v,            // v ���룬��ŷ�ʽ VV0 VV2 VV4 VV6 ����
									 IN int v_p_2,
									 IN int na16,           // һ�����ؿ�ȳ��� 16 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
									 IN int nm16);
// �м���
void yuv420_to_yuyv_xdcam_F_line_2(OUT BYTE* yuyv,        // yuyv �������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
								   IN int yuyv_p,
								   IN BYTE* y,            // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
								   IN int y_p_2,
								   IN BYTE* u,            // u ���룬��ŷ�ʽ UU0 UU2 UU4 UU6 ����
								   IN int u_p_2,
								   IN BYTE* v,            // v ���룬��ŷ�ʽ VV0 VV2 VV4 VV6 ����
								   IN int v_p_2,
								   IN int na16,           // һ�����ؿ�ȳ��� 16 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
								   IN int nm16);
// ���һ��
void yuv420_to_yuyv_xdcam_F_line_1_L(OUT BYTE* yuyv,        // yuyv �������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
									 IN BYTE* y,            // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
									 IN BYTE* u,            // u ���룬��ŷ�ʽ UU0 UU2 UU4 UU6 ����
									 IN int u_p_2,
									 IN BYTE* v,            // v ���룬��ŷ�ʽ VV0 VV2 VV4 VV6 ����
									 IN int v_p_2,
									 IN int na16,           // һ�����ؿ�ȳ��� 16 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
									 IN int nm16);
// XDCAM ��ֵ��ʽ��420->422 һ�в�ֵ�����У�ʹ�������У����ڶ�����ʽΪ
// F2_Out0 = 0.375 * F1_Prev + 0.625 * F1_Curr    F1_Out1 = 0.875 * F1_Curr + 0.125 * F1_Next
// ��һ��
void yuv420_to_yuyv_xdcam_S_line_2_F(OUT BYTE* yuyv,        // yuyv �������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
									 IN int yuyv_p,
									 IN BYTE* y,            // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
									 IN int y_p_2,
									 IN BYTE* u,            // u ���룬��ŷ�ʽ UU0 UU2 UU4 UU6 ����
									 IN int u_p_2,
									 IN BYTE* v,            // v ���룬��ŷ�ʽ VV0 VV2 VV4 VV6 ����
									 IN int v_p_2,
									 IN int na16,           // һ�����ؿ�ȳ��� 16 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
									 IN int nm16);
// �м���
void yuv420_to_yuyv_xdcam_S_line_2(OUT BYTE* yuyv,        // yuyv �������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
								   IN int yuyv_p,
								   IN BYTE* y,            // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
								   IN int y_p_2,
								   IN BYTE* u,            // u ���룬��ŷ�ʽ UU0 UU2 UU4 UU6 ����
								   IN int u_p_2,
								   IN BYTE* v,            // v ���룬��ŷ�ʽ VV0 VV2 VV4 VV6 ����
								   IN int v_p_2,
								   IN int na16,           // һ�����ؿ�ȳ��� 16 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
								   IN int nm16);
// ���һ��
void yuv420_to_yuyv_xdcam_S_line_1_L(OUT BYTE* yuyv,        // yuyv �������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
									 IN BYTE* y,            // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
									 IN BYTE* u,            // u ���룬��ŷ�ʽ UU0 UU2 UU4 UU6 ����
									 IN int u_p_2,
									 IN BYTE* v,            // v ���룬��ŷ�ʽ VV0 VV2 VV4 VV6 ����
									 IN int v_p_2,
									 IN int na16,           // һ�����ؿ�ȳ��� 16 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
									 IN int nm16);
// XDCAM ��ֵ��ʽ��422->420 ���в�ֵ��һ�У���һ����ʽΪ F1_Out = 0.75 * F1_0 + 0.25 * F1_1
void yuyv_to_yuv420_xdcam_F_line_2(OUT BYTE* y,         // ��һ�� y �������ŷ�ʽ Y0 Y1 Y2 Y3 ����
								   IN int y_pitch,		// �ڶ��� y ������һ�е�ƫ��
								   OUT BYTE* u,			// һ�� u �������ŷ�ʽ UU0 UU2 UU4 UU6 ����
								   OUT BYTE* v,			// һ�� v �������ŷ�ʽ VV0 VV2 VV4 VV6 ����
								   IN BYTE* yuyv,		// ��һ�� yuyv ���룬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
								   IN int yuyv_pitch,	// �ڶ��� yuyv �������һ�е�ƫ��
								   IN int na8,          // һ�����ؿ�ȳ��� 8 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
								   IN int nm8);
// XDCAM ��ֵ��ʽ��422->420 ���в�ֵ��һ�У��ڶ�����ʽΪ F2_Out = 0.25 * F2_0 + 3 * F2_1
void yuyv_to_yuv420_xdcam_S_line_2(OUT BYTE* y,         // ��һ�� y �������ŷ�ʽ Y0 Y1 Y2 Y3 ����
								   IN int y_pitch,		// �ڶ��� y ������һ�е�ƫ��
								   OUT BYTE* u,			// һ�� u �������ŷ�ʽ UU0 UU2 UU4 UU6 ����
								   OUT BYTE* v,			// һ�� v �������ŷ�ʽ VV0 VV2 VV4 VV6 ����
								   IN BYTE* yuyv,		// ��һ�� yuyv ���룬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
								   IN int yuyv_pitch,	// �ڶ��� yuyv �������һ�е�ƫ��
								   IN int na8,          // һ�����ؿ�ȳ��� 8 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
								   IN int nm8);

void yuyv_to_yuv420_line_2_avr(OUT BYTE* y,         // ��һ�� y �������ŷ�ʽ Y0 Y1 Y2 Y3 ����
							   IN int y_pitch,		// �ڶ��� y ������һ�е�ƫ��
							   OUT BYTE* u,			// һ�� u �������ŷ�ʽ UU0 UU2 UU4 UU6 ����
							   OUT BYTE* v,			// һ�� v �������ŷ�ʽ VV0 VV2 VV4 VV6 ����
							   IN BYTE* yuyv,		// ��һ�� yuyv ���룬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
							   IN int yuyv_pitch,	// �ڶ��� yuyv �������һ�е�ƫ��
							   IN int na8,          // һ�����ؿ�ȳ��� 8 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
							   IN int nm8);
void yuyv_to_yuv420_line_1(OUT BYTE* y,         // һ�� y �������ŷ�ʽ Y0 Y1 Y2 Y3 ����
						   OUT BYTE* u,			// һ�� u �������ŷ�ʽ UU0 UU2 UU4 UU6 ����
						   OUT BYTE* v,			// һ�� v �������ŷ�ʽ VV0 VV2 VV4 VV6 ����
						   IN BYTE* yuyv,		// һ�� yuyv ���룬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
						   IN int na8,			// һ�����ؿ�ȳ��� 8 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
						   IN int nm8);
// ���ó������� -1, 3, 3, -1 ��ϵ���� UV ���в���
// ��һ��
void yuyv_to_yuv420_line_4_F(OUT BYTE* y,			// ��һ�� y �������ŷ�ʽ Y0 Y1 Y2 Y3 ����
							 IN int y_pitch,		// �ڶ��� y ������һ�е�ƫ��
							 OUT BYTE* u,			// һ�� u �������ŷ�ʽ UU0 UU2 UU4 UU6 ����
							 OUT BYTE* v,			// һ�� v �������ŷ�ʽ VV0 VV2 VV4 VV6 ����
							 IN BYTE* yuyv1,		// ��һ�� yuyv ���루Ȩ�� -1+3������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
							 IN BYTE* yuyv2,		// �ڶ��� yuyv ���루Ȩ�� 3������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
							 IN int yuyv_pitch,		// ���� yuyv ������ƫ��
							 IN int na8,			// һ�����ؿ�ȳ��� 8 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
							 IN int nm8);
// �м���
void yuyv_to_yuv420_line_4(OUT BYTE* y,			// ��һ�� y �������ŷ�ʽ Y0 Y1 Y2 Y3 ����
						   IN int y_pitch,		// �ڶ��� y ������һ�е�ƫ��
						   OUT BYTE* u,			// һ�� u �������ŷ�ʽ UU0 UU2 UU4 UU6 ����
						   OUT BYTE* v,			// һ�� v �������ŷ�ʽ VV0 VV2 VV4 VV6 ����
						   IN BYTE* yuyv0,		// ��һ�� yuyv ���루Ȩ�� -1������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
						   IN BYTE* yuyv2,		// �ڶ��� yuyv ���루Ȩ�� 3������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
						   IN int yuyv_pitch,	// ���� yuyv ������ƫ��
						   IN int na8,			// һ�����ؿ�ȳ��� 8 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
						   IN int nm8);
// ���һ��
void yuyv_to_yuv420_line_4_L(OUT BYTE* y,			// ��һ�� y �������ŷ�ʽ Y0 Y1 Y2 Y3 ����
							 IN int y_pitch,		// �ڶ��� y ������һ�е�ƫ��
							 OUT BYTE* u,			// һ�� u �������ŷ�ʽ UU0 UU2 UU4 UU6 ����
							 OUT BYTE* v,			// һ�� v �������ŷ�ʽ VV0 VV2 VV4 VV6 ����
							 IN BYTE* yuyv0,		// ��һ�� yuyv ���루Ȩ�� -1������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
							 IN BYTE* yuyv2,		// �ڶ��� yuyv ���루Ȩ�� 3-1������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
							 IN int yuyv_pitch,		// ���� yuyv ������ƫ��
							 IN int na8,			// һ�����ؿ�ȳ��� 8 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
							 IN int nm8);
// ��������һ�У�һ�㲻���ܳ���
void yuyv_to_yuv420_line_4_LS(OUT BYTE* y,			// һ�� y �������ŷ�ʽ Y0 Y1 Y2 Y3 ����
							  OUT BYTE* u,			// һ�� u �������ŷ�ʽ UU0 UU2 UU4 UU6 ����
							  OUT BYTE* v,			// һ�� v �������ŷ�ʽ VV0 VV2 VV4 VV6 ����
							  IN BYTE* yuyv0,		// ��һ�� yuyv ���루Ȩ�� -1-1������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
							  IN BYTE* yuyv1,		// ��һ�� yuyv ���루Ȩ�� 3+3������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
							  IN int na8,			// һ�����ؿ�ȳ��� 8 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
							  IN int nm8);

void yuv422_to_yuyv_line(OUT BYTE* yuyv,        // yuyv �������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
						 IN BYTE* y,            // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
						 IN BYTE* u,            // u ���룬��ŷ�ʽ U0 U2 U4 U6 ����
						 IN BYTE* v,            // v ���룬��ŷ�ʽ V0 V2 V4 V6 ����
						 IN int na16,           // һ�����ؿ�ȳ��� 16 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
						 IN int nm16);

// һ�� Buffer ��ɫ�ռ�ת��

// ��Ƭ�洢��Դ YUV422 �� YUV420

// ǰ����
void uv20_to_uv22_line_2_F(OUT void* u_o,         // 422 u ������洢��ʽ U0 U2 U4 U6 ���������У�
                           IN int u_o_p,
                           OUT void* v_o,         // 422 v ������洢��ʽ V0 V2 V4 V6 ���������У�
                           IN int v_o_p,
                           IN void* u_i,          // 420 u ���룬�洢��ʽ UU0 UU2 UU4 UU6 ����
                           IN int u_i_p_2,
                           IN void* v_i,          // 420 v ���룬�洢��ʽ VV0 VV2 VV4 VV6 ����
                           IN int v_i_p_2,
                           IN int na16,           // һ�����ؿ�ȵ�һ�루��Ϊһ���ڵ� UV �� Y Ϊ 422 �Ĺ�ϵ������ 16 ����������
                           IN int nm16);
// �м���
void uv20_to_uv22_line_2(OUT void* u_o,         // 422 u ������洢��ʽ U0 U2 U4 U6 ���������У�
                         IN int u_o_p,
                         OUT void* v_o,         // 422 v ������洢��ʽ V0 V2 V4 V6 ���������У�
                         IN int v_o_p,
                         IN void* u_i,          // 420 u ���룬�洢��ʽ UU0 UU2 UU4 UU6 ����
                         IN int u_i_p_2,
                         IN void* v_i,          // 420 v ���룬�洢��ʽ VV0 VV2 VV4 VV6 ����
                         IN int v_i_p_2,
                         IN int na16,           // һ�����ؿ�ȵ�һ�루��Ϊһ���ڵ� UV �� Y Ϊ 422 �Ĺ�ϵ������ 16 ����������
                         IN int nm16);
// �������
void uv20_to_uv22_line_2_L(OUT void* u_o,         // 422 u ������洢��ʽ U0 U2 U4 U6 ���������У�
                           IN int u_o_p,
                           OUT void* v_o,         // 422 v ������洢��ʽ V0 V2 V4 V6 ���������У�
                           IN int v_o_p,
                           IN void* u_i,          // 420 u ���룬�洢��ʽ UU0 UU2 UU4 UU6 ����
                           IN int u_i_p_2,
                           IN void* v_i,          // 420 v ���룬�洢��ʽ VV0 VV2 VV4 VV6 ����
                           IN int v_i_p_2,
                           IN int na16,           // һ�����ؿ�ȵ�һ�루��Ϊһ���ڵ� UV �� Y Ϊ 422 �Ĺ�ϵ������ 16 ����������
                           IN int nm16);
// ���һ��
void uv20_to_uv22_line_1_L(OUT void* u_o,         // 422 u ������洢��ʽ U0 U2 U4 U6 ���������У�
                           OUT void* v_o,         // 422 v ������洢��ʽ V0 V2 V4 V6 ���������У�
                           IN void* u_i,          // 420 u ���룬�洢��ʽ UU0 UU2 UU4 UU6 ����
                           IN int u_i_p_2,
                           IN void* v_i,          // 420 v ���룬�洢��ʽ VV0 VV2 VV4 VV6 ����
                           IN int v_i_p_2,
                           IN int na16,           // һ�����ؿ�ȵ�һ�루��Ϊһ���ڵ� UV �� Y Ϊ 422 �Ĺ�ϵ������ 16 ����������
                           IN int nm16);

void yuv422_to_color32_line(OUT void* color32,      // *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ������Alpha ֱ���� 0xff��
                            IN void* y,             // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
                            IN void* u,             // u ���룬��ŷ�ʽ UU0 UU2 UU4 UU6 ����
                            IN void* v,             // v ���룬��ŷ�ʽ VV0 VV2 VV4 VV6 ����
                            IN int na8, 		    // Դ���ؿ�ȼ� 1 ����� 8 ����������
                            IN int nm8,
                            IN int bHD,             // �߱����־
                            IN BOOL bBGRA);         // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
void yuv422_to_color32_inner(OUT void* color32,     // *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
                             IN void* y,            // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
                             IN void* u,            // u ���룬��ŷ�ʽ UU0 UU2 UU4 UU6 ����
                             IN void* v,            // v ���룬��ŷ�ʽ VV0 VV2 VV4 VV6 ����
                             IN int na8,		    // Դ���ؿ�ȼ� 1 ����� 8 ����������
                             IN int nm8,
                             IN int* pnR_V,         // ϵ��
                             IN int* pnG_U,         // ��ͨ������ U �� V �������� RGBA �� BGRA �ļ��ݣ�
                             IN int* pnG_V,         // ������ u �� v ������pnR_V �� pnB_U ������pnG_V �� pnG_U ��������ʵ�֣�
                             IN int* pnB_U);

// ���� Alpha ͨ����ֱ�Ӷ����ڶ��� UV
void color32_to_yuyva_line(OUT void* yuyv,		// Ŀ�� yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
                           OUT void* alpha,	    // Ŀ�� alpha ���棬��ŷ�ʽ A0 A1 A2 A3 ����
                           IN void* color32,	// Դ *g@a ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
                           IN int na16,	    	// Դ�ֽڿ�ȳ��� 16 ����������
                           IN int nm16,
                           IN BOOL bHD,         // �߱����־
                           IN BOOL bBGRA);      // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
void color32_to_yuyva_inner(OUT void* yuyv,	        // Ŀ�� yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
                            OUT void* alpha,	    // Ŀ�� alpha ���棬��ŷ�ʽ A0 A1 A2 A3 ����
                            IN void* color32,	    // Դ *g@a ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
                            IN int na16,		    // Դ�ֽڿ�ȳ��� 16 ����������
                            IN int nm16,
                            IN int* pnY32_BR,       // ϵ����ͨ����� R ������ B ������ϵ��λ�ÿ������� BGRA �� RGBA �ļ��ݣ�
                            IN int* pnY32_G,        // BR ��ʾ R �ڵ�λ
                            IN int* pnY32_BGR,
                            IN int* pnUV_BGR,
                            IN int* pnU_BGR,
                            IN int* pnV_BGR);
// ���� Alpha ͨ����ֱ�Ӷ����ڶ��� UV
void color32_to_yuyv_line(OUT void* yuyv,		// Ŀ�� yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
                          IN void* color32,	    // Դ *g@a ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
                          IN int na16,	    	// Դ�ֽڿ�ȳ��� 16 ����������
                          IN int nm16,
                          IN BOOL bHD,          // �߱����־
                          IN BOOL bBGRA);       // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
void color32_to_yuyv_inner(OUT void* yuyv, 	    // Ŀ�� yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
                           IN void* color32,	// Դ *g@a ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
                           IN int na16,		    // Դ�ֽڿ�ȳ��� 16 ����������
                           IN int nm16,
                           IN int* pnY32_BR,    // ϵ����ͨ����� R ������ B ������ϵ��λ�ÿ������� BGRA �� RGBA �ļ��ݣ�
                           IN int* pnY32_G,     // BR ��ʾ R �ڵ�λ
                           IN int* pnY32_BGR,
                           IN int* pnUV_BGR,
                           IN int* pnU_BGR,
                           IN int* pnV_BGR);
// ʹ�� Alpha ͨ����ǰ���ֵ��õڶ��� UV
void yuyva_to_color32_line(OUT void* color32,    // *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
                           IN void* yuyv,	 	 // Դ yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
                           IN void* alpha,		 // Դ alpha ���棬��ŷ�ʽ A0 A1 A2 A3 ����
                           IN int na16,		     // Դ�ֽڿ�ȼ� 4 ����� 16 ����������
                           IN int nm16,
                           IN BOOL bHD,          // �߱����־
                           IN BOOL bBGRA);        // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
void yuyva_to_rgba_inner(OUT void* rgba,        // RGBA ���棬��ŷ�ʽ R0 G0 B0 A0 R1 G1 B1 A1 ����
                         IN void* yuyv, 		// Դ yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
                         IN void* alpha,		// Դ alpha ���棬��ŷ�ʽ A0 A1 A2 A3 ����
                         IN int na16,		    // Դ�ֽڿ�ȼ� 4 ����� 16 ����������
                         IN int nm16,
                         IN int* pnR_V,         // ϵ�����޷����� RGBA �� BGRA �ļ��ݣ�
                         IN int* pnG_U,       
                         IN int* pnG_V,
                         IN int* pnB_U);
void yuyva_to_bgra_inner(OUT void* bgra,        // BGRA ���棬��ŷ�ʽ B0 G0 R0 A0 B1 G1 R1 A1 ����
                         IN void* yuyv, 		// Դ yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
                         IN void* alpha,		// Դ alpha ���棬��ŷ�ʽ A0 A1 A2 A3 ����
                         IN int na16,		    // Դ�ֽڿ�ȼ� 4 ����� 16 ����������
                         IN int nm16,
                         IN int* pnR_V,         // ϵ�����޷����� RGBA �� BGRA �ļ��ݣ�
                         IN int* pnG_U,       
                         IN int* pnG_V,
                         IN int* pnB_U);
// ���� Alpha ͨ��Ϊ 255��ǰ���ֵ��õڶ��� UV
void yuyv_to_color32_line(OUT void* color32,    // *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ������Alpha ֱ���� 0xff��
                          IN void* yuyv,		// Դ yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
                          IN int na16,			// Դ�ֽڿ�ȼ� 4 ����� 16 ����������
                          IN int nm16,
                          IN int bHD,           // �߱����־
                          IN BOOL bBGRA);       // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
void yuyv_to_rgba_inner(OUT void* rgba,	        // RGBA ���棬��ŷ�ʽ R0 G0 B0 A0 R1 G1 B1 A1 ����
                          IN void* yuyv, 		// Դ yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
                          IN int na16,		    // Դ�ֽڿ�ȼ� 4 ����� 16 ����������
                          IN int nm16,
                          IN int* pnR_V,        // ϵ�����޷����� RGBA �� BGRA �ļ��ݣ�
                          IN int* pnG_U,
                          IN int* pnG_V,
                          IN int* pnB_U);
void yuyv_to_bgra_inner(OUT void* bgra,     // BGRA ���棬��ŷ�ʽ B0 G0 R0 A0 B1 G1 R1 A1 ����
                        IN void* yuyv, 		// Դ yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
                        IN int na16,		// Դ�ֽڿ�ȼ� 4 ����� 16 ����������
                        IN int nm16,
                        IN int* pnR_V,      // ϵ�����޷����� RGBA �� BGRA �ļ��ݣ�
                        IN int* pnG_U,
                        IN int* pnG_V,
                        IN int* pnB_U);
// ֱ�Ӷ����ڶ��� UV
void color24_to_yuyv_line(OUT void* yuyv,		// Ŀ�� yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
                          IN void* color24,		// *G@ ���룬��ŷ�ʽ *0 G0 @0 *1 G1 @1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
                          IN int na12,			// Դ�ֽڿ�ȼ� 4 ����� 12 ����
                          IN int nmp,			// ���� na12 ѭ����ʣ�������
                          IN int bHD,           // �߱����־
                          IN BOOL bBGR);        // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
void color24_to_yuyv_inner(OUT void* yuyv,		// Ŀ�� yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
                           IN void* color24,	// *G@ ���룬��ŷ�ʽ *0 G0 @0 *1 G1 @1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
                           IN int na12,			// Դ�ֽڿ�ȼ� 4 ����� 12 ����
                           IN int nmp,			// ���� na12 ѭ����ʣ�������'
                           IN int* pnY24_RGB,   // ϵ����ͨ����� R ������ B ������ϵ��λ�ÿ������� BGRA �� RGBA �ļ��ݣ�
                           IN int* pnY32_BGR,
                           IN int* pnU_BGR,
                           IN int* pnV_BGR,
                           IN int* pnUV_BGR);
// ǰ���ֵ��õڶ��� UV
void yuyv_to_color24_line(OUT void* color24,	// *G@ ���棬��ŷ�ʽ *0 G0 @0 *1 G1 @1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
                          IN void* yuyv,		// Դ yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
                          IN int na16,			// Դ�ֽڿ�ȼ� 4 ����� 16 ����������
                          IN int nm16,
                          IN int bHD,           // �߱����־
                          IN BOOL bBGR);        // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
void yuyv_to_color24_inner(OUT void* color24,   // *G@ ���棬��ŷ�ʽ *0 G0 @0 *1 G1 @1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
                           IN void* yuyv,		// Դ yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
                           IN int na16,			// Դ�ֽڿ�ȼ� 4 ����� 16 ����������
                           IN int nm16,
                           IN int* pnR_V,       // ϵ����ͨ����� R ������ B ������ϵ��λ�ÿ������� BGRA �� RGBA �ļ��ݣ�
                           IN int* pnG_U,       // ���˴�ֻ��Ҫ�� pnR_V �� pnB_U �����뽻������ʵ�֣�
                           IN int* pnG_V,
                           IN int* pnB_U);
// ���� Alpha ͨ��Ϊ 0xff
void color24_to_color32(OUT void* color32,	// *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ����
                        IN void* color24,	// *G@ ���룬��ŷ�ʽ *0 G0 @0 *1 G1 @1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
                        IN int nap5,		// Դ���ؿ�ȳ��� 5 ��������������������ʮ���ֽڶ��룩
                        IN int nmp5);
// ���� RGBA �� BGRA �� R��B ͨ��
void rgb32_swaprbself(OUT void* rgb32,		// *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ����
					  IN int pixelcount);	// ���ظ���

//////////////////////////////////////////////////////////////////////////
// ƽ����ֵ����ʱ�ķ�Χ����
// ��Χ��Чʱ���� TRUE ��Чʱ���� FALSE
//////////////////////////////////////////////////////////////////////////
BOOL TPMeanStretch_CurRange(IN OUT int &nDestX,			// Ŀ�����
							IN OUT int &nDestY,
							IN OUT int &nDestWidth,		// Ŀ�귶Χ
							IN OUT int &nDestHeight,
							IN int nDestBufferWidth,	// Ŀ�� Buffer ��С
							IN int nDestBufferHeight,
							IN OUT int &nSrcX,			// Դ���
							IN OUT int &nSrcY,
							IN OUT int &nSrcWidth,		// Դ��Χ
							IN OUT int &nSrcHeight,
							IN int nSrcBufferWidth,		// Դ Buffer ��С
							IN int nSrcBufferHeight,
							OUT float &fScaleWidth,		// ���ű���
							OUT float &fScaleHeight,
							OUT float &fSrcStartX,		// Դ�����������
							OUT float &fSrcStartY,
							OUT int &nDestAddWidth,		// Ŀ�� Buffer ����
							OUT int &nLineIndex,		// ����ʼ
							OUT int &nLineInc,			// ������
							IN int nParity);				// ��ż������
//////////////////////////////////////////////////////////////////////////
// ���ڼ�Ȩƽ������ʱ����Ȩ�صĽӿ�
//////////////////////////////////////////////////////////////////////////
// ����ˮƽ��ֱ������ĳ�����ص�Ȩ�ظ���������βȨ��
int calc_power_pixel(OUT float* pPower,		// ��βȨ�ر�
					 IN OUT float& fStart,	// ��ʼ������ֵ
					 IN float fStep);		// ���Ų���
// ����ˮƽ��ֱ�����ϸ����ص�Ȩ�ظ���������βȨ�ر�
void strech_calc_power(OUT float* pPower,		// Ȩ�ر�
					   OUT int* pMap,			// ÿ����ռ�õ�Ȩ�ظ�����
					   IN int nCount,			// Ŀ�꿽�����Ȼ�߶�
					   IN float fStep,			// ���Ų���
					   IN float fStart = 0.0f);	// ��ʼ������ֵ
// yuyv �� bgra/rgba ��ƽ����ֵ���ű任������ Alpha ͨ��Ϊ 0xff
BOOL yuyv_to_color32_meanstretch(OUT BYTE* color32,		// *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ������Alpha ֱ���� 0xff��
								 IN int nDesPitch,
								 IN int nDesX,			// Ŀ�����
								 IN int nDesY,
								 IN int nDesWidth,		// Ŀ�������С
								 IN int nDesHeight,
								 IN int nDesImgWidth,	// Ŀ��ͼ���С
								 IN int nDesImgHeight,
								 IN BYTE* pSrcYUYV,		// YUYV ���루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ������
								 IN int nSrcPitch,
								 IN int nSrcX,			// Դ���
								 IN int nSrcY,
								 IN int nSrcWidth,		// Դ������С
								 IN int nSrcHeight,
								 IN int nSrcImgWidth,	// Դͼ���С������ԴΪ16λ��Ҫ��˿��Ϊż����
								 IN int nSrcImgHeight,
								 IN BOOL bSrcHD,		// Դ�źŵĸ߱����־������ʹ����ɫ�任�Ĺ�ʽ��
								 IN BOOL bBGRA,			// ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
								 IN BOOL bDesRevert);	// ͼ���Ƿ��з�ת��־
// ƽ����ֵ����ʱ������ YUYV �� YUVY �ı任
void yuyv_mean_color32_midres(OUT void* yuvy,		// ����������ŷ�ʽ Y0 U0 V0 Y0 Y1 U1 V1 Y1 ����
							  IN void* yuyv,		// ���㴹ֱ���ӽ������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
							  IN int nCount);		// ���ص��������Ϊż����
// ƽ����ֵ��ˮƽ�����Ȩ��ͣ��������� YUV �����ر任Ϊ BGRA/RGBA ��ɫ��
void yuyv_mean_color32_add_x(OUT void* des,			// Ŀ�� 32 ��ɫ��
							 IN int dcw,			// Ŀ����
							 IN void* src,			// ��ʱ�����У����м�Ȩ�õ��Ľ����
							 IN void* pPowerX,		// ˮƽ�����ϵ�Ȩ�ر�
							 IN void* pMapX,		// ˮƽ�����ϵ�ӳ���
							 IN void* pCoefW,		// ˮƽ�����ϵ�����ϵ��
							 IN float* pfRB_YUV,	// YUVY �� RB �任��ϵ����Ry Bu Rv By��
							 IN float* pfG_YUV,		// YUVY �� G �任��ϵ����Gy Gu Gv 0��
							 IN BOOL bBGRA);		// BGRA/RGBA ��ɫ����

//////////////////////////////////////////////////////////////////////////
//
// LaiShoubo @ 2009-11-5
//
// ��������A��24λתΪ32λ
//
void color24_a_2_color32(OUT void* color32,			// 32λbuffer
						 IN void* color24,			// 24λbuffer
						 IN void* alpha,			// Aͨ��
						 IN int count);				// ���ظ���

//////////////////////////////////////////////////////////////////////////
// �ļ�����