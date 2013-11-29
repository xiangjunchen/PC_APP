// SeMouseDeviceHooker.h: CTPMouseDeviceHooker クラスのインターフェイス
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
// DirectXを使ってMouse Deviceを制御するクラス

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

	// 初期化(DirectInputを利用できるようにする)
	BOOL	InstallMouseDeviceHook(HWND hDirectInputOwer);
	// 後始末
	void	UninstallMouseDeviceHook();
	// Hook開始
	virtual BOOL	StartHook(HWND hWndTarget,
							  const CPoint& ptPos,
							  CTPMouseCalc* pCalculator,
							  DWORD dwEndHookMsg);
	// Hook終了
	virtual BOOL	ExitHook();

public:
	// Attribute

	// 最新のカーソル位置(Screen座標)を取得
	CPoint	GetCurrentPos();
	// 最新のカーソル位置(Screen座標)を実数で取得
	SEPOINTREAL	GetCurrentPosReal();
	// スライダ計算用デフォルト計算クラス取得
	CTPMouseCalc*	GetDefaultCalclator();
	// スライダ計算用デフォルト計算クラス設定
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

	// Mouse Device処理スレッド
	virtual BOOL	MouseDeviceMainThread();
	// Mouse Device処理スレッド
	virtual BOOL	MouseDeviceSubThread();

	// Hook対象のリソース取得
	virtual BOOL	GetHookResource(SEHOOKSOURCE& sResource);

protected:
	// Thread operation

	// Mouse Device Hook開始
	virtual BOOL	StartHookMouseThread();
	// Mouse Device Hook開放
	virtual void	EndHookMouseThread();
	// Hook判定
	virtual BOOL	IsHook();
	// Hook中に利用するリソース設定
	virtual void	SetHookSource(const SEHOOKSOURCE* pSource);

	// Mouse Deviceの入力処理
	virtual BOOL	OnMouseInput(const SEHOOKSOURCE& sHookSource);
	// カーソル更新処理
	virtual BOOL	UpdateCursorPosition(const SEHOOKSOURCE& sHookSource, 
										 int dx, int dy, DWORD dwTime, DWORD dwMBState);

	// Targetへメッセージ送信
	virtual void	PostTargetMouseMessage(const SEHOOKSOURCE& sHookSource, UINT nMessage, DWORD dwMBState);
	// KeyState取得
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

	// DirectInputの初期化とMouseDevice取得
	virtual HRESULT CreateDirectInput();
	// DirectInputの後始末
	virtual void	FreeDirectInput();

	// Mouse Deviceアクセス権取得
	virtual BOOL	AcquireMouseDevice();
	// Mouse Deviceアクセス権放棄
	virtual void	UnAcquireMouseDevice();

	static unsigned _stdcall MouseDeviceThreadProxy(LPVOID pParameter); 
	virtual BOOL	InitializeThread();
	virtual void	UninitializeThread();

protected:
	CComPtr<IDirectInput8>			m_IDeviceInput8;	// DirectInputマネージャ
	CComPtr<IDirectInputDevice8>	m_IMouseDevice8;	// MouseDeviceにアクセスするオブジェクト

	HWND				m_hDirectInputOwner;	// DirectInputのOwner

	HANDLE				m_hMouseEvent;			// DirectInputのハードウェア割り込みイベント
	HANDLE				m_hHookStart;
	HANDLE				m_hHookExit;
	HANDLE				m_hCallback;			// HookThreadCallbackイベント
	HANDLE				m_hThread;

	DWORD				m_dwThreadID;

	CCriticalSection	m_csHookSource;			// Hook中のリソースロック

	SEHOOKSOURCE		m_sHookSource;
	int					m_nSwapMouseButton;		// マウスのLRボタンシステム設定
	SEPOINTREAL			m_ptPrevOffset;			// 直前のOffsetポイント

	BOOL				m_bThreadResult;
	int					m_nThreadState;

	CTPMouseCalc*		m_pNormalEmuCalc;		// ノーマルエミュレート用計算クラス
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
