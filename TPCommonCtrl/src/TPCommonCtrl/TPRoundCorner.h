#pragma once


#define TP_ROUNDCORNER_MAX 2



#define CORNER_LEFT    0x00000001 
#define CORNER_TOP     0x00000002 
#define CORNER_RIGHT   0x00000004 
#define CORNER_BOTTOM  0x00000008 

class AFX_EXT_CLASS CTPRoundCorner : public CTPBaseCtrl
{
public:
	CTPRoundCorner(void);
	~CTPRoundCorner(void);
public:
	
	TPCommonRes        *m_aBmpres[TP_ROUNDCORNER_MAX];  	 
	TPCommonRes        *m_aVideores[2][16];
	int                 m_iBackColor; 
public:
	void               PaintRect(CDC *pDC,CRect rtFrame);
	void               PaintLine(CDC *pDC,CRect rtRoundFrame,UINT nLine);
	void               SetState(UINT uState);	

	void               PaintRect(TP_BORDER_STYLE eBorderStyle,CWnd *pWnd,TPScrollBarData *pScrollData = NULL);
	void               PaintRect(TP_BORDER_STYLE eBorderStyle,CDC *pDC ,CRect rtRect,int iWidth =2);
	void               PaintRect(TP_BORDER_STYLE eBorderStyle,CDC *pDC ,CRect rtRect,int iWidth ,UINT uFlag);

	void               LoadRes();
	void               UnloadRes();

	void               LoadVideo();
	void               UnloadVideo();
	void               PaintVideo(int iIndex,CRect &rtRect,CDC *pDC);

	void               PaintRect(CImageList *pImageList,CDC *pDC, CRect rtCorner);
};
