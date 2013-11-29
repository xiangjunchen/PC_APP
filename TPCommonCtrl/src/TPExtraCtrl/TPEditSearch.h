#pragma once


// CTPEditSearch

class AFX_EXT_CLASS CTPEditSearch : public CTPEditEx
{
	DECLARE_DYNAMIC(CTPEditSearch)

public:
	CTPEditSearch();
	virtual ~CTPEditSearch();

private:
	TPCommonRes     *m_pCommomRes[2];
    UINT             m_uCommonRes[2];
	CString          m_sCommonText;
public:
	void  SetResourceName(CString &sFileName,UINT uState);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg void OnDestroy();
};


