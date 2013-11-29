#pragma once


// CTPComboBoxEx

class AFX_EXT_CLASS CTPComboBoxEx : public CComboBoxEx ,public CTPBaseCtrl 
{
	DECLARE_DYNAMIC(CTPComboBoxEx)

public:
	CTPComboBoxEx();
	virtual ~CTPComboBoxEx();
private:
	BOOL                    m_bSubWindow; 	
	BOOL                    m_bCreate;
	CTPComboBox            *m_pCombobox;
	CTPEdit                *m_pEdit;
public:
    void                    PaintItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
private:
	void                    SubWindow(BOOL bSub);
	

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchange();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


