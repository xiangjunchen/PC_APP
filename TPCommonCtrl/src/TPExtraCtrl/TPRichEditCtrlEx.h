
#pragma once

#define WC_RIECHEDITEX _L("CTPRichEditEx")

#include "TPBmpPicker.h"
#define CHATMSG	1
#define CHATLOG	2


class AFX_EXT_CLASS CTPRichEditCtrlEx : public CTPRichEditCtrl
{
	DECLARE_DYNAMIC(CTPRichEditCtrlEx)

public:
	CTPRichEditCtrlEx();
	virtual ~CTPRichEditCtrlEx();

	static DWORD CALLBACK CStringFormatToRTF(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
	static DWORD CALLBACK RTFFormatToCString(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
	void SaveToRTFFile(CString sFilePath ,CString sFileName,BOOL bDelete);
	void ReadFromRTFFile(CString sFilePath, CString sFileName,double filePos);
	void SetRTF(CString strRTF);
	CString GetRTF();
	void InsertBitmap(HBITMAP hBitmap,int index);
	void InsertBitmap(CString strFileName);
	void SetType(int type);	//设置聊天编辑框，或者聊天记录框（★★★★★）
	CString EncodeRichMessage();   //编码成转义字符
	void DecodeRichMessage(CString strText,CBmpPicker* m_BmpPicker);  //解码成表情图片
	void AddMessageUser(CString strName,COLORREF crColor);
	void AddMessgaeText(CString strText,CString crFontName,UINT crFontSize,BOOL bIsBold,BOOL bIsItalic,BOOL bIsUnderline,COLORREF crColor,CBmpPicker* m_BmpPicker);
	BOOL PreTranslateMessage(MSG* pMsg);
	static void  RegisterClass(BOOL bRegister);

protected:
	DECLARE_MESSAGE_MAP()
	LPSTORAGE m_lpStorage;  // provides storage for m_lpObject
	LPLOCKBYTES m_lpLockBytes;  // part of implementation of m_lpStorage
	LPOLEOBJECT m_lpObject; // in case you want direct access to the OLE object
	LPVIEWOBJECT2 m_lpViewObject;// IViewObject for IOleObject above
	COleDataSource *m_pDataSource;
	int m_type;

	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCopy() ;
	afx_msg void OnCut() { Cut(); }
	afx_msg void OnPaste();
	afx_msg void OnPaint();
	afx_msg void OnSelectall() { SetSel(0, -1); }
	afx_msg void OnUndo() { Undo(); }
	afx_msg void OnClear() { SetWindowText(_T("")); }
	afx_msg void OnSelectfont();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnEnLink(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
protected:
	virtual void PreSubclassWindow();
};


struct stFace
{
	long nPos;			//要插入的位置
	int nAtFace;		//插入表情的代号
	CString strFace;		//要插入的图的转义字符
	int nLength;		//转义字符表情的长度
};