// SeMouseDeviceHooker.h: CTPMouseDeviceHooker �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMOUSEDEVICEHOOKER_H__5FBC0C26_A728_4CAB_9B6F_7282A50384B3__INCLUDED_)
#define AFX_SEMOUSEDEVICEHOOKER_H__5FBC0C26_A728_4CAB_9B6F_7282A50384B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <atlbase.h>
#include <afxmt.h>
interface IDirectInput8;
interface IDirectInputDevice8;
class CTPMouseCalc;
/////////////////////////////////////////////////////////////
// DirectX���g����Mouse Device�𐧌䂷��N���X

class CTPMouseDeviceHooker  
{
public:
	CTPMouseDeviceHooker();
	virtual ~CTPMouseDeviceHooker();

public:
	// Operation
	enum
	{
		Chk_LButtonDown	= 0x01
	,	Chk_LButtonUp	= 0x02
	,	Chk_RButtonDown	= 0x04
	,	Chk_RButtonUp	= 0x08
	};

	struct SEHOOKSOURCE
	{
		CTPMouseCalc*	pCalculator;
		HWND			hWndTarget;
		DWORD			dwEndHookMsg;
		POINT			ptBase;
		BOOL			bContinueCalc;
	};

	enum
	{
		SE_None				= 0
	,	SE_PeekMessage
	,	SE_CoInitialize
	,	SE_DirectInput8Create
	,	SE_CreateDevice
	,	SE_SetDataFormat
	,	SE_SetCooperativeLevel
	,	SE_SetEventNotification
	,	SE_SetProperty
	,	SE_Acquire
	,	SE_GetDeviceData
	,	SE_GetDeviceState
	,	SE_GetDeviceData_Flash
	,	SE_Unacquire
	,	SE_WaitStart
	,	SE_GetHookResource
	,	SE_UpdateCursorPosition
	,	SE_PostTargetMouseMessage
	,	SE_WaitMouseEvent
	,	SE_UninitializeThread
	};

	// ������(DirectInput�𗘗p�ł���悤�ɂ���)
	BOOL	InstallMouseDeviceHook(HWND hDirectInputOwer);
	// ��n��
	void	UninstallMouseDeviceHook();
	// Hook�J�n
	virtual BOOL	StartHook(HWND hWndTarget,
							  const CPoint& ptPos,
							  CTPMouseCalc* pCalculator,
							  DWORD dwEndHookMsg);
	// Hook�I��
	virtual BOOL	ExitHook();

public:
	// Attribute

	// �ŐV�̃J�[�\���ʒu(Screen���W)���擾
	CPoint	GetCurrentPos();
	// �ŐV�̃J�[�\���ʒu(Screen���W)�������Ŏ擾
	SEPOINTREAL	GetCurrentPosReal();
	// �X���C�_�v�Z�p�f�t�H���g�v�Z�N���X�擾
	CTPMouseCalc*	GetDefaultCalclator();
	// �X���C�_�v�Z�p�f�t�H���g�v�Z�N���X�ݒ�
	void	SetDefaultCalclator(CTPMouseCalc* pCalculator);
	//set param step
	void    SetRange(double dRange);
	//thumb position
	void SetChannelRect(CRect rtThumb);
	//set mouse axis
	void    SetMouseAxis(DWORD dwAxis);
	void    SetCtrl(BOOL bDown);

public:
	// ThreadFunc

	// Mouse Device�����X���b�h
	virtual BOOL	MouseDeviceMainThread();
	// Mouse Device�����X���b�h
	virtual BOOL	MouseDeviceSubThread();

	// Hook�Ώۂ̃��\�[�X�擾
	virtual BOOL	GetHookResource(SEHOOKSOURCE& sResource);

protected:
	// Thread operation

	// Mouse Device Hook�J�n
	virtual BOOL	StartHookMouseThread();
	// Mouse Device Hook�J��
	virtual void	EndHookMouseThread();
	// Hook����
	virtual BOOL	IsHook();
	// Hook���ɗ��p���郊�\�[�X�ݒ�
	virtual void	SetHookSource(const SEHOOKSOURCE* pSource);

	// Mouse Device�̓��͏���
	virtual BOOL	OnMouseInput(const SEHOOKSOURCE& sHookSource);
	// �J�[�\���X�V����
	virtual BOOL	UpdateCursorPosition(const SEHOOKSOURCE& sHookSource, 
										 int dx, int dy, DWORD dwTime, DWORD dwMBState);

	// Target�փ��b�Z�[�W���M
	virtual void	PostTargetMouseMessage(const SEHOOKSOURCE& sHookSource, UINT nMessage, DWORD dwMBState);
	// KeyState�擾
	UINT	GetKeyState();

	virtual BOOL	CheckDeviceState(DWORD dwMsgFlags);

	void	ClearThreadResult();
	void	SetThreadResult(BOOL bResult);
	BOOL	GetThreadResult() const;

	void	ResetThreadState();
	void	SetThreadState(int nResult);
	int		GetThreadState() const;

protected:
	// DirectInput operation

	// DirectInput�̏�������MouseDevice�擾
	virtual HRESULT CreateDirectInput();
	// DirectInput�̌�n��
	virtual void	FreeDirectInput();

	// Mouse Device�A�N�Z�X���擾
	virtual BOOL	AcquireMouseDevice();
	// Mouse Device�A�N�Z�X������
	virtual void	UnAcquireMouseDevice();

	static unsigned _stdcall MouseDeviceThreadProxy(LPVOID pParameter); 
	virtual BOOL	InitializeThread();
	virtual void	UninitializeThread();

protected:
	CComPtr<IDirectInput8>			m_IDeviceInput8;	// DirectInput�}�l�[�W��
	CComPtr<IDirectInputDevice8>	m_IMouseDevice8;	// MouseDevice�ɃA�N�Z�X����I�u�W�F�N�g

	HWND				m_hDirectInputOwner;	// DirectInput��Owner

	HANDLE				m_hMouseEvent;			// DirectInput�̃n�[�h�E�F�A���荞�݃C�x���g
	HANDLE				m_hHookStart;
	HANDLE				m_hHookExit;
	HANDLE				m_hCallback;			// HookThreadCallback�C�x���g
	HANDLE				m_hThread;

	DWORD				m_dwThreadID;

	CCriticalSection	m_csHookSource;			// Hook���̃��\�[�X���b�N

	SEHOOKSOURCE		m_sHookSource;
	int					m_nSwapMouseButton;		// �}�E�X��LR�{�^���V�X�e���ݒ�
	SEPOINTREAL			m_ptPrevOffset;			// ���O��Offset�|�C���g

	BOOL				m_bThreadResult;
	int					m_nThreadState;

	CTPMouseCalc*		m_pNormalEmuCalc;		// �m�[�}���G�~�����[�g�p�v�Z�N���X
	double				m_dRange;
	CRect				m_rtChannel;
	BOOL				m_bCtrl;
};

inline void CTPMouseDeviceHooker::SetCtrl(BOOL bDown)
{m_bCtrl = bDown;}

inline void CTPMouseDeviceHooker::SetChannelRect(CRect rtChannel)
{ m_rtChannel = rtChannel;}
inline void CTPMouseDeviceHooker::SetRange(double dRange)
{ m_dRange = dRange;}
inline void	CTPMouseDeviceHooker::ClearThreadResult()
{	m_bThreadResult = TRUE; }
inline void CTPMouseDeviceHooker::SetThreadResult(BOOL bResult)
{	m_bThreadResult = bResult; }
inline BOOL CTPMouseDeviceHooker::GetThreadResult() const
{	return m_bThreadResult; }

inline void CTPMouseDeviceHooker::ResetThreadState()
{	m_nThreadState = SE_None; }
inline void CTPMouseDeviceHooker::SetThreadState(int nResult)
{	m_nThreadState = nResult; }
inline int CTPMouseDeviceHooker::GetThreadState() const
{	return m_nThreadState; }


#endif // !defined(AFX_SEMOUSEDEVICEHOOKER_H__5FBC0C26_A728_4CAB_9B6F_7282A50384B3__INCLUDED_)
