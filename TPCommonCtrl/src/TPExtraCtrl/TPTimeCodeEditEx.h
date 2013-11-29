#pragma once




#define WM_TIMECODE_CHANGE  WM_USER +1324//时码发送给外面的消息
#define WM_TIMECODE_THIS    WM_USER +1325


#define TP_TIMECODE_CHANGE     0x00000001//时码改变
#define TP_TIMECODE_EDITEND    0x00000002//
#define TP_TIMECODE_RETURN     0x00000004//回车
#define TP_TIMECODE_KILLFOCUS  0x00000008//失去焦点
#define TP_TIMECODE_ESC        0x00000010//esc   
#define TP_TIMECODE_RESETCTL   0x00000020//选择ctlreset 选项 
#define TP_TIMECODE_FRAMEVIEW  0x00000040//设置为帧显示
#define TP_TIMECODE_FTCVIEW    0x00000080//
#define TP_TIMECODE_LTCVIEW    0x00000100//
#define TP_TIMECODE_VITCVIEW   0x00000200//
#define TP_TIMECODE_CTLVIEW    0x00000400//
#define TP_TIMECODE_TCVIEW     0x00000800//
#define TP_TIMECODE_EDITCHANGE 0x00001000//时码改变


#define WC_TPTIMECODEEDITEX _L("TPTimeCodeEditExClass")

#define TP_TIMECODE_NEGATIVE         0x00000001//是否显示负号(不推荐使用 ,规则不是正常负号的规则)
#define TP_TIMECODE_POSITIVE_TEXT    0x00000002 //当不显示负号时,SetFrame()负数的处理宏 
#define TP_TIMECODE_POSITIVE_SIGN    0x00000004//允许模式为（!TP_TIMECODE_NEGATIVE）时做符号的显示
#define TP_TIMECODE_NEGATIVE_POS     0x00000008//符号的显示位置
#define TP_TIMECODE_30M_DURATION     0x00000010//计算Duration
#define TP_TIMECODE_WANTRETURN       0x00000020//决定回车的响应动作,是否切换到下一个时码控件
#define TP_TIMECODE_DRAG             0x00000040//拖动时用宏(内部使用)
#define TP_TIMECODE_LOGO             0x00000080//logo标志宏
#define TP_TIMECODE_MENU             0x00000100//菜单标志宏
#define TP_TIMECODE_SHOWNEG          0x00000200//符号的显示方式
#define TP_TIMECODE_OldNew           0x00000400//冒号的显示方式： .(可不置)
#define TP_TIMECODE_POPINPUT         0x00000800//sv,mv时码标志宏
#define TP_TIMECODE_DISABLE_ZERO_NEG 0x00001000 //当显示为0时,不显示符号
#define TP_TIMECODE_NSCTLDISABLE     0x00002000 //屏蔽CTL_RESET
#define TP_TIMECODE_CHANGEOPEN       0x00004000//换画符号的笔.
#define TP_TIMECODE_TOOLTIPS         0x00008000//是否使用tooltip的宏
#define TP_TIMECODE_DISABLE          0x00010000//disable timecode //不对任何消息进行响应.
#define TP_TIMECODE_DISPLAYNEG       0x00020000//设置正数显示符号的宏

#define TP_TIMECODE_NSLTCTYPE        0x00040000//Ctl模式
#define TP_TIMECODE_NSVITCTYPE       0x00080000//Ctl模式
#define TP_TIMECODE_NSFTCTYPE        0x00100000//Ctl模式
#define TP_TIMECODE_NSCTLTYPE        0x00200000//Ctl模式
#define TP_TIMECODE_NSTCTYPE         0x00400000//Ctl模式

#define TP_TIMECODE_EDITTYPEFFECT    0x00800000//决定弹出的菜单种类
#define TP_TIMECODE_EDITTYPESV       0x01000000//决定弹出的菜单种类
#define TP_TIMECODE_EDITTYPEMV       0x02000000//决定弹出的菜单种类
#define TP_TIMECODE_EDITTYPEREC      0x04000000//决定弹出的菜单种类
#define TP_TIMECODE_EDITTYPEDIGITIZE 0x08000000//决定弹出的菜单种类
#define TP_TIMECODE_EDITTYPTIMECODE  0x10000000//决定弹出的菜单种类
#define TP_TIMECODE_DISABLECOLOR     0x20000000//决定弹出的菜单种类



#define TP_TIMECODE_NSALLTYPE      (TP_TIMECODE_NSFTCTYPE|TP_TIMECODE_NSVITCTYPE|TP_TIMECODE_NSLTCTYPE|TP_TIMECODE_NSCTLTYPE|TP_TIMECODE_NSTCTYPE) 
#define TP_TIMECODE_NSEDITTYPE     (TP_TIMECODE_EDITTYPESV|TP_TIMECODE_EDITTYPEMV|TP_TIMECODE_EDITTYPEREC|TP_TIMECODE_EDITTYPEDIGITIZE|TP_TIMECODE_EDITTYPTIMECODE) 

#define TP_TIMECODE_SHOW_MID         0x40000000//设置时码显示在中间
#define TP_TIMECODE_SHOW_ONLY        0x80000000// 设置时码为只读模式
#define TP_TIMECODE_MENU_READ       0x100000000// 改变时码值的菜单项disable

#define TP_TIMECODE_EDITTYPESPEED   0x200000000   //xl added 

//StyleEx属性
#define TP_TIMECODE_STYLE_EX_CALCULATE		0x00000001//在设置"+/-"模式下 回车 应自己计算出"+/-"后的结果，显示。
#define TP_TIMECODE_STYLE_EX_FRAMETOTC		0x00000002//设置帧显示模式下 回车 自动转换为对应的时码模式，显示。



#define TP_TIMECODE_LOGO_WIDTH       20    
class AFX_EXT_CLASS CTPTimeCodeEditEx : public CTPWnd,public CTPBaseCtrl
{
	DECLARE_DYNAMIC(CTPTimeCodeEditEx)

public:
	CTPTimeCodeEditEx();
	virtual ~CTPTimeCodeEditEx();
public:
	VIDEO_TYPE              m_eVideoType;    //时码制式
	__int64                 m_uEditStyle;    //时码的编辑模式
	UINT                    m_uEditEnable;   //时码的有效模式
	BOOL                    m_bSubWindow;    //系统
	BOOL                    m_bCreate; 
	BOOL					m_bBeep;
	TP_BORDER_STYLE         m_eBorderStyle;  //边框模式
	int                     m_iBackColor;    //背景色
	int                     m_iTextColor;    //文本颜色
	__int64                 m_iabsFrame;     //永远保存着时码的绝对值,不论符号,CTL/TL模式之分.
	__int64                 m_lMin;          //时码显示最小值
	__int64                 m_lMax; 	     //时码显示最大值
	CFont                  *m_pFont;         
	CFont                  *m_pFontNeg;      //画符号的画笔,可以用来调整控件符号.
	BOOL                    m_bTimeView;	 //是否显示的是时间视图
	CString                 m_strInput;      //输入数据
	__int64                 m_iFrameInPut;
	__int64                 m_iUnDoFrame;
	static __int64          m_iCopyFrame;
	INT64                   m_iCTLFrame;	 //基准帧
	INT64                   m_iDragFrame;	 //用于拖拽的变量 

	CRect                   m_rtText;        //显示位置
	CRect                   m_rtNegative;
	CRect                   m_rtFocus;
	CRect                   m_rtSelText;     //码数显示时候的阴影

	int                     m_iFocusPos;     //显示位置
	int                     m_iFocusUndoPos;
	__int64                 m_iSetFocusFrame;
	int                     m_iBeginSel;   
	int                     m_iEndSel;          //历史遗留,没什么大用
	BOOL                    m_bFirstBegin;	    //历史遗留,没什么大用.
	BOOL                    m_bInvalid;         //GetFrame后可以用来判断输入时码值是否有效:有效 TRUE,无效 FALSE.(名字不是我起的...)
	__int64                 m_iDrawNegOffetVal;//如果符号画的位置不标准,可以使用这个变量 (不推荐...)
	__int64                 m_iDrawTimeCodeOffsetVal ;//如果画的时码位置要做偏移,可使用这个变量.
	CWnd                   *m_pMsgWnd;
	UINT                    m_uPreEditStyle;		// FTC,VITC,LTC
	                                                 
public:
                    
	BOOL                    m_boffsetvalueshow;//判断是否显示偏移值 
	__int64                 m_offsetView;   //存储的偏移值 
	BOOL                    m_bNegative;	  //当前是否为副
	BOOL                    m_bLable;         //是否显示 + -
	int                     m_iEditMode;      //编辑模式
	BOOL                    m_bDrawFocus;     //是否显示焦点
	BOOL                    m_bSkipTimer;
	CPoint                  m_ptSave;       
	BOOL                    m_bInsert;       //插入覆盖  //暂时不要考虑
	int                     m_lMaxLength;    //最大文本长度
	BOOL                    m_bFocus;        //是否有焦点
	UINT                    m_uState;        //状态
	BOOL                    m_bViewExchange;//按 *号是否切换显示方式
	UINT                    m_uShowStyle;//选中数字后阴影得标志位
	                                     //0:表示没有选中 ，不显示阴影
                                         //1:表示选中整体，显示全部阴影
	                                     //2:表示选中部分（2个数字位），显示部分阴影
	BOOL                    m_b2BitFlag;//2位输入时的标志位
	BOOL                    m_boffSet;//是否是偏移值
	BOOL                    m_boffsetNeg;//保存负数的符号位.
	int                     m_shadow;
	CTPToolTipCtrl          *m_pShowTips;//未使用
	CString                 m_sToolTips; //ToolTip显示的字符串
	BOOL                    m_bCtlBtn;//模拟ctrl键的标志.用在复制,粘贴的快捷键中.  

	BOOL                    m_bSelCalc;//响应“+/-”后，自己计算结果
	BOOL					m_bFrameToTC;//帧模式下回车 是否转换为时码。

	__int64                 m_iLastInput;//存储输入“+/-”之前的输入数值，用于计算。

protected:
	int                     m_iBaseTextWidth;
	BOOL                    m_bSendChangeMsg;

	//{ 4.6Feature - 1269. Identification for Ingesting Add by CZ.Lee : CD2009.10.01
#ifdef TP_NS_NET
	BOOL					m_bNotifyMode; // 提醒模式
	int						m_iBackColor_Notify;
	int						m_iTextColor_Notify;
#endif
	//}

public:
	void            SetToolTips(CString sText);
    long            OffsetType4();//type4的时候用来判断返回的是否为绝对值 
	BOOL			OffSet( );    //判断返回值类型以及符号,不推荐使用,
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

	void			FunCalcFrame();//lzq:+/-模式下，计算最终结果。2009.08.28
	void            SetTimeCodeStyleEx(UINT uStyleEx);//lzq:设置Ex属性。

	HWND			  UnsubclassWindow();
	static  void      SetTCClipBoardData(__int64 i64CopyData);
	static __int64&   GetTCClipBoardData();



	//{ 4.6Feature - 1269. Identification for Ingesting Add by CZ.Lee : CD2009.10.01
#ifdef TP_NS_NET
	void			SetNotifyMode( BOOL bSet = TRUE ); // 设置提醒模式
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






















































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































