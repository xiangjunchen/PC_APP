#pragma once




#define WM_TIMECODE_CHANGE  WM_USER +1324//ʱ�뷢�͸��������Ϣ
#define WM_TIMECODE_THIS    WM_USER +1325


#define TP_TIMECODE_CHANGE     0x00000001//ʱ��ı�
#define TP_TIMECODE_EDITEND    0x00000002//
#define TP_TIMECODE_RETURN     0x00000004//�س�
#define TP_TIMECODE_KILLFOCUS  0x00000008//ʧȥ����
#define TP_TIMECODE_ESC        0x00000010//esc   
#define TP_TIMECODE_RESETCTL   0x00000020//ѡ��ctlreset ѡ�� 
#define TP_TIMECODE_FRAMEVIEW  0x00000040//����Ϊ֡��ʾ
#define TP_TIMECODE_FTCVIEW    0x00000080//
#define TP_TIMECODE_LTCVIEW    0x00000100//
#define TP_TIMECODE_VITCVIEW   0x00000200//
#define TP_TIMECODE_CTLVIEW    0x00000400//
#define TP_TIMECODE_TCVIEW     0x00000800//
#define TP_TIMECODE_EDITCHANGE 0x00001000//ʱ��ı�


#define WC_TPTIMECODEEDITEX _L("TPTimeCodeEditExClass")

#define TP_TIMECODE_NEGATIVE         0x00000001//�Ƿ���ʾ����(���Ƽ�ʹ�� ,�������������ŵĹ���)
#define TP_TIMECODE_POSITIVE_TEXT    0x00000002 //������ʾ����ʱ,SetFrame()�����Ĵ���� 
#define TP_TIMECODE_POSITIVE_SIGN    0x00000004//����ģʽΪ��!TP_TIMECODE_NEGATIVE��ʱ�����ŵ���ʾ
#define TP_TIMECODE_NEGATIVE_POS     0x00000008//���ŵ���ʾλ��
#define TP_TIMECODE_30M_DURATION     0x00000010//����Duration
#define TP_TIMECODE_WANTRETURN       0x00000020//�����س�����Ӧ����,�Ƿ��л�����һ��ʱ��ؼ�
#define TP_TIMECODE_DRAG             0x00000040//�϶�ʱ�ú�(�ڲ�ʹ��)
#define TP_TIMECODE_LOGO             0x00000080//logo��־��
#define TP_TIMECODE_MENU             0x00000100//�˵���־��
#define TP_TIMECODE_SHOWNEG          0x00000200//���ŵ���ʾ��ʽ
#define TP_TIMECODE_OldNew           0x00000400//ð�ŵ���ʾ��ʽ�� .(�ɲ���)
#define TP_TIMECODE_POPINPUT         0x00000800//sv,mvʱ���־��
#define TP_TIMECODE_DISABLE_ZERO_NEG 0x00001000 //����ʾΪ0ʱ,����ʾ����
#define TP_TIMECODE_NSCTLDISABLE     0x00002000 //����CTL_RESET
#define TP_TIMECODE_CHANGEOPEN       0x00004000//�������ŵı�.
#define TP_TIMECODE_TOOLTIPS         0x00008000//�Ƿ�ʹ��tooltip�ĺ�
#define TP_TIMECODE_DISABLE          0x00010000//disable timecode //�����κ���Ϣ������Ӧ.
#define TP_TIMECODE_DISPLAYNEG       0x00020000//����������ʾ���ŵĺ�

#define TP_TIMECODE_NSLTCTYPE        0x00040000//Ctlģʽ
#define TP_TIMECODE_NSVITCTYPE       0x00080000//Ctlģʽ
#define TP_TIMECODE_NSFTCTYPE        0x00100000//Ctlģʽ
#define TP_TIMECODE_NSCTLTYPE        0x00200000//Ctlģʽ
#define TP_TIMECODE_NSTCTYPE         0x00400000//Ctlģʽ

#define TP_TIMECODE_EDITTYPEFFECT    0x00800000//���������Ĳ˵�����
#define TP_TIMECODE_EDITTYPESV       0x01000000//���������Ĳ˵�����
#define TP_TIMECODE_EDITTYPEMV       0x02000000//���������Ĳ˵�����
#define TP_TIMECODE_EDITTYPEREC      0x04000000//���������Ĳ˵�����
#define TP_TIMECODE_EDITTYPEDIGITIZE 0x08000000//���������Ĳ˵�����
#define TP_TIMECODE_EDITTYPTIMECODE  0x10000000//���������Ĳ˵�����
#define TP_TIMECODE_DISABLECOLOR     0x20000000//���������Ĳ˵�����



#define TP_TIMECODE_NSALLTYPE      (TP_TIMECODE_NSFTCTYPE|TP_TIMECODE_NSVITCTYPE|TP_TIMECODE_NSLTCTYPE|TP_TIMECODE_NSCTLTYPE|TP_TIMECODE_NSTCTYPE) 
#define TP_TIMECODE_NSEDITTYPE     (TP_TIMECODE_EDITTYPESV|TP_TIMECODE_EDITTYPEMV|TP_TIMECODE_EDITTYPEREC|TP_TIMECODE_EDITTYPEDIGITIZE|TP_TIMECODE_EDITTYPTIMECODE) 

#define TP_TIMECODE_SHOW_MID         0x40000000//����ʱ����ʾ���м�
#define TP_TIMECODE_SHOW_ONLY        0x80000000// ����ʱ��Ϊֻ��ģʽ
#define TP_TIMECODE_MENU_READ       0x100000000// �ı�ʱ��ֵ�Ĳ˵���disable

#define TP_TIMECODE_EDITTYPESPEED   0x200000000   //xl added 

//StyleEx����
#define TP_TIMECODE_STYLE_EX_CALCULATE		0x00000001//������"+/-"ģʽ�� �س� Ӧ�Լ������"+/-"��Ľ������ʾ��
#define TP_TIMECODE_STYLE_EX_FRAMETOTC		0x00000002//����֡��ʾģʽ�� �س� �Զ�ת��Ϊ��Ӧ��ʱ��ģʽ����ʾ��



#define TP_TIMECODE_LOGO_WIDTH       20    
class AFX_EXT_CLASS CTPTimeCodeEditEx : public CTPWnd,public CTPBaseCtrl
{
	DECLARE_DYNAMIC(CTPTimeCodeEditEx)

public:
	CTPTimeCodeEditEx();
	virtual ~CTPTimeCodeEditEx();
public:
	VIDEO_TYPE              m_eVideoType;    //ʱ����ʽ
	__int64                 m_uEditStyle;    //ʱ��ı༭ģʽ
	UINT                    m_uEditEnable;   //ʱ�����Чģʽ
	BOOL                    m_bSubWindow;    //ϵͳ
	BOOL                    m_bCreate; 
	BOOL					m_bBeep;
	TP_BORDER_STYLE         m_eBorderStyle;  //�߿�ģʽ
	int                     m_iBackColor;    //����ɫ
	int                     m_iTextColor;    //�ı���ɫ
	__int64                 m_iabsFrame;     //��Զ������ʱ��ľ���ֵ,���۷���,CTL/TLģʽ֮��.
	__int64                 m_lMin;          //ʱ����ʾ��Сֵ
	__int64                 m_lMax; 	     //ʱ����ʾ���ֵ
	CFont                  *m_pFont;         
	CFont                  *m_pFontNeg;      //�����ŵĻ���,�������������ؼ�����.
	BOOL                    m_bTimeView;	 //�Ƿ���ʾ����ʱ����ͼ
	CString                 m_strInput;      //��������
	__int64                 m_iFrameInPut;
	__int64                 m_iUnDoFrame;
	static __int64          m_iCopyFrame;
	INT64                   m_iCTLFrame;	 //��׼֡
	INT64                   m_iDragFrame;	 //������ק�ı��� 

	CRect                   m_rtText;        //��ʾλ��
	CRect                   m_rtNegative;
	CRect                   m_rtFocus;
	CRect                   m_rtSelText;     //������ʾʱ�����Ӱ

	int                     m_iFocusPos;     //��ʾλ��
	int                     m_iFocusUndoPos;
	__int64                 m_iSetFocusFrame;
	int                     m_iBeginSel;   
	int                     m_iEndSel;          //��ʷ����,ûʲô����
	BOOL                    m_bFirstBegin;	    //��ʷ����,ûʲô����.
	BOOL                    m_bInvalid;         //GetFrame����������ж�����ʱ��ֵ�Ƿ���Ч:��Ч TRUE,��Ч FALSE.(���ֲ��������...)
	__int64                 m_iDrawNegOffetVal;//������Ż���λ�ò���׼,����ʹ��������� (���Ƽ�...)
	__int64                 m_iDrawTimeCodeOffsetVal ;//�������ʱ��λ��Ҫ��ƫ��,��ʹ���������.
	CWnd                   *m_pMsgWnd;
	UINT                    m_uPreEditStyle;		// FTC,VITC,LTC
	                                                 
public:
                    
	BOOL                    m_boffsetvalueshow;//�ж��Ƿ���ʾƫ��ֵ 
	__int64                 m_offsetView;   //�洢��ƫ��ֵ 
	BOOL                    m_bNegative;	  //��ǰ�Ƿ�Ϊ��
	BOOL                    m_bLable;         //�Ƿ���ʾ + -
	int                     m_iEditMode;      //�༭ģʽ
	BOOL                    m_bDrawFocus;     //�Ƿ���ʾ����
	BOOL                    m_bSkipTimer;
	CPoint                  m_ptSave;       
	BOOL                    m_bInsert;       //���븲��  //��ʱ��Ҫ����
	int                     m_lMaxLength;    //����ı�����
	BOOL                    m_bFocus;        //�Ƿ��н���
	UINT                    m_uState;        //״̬
	BOOL                    m_bViewExchange;//�� *���Ƿ��л���ʾ��ʽ
	UINT                    m_uShowStyle;//ѡ�����ֺ���Ӱ�ñ�־λ
	                                     //0:��ʾû��ѡ�� ������ʾ��Ӱ
                                         //1:��ʾѡ�����壬��ʾȫ����Ӱ
	                                     //2:��ʾѡ�в��֣�2������λ������ʾ������Ӱ
	BOOL                    m_b2BitFlag;//2λ����ʱ�ı�־λ
	BOOL                    m_boffSet;//�Ƿ���ƫ��ֵ
	BOOL                    m_boffsetNeg;//���渺���ķ���λ.
	int                     m_shadow;
	CTPToolTipCtrl          *m_pShowTips;//δʹ��
	CString                 m_sToolTips; //ToolTip��ʾ���ַ���
	BOOL                    m_bCtlBtn;//ģ��ctrl���ı�־.���ڸ���,ճ���Ŀ�ݼ���.  

	BOOL                    m_bSelCalc;//��Ӧ��+/-�����Լ�������
	BOOL					m_bFrameToTC;//֡ģʽ�»س� �Ƿ�ת��Ϊʱ�롣

	__int64                 m_iLastInput;//�洢���롰+/-��֮ǰ��������ֵ�����ڼ��㡣

protected:
	int                     m_iBaseTextWidth;
	BOOL                    m_bSendChangeMsg;

	//{ 4.6Feature - 1269. Identification for Ingesting Add by CZ.Lee : CD2009.10.01
#ifdef TP_NS_NET
	BOOL					m_bNotifyMode; // ����ģʽ
	int						m_iBackColor_Notify;
	int						m_iTextColor_Notify;
#endif
	//}

public:
	void            SetToolTips(CString sText);
    long            OffsetType4();//type4��ʱ�������жϷ��ص��Ƿ�Ϊ����ֵ 
	BOOL			OffSet( );    //�жϷ���ֵ�����Լ�����,���Ƽ�ʹ��,
    int             GetOffSet();
	BOOL            Create(UINT uStyle,CRect rectWnd,CWnd *pParent,UINT nID);
	void            SetFrame(__int64 iFrame,BOOL bPaint = TRUE ,BOOL ResetNeg = FALSE);	
	void            SetString(CString strFrame,BOOL bPaint = TRUE);
	void            SetVideoType(VIDEO_TYPE eVideoType,BOOL bReCalc = FALSE,BOOL bFrameBase = FALSE);
	__int64         GetFrame();
	CString         GetString();
	void            SetEditMode(int iMode);
	void            SetEditStyle(UINT uStyle,BOOL flag=FALSE);
	void            SetRange(__int64 lMin,__int64 lMax);
	void            SetTextColor(TP_SYSTEM_COLOR eColorIndex);
	void            SetBackColor(TP_SYSTEM_COLOR eColorIndex);
	void            SetFont(CFont &ftSet);
	virtual void    PaintDC(CDC *pDC);
	virtual void    GetTimeCodeRect(CRect &rtTimeCode);
	virtual void    DrawNeg(CDC *pDC ,CRect rtText);
	void            ResetCTL();
	virtual void    SubWindow(BOOL bSub);
	static void     RegisterClass(BOOL bRegister);
	void            SetCTL(BOOL bSet);
	BOOL            TestTheString(CString &strtest);
	void            SetNegFont(CFont &);
	void            FunCalcPos(int iMode,BOOL show=FALSE); 
	BOOL            IsEditState(){ return ( m_uShowStyle != 0);}; //by zhh on 2007-10-16
	void            SetTimeCodeStyle(INT64 uStyle,BOOL bSendChangeMsg = TRUE);
	static int      GetMaxFrame(VIDEO_TYPE Video_Type);

	void			FunCalcFrame();//lzq:+/-ģʽ�£��������ս����2009.08.28
	void            SetTimeCodeStyleEx(UINT uStyleEx);//lzq:����Ex���ԡ�

	HWND			  UnsubclassWindow();
	static  void      SetTCClipBoardData(__int64 i64CopyData);
	static __int64&   GetTCClipBoardData();



	//{ 4.6Feature - 1269. Identification for Ingesting Add by CZ.Lee : CD2009.10.01
#ifdef TP_NS_NET
	void			SetNotifyMode( BOOL bSet = TRUE ); // ��������ģʽ
#endif
	//}

private:

	BOOL            TestTwoBitString();
	BOOL            VIDEO_30MReduce();
	void            ChangeStringShowStyle(BOOL flag=TRUE);
    void            SetEditModeNoChange(int iMode);  
    void            SwitchTestShow(CString &strShow,UINT flag);
	int             GetFrame2Bite(int m_iFocusPos,int &maxnum);
	void            AddCharToFrame(char nChar);
	void            MoveTheTimeCodex(CString &strInput,UINT nChar);
	void            AddFrameTo2Bite(CString &str,int num);
	void            AjustFocus(int &focus);
	BOOL            FunKeyDown(UINT nChar);	 
	bool            FunVertiftPos(__int64 iFrame);
	int             AddFrameFromString(int iPos,int iValue);   	
	int             AddFrameFromNormalString(int iPos,int iValue);
	LRESULT         OnTimeCodeThis(WPARAM wp,LPARAM lp); 
	LRESULT         OnUnsubWindow(WPARAM wp,LPARAM lp); 
	CString         StringFromFrame(VIDEO_TYPE enVideoType,__int64 iFrame);
	INT64           FrameFromString(VIDEO_TYPE eType,CString strFrame,BOOL bIsAddRemove=FALSE); //XN00022424 linhao 2009-11-03 
	void            MoveTheTimeCode(CString &strTimeCode,UINT nChar);
	void            GetBaseTextWidth();

protected:
	virtual int     OnToolHitTest(CPoint point,TOOLINFO* pTI ) const;
	BOOL            OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	DECLARE_MESSAGE_MAP()
	virtual BOOL    PreCreateWindow(CREATESTRUCT& cs);
	virtual void    PreSubclassWindow();
	void			DisableCover();
public:
	afx_msg int     OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void    OnDestroy();
	afx_msg void    OnPaint();
	afx_msg BOOL    OnEraseBkgnd(CDC* pDC);
	afx_msg void    OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL    PreTranslateMessage(MSG* pMsg);
	afx_msg void    OnSize(UINT nType, int cx, int cy);
	afx_msg void    OnTimer(UINT nIDEvent);
	afx_msg void    OnSetFocus(CWnd* pOldWnd);
	afx_msg void    OnKillFocus(CWnd* pNewWnd);
	afx_msg void    OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL    OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void    OnNcPaint();
	afx_msg void    OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void    OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void    OnLButtonDblClk(UINT nFlags, CPoint point);
};






















































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































