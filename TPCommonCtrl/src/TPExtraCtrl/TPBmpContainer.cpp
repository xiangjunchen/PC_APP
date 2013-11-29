/*
 *	�ļ�����BmpContainer.cpp
 *	���ã�λͼʰȡ�����������
 */

// BmpContainer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TPBmpContainer.h"
#include "TPBmpPicker.h"
#include "math.h"
// CBmpContainer
#define IDC_INNERBUTTON 10000
#define IDC_ICONCONTAINER 10001
IMPLEMENT_DYNAMIC(CBmpContainer, CWnd)

CBmpContainer::CBmpContainer()
{
	m_nRow=0;
	m_nCol=0;
	m_bClosed=FALSE;
}

CBmpContainer::~CBmpContainer()
{
	for(int i=0;i<m_InnerButtonArray.GetSize();i++)
	{
		CInnerButton* pInnerButton;
		pInnerButton=m_InnerButtonArray.GetAt(i);
		pInnerButton->SendMessage(WM_CLOSE,0,0);
		delete pInnerButton;
		pInnerButton = NULL;
	}
}


BEGIN_MESSAGE_MAP(CBmpContainer, CWnd)
	ON_WM_KILLFOCUS()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CBmpContainer ��Ϣ�������



BOOL CBmpContainer::Create(POINT pt,CButton* pParentButton,CArray<CBitmap*,CBitmap*> *pBitmapArray) 
{
	if(pBitmapArray->GetSize()<=0)return FALSE;

	m_pParentButton=pParentButton;

	///����ÿ��ͼƬ�Ĵ�С����IconContainer
	//m_nCol=int(sqrt((double)pBitmapArray->GetSize()))+1;    //��������
	m_nCol=10;
	BITMAP bm;
	pBitmapArray->GetAt(0)->GetBitmap(&bm);			//��ͼƬ�б��еĵ�0��ͼƬ�Ĵ�СΪ��׼
	m_nCellWidth=bm.bmWidth+4;						//�ڲ���Ԫ�Ŀ��
	m_nCellHeight=bm.bmHeight+4;					//�ڲ���Ԫ�ĸ߶�

	CRect rect;
	rect.left=pt.x,rect.top=pt.y;
	rect.right=pt.x+m_nCellWidth*m_nCol;			//�����Ŀ��

	if(pBitmapArray->GetSize()%m_nCol==0)			//��������
	{
		m_nRow=pBitmapArray->GetSize()/m_nCol;
	}
	else
	{
		m_nRow=pBitmapArray->GetSize()/m_nCol+1;
	}
	//rect.bottom=pt.y+m_nCellHeight*m_nRow+2+46;		///�����ĸ߶�=(����+2)*��Ԫ���,Ҫ��������չ���ܰ�ť�Ļ�������һ��
	rect.bottom=pt.y+m_nCellHeight*m_nRow+2;
	rect.top=pt.y-m_nCellHeight*m_nRow-2;
	rect.bottom=pt.y;
	//pParentButton->GetParent()->ScreenToClient(&rect);

	///��������
	//CWnd::Create(NULL, NULL, WS_VISIBLE | WS_CHILD,
	//rect,pParentButton->GetParent(),IDC_ICONCONTAINER, NULL);
	CWnd::CreateEx(WS_EX_LEFT| WS_EX_TOOLWINDOW,AfxRegisterWndClass(0),NULL,WS_VISIBLE|WS_POPUP,rect,NULL,NULL );
	///����ͼƬ����+2����ť
	for(int i=0;i<m_nRow;i++)
	{
		for(int j=0;j<m_nCol&& i*m_nCol+j<pBitmapArray->GetSize();j++)
		{
			///���㰴ť��λ��
			CRect innerrect;
			innerrect.left=j*m_nCellWidth;
			innerrect.top=i*m_nCellHeight;
			innerrect.right=innerrect.left+m_nCellWidth;
			innerrect.bottom=innerrect.top+m_nCellHeight;

			innerrect.DeflateRect(2,2);

			///�½���ť
			CInnerButton *pInnerButton;
			pInnerButton=new CInnerButton;
			pInnerButton->Create(NULL,WS_CHILD |WS_VISIBLE,
				innerrect,this,IDC_INNERBUTTON+i*m_nCol+j);

			///���ð�ť��ͼ��
			pInnerButton->SetBitmap(pBitmapArray->GetAt(i*m_nCol+j));
			pInnerButton->ShowWindow(SW_SHOW);

			///��¼�ð�ť��ָ��
			m_InnerButtonArray.Add(pInnerButton);
		}
	}


	////����������չ���ܰ�ť
	/*
	this->GetClientRect(&rect);
	CInnerButton *pInnerButton;
	pInnerButton=new CInnerButton;

	rect.left=2;
	rect.right-=2;
	rect.top=m_nRow*m_nCellHeight+3;
	rect.bottom=rect.top+20;  //��ť�ĸ߶�Ϊ22


	pInnerButton->Create(NULL,WS_CHILD |WS_VISIBLE,
		rect,this,IDC_INNERBUTTON-1);
	pInnerButton->SetFont(m_pParentButton->GetParent()->GetFont());
	pInnerButton->SetWindowText(_T("��ʾ�����ͼ��"));

	pInnerButton=new CInnerButton;

	rect.top=rect.bottom+3;
	rect.bottom=rect.top+20;
	pInnerButton->Create(NULL,WS_CHILD |WS_VISIBLE,
		rect,this,IDC_INNERBUTTON-2);
	pInnerButton->SetWindowText(_T("�ҵ��Զ���ͼ��"));
	pInnerButton->SetFont(m_pParentButton->GetParent()->GetFont());
	*/
	return TRUE;
}

void CBmpContainer::OnKillFocus(CWnd* pNewWnd)
{
	POINT pt;
	::GetCursorPos(&pt);	
	if(::IsChild(this->m_hWnd,::WindowFromPoint(pt)))
	{
		///��ʱ������СͼƬ����,���Բ��رձ�����
		return;
	}
	///�û�û��ѡ���κ�ͼƬ�������������ط�
	Close();
}

void CBmpContainer::OnPaint()
{
	CPaintDC dc(this); 

	///׼������
	RECT rect;
	this->GetClientRect(&rect);
	CPen BorderPen,*pOldPen,LinePen,VPen;
	LinePen.CreatePen(PS_SOLID,1,RGB(222,222,222));
	BorderPen.CreatePen(PS_SOLID,1,RGB(79,107,160));
	VPen.CreatePen(PS_SOLID,1,RGB(165,182,222));

	///����ɫ
	pOldPen=dc.SelectObject(&BorderPen);

	dc.Rectangle(&rect);

	dc.SelectObject(pOldPen);
	///������
	/**/
	dc.SelectObject(&LinePen);
	for(int i=1;i<m_nRow;i++)
	{
		dc.MoveTo(2,i*m_nCellHeight);
		dc.LineTo(rect.right-2,i*m_nCellHeight);
	}
	///��������չ���ܺ���
	/*
	dc.MoveTo(2,m_nRow*m_nCellHeight);
	dc.LineTo(rect.right-2,m_nRow*m_nCellHeight);
	dc.MoveTo(2,m_nRow*m_nCellHeight+24);
	dc.LineTo(rect.right-2,m_nRow*m_nCellHeight+24);
	*/
	//������
	dc.SelectObject(&VPen);
	for(int j=1;j<m_nCol;j++)
	{
		dc.MoveTo(j*m_nCellWidth,2);
		dc.LineTo(j*m_nCellWidth,m_nRow*m_nCellHeight-2);
	}
}

////���û�ѡ���ͼƬ��ŷ��͸�������CIconPicker,���ر��Լ�
void CBmpContainer::Close(int index)
{
	this->ShowWindow(SW_HIDE);

	RECT rect;
	this->GetWindowRect(&rect);
	m_pParentButton->GetParent()->ScreenToClient(&rect);
	m_pParentButton->GetParent()->InvalidateRect(&rect);

	::SendMessage(m_pParentButton->GetSafeHwnd(),WM_USER+101,index,0);
	//this->SendMessage(WM_CLOSE,0,0);

}

BOOL CBmpContainer::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if(LOWORD(wParam)-IDC_INNERBUTTON==-1)
	{
		////��������Ӧ"��ʾ�����ͼ��"
	}
	if(LOWORD(wParam)-IDC_INNERBUTTON==-2)
	{
		////��������Ӧ"�ҵ��Զ���ͼ��"
		CFileDialog SelectFileDlg(TRUE,_T("bmp"),_T("noname"),OFN_FILEMUSTEXIST,
			_T("Bitmap File(*.bmp)|*.bmp"),m_pParentButton);
		if(SelectFileDlg.DoModal()==IDOK)
		{
			((CBmpPicker*)m_pParentButton)->AddBitmap(SelectFileDlg.GetPathName());
		}
	}
	///�رձ�����
	Close(LOWORD(wParam)-IDC_INNERBUTTON);
	return TRUE;
}
