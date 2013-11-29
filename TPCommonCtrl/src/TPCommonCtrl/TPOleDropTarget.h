#pragma once

#define WM_OLEDROPTARGET_MSG    WM_USER  +1467
struct AFX_EXT_CLASS TPOleDropData
{
	CWnd           *pWnd;
	COleDataObject *pDataObject;
	DWORD           dwKeyState;
	CPoint          point;
	DROPEFFECT      eRetrun;

public:
	BOOL           CheckType(UINT uType,LPCTSTR sType);
};
class AFX_EXT_CLASS CTPOleDropSource: public COleDropSource
{
public:
	CTPOleDropSource();
	virtual ~CTPOleDropSource();

};
class AFX_EXT_CLASS CTPOleDropTarget: public COleDropTarget
{
	DECLARE_DYNAMIC(CTPOleDropTarget)

public:
	CTPOleDropTarget();
	virtual ~CTPOleDropTarget();

public:
	CWnd              *m_pMsgWnd;
private:
	TPOleDropData      m_stuOleDropData;
public:
	virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject,	DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject,	DWORD dwKeyState, CPoint point);
	virtual BOOL       OnDrop(CWnd* pWnd, COleDataObject* pDataObject,DROPEFFECT dropEffect, CPoint point);	
	virtual void       OnDragLeave(CWnd* pWnd);
protected:
	DECLARE_MESSAGE_MAP()
};


