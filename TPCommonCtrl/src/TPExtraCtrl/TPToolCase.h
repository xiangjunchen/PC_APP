#pragma once


// CTPToolCase
#define WC_TOOLCASE     _L("TPToolCase")
typedef struct tag_TPToolCase_Item
{
	CString      sLable;
	int          iImage;
	CRect        rtItem;
	CRect        rtLable;
	CRect        rtIamge;
	UINT         uState;
	LPARAM       pParam;
	tag_TPToolCase_Item()
	{
		sLable   = _L("");
		iImage   = -1;
		rtItem   = CRect(0,0,0,0);
		rtLable  = CRect(0,0,0,0);
		rtIamge  = CRect(0,0,0,0);
		uState   = 0;
		pParam   = 0;
	}

}TPToolCaseItem,*LPTPToolCaseItem;
typedef CArray<TPToolCaseItem *,TPToolCaseItem *&> TPToolCaseItemArray;

class AFX_EXT_CLASS CTPToolCase : public CTPWnd,public CTPBaseCtrl
{
	DECLARE_DYNAMIC(CTPToolCase)

public:
	CTPToolCase();
	virtual ~CTPToolCase();
protected:
	BOOL                    m_bSubWindow;
	BOOL                    m_bCreate;
	TP_BORDER_STYLE         m_eBorderStyle;
	BOOL                    m_bVert;
	BOOL                    m_bPaint;

protected:
	int                    m_iBackColor;
	TPToolCaseItemArray    m_aRoot;
	HIMAGELIST             m_hImageList;
	CSize                  m_szImage;
	int                    m_iTopSize;
 
public:
	static void    RegisterClass(BOOL bRegister);
	void           SubWindow(BOOL bSub);

	void           SetImageList(HIMAGELIST hImageList);
	int            InsertItem(int iItem,CString sLabel,int iImage = -1);
	void           DeleteItem(int iItem);
	int            GetItemCount();

	void           SetItemData(int iItem,LPARAM lParam);
	LPARAM         GetItemData(int iItem);

	void           SetReDraw(BOOL bPaint);

protected:
	void           FunCalcSize(); 
	void           PaintItem(CDC *pDC,int iItem);

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


