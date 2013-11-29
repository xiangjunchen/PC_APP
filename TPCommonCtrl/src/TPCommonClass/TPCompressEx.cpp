#include "stdafx.h"
#include <assert.h>
#include "tpcompressex.h"

//===================================================================
//	RLE - ѹ���㷨
//===================================================================

//////////////////////////////////////////////////////////////////////////
// ��������

#define RLE_MIN_REPEAT			3		// ��С�ظ����ݿ鳤��
#define RLE_MAX_REPEAT			130		// ����ظ����ݿ鳤��
#define RLE_MAX_NOREAPT			128		// ���ظ����ݿ���󳤶�
#define RLE_LEN_MASK			0x7f	// ���ݿ鳤������
#define RLE_REPEAT_ID			0x80	// �ظ����ݿ��ʶ

//////////////////////////////////////////////////////////////////////////
// �ӿ�ʵ��

/********************************************************************/
/*	��  �� ���ڲ� RLE ���룬���ؽ�ֹ��ѹ����ʽ��ʶ					*/
/*	˵  �� ��OutBuffer �����㹻�󣬴���InBuffer*1.5                 */
/*  ����ֵ ������:FALSE �ɹ���TRUE;                                 */
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

	BYTE *pOutStart = OutBuffer;	// �������ʼλ��
	BYTE *pNoRepeat = NULL;			// ���ظ����ݿ����ʼλ��
	register BYTE byCur = 0;		// ��ǰ�ַ�
	register BYTE nCount = 0;		// ���ݿ鳤��
	register BYTE nRepeat = 0;		// ͳ�Ʋ��ظ����ݿ�ʱ�����ظ����ݿ�ĳ��ȣ������ж��Ƿ���ֹ���ظ����ݿ��ͳ��

	while (dwInLen-- > 0)
	{
		nCount = 1;			// ��СΪ 1�����ظ��������Ϊ RLE_MAX_REPEAT �� RLE_MAX_NOREPEAT
		byCur = *InBuffer++;
		// �ظ��ֽڵĳ���ͳ��
		while ((nCount < RLE_MAX_REPEAT) && (dwInLen > 0))
		{
			if (*InBuffer != byCur)	// ����һ�ֽ��뵱ǰ�ֽڲ����ʱ˵���ظ�ͳ�ƽ���
			{
				break;
			}
			else		// ���������ǰͳ��
			{
				nCount++;
				InBuffer++;
				dwInLen--;
			}
		}
		// ����ֽڱ�����
		if ((nCount == 1) || ((nCount < RLE_MIN_REPEAT) && (dwInLen > 0)))
		{
			// �ظ����Ȳ��ϸ��Ҳ����ļ�����ʱҲ�����ظ�����
			// �趨���ظ����ݵ���ʼλ��
			pNoRepeat = InBuffer - nCount;
			// ͳ�Ʋ��ظ����ݿ�ĳ��ȣ���ǰ�ж�ͳ���ж�����
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
			// ������ظ����ݿ鳤�ȼ�����
			*OutBuffer++ = (nCount - 1);
			memcpy(OutBuffer, pNoRepeat, nCount);
			OutBuffer += nCount;
		}
		else		// �ظ����ݿ�ĳ��ȼ����벻�ظ�ʱ����ͬ
		{
			*OutBuffer++ = RLE_REPEAT_ID | (nCount - RLE_MIN_REPEAT);
			*OutBuffer++ = byCur;
		}
	}
	dwOutLen = (DWORD)(OutBuffer - pOutStart);

	return TRUE;
}

/********************************************************************/
/*	��  �� ���ڲ� RLE ���룬���ؽ����� BufferSize					*/
/*	˵  �� ��OutBuffer �����㹻��									*/
/*  ����ֵ ����ѹ������ݴ�С,��ѹ������0��										*/
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

	BYTE *pOutStart = OutBuffer;		// �������ʼλ��
	register BYTE nCount = 0;			// ���ݿ��ʶ������

	while (dwInLen-- > 0)
	{
		nCount = *InBuffer++;
		if (nCount & RLE_REPEAT_ID)		// �ظ����ݿ�
		{
			nCount = (nCount & RLE_LEN_MASK) + RLE_MIN_REPEAT;
			memset(OutBuffer, *InBuffer++, nCount);
			dwInLen--;
		}
		else				// ���ظ����ݿ�
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
//	LZ77 - ѹ���㷨
//===================================================================

//////////////////////////////////////////////////////////////////////////
// ��������

#define LZ77_BLOCK_HEADSIZE		2			// ���ݿ�ͷ��С
#define LZ77_BLOCK_END			0x0000		// �������ݿ�
#define LZ77_COMPRESS_METHOD	0x4000		// LZ77 ѹ����ʽ��־
#define LZ77_BLOCKSIZE_MASK		0x3fff		// ���ݿ��С����

#define LZ77_IDBIT_BUFLEN		1024		// ��ʶλ�����С��8K bits��
#define LZ77_DATA_BUFLEN		8192		// δƥ���ֽ���ƥ�䳤�Ȼ����С��ƥ����뻺���С
#define LZ77_IDBIT_OUTSIZE		512			// ��ʶλ������
#define LZ77_DATA_OUTSIZE		4096		// ���ݿ�������

#define LZ77_NO_MATCH			0			// û��ƥ���־
#define LZ77_MIN_MATCH			3			// ���ƥ��
#define LZ77_MAX_MATCH			258			// �ƥ��

#define LZ77_WINDOW_BITS		15			// ���ڴ�С��λ��
#define LZ77_WINDOW_SIZE		(1 << LZ77_WINDOW_BITS)		// ���ڴ�С
#define LZ77_WINDOW_MASK		0xffff		// ����λ������

#define LZ77_HASH_BITS			15			// ��ϣ���С��λ��
#define LZ77_HASH_SIZE			(1 << LZ77_HASH_BITS)		// ��ϣ���С
#define LZ77_HASH_MASK			0x7fff		// ��ϣֵ����
#define LZ77_HASH_SHIFT			5			// ��������¹�ϣֵ����λ

#define LZ77_IDBIT_SHIFT		4			// ���ֽڵ��ж�ƥ�����
#define LZ77_IDBIT_MASK			0xf			// ����ƥ�����������
static const BYTE LZ77_IDBIT_COUNT[0x00000001 << LZ77_IDBIT_SHIFT] = {
	0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4
};			// ����ֽ���ƥ�������

//////////////////////////////////////////////////////////////////////////
// �ڲ��ӿڶ���

// ��ϣֵ���㷽ʽ A(4,5)+A(6,7,8)^B(1,2,3)+B(4,5)+B(6,7,8)^C(1,2,3)+C(4,5,6,7,8)
#define LZ77_CALC_HASH(h, a, b, c)	\
	(h = ((((((DWORD)(a))<<LZ77_HASH_SHIFT)^((DWORD)(b)))<<LZ77_HASH_SHIFT)^((DWORD)(c)))&LZ77_HASH_MASK)
// ���¹�ϣֵ��ʽ ( ( hash_pre << 5 ) ^ next_char ) & 0x7fff
#define LZ77_UPDATE_HASH(h, c, ph, pm)	((h) = (((h)<<LZ77_HASH_SHIFT)^((DWORD)(c)))&LZ77_HASH_MASK, (pm) = (ph)[(h)])
// ��ȡ�������Чƥ��
#define LZ77_VALID_NEARPREV(pp, pm, s)	while(((pm)!=LZ77_NO_MATCH)&&((s)-(pm)<LZ77_MIN_MATCH)) (pm)=(pp)[(pm)&LZ77_HASH_MASK]
// �����ϣֵ�����¹�ϣ��
#define LZ77_INSERT_HASH(w, s, h, ph, pp, pm)	\
	(LZ77_UPDATE_HASH((h), ((w)[(s)+(LZ77_MIN_MATCH-1)]), (ph), (pm)), \
	(pp)[(s)&LZ77_HASH_MASK] = (ph)[(h)], (ph)[h] = (WORD)(s))
// ͳ��ƥ�����
#define LZ77_COUNT_MATCH(id, m, n)	\
{(m) = 0;  BYTE *pTmpId=(id); for(int i=0;i<(n);i++,pTmpId++){ \
	(m) += (LZ77_IDBIT_COUNT[(*pTmpId)&LZ77_IDBIT_MASK] + \
	LZ77_IDBIT_COUNT[((*pTmpId)>>LZ77_IDBIT_SHIFT)&LZ77_IDBIT_MASK]);}}
// ������ݿ�
#define LZ77_ENCODE_OUTPUTDATA(o, nSize, l, id, nId, dis, m)	\
{*(WORD*)(o) = (nSize) | LZ77_COMPRESS_METHOD; (o) += LZ77_BLOCK_HEADSIZE; memcpy((o), (id), (nId)); \
	(o) += (nId); memcpy((o), (l), (nSize)); (o) += (nSize); memcpy((o), (dis), (m)); (o) += (m);}
// �ж�ƥ���Ƿ���Ч
#define LZ77_ISVALID_MATCH(w, s, pm)	\
	((pm)!=LZ77_NO_MATCH&&((s)-(pm)<LZ77_WINDOW_SIZE)&&(w)[(s)]==(w)[(pm)]&&(w)[(s)+1]==(w)[(pm)+1]&&(w)[(s)+2]==(w)[(pm)+2])
// �ж�ƥ�䳤���Ƿ��Ѿ����
#define LZ77_IS_BESTMATCH(bm, len)	if((bm)==LZ77_MAX_MATCH||(bm)==(len)) break
// ������ǰ�����µ��ƥ��
#define LZ77_LONGEST_MATCH(w, len, s, pm, bm, md, bs, pp)	\
{ do{ BYTE* pTmpS = (w)+(s)+LZ77_MIN_MATCH, *pTmpP = (w)+(pm)+LZ77_MIN_MATCH; WORD i = LZ77_MIN_MATCH;\
	while((i<(len))&&(i<(s)-(pm))&&(i<LZ77_MAX_MATCH)&&(*pTmpP==*pTmpS)){ i++; pTmpS++; pTmpP++; } \
	if((bm)<i){ (bm) = i; (md) = (WORD)((s)-(pm)); (bs) = (s); LZ77_IS_BESTMATCH((bm), (len)); } \
	(pm) = (pp)[(pm)&LZ77_HASH_MASK]; }while(LZ77_ISVALID_MATCH((w), (s), (pm))); }
// ȫ����¹�ϣ��
#define LZ77_UPDATE_ALLHASH(w, s, ph, pp)	\
{ if((s)>LZ77_WINDOW_MASK){	(s) -= LZ77_WINDOW_SIZE; (w) += LZ77_WINDOW_SIZE; \
	WORD *tmpHead = (ph), *tmpPrev = (pp); for(int i=0;i<LZ77_HASH_SIZE;i++,tmpHead++,tmpPrev++){ \
	*tmpHead = (*tmpHead>LZ77_WINDOW_SIZE)?(*tmpHead-LZ77_WINDOW_SIZE):LZ77_NO_MATCH; \
	*tmpPrev = (*tmpPrev>LZ77_WINDOW_SIZE)?(*tmpPrev-LZ77_WINDOW_SIZE):LZ77_NO_MATCH; } } }
// ���δƥ����ֽ�
#define LZ77_OUTPUT_LITERAL(l, i, w, s, len)	((l)[(i)++] = (w)[(s)++], (len)--)
// ���ƥ����Ϣ��
#define LZ77_OUTPUT_MATCH(id, l, d, nl, nd, nmc, nmd, len)	\
	((id)[(nl)>>3] |= (1<<((nl)&0x07)), (l)[(nl)++] = (BYTE)((nmc)-LZ77_MIN_MATCH), (d)[(nd)++] = (nmd), (len) -= (nmc))



//////////////////////////////////////////////////////////////////////////
// �ӿ�ʵ��

//===================================================================
//	LZ77 - ѹ���㷨
//===================================================================

/********************************************************************/
/*	��  �� ���ڲ� LZ77 ���룬���ؽ�ֹ��ѹ����ʽ��ʶ					*/
/*	˵  �� ��OutBuffer ����Ԥ���� 0									*/
/*  ����ֵ ���ɹ���TRUE�� ʧ�ܣ�FALSE;							 	*/
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

	BYTE	*pOutStart = OutBuffer;					// �������ʼָ��
	BYTE	*window = InBuffer;						// �̶�������ʼָ��
	WORD	head[LZ77_HASH_SIZE] = {LZ77_NO_MATCH};	// ��ϣ��
	WORD	prev[LZ77_HASH_SIZE] = {LZ77_NO_MATCH};	// ��ϣ����ǰһ�ڵ�
	DWORD	ins_h = 0;								// ��ϣ����
	DWORD	strstart = 0;							// ���������ڹ̶������е�λ��
	BYTE	id[LZ77_IDBIT_BUFLEN] = {0};			// ��ʶλ����
	BYTE	l_l[LZ77_DATA_BUFLEN] = {0};			// δƥ���ֽ���ƥ�䳤�Ȼ���
	WORD	dis[LZ77_DATA_BUFLEN] = {0};			// ƥ����뻺��
	WORD	nDataIndex = 0;							// �����������
	WORD	nDistIndex = 0;							// �����������
	WORD	nMatchCount = 0;						// ƥ�����
	WORD	prevMatch = 0;							// ��һ��ƥ���ڴ����е�λ��

	// ��������ǰ�����ֽڲ�����ѹ����ֱ���������Ϊ�˷�������Ժ�Ĺ�ϣֵ��Ҫ�������ǵ�ֵ
	LZ77_CALC_HASH(ins_h, window[strstart], window[strstart+1], window[strstart+2]);
	strstart += LZ77_MIN_MATCH;
	for (int i=1; i<LZ77_MIN_MATCH; i++)
	{
		LZ77_INSERT_HASH(window, i, ins_h, head, prev, prevMatch);
	}
	memcpy(l_l, window, LZ77_MIN_MATCH);
	nDataIndex += LZ77_MIN_MATCH;
	dwInLen -= LZ77_MIN_MATCH;
	// ѹ�����е�ʣ�೤�Ȳ����Լ����ϣֵ
	while (LZ77_MIN_MATCH <= dwInLen)
	{
		// ѹ�����������ݿ�ﵽ�޶�ʱ������ݿ�
		if (nDataIndex >= LZ77_DATA_OUTSIZE)
		{
			nDataIndex -= LZ77_DATA_OUTSIZE;
			// ͳ�����ݿ��ڵ�ƥ�����
			LZ77_COUNT_MATCH(id + LZ77_IDBIT_OUTSIZE, nMatchCount, (nDataIndex + 7) >> 3);
			nMatchCount = nDistIndex - nMatchCount;
			nDistIndex -= nMatchCount;
			nMatchCount <<= 1;
			// д�����ݿ�ͷ����ʶλ��δƥ���ֽ���ƥ�䳤�ȡ�ƥ�����
			LZ77_ENCODE_OUTPUTDATA(OutBuffer, LZ77_DATA_OUTSIZE, l_l, id, LZ77_IDBIT_OUTSIZE, dis, nMatchCount);
			// ���»���
			memcpy(id, id + LZ77_IDBIT_OUTSIZE, (LZ77_IDBIT_BUFLEN - LZ77_IDBIT_OUTSIZE));
			memset(id + LZ77_IDBIT_OUTSIZE, 0, nDataIndex);
			memcpy(l_l, l_l + LZ77_DATA_OUTSIZE, (LZ77_DATA_BUFLEN - LZ77_DATA_OUTSIZE));
			memset(l_l + LZ77_DATA_OUTSIZE, 0, nDataIndex);
			memcpy(dis, (BYTE*)dis + nMatchCount, nMatchCount);
			memset((BYTE*)dis + nMatchCount, 0, (nDistIndex << 1));
		}
		// ���뵱ǰ��ϣֵ�����¹�ϣ��prevMatch �д洢��һ�����ܵ���Чƥ��
		LZ77_INSERT_HASH(window, strstart, ins_h, head, prev, prevMatch);
		// ��ȡ�������Чƥ��
		LZ77_VALID_NEARPREV(prev, prevMatch, strstart);
		// �ж��Ƿ������Чƥ��
		if (LZ77_ISVALID_MATCH(window, strstart, prevMatch))
		{
			WORD nBestMatch = 0;			// ��һ��ƥ��ĳ���
			WORD nMatchDist = 0;			// ��һ��ƥ��ľ���

			// ������ǰ��ϣֵ�µ��ƥ��
			LZ77_LONGEST_MATCH(window, dwInLen, strstart, prevMatch, nBestMatch, nMatchDist, strstart, prev);
#ifdef ZCP_SLOW
			// ����ڵ�ǰ��ϣֵ���ҵ�����Чƥ�䳤�Ȳ������ƥ�䳤�ȣ���ȡ����ƥ�䷽ʽ����������ƥ��
			if ((nBestMatch < LZ77_MAX_MATCH) && (nBestMatch < dwInLen) && (dwInLen > LZ77_MIN_MATCH))
			{
				WORD	nGreed = 0;					// ʵ�ʵ�̰�Ĵ���
				DWORD	lazy_h = ins_h;				// ����ƥ���ϣֵ
				WORD	nBestLazy = 0;				// ����ƥ�����󳤶�
				WORD	nBestDist = 0;				// ����ƥ��ľ���
				DWORD	nBestStart = strstart;		// �����ƥ�����ʼ��ַ
				DWORD	nLookAhead = dwInLen - 1;	// ��ǰʣ����ַ���
				DWORD	lazy_s = strstart + 1;		// ����ƥ����ʼ����

				// �����ϣֵ����ȡ�������Чƥ��
				LZ77_UPDATE_HASH(lazy_h, window[lazy_s + LZ77_MIN_MATCH - 1], head, prevMatch);
				LZ77_VALID_NEARPREV(prev, prevMatch, strstart);
				// ����ƥ����еĴ������ڵ�ǰ�������ƥ��ʱ������ƥ��ʧȥ����
				while (LZ77_ISVALID_MATCH(window, lazy_s, prevMatch) && (nLookAhead >= LZ77_MIN_MATCH))
				{
					nGreed++;
					// �������������ֽڵ��ƥ��
					LZ77_LONGEST_MATCH(window, nLookAhead, lazy_s, prevMatch, nBestLazy, nBestDist, nBestStart, prev);
					// �ж�����ƥ���Ƿ���Ҫ��������
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
				// �ж�����ƥ��Ľ���Ƿ������壬�Ƿ���Ҫ���г���ƥ��
				if (nBestMatch < nBestLazy)
				{
					nGreed = (WORD)(nBestStart - strstart);
					// �����м��ִ�������Թ���Сƥ�����ȡ��ͨƥ���ʩ
					while (nGreed >= LZ77_MIN_MATCH)
					{
						// ����֮ǰ nBestMatch �� nMatchDist �д�ŵļ���ǰ�������ƥ�䣬��ֱ��ʹ��
						if (nBestMatch >= nGreed)		// �жϳ���ƥ����������
						{
							LZ77_OUTPUT_MATCH(id, l_l, dis, nDataIndex, nDistIndex, nGreed, nMatchDist, dwInLen);
							while (nGreed--)
							{
								strstart++;
								// �жϴ����Ƿ񳬹��涨��С����������Ҫ���´������뼰��ϣ��
								LZ77_UPDATE_ALLHASH(window, strstart, head, prev);
								// ���뵱ǰ��ϣֵ�����¹�ϣ��Խ�����ƥ�䴮���׵�ַ����δ���г���ƥ���Ⱥ�
								LZ77_INSERT_HASH(window, strstart, ins_h, head, prev, prevMatch);
							}
							nGreed = 0;			// Ϊ�������¹�ϣ��Ĵ����ṩ��ȫ����
						}
						else
						{
							LZ77_OUTPUT_MATCH(id, l_l, dis, nDataIndex, nDistIndex, nBestMatch, nMatchDist, dwInLen);
							nGreed -= nBestMatch;
							while (nBestMatch--)
							{
								strstart++;
								// �жϴ����Ƿ񳬹��涨��С����������Ҫ���´������뼰��ϣ��
								LZ77_UPDATE_ALLHASH(window, strstart, head, prev);
								// ���뵱ǰ��ϣֵ�����¹�ϣ��Խ�����ƥ�䴮���׵�ַ����δ���г���ƥ���Ⱥ�
								LZ77_INSERT_HASH(window, strstart, ins_h, head, prev, prevMatch);
							}
							// ��ʱ prevMatch �д�ŵļ�Ϊ��ǰ������ǰһƥ�䣬�������ƥ��һ����Ч
							// �����ж��Ƿ��Կ�������ƥ�䣬�������ó��ƥ�䣬�˴�Ϊ����ƥ��Ĺؼ�
							if (nGreed >= LZ77_MIN_MATCH)
							{
								LZ77_VALID_NEARPREV(prev, prevMatch, strstart);
								// �Է���һ
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
						// ���δƥ����ֽ�
						LZ77_OUTPUT_LITERAL(l_l, nDataIndex, window, strstart, dwInLen);
						// �жϴ����Ƿ񳬹��涨��С����������Ҫ���´������뼰��ϣ��
						LZ77_UPDATE_ALLHASH(window, strstart, head, prev);
						// ���뵱ǰ��ϣֵ�����¹�ϣ��Խ�����ƥ�䴮���׵�ַ����δ��������ƥ���Ⱥ�
						LZ77_INSERT_HASH(window, strstart, ins_h, head, prev, prevMatch);
					}
					nBestMatch = nBestLazy;
					nMatchDist = nBestDist;
				}
			}
#endif
			// ��������ƥ�䲢���¹�ϣ��
			LZ77_OUTPUT_MATCH(id, l_l, dis, nDataIndex, nDistIndex, nBestMatch, nMatchDist, dwInLen);
			while (--nBestMatch)
			{
				strstart++;
				// �жϴ����Ƿ񳬹��涨��С����������Ҫ���´������뼰��ϣ��
				LZ77_UPDATE_ALLHASH(window, strstart, head, prev);
				// ���뵱ǰ��ϣֵ�����¹�ϣ��
				LZ77_INSERT_HASH(window, strstart, ins_h, head, prev, prevMatch);
			}
			// ��ֹ������뵱ǰ�����Ĺ�ϣֵ
			strstart++;
			// �жϴ����Ƿ񳬹��涨��С����������Ҫ���´������뼰��ϣ��
			LZ77_UPDATE_ALLHASH(window, strstart, head, prev);
		}
		else		// ��������Ч��ƥ���������ǰ�ֽ�
		{
			LZ77_OUTPUT_LITERAL(l_l, nDataIndex, window, strstart, dwInLen);
			// �жϴ����Ƿ񳬹��涨��С����������Ҫ���´������뼰��ϣ��
			LZ77_UPDATE_ALLHASH(window, strstart, head, prev);
		}
	}
	// �����󼸸�δƥ����ֽڵ����ݻ���
	memcpy(l_l + nDataIndex, window + strstart, dwInLen);
	nDataIndex += (WORD)dwInLen;
	// ����д滺���е�����
	if (nDataIndex > 0)
	{
		WORD nIdSize = (nDataIndex + 7) >> 3;	// �ȼ����ʶλ�Ĵ�С

		// д�����ݿ�ͷ����ʶλ��δƥ���ֽ���ƥ�䳤�ȡ�ƥ�����
		LZ77_ENCODE_OUTPUTDATA(OutBuffer, nDataIndex, l_l, id, nIdSize, dis, (nDistIndex << 1));
	}
	// ������ݿ������
	*(WORD*)OutBuffer = LZ77_BLOCK_END;
	OutBuffer += LZ77_BLOCK_HEADSIZE;
	dwOutLen = (DWORD)(OutBuffer - pOutStart);

	return TRUE;
}



/********************************************************************/
/*	��  �� ���ڲ� LZ77 ���룬���ؽ����� BufferSize				*/
/*	˵  �� ��OutBuffer �����㹻��									*/
/*  ����ֵ ���ɹ�����ѹ������ݴ�С��ʧ�ܣ�0						*/
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

	BYTE	*pOutStart = OutBuffer;				// �������ʼ��ַ
	WORD	*pHead = (WORD*)InBuffer;			// ����ͷ
	BYTE	id[LZ77_IDBIT_BUFLEN] = {0};		// ��ʶλ����
	BYTE	l_l[LZ77_DATA_BUFLEN] = {0};		// δƥ���ֽ���ƥ�䳤�Ȼ���
	WORD	dis[LZ77_DATA_BUFLEN] = {0};		// ƥ����뻺��
	WORD	nIdSize = 0;						// ��ʶλ�Ĵ�С
	WORD	nMatchCount = 0;					// ƥ�����
	WORD	nDistIndex = 0;						// ƥ���������

	while (*pHead != LZ77_BLOCK_END)
	{
		// ��ȡ���ݿ�
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
		// ��ѹ���ݿ�
		nDistIndex = 0;
		for (WORD i=0; i<*pHead; i++)
		{
			// Ϊƥ����Ϣ��
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
