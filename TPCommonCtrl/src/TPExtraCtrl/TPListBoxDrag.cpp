// DragAndDropListBox.cpp : implementation file
//

#include "stdafx.h"
#include "TPListBoxDrag.h"
#include ".\tplistboxdrag.h"

#define TID_SCROLLDOWN  100
#define TID_SCROLLUP    101

#define TP_DRAG_DIALOG    0x0001
#define TP_DRAG_TMIELINE  0x0002
#define TP_DRAG_OUTSIDE    0x0004


#define TP_WM_USER_LBUTTONDOWN		0x0001
#define TP_WM_USER_MOUSEMOVE		0x0002
#define TP_WM_USER_LBUTTONUP		0x0004
#define TP_WM_USER_DELETE			0x0008
#define TP_WM_USER_MENU				0x0010
#define TP_WM_USER_LBUTTONDOWNOUT	0x0020


// CTPListBoxDrag

IMPLEMENT_DYNAMIC(CTPListBoxDrag, CTPListBox)
CTPListBoxDrag::CTPListBoxDrag()
: m_MovingIndex(-1)
, m_MoveToIndex(-1)
, m_Captured(FALSE)
, m_Interval(0)
, m_uState(FALSE)
,m_iUnShiftBtnDown(-1)
,m_iLastHitIndex(-1)
,m_bButtondown(FALSE)
,m_pCheck(NULL)
{
	m_bSelect = FALSE;
}

CTPListBoxDrag::~CTPListBoxDrag()
{
}


BEGIN_MESSAGE_MAP(CTPListBoxDrag, CTPListBox)
   ON_WM_LBUTTONDOWN()
   ON_WM_LBUTTONUP()
   ON_WM_MOUSEMOVE()
   ON_WM_TIMER()
   ON_WM_MOUSEWHEEL()
   ON_WM_KEYDOWN()
   ON_WM_RBUTTONDOWN()
   ON_WM_RBUTTONUP()
END_MESSAGE_MAP()



// CTPListBoxDrag message handlers


void CTPListBoxDrag::SetListBoxSel(int Index,BOOL bDown)
{
	m_bSelect = TRUE;
	if(Index<0) return;
	else if (Index >=m_aItemData.GetSize()) return ;
	else
	{
		BOOL    bCtrlDown      = TP_IsCtrlKeyDown();
		BOOL    bAltDown       = TP_IsAltKeyDown();
		BOOL    bShiftCtrl     = TP_IsShiftKeyDown(); 

		if(bShiftCtrl  || bCtrlDown   )
		{
			//DQAdb00009147
			if(bShiftCtrl)
			{
				if(m_iUnShiftBtnDown==-1)   m_iUnShiftBtnDown=Index;
				int effbottom = (INT)m_aItemData.GetSize()-1;  //计算最下边一个特技的序号
				if(Index > m_iUnShiftBtnDown)  //wxd 20100603
				{
					SelItemRange(TRUE,m_iUnShiftBtnDown,Index);
                    if(m_iUnShiftBtnDown>0)
					   SelItemRange(FALSE,0,m_iUnShiftBtnDown-1);
					if(Index<effbottom)
					   SelItemRange(FALSE,Index+1,effbottom);
				}
				else if(Index < m_iUnShiftBtnDown)
				{
					SelItemRange(TRUE,Index,m_iUnShiftBtnDown);
					if(Index>0)
					   SelItemRange(FALSE,0,Index-1);
					if(m_iUnShiftBtnDown<effbottom)
                       SelItemRange(FALSE,m_iUnShiftBtnDown+1,effbottom);
				}
				else
				{
					if(Index>0)
					   SelItemRange(FALSE,0,Index-1);
					if(Index<effbottom)
					   SelItemRange(FALSE,Index+1,effbottom);
					SetSel(Index,TRUE);
				}
			}
			else if(bCtrlDown)
			{
				if(GetSel(Index)) SetSel(Index,FALSE);
				else              SetSel(Index,TRUE);
			}
		}
		else
		{
			for(int i=0;i<GetCount();i++)
			{
				if(i==Index) SetSel(i);
				else SetSel(i,FALSE);
			}
		}

	}
}
void CTPListBoxDrag::OnLButtonDown(UINT nFlags, CPoint point)
{
  // CTPListBox::OnLButtonDown(nFlags, point);
   if(GetFocus()!=this) SetFocus(); // li song ming snp4100033350 和CTPListCtrlEx::OnLButtonDown中做法一致。

   BOOL    bCtrlDown      = TP_IsCtrlKeyDown();
   BOOL    bShiftCtrl     = TP_IsShiftKeyDown(); 

   m_MovingIndex = LB_ERR;
   m_MoveToIndex = LB_ERR;
   m_bSelect     = FALSE;
   m_Captured    = FALSE;
   m_Interval    = 0;
   BOOL Outside;
   
   int Index = ItemFromPoint(point,Outside);
   if (Index != LB_ERR && !Outside)
   {
      m_MovingIndex = Index;
	  m_MoveToIndex = Index;
	  if(!bShiftCtrl) m_iUnShiftBtnDown = Index;
	  m_iLastHitIndex = Index;
	  if(!bCtrlDown  )
		  SetListBoxSel(Index,TRUE);
   }
   else
   {
	   GetParent() ->SendMessage( WM_LISTCTRL, (WPARAM)TP_WM_USER_LBUTTONDOWNOUT,(LPARAM)this);
	   return;
   }

   if(bCtrlDown ||   bShiftCtrl) 
   {
	   m_uState = 0;
	   m_uState |= TP_DRAG_TMIELINE;
	   GetParent() ->SendMessage( WM_LISTCTRL, (WPARAM)TP_WM_USER_LBUTTONDOWN,(LPARAM)this);
   }
   else {m_uState = 0;m_uState |= TP_DRAG_DIALOG;}
   m_bButtondown = TRUE;

}
void CTPListBoxDrag::SetUpState()
{
	m_MovingIndex = LB_ERR;
	m_MoveToIndex = LB_ERR;
	m_uState &= ~(TP_DRAG_DIALOG | TP_DRAG_TMIELINE | TP_DRAG_OUTSIDE);
	ReleaseCapture();
	m_bButtondown = FALSE;
	Invalidate();
	UpdateWindow();
}
void CTPListBoxDrag::PreSetUpState()
{
	if (m_MovingIndex < m_MoveToIndex)
	{
		m_MoveToIndex = m_MoveToIndex - 1;
	}
}
		


void CTPListBoxDrag::OnLButtonUp(UINT nFlags, CPoint point)
{
   if (m_MovingIndex != LB_ERR && m_Captured)
   {
	  if(m_uState & TP_DRAG_TMIELINE)
	  {
		  GetParent() ->SendMessage(WM_LISTCTRL,(WPARAM)TP_WM_USER_LBUTTONUP,(LPARAM)this);
		  ReleaseCapture();
	  }
	  else if(m_uState & TP_DRAG_DIALOG )
	  {
		  KillTimer(TID_SCROLLDOWN);
		  KillTimer(TID_SCROLLUP);
		  m_Interval = 0;
		  m_Captured = FALSE;
		  ReleaseCapture();

		  CRect Rect; 
		  GetClientRect(&Rect);

		  if (Rect.PtInRect(point))
		  {
			  if (m_pCheck && !m_pCheck(this))  Invalidate();
			  else								InsertDraggedItem();
		  }
		  else
		  {
			  Invalidate();
			  UpdateWindow();
		  }
	  }
      
      m_MovingIndex = LB_ERR;
      m_MoveToIndex = LB_ERR;
   }
   else
   {
	   BOOL bOutside = FALSE;
	   int Index = ItemFromPoint(point,bOutside);
	   if (Index != LB_ERR && !bOutside && !m_bSelect)
	   {
		   SetListBoxSel(Index,FALSE);
	   }
   }


   //归位标志处理
   m_uState &= ~(TP_DRAG_DIALOG | TP_DRAG_TMIELINE | TP_DRAG_OUTSIDE);
   m_bButtondown = FALSE;
//   CTPListBox::OnLButtonUp(nFlags, point);
}


void CTPListBoxDrag::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bButtondown)
	{
		if (m_MovingIndex != LB_ERR && !m_Captured)
		{
			BOOL    bCtrlDown      = TP_IsCtrlKeyDown();
			BOOL    bShiftCtrl     = TP_IsShiftKeyDown(); 

			if(bCtrlDown && !bShiftCtrl && !m_bSelect &&   m_MovingIndex != LB_ERR)
				 SetListBoxSel(m_MovingIndex,TRUE);

			SetCapture();
			m_Captured = TRUE;
		}
		if(m_uState & TP_DRAG_TMIELINE)
		{
			GetParent() ->SendMessage(WM_LISTCTRL,(WPARAM)TP_WM_USER_MOUSEMOVE,(LPARAM)this);
			CRect rect;
			GetClientRect(&rect);

			if(!rect.PtInRect(point))
			{
				m_uState |= TP_DRAG_OUTSIDE;
			}

		}
		else if(m_uState & TP_DRAG_DIALOG)//本对话框内部拖拽
		{
			BOOL Outside;
			int Index = ItemFromPoint(point,Outside);
			if (Outside)
			{
				CRect ClientRect;
				GetClientRect(&ClientRect);

				if (ClientRect.PtInRect(point))
				{
					KillTimer(TID_SCROLLDOWN);
					KillTimer(TID_SCROLLUP);
					m_Interval = 0;
					Index = LB_ERR;
					Outside = FALSE;
				}
				else
				{
					DoTheScrolling(point,ClientRect);
				}
			}
			else
			{
				KillTimer(TID_SCROLLDOWN);
				KillTimer(TID_SCROLLUP);
				m_Interval = 0;
			}

			if (Index != m_MoveToIndex && !Outside)
			{
				DrawTheLines(Index);
			}
		}
	}
}

void CTPListBoxDrag::DrawTheLines(int Index)
{
   CRect ClientRect;
   GetClientRect(&ClientRect);

   CDC *pDC = GetDC();

   CRect Rect;
   int Width = 4;

   Invalidate(FALSE);
   UpdateWindow();//jyn added 20090608 不能删
   m_MoveToIndex = Index;
   //if (m_MoveToIndex != m_MovingIndex)
   {
      CPen Pen(PS_SOLID,Width,RGB(100,100,100));
      CPen *pOldPen = pDC->SelectObject(&Pen);
      if (m_MoveToIndex != LB_ERR)
      {
         GetItemRect(Index,&Rect);
         if (ClientRect.PtInRect(Rect.TopLeft()))
         {
            pDC->MoveTo(Rect.left,Rect.top+1);
            pDC->LineTo(Rect.right-(Width/2),Rect.top+1);
         }
      }
      else
      {
         GetItemRect(GetCount()-1,&Rect);
         if (ClientRect.PtInRect(CPoint(0,Rect.bottom)))
         {
            pDC->MoveTo(Rect.left,Rect.bottom);
            pDC->LineTo(Rect.right-(Width/2),Rect.bottom);
         }
      }
      pDC->SelectObject(pOldPen);
   }
   ReleaseDC(pDC);
}

void CTPListBoxDrag::InsertDraggedItem()
{
   CWnd* pParWnd = GetParent();
   

   CString Text;
   DWORD_PTR ItemData;
   GetText(m_MovingIndex,Text);
   ItemData = GetItemData(m_MovingIndex);

   if( m_MoveToIndex == -1)
	   m_MoveToIndex = (int)m_aItemData.GetCount();

   if (m_MovingIndex < m_MoveToIndex)
   {
	   SetRedraw(FALSE);
	   int TopIndex = GetTopIndex();

	   _tagItem *pItem = m_aItemData[m_MovingIndex];
	   m_aItemData[m_MovingIndex] = NULL;
	   DeleteString( m_MovingIndex);

	   int NewIndex = InsertString( m_MoveToIndex - 1, Text);
	   if(NewIndex !=m_MoveToIndex - 1)
	   delete m_aItemData[m_MoveToIndex - 1];
	   m_aItemData[m_MoveToIndex - 1] = pItem;

	   SetItemData( NewIndex, ItemData);
	   SetSel( NewIndex );		
	   SetTopIndex(TopIndex);
	   SetRedraw(TRUE);

	   Invalidate();
	   UpdateWindow();
	   GetParent() ->SendMessage( WM_DELBASEFRAME, MAKELONG( m_MovingIndex, NewIndex),(LPARAM)this);


   }
   else if (m_MovingIndex > m_MoveToIndex)
   {
      SetRedraw(FALSE);
      int TopIndex = GetTopIndex();

	  _tagItem *pItem = m_aItemData[m_MovingIndex];
	  m_aItemData[m_MovingIndex] = NULL;
      DeleteString(m_MovingIndex);

      int NewIndex = InsertString( m_MoveToIndex,Text);
	  delete m_aItemData[m_MoveToIndex];
	  m_aItemData[m_MoveToIndex] = pItem;

      SetItemData(NewIndex,ItemData);
      SetSel(NewIndex);
      SetTopIndex(TopIndex);
      SetRedraw(TRUE);

      Invalidate();
      UpdateWindow();
	  GetParent() ->SendMessage( WM_DELBASEFRAME, MAKELONG( m_MovingIndex, NewIndex),(LPARAM)this);
   }
   else 
   {
	   Invalidate();
   }
   GetParent()->SendMessage(WM_COMMAND,MAKEWPARAM(GetDlgCtrlID(),LBN_SELCHANGE),(LPARAM)m_hWnd);
}

void CTPListBoxDrag::DoTheScrolling(CPoint Point,CRect ClientRect)
{
	if(ClientRect.Height() > (GetItemHeight(0) * GetCount())) return;
   if (Point.y > ClientRect.Height())
   {
      DWORD Interval = 250 / (1+ ((Point.y-ClientRect.Height())/GetItemHeight(0)));
      if (Interval != m_Interval)
      {
         m_Interval = Interval;
         SetTimer(TID_SCROLLDOWN,Interval,NULL);
         OnTimer(TID_SCROLLDOWN);
      }
   }
   else if (Point.y < 0)
   {
      DWORD Interval = 250 / (1+(abs(Point.y)/GetItemHeight(1)));
      if (Interval != m_Interval)
      {
         m_Interval = Interval;
         SetTimer(TID_SCROLLUP,Interval,NULL);
         OnTimer(TID_SCROLLUP);
      }
   }
   else
   {
      KillTimer(TID_SCROLLDOWN);
      KillTimer(TID_SCROLLUP);
      m_Interval = 0;
   }
}

void CTPListBoxDrag::OnTimer(UINT nIDEvent)
{
   if (nIDEvent == TID_SCROLLDOWN)
   {
      SetTopIndex(GetTopIndex()+1);
	  Invalidate();
	  UpdateWindow();
   }
   else if (nIDEvent == TID_SCROLLUP)
   {
      SetTopIndex(GetTopIndex()-1);
	  Invalidate();
	  UpdateWindow();
   }

   CTPListBox::OnTimer(nIDEvent);
}

BOOL CTPListBoxDrag::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	Invalidate();
	return CTPListBox::OnMouseWheel(nFlags, zDelta, pt);
}

void CTPListBoxDrag::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if(nChar==VK_DELETE)
	{
		GetParent() ->SendMessage( WM_LISTCTRL, (WPARAM)TP_WM_USER_DELETE,(LPARAM)this);

	}
	//xl added 
	if (nChar == VK_UP)
	{
		if (m_iLastHitIndex != 0)
		{
			m_iLastHitIndex--;
			m_MovingIndex = m_iLastHitIndex;
			m_MoveToIndex = m_iLastHitIndex;
			SetListBoxSel(m_iLastHitIndex);
		}
	}
	if (nChar == VK_DOWN)
	{
		if (m_iLastHitIndex != GetCount()-1 )
		{
			m_iLastHitIndex++;
			m_MovingIndex = m_iLastHitIndex;
			m_MoveToIndex = m_iLastHitIndex;
			SetListBoxSel(m_iLastHitIndex);
		}
	}//xl added ok

	CTPListBox::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CTPListBoxDrag::DrawText(CDC *pDC,CRect &rtItem,CString &sText)
{
	CRect rtLeft,rtRight;
	rtLeft = rtItem;
	rtLeft.right = rtItem.left+(rtItem.Width()/2-1);
	rtRight = rtItem;
	rtRight.left = rtItem.right-(rtItem.Width()/2+1);

	CString strText;
	CString leftStr;
	CString rightStr;
	strText.Empty();
	leftStr.Empty();
	rightStr.Empty();

	strText.Format(_T("%s"),sText);

	int tablocr = strText.ReverseFind(_T('\t'));
	if(tablocr!=-1)
	{
		rightStr = strText.Mid(tablocr+1);
		leftStr = strText.Left(strText.Find(_T('\t')));
	}
	else
	{
		leftStr=strText;
	}

	//xl  add for  XN00027604
	CSize  size=pDC->GetTextExtent(leftStr);
	if (size.cx>rtLeft.Width())
	{
		size.cx = size.cx/2;
		rtLeft.right +=size.cx;
		rtRight.left-=size.cx;
	}
	//xl
	pDC ->DrawText(leftStr,rtLeft,DT_LEFT |DT_VCENTER|DT_WORD_ELLIPSIS);
	pDC ->DrawText(rightStr,rtRight,DT_RIGHT |DT_VCENTER|DT_WORD_ELLIPSIS);
}
void CTPListBoxDrag::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	SetCapture();
}

void CTPListBoxDrag::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture();
	GetParent() ->SendMessage( WM_LISTCTRL, (WPARAM)TP_WM_USER_MENU,(LPARAM)this);
}

BOOL CTPListBoxDrag::GetItemIsEnable(int iItem)
{
	if(iItem>=0 && iItem<m_aItemData.GetSize())
		return  m_aItemData[iItem]->bItemEnable;
	return FALSE;

}