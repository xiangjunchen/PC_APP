#include "stdafx.h"
#include <assert.h>
#include "tpcompressex.h"

//===================================================================
//	RLE - 压缩算法
//===================================================================

//////////////////////////////////////////////////////////////////////////
// 基本定义

#define RLE_MIN_REPEAT			3		// 最小重复数据块长度
#define RLE_MAX_REPEAT			130		// 最大重复数据块长度
#define RLE_MAX_NOREAPT			128		// 不重复数据块最大长度
#define RLE_LEN_MASK			0x7f	// 数据块长度掩码
#define RLE_REPEAT_ID			0x80	// 重复数据块标识

//////////////////////////////////////////////////////////////////////////
// 接口实现

/********************************************************************/
/*	功  能 ：内部 RLE 编码，返回禁止的压缩方式标识					*/
/*	说  明 ：OutBuffer 必须足够大，大于InBuffer*1.5                 */
/*  返回值 ：出错:FALSE 成功：TRUE;                                 */
/********************************************************************/
BOOL RLE_Encode(IN BYTE *InBuffer, IN DWORD dwInLen, OUT BYTE *OutBuffer, OUT DWORD &dwOutLen)
{
#ifdef _DEBUG
	assert(InBuffer && OutBuffer && dwInLen > 2);
#endif

	if (InBuffer && OutBuffer && dwInLen <= 2)
	{
		return FALSE;
	}

	BYTE *pOutStart = OutBuffer;	// 输出流起始位置
	BYTE *pNoRepeat = NULL;			// 不重复数据块的起始位置
	register BYTE byCur = 0;		// 当前字符
	register BYTE nCount = 0;		// 数据块长度
	register BYTE nRepeat = 0;		// 统计不重复数据块时遇到重复数据块的长度，用于判断是否终止不重复数据块的统计

	while (dwInLen-- > 0)
	{
		nCount = 1;			// 最小为 1（不重复），最大为 RLE_MAX_REPEAT 或 RLE_MAX_NOREPEAT
		byCur = *InBuffer++;
		// 重复字节的长度统计
		while ((nCount < RLE_MAX_REPEAT) && (dwInLen > 0))
		{
			if (*InBuffer != byCur)	// 当下一字节与当前字节不相等时说明重复统计结束
			{
				break;
			}
			else		// 否则持续向前统计
			{
				nCount++;
				InBuffer++;
				dwInLen--;
			}
		}
		// 输出字节编码流
		if ((nCount == 1) || ((nCount < RLE_MIN_REPEAT) && (dwInLen > 0)))
		{
			// 重复长度不合格且不是文件结束时也按不重复处理
			// 设定不重复数据的起始位置
			pNoRepeat = InBuffer - nCount;
			// 统计不重复数据块的长度，超前判断统计中断条件
			while ((nCount < RLE_MAX_NOREAPT) && (dwInLen > 0))
			{
				if ((dwInLen >= RLE_MIN_REPEAT)
					&& (*InBuffer == InBuffer[1])
					&& (*InBuffer == InBuffer[2]))
				{
					break;
				}
				nCount++;
				InBuffer++;
				dwInLen--;
			}
			// 输出不重复数据块长度及数据
			*OutBuffer++ = (nCount - 1);
			memcpy(OutBuffer, pNoRepeat, nCount);
			OutBuffer += nCount;
		}
		else		// 重复数据块的长度计算与不重复时不相同
		{
			*OutBuffer++ = RLE_REPEAT_ID | (nCount - RLE_MIN_REPEAT);
			*OutBuffer++ = byCur;
		}
	}
	dwOutLen = (DWORD)(OutBuffer - pOutStart);

	return TRUE;
}

/********************************************************************/
/*	功  能 ：内部 RLE 解码，返回解码后的 BufferSize					*/
/*	说  明 ：OutBuffer 必须足够大									*/
/*  返回值 ：解压后的数据大小,解压出错返回0；										*/
/********************************************************************/
DWORD RLE_Decode(IN BYTE *InBuffer, IN DWORD dwInLen, OUT BYTE *OutBuffer)
{
#ifdef _DEBUG
	assert(InBuffer && OutBuffer && dwInLen > 2);
#endif

	if (InBuffer && OutBuffer && dwInLen <= 2) 
	{
		return 0;
	}

	BYTE *pOutStart = OutBuffer;		// 输出流起始位置
	register BYTE nCount = 0;			// 数据块标识及长度

	while (dwInLen-- > 0)
	{
		nCount = *InBuffer++;
		if (nCount & RLE_REPEAT_ID)		// 重复数据块
		{
			nCount = (nCount & RLE_LEN_MASK) + RLE_MIN_REPEAT;
			memset(OutBuffer, *InBuffer++, nCount);
			dwInLen--;
		}
		else				// 不重复数据块
		{
#ifdef _DEBUG
			assert(dwInLen > nCount);
#endif
			if (dwInLen <= nCount)
			{
				return 0;
			}

			memcpy(OutBuffer, InBuffer, ++nCount);
			InBuffer += nCount;
			dwInLen -= nCount;
		}
		OutBuffer += nCount;
	}

	return (DWORD)(OutBuffer - pOutStart);
}






//===================================================================
//	LZ77 - 压缩算法
//===================================================================

//////////////////////////////////////////////////////////////////////////
// 基本定义

#define LZ77_BLOCK_HEADSIZE		2			// 数据块头大小
#define LZ77_BLOCK_END			0x0000		// 结束数据块
#define LZ77_COMPRESS_METHOD	0x4000		// LZ77 压缩方式标志
#define LZ77_BLOCKSIZE_MASK		0x3fff		// 数据块大小掩码

#define LZ77_IDBIT_BUFLEN		1024		// 标识位缓存大小（8K bits）
#define LZ77_DATA_BUFLEN		8192		// 未匹配字节与匹配长度缓存大小、匹配距离缓存大小
#define LZ77_IDBIT_OUTSIZE		512			// 标识位最大输出
#define LZ77_DATA_OUTSIZE		4096		// 数据块最大输出

#define LZ77_NO_MATCH			0			// 没有匹配标志
#define LZ77_MIN_MATCH			3			// 最短匹配
#define LZ77_MAX_MATCH			258			// 最长匹配

#define LZ77_WINDOW_BITS		15			// 窗口大小的位数
#define LZ77_WINDOW_SIZE		(1 << LZ77_WINDOW_BITS)		// 窗口大小
#define LZ77_WINDOW_MASK		0xffff		// 窗口位置掩码

#define LZ77_HASH_BITS			15			// 哈希表大小的位数
#define LZ77_HASH_SIZE			(1 << LZ77_HASH_BITS)		// 哈希表大小
#define LZ77_HASH_MASK			0x7fff		// 哈希值掩码
#define LZ77_HASH_SHIFT			5			// 计算与更新哈希值的移位

#define LZ77_IDBIT_SHIFT		4			// 半字节的判断匹配个数
#define LZ77_IDBIT_MASK			0xf			// 计算匹配个数的掩码
static const BYTE LZ77_IDBIT_COUNT[0x00000001 << LZ77_IDBIT_SHIFT] = {
	0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4
};			// 半个字节内匹配个数表

//////////////////////////////////////////////////////////////////////////
// 内部接口定义

// 哈希值计算方式 A(4,5)+A(6,7,8)^B(1,2,3)+B(4,5)+B(6,7,8)^C(1,2,3)+C(4,5,6,7,8)
#define LZ77_CALC_HASH(h, a, b, c)	\
	(h = ((((((DWORD)(a))<<LZ77_HASH_SHIFT)^((DWORD)(b)))<<LZ77_HASH_SHIFT)^((DWORD)(c)))&LZ77_HASH_MASK)
// 更新哈希值方式 ( ( hash_pre << 5 ) ^ next_char ) & 0x7fff
#define LZ77_UPDATE_HASH(h, c, ph, pm)	((h) = (((h)<<LZ77_HASH_SHIFT)^((DWORD)(c)))&LZ77_HASH_MASK, (pm) = (ph)[(h)])
// 获取最近的有效匹配
#define LZ77_VALID_NEARPREV(pp, pm, s)	while(((pm)!=LZ77_NO_MATCH)&&((s)-(pm)<LZ77_MIN_MATCH)) (pm)=(pp)[(pm)&LZ77_HASH_MASK]
// 插入哈希值并更新哈希表
#define LZ77_INSERT_HASH(w, s, h, ph, pp, pm)	\
	(LZ77_UPDATE_HASH((h), ((w)[(s)+(LZ77_MIN_MATCH-1)]), (ph), (pm)), \
	(pp)[(s)&LZ77_HASH_MASK] = (ph)[(h)], (ph)[h] = (WORD)(s))
// 统计匹配个数
#define LZ77_COUNT_MATCH(id, m, n)	\
{(m) = 0;  BYTE *pTmpId=(id); for(int i=0;i<(n);i++,pTmpId++){ \
	(m) += (LZ77_IDBIT_COUNT[(*pTmpId)&LZ77_IDBIT_MASK] + \
	LZ77_IDBIT_COUNT[((*pTmpId)>>LZ77_IDBIT_SHIFT)&LZ77_IDBIT_MASK]);}}
// 输出数据块
#define LZ77_ENCODE_OUTPUTDATA(o, nSize, l, id, nId, dis, m)	\
{*(WORD*)(o) = (nSize) | LZ77_COMPRESS_METHOD; (o) += LZ77_BLOCK_HEADSIZE; memcpy((o), (id), (nId)); \
	(o) += (nId); memcpy((o), (l), (nSize)); (o) += (nSize); memcpy((o), (dis), (m)); (o) += (m);}
// 判断匹配是否有效
#define LZ77_ISVALID_MATCH(w, s, pm)	\
	((pm)!=LZ77_NO_MATCH&&((s)-(pm)<LZ77_WINDOW_SIZE)&&(w)[(s)]==(w)[(pm)]&&(w)[(s)+1]==(w)[(pm)+1]&&(w)[(s)+2]==(w)[(pm)+2])
// 判断匹配长度是否已经最大
#define LZ77_IS_BESTMATCH(bm, len)	if((bm)==LZ77_MAX_MATCH||(bm)==(len)) break
// 搜索当前索引下的最长匹配
#define LZ77_LONGEST_MATCH(w, len, s, pm, bm, md, bs, pp)	\
{ do{ BYTE* pTmpS = (w)+(s)+LZ77_MIN_MATCH, *pTmpP = (w)+(pm)+LZ77_MIN_MATCH; WORD i = LZ77_MIN_MATCH;\
	while((i<(len))&&(i<(s)-(pm))&&(i<LZ77_MAX_MATCH)&&(*pTmpP==*pTmpS)){ i++; pTmpS++; pTmpP++; } \
	if((bm)<i){ (bm) = i; (md) = (WORD)((s)-(pm)); (bs) = (s); LZ77_IS_BESTMATCH((bm), (len)); } \
	(pm) = (pp)[(pm)&LZ77_HASH_MASK]; }while(LZ77_ISVALID_MATCH((w), (s), (pm))); }
// 全面更新哈希表
#define LZ77_UPDATE_ALLHASH(w, s, ph, pp)	\
{ if((s)>LZ77_WINDOW_MASK){	(s) -= LZ77_WINDOW_SIZE; (w) += LZ77_WINDOW_SIZE; \
	WORD *tmpHead = (ph), *tmpPrev = (pp); for(int i=0;i<LZ77_HASH_SIZE;i++,tmpHead++,tmpPrev++){ \
	*tmpHead = (*tmpHead>LZ77_WINDOW_SIZE)?(*tmpHead-LZ77_WINDOW_SIZE):LZ77_NO_MATCH; \
	*tmpPrev = (*tmpPrev>LZ77_WINDOW_SIZE)?(*tmpPrev-LZ77_WINDOW_SIZE):LZ77_NO_MATCH; } } }
// 输出未匹配的字节
#define LZ77_OUTPUT_LITERAL(l, i, w, s, len)	((l)[(i)++] = (w)[(s)++], (len)--)
// 输出匹配信息对
#define LZ77_OUTPUT_MATCH(id, l, d, nl, nd, nmc, nmd, len)	\
	((id)[(nl)>>3] |= (1<<((nl)&0x07)), (l)[(nl)++] = (BYTE)((nmc)-LZ77_MIN_MATCH), (d)[(nd)++] = (nmd), (len) -= (nmc))



//////////////////////////////////////////////////////////////////////////
// 接口实现

//===================================================================
//	LZ77 - 压缩算法
//===================================================================

/********************************************************************/
/*	功  能 ：内部 LZ77 编码，返回禁止的压缩方式标识					*/
/*	说  明 ：OutBuffer 必须预先置 0									*/
/*  返回值 ：成功：TRUE， 失败：FALSE;							 	*/
/********************************************************************/
BOOL LZ77_Encode(IN BYTE *InBuffer, IN DWORD dwInLen, OUT BYTE *OutBuffer, OUT DWORD &dwOutLen)
{
#ifdef _DEBUG
	assert(InBuffer && OutBuffer && dwInLen > 3);
#endif

	if (InBuffer && OutBuffer && dwInLen <= 3)
	{
		return FALSE;
	}

	BYTE	*pOutStart = OutBuffer;					// 输出流起始指针
	BYTE	*window = InBuffer;						// 固定窗口起始指针
	WORD	head[LZ77_HASH_SIZE] = {LZ77_NO_MATCH};	// 哈希表
	WORD	prev[LZ77_HASH_SIZE] = {LZ77_NO_MATCH};	// 哈希链表前一节点
	DWORD	ins_h = 0;								// 哈希索引
	DWORD	strstart = 0;							// 滑动窗口在固定窗口中的位置
	BYTE	id[LZ77_IDBIT_BUFLEN] = {0};			// 标识位缓存
	BYTE	l_l[LZ77_DATA_BUFLEN] = {0};			// 未匹配字节与匹配长度缓存
	WORD	dis[LZ77_DATA_BUFLEN] = {0};			// 匹配距离缓存
	WORD	nDataIndex = 0;							// 输出数据索引
	WORD	nDistIndex = 0;							// 输出距离索引
	WORD	nMatchCount = 0;						// 匹配个数
	WORD	prevMatch = 0;							// 上一次匹配在窗口中的位置

	// 输入流的前三个字节不参与压缩而直接输出，但为了方便计算以后的哈希值先要计算它们的值
	LZ77_CALC_HASH(ins_h, window[strstart], window[strstart+1], window[strstart+2]);
	strstart += LZ77_MIN_MATCH;
	for (int i=1; i<LZ77_MIN_MATCH; i++)
	{
		LZ77_INSERT_HASH(window, i, ins_h, head, prev, prevMatch);
	}
	memcpy(l_l, window, LZ77_MIN_MATCH);
	nDataIndex += LZ77_MIN_MATCH;
	dwInLen -= LZ77_MIN_MATCH;
	// 压缩进行到剩余长度不足以计算哈希值
	while (LZ77_MIN_MATCH <= dwInLen)
	{
		// 压缩产生的数据块达到限定时输出数据块
		if (nDataIndex >= LZ77_DATA_OUTSIZE)
		{
			nDataIndex -= LZ77_DATA_OUTSIZE;
			// 统计数据块内的匹配个数
			LZ77_COUNT_MATCH(id + LZ77_IDBIT_OUTSIZE, nMatchCount, (nDataIndex + 7) >> 3);
			nMatchCount = nDistIndex - nMatchCount;
			nDistIndex -= nMatchCount;
			nMatchCount <<= 1;
			// 写入数据块头、标识位、未匹配字节与匹配长度、匹配距离
			LZ77_ENCODE_OUTPUTDATA(OutBuffer, LZ77_DATA_OUTSIZE, l_l, id, LZ77_IDBIT_OUTSIZE, dis, nMatchCount);
			// 更新缓存
			memcpy(id, id + LZ77_IDBIT_OUTSIZE, (LZ77_IDBIT_BUFLEN - LZ77_IDBIT_OUTSIZE));
			memset(id + LZ77_IDBIT_OUTSIZE, 0, nDataIndex);
			memcpy(l_l, l_l + LZ77_DATA_OUTSIZE, (LZ77_DATA_BUFLEN - LZ77_DATA_OUTSIZE));
			memset(l_l + LZ77_DATA_OUTSIZE, 0, nDataIndex);
			memcpy(dis, (BYTE*)dis + nMatchCount, nMatchCount);
			memset((BYTE*)dis + nMatchCount, 0, (nDistIndex << 1));
		}
		// 插入当前哈希值并更新哈希表，prevMatch 中存储上一个可能的有效匹配
		LZ77_INSERT_HASH(window, strstart, ins_h, head, prev, prevMatch);
		// 获取最近的有效匹配
		LZ77_VALID_NEARPREV(prev, prevMatch, strstart);
		// 判断是否存在有效匹配
		if (LZ77_ISVALID_MATCH(window, strstart, prevMatch))
		{
			WORD nBestMatch = 0;			// 上一次匹配的长度
			WORD nMatchDist = 0;			// 上一次匹配的距离

			// 搜索当前哈希值下的最长匹配
			LZ77_LONGEST_MATCH(window, dwInLen, strstart, prevMatch, nBestMatch, nMatchDist, strstart, prev);
#ifdef ZCP_SLOW
			// 如果在当前哈希值下找到的有效匹配长度不是最大匹配长度，采取懒惰匹配方式搜索更长的匹配
			if ((nBestMatch < LZ77_MAX_MATCH) && (nBestMatch < dwInLen) && (dwInLen > LZ77_MIN_MATCH))
			{
				WORD	nGreed = 0;					// 实际的贪心次数
				DWORD	lazy_h = ins_h;				// 懒惰匹配哈希值
				WORD	nBestLazy = 0;				// 懒惰匹配的最大长度
				WORD	nBestDist = 0;				// 懒惰匹配的距离
				DWORD	nBestStart = strstart;		// 最长懒惰匹配的起始地址
				DWORD	nLookAhead = dwInLen - 1;	// 超前剩余的字符数
				DWORD	lazy_s = strstart + 1;		// 懒惰匹配起始索引

				// 计算哈希值并获取最近的有效匹配
				LZ77_UPDATE_HASH(lazy_h, window[lazy_s + LZ77_MIN_MATCH - 1], head, prevMatch);
				LZ77_VALID_NEARPREV(prev, prevMatch, strstart);
				// 懒惰匹配进行的次数大于当前索引的最长匹配时，超懒匹配失去意义
				while (LZ77_ISVALID_MATCH(window, lazy_s, prevMatch) && (nLookAhead >= LZ77_MIN_MATCH))
				{
					nGreed++;
					// 依次搜索后面字节的最长匹配
					LZ77_LONGEST_MATCH(window, nLookAhead, lazy_s, prevMatch, nBestLazy, nBestDist, nBestStart, prev);
					// 判断懒惰匹配是否需要继续进行
					if ((nBestStart - strstart >= nBestMatch) || (nBestLazy <= nBestMatch && nGreed >= nBestMatch))
					{
						break;
					}
					LZ77_IS_BESTMATCH(nBestLazy, nLookAhead);
					lazy_s++;
					nLookAhead--;
					LZ77_UPDATE_HASH(lazy_h, window[lazy_s + LZ77_MIN_MATCH - 1], head, prevMatch);
					LZ77_VALID_NEARPREV(prev, prevMatch, strstart);
				}
				// 判断懒惰匹配的结果是否有意义，是否需要进行超懒匹配
				if (nBestMatch < nBestLazy)
				{
					nGreed = (WORD)(nBestStart - strstart);
					// 处理中间字串，如果仍够最小匹配则采取普通匹配措施
					while (nGreed >= LZ77_MIN_MATCH)
					{
						// 由于之前 nBestMatch 与 nMatchDist 中存放的即当前索引的最长匹配，可直接使用
						if (nBestMatch >= nGreed)		// 判断超懒匹配的特殊情况
						{
							LZ77_OUTPUT_MATCH(id, l_l, dis, nDataIndex, nDistIndex, nGreed, nMatchDist, dwInLen);
							while (nGreed--)
							{
								strstart++;
								// 判断窗口是否超过规定大小，超过则需要更新窗口输入及哈希表
								LZ77_UPDATE_ALLHASH(window, strstart, head, prev);
								// 插入当前哈希值并更新哈希表，越界插入匹配串的首地址，与未进行超懒匹配稳合
								LZ77_INSERT_HASH(window, strstart, ins_h, head, prev, prevMatch);
							}
							nGreed = 0;			// 为后来更新哈希表的代码提供安全保障
						}
						else
						{
							LZ77_OUTPUT_MATCH(id, l_l, dis, nDataIndex, nDistIndex, nBestMatch, nMatchDist, dwInLen);
							nGreed -= nBestMatch;
							while (nBestMatch--)
							{
								strstart++;
								// 判断窗口是否超过规定大小，超过则需要更新窗口输入及哈希表
								LZ77_UPDATE_ALLHASH(window, strstart, head, prev);
								// 插入当前哈希值并更新哈希表，越界插入匹配串的首地址，与未进行超懒匹配稳合
								LZ77_INSERT_HASH(window, strstart, ins_h, head, prev, prevMatch);
							}
							// 此时 prevMatch 中存放的即为当前索引的前一匹配，而且这个匹配一定有效
							// 接着判断是否仍可以搜索匹配，如果是则得出最长匹配，此处为超懒匹配的关键
							if (nGreed >= LZ77_MIN_MATCH)
							{
								LZ77_VALID_NEARPREV(prev, prevMatch, strstart);
								// 以防万一
								if (!LZ77_ISVALID_MATCH(window, strstart, prevMatch))
								{
									break;
								}
								nBestMatch = 0;
								nMatchDist = 0;
								LZ77_LONGEST_MATCH(window, nGreed, strstart, prevMatch, nBestMatch, nMatchDist, strstart, prev);
							}
						}
					}
					while (nGreed--)
					{
						// 输出未匹配的字节
						LZ77_OUTPUT_LITERAL(l_l, nDataIndex, window, strstart, dwInLen);
						// 判断窗口是否超过规定大小，超过则需要更新窗口输入及哈希表
						LZ77_UPDATE_ALLHASH(window, strstart, head, prev);
						// 插入当前哈希值并更新哈希表，越界插入匹配串的首地址，与未进行懒惰匹配稳合
						LZ77_INSERT_HASH(window, strstart, ins_h, head, prev, prevMatch);
					}
					nBestMatch = nBestLazy;
					nMatchDist = nBestDist;
				}
			}
#endif
			// 编码输出最长匹配并更新哈希表
			LZ77_OUTPUT_MATCH(id, l_l, dis, nDataIndex, nDistIndex, nBestMatch, nMatchDist, dwInLen);
			while (--nBestMatch)
			{
				strstart++;
				// 判断窗口是否超过规定大小，超过则需要更新窗口输入及哈希表
				LZ77_UPDATE_ALLHASH(window, strstart, head, prev);
				// 插入当前哈希值并更新哈希表
				LZ77_INSERT_HASH(window, strstart, ins_h, head, prev, prevMatch);
			}
			// 防止超界插入当前索引的哈希值
			strstart++;
			// 判断窗口是否超过规定大小，超过则需要更新窗口输入及哈希表
			LZ77_UPDATE_ALLHASH(window, strstart, head, prev);
		}
		else		// 不存在有效的匹配则输出当前字节
		{
			LZ77_OUTPUT_LITERAL(l_l, nDataIndex, window, strstart, dwInLen);
			// 判断窗口是否超过规定大小，超过则需要更新窗口输入及哈希表
			LZ77_UPDATE_ALLHASH(window, strstart, head, prev);
		}
	}
	// 输出最后几个未匹配的字节到数据缓存
	memcpy(l_l + nDataIndex, window + strstart, dwInLen);
	nDataIndex += (WORD)dwInLen;
	// 输出残存缓存中的数据
	if (nDataIndex > 0)
	{
		WORD nIdSize = (nDataIndex + 7) >> 3;	// 先计算标识位的大小

		// 写入数据块头、标识位、未匹配字节与匹配长度、匹配距离
		LZ77_ENCODE_OUTPUTDATA(OutBuffer, nDataIndex, l_l, id, nIdSize, dis, (nDistIndex << 1));
	}
	// 输出数据块结束码
	*(WORD*)OutBuffer = LZ77_BLOCK_END;
	OutBuffer += LZ77_BLOCK_HEADSIZE;
	dwOutLen = (DWORD)(OutBuffer - pOutStart);

	return TRUE;
}



/********************************************************************/
/*	功  能 ：内部 LZ77 解码，返回解码后的 BufferSize				*/
/*	说  明 ：OutBuffer 必须足够大									*/
/*  返回值 ：成功：解压后的数据大小，失败：0						*/
/********************************************************************/

DWORD LZ77_Decode(IN BYTE *InBuffer, IN DWORD dwInLen, OUT BYTE *OutBuffer)
{
#ifdef _DEBUG
	assert(InBuffer && OutBuffer);
#endif
	if (!(InBuffer && OutBuffer)) 
	{
		return 0;
	}

	BYTE	*pOutStart = OutBuffer;				// 输出流起始地址
	WORD	*pHead = (WORD*)InBuffer;			// 数据头
	BYTE	id[LZ77_IDBIT_BUFLEN] = {0};		// 标识位缓存
	BYTE	l_l[LZ77_DATA_BUFLEN] = {0};		// 未匹配字节与匹配长度缓存
	WORD	dis[LZ77_DATA_BUFLEN] = {0};		// 匹配距离缓存
	WORD	nIdSize = 0;						// 标识位的大小
	WORD	nMatchCount = 0;					// 匹配个数
	WORD	nDistIndex = 0;						// 匹配距离索引

	while (*pHead != LZ77_BLOCK_END)
	{
		// 读取数据块
		*pHead &= LZ77_BLOCKSIZE_MASK;
		InBuffer += LZ77_BLOCK_HEADSIZE;
		nIdSize = (*pHead + 7) >> 3;
		memcpy(id, InBuffer, nIdSize);
		InBuffer += nIdSize;
		memcpy(l_l, InBuffer, *pHead);
		InBuffer += *pHead;
		LZ77_COUNT_MATCH(id, nMatchCount, nIdSize);
		nMatchCount <<= 1;
		memcpy(dis, InBuffer, nMatchCount);
		InBuffer += nMatchCount;
		// 解压数据块
		nDistIndex = 0;
		for (WORD i=0; i<*pHead; i++)
		{
			// 为匹配信息对
			if (id[i >> 3] & (1 << (i % 8)))
			{
				memcpy(OutBuffer, OutBuffer - dis[nDistIndex++], l_l[i] + LZ77_MIN_MATCH);
				OutBuffer += (l_l[i] + LZ77_MIN_MATCH);
			}
			else
			{
				*OutBuffer++ = l_l[i];
			}
		}
		pHead = (WORD*)InBuffer;
	}

	return (DWORD)(OutBuffer - pOutStart);
}
