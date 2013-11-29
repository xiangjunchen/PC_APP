// TPCommonCtrl.cpp : 定义 DLL 的初始化例程。
//
#include "stdafx.h"
#include "TPExtraCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


namespace _tp_extractrl
{
	CWnd    *TP_ExtraSubControl(CString cClassName,DWORD dwWindowLevel)
	{
		CWnd *pWndAdd = NULL;
		if(lstrcmpi(cClassName,WC_IMAGEBUTTON) == 0)
		{			
			CTPImageButton *pImageButton = new CTPImageButton();	
			pImageButton ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pImageButton;									
		}		
		else if(lstrcmpi(cClassName,WC_VUTABLE) == 0)
		{			
			CTPVUTableCtrl *pVUTableCtrl = new CTPVUTableCtrl();											
			pVUTableCtrl ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pVUTableCtrl;									
		}	
		else if(lstrcmpi(cClassName,WC_EDITEX) == 0)
		{			
			CTPEditEx *pEditEx = new CTPEditEx();											
			pEditEx ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pEditEx;									
		}	
		else if(lstrcmpi(cClassName,WC_VUSTATIC) == 0)
		{			
			CTPVUStatic *pVUStatic = new CTPVUStatic();											
			pVUStatic ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pVUStatic;									
		}	
		else if(lstrcmpi(cClassName,WC_TPNUMBEREDIT) == 0)
		{			
			CTPNumberEditExEx *pNumberEdit = new CTPNumberEditExEx();											
			pWndAdd = pNumberEdit;									
		}	
		else if(lstrcmpi(cClassName,WC_TPTIMECODEEDITEX) == 0)
		{			
			CTPTimeCodeEditEx *pTimeCodeEdit = new CTPTimeCodeEditEx();											
			pWndAdd = pTimeCodeEdit;									
		}	
		else if(lstrcmpi(cClassName,WC_TPTIMECODEEDITNS) == 0)
		{			
			CTPTimeCodeEditNS *pTimeCodeEdit = new CTPTimeCodeEditNS();					
			pWndAdd = pTimeCodeEdit;									
		}	
		else if(lstrcmpi(cClassName,WC_TPTIMECODESTATIC) == 0)
		{			
			CTPTimeCodeStaticEx *pTimeCodeStatic = new CTPTimeCodeStaticEx();											
			pWndAdd = pTimeCodeStatic;									
		}
		else if(lstrcmpi(cClassName,WC_PANEWND) == 0)
		{			
			CTPPaneWnd *pPaneWnd = new CTPPaneWnd();	
			pPaneWnd ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pPaneWnd;									
		}
		else if(lstrcmpi(cClassName,WC_SPLITTERBAR) == 0)
		{			
			CTPSplitterBar *pSplitterBar = new CTPSplitterBar();											
			pSplitterBar ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pSplitterBar;									
		}
		else if(lstrcmpi(cClassName,WC_SLIDERROUNDCTRL) == 0)
		{			
			CTPSliderRoundCtrl *pSliderRoundCtrl = new CTPSliderRoundCtrl();											
			pSliderRoundCtrl ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pSliderRoundCtrl;											
		}
		else if(lstrcmpi(cClassName,WC_VUSLIDERCTRL) == 0)
		{			
			CTPVUSliderCtrl *pVUSliderCtrl = new CTPVUSliderCtrl();											
			pVUSliderCtrl ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pVUSliderCtrl;											
		}
		else if(lstrcmpi(cClassName,WC_VUSLIDERCTRLEX) == 0)
		{			
			CTPVUSliderCtrlEx *pVUSliderCtrl = new CTPVUSliderCtrlEx();											
			pVUSliderCtrl ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pVUSliderCtrl;											
		}
		else if(lstrcmpi(cClassName,WC_CLASSCASECTRL) == 0)
		{			
			CTPClassCaseCtrl *pClassCaseCtrl = new CTPClassCaseCtrl();											
			pClassCaseCtrl ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pClassCaseCtrl;											
		}
		else if(lstrcmpi(cClassName,WC_BTNTOOLBAR) == 0)
		{			
			CTPBtnToolBar *pBtnToolBar = new CTPBtnToolBar();											
			pBtnToolBar ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pBtnToolBar;											
		}
		else if(lstrcmpi(cClassName,WC_TREECTRLEX) == 0)
		{			
			CTPTreeCtrlEx *pTreeCtrlEx = new CTPTreeCtrlEx();											
			pTreeCtrlEx ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pTreeCtrlEx;											
		}
		else if(lstrcmpi(cClassName,WC_LISTCTRLEX) == 0)
		{			
			CTPListCtrlEx *pListCtrlEx = new CTPListCtrlEx();											
			pListCtrlEx ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pListCtrlEx;											
		}
		else if(lstrcmpi(cClassName,WC_SLIDERCTRLEXEX) == 0)
		{			
			CTPSliderCtrlExEx *pListCtrlEx = new CTPSliderCtrlExEx();											
			pListCtrlEx ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pListCtrlEx;											
		}
		else if(lstrcmpi(cClassName,WC_LISTPROPERTY) == 0)
		{			
			CTPListPropertyCtrl *pListProperty = new CTPListPropertyCtrl();											
			pListProperty ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pListProperty;											
		}
		else if(lstrcmpi(cClassName,WC_ICONLISTCTRLEX) == 0)
		{			
			CTPIconListCtrl *pIconListCtrl = new CTPIconListCtrl();											
			pIconListCtrl ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pIconListCtrl;											
		}		
		else if(lstrcmpi(cClassName,WC_TIMELINE) == 0)
		{			
			CTPTimeLine *pTimeLine = new CTPTimeLine();	
			pTimeLine ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pTimeLine;											
		}
		else if(lstrcmpi(cClassName,WC_RIECHEDITEX) == 0)
		{			
			CTPRichEditCtrlEx *pRichEditEx = new CTPRichEditCtrlEx();	
			pRichEditEx ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pRichEditEx;											
		}
		else if(lstrcmpi(cClassName,WC_SCROLLBARZOOM) == 0)
		{			
			CTPScrollBarZoom *pTimeLine = new CTPScrollBarZoom();	
			pTimeLine ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pTimeLine;											
		}
		else if(lstrcmpi(cClassName,WC_TABCTRLEX) == 0)
		{		
			CTPTabCtrlEx *pTabCtrlEx = new CTPTabCtrlEx();											
			pTabCtrlEx ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pTabCtrlEx;											
		}
		else if(lstrcmpi(cClassName,WC_LINECTRL) == 0)
		{		
			CTPLineCtrl *pLineCtrl = new CTPLineCtrl();											
			pLineCtrl ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pLineCtrl;											
		}
		else if(lstrcmpi(cClassName,WC_TOOLCASE) == 0)
		{		
			CTPToolCase *pToolCase = new CTPToolCase();											
			pToolCase ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pToolCase;											
		}
		else if(lstrcmpi(cClassName,WC_EDITLISTCTRL) == 0)
		{		
			CTPEditListCtrl *pEditListCtrl = new CTPEditListCtrl();											
			pEditListCtrl ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pEditListCtrl;											
		}
		else if(lstrcmpi(cClassName,WC_TPRECTTRACKER) == 0)
		{		
			CTPRectTracker *pRectTracker = new CTPRectTracker();											
			pRectTracker ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pRectTracker;											
		}
		else if(lstrcmpi(cClassName,WC_COMBOMENU) == 0)
		{		
			CTPComboMenu *pComboMenu = new CTPComboMenu();											
			pComboMenu ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pComboMenu;											
		}
		
		return pWndAdd;
	}
	CWnd    *TP_ExtraCreateControl(CWnd *pParentWnd,TPControlData *pCtrlData,DWORD dwWindowLevel)
	{
		CString cClassName = pCtrlData ->sClassType;
		CWnd *pWndAdd = NULL;
		if(lstrcmpi(cClassName,WC_IMAGEBUTTON) == 0)
		{			
			CTPImageButton *pImageButton = new CTPImageButton();	
			pImageButton ->m_dwWindowLevel = dwWindowLevel;
			pImageButton ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pImageButton;									
		}		
		else if(lstrcmpi(cClassName,WC_VUTABLE) == 0)
		{			
			CTPVUTableCtrl *pVUTableCtrl = new CTPVUTableCtrl();											
			pVUTableCtrl ->m_dwWindowLevel = dwWindowLevel;
			pVUTableCtrl ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pVUTableCtrl;									
		}	
		else if(lstrcmpi(cClassName,WC_VUSTATIC) == 0)
		{			
			CTPVUStatic *pVUStatic = new CTPVUStatic();											
			pVUStatic ->m_dwWindowLevel = dwWindowLevel;
			pVUStatic ->Create(_L(""),pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pVUStatic;									
		}	
		else if(lstrcmpi(cClassName,WC_EDITEX) == 0)
		{			
			CTPEditEx *pEditEx = new CTPEditEx();											
			pEditEx ->m_dwWindowLevel = dwWindowLevel;
			pEditEx ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pEditEx;									
		}	
		else if(lstrcmpi(cClassName,WC_TPNUMBEREDIT) == 0)
		{			
			CTPNumberEditExEx *pNumberEdit = new CTPNumberEditExEx();				
			pNumberEdit ->m_dwWindowLevel = dwWindowLevel;
			pNumberEdit ->Create(pCtrlData ->dwStyle, pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pNumberEdit;									
		}	
		else if(lstrcmpi(cClassName,WC_TPTIMECODEEDITEX) == 0)
		{			
			CTPTimeCodeEditEx *pTimeCodeEdit = new CTPTimeCodeEditEx();		
			pTimeCodeEdit ->m_dwWindowLevel = dwWindowLevel;
			pTimeCodeEdit ->Create(pCtrlData ->dwStyle, pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pTimeCodeEdit;									
		}	
		else if(lstrcmpi(cClassName,WC_TPTIMECODEEDITNS) == 0)
		{			
			CTPTimeCodeEditNS *pTimeCodeEdit = new CTPTimeCodeEditNS();		
			pTimeCodeEdit ->m_dwWindowLevel = dwWindowLevel;
			pTimeCodeEdit ->Create(pCtrlData ->dwStyle, pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pTimeCodeEdit;									
		}	
		else if(lstrcmpi(cClassName,WC_TPTIMECODESTATIC) == 0)
		{			
			CTPTimeCodeStaticEx *pTimeCodeStatic = new CTPTimeCodeStaticEx();											
			pTimeCodeStatic ->m_dwWindowLevel = dwWindowLevel;
			pTimeCodeStatic ->Create(pCtrlData ->dwStyle, pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pTimeCodeStatic;									
		}
		else if(lstrcmpi(cClassName,WC_PANEWND) == 0)
		{			
			CTPPaneWnd *pPaneWnd = new CTPPaneWnd();	
			pPaneWnd ->m_dwWindowLevel = dwWindowLevel;
			pPaneWnd ->Create(pParentWnd,pCtrlData ->dwStyle,pCtrlData->nID);
			pWndAdd = pPaneWnd;									
		}
		else if(lstrcmpi(cClassName,WC_SPLITTERBAR) == 0)
		{			
			CTPSplitterBar *pSplitterBar = new CTPSplitterBar();											
			pSplitterBar ->m_dwWindowLevel = dwWindowLevel;
			pSplitterBar ->Create(pCtrlData ->dwStyle&SB_HORZ,pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pSplitterBar;									
		}
		else if(lstrcmpi(cClassName,WC_SLIDERROUNDCTRL) == 0)
		{			
			CTPSliderRoundCtrl *pSliderRoundCtrl = new CTPSliderRoundCtrl();											
			pSliderRoundCtrl ->m_dwWindowLevel = dwWindowLevel;
			pSliderRoundCtrl ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pSliderRoundCtrl;											
		}
		else if(lstrcmpi(cClassName,WC_VUSLIDERCTRL) == 0)
		{			
			CTPVUSliderCtrl *pVUSliderCtrl = new CTPVUSliderCtrl();											
			pVUSliderCtrl ->m_dwWindowLevel = dwWindowLevel;
			pVUSliderCtrl ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pVUSliderCtrl;											
		}
		else if(lstrcmpi(cClassName,WC_VUSLIDERCTRLEX) == 0)
		{			
			CTPVUSliderCtrlEx *pVUSliderCtrl = new CTPVUSliderCtrlEx();											
			pVUSliderCtrl ->m_dwWindowLevel = dwWindowLevel;
			pVUSliderCtrl ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pVUSliderCtrl;											
		}
		else if(lstrcmpi(cClassName,WC_CLASSCASECTRL) == 0)
		{			
			CTPClassCaseCtrl *pClassCaseCtrl = new CTPClassCaseCtrl();											
			pClassCaseCtrl ->m_dwWindowLevel = dwWindowLevel;
			pClassCaseCtrl ->Create(NULL,NULL,pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pClassCaseCtrl;											
		}
		else if(lstrcmpi(cClassName,WC_BTNTOOLBAR) == 0)
		{			
			CTPBtnToolBar *pBtnToolBar = new CTPBtnToolBar();											
			pBtnToolBar ->m_dwWindowLevel = dwWindowLevel;
			pBtnToolBar ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pBtnToolBar;											
		}
		else if(lstrcmpi(cClassName,WC_TREECTRLEX) == 0)
		{			
			CTPTreeCtrlEx *pTreeCtrlEx = new CTPTreeCtrlEx();											
			pTreeCtrlEx ->m_dwWindowLevel = dwWindowLevel;
			pTreeCtrlEx ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pTreeCtrlEx;											
		}
		else if(lstrcmpi(cClassName,WC_LISTCTRLEX) == 0)
		{			
			CTPListCtrlEx *pListCtrlEx = new CTPListCtrlEx();											
			pListCtrlEx ->m_dwWindowLevel = dwWindowLevel;
			pListCtrlEx ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pListCtrlEx;											
		}
		else if(lstrcmpi(cClassName,WC_SLIDERCTRLEXEX) == 0)
		{			
			CTPSliderCtrlExEx *pListCtrlEx = new CTPSliderCtrlExEx();											
			pListCtrlEx ->m_dwWindowLevel = dwWindowLevel;
			pListCtrlEx ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pListCtrlEx;	 							
		}
		else if(lstrcmpi(cClassName,WC_LISTPROPERTY) == 0)
		{			
			CTPListPropertyCtrl *pListProperty = new CTPListPropertyCtrl();											
			pListProperty ->m_dwWindowLevel = dwWindowLevel;
			pListProperty ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pListProperty;											
		}
		else if(lstrcmpi(cClassName,WC_ICONLISTCTRLEX) == 0)
		{			
			CTPIconListCtrl *pIconListCtrl = new CTPIconListCtrl();											
			pIconListCtrl ->m_dwWindowLevel = dwWindowLevel;
			pIconListCtrl ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pIconListCtrl;									
		}		

		else if(lstrcmpi(cClassName,WC_TIMELINE) == 0)
		{			
			CTPTimeLine *pTimeLine = new CTPTimeLine();	
			pTimeLine ->m_dwWindowLevel = dwWindowLevel;
			pTimeLine ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pTimeLine;											
		}
		else if(lstrcmpi(cClassName,WC_RIECHEDITEX) == 0)
		{			
			CTPRichEditCtrlEx *pRichEditEx = new CTPRichEditCtrlEx();	
			pRichEditEx ->m_dwWindowLevel = dwWindowLevel;
			pRichEditEx ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pRichEditEx;											
		}
		else if(lstrcmpi(cClassName,WC_SCROLLBARZOOM) == 0)
		{			
			CTPScrollBarZoom *pTimeLine = new CTPScrollBarZoom();	
			pTimeLine ->m_dwWindowLevel = dwWindowLevel;
			pTimeLine ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pTimeLine;											
		}
		else if(lstrcmpi(cClassName,WC_TABCTRLEX) == 0)
		{			
			CTPTabCtrlEx *pTabCtrlEx = new CTPTabCtrlEx();											
			pTabCtrlEx ->m_dwWindowLevel = dwWindowLevel;
			pTabCtrlEx ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pTabCtrlEx;											
		}

		else if(lstrcmpi(cClassName,WC_LINECTRL) == 0)
		{		
			CTPLineCtrl *pLineCtrl = new CTPLineCtrl();											
			pLineCtrl ->m_dwWindowLevel = dwWindowLevel;
			pLineCtrl ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pLineCtrl;											
		}
		else if(lstrcmpi(cClassName,WC_TOOLCASE) == 0)
		{		
			CTPToolCase *pToolCase = new CTPToolCase();											
			pToolCase ->m_dwWindowLevel = dwWindowLevel;
			pToolCase ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pToolCase;											
		}
		else if(lstrcmpi(cClassName,WC_EDITLISTCTRL) == 0)
		{		
			CTPEditListCtrl *pEditListCtrl = new CTPEditListCtrl();											
			pEditListCtrl ->m_dwWindowLevel = dwWindowLevel;
			pEditListCtrl ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pEditListCtrl;											
		}
		else if(lstrcmpi(cClassName,WC_TPRECTTRACKER) == 0)
		{		
			CTPRectTracker *pRectTracker = new CTPRectTracker();											
			pRectTracker ->m_dwWindowLevel = dwWindowLevel;
			pRectTracker ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pRectTracker;											
		}
		else if(lstrcmpi(cClassName,WC_COMBOMENU) == 0)
		{		
			CTPComboMenu *pComboMenu = new CTPComboMenu();											
			pComboMenu ->m_dwWindowLevel = dwWindowLevel;
			pComboMenu ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
			pWndAdd = pComboMenu;											
		}
		
		return pWndAdd;
	}

};

