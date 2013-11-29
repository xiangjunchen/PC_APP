// SeBaseWndController.h: CTPMouseDeviceImpl クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEBASEWNDCONTROLLER_H__91EDA003_0360_4AC2_AF2C_061867511C2E__INCLUDED_)
#define AFX_SEBASEWNDCONTROLLER_H__91EDA003_0360_4AC2_AF2C_061867511C2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTPMouseDeviceHooker;
class CTPMouseCalc;
///
/// </summary>
class CTPMouseDeviceImpl  
{
public:
	CTPMouseDeviceImpl();
	virtual ~CTPMouseDeviceImpl();

public:

	// マウスデバイスフック(DirectInput)機能をFxEditorに付ける
	static bool	InstallMouseDeviceHooker(CWnd* pHookOwner,DWORD dwWinType);
	// マウスデバイスフック(DirectInput)機能をFxEditorからはずす
	static void	UninstallMouseDeviceHooker(DWORD dwWinType);
	// スライダ操作モード(Intelligent/Normal/Original)を取得する
	static int	GetSliderOperationType();
	// マウスデバイスフッククラスを取得する
	static CTPMouseDeviceHooker*	GetMouseDeviceHooker();
	// Intelligentモードマウス計算クラスを取得する
	static CTPMouseCalc*			GetDefMouseCalcIntelligent();

	

protected:
	static CTPMouseDeviceHooker*	m_pMouseDvHooker;	/// マウスフッククラス
	static int						m_nOperateType;		/// スライダ操作モード
	static CTPMouseCalc*			m_pDefCalculator;	/// Intellilgentモードマウス計算クラス
	static INT						m_uCount;			
	static DWORD					m_dwWinAry;
};


inline CTPMouseDeviceHooker* CTPMouseDeviceImpl::GetMouseDeviceHooker()
{	return m_pMouseDvHooker;	}
// Intelligentモードマウス計算クラスを取得する
inline CTPMouseCalc* CTPMouseDeviceImpl::GetDefMouseCalcIntelligent()
{	return m_pDefCalculator;	}

#endif
