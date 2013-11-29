// SeMouseCalc.cpp: CTPMouseCalc �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TPMouseCalc.h"
#include <cmath>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace TPMouseCalcData
{
	const double DEFAULT_SPEED_TABLE[] = 
	{
		1.0
	,	0.85
	,	0.625
	,	0.45
	,	0.3
	,	0.2
	,	0.1
	,	0.08
	,	0.06
	,	0.04
	,	0.02
	,	0.01
	,	0.008
	,	0.006
	,	0.004
	,	0.002
	,	0.001
	};

	const DWORD		STCOK_FOR_PREVIOUS		= 3;
	const DWORD		STOCK_FOR_AVERAGE		= 10;
	const double	TOTAL_ADJUST			= 1.0;
	const double	INITIAL_MM_RATE			= 0.5;
	const double	INTERVAL_MM_RATE		= 0.8;
	const double	INITIAL_M2DP_RATE		= 1.0;
	const double	INTERVAL_M2DP_RATE		= 0.6;
	const double	MINIMUM_DIVISION		= 0.01;
	const double	INITIAL_FIRSTMM_RATE	= INITIAL_MM_RATE/5.0;
	const int		FIRSTMM_GEAR_NUM		= 10;

	// Root
	TCHAR SE_ROOT_REG[]				= _T("Software\\Sony\\DMW\\FxSliderSetting");

	// Value
	TCHAR SE_TOTAL_ADJ[]			= _T("TotalAdjust");
	TCHAR SE_FIRST_RATE[]			= _T("FirstRate");
	TCHAR SE_INTERVAL_RATE[]		= _T("IntervalRate");
	// DEBUG�p�B�����
	TCHAR SE_FG_COUNT[]				= _T("FG Count");
	TCHAR SE_FG_FIRST_RATE[]		= _T("FG FirstRate");
	TCHAR SE_DO_DBGDUMP[]			= _T("DbgDump");

	// SubKey
	TCHAR SE_GEAR_SETTING[]			= _T("GearSetting");
	TCHAR SE_NORMALIZE_SETTING[]	= _T("Normalize");

	// Sub-value
	TCHAR SE_AVG_COUNT[]			= _T("AverageCount");
	TCHAR SE_PREV_COUNT[]			= _T("PreviousCount");

	const double		TOTALADJ_MIN	= 0.0001;
	const double		TOTALADJ_MAX	= 3.0;
	const double		FIRSTRATE_MIN	= 0.0001;
	const double		FIRSTRATE_MAX	= 1.0;
	const double		INTERVALRATE_MIN	= 0.1;
	const double		INTERVALRATE_MAX	= 1.0;

	const double		M2SPEEDTABLE_MIN	= 0.0;
	const double		M2SPEEDTABLE_MAX	= 2.0;
	const int			AVGCOUNT_MIN	= 0;
	const int			AVGCOUNT_MAX	= 50;
	const int			PREVCOUNT_MIN	= 0;
	const int			PREVCOUNT_MAX	= 10;

	const int			M2SPTABLE_MAX	= 255;
}

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CTPMouseCalc::CTPMouseCalc()
:	m_dwAxis(SE_CALC_X)
,	m_vCumulativeVal(1)
{
}

CTPMouseCalc::~CTPMouseCalc()
{
}

//////////////////////////////////////////////////////////////////////
// �J�n�����BSlider�v�Z���s���ׂ̒l�����Z�b�g����

void CTPMouseCalc::Initialize()
{
	int nSize = (int)m_vCumulativeVal.size();
	for(int i=0; i< nSize;++i)
	{
		m_vCumulativeVal[i].cx = 0;
		m_vCumulativeVal[i].cy = 0;
	}
}

//////////////////////////////////////////////////////////////////////
// �I������

void CTPMouseCalc::Uninitialize()
{
}

//////////////////////////////////////////////////////////////////////
// �J�[�\���̈ړ��ʂ��A�ړ��ʁA�ړ��Ԋu����eTransmission�p��
// �p�ӂ���Table�ɗݐϒl�Ƃ��Ċi�[����

BOOL CTPMouseCalc::CalcCursorPos(int xMickey, int yMickey, DWORD dwTime,BOOL bCtrl)
{
	m_vCumulativeVal[0].cx += xMickey;
	m_vCumulativeVal[0].cy += yMickey;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ResetData()���Ăяo����Ă���ݐς������f�[�^�������Ŏ擾����

SEPOINTREAL	CTPMouseCalc::GetAccumulatePostionReal() const
{
	SEPOINTREAL pt = SEPOINTREAL(0.0,0.0);
	int nSize = (int)m_vCumulativeVal.size();
	for(int i=0; i< nSize;++i)
	{
		pt.x = (double)(m_vCumulativeVal[i].cx);
		pt.y = (double)(m_vCumulativeVal[i].cy);
	}
	return pt;
}

//////////////////////////////////////////////////////////////////////
// ResetData()���Ăяo����Ă���ݐς����}�E�X�f�o�C�X���̓f�[�^�𐮐��Ŏ擾����

CPoint CTPMouseCalc::GetAccumulatePosition() const
{
	SEPOINTREAL ptReal = GetAccumulatePostionReal();
	CPoint pt;
	pt.x = (int)(ptReal.x + (ptReal.x > 0)? 0.5 : -0.5);
	pt.y = (int)(ptReal.y + (ptReal.y > 0)? 0.5 : -0.5);
	return pt;
}

//////////////////////////////////////////////////////////////////////////////////
//////																		//////
//////					Intelligent Slider �v�Z�N���X�̎���					//////
//////																		//////
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CTPMouseCalcIntelligent::CTPMouseCalcIntelligent()
:	m_dwLastTime(0)
,	m_nLastAccumuIndex(0)
,	m_pNextGearForPrev(NULL)
#ifdef _INTELLIGENT_SLIDER_DEBUG_
,	m_bDbgDump(FALSE)
,	m_strDumpPath(_T(""))
#endif
{
	ResetDataAll();

	VERIFY(ReadDataRegistryAll());
}

CTPMouseCalcIntelligent::~CTPMouseCalcIntelligent()
{
}

//////////////////////////////////////////////////////////////////////
// �J�n�����BSlider�v�Z���s���ׂ̒l�����Z�b�g����

void CTPMouseCalcIntelligent::Initialize()
{
	CTPMouseCalc::Initialize();

	int i = 0;
	int nSize = 0;
	nSize = (int)m_vAccumuGearForAvg.size();
	for(i=0;i<nSize;++i){
		m_vAccumuGearForAvg[i] = SE_INVALIDE;
	}

	nSize = (int)m_vAccumeGearForPrev.size();
	for(i=0;i<nSize;++i){
		m_vAccumeGearForPrev[i].nGear = SE_INVALIDE;
	}

	m_nLastAccumuIndex	 = SE_INVALIDE;
	m_dwLastTime		 = ::GetTickCount();

#ifdef _INTELLIGENT_SLIDER_DEBUG_
	//////////////////////////////////////////////////
	// DEBUG�p�f�[�^�̏�����
	//////////////////////////////////////////////////
	if(m_bDbgDump)
	{
		nSize = m_vSelectGearCounter.size();
		for(i=0; i< nSize;++i)
		{
			m_vSelectGearCounter[i] = 0;
		}
		m_vRawDeviceData.clear();
		m_vAccumuAllGear.clear();
	}
#endif
}

//////////////////////////////////////////////////////////////////////
// �J�[�\���̈ړ��ʂ��A�ړ��ʁA�ړ��Ԋu����eTransmission�p��
// �p�ӂ���Table�ɗݐϒl�Ƃ��Ċi�[����

BOOL CTPMouseCalcIntelligent::CalcCursorPos(int xMickey, int yMickey, DWORD dwTime,BOOL bCtrl)
{
	int nCalcBase	= 0;
	int	nNonCalc	= 0;
	int nSelectGear = 0;

	switch(m_dwAxis)
	{
	case SE_CALC_Y:
		nCalcBase = yMickey;
		nNonCalc = xMickey;
		break;
	case SE_CALC_X:
		nCalcBase = xMickey;
		nNonCalc = yMickey;
		break;
	case SE_CALC_BOTH:
	default:
		if(xMickey > yMickey)
		{
			nCalcBase = xMickey;
			nNonCalc = yMickey;
		}
		else
		{
			nCalcBase = yMickey;
			nNonCalc = xMickey;
		}
		break;
	}

	if(nCalcBase){
		nSelectGear = CalcAdjustGear(nCalcBase, dwTime);
	}
	else{
		nSelectGear = CalcGear(nNonCalc, dwTime, FALSE,bCtrl);
	}

	m_vCumulativeVal[nSelectGear].cx += xMickey;
	m_vCumulativeVal[nSelectGear].cy += yMickey;

#ifdef _INTELLIGENT_SLIDER_DEBUG_
	// DEBUG:�I�����ꂽ�M���̉񐔂��L�^����
	if(m_bDbgDump && nCalcBase){
		m_vSelectGearCounter[nSelectGear]++;
	}
#endif

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ResetData()���Ăяo����Ă���ݐς����}�E�X�f�o�C�X���f�[�^(Mickey)
// ��Mickey->DevicePoint�ϊ��e�[�u���ϊ����Ď擾����

SEPOINTREAL	CTPMouseCalcIntelligent::GetAccumulatePostionReal() const
{
	SEPOINTREAL pt = SEPOINTREAL(0.0,0.0);

	int nSize = (int)m_vMickey2DPTable.size();
	for(int i=0;i<nSize;++i)
	{
		pt.x += double(m_vCumulativeVal[i].cx) * m_vMickey2DPTable[i] * m_sTableInfo.dTotalAdjust;
		pt.y += double(m_vCumulativeVal[i].cy) * m_vMickey2DPTable[i] * m_sTableInfo.dTotalAdjust;
	}
	return pt;
}

//////////////////////////////////////////////////////////////////////
// �J�[�\���̈ړ��ʂƃJ�[�\���ړ��̊Ԋu����K�؂�Gear���Z�o����

int CTPMouseCalcIntelligent::CalcAdjustGear(int nMickey, DWORD dwTime)
{
	int nCurGear = 0;
	int nAdjGear = 0;
	int nCompGear = SE_INVALIDE;
	int nCompSize = 0;

	nAdjGear = nCurGear = CalcGear(nMickey, dwTime, TRUE);

	////////////////////////////////////////////////////////////////////
	// ���x�ω����r����ׂ̒l�����߂�
	// Chain���ݒ肳��Ă���ꍇ�AChain������gear�̕��ς�nCompGear�Ƃ���
	// Chain���ݒ肳��Ă��Ȃ��ꍇ�A���O�̒l��O��nCompGear�Ƃ���
	////////////////////////////////////////////////////////////////////
	nCompSize = (int)m_vAccumeGearForPrev.size();

	if(nCompSize)
	{
		if(m_vAccumeGearForPrev[0].nGear == SE_INVALIDE)
		{
			m_vAccumeGearForPrev[0].nGear = nCurGear;
			m_pNextGearForPrev = m_vAccumeGearForPrev[0].pNext;
		}
		else
		{
			m_pNextGearForPrev->nGear = nCurGear;
			m_pNextGearForPrev = m_pNextGearForPrev->pNext;
			nCompGear = 0;
			int nChainCnt = 0;
			for(int i=0;i<nCompSize;++i)
			{
				if(m_vAccumeGearForPrev[i].nGear != SE_INVALIDE)
				{
					nCompGear += m_vAccumeGearForPrev[i].nGear;
					++nChainCnt;
				}
			}

			if(nChainCnt){
				nCompGear = (int)((double)nCompGear / nChainCnt + 0.5);
			}
		}
	}
	else if(m_nLastAccumuIndex != -1){
		nCompGear = m_vAccumuGearForAvg[m_nLastAccumuIndex];
	}

	int nIntervalSize = (int)m_vAccumuGearForAvg.size();

	++m_nLastAccumuIndex;

	if(m_nLastAccumuIndex >= nIntervalSize){
		m_nLastAccumuIndex = 0;
	}
	m_vAccumuGearForAvg[m_nLastAccumuIndex] = nCurGear;

	////////////////////////////////////////////////////////////////////
	// ��r�Ώۂ�gear�ƍ���I�����ꂽgear��2�ȏ㗣���ꍇ����gear�ɂ���
	///////////////////////////////////////////////////////////////////
	if((SE_INVALIDE != nCompGear) && (abs(nCompGear - nCurGear) >= 2))
	{
		int nStockNum = 0;
		nAdjGear = 0;
		for(int i=0;i<nIntervalSize;++i)
		{
			int nStockGear = m_vAccumuGearForAvg[i];
			if(SE_INVALIDE != nStockGear)
			{
				nAdjGear += nStockGear;
				++nStockNum;
			}
		}

		if(nStockNum){
			nAdjGear = (int)((double)nAdjGear / nStockNum + 0.5);
		}
		else{
			nAdjGear = nCurGear;
		}
	}

	ASSERT(nAdjGear < (int)m_vMickey2DPTable.size());

#ifdef _INTELLIGENT_SLIDER_DEBUG_
	// DEBUG:���肵���M����S�ċL�^���Ă���
	if(m_bDbgDump){
		m_vAccumuAllGear.push_back(nAdjGear);
	}
#endif

	return nAdjGear;
}

//////////////////////////////////////////////////////////////////////
// �}�E�X�f�o�C�X�̐��f�[�^(Mickey)�Ƃ��̓ǂݏo���Ԋu(msec)����K�؂�Gear���Z�o����

int CTPMouseCalcIntelligent::CalcGear(int nMickey, DWORD dwTime, BOOL bUpdateData,BOOL bCtrl)
{
	/*jyn added 20090509
	*comment:Slider mini adjust with control key down. set the time interval to 100.
	*/
	if(bCtrl)
		dwTime = m_dwLastTime+100;
	/*jyn added end*/
	DWORD dwDiff = dwTime - m_dwLastTime;

//	afxDump<<_T("~~~~~~~~~~~~~~~~")<<_T("dwDiff:")<<dwDiff<<_T("\n");

	double dBaseRate = 0.0;
	int nGear = (int)m_vMickey2DPTable.size() - 1;
	int i=0;

	double dMMRate = (double)abs(nMickey);
	if(dwDiff){
		dMMRate /= dwDiff;
	}

	//////////////////////////////////////////////////////////////
	// ���񔻒�
	//////////////////////////////////////////////////////////////
	if(m_nLastAccumuIndex == SE_INVALIDE && m_sTableInfo.nFGCount)
	{
		//////////////////////////////////////////////////////////////
		// ���񑀍��OnLButtonDown��OnMouseMove�Ƃ����Q�i�K�̑���ɂ�
		// ��̂ŁA�ӎ����Ă��Ȃ��Ƃ��ʏ펞��莞�Ԃ�������ׁA�ʏ�Rate
		// �Ōv�Z����Əd��Gear���I�������B����������ׁA����̂�
		// ��Table�Ŕ�����s��
		//////////////////////////////////////////////////////////////

		vector<double> vM2DPTable;

		dBaseRate		= m_sTableInfo.dFGInitialMMRate;
		int nTableNum	= m_sTableInfo.nFGCount;
		double dSqrt	= sqrt((double)nGear);
		double dPer		= 1.0 / nTableNum;

		for(i=0;i<nTableNum;++i){
			vM2DPTable.push_back( (dSqrt * (dPer * i)) );
		}

		for(i=0;i<nTableNum;++i)
		{
			if(dMMRate >= dBaseRate)
			{
				nGear = (int)(vM2DPTable[i] * vM2DPTable[i] + 0.5);
				break;
			}
			dBaseRate *= m_sTableInfo.dIntervalMMRate;
		}

		//////////////////////////////////////////////////////////////////////
		// ���ϒl�����߂�ׂ̉ߋ�gear�������̂ŁA����J�n���̕��������Ƀ���
		// ���o��̂�h���ׂɏ���M����������x�~�ς��Ă���
		//////////////////////////////////////////////////////////////////////
		if(bUpdateData)
		{
			m_nLastAccumuIndex = (int)m_vAccumuGearForAvg.size() / 3;
			for(i=0;i<=m_nLastAccumuIndex;++i){
				m_vAccumuGearForAvg[i] = nGear;
			}
		}
	}
	//////////////////////////////////////////////////////////////
	// �ʏ픻��
	//////////////////////////////////////////////////////////////
	else
	{
		dBaseRate = m_sTableInfo.dInitialMMRate;
		for(i=0;i<nGear;++i)
		{
			if(dMMRate >= dBaseRate)
			{
				nGear = i;
				break;
			}
			dBaseRate *= m_sTableInfo.dIntervalMMRate;
		}
	}

	if(bUpdateData)
	{
		m_dwLastTime = dwTime;

#ifdef _INTELLIGENT_SLIDER_DEBUG_
		// DEBUG: mickey / interval / gear��S�ċL�^���Ă���
		if(m_bDbgDump)
		{
			SEMOUSEDEICELOGINFO seInfo;
			seInfo.nMickey = nMickey;
			seInfo.dwTime = dwDiff;
			seInfo.nGear = nGear;
			m_vRawDeviceData.push_back(seInfo);
		}
#endif
	}

	return nGear;
}

//////////////////////////////////////////////////////////////////////
// Mickey->DevicePoint�ϊ��pTable�̐ݒ�

void CTPMouseCalcIntelligent::SetMickey2DPTable(const vector<double>& vMickey2DPTable)
{
	int nSize = (int)vMickey2DPTable.size();
	if(nSize != (int)m_vMickey2DPTable.size())
	{
		m_vMickey2DPTable.clear();
		m_vMickey2DPTable.resize(nSize);
	}
	
	for(int i=0;i<nSize;++i){
		m_vMickey2DPTable[i] = vMickey2DPTable[i];
	}

	m_vCumulativeVal.resize(nSize);
#ifdef _INTELLIGENT_SLIDER_DEBUG_
	// DEBUG:�I�����ꂽ�M���̃J�E���^���X�g���g������
	m_vSelectGearCounter.resize(nSize);
#endif
}

//////////////////////////////////////////////////////////////////////
// TableInfo�ݒ�

void CTPMouseCalcIntelligent::SetTableCalcInfo(const SETABLECALCINFO& seTableInfo)
{
	m_sTableInfo = seTableInfo;

	InitTableInfo();
}

//////////////////////////////////////////////////////////////////////
// Table������

void CTPMouseCalcIntelligent::InitTableInfo()
{
	InitNormalizeInfo();

	if(m_sTableInfo.dwTableType == SE_CALCULATE)
	{
		MakeMickey2DPTable(m_sTableInfo.dMinimumDivision, 
						   m_sTableInfo.dInitialM2DPRate,
						   m_sTableInfo.dIntervalM2DPRate,
						   m_vMickey2DPTable);

		int nSize = (int)m_vMickey2DPTable.size();

		m_vCumulativeVal.resize(nSize);
#ifdef _INTELLIGENT_SLIDER_DEBUG_
		// DEBUG:�I�����ꂽ�M���̃J�E���^���X�g���g������
		m_vSelectGearCounter.resize(nSize);
#endif
	}
	
}

//////////////////////////////////////////////////////////////////////
// ���������邽�߂̏��̏�����

void CTPMouseCalcIntelligent::InitNormalizeInfo()
{
	if(m_sTableInfo.nCountAverage != (int)m_vAccumuGearForAvg.size()){
		m_vAccumuGearForAvg.resize(m_sTableInfo.nCountAverage);
	}

	if(m_sTableInfo.nCountPrevious >0 && m_sTableInfo.nCountPrevious != (int)m_vAccumeGearForPrev.size())
	{
		m_vAccumeGearForPrev.clear();
		m_vAccumeGearForPrev.resize(m_sTableInfo.nCountPrevious);

		int nNext = 1;
		for(int i=0;i<m_sTableInfo.nCountPrevious;++i)
		{
			if(nNext == m_sTableInfo.nCountPrevious){
				m_vAccumeGearForPrev[i].pNext = &m_vAccumeGearForPrev[0];
			}
			else{
				m_vAccumeGearForPrev[i].pNext = &m_vAccumeGearForPrev[nNext++];
			}
		}

		m_pNextGearForPrev = &m_vAccumeGearForPrev[0];
	}
}

//////////////////////////////////////////////////////////////////////
// Mickey->DevicePoint�֕ϊ�����Table���쐬����

void CTPMouseCalcIntelligent::MakeMickey2DPTable(double dMinVal, double dInitialRate, double dIntervalRate, vector<double>& vM2DPTable)
{
	vM2DPTable.clear();

	if(dMinVal > 0.0)
	{
		double dM2DPRate = dInitialRate;
		vM2DPTable.push_back(dM2DPRate);

		while(TRUE)
		{
			if(dMinVal > dM2DPRate){
				break;
			}

			dM2DPRate *= dIntervalRate;
			vM2DPTable.push_back(dM2DPRate);
		}
	}
}

//////////////////////////////////////////////////////////////////////
// ���W�X�g������X���C�_�[�ݒ�l��ǂݏo��

BOOL CTPMouseCalcIntelligent::ReadDataRegistryAll()
{
	using namespace TPMouseCalcData;

	HKEY hRootKey = NULL;
	BOOL bResInfo(TRUE), bResGear(FALSE), bResNormalize(FALSE);

	if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE, SE_ROOT_REG, 0, KEY_ALL_ACCESS, &hRootKey) == ERROR_SUCCESS)
	{
		////////////////////////////////////////////////////////////////////////
		// TotalAdjust/InitialM2DPRate/IntervalM2DPRate�����W�X�g������ǂݏo��
		////////////////////////////////////////////////////////////////////////

		double* ppdValue[] = 
		{
			&m_sTableInfo.dTotalAdjust
		,	&m_sTableInfo.dInitialMMRate
		,	&m_sTableInfo.dIntervalMMRate
		};

		double dValueMin[] = 
		{
			TOTALADJ_MIN
		,	FIRSTRATE_MIN
		,	INTERVALRATE_MIN
		};
		double dValueMax[] = 
		{
			TOTALADJ_MAX
		,	FIRSTRATE_MAX
		,	INTERVALRATE_MAX
		};

		LPCTSTR lpszValueName[] = 
		{
			SE_TOTAL_ADJ
		,	SE_FIRST_RATE
		,	SE_INTERVAL_RATE
		};

		int nSize = sizeof(ppdValue) / sizeof(ppdValue[0]);

		for(int i=0;i<nSize;++i)
		{
			DWORD dwType = 0;
			TCHAR szValue[100];
			DWORD dwValSize = 100;

			// �l���擾����
			if(::RegQueryValueEx(hRootKey, lpszValueName[i], NULL, &dwType, (LPBYTE)&szValue, &dwValSize) == ERROR_SUCCESS)
			{
				ASSERT(dwValSize);

				if(dwType == REG_SZ)
				{
					*ppdValue[i] = _tcstod(szValue, NULL);

					if(dValueMin[i] > *ppdValue[i]){
						*ppdValue[i] = dValueMin[i];
					}
					else if(dValueMax[i] < *ppdValue[i]){
						*ppdValue[i] = dValueMax[i];
					}
				}
				else{
					ASSERT(FALSE);
				}
			}
			// �l�������ꍇ(�f�[�^���ǂݍ��߂Ȃ��ꍇ���܂�)�͌��݂̒l����������
			else
			{
				swprintf_s(szValue, _T("%f"), *ppdValue[i]);
				dwValSize = (DWORD)sizeof(TCHAR)*(_tcslen(szValue) + 1);

				if(::RegSetValueEx(hRootKey, lpszValueName[i], 0, REG_SZ, (LPBYTE)szValue, dwValSize) != ERROR_SUCCESS){
					bResInfo = FALSE;
				}
			}
		}

		////////////////////////////////////////////////////////////////////////
		// Gear�e�[�u�������W�X�g������ǂݏo��
		////////////////////////////////////////////////////////////////////////
		if((bResGear = ReadM2DPTableFormRegistry(hRootKey)) == FALSE)
		{
			bResGear = WriteM2DPTableFormRegistry(hRootKey);
		}

		////////////////////////////////////////////////////////////////////////
		// �������������W�X�g������ǂݏo��
		////////////////////////////////////////////////////////////////////////
		if((bResNormalize = ReadNormalizeInfoFormRegistry(hRootKey)) == FALSE)
		{
			bResNormalize = WriteNormalizeInfoFormRegistry(hRootKey);
		}

		// DEBUG:�B���l�ݒ�
		DWORD dwValue = 0;
		DWORD dwType = 0;
		TCHAR szValue[100];
		DWORD dwValSize = 100;
		if((::RegQueryValueEx(hRootKey, SE_FG_COUNT, NULL, &dwType, (LPBYTE)&dwValue, &dwValSize) == ERROR_SUCCESS) && (dwType == REG_DWORD)){
			m_sTableInfo.nFGCount = (int)dwValue;
		}
		if((::RegQueryValueEx(hRootKey, SE_FG_FIRST_RATE, NULL, &dwType, (LPBYTE)&szValue, &dwValSize) == ERROR_SUCCESS) && (dwType == REG_SZ)){
			m_sTableInfo.dFGInitialMMRate = _tcstod(szValue, NULL);
		}
#ifdef _INTELLIGENT_SLIDER_DEBUG_
		if((::RegQueryValueEx(hRootKey, SE_DO_DBGDUMP, NULL, &dwType, (LPBYTE)&dwValue, &dwValSize) == ERROR_SUCCESS) && (dwType == REG_DWORD)){
			m_bDbgDump = (int)dwValue;
		}
#endif
		::RegCloseKey(hRootKey);
	}
	// MainRoot���疳���̂őS�Ẵf�[�^����������
	else
	{
		bResInfo = bResGear = bResNormalize = WriteDataRegistryAll();
	}

	return (bResInfo && bResGear && bResNormalize);
}

BOOL CTPMouseCalcIntelligent::ReadM2DPTableFormRegistry(HKEY hRootKey)
{
	ASSERT(hRootKey);
	HKEY hGearSubKey = NULL;

	if(::RegOpenKeyEx(hRootKey, TPMouseCalcData::SE_GEAR_SETTING, 0, KEY_ALL_ACCESS, &hGearSubKey) != ERROR_SUCCESS){
		return FALSE;
	}

	///////////////////////////////////////////////////////////////////
	// Regstry�́u���O�v����Gear�ԍ��Ƃ��Ăł͂Ȃ��ǂݍ��񂾃f�[�^��
	// �������X�g�Ɋi�[����ׂɔ�r���鏇�ʂƂ��Ďg�p����
	///////////////////////////////////////////////////////////////////

	int		nOrder = 0;
	int		nIndex = 0;
	int		nMax = 0;
	BOOL	bContinue = TRUE;

	SEGEARORDERINFO seInfo;
	vector<SEGEARORDERINFO>	vGearInfos;

	while(bContinue)
	{
		DWORD	dwType = 0;
		TCHAR	szGear[30];
		DWORD	dwBuffSize = 30;
		TCHAR	szValue[100];
		DWORD	dwValSize = 100;

		if(::RegEnumValue(hGearSubKey, nIndex++, szGear, &dwBuffSize, NULL, &dwType, (LPBYTE)&szValue, &dwValSize) == ERROR_SUCCESS)
		{
			if(dwType != REG_SZ)
			{
				ASSERT(FALSE);
				continue;
			}

			ASSERT(dwBuffSize);
			ASSERT(dwValSize);

			if(nIndex == 1){
				nOrder = nMax = _ttoi(szGear);
			}
			else{
				nOrder = _ttoi(szGear);
			}

			seInfo.nOrder = nOrder;
			seInfo.dM2DPRate = _tcstod(szValue, NULL);

			if(nOrder >= nMax)
			{
				vGearInfos.push_back(seInfo);
				nMax = nOrder;
			}
			else
			{
				vector<SEGEARORDERINFO>::iterator it = vGearInfos.begin();
				for(;it != vGearInfos.end();++it)
				{
					if(it->nOrder >= nOrder)
					{
						vGearInfos.insert(it, 1, seInfo);
					}
				}
			}

			// mickey -> speed �ϊ�Table�ɓo�^�ł���ő吔�Ɏ����
			// �ǂݍ��ݏ������I������
			if(nIndex >= TPMouseCalcData::M2SPTABLE_MAX)
			{
				bContinue = FALSE;
			}
		}
		else
		{
			bContinue = FALSE;
		}
	}

	::RegCloseKey(hGearSubKey);

	UINT nSize = (UINT)vGearInfos.size();

	if(nSize)
	{
		// Table���̕ύX
		if(m_vMickey2DPTable.size() != nSize)
		{
			m_vMickey2DPTable.resize(nSize);
			m_vCumulativeVal.resize(nSize);
#ifdef _INTELLIGENT_SLIDER_DEBUG_
			// DEBUG:�I�����ꂽ�M���̃J�E���^���X�g���g������
			m_vSelectGearCounter.resize(nSize);
#endif
		}

		for(UINT i=0;i<nSize;++i)
		{
			double dValue = vGearInfos[i].dM2DPRate;
			if(dValue < TPMouseCalcData::M2SPEEDTABLE_MIN){
				dValue = TPMouseCalcData::M2SPEEDTABLE_MIN;
			}
			else if(dValue > TPMouseCalcData::M2SPEEDTABLE_MAX){
				dValue = TPMouseCalcData::M2SPEEDTABLE_MAX;
			}

			m_vMickey2DPTable[i] = dValue;
		}
	}

	return nSize;
}

BOOL CTPMouseCalcIntelligent::ReadNormalizeInfoFormRegistry(HKEY hRootKey)
{
	ASSERT(hRootKey);
	using namespace TPMouseCalcData;

	HKEY hNormalizeSubKey = NULL;
	BOOL bResult = TRUE;

	if(::RegOpenKeyEx(hRootKey, SE_NORMALIZE_SETTING, 0, KEY_ALL_ACCESS, &hNormalizeSubKey) != ERROR_SUCCESS){
		return FALSE;
	}

	BOOL bUpdate = FALSE;

	int* ppnValue[] = 
	{
		&m_sTableInfo.nCountAverage
	,	&m_sTableInfo.nCountPrevious
	};

	DWORD dwValueMin[] = 
	{
		AVGCOUNT_MIN
	,	PREVCOUNT_MIN
	};
	DWORD dwValueMax[] = 
	{
		AVGCOUNT_MAX
	,	PREVCOUNT_MAX
	};

	LPCTSTR lpszValueName[] = 
	{
		SE_AVG_COUNT
	,	SE_PREV_COUNT
	};

	int nSize = sizeof(ppnValue) / sizeof(ppnValue[0]);

	for(int i=0;i<nSize;++i)
	{
		DWORD dwType = 0;
		DWORD dwValue = 0;
		DWORD dwValSize = sizeof(DWORD);

		// �l���擾����
		if(::RegQueryValueEx(hNormalizeSubKey, lpszValueName[i], NULL, &dwType, (LPBYTE)&dwValue, &dwValSize) == ERROR_SUCCESS)
		{
			ASSERT(dwValSize);

			if(dwType == REG_DWORD)
			{
				if(dwValueMin[i] > dwValue){
					dwValue = dwValueMin[i];
				}
				else if(dwValueMax[i] < dwValue){
					dwValue = dwValueMax[i];
				}

				if(*ppnValue[i] != (int)dwValue)
				{
					*ppnValue[i] = (int)dwValue;
					bUpdate = TRUE;
				}
			}
			else{
				ASSERT(FALSE);
			}
		}
		// �l�������ꍇ(�f�[�^���ǂݍ��߂Ȃ��ꍇ���܂�)�͌��݂̒l����������
		else
		{
			if(::RegSetValueEx(hNormalizeSubKey, lpszValueName[i], 0, REG_DWORD, (LPBYTE)ppnValue[i], sizeof(DWORD)) != ERROR_SUCCESS){
				bResult = FALSE;
			}
		}
	}

	if(bUpdate){
		InitNormalizeInfo();
	}

	::RegCloseKey(hNormalizeSubKey);

	return bResult;
}

BOOL CTPMouseCalcIntelligent::WriteDataRegistryAll()
{
	using namespace TPMouseCalcData;

	HKEY hRootKey = NULL;
	BOOL bResInfo(FALSE), bResGear(FALSE), bResNormalize(FALSE);
	DWORD dwResult = 0;

	if(::RegCreateKeyEx(HKEY_LOCAL_MACHINE, SE_ROOT_REG, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hRootKey, &dwResult) == ERROR_SUCCESS)
	{
		double pdValue[] = 
		{
			m_sTableInfo.dTotalAdjust
		,	m_sTableInfo.dInitialMMRate
		,	m_sTableInfo.dIntervalMMRate
		};

		LPCTSTR lpszValueName[] = 
		{
			SE_TOTAL_ADJ
		,	SE_FIRST_RATE
		,	SE_INTERVAL_RATE
		};

		int nSize = sizeof(pdValue) / sizeof(pdValue[0]);

		for(int i=0;i<nSize;++i)
		{
			TCHAR szValue[100];
			swprintf_s(szValue, _T("%f"), pdValue[i]);

			DWORD dwSize = (DWORD)sizeof(TCHAR)*(_tcslen(szValue) + 1);

			if(::RegSetValueEx(hRootKey, lpszValueName[i], 0, REG_SZ, (LPBYTE)szValue, dwSize) == ERROR_SUCCESS){
				++bResInfo;
			}
			else{
				ASSERT(FALSE);	// �������ݎ��s
			}
		}

		bResGear = WriteM2DPTableFormRegistry(hRootKey);
		bResNormalize = WriteNormalizeInfoFormRegistry(hRootKey);

		::RegCloseKey(hRootKey);
	}

	return (bResInfo && bResGear && bResNormalize);
}

BOOL CTPMouseCalcIntelligent::WriteM2DPTableFormRegistry(HKEY hRootKey)
{
	ASSERT(hRootKey);
	DWORD dwResult = 0;
	HKEY hGearKey = NULL;

	if(::RegCreateKeyEx(hRootKey, TPMouseCalcData::SE_GEAR_SETTING, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hGearKey, &dwResult) != ERROR_SUCCESS){
		return FALSE;
	}

	int nSize = (int)m_vMickey2DPTable.size();
	TCHAR	szGear[100];
	TCHAR	szValue[100];

	int nResult = 0;

	for(int i=0;i<nSize;++i)
	{
		swprintf_s(szGear, _T("%d"), i+1);

		swprintf_s(szValue, _T("%f"), m_vMickey2DPTable[i]);
		DWORD dwSize = (DWORD)sizeof(TCHAR)*(_tcslen(szValue) + 1);

		if(::RegSetValueEx(hGearKey, szGear, 0, REG_SZ, (LPBYTE)szValue, dwSize) == ERROR_SUCCESS){
			++nResult;
		}
		else{
			ASSERT(FALSE);	// �������ݎ��s
		}
	}

	::RegCloseKey(hGearKey);

	return nResult;
}

BOOL CTPMouseCalcIntelligent::WriteNormalizeInfoFormRegistry(HKEY hRootKey)
{
	ASSERT(hRootKey);
	HKEY hNormalizeKey = NULL;
	DWORD dwResult = 0;

	if(::RegCreateKeyEx(hRootKey, TPMouseCalcData::SE_NORMALIZE_SETTING, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hNormalizeKey, &dwResult) != ERROR_SUCCESS){
		return FALSE;
	}

	int pnValue[] = 
	{
		m_sTableInfo.nCountAverage
	,	m_sTableInfo.nCountPrevious
	};

	LPCTSTR lpszValueName[] = 
	{
		TPMouseCalcData::SE_AVG_COUNT
	,	TPMouseCalcData::SE_PREV_COUNT
	};

	int nResult = 0;
	int nSize = sizeof(pnValue) / sizeof(pnValue[0]);

	for(int i=0;i<nSize;++i)
	{
		if(::RegSetValueEx(hNormalizeKey, lpszValueName[i], 0, REG_DWORD, (LPBYTE)&pnValue[i], sizeof(DWORD)) == ERROR_SUCCESS){
			++nResult;
		}
		else{
			ASSERT(FALSE);	// �������ݎ��s
		}
	}

	::RegCloseKey(hNormalizeKey);

	return nResult;
}

void CTPMouseCalcIntelligent::ResetDataAll()
{
	using namespace TPMouseCalcData;
	SETABLECALCINFO seTable;
	
	seTable.dwTableType			= SE_MANUAL;
	seTable.dMinimumDivision	= MINIMUM_DIVISION;

	seTable.dInitialM2DPRate	= INITIAL_M2DP_RATE;
	seTable.dIntervalM2DPRate	= INTERVAL_M2DP_RATE;

	seTable.dInitialMMRate		= INITIAL_MM_RATE;
	seTable.dIntervalMMRate		= INTERVAL_MM_RATE;

	seTable.dFGInitialMMRate	= INITIAL_FIRSTMM_RATE;
	seTable.nFGCount			= FIRSTMM_GEAR_NUM;

	seTable.dTotalAdjust		= TOTAL_ADJUST;

	seTable.nCountAverage		= STOCK_FOR_AVERAGE;
	seTable.nCountPrevious		= STCOK_FOR_PREVIOUS;

	SetTableCalcInfo(seTable);

	// ����MickeyToDevicePoint�e�[�u���쐬
	int nSize = sizeof(DEFAULT_SPEED_TABLE) / sizeof(DEFAULT_SPEED_TABLE[0]);
	for(int i=0;i<nSize; ++i)
	{
		m_vMickey2DPTable.push_back(DEFAULT_SPEED_TABLE[i]);
	}

	m_vCumulativeVal.resize(nSize);
#ifdef _INTELLIGENT_SLIDER_DEBUG_
	// DEBUG:�I�����ꂽ�M���̃J�E���^���X�g���g������
	m_vSelectGearCounter.resize(nSize);
#endif
}

#ifdef _INTELLIGENT_SLIDER_DEBUG_
void CTPMouseCalcIntelligent::DebugDump()
{
	if(!m_bDbgDump)
	{
		return ;
	}

	const vector<CTPMouseCalcIntelligent::SEMOUSEDEICELOGINFO>& vRawDataList = GetRawDeviceData();
	const vector<int>& vIntervalList = GetAccumulatedAllGear();
	const vector<int>& vSelGearList = GetSelectCounterList();
	if(vRawDataList.size() != vIntervalList.size())
	{
		ASSERT(FALSE);
		return ;
	}

	if(!m_strDumpPath.GetLength())
	{
		TCHAR szPath[MAX_PATH];
		if(!GetCurrentDirectory(MAX_PATH, szPath))
		{
			return ;
		}

		m_strDumpPath = szPath;
		m_strDumpPath.TrimRight(_T("\\"));
		m_strDumpPath += _T("\\FxEditorSliderDump.csv");
	}

	SYSTEMTIME st;
	GetLocalTime(&st);

	CString strDate;

	strDate.Format(_T("LogTime : %04d/%02d/%02d %02d:%02d:%02d.%d\n"),
		st.wYear,
		st.wMonth,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond,
		st.wMilliseconds);

	TRY
	{
		CStdioFile ioFile(m_strDumpPath, CFile::modeCreate|CFile::modeWrite|CFile::typeText);
		ioFile.WriteString(strDate);

		// Header�}��
		strDate = _T("Index,RawData,SysTime,RealGear,AvgGear,MMRate\n");
		ioFile.WriteString(strDate);

		int nCnt = vRawDataList.size();
		int i = 0;
		for(i=0;i<nCnt;++i)
		{
			CTPMouseCalcIntelligent::SEMOUSEDEICELOGINFO seInfo = vRawDataList[i];

			double dRate = (double)abs(seInfo.nMickey);
			if(seInfo.dwTime){
				dRate /= seInfo.dwTime;
			}

			strDate.Format(_T("%d,%d,%d,%d,%d,%f\n"), i+1, seInfo.nMickey, seInfo.dwTime, seInfo.nGear+1, vIntervalList[i]+1, dRate);
			ioFile.WriteString(strDate);
		}

		// Header�}��
		strDate = _T("GearNum,UsedCount\n");
		ioFile.WriteString(strDate);

		nCnt = vSelGearList.size();
		for(i=0;i<nCnt;++i)
		{
			strDate.Format(_T("%d,%d\n"), i+1, vSelGearList[i]);
			ioFile.WriteString(strDate);
		}
	}
	CATCH_ALL(e)
	{
		ASSERT(FALSE);
	}
	END_CATCH_ALL
}
#endif