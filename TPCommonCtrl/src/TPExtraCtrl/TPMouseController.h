// SeBaseWndController.h: CTPMouseDeviceImpl �N���X�̃C���^�[�t�F�C�X
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

	// �}�E�X�f�o�C�X�t�b�N(DirectInput)�@�\��FxEditor�ɕt����
	static bool	InstallMouseDeviceHooker(CWnd* pHookOwner,DWORD dwWinType);
	// �}�E�X�f�o�C�X�t�b�N(DirectInput)�@�\��FxEditor����͂���
	static void	UninstallMouseDeviceHooker(DWORD dwWinType);
	// �X���C�_���샂�[�h(Intelligent/Normal/Original)���擾����
	static int	GetSliderOperationType();
	// �}�E�X�f�o�C�X�t�b�N�N���X���擾����
	static CTPMouseDeviceHooker*	GetMouseDeviceHooker();
	// Intelligent���[�h�}�E�X�v�Z�N���X���擾����
	static CTPMouseCalc*			GetDefMouseCalcIntelligent();

	

protected:
	static CTPMouseDeviceHooker*	m_pMouseDvHooker;	/// �}�E�X�t�b�N�N���X
	static int						m_nOperateType;		/// �X���C�_���샂�[�h
	static CTPMouseCalc*			m_pDefCalculator;	/// Intellilgent���[�h�}�E�X�v�Z�N���X
	static INT						m_uCount;			
	static DWORD					m_dwWinAry;
};


inline CTPMouseDeviceHooker* CTPMouseDeviceImpl::GetMouseDeviceHooker()
{	return m_pMouseDvHooker;	}
// Intelligent���[�h�}�E�X�v�Z�N���X���擾����
inline CTPMouseCalc* CTPMouseDeviceImpl::GetDefMouseCalcIntelligent()
{	return m_pDefCalculator;	}

#endif
