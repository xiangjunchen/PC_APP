// SeMouseDeviceHooker.cpp: CTPMouseDeviceHooker �N���X�̃C���v�������e�[�V����
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
// �\�z/����
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
// Mouse Device�t�b�N���W���[���̏�����
//-----------------------------------------------------------------------------
// hDirectInputOwer�ɕt����
// @@@����@@@
// DirectInput�𗘗p����v���Z�X�̍ŏ�ʃE�B���h�E�łȂ���΂����Ȃ�
// DirectInputDevice�̃A�N�Z�X�����擾���ɔj������Ă͂����Ȃ�

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
// Mouse Device�t�b�N���W���[���̌�n��
//-----------------------------------------------------------------------------

void CTPMouseDeviceHooker::UninstallMouseDeviceHook()
{
	// ������Thread���I��������
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
// Mouse Device�t�b�N
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

	// �X���b�h���N����Device�̓��͏���������
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

	// �X���b�h���N������܂Ŏw�莞�ԑ҂����킹��
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
// Mouse Device�t�b�N
//-----------------------------------------------------------------------------

BOOL CTPMouseDeviceHooker::StartHook(HWND hWndTarget, 
									 const CPoint& ptPos, 
									 CTPMouseCalc* pCalc, 
									 DWORD dwEndHookMsg)
{

	if(!::IsWindow(hWndTarget) || !m_hThread || !m_hHookStart || !m_hHookExit || !m_hCallback)
		return FALSE;

	// �܂��t�b�N���Ȃ��Hook�����I������
	if(IsHook())
	{
		// Hook�𔲂����Ȃ��̂ŊJ�n���s�Ƃ���
		if(!ExitHook())
			return FALSE;
	}

	// CustomCalclator/DefaultCalclator�𗘗p���邩���肷��
	BOOL bResult = TRUE;
	CTPMouseCalc* pCalculator = m_pNormalEmuCalc;
	if(pCalc)
		pCalculator = pCalc;

	if(!pCalculator)
		return FALSE;

	pCalculator->Initialize();

	SEHOOKSOURCE sHookSource = { pCalculator, hWndTarget, dwEndHookMsg, {ptPos.x, ptPos.y}, TRUE };

	SetHookSource(&sHookSource);

	// �eClient�̌v�Z���[�e�B�����Z�b�g
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
// Mouse Device�t�b�N�J�n
//-----------------------------------------------------------------------------

BOOL CTPMouseDeviceHooker::InitializeThread()
{
	m_dwThreadID = ::GetCurrentThreadId();

	// �X���b�h�Ƀ��b�Z�[�W�L���[���쐬����
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
		// Thread�̏�����
		if(InitializeThread())
		{
			// Thread����̏���擾�Ɏ��Ԃ��|����̂�1�񂾂��擾���Ă���
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
			// Hook�̊J�n/Thread�I���C�x���g�܂őҋ@
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
			{	// PostWork�C�x���g���V�O�i����ԂɂȂ���
				MouseDeviceSubThread();
			}
			else
			{	// ���b�Z�[�W����M����
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
// Mouse Device�t�b�N�J�n
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
	// MouseDevice��v��
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
	// Hook�������Ɉړ������J�[�\�����v�Z����
	/////////////////////////////////////////////////////////////
	if(pt.x != 0 || pt.y != 0)
	{
		DWORD dwCurTime = ::GetTickCount();
		UpdateCursorPosition(sHookSource, pt.x, pt.y, dwCurTime, 0);
	}

	// MouseHook��������
	::SetEvent(m_hCallback);

	// �n�[�h�E�F�A���荞�݂�������Hook�I���C�x���g�܂őҋ@
	while( !bDone ) 
	{
		SetThreadState(SE_WaitMouseEvent);
		dwResult = WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);

		switch( dwResult ) 
		{
		// m_hHookExit�C�x���g�V�O�i��
		case WAIT_OBJECT_0 + 0:
			bDone = TRUE;
			break;
		// m_hMouseEvent�C�x���g�V�O�i��
		case WAIT_OBJECT_0 + 1:
			OnMouseInput(sHookSource);
			break;
		default:
			bDone = TRUE;
			SetThreadResult(FALSE);
			break;
		}
	}

	// Mouse Device�A�N�Z�X�����J��
	UnAcquireMouseDevice();

	// ���\�[�X�J��
	SetHookSource(NULL);

	sHookSource.pCalculator->Uninitialize();

	// Thread�I��Callback
	::SetEvent(m_hCallback);

	return TRUE;
}

//-----------------------------------------------------------------------------
// Mouse Device�t�b�N�I��
//-----------------------------------------------------------------------------
BOOL CTPMouseDeviceHooker::ExitHook()
{
	if(m_hThread == NULL)
		return FALSE;

	SEHOOKSOURCE sHookSource;
	if(GetHookResource(sHookSource))
	{
		// Thread���f�C�x���g�ݒ�
		::ResetEvent(m_hCallback);
		::SetEvent(m_hHookExit);

		// Thread���I������܂�MainThread��őҋ@
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
// Mouse Device�t�b�N�I��
//-----------------------------------------------------------------------------
void CTPMouseDeviceHooker::EndHookMouseThread()
{

	DWORD dwResult = WAIT_FAILED;

	// Thread�I���܂őҋ@
	if(m_hThread != NULL)
	{
		// Hook���ł���΋���Hook����
		ExitHook();

		if(m_dwThreadID != 0)
		{
			// Thread���f�C�x���g�ݒ�
			::PostThreadMessage(m_dwThreadID, WM_QUIT, 0,0);

			// Thread���I������܂�MainThread��őҋ@
			dwResult = ::WaitForSingleObject(m_hThread, SE_MOUSE_HOOK_THREAD_WAITTIME);
		}

		if(dwResult != WAIT_OBJECT_0)
		{
			::TerminateThread(m_hThread, 0);
		}

		// Thread�I����̓C�x���g��j������
		::CloseHandle(m_hThread);

		// �X���b�h�̋����j�����s��ꂽ�ꍇ�A��񂪎c���Ă��܂��̂ŔO�ׂ̈ɔj�����Ă���
		SEHOOKSOURCE sHookSource;
		if(GetHookResource(sHookSource))
			sHookSource.pCalculator->Uninitialize();
	}

	SetHookSource(NULL);
	m_hThread = NULL;
}

//-----------------------------------------------------------------------------
// Mouse Device�̐��f�[�^(Mickey)����
// Mouse����擾���鐶�f�[�^(Mickey)�̓f�o�C�X�ˑ��̃f�[�^��Windows��̐ݒ�
// �Ɉˑ����Ȃ�
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

	// DirectInputDevice����o�b�t�@�����O���ꂽ���f�[�^��ǂݏo���ď���������
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

		// �L���ȃf�[�^�����������̂ŏI��
		if(dwElements == 0){
			break;
		}

		for(UINT i=0;i<dwElements && bContinue;++i)
		{
			// �ύX���ꂽDeviceState���Ƃɕ��򏈗�
			switch( od[i].dwOfs ) 
			{     
			case DIMOFS_X:	// X��
				bContinue = UpdateCursorPosition(sHookSource, od[i].dwData, 0, od[i].dwTimeStamp, dwMBState);
				break;

			case DIMOFS_Y:	// Y��
				bContinue = UpdateCursorPosition(sHookSource, 0, od[i].dwData, od[i].dwTimeStamp, dwMBState);
				break;

			case DIMOFS_BUTTON0: // �}�E�X�{�^��0��������1�̏���
			case DIMOFS_BUTTON1: // ���̑��̃J�X�^���{�^���̓T�|�[�g���Ȃ�
				// Left�{�^���N���b�N������(System�ݒ��Swap�����ꍇ�ɂ��Ή�)
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

				// Right�{�^���N���b�N������(System�ݒ��Swap�����ꍇ�ɂ��Ή�)
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
				Sleep(0);	// �I���҂������̃X���b�h�Ɏc��̃^�C���X���C�X������
			}
		}
	}

	return bContinue;
}

//-----------------------------------------------------------------------------
// Mouse Device�̃{�^���̏�Ԃ��m�F����
// dwMsgFlags�r�b�g�t���O�Ŏw�肵���{�^���̏�ԂɂȂ��Ă��邩�m�F����
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
		// LButtonDown���Ă��Ȃ������ׂ�
		if(dwMsgFlags & Chk_LButtonDown)
		{
			if( ( m_nSwapMouseButton  && 0x80 == diState.rgbButtons[1]) ||
				( !m_nSwapMouseButton && 0x80 == diState.rgbButtons[0]))
			{
				bStateOn = TRUE;
			}
		}

		// LButtonUp���Ă��Ȃ������ׂ�
		if(dwMsgFlags & Chk_LButtonUp)
		{
			if( ( m_nSwapMouseButton  && 0x00 == diState.rgbButtons[1]) ||
				( !m_nSwapMouseButton && 0x00 == diState.rgbButtons[0]))
			{
				bStateOn = TRUE;
			}
		}

		// RButtonDwon���Ă��Ȃ������ׂ�
		if(dwMsgFlags & Chk_RButtonDown)
		{
			if( ( m_nSwapMouseButton  && 0x80 == diState.rgbButtons[0]) ||
				( !m_nSwapMouseButton && 0x80 == diState.rgbButtons[1]))
			{
				bStateOn = TRUE;
			}
		}

		// RButtonUp���Ă��Ȃ������ׂ�
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
// Direct Input������������
//-----------------------------------------------------------------------------
HRESULT CTPMouseDeviceHooker::CreateDirectInput()
{
	// MouseDevice���m�ۍς�
	if(m_IMouseDevice8 != NULL)
		return S_FALSE;

	// MouseDevice��Hook�悪���ݒ�
	if(!::IsWindow(m_hDirectInputOwner))
		return E_FAIL;
	// �B���DirectInput���쐬�ς�
	if(m_IDeviceInput8 != NULL)
		return E_FAIL;

	if(m_hMouseEvent == NULL)
	    m_hMouseEvent = CreateEvent( NULL, FALSE, FALSE,NULL);

	if(m_hMouseEvent == NULL)
	{

		return E_FAIL;
	}

	HRESULT hr(E_FAIL);
	// DirectInput�I�u�W�F�N�g�쐬
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
    
	// System Mouse Device�C���^�[�t�F�[�X���擾����
	SetThreadState(SE_CreateDevice);
	if( FAILED( hr = m_IDeviceInput8->CreateDevice( GUID_SysMouse,		// Mouse Device��GUID
													&m_IMouseDevice8,	// Interface�|�C���^
													NULL) ) )
	{
		return hr;
	}

	if(m_IMouseDevice8 == NULL)
	{
		return E_POINTER;
	}
 
	// �f�o�C�X�̃f�[�^�`�����w�肷��
	// ��{�f�[�^�Ŗ��Ȃ��̂�MS���񋟂���Mouse Device�p�f�[�^�`�����w��
	SetThreadState(SE_SetDataFormat);
	if( FAILED( hr = m_IMouseDevice8->SetDataFormat( &c_dfDIMouse ) ) )
	{
		return hr;
	}

	// �f�o�C�X�̃C���X�^���X�Ԃ̋������x����ݒ�
	// @@����@@
	// DISCL_EXCLUSIVE�𗘗p�����Cursor��System�ɂ��\������Ȃ��Ȃ�̂Œ���
	SetThreadState(SE_SetCooperativeLevel);
	if( FAILED( hr = m_IMouseDevice8->SetCooperativeLevel( m_hDirectInputOwner,
														   DISCL_NONEXCLUSIVE | DISCL_BACKGROUND ) ) )
	{
		return hr;
	}

	// Device����(�n�[�h�E�F�A���荞��)�҂����킹Event�̐ݒ�
	SetThreadState(SE_SetEventNotification);
	if( FAILED( hr = m_IMouseDevice8->SetEventNotification( m_hMouseEvent ) ) )
	{
		return hr;
	}

    // �f�o�C�X�̃o�b�t�@�T�C�Y��ݒ肷��
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
	// �}�E�X�̃{�^���ݒ肪�؂�ւ����Ă��邩�ݒ���擾
	m_nSwapMouseButton = ::GetSystemMetrics( SM_SWAPBUTTON );

    return hr;
}

//-----------------------------------------------------------------------------
// Direct Input�̌�n��
//-----------------------------------------------------------------------------
void CTPMouseDeviceHooker::FreeDirectInput()
{
    // Direct Input�I�u�W�F�N�g�̔j��
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
// KeyState�̎擾
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
// �J�[�\���̈ړ�
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
		// 1�x�f�o�C�X�̏�ԕω������o���Ă��ă`�F�b�N����ƌ��o�ł��Ȃ��ꍇ��
		// ����̂Ōv�Z�p���t���O�𖳗���藎�Ƃ��Ă��܂�
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
			m_bCtrl = FALSE;/*jyn added ��ʱ������Ϣ�����ӳ٣���ʱ��Ҫ���ñ���*/

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
	// �V�X�e��Cursor���ړ�����

	::SetCursorPos(pt.x, pt.y);

	// Client�ւ̖��ʂȌĂяo���������ׁA�v�Z���Ƃ���l�ɕύX�����邩�m�F����
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

	// Post�O�ɒ��ڒʒm����
	dwMBState |= GetKeyState();

	if(::IsWindow(sHookSource.hWndTarget))
	{
		::PostMessage(sHookSource.hWndTarget, WM_SE_MOUSEHOOK, dwMBState, MAKELONG(pt.x, pt.y));
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// �}�E�X���b�Z�[�W�ʒm
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

	// Target�E�B���h�E��PostMessage����
   	if(::IsWindow(sHookSource.hWndTarget)){
		::PostMessage(sHookSource.hWndTarget, nMessage, dwMBState, MAKELONG(pt.x, pt.y));
	}
}

//-----------------------------------------------------------------------------
// Mouse Device�ւ̃A�N�Z�X�����擾����
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
// Mouse Device�ւ̃A�N�Z�X�����J������
//-----------------------------------------------------------------------------

void CTPMouseDeviceHooker::UnAcquireMouseDevice()
{
	if(m_IMouseDevice8)
	{
		// �f�o�C�X�f�[�^���c�����܂܃A�N�Z�X�����J������Ǝ���A�N�Z�X���擾
		// ���ɂ��̃f�[�^���擾���Ă��܂��ꍇ������B
		// �����h���ׁA�c�����f�[�^�̓t���b�V�����Ēu��
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
// Current�̍��W��Ԃ�
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
// Current�̎������W��Ԃ�
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
// Hook���ɗ��p����\�[�X��ݒ肷��
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
// Hook���ɗ��p����\�[�X���擾����
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
// Hook���ɗ��p����\�[�X�̗L����Hook�����ǂ������肷��
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

