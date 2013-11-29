// SeMouseDeviceHooker.cpp: CTPMouseDeviceHooker クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////




#include "stdafx.h"
#include <dinput.h>
#include "TPMouseDeviceHooker.h"
#include "TPMouseCalc.h"
#include <process.h>
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	DEVICE_BUFFER_SIZE	16
#define	RECEIVE_BUFFER_SIZE	1

#ifdef _DEBUG
	#define SE_MOUSE_HOOK_THREAD_WAITTIME	INFINITE
#else
	#define SE_MOUSE_HOOK_THREAD_WAITTIME	15000
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CTPMouseDeviceHooker::CTPMouseDeviceHooker()
:	m_hDirectInputOwner(NULL)
,	m_hMouseEvent(NULL)
,	m_hHookStart(NULL)
,	m_hHookExit(NULL)
,	m_hCallback(NULL)
,	m_IDeviceInput8(NULL)
,	m_IMouseDevice8(NULL)
,	m_nSwapMouseButton(0)
,	m_pNormalEmuCalc(NULL)
,	m_dwThreadID(0)
,	m_hThread(NULL)
,	m_dRange(0)
,   m_bCtrl(0)
{
	m_pNormalEmuCalc = new CTPMouseCalc;
	SetHookSource(NULL);
}

CTPMouseDeviceHooker::~CTPMouseDeviceHooker()
{
	UninstallMouseDeviceHook();
	if(m_pNormalEmuCalc)
	{
		delete m_pNormalEmuCalc;
		 m_pNormalEmuCalc = NULL;
	}
}

//-----------------------------------------------------------------------------
// Mouse Deviceフックモジュールの初期化
//-----------------------------------------------------------------------------
// hDirectInputOwerに付いて
// @@@注意@@@
// DirectInputを利用するプロセスの最上位ウィンドウでなければいけない
// DirectInputDeviceのアクセス権を取得中に破棄されてはいけない

BOOL CTPMouseDeviceHooker::InstallMouseDeviceHook(HWND hDirectInputOwer)
{

	if(!::IsWindow(hDirectInputOwer))
		return FALSE;

	m_hDirectInputOwner = hDirectInputOwer;

	BOOL bResult = StartHookMouseThread();

	if(!bResult)
		UninstallMouseDeviceHook();

	return bResult;
}

//-----------------------------------------------------------------------------
// Mouse Deviceフックモジュールの後始末
//-----------------------------------------------------------------------------

void CTPMouseDeviceHooker::UninstallMouseDeviceHook()
{
	// 強制でThreadを終了させる
	EndHookMouseThread();

	m_hDirectInputOwner = NULL;
	m_nSwapMouseButton = 0;

	if(m_hHookStart)
	{
		::CloseHandle(m_hHookStart);
		m_hHookStart = NULL;
	}
	if(m_hHookExit)
	{
		::CloseHandle(m_hHookExit);
		m_hHookExit = NULL;
	}
	if(m_hCallback)
	{
		::CloseHandle(m_hCallback);
		m_hCallback = NULL;
	}
}

//-----------------------------------------------------------------------------
// Mouse Deviceフック
//-----------------------------------------------------------------------------

BOOL CTPMouseDeviceHooker::StartHookMouseThread()
{
	if(!::IsWindow(m_hDirectInputOwner))
		return FALSE;
	if(m_hThread != NULL)
		return FALSE;
	if(m_hHookStart || m_hHookExit || m_hCallback)
		return FALSE;

	m_hHookStart	= ::CreateEvent(NULL, FALSE, FALSE,NULL);
	m_hHookExit		= ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hCallback		= ::CreateEvent(NULL, FALSE, FALSE, NULL);

	if(!m_hHookStart || !m_hHookExit || !m_hCallback)
		return FALSE;

	// スレッドを起こしDeviceの入力処理をする
	ClearThreadResult();
	ResetThreadState();

	UINT	unThreadAddr(0);

	m_hThread = (HANDLE)_beginthreadex(NULL,
										 0,
										 MouseDeviceThreadProxy,
										 (LPVOID)this,
										 0,
										 &unThreadAddr);
	if(m_hThread == NULL)
	{

		return FALSE;
	}

	// スレッドが起動するまで指定時間待ち合わせる
	DWORD dwResult = ::WaitForSingleObject(m_hCallback, SE_MOUSE_HOOK_THREAD_WAITTIME);
	if(dwResult != WAIT_OBJECT_0)
	{
		return FALSE;
	}

	if(!GetThreadResult())
	{
		
	}

	return GetThreadResult();
}

void   CTPMouseDeviceHooker::SetMouseAxis(DWORD dwAxis)
{
	if(m_pNormalEmuCalc)
	{
		m_pNormalEmuCalc->SetCalcAxis(dwAxis);
	}
}

//-----------------------------------------------------------------------------
// Mouse Deviceフック
//-----------------------------------------------------------------------------

BOOL CTPMouseDeviceHooker::StartHook(HWND hWndTarget, 
									 const CPoint& ptPos, 
									 CTPMouseCalc* pCalc, 
									 DWORD dwEndHookMsg)
{

	if(!::IsWindow(hWndTarget) || !m_hThread || !m_hHookStart || !m_hHookExit || !m_hCallback)
		return FALSE;

	// まだフック中ならばHook強制終了する
	if(IsHook())
	{
		// Hookを抜けられないので開始失敗とする
		if(!ExitHook())
			return FALSE;
	}

	// CustomCalclator/DefaultCalclatorを利用するか決定する
	BOOL bResult = TRUE;
	CTPMouseCalc* pCalculator = m_pNormalEmuCalc;
	if(pCalc)
		pCalculator = pCalc;

	if(!pCalculator)
		return FALSE;

	pCalculator->Initialize();

	SEHOOKSOURCE sHookSource = { pCalculator, hWndTarget, dwEndHookMsg, {ptPos.x, ptPos.y}, TRUE };

	SetHookSource(&sHookSource);

	// 各Clientの計算ルーティンをセット
	m_ptPrevOffset.x = m_ptPrevOffset.y = 0.0;

	ClearThreadResult();
	::ResetEvent(m_hCallback);
	::SetEvent(m_hHookStart);

	DWORD dwResult = ::WaitForSingleObject(m_hCallback, SE_MOUSE_HOOK_THREAD_WAITTIME);
	if(dwResult != WAIT_OBJECT_0)
	{
		bResult = FALSE;
	}
	else
	{
		bResult = GetThreadResult();
	}
	
	if(!bResult)
	{
		pCalculator->Uninitialize();
		SetHookSource(NULL);

	}

	return bResult;
}

//-----------------------------------------------------------------------------
// Mouse Deviceフック開始
//-----------------------------------------------------------------------------

BOOL CTPMouseDeviceHooker::InitializeThread()
{
	m_dwThreadID = ::GetCurrentThreadId();

	// スレッドにメッセージキューを作成する
	MSG msg;
	::PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

	SetThreadState(SE_CoInitialize);
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(SUCCEEDED(hr))
	{
		hr = CreateDirectInput();
	}
	else
	{
	}

	return SUCCEEDED(hr);
}

void CTPMouseDeviceHooker::UninitializeThread()
{
	SetThreadState(SE_UninitializeThread);
	FreeDirectInput();
	CoUninitialize();
}

BOOL CTPMouseDeviceHooker::MouseDeviceMainThread()
{

	BOOL	bDone = (m_hHookStart == NULL);
	DWORD	dwEventCause = 0;
	MSG		msg;
	HANDLE	hCallback = m_hCallback;
	
	try
	{
		// Threadの初期化
		if(InitializeThread())
		{
			// Threadからの初回取得に時間が掛かるので1回だけ取得しておく
			if(AcquireMouseDevice())
			{
				UnAcquireMouseDevice();
			}
			else
			{
				bDone = TRUE;
				SetThreadResult(FALSE);

			}
		}
		else
		{
			bDone = TRUE;
			SetThreadResult(FALSE);
		}
		
		::SetEvent(hCallback);
		hCallback = NULL;

		while( !bDone ) 
		{
			/////////////////////////////////////////////////////////////
			// Hookの開始/Thread終了イベントまで待機
			/////////////////////////////////////////////////////////////
			if(!::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
				SetThreadState(SE_WaitStart);
				dwEventCause = ::MsgWaitForMultipleObjects(	1,
															&m_hHookStart,
															FALSE,
															INFINITE,
															QS_ALLEVENTS);
			}
			else
			{
				dwEventCause = WAIT_OBJECT_0 + 1;
			}

			if(dwEventCause == WAIT_OBJECT_0)
			{	// PostWorkイベントがシグナル状態になった
				MouseDeviceSubThread();
			}
			else
			{	// メッセージを受信した
				if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					if(msg.message == WM_QUIT)
					{
						bDone = TRUE;
					}
				}
			}
		}
	}
	catch(...)
	{
		ASSERT(FALSE);
		if(hCallback != NULL)
		{
			SetThreadResult(FALSE);
			::SetEvent(hCallback);
		}
	}

	UninitializeThread();

	return TRUE;
}

//-----------------------------------------------------------------------------
// Mouse Deviceフック開始
//-----------------------------------------------------------------------------

BOOL CTPMouseDeviceHooker::MouseDeviceSubThread()
{
	if(m_hCallback == NULL)
		return FALSE;

	if(m_hHookExit == NULL || m_hMouseEvent == NULL)
	{
		SetThreadResult(FALSE);
		::SetEvent(m_hCallback);
		return FALSE;
	}

	HANDLE hEvents[] = 
	{
		m_hHookExit
	,	m_hMouseEvent
	};

	SEHOOKSOURCE sHookSource;
	DWORD	dwResult = 0;
	BOOL	bDone = FALSE;

	if(!GetHookResource(sHookSource))
	{
		SetThreadResult(FALSE);
		::SetEvent(m_hCallback);
		return FALSE;
	}

	/////////////////////////////////////////////////////////////
	// MouseDeviceを要求
	/////////////////////////////////////////////////////////////
	if(!AcquireMouseDevice())
	{
		SetThreadResult(FALSE);
		::SetEvent(m_hCallback);
		return FALSE;
	}

	CPoint pt;
	::GetCursorPos(&pt);
	pt = pt - sHookSource.ptBase;

	/////////////////////////////////////////////////////////////
	// Hook準備中に移動したカーソルを計算する
	/////////////////////////////////////////////////////////////
	if(pt.x != 0 || pt.y != 0)
	{
		DWORD dwCurTime = ::GetTickCount();
		UpdateCursorPosition(sHookSource, pt.x, pt.y, dwCurTime, 0);
	}

	// MouseHook準備完了
	::SetEvent(m_hCallback);

	// ハードウェア割り込みもしくはHook終了イベントまで待機
	while( !bDone ) 
	{
		SetThreadState(SE_WaitMouseEvent);
		dwResult = WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);

		switch( dwResult ) 
		{
		// m_hHookExitイベントシグナル
		case WAIT_OBJECT_0 + 0:
			bDone = TRUE;
			break;
		// m_hMouseEventイベントシグナル
		case WAIT_OBJECT_0 + 1:
			OnMouseInput(sHookSource);
			break;
		default:
			bDone = TRUE;
			SetThreadResult(FALSE);
			break;
		}
	}

	// Mouse Deviceアクセス権を開放
	UnAcquireMouseDevice();

	// リソース開放
	SetHookSource(NULL);

	sHookSource.pCalculator->Uninitialize();

	// Thread終了Callback
	::SetEvent(m_hCallback);

	return TRUE;
}

//-----------------------------------------------------------------------------
// Mouse Deviceフック終了
//-----------------------------------------------------------------------------
BOOL CTPMouseDeviceHooker::ExitHook()
{
	if(m_hThread == NULL)
		return FALSE;

	SEHOOKSOURCE sHookSource;
	if(GetHookResource(sHookSource))
	{
		// Thread中断イベント設定
		::ResetEvent(m_hCallback);
		::SetEvent(m_hHookExit);

		// Threadが終了するまでMainThread上で待機
		DWORD dwResult = ::WaitForSingleObject(m_hCallback, SE_MOUSE_HOOK_THREAD_WAITTIME);
		if(dwResult != WAIT_OBJECT_0)
		{
			return FALSE;
		}
		else
		{
			sHookSource.pCalculator->Uninitialize();
		}
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// Mouse Deviceフック終了
//-----------------------------------------------------------------------------
void CTPMouseDeviceHooker::EndHookMouseThread()
{

	DWORD dwResult = WAIT_FAILED;

	// Thread終了まで待機
	if(m_hThread != NULL)
	{
		// Hook中であれば強制Hook解除
		ExitHook();

		if(m_dwThreadID != 0)
		{
			// Thread中断イベント設定
			::PostThreadMessage(m_dwThreadID, WM_QUIT, 0,0);

			// Threadが終了するまでMainThread上で待機
			dwResult = ::WaitForSingleObject(m_hThread, SE_MOUSE_HOOK_THREAD_WAITTIME);
		}

		if(dwResult != WAIT_OBJECT_0)
		{
			::TerminateThread(m_hThread, 0);
		}

		// Thread終了後はイベントを破棄する
		::CloseHandle(m_hThread);

		// スレッドの強制破棄が行われた場合、情報が残ってしまうので念の為に破棄しておく
		SEHOOKSOURCE sHookSource;
		if(GetHookResource(sHookSource))
			sHookSource.pCalculator->Uninitialize();
	}

	SetHookSource(NULL);
	m_hThread = NULL;
}

//-----------------------------------------------------------------------------
// Mouse Deviceの生データ(Mickey)処理
// Mouseから取得する生データ(Mickey)はデバイス依存のデータでWindows上の設定
// に依存しない
//-----------------------------------------------------------------------------
BOOL CTPMouseDeviceHooker::OnMouseInput(const SEHOOKSOURCE& sHookSource)
{
	ASSERT(m_IMouseDevice8);
	ASSERT(sHookSource.pCalculator);
	ASSERT(::IsWindow(sHookSource.hWndTarget));

    BOOL                bContinue = TRUE;
    DIDEVICEOBJECTDATA  od[RECEIVE_BUFFER_SIZE];
    DWORD               dwElements;
    HRESULT             hr(E_FAIL);
	DWORD				dwMBState(0);

	int nCnt = 0;

	// DirectInputDeviceからバッファリングされた生データを読み出して処理をする
	while( bContinue ) 
	{
		++nCnt;
		dwElements = RECEIVE_BUFFER_SIZE;
		SetThreadState(SE_GetDeviceData);
		hr = m_IMouseDevice8->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), od, &dwElements, 0 );

		if( hr == DIERR_INPUTLOST ) 
		{
			AcquireMouseDevice();
			break;
		}

		if(FAILED(hr))
		{
			break;
		}

		// 有効なデータがもう無いので終了
		if(dwElements == 0){
			break;
		}

		for(UINT i=0;i<dwElements && bContinue;++i)
		{
			// 変更されたDeviceStateごとに分岐処理
			switch( od[i].dwOfs ) 
			{     
			case DIMOFS_X:	// X軸
				bContinue = UpdateCursorPosition(sHookSource, od[i].dwData, 0, od[i].dwTimeStamp, dwMBState);
				break;

			case DIMOFS_Y:	// Y軸
				bContinue = UpdateCursorPosition(sHookSource, 0, od[i].dwData, od[i].dwTimeStamp, dwMBState);
				break;

			case DIMOFS_BUTTON0: // マウスボタン0もしくは1の処理
			case DIMOFS_BUTTON1: // その他のカスタムボタンはサポートしない
				// Leftボタンクリックか判定(System設定でSwapした場合にも対応)
				if( ( m_nSwapMouseButton  && DIMOFS_BUTTON1 == od[i].dwOfs ) ||
					( !m_nSwapMouseButton && DIMOFS_BUTTON0 == od[i].dwOfs ) )
				{
					if( (od[i].dwData & 0x80))
					{
						if((sHookSource.dwEndHookMsg & Chk_LButtonDown)){
							bContinue = FALSE;
						}
						else{
							PostTargetMouseMessage(sHookSource, WM_LBUTTONDOWN, dwMBState);
						}
						dwMBState |= MK_LBUTTON;
					}
					else
					{
						if(sHookSource.dwEndHookMsg & Chk_LButtonUp){
							bContinue = FALSE;
						}
						else{
							PostTargetMouseMessage(sHookSource, WM_LBUTTONUP, dwMBState);
						}
						dwMBState &= ~MK_LBUTTON;
					}
				}

				// Rightボタンクリックか判定(System設定でSwapした場合にも対応)
				if( ( m_nSwapMouseButton  && DIMOFS_BUTTON0 == od[i].dwOfs ) ||
					( !m_nSwapMouseButton && DIMOFS_BUTTON1 == od[i].dwOfs ) )
				{
					if( od[i].dwData & 0x80 )
					{
						if(sHookSource.dwEndHookMsg & Chk_RButtonDown){
							bContinue = FALSE;
						}
						else{
							PostTargetMouseMessage(sHookSource, WM_RBUTTONDOWN, dwMBState);
						}
						dwMBState |= MK_RBUTTON;
					}
					else
					{
						if(sHookSource.dwEndHookMsg & Chk_RButtonUp){
							bContinue = FALSE;
						}
						else{
							PostTargetMouseMessage(sHookSource, WM_RBUTTONUP, dwMBState);
						}
						dwMBState &= ~MK_RBUTTON;
					}
				}
				break;
			default:
				break;
			}

			if(!bContinue){
				Sleep(0);	// 終了待ち→他のスレッドに残りのタイムスライスを譲る
			}
		}
	}

	return bContinue;
}

//-----------------------------------------------------------------------------
// Mouse Deviceのボタンの状態を確認する
// dwMsgFlagsビットフラグで指定したボタンの状態になっているか確認する
//-----------------------------------------------------------------------------

BOOL CTPMouseDeviceHooker::CheckDeviceState(DWORD dwMsgFlags)
{
	ASSERT(m_IMouseDevice8);
	DIMOUSESTATE diState;
	BOOL bStateOn = FALSE;
	
	SetThreadState(SE_GetDeviceState);
	HRESULT hr = m_IMouseDevice8->GetDeviceState( sizeof(DIMOUSESTATE), (LPVOID)&diState );

	if(SUCCEEDED(hr))
	{
		// LButtonDownしていないか調べる
		if(dwMsgFlags & Chk_LButtonDown)
		{
			if( ( m_nSwapMouseButton  && 0x80 == diState.rgbButtons[1]) ||
				( !m_nSwapMouseButton && 0x80 == diState.rgbButtons[0]))
			{
				bStateOn = TRUE;
			}
		}

		// LButtonUpしていないか調べる
		if(dwMsgFlags & Chk_LButtonUp)
		{
			if( ( m_nSwapMouseButton  && 0x00 == diState.rgbButtons[1]) ||
				( !m_nSwapMouseButton && 0x00 == diState.rgbButtons[0]))
			{
				bStateOn = TRUE;
			}
		}

		// RButtonDwonしていないか調べる
		if(dwMsgFlags & Chk_RButtonDown)
		{
			if( ( m_nSwapMouseButton  && 0x80 == diState.rgbButtons[0]) ||
				( !m_nSwapMouseButton && 0x80 == diState.rgbButtons[1]))
			{
				bStateOn = TRUE;
			}
		}

		// RButtonUpしていないか調べる
		if(dwMsgFlags & Chk_RButtonUp)
		{
			if( ( m_nSwapMouseButton  && 0x00 == diState.rgbButtons[0]) ||
				( !m_nSwapMouseButton && 0x00 == diState.rgbButtons[1]))
			{
				bStateOn = TRUE;
			}
		}
	}

	return bStateOn;
}

//-----------------------------------------------------------------------------
// Direct Inputを初期化する
//-----------------------------------------------------------------------------
HRESULT CTPMouseDeviceHooker::CreateDirectInput()
{
	// MouseDeviceが確保済み
	if(m_IMouseDevice8 != NULL)
		return S_FALSE;

	// MouseDeviceのHook先が未設定
	if(!::IsWindow(m_hDirectInputOwner))
		return E_FAIL;
	// 唯一のDirectInputが作成済み
	if(m_IDeviceInput8 != NULL)
		return E_FAIL;

	if(m_hMouseEvent == NULL)
	    m_hMouseEvent = CreateEvent( NULL, FALSE, FALSE,NULL);

	if(m_hMouseEvent == NULL)
	{

		return E_FAIL;
	}

	HRESULT hr(E_FAIL);
	// DirectInputオブジェクト作成
	SetThreadState(SE_DirectInput8Create);
	if( FAILED( hr = DirectInput8Create(AfxGetInstanceHandle(),
										DIRECTINPUT_VERSION, 
										IID_IDirectInput8,
										(VOID**)&m_IDeviceInput8,
										NULL)) )
	{
		return hr;
	}

	if(m_IDeviceInput8 == NULL)
	{
		return E_POINTER;
	}
    
	// System Mouse Deviceインターフェースを取得する
	SetThreadState(SE_CreateDevice);
	if( FAILED( hr = m_IDeviceInput8->CreateDevice( GUID_SysMouse,		// Mouse DeviceのGUID
													&m_IMouseDevice8,	// Interfaceポインタ
													NULL) ) )
	{
		return hr;
	}

	if(m_IMouseDevice8 == NULL)
	{
		return E_POINTER;
	}
 
	// デバイスのデータ形式を指定する
	// 基本データで問題ないのでMSが提供するMouse Device用データ形式を指定
	SetThreadState(SE_SetDataFormat);
	if( FAILED( hr = m_IMouseDevice8->SetDataFormat( &c_dfDIMouse ) ) )
	{
		return hr;
	}

	// デバイスのインスタンス間の協調レベルを設定
	// @@注意@@
	// DISCL_EXCLUSIVEを利用するとCursorがSystemにより表示されなくなるので注意
	SetThreadState(SE_SetCooperativeLevel);
	if( FAILED( hr = m_IMouseDevice8->SetCooperativeLevel( m_hDirectInputOwner,
														   DISCL_NONEXCLUSIVE | DISCL_BACKGROUND ) ) )
	{
		return hr;
	}

	// Device入力(ハードウェア割り込み)待ち合わせEventの設定
	SetThreadState(SE_SetEventNotification);
	if( FAILED( hr = m_IMouseDevice8->SetEventNotification( m_hMouseEvent ) ) )
	{
		return hr;
	}

    // デバイスのバッファサイズを設定する
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj        = 0;
	dipdw.diph.dwHow        = DIPH_DEVICE;
	dipdw.dwData            = DEVICE_BUFFER_SIZE; // Arbitary buffer size

	SetThreadState(SE_SetProperty);
	if( FAILED( hr = m_IMouseDevice8->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) ) )
	{
		return hr;
	}
	// マウスのボタン設定が切り替えられているか設定を取得
	m_nSwapMouseButton = ::GetSystemMetrics( SM_SWAPBUTTON );

    return hr;
}

//-----------------------------------------------------------------------------
// Direct Inputの後始末
//-----------------------------------------------------------------------------
void CTPMouseDeviceHooker::FreeDirectInput()
{
    // Direct Inputオブジェクトの破棄
	if(m_IMouseDevice8)
		m_IMouseDevice8 = NULL;
	if(m_IDeviceInput8)
		m_IDeviceInput8 = NULL;

    if( m_hMouseEvent )
	{
        ::CloseHandle( m_hMouseEvent );
		m_hMouseEvent = NULL;
	}
}

//-----------------------------------------------------------------------------
// KeyStateの取得
//-----------------------------------------------------------------------------
UINT CTPMouseDeviceHooker::GetKeyState()
{
	UINT nKeyState = 0;
	UINT nCheckKey[] = 
	{
		VK_CONTROL
	,	VK_SHIFT
	};
	UINT nSetKeyState[] = 
	{
		MK_CONTROL
	,	MK_SHIFT
	};

	int nSize = sizeof(nCheckKey) / sizeof(nCheckKey[0]);

	for(int i=0;i<nSize;++i)
	{
		if(::GetKeyState(nCheckKey[i]) & 0x8000){
			nKeyState |= nSetKeyState[i];
		}
	}

	return nKeyState;
}

//-----------------------------------------------------------------------------
// カーソルの移動
//-----------------------------------------------------------------------------

BOOL CTPMouseDeviceHooker::UpdateCursorPosition(const SEHOOKSOURCE& sHookSource,
												int dx, int dy, DWORD dwTime, DWORD dwMBState)
{
	SetThreadState(SE_UpdateCursorPosition);
	if(!sHookSource.pCalculator || !sHookSource.bContinueCalc){
		return FALSE;
	}

	if(CheckDeviceState(sHookSource.dwEndHookMsg))
	{
		// 1度デバイスの状態変化を検出しても再チェックすると検出できない場合が
		// あるので計算継続フラグを無理やり落としてしまう
		const_cast<SEHOOKSOURCE*>(&sHookSource)->bContinueCalc = FALSE;
		return FALSE;
	}

	/*jyn added 20090509
	*comment:Slider mini adjust with control key down. set the pos plus to 1 .
	*/
	BOOL bCtrl = FALSE;
	if(::GetKeyState(VK_CONTROL)<0 || m_bCtrl)
	{
		if(::GetKeyState(VK_CONTROL)<0)
			m_bCtrl = FALSE;/*jyn added ﾓﾐﾊｱｼ�ﾅﾌﾏ鋧｢ｻ瞠ﾐﾑﾓｳﾙ｣ｬｴﾋﾊｱﾐ靨ｪﾖﾘﾖﾃｱ菽ｿ*/

		CPoint pt;
		::GetCursorPos(&pt);
		::ScreenToClient(sHookSource.hWndTarget, &pt);
		if(sHookSource.pCalculator->GetCalcAxis() & SE_CALC_X)
		{
			if(pt.x<=m_rtChannel.right  && pt.x>=m_rtChannel.left)
				bCtrl = TRUE;
			else
				bCtrl = FALSE;
		}
		else if(sHookSource.pCalculator->GetCalcAxis() & SE_CALC_Y)
		{
			if(pt.y<=m_rtChannel.bottom&& pt.y>=m_rtChannel.top)
				bCtrl = TRUE;
			else
				bCtrl = FALSE;
		}
	}
	
	INT dStep =1;
	if(m_dRange<=100)
		dStep = 5;
	if(bCtrl)
	{
		if(dx >0&&sHookSource.pCalculator->GetCalcAxis() & SE_CALC_X)
		{
			dx =dStep;
			//dy =0;
		}
		else if(dx<0&&sHookSource.pCalculator->GetCalcAxis() & SE_CALC_X)
		{
			dx =-dStep;
			//dy = 0;
		}

		if(dy >0&&sHookSource.pCalculator->GetCalcAxis() & SE_CALC_Y)
		{
			//dx =0;
			dy =dStep;
		}
		else if(dy<0&&sHookSource.pCalculator->GetCalcAxis() & SE_CALC_Y)
		{
			//dx =0;
			dy =-dStep;
		}
	}
	/*jyn added end*/
	if(!const_cast<SEHOOKSOURCE*>(&sHookSource)->pCalculator->CalcCursorPos(dx, dy, dwTime,bCtrl))
	{
		return FALSE;
	}

	SEPOINTREAL ptReal = sHookSource.pCalculator->GetAccumulatePostionReal();
	CPoint pt;
	pt.x = (int)(ptReal.x + 0.5) + sHookSource.ptBase.x;
	pt.y = (int)(ptReal.y + 0.5) + sHookSource.ptBase.y;

	//afxDump<<_T("~~~~~~~~~~~~~~~~")<<_T("dx:")<<dx<<_T("      ")<<_T("dy:")<<dy<<_T("       ")<<_T("ptReal.:")<<ptReal.y<<_T("\n");
	// システムCursorを移動する

	::SetCursorPos(pt.x, pt.y);

	// Clientへの無駄な呼び出しを避ける為、計算軸とする値に変更があるか確認する
	BOOL bDiff(FALSE);
	if(sHookSource.pCalculator->GetCalcAxis() & SE_CALC_X){
		bDiff = (m_ptPrevOffset.x != ptReal.x);
	}
	if(!bDiff && sHookSource.pCalculator->GetCalcAxis() & SE_CALC_Y){
		bDiff = (m_ptPrevOffset.y != ptReal.y);
	}
	if(!bDiff){
		return TRUE;
	}
	m_ptPrevOffset = ptReal;
	::ScreenToClient(sHookSource.hWndTarget, &pt);

	// Post前に直接通知する
	dwMBState |= GetKeyState();

	if(::IsWindow(sHookSource.hWndTarget))
	{
		::PostMessage(sHookSource.hWndTarget, WM_SE_MOUSEHOOK, dwMBState, MAKELONG(pt.x, pt.y));
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// マウスメッセージ通知
//-----------------------------------------------------------------------------
void CTPMouseDeviceHooker::PostTargetMouseMessage(const SEHOOKSOURCE& sHookSource,
												  UINT nMessage, DWORD dwMBState)
{
	SetThreadState(SE_PostTargetMouseMessage);
	ASSERT(sHookSource.pCalculator);

	SEPOINTREAL ptReal = sHookSource.pCalculator->GetAccumulatePostionReal();
	CPoint pt;
	pt.x = (int)(ptReal.x + 0.5) + sHookSource.ptBase.x;
	pt.y = (int)(ptReal.y + 0.5) + sHookSource.ptBase.y;

	::ScreenToClient(sHookSource.hWndTarget, &pt);

	dwMBState |= GetKeyState();

	// TargetウィンドウにPostMessageする
   	if(::IsWindow(sHookSource.hWndTarget)){
		::PostMessage(sHookSource.hWndTarget, nMessage, dwMBState, MAKELONG(pt.x, pt.y));
	}
}

//-----------------------------------------------------------------------------
// Mouse Deviceへのアクセス権を取得する
//-----------------------------------------------------------------------------

BOOL CTPMouseDeviceHooker::AcquireMouseDevice()
{
	if(!m_hMouseEvent){
		return FALSE;
	}

	if(!m_IDeviceInput8 || !m_IMouseDevice8){
		return FALSE;
	}

	SetThreadState(SE_Acquire);
	HRESULT hr = m_IMouseDevice8->Acquire();

	return SUCCEEDED(hr);
}

//-----------------------------------------------------------------------------
// Mouse Deviceへのアクセス権を開放する
//-----------------------------------------------------------------------------

void CTPMouseDeviceHooker::UnAcquireMouseDevice()
{
	if(m_IMouseDevice8)
	{
		// デバイスデータを残したままアクセス権を開放すると次回アクセス権取得
		// 時にそのデータを取得してしまう場合がある。
		// それを防ぐ為、残ったデータはフラッシュして置く
		DWORD dwItems = INFINITE;
		SetThreadState(SE_GetDeviceData_Flash);
		HRESULT hr = m_IMouseDevice8->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), NULL, &dwItems, 0);
		if(FAILED(hr))
		{
		}

		SetThreadState(SE_Unacquire);
		hr = m_IMouseDevice8->Unacquire();
		if(FAILED(hr))
		{
		}
	}
}

//-----------------------------------------------------------------------------
// Currentの座標を返す
//-----------------------------------------------------------------------------

CPoint CTPMouseDeviceHooker::GetCurrentPos()
{
	CPoint pt(0,0);
	SEPOINTREAL ptReal = GetCurrentPosReal();
	pt.x = (int)(ptReal.x + 0.5);
	pt.y = (int)(ptReal.y + 0.5);
	return pt;
}

//-----------------------------------------------------------------------------
// Currentの実数座標を返す
//-----------------------------------------------------------------------------

SEPOINTREAL	CTPMouseDeviceHooker::GetCurrentPosReal()
{
	SEPOINTREAL ptReal = SEPOINTREAL(0.0,0.0);
	SEHOOKSOURCE sHookSource;

	if(GetHookResource(sHookSource))
	{
		ASSERT(sHookSource.pCalculator);

		ptReal = sHookSource.pCalculator->GetAccumulatePostionReal();
		ptReal.x += sHookSource.ptBase.x;
		ptReal.y += sHookSource.ptBase.y;
	}
	else
	{
		ptReal.x = m_ptPrevOffset.x + sHookSource.ptBase.x;
		ptReal.y = m_ptPrevOffset.y + sHookSource.ptBase.y;
	}

	return ptReal;
}

//-----------------------------------------------------------------------------
// Hook中に利用するソースを設定する
//-----------------------------------------------------------------------------

void CTPMouseDeviceHooker::SetHookSource(const SEHOOKSOURCE* pSrc)
{
	m_csHookSource.Lock();

	if(pSrc)
	{
		m_sHookSource.pCalculator	= pSrc->pCalculator;
		m_sHookSource.hWndTarget	= pSrc->hWndTarget;
		m_sHookSource.dwEndHookMsg	= pSrc->dwEndHookMsg;
		m_sHookSource.ptBase		= pSrc->ptBase;
		m_sHookSource.bContinueCalc	= pSrc->bContinueCalc;
	}
	else
	{
		m_sHookSource.pCalculator	= NULL;
		m_sHookSource.hWndTarget	= NULL;
		m_sHookSource.dwEndHookMsg	= 0;
		m_sHookSource.bContinueCalc	= TRUE;
	}
	m_csHookSource.Unlock();
}

//-----------------------------------------------------------------------------
// Hook中に利用するソースを取得する
//-----------------------------------------------------------------------------

BOOL CTPMouseDeviceHooker::GetHookResource(SEHOOKSOURCE& sDest)
{
	SetThreadState(SE_GetHookResource);
	m_csHookSource.Lock();
	
	sDest.pCalculator	 = m_sHookSource.pCalculator;
	sDest.hWndTarget	 = m_sHookSource.hWndTarget;
	sDest.dwEndHookMsg	 = m_sHookSource.dwEndHookMsg;
	sDest.ptBase		 = m_sHookSource.ptBase;
	sDest.bContinueCalc	 = m_sHookSource.bContinueCalc;

	m_csHookSource.Unlock();

	return (sDest.pCalculator != NULL && sDest.hWndTarget != NULL);
}

//-----------------------------------------------------------------------------
// Hook中に利用するソースの有無でHook中かどうか判定する
//-----------------------------------------------------------------------------

BOOL CTPMouseDeviceHooker::IsHook()
{
	BOOL bIsHook = FALSE;
	m_csHookSource.Lock();

	bIsHook = (m_sHookSource.pCalculator != NULL && m_sHookSource.hWndTarget != NULL);

	m_csHookSource.Unlock();
	return bIsHook;
}

//-----------------------------------------------------------------------------
// Thread Proxy
//-----------------------------------------------------------------------------

unsigned CTPMouseDeviceHooker::MouseDeviceThreadProxy(LPVOID pParameter)
{
	if(!pParameter){
		return 0;
	}

	CTPMouseDeviceHooker* pHooker = (CTPMouseDeviceHooker*)pParameter;

	return pHooker->MouseDeviceMainThread();
}

