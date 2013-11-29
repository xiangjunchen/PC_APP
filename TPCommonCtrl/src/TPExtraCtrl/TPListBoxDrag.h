#pragma once


// CTPListBoxDrag
//WM_DRAGLISTITEM  WPARAM LOWORD Src HIWORD iDest ,LPARAM this
class AFX_EXT_CLASS CTPListBoxDrag : public CTPListBox/////jyn
{
	DECLARE_DYNAMIC(CTPListBoxDrag)

public:
	CTPListBoxDrag();
	virtual ~CTPListBoxDrag();
	UINT GetState()
	{return m_uState;}
	void SetUpState();
	int  GetLastHitIndex()
	{return m_iLastHitIndex;}
	int GetMovingIndex()
	{return m_MovingIndex;}
	int GetMoveToIndex()
	{return m_MoveToIndex;}
	void SetLastHitIndex(int index)
	{m_iLastHitIndex =index;}
	void PreSetUpState();

	BOOL   GetItemIsEnable(int iItem);//xl added

public:
	BOOL (*m_pCheck)(CTPListBoxDrag* pListBoxDrag);

private:
   int   m_MovingIndex;
   int   m_MoveToIndex;
   UINT  m_uState;        // decide whether is drop to timeline.(0x0002)
   int   m_iUnShiftBtnDown; // shift联动的计算参数之一
   int   m_iLastHitIndex;//记录最近一次点击高亮的记录索引
   BOOL  m_Captured;
   BOOL  m_bButtondown;
   DWORD m_Interval;    //scroll speed when an item is dragged above or below the window
   BOOL  m_bSelect;

   virtual void  DrawText(CDC *pDC,CRect &rtItem,CString &sText); 

protected:
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
   afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()


   void SetListBoxSel(int iindex,BOOL bDown = TRUE);
   void InsertDraggedItem();
   void DoTheScrolling(CPoint Point,CRect ClientRect);
   void DrawTheLines(int Index);
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};


