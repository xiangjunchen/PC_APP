// SeMouseCalc.h: CTPMouseCalc �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMOUSECALC_H__0D451A6E_1194_4CBE_9B2C_AAF17DD74C76__INCLUDED_)
#define AFX_SEMOUSECALC_H__0D451A6E_1194_4CBE_9B2C_AAF17DD74C76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(push, 3)
#include <vector>
#pragma warning(pop)

using namespace std;

//#define	_INTELLIGENT_SLIDER_DEBUG_

//////////////////////////////////////////////////////////////////////
// MouseDeviceHooker���}�E�X�̈ړ��ʂ��v�Z����v�Z�N���X
// ���h�����ēƎ��̃}�E�X�v�Z��MouseDeviceHooker�ɍs���邱�Ƃ��o���܂��B
enum
{
	SE_CALC_Y		= 0x01
	,	SE_CALC_X		= 0x02
	,	SE_CALC_BOTH	= SE_CALC_Y | SE_CALC_X
};


class CTPMouseCalc  
{
public:
	CTPMouseCalc();
	virtual ~CTPMouseCalc();

public:
	// Attribute
	enum
	{
		SE_SLT_ORIGINAL		= 0
	,	SE_SLT_INTELLIGENT
	,	SE_SLT_NORMAL	
	};

public:
	// Attribte

	// �v�Z�̃x�[�X�ƂȂ鎲(X,Y,XY)�̐ݒ�
	virtual void		SetCalcAxis(DWORD dwAxis);
	// �v�Z�̃x�[�X�ƂȂ鎲(X,Y,XY)�̎擾
	virtual DWORD		GetCalcAxis();

	//////////////////////////////////////////////////////////////////
	// DEBUG�p
	//////////////////////////////////////////////////////////////////
	// �f�o�C�X����ǂݏo�����}�E�X�̈ړ��ʗݐϒl�̃��X�g�擾
	virtual const vector<SIZE>&		GetMouseAccumulateList();

public:
	// Operation

	// �}�E�X�f�o�C�XHook�J�n
	virtual void		Initialize();
	// �}�E�X�f�o�C�XHook�I��
	virtual void		Uninitialize();

	// �}�E�X�f�o�C�X���͌v�Z(�l�̗ݐς�����)
#ifdef TP_NS_NET
	virtual BOOL		CalcCursorPos(int xMickey, int yMickey, DWORD dwTime,BOOL bCtrl = FALSE);
#else
	virtual BOOL		CalcCursorPos(int xMickey, int yMickey, DWORD dwTime,BOOL bCtrl);
#endif
	// �ݐς����l�𐮐��̍��W�ɕϊ����Ď擾����
	virtual CPoint		GetAccumulatePosition() const;
	// �ݐς����l�������̍��W�ɕϊ����Ď擾����
	virtual SEPOINTREAL	GetAccumulatePostionReal() const;

protected:
	DWORD			m_dwAxis;				// �v�Z��
	vector<SIZE>	m_vCumulativeVal;		// �egear����Mickey�l��ݐς��郊�X�g
};

inline void CTPMouseCalc::SetCalcAxis(DWORD dwAxis)
{	m_dwAxis = dwAxis; }

inline DWORD CTPMouseCalc::GetCalcAxis()
{	return m_dwAxis; }

inline const vector<SIZE>& CTPMouseCalc::GetMouseAccumulateList()
{	return m_vCumulativeVal;	}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Intelligent�X���C�_�p�ɔh�������}�E�X�v�Z�N���X
//////////////////////////////////////////////////////////////////////

class CTPMouseCalcIntelligent : public CTPMouseCalc
{
public:
	CTPMouseCalcIntelligent();
	virtual ~CTPMouseCalcIntelligent();

public:
	// Attribute
	enum 
	{
		SE_INVALIDE	= -1
	,	SE_SPEED_HIGHTEST
	};
	enum 
	{
		SE_MANUAL
	,	SE_CALCULATE
	};

	/////////////////////////////////////////////////////////////////////////
	// mickey->device point�ϊ�table��egear��mickey/msec�̕ϊ�����v�Z�����
	// �̏��\����
	/////////////////////////////////////////////////////////////////////////
	struct SETABLECALCINFO
	{
		DWORD	dwTableType;		// table�\�z�^�C�v(manual/auto)

		// manual/auto����
		double	dTotalAdjust;		// mickey->device point�ϊ��̍ŏI�����l
		double	dInitialMMRate;		// gear�𔻒肷��mickey/msec�̊�l
		double	dIntervalMMRate;	// �egear�𔻒肷��mickey/msec��l�����Ԋu�ŕ�������l(�䗦)
		int		nCountPrevious;		// ���O��gear�Ƃ��ĎQ�Ƃ��鐔
		int		nCountAverage;		// ���ω����鎞�ɎQ�Ƃ���ߋ���gear��

		// auto�̎��̂ݎg�p(manual���͖��������)
		double	dMinimumDivision;	// �ŏ������l
		double	dInitialM2DPRate;	// mickey->device point�ϊ�table�̊�l
		double	dIntervalM2DPRate;	// mickey->device point�ϊ�table���쐬���鎞�Ɋ�l�����Ԋu�ŕ�������l(�䗦)

		// ����gear����v�Z�p�f�[�^
		int		nFGCount;			// �����Gear�𔻒肷��gear��
		double	dFGInitialMMRate;	// �����Gear�𔻒肷��mickey/msec��l
	};

	/////////////////////////////////////////////////////////////////////////
	// �A������gear�\����
	/////////////////////////////////////////////////////////////////////////
	struct SChainGear
	{
		int			nGear;			// Gear
		SChainGear*	pNext;			// ����Gear�ւ̃|�C���^
	};

	/////////////////////////////////////////////////////////////////////////
	// gear�̏��ԏ��\����(���W�X�g���ǂݍ��݂Ɏg�p)
	/////////////////////////////////////////////////////////////////////////
	struct SEGEARORDERINFO
	{
		int		nOrder;			// ����
		double	dM2DPRate;		// �ϊ����[�g
	};

#ifdef _INTELLIGENT_SLIDER_DEBUG_
	/////////////////////////////////////////////////////////////////////////
	// DEBUG:mouse device����擾�����f�[�^���O�\����
	/////////////////////////////////////////////////////////////////////////
	struct SEMOUSEDEICELOGINFO
	{
		int		nMickey;			// ���f�[�^
		DWORD	dwTime;				// �f�[�^���荞�݊Ԋu
		int		nGear;				// �v�Z����
	};
#endif

public:
	// Attribte

	// mickey->device point�ϊ��ptable�̎擾
	virtual const vector<double>&	GetMickey2DPTable();
	// mickey->device point�ϊ��ptable�̐ݒ�
	virtual void		SetMickey2DPTable(const vector<double>& vMickey2DPTable);

	// gear�S�̂�mickey->device point�ϊ��l�𒲐�����l�̐ݒ�
	virtual void		SetTotalAdjust(double dRegVal);
	// gear�S�̂�mickey->device point�ϊ��l�𒲐�����l�̎擾
	virtual double		GetTotalAdjust();

	// table info�ݒ�
	virtual void		SetTableCalcInfo(const SETABLECALCINFO& seTableInfo);
	// table info�̎擾
	virtual SETABLECALCINFO	GetTableCalcInfo();

#ifdef _INTELLIGENT_SLIDER_DEBUG_
	//////////////////////////////////////////////////////////////////
	// DEBUG�F�f�o�b�O�����擾����ׂ̃��\�b�h�S
	//////////////////////////////////////////////////////////////////
	// Dump
	void	DebugDump();
	// �f�o�C�X����ǂݏo�����f�[�^���ǂ�gear�ɗݐς�����gear����counter
	virtual const vector<int>&		GetSelectCounterList();
	// ���ϒl���v�Z����ׂɒ~�ς���gear�̃��X�g
	virtual const vector<int>&		GetAvgAccumuGearList();
	// ��L�̃��X�g�ɍŌ�ɒ~�ς����C���f�b�N�X
	virtual int						GetLatestAccumuIndex();
	// �}�E�X�f�o�C�X����擾�������f�[�^(mickey)�Ɗ��荞�݊Ԋu(msec)�A�I�����ꂽgear���L�^�������X�g���擾
	virtual const vector<SEMOUSEDEICELOGINFO>&		GetRawDeviceData();
	// ���ω�������̑S�Ă�gear���X�g���擾
	virtual const vector<int>&		GetAccumulatedAllGear();
#endif

public:
	// Operation

	// �}�E�X�f�o�C�XHook�J�n
	virtual void		Initialize();

	// �}�E�X�f�o�C�X���͌v�Z(�l�̗ݐς�����)
	virtual BOOL		CalcCursorPos(int xMickey, int yMickey, DWORD dwTime,BOOL bCtrl = FALSE);
	// �ݐϒl�������̍��W�ʒu�ɕϊ����Ď擾����
	virtual SEPOINTREAL	GetAccumulatePostionReal() const;
	// mickey -> device point�ϊ��e�[�u�����쐬����
	static	void		MakeMickey2DPTable(double dMinVal, double dInitialRate, double dIntervalRate, vector<double>& vM2DPTable);

	// Data Operation

	// ���߂�ꂽ���W�X�g������v�Z�p�f�[�^���擾����
	virtual BOOL		ReadDataRegistryAll();
	// ���߂�ꂽ���W�X�g���֌v�Z�p�f�[�^��ݒ肷��
	virtual BOOL		WriteDataRegistryAll();
	// �v�Z�p�f�[�^���`�ς݂̒l�ŏ���������
	virtual void		ResetDataAll();

protected:
	// Calculate

	// ���x�v�Z(���x�ω������炩�ɂȂ�悤��������)
	virtual int			CalcGear(int nMickey, DWORD dwTime, BOOL bUpdateData,BOOL bCtrl = FALSE);
	// ���x�v�Z(mickey/msec�ł̒P���v�Z)
	virtual int			CalcAdjustGear(int nMickey, DWORD dwTime);
	// Table�̏�����
	virtual void		InitTableInfo();
	// ���������邽�߂̏��̏�����
	virtual void		InitNormalizeInfo();

	// Data

	// �w�肵�����W�X�g��(Key)����M2DP�ϊ��e�[�u���f�[�^��ǂݍ���
	virtual BOOL		ReadM2DPTableFormRegistry(HKEY hRootKey);
	// �w�肵�����W�X�g��(Key)���畽��������ǂݍ���
	virtual BOOL		ReadNormalizeInfoFormRegistry(HKEY hRootKey);
	// �w�背�W�X�g��(Key)��M2DP�ϊ��e�[�u���f�[�^����������
	virtual BOOL		WriteM2DPTableFormRegistry(HKEY hRootKey);
	// �w�背�W�X�g��(Key)�֕�����������������
	virtual BOOL		WriteNormalizeInfoFormRegistry(HKEY hRootKey);

protected:
	DWORD			m_dwLastTime;			// �Ō�Ɍv�Z��������(msec)

	vector<double>	m_vMickey2DPTable;		// mickey�l(���f�[�^)->device point�ϊ��ptable

	vector<int>		m_vAccumuGearForAvg;	// ���ϒl�����߂�ׂ�gear���X�g
	int				m_nLastAccumuIndex;		// �Ō�Ɋi�[����gear���X�g�C���f�b�N�X

	SETABLECALCINFO	m_sTableInfo;			// table�v�Z����ׂ̏��\����

	vector<SChainGear>	m_vAccumeGearForPrev;	// ���O��gear�Ƃ��ĎQ�Ƃ���M�����X�g
	SChainGear*			m_pNextGearForPrev;		// ����gear�ւ̃|�C���^

#ifdef _INTELLIGENT_SLIDER_DEBUG_
	vector<int>					m_vAccumuAllGear;		// �I�����ꂽgear�̃��X�g
	vector<int>					m_vSelectGearCounter;	// �I�����ꂽ�egear��counter
	vector<SEMOUSEDEICELOGINFO>	m_vRawDeviceData;		// mickey�l(���f�[�^)�ƃn�[�h�E�F�A���荞�ݎ��Ԃ̃��O
	BOOL						m_bDbgDump;
	CString						m_strDumpPath;
#endif
};

inline const vector<double>& CTPMouseCalcIntelligent::GetMickey2DPTable()
{	return m_vMickey2DPTable;	}

inline double CTPMouseCalcIntelligent::GetTotalAdjust()
{	return m_sTableInfo.dTotalAdjust; }

inline void CTPMouseCalcIntelligent::SetTotalAdjust(double dVal)
{	m_sTableInfo.dTotalAdjust = dVal; }

inline CTPMouseCalcIntelligent::SETABLECALCINFO CTPMouseCalcIntelligent::GetTableCalcInfo()
{	return m_sTableInfo; }

#ifdef _INTELLIGENT_SLIDER_DEBUG_
//////////////////////////////////////////////////////////////////
// DEBUG�p�F�f�[�^�擾�֐�
//////////////////////////////////////////////////////////////////
inline const vector<int>& CTPMouseCalcIntelligent::GetSelectCounterList()
{	return m_vSelectGearCounter;	}

inline const vector<int>& CTPMouseCalcIntelligent::GetAvgAccumuGearList()
{	return m_vAccumuGearForAvg;	}

inline int CTPMouseCalcIntelligent::GetLatestAccumuIndex()
{	return m_nLastAccumuIndex; }

inline const vector<CTPMouseCalcIntelligent::SEMOUSEDEICELOGINFO>& CTPMouseCalcIntelligent::GetRawDeviceData()
{	return m_vRawDeviceData; }

inline const vector<int>& CTPMouseCalcIntelligent::GetAccumulatedAllGear()
{	return m_vAccumuAllGear;	}
#endif

#endif // !defined(AFX_SEMOUSECALC_H__0D451A6E_1194_4CBE_9B2C_AAF17DD74C76__INCLUDED_)
