//

#include "stdafx.h"
#include "TPCommonCtrlGdi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#define MAX_COLOR 300


COLORREF         g_tpSysColor[MAX_COLOR];
CFont            g_tpSysFont[FONT_BASE_MAX];
HBRUSH           g_hBrush[BRUSH_MAX];

namespace _tp_commonctrl
{

	void  TP_InitSysFont()
	{	
#ifndef TP_NS_EDIT
		g_tpSysFont[FONT_BASE_TEXT].CreatePointFont(90,_L("宋体"));
#else
		g_tpSysFont[FONT_BASE_TEXT].CreatePointFont(100,_L("Arial"));
		g_tpSysFont[FONT_BASE_TEXT1].CreatePointFont(90,_L("Arial"));
		g_tpSysFont[FONT_BASE_TEXT4].CreatePointFont(75,_L("Arial"));
		g_tpSysFont[FONT_BASE_TEXT2].CreatePointFont(65,_L("Arial"));
		g_tpSysFont[FONT_BASE_TEXT7].CreatePointFont(50,_L("Arial"));
		LOGFONT stuFont;
		g_tpSysFont[FONT_BASE_TEXT].GetLogFont(&stuFont);
		stuFont.lfWeight = FW_BOLD;
		g_tpSysFont[FONT_BASE_TEXT3].CreateFontIndirect(&stuFont);
		g_tpSysFont[FONT_BASE_TEXT4].GetLogFont(&stuFont);
		stuFont.lfWeight = FW_BOLD;
		g_tpSysFont[FONT_BASE_TEXT5].CreateFontIndirect(&stuFont);
		g_tpSysFont[FONT_BASE_TEXT].GetLogFont(&stuFont);
		stuFont.lfWeight = FW_BOLD;
		g_tpSysFont[FONT_BASE_TEXT6].CreateFontIndirect(&stuFont);
#endif

		g_hBrush[BRUSH_WINDOW]  = ::CreateSolidBrush(g_tpSysColor[COLOR_BASE_WINDOW]);
		g_hBrush[BRUSH_FRAME]   = ::CreateSolidBrush(g_tpSysColor[COLOR_BASE_FRAME]);
		g_hBrush[BRUSH_WINDOW2] = ::CreateSolidBrush(g_tpSysColor[COLOR_BASE_WINDOW2]);
		g_hBrush[BRUSH_FRAME2]  = ::CreateSolidBrush(g_tpSysColor[COLOR_BASE_FRAME2]);
#ifdef TP_NS_EDIT		
		g_hBrush[BRUSH_EDIT]         = ::CreateSolidBrush(g_tpSysColor[COLOR_BASE_EDIT]);
		g_hBrush[BRUSH_EDITFOCUS]    = ::CreateSolidBrush(g_tpSysColor[COLOR_BASE_EDITFOCUS]);
		g_hBrush[BRUSH_EDITREDONLY]  = ::CreateSolidBrush(g_tpSysColor[COLOR_BASE_EDITREDONLY]);
		g_hBrush[BRUSH_CLIPTREE]     = ::CreateSolidBrush(g_tpSysColor[COLOR_CLIP_TREE]);
#endif

	}
	void  TP_InitSysColor(DWORD dwBegin,DWORD dwEnd)
	{
#ifndef  _NOCOLORHOOK
		//Window 公有
		g_tpSysColor[COLOR_SCROLLBAR]            = GetSysColor(COLOR_SCROLLBAR);
		g_tpSysColor[COLOR_BACKGROUND]           = GetSysColor(COLOR_BACKGROUND);
		g_tpSysColor[COLOR_ACTIVECAPTION]        = RGB(204,204,204);
		g_tpSysColor[COLOR_INACTIVECAPTION]      = RGB(154,154,154);
		g_tpSysColor[COLOR_MENU]                 = GetSysColor(COLOR_MENU);
		g_tpSysColor[COLOR_WINDOW]               = RGB(108,108,108);
		g_tpSysColor[COLOR_WINDOWFRAME]          = GetSysColor(COLOR_WINDOWFRAME);
		g_tpSysColor[COLOR_MENUTEXT]             = GetSysColor(COLOR_MENUTEXT);
		g_tpSysColor[COLOR_WINDOWTEXT]           = RGB(154,154,154);
		g_tpSysColor[COLOR_CAPTIONTEXT]          = GetSysColor(COLOR_CAPTIONTEXT);
		g_tpSysColor[COLOR_ACTIVEBORDER]         = GetSysColor(COLOR_ACTIVEBORDER);
		g_tpSysColor[COLOR_INACTIVEBORDER]       = GetSysColor(COLOR_INACTIVEBORDER);
		g_tpSysColor[COLOR_APPWORKSPACE]         = GetSysColor(COLOR_APPWORKSPACE);
		g_tpSysColor[COLOR_HIGHLIGHT]            = RGB(33,33,33);
		g_tpSysColor[COLOR_HIGHLIGHTTEXT]        = RGB(204,204,204);
		g_tpSysColor[COLOR_BTNFACE]              = RGB(95,95,95);
		g_tpSysColor[COLOR_BTNSHADOW]            = GetSysColor(COLOR_BTNSHADOW);
		g_tpSysColor[COLOR_GRAYTEXT]             = RGB(154,154,154);
		g_tpSysColor[COLOR_BTNTEXT]              = RGB(104,104,104);
		g_tpSysColor[COLOR_INACTIVECAPTIONTEXT]  = GetSysColor(COLOR_INACTIVECAPTIONTEXT);
		g_tpSysColor[COLOR_BTNHIGHLIGHT]         = GetSysColor(COLOR_BTNHIGHLIGHT);


		g_tpSysColor[ COLOR_3DDKSHADOW]          = RGB(79,79,79);
		g_tpSysColor[ COLOR_3DLIGHT]             = RGB(124,124,124);
		g_tpSysColor[ COLOR_INFOTEXT]            = GetSysColor(COLOR_INFOTEXT);
		g_tpSysColor[ COLOR_INFOBK]              = GetSysColor(COLOR_INFOBK);


		g_tpSysColor[ COLOR_HOTLIGHT]                 = GetSysColor(COLOR_HOTLIGHT);
		g_tpSysColor[ COLOR_GRADIENTACTIVECAPTION]    = GetSysColor(COLOR_GRADIENTACTIVECAPTION);
		g_tpSysColor[ COLOR_GRADIENTINACTIVECAPTION]  = GetSysColor(COLOR_GRADIENTINACTIVECAPTION);
		g_tpSysColor[ COLOR_MENUHILIGHT]              = RGB(255,0,255);
		g_tpSysColor[ COLOR_MENUBAR]                  = RGB(255,0,0);
#else

		g_tpSysColor[COLOR_SCROLLBAR]            = GetSysColor(COLOR_SCROLLBAR);
		g_tpSysColor[COLOR_BACKGROUND]           = GetSysColor(COLOR_BACKGROUND);
		g_tpSysColor[COLOR_ACTIVECAPTION]        = GetSysColor(COLOR_ACTIVECAPTION);
		g_tpSysColor[COLOR_INACTIVECAPTION]      = GetSysColor(COLOR_INACTIVECAPTION);
		g_tpSysColor[COLOR_MENU]                 = GetSysColor(COLOR_MENU);
		g_tpSysColor[COLOR_WINDOW]               = GetSysColor(COLOR_WINDOW);
		g_tpSysColor[COLOR_WINDOWFRAME]          = GetSysColor(COLOR_WINDOWFRAME);
		g_tpSysColor[COLOR_MENUTEXT]             = GetSysColor(COLOR_MENUTEXT);
		g_tpSysColor[COLOR_WINDOWTEXT]           = GetSysColor(COLOR_WINDOWTEXT);
		g_tpSysColor[COLOR_CAPTIONTEXT]          = GetSysColor(COLOR_CAPTIONTEXT);
		g_tpSysColor[COLOR_ACTIVEBORDER]         = GetSysColor(COLOR_ACTIVEBORDER);
		g_tpSysColor[COLOR_INACTIVEBORDER]       = GetSysColor(COLOR_INACTIVEBORDER);
		g_tpSysColor[COLOR_APPWORKSPACE]         = GetSysColor(COLOR_APPWORKSPACE);
		g_tpSysColor[COLOR_HIGHLIGHT]            = GetSysColor(COLOR_HIGHLIGHT);
		g_tpSysColor[COLOR_HIGHLIGHTTEXT]        = GetSysColor(COLOR_HIGHLIGHTTEXT);
		g_tpSysColor[COLOR_BTNFACE]              = GetSysColor(COLOR_BTNFACE);
		g_tpSysColor[COLOR_BTNSHADOW]            = GetSysColor(COLOR_BTNSHADOW);
		g_tpSysColor[COLOR_GRAYTEXT]             = GetSysColor(COLOR_GRAYTEXT);
		g_tpSysColor[COLOR_BTNTEXT]              = GetSysColor(COLOR_BTNTEXT);
		g_tpSysColor[COLOR_INACTIVECAPTIONTEXT]  = GetSysColor(COLOR_INACTIVECAPTIONTEXT);
		g_tpSysColor[COLOR_BTNHIGHLIGHT]         = GetSysColor(COLOR_BTNHIGHLIGHT);


		g_tpSysColor[ COLOR_3DDKSHADOW]          = GetSysColor(COLOR_3DDKSHADOW);
		g_tpSysColor[ COLOR_3DLIGHT]             = GetSysColor(COLOR_3DLIGHT);
		g_tpSysColor[ COLOR_INFOTEXT]            = GetSysColor(COLOR_INFOTEXT);
		g_tpSysColor[ COLOR_INFOBK]              = GetSysColor(COLOR_INFOBK);


		g_tpSysColor[ COLOR_HOTLIGHT]                 = GetSysColor(COLOR_HOTLIGHT);
		g_tpSysColor[ COLOR_GRADIENTACTIVECAPTION]    = GetSysColor(COLOR_GRADIENTACTIVECAPTION);
		g_tpSysColor[ COLOR_GRADIENTINACTIVECAPTION]  = GetSysColor(COLOR_GRADIENTINACTIVECAPTION);
		g_tpSysColor[ COLOR_MENUHILIGHT]              = GetSysColor(COLOR_MENUHILIGHT);
		g_tpSysColor[ COLOR_MENUBAR]                  = GetSysColor(COLOR_MENUBAR);	
#endif	

		//我们定义的数据

		//基本
		//(255, 251, 240),自己感觉一下，相应调整一下数值。 
		//	Windows的标准色(256色模式)
		//	RGB( 0, 0, 0) 黑色
		//	RGB( 0, 0, 255) 蓝色
		//	RGB( 0, 255, 0) 绿色
		//	RGB( 0, 255, 255) 青色
		//	RGB( 255, 0, 0) 红色
		//	RGB( 255, 0, 255) 品红色
		//	RGB( 255, 255, 0) 黄色
		//	RGB( 255, 255, 255) 白色
		//	RGB( 0, 0, 128) 暗蓝色
		//	RGB( 0, 128, 0) 暗绿色
		//	RGB( 0, 128, 128) 暗青色
		//	RGB( 128, 0, 0) 暗红色
		//	RGB( 128, 0, 128) 暗紫色
		//	RGB( 128, 128, 0) 橄榄色
		//	RGB( 128, 128, 128) 暗灰色
		//	RGB( 192, 192, 192) 亮灰色
		//	RGB( 192, 220, 192) 淡绿色
		//	RGB( 166, 202, 240) 天蓝色
		//	RGB( 255, 251, 240) 乳白色
		//	RGB( 160, 160, 164) 中灰色
		if(dwBegin<=COLOR_BASE_BALCK && dwEnd >=COLOR_VU_DISABLE)
		{	
			g_tpSysColor[COLOR_BASE_BALCK]               = RGB(0,0,0);     //一级窗口背景

			g_tpSysColor[COLOR_BASE_FRAME]               = RGB(234,234,233);  //一级窗口背景	//2006-9-15
			g_tpSysColor[COLOR_BASE_FRAME2]              = RGB(90,90,90);     //二级窗口背景

			g_tpSysColor[COLOR_BASE_WINDOW]              = RGB(234,234,233);  //控件窗口背景	
			g_tpSysColor[COLOR_BASE_WINDOW2]             = RGB(125,125,125);  //控件窗口背景	//2006-12-21
			g_tpSysColor[COLOR_BASE_HIGHLIGHT]           = RGB(33,33,33);     //控件高亮背景
			g_tpSysColor[COLOR_BASE_HIGHLIGHT2]          = RGB(33,33,33);     //控件高亮背景

			g_tpSysColor[COLOR_BASE_3DLEFT]				 = RGB(146,146,146);
			g_tpSysColor[COLOR_BASE_3DRIGHT]             = RGB(59,59,59);     //显示立体线条

			g_tpSysColor[COLOR_BASE_3DLEFT2]             = RGB(104,104,104);     //显示立体线条
			g_tpSysColor[COLOR_BASE_3DRIGHT2]            = RGB(59,59,59);

			//{{2006-11-08 hezhou:same as xpri

			g_tpSysColor[COLOR_BASE_TEXT]                = RGB(0, 0, 0);	//正常文本
			g_tpSysColor[COLOR_BASE_TEXTFOCUS]           = RGB(0, 0, 0);	//焦点文本
			g_tpSysColor[COLOR_BASE_TEXTDIS]             = RGB(182,182,182);	//无效文本 
			g_tpSysColor[COLOR_BASE_TEXTHIGHLIGHT]       = RGB(255,255,255);    //高亮文本	
			//}}
			//VU表
			g_tpSysColor[COLOR_VU_NORMAL]                = RGB(98,137,47);     
			g_tpSysColor[COLOR_VU_ALARM]                 = RGB(186,206,77);     
			g_tpSysColor[COLOR_VU_DANGER]                = RGB(199,105,55);      
			g_tpSysColor[COLOR_VU_DIVISION]              = RGB(59,102,110);   
			g_tpSysColor[COLOR_VU_DISABLE]               = RGB(202,202,202);  
		}


		//时间线	
		if(dwBegin<=COLOR_TL_BEGIN && dwEnd >=COLOR_TL_END)
		{
			g_tpSysColor[COLOR_TL_TIMECODE]               = RGB(255,153,0);   
			g_tpSysColor[COLOR_TL_TIMECODECTL]            = RGB(100,190,80);
			g_tpSysColor[COLOR_TL_TIMECODEBACK]           = RGB(85,85,85);	 //2006-9-18::
			g_tpSysColor[COLOR_TL_RECT]                   = RGB(0,0,0);



			g_tpSysColor[COLOR_TL_HEAD]                  = RGB(51,51,51);
			g_tpSysColor[COLOR_TL_HEAD3DL]               = RGB(72,72,72);
			g_tpSysColor[COLOR_TL_HEAD3DR]               = RGB(32,32,32);


			g_tpSysColor[COLOR_TL_GHEAD]                  = RGB(71,81,90);	
			g_tpSysColor[COLOR_TL_GHEAD3DL]               = RGB(90,98,107);
			g_tpSysColor[COLOR_TL_GHEAD3DR]               = RGB(47,55,62);


			g_tpSysColor[COLOR_TL_VHEAD]                  = RGB(210,183,174);	//2006-9-15::
			g_tpSysColor[COLOR_TL_VHEAD3DL]               = RGB(231,212,208);	
			g_tpSysColor[COLOR_TL_VHEAD3DR]               = RGB(157,142,137);	

			g_tpSysColor[COLOR_TL_VTRHEAD]                  = RGB(78,85,92);
			g_tpSysColor[COLOR_TL_VTRHEAD3DL]               = RGB(125,125,125);
			g_tpSysColor[COLOR_TL_VTRHEAD3DR]               = RGB(46,50,58);


			g_tpSysColor[COLOR_TL_AHEAD]                  = RGB(150,157,186);	//2006-9-15::
			g_tpSysColor[COLOR_TL_AHEAD3DL]               = RGB(194,197,214);	
			g_tpSysColor[COLOR_TL_AHEAD3DR]               = RGB(40,43,52);		



			g_tpSysColor[COLOR_TL_ACTTRACK]              = RGB(114,114,114);
			g_tpSysColor[COLOR_TL_VTRACK]                = RGB(93,93,93);		//2006-9-15::
			g_tpSysColor[COLOR_TL_VCLIP]                 = RGB(210,183,174);	
			g_tpSysColor[COLOR_TL_VCLIP3DL]              = RGB(231,212,208);	
			g_tpSysColor[COLOR_TL_VCLIP3DR]              = RGB(157,142,137);	


			g_tpSysColor[COLOR_TL_VTRCLIP]               = RGB(60,64,74);		//过渡轨道素材
			g_tpSysColor[COLOR_TL_VTRCLIP3DL]            = RGB(125,125,125);
			g_tpSysColor[COLOR_TL_VTRCLIP3DR]            = RGB(0,0,0);

			g_tpSysColor[COLOR_TL_PRECLIP]                 = RGB(78,85,92);
			g_tpSysColor[COLOR_TL_PRECLIP3DL]              = RGB(125,125,125);
			g_tpSysColor[COLOR_TL_PRECLIP3DR]              = RGB(46,50,58);

			//{2007-1-12::merge
			g_tpSysColor[COLOR_TL_LAYTRACK]                = RGB(45,46,49);
			g_tpSysColor[COLOR_TL_LAYCLIP]                 = RGB(63,76,104);
			g_tpSysColor[COLOR_TL_LAYCLIP3DL]              = RGB(81,97,131);
			g_tpSysColor[COLOR_TL_LAYCLIP3DR]              = RGB(0,0,0);
			//}

			g_tpSysColor[COLOR_TL_ATRACK]                = RGB(93,93,93);		//2006-9-15::
			g_tpSysColor[COLOR_TL_ACLIP]                 = RGB(150,157,186);	
			g_tpSysColor[COLOR_TL_ACLIP3DL]              = RGB(194,197,214);	
			g_tpSysColor[COLOR_TL_ACLIP3DR]              = RGB(40,43,52);		


			g_tpSysColor[COLOR_TL_ETRACK]                = RGB(93,93,93);		//2006-9-15::
			g_tpSysColor[COLOR_TL_ECLIP]                 = RGB(145,168,173);	//2006-9-27::
			g_tpSysColor[COLOR_TL_ECLIP3DL]              = RGB(183,199,203);	//2006-9-30::
			g_tpSysColor[COLOR_TL_ECLIP3DR]              = RGB(117,127,129);	//2006-9-30::

			g_tpSysColor[COLOR_TL_KTRACK]                = RGB(93,93,93);		//2006-9-15::
			g_tpSysColor[COLOR_TL_KCLIP]                 = RGB(136,147,149);	//2006-9-27::
			g_tpSysColor[COLOR_TL_KCLIP3DL]              = RGB(170,177,178);	//2006-9-30::
			g_tpSysColor[COLOR_TL_KCLIP3DR]              = RGB(117,127,129);	//2006-9-30::


			g_tpSysColor[COLOR_TL_GTRACK]                = RGB(93,93,93);		//2006-9-15::
			g_tpSysColor[COLOR_TL_GCLIP]                 = RGB(78,85,92);		// 字幕物件或简单字幕
			g_tpSysColor[COLOR_TL_GCLIP3DL]              = RGB(125,125,125);
			g_tpSysColor[COLOR_TL_GCLIP3DR]              = RGB(0,0,0);

			g_tpSysColor[COLOR_TL_GCCLIP]                = RGB(81,84,89);		//字幕容器
			g_tpSysColor[COLOR_TL_GCCLIP3DL]             = RGB(125,125,125);
			g_tpSysColor[COLOR_TL_GCCLIP3DR]             = RGB(0,0,0);

			g_tpSysColor[COLOR_TL_GSCLIP]                = RGB(67,74,76);		//字幕子时间线
			g_tpSysColor[COLOR_TL_GSCLIP3DL]             = RGB(125,125,125);
			g_tpSysColor[COLOR_TL_GSCLIP3DR]             = RGB(0,0,0);


			g_tpSysColor[COLOR_TL_CCLIP]                 = RGB(181,138,125);		 //容器     
			g_tpSysColor[COLOR_TL_CCLIP3DL]              = RGB(194,163,152);
			g_tpSysColor[COLOR_TL_CCLIP3DR]              = RGB(94,60,50);

			g_tpSysColor[COLOR_TL_SCLIP]                 = RGB(133,103,102);		//子时间线
			g_tpSysColor[COLOR_TL_SCLIP3DL]              = RGB(173,151,150);
			g_tpSysColor[COLOR_TL_SCLIP3DR]              = RGB(70,53,52);

			g_tpSysColor[COLOR_TL_ICLIP]                 = RGB(59,59,59);
			g_tpSysColor[COLOR_TL_ICLIP3DL]              = RGB(125,125,125);
			g_tpSysColor[COLOR_TL_ICLIP3DR]              = RGB(0,0,0);

			g_tpSysColor[COLOR_TL_WNDBACK]               = RGB(52,52,52);	
			g_tpSysColor[COLOR_TL_HEADBACK]              = RGB(53,53,53);
			g_tpSysColor[COLOR_TL_INOUT]                 = RGB(0,0,0);


			g_tpSysColor[COLOR_TL_HEADTEXT]              = RGB(127,127,127);
			g_tpSysColor[COLOR_TL_HEADTEXTSEL]           = RGB(0,0,0);
			g_tpSysColor[COLOR_TL_CLIPTEXT]              = RGB(255,255,255);
			g_tpSysColor[COLOR_TL_CLIPTEXTSEL]           = RGB(0,0,0);
			g_tpSysColor[COLOR_TL_VATEXT]                = RGB(0,0,0);	//2006-10-13::

			g_tpSysColor[COLOR_LINE_TRACK]               = RGB(0,0,0);

		}
		if(dwBegin<=COLOR_BASE_BACK && dwEnd >= COLOR_WHITE)
		{
			g_tpSysColor[COLOR_BASE_BACK]                = RGB(53,53,53);
			g_tpSysColor[COLOR_CLIP_TEXT]                = RGB(122,122,122);
			g_tpSysColor[COLOR_CLIP_TEXTFOCUS]           = RGB(142,142,142);
			g_tpSysColor[COLOR_CLIP_BACK]                = RGB(14,14,14);
			g_tpSysColor[COLOR_BASE_LIST]                = g_tpSysColor[COLOR_BASE_WINDOW];	//2006-10-13:


			g_tpSysColor[COLOR_CODE_TEXT]                = RGB(87,87,87);
			g_tpSysColor[COLOR_CODE_TEXTFOCUS]           = RGB(88,88,88);
			g_tpSysColor[COLOR_CODE_TEXTDIS]             = RGB(154,154,154);

			g_tpSysColor[COLOR_CODE_BACK]                = RGB(0,0,0);
			g_tpSysColor[COLOR_CODE_BACKDIS]             = RGB(26,26,26);
			g_tpSysColor[COLOR_BLOCK_BACK]               = RGB(27,55,75);	//2007-1-12:
			g_tpSysColor[COLOR_TRIM_BACK]                = RGB(51,51,51);
			g_tpSysColor[COLOR_TRIM_BACKDIS]             = RGB(51,51,51);
			g_tpSysColor[COLOR_CODE_LINE]                = RGB(255,153,0);
			g_tpSysColor[COLOR_CODE_LINEDIS]             = RGB(255,104,0);

			g_tpSysColor[COLOR_CODE_UP]                  = RGB(67,67,67);
			g_tpSysColor[COLOR_CODE_DOWN1]               = RGB(154,154,154);
			g_tpSysColor[COLOR_CODE_DOWN2]               = RGB(21,32,53);

			g_tpSysColor[COLOR_LINE_SELCLIP]             = RGB(160,160,160);
			g_tpSysColor[COLOR_LINE_WAVE]                = RGB(200,200,200);
			g_tpSysColor[COLOR_LINE_WAVEA]               = RGB(94,94,94),
			g_tpSysColor[COLOR_LINE_WAVEA1]              = RGB(252,163,8),

			g_tpSysColor[COLOR_LINE_CLIPLEVEL]           = RGB(169,122,44), // XN00024731 wangjin 20091117
			g_tpSysColor[COLOR_LINE_CLIPLEVELACT]        = RGB(169,122,44),

			g_tpSysColor[COLOR_LINE_LINE]                = RGB(255,0,0);		//
			g_tpSysColor[COLOR_LINE_LINEEX]              = RGB(163,163,163);		//
			g_tpSysColor[COLOR_LINE_LINESEEKEFF]         = RGB(255,255,0);		//
			g_tpSysColor[COLOR_LINE_LINEFOCUS]           = RGB(26,197,255);
			g_tpSysColor[COLOR_LINE_LINEDIS]             = RGB(26,197,255);

			g_tpSysColor[COLOR_BAR_BACK]                = RGB(67,67,67);
			g_tpSysColor[COLOR_BAR_BACKFOCUS]           = RGB(67,67,67);
			g_tpSysColor[COLOR_BAR_BACKDIS]             = RGB(67,67,67);


			g_tpSysColor[COLOR_LINE_SPLITBAR]            = RGB(51,51,51);

			g_tpSysColor[COLOR_LINE_INOUT]               = RGB(179,80,0);
			g_tpSysColor[COLOR_LINE_EXTRECT]             = RGB(82,82,82);
			g_tpSysColor[COLOR_LINE_FX]                  = RGB(189,158,64);
			g_tpSysColor[COLOR_LINE_CLIPEDGETOP]         = RGB(166,166,166);
			g_tpSysColor[COLOR_LINE_CLIPEDGEBOTTOM]      = RGB(0,0,0);


			g_tpSysColor[COLOR_LINE_COMPOSEBACK]         = RGB(255,158,64);
			g_tpSysColor[COLOR_LINE_COMPOSE]             = RGB(60,213,0);
			g_tpSysColor[COLOR_LINE_REALBASE]            = RGB(0,0,0);
			g_tpSysColor[COLOR_LINE_REALFALSE]           = RGB(222,12,37);
			g_tpSysColor[COLOR_LINE_DARK]                = RGB(178,178,178);


			g_tpSysColor[COLOR_LIST_TOP]                 = RGB(51,51,51);
			g_tpSysColor[COLOR_LIST_BOTTOM]              = RGB(101,101,101);
			g_tpSysColor[COLOR_LIST_BACK1]               = RGB(43,43,43);	
			g_tpSysColor[COLOR_LIST_BACK2]               = RGB(57,57,57);
			g_tpSysColor[COLOR_LIST_BACK3]               = RGB(33,33,33);
			//g_tpSysColor[COLOR_LIST_BACKE7NS]            = RGB(217,217,217);//2006-9-20::add


			g_tpSysColor[COLOR_EFFLINE_DIS]              = RGB(119,119,119);
			g_tpSysColor[COLOR_EFFLINE_1]                = RGB(255,255,0);
			g_tpSysColor[COLOR_EFFLINE_2]                = RGB(255,0,0);
			g_tpSysColor[COLOR_EFFLINE_3]                = RGB(255,0,255);	
			g_tpSysColor[COLOR_EFFLINE_4]                = RGB(0,0,255);
			g_tpSysColor[COLOR_EFFLINE_5]                = RGB(0,255,255);
			g_tpSysColor[COLOR_EFFLINE_6]                = RGB(0,255,0);
			g_tpSysColor[COLOR_EFFLINE_7]                = RGB(128,0,128);
			g_tpSysColor[COLOR_EFFLINE_8]                = RGB(128,255,128);

			g_tpSysColor[COLOR_YELLOW]                   = RGB(250,180,0);
			g_tpSysColor[COLOR_RED]						 = RGB(250,0,0);
			g_tpSysColor[COLOR_BLUE]                     = RGB(0,0,255);
			g_tpSysColor[COLOR_ORANGE]                   = RGB(150,180,0);
			g_tpSysColor[COLOR_WHITE]                    = RGB(217,217,217);//2006-10-9::
			g_tpSysColor[COLOR_BLACK]                    = RGB(0,0,0);
			g_tpSysColor[COLOR_GREEN]                    = RGB(0,255,0);
			g_tpSysColor[COLOR_0255255]                  = RGB(0,255,255);

			//{2006-11-28::add for s2
			g_tpSysColor[COLOR_TL_IVCLIP]				 = RGB(85,63,62);
			g_tpSysColor[COLOR_TL_IVCLIP3DL]             = RGB(103,86,85);
			g_tpSysColor[COLOR_TL_IVCLIP3DR]             = RGB(70,49,48);

			g_tpSysColor[COLOR_TL_IACLIP]				 = RGB(60,63,73);
			g_tpSysColor[COLOR_TL_IACLIP3DL]             = RGB(81,83,88);
			g_tpSysColor[COLOR_TL_IACLIP3DR]             = RGB(40,43,52);

			g_tpSysColor[COLOR_TL_IGCLIP]				 = RGB(85,63,22);
			g_tpSysColor[COLOR_TL_IGCLIP3DL]             = RGB(103,86,85);
			g_tpSysColor[COLOR_TL_IGCLIP3DR]             = RGB(70,49,48);

			g_tpSysColor[COLOR_TL_IECLIP]				 = RGB(85,63,22);
			g_tpSysColor[COLOR_TL_IECLIP3DL]             = RGB(103,86,85);
			g_tpSysColor[COLOR_TL_IECLIP3DR]             = RGB(70,49,48);

			g_tpSysColor[COLOR_TL_IKCLIP]				 = RGB(85,63,22);
			g_tpSysColor[COLOR_TL_IKCLIP3DL]             = RGB(103,86,85);
			g_tpSysColor[COLOR_TL_IKCLIP3DR]             = RGB(70,49,48);

			g_tpSysColor[COLOR_TL_IHEADTEXT]             = RGB(127,127,127);
			g_tpSysColor[COLOR_TL_TRACKMARK]             = RGB(223,103,24);
			g_tpSysColor[COLOR_CLIP_LIST]                = RGB(139,139,139);
			g_tpSysColor[COLOR_CLIP_TREE]                = RGB(87,87,87);  


			g_tpSysColor[COLOR_BASE_TIMECODE]            = RGB(255,153,0);
			g_tpSysColor[COLOR_BASE_EDIT]                = RGB(255,255,255);
			g_tpSysColor[COLOR_BASE_EDITFOCUS]           = RGB(202,207,252);
//			g_tpSysColor[COLOR_BASE_EDITREDONLY]         = RGB(199,199,199); 
			g_tpSysColor[COLOR_BASE_EDITREDONLY]         = RGB(185,185,185);  // li hui li 2007-09-26 Change BackColor

			g_tpSysColor[COLOR_BASE_SELECTITEMA]         = RGB(80,89,160); 
			g_tpSysColor[COLOR_BASE_SELECTITEM]          = RGB(64,64,64); 
			g_tpSysColor[COLOR_TL_SYNCBREAK]             = RGB(0,0,255);
			g_tpSysColor[COLOR_TL_SYNCBREAKSELECT]       = RGB(0,0,255);
			g_tpSysColor[COLOR_TL_SYNCBREAKDIS]          = RGB(0,128,255);

			//}
		}

	}

	void  TP_InitSysGdi()
	{	
		TP_InitSysColor(0,MAX_COLOR);
		TP_InitSysFont();	
	}
	void  TP_ReleaseSysGdi()
	{
		for(int  l=0;l<FONT_BASE_MAX;l++)
		{	
			CDC    *pDC  = CDC::FromHandle(::GetDC(GetDesktopWindow()));
			CFont  *pFnt = NULL;
			if(g_tpSysFont[l].m_hObject) 
			{
				pFnt = pDC ->SelectObject(&g_tpSysFont[l]);
				pDC ->SelectObject(pFnt);
				g_tpSysFont[l].DeleteObject();
			}
			CWnd::FromHandle(GetDesktopWindow()) ->ReleaseDC(pDC);
		}	
		for(INT l=0;l<BRUSH_MAX;l++)	::DeleteObject(g_hBrush[l]);
	}
	void      TP_GetSysColor(COLORREF *&pColor,long &lCount)
	{
		pColor = g_tpSysColor;
		lCount = MAX_COLOR;	
	}
	void      TP_ReSetSysColor(TP_SYSTEM_COLOR dwBegin,TP_SYSTEM_COLOR dwEnd)
	{
		TP_InitSysColor(dwBegin,dwEnd);
	}
	COLORREF  TP_GetSysColor(UINT dwIndex)
	{
		if(dwIndex>=MAX_COLOR) return RGB(0,0,0);
		return g_tpSysColor[dwIndex];
	}
	COLORREF  TP_GetResColor(TP_RES_COLOR dwIndex,BOOL bSelect,BOOL bActTrack)
	{	
		if(bActTrack)
		{
			if(bSelect)
			{
				//选中:
				switch(dwIndex)
				{
				case COLOR_BASE_RES_1: return RGB(255,112,112); //red
				case COLOR_BASE_RES_2: return RGB(0,112,255); //blue
				case COLOR_BASE_RES_3: return RGB(76,219,76); //green
				case COLOR_BASE_RES_4: return RGB(255,182,76); //Orange
				case COLOR_BASE_RES_5: return RGB(181,77,181); //purple
				case COLOR_BASE_RES_6: return RGB(255,181,181); //pink
				case COLOR_BASE_RES_7: return RGB(77,174,185); //pale blue
				case COLOR_BASE_RES_8: return RGB(148,255,112); //Light Green
				case COLOR_BASE_RES_9: return RGB(255,255,148); //buff
				case COLOR_BASE_RES_10: return RGB(209,146,181); //lavender
				case COLOR_BASE_RES_11: return RGB(209,209,209); //white
				case COLOR_BASE_RES_DEF: return RGB(102,102,102); //Normal
				case COLOR_BASE_RES_PREROLLOVEWRITE: return RGB(102,102,102); //Normal
				case COLOR_BASE_RES_PREROLLINSERT:   return RGB(102,102,102); //Normal
				default:
					return RGB(102,102,102);
				}
			}
			else
			{		

				//正常:
				switch(dwIndex)
				{
				case COLOR_BASE_RES_1:   return RGB(255,51,51); //red
				case COLOR_BASE_RES_2:   return RGB(0,51,255); //blue
				case COLOR_BASE_RES_3:   return RGB(0,204,0); //green
				case COLOR_BASE_RES_4:   return RGB(255,153,0); //Orange
				case COLOR_BASE_RES_5:   return RGB(150,2,150); //purple
				case COLOR_BASE_RES_6:   return RGB(255,150,150); //pink
				case COLOR_BASE_RES_7:   return RGB(2,140,155); //pale blue
				case COLOR_BASE_RES_8:   return RGB(102,255,51); //Light Green
				case COLOR_BASE_RES_9:   return RGB(255,255,102); //buff
				case COLOR_BASE_RES_10:  return RGB(190,100,150); //lavender
				case COLOR_BASE_RES_11:  return RGB(190,190,190); //white
				case COLOR_BASE_RES_DEF: return RGB(102,102,102); //Normal
				case COLOR_BASE_RES_PREROLLOVEWRITE: return RGB(102,102,102); //Normal
				case COLOR_BASE_RES_PREROLLINSERT:   return RGB(102,102,102); //Normal
				default:
					return RGB(102,102,102);
				}
			}

		}
		else
		{

			//非激活:
			switch(dwIndex)
			{
			case COLOR_BASE_RES_1: return RGB(128,26,26); //red
			case COLOR_BASE_RES_2: return RGB(0,26,128); //blue
			case COLOR_BASE_RES_3: return RGB(0,102,0); //green
			case COLOR_BASE_RES_4: return RGB(128,76,0); //Orange
			case COLOR_BASE_RES_5: return RGB(75,1,75); //purple
			case COLOR_BASE_RES_6: return RGB(128,75,75); //pink
			case COLOR_BASE_RES_7: return RGB(1,70,78); //pale blue
			case COLOR_BASE_RES_8: return RGB(51,128,26); //Light Green
			case COLOR_BASE_RES_9: return RGB(128,128,51); //buff
			case COLOR_BASE_RES_10: return RGB(95,50,75); //lavender
			case COLOR_BASE_RES_11: return RGB(95,95,95); //white
			case COLOR_BASE_RES_DEF: return RGB(102,102,102); //Normal
			case COLOR_BASE_RES_PREROLLOVEWRITE: return RGB(102,102,102); //Normal
			case COLOR_BASE_RES_PREROLLINSERT:   return RGB(102,102,102); //Normal
			default:
				return RGB(102,102,102);
			}
		}
	}

	LPCTSTR	TP_GetResColorName(TP_RES_COLOR dwIndex)
	{
		switch(dwIndex)
		{
		case COLOR_BASE_RES_1: return CTPBaseCtrl::m_stuSBDictionary.SBLoadString(_T("IDS_COMMONCTRL_RED")); //red
		case COLOR_BASE_RES_2: return CTPBaseCtrl::m_stuSBDictionary.SBLoadString(_T("IDS_COMMONCTRL_BLUE")); //blue
		case COLOR_BASE_RES_3: return CTPBaseCtrl::m_stuSBDictionary.SBLoadString(_T("IDS_COMMONCTRL_GREEN")); //green
		case COLOR_BASE_RES_4: return CTPBaseCtrl::m_stuSBDictionary.SBLoadString(_T("IDS_COMMONCTRL_YELLOW")); //yellow
		case COLOR_BASE_RES_5: return CTPBaseCtrl::m_stuSBDictionary.SBLoadString(_T("IDS_COMMONCTRL_PURPLE")); //purple
		case COLOR_BASE_RES_6: return CTPBaseCtrl::m_stuSBDictionary.SBLoadString(_T("IDS_COMMONCTRL_PINK")); //pink
		case COLOR_BASE_RES_7: return CTPBaseCtrl::m_stuSBDictionary.SBLoadString(_T("IDS_COMMONCTRL_PALEBLUE")); //pale blue
		case COLOR_BASE_RES_8: return CTPBaseCtrl::m_stuSBDictionary.SBLoadString(_T("IDS_COMMONCTRL_LIGHTGREEN")); //Light Green
		case COLOR_BASE_RES_9: return CTPBaseCtrl::m_stuSBDictionary.SBLoadString(_T("IDS_COMMONCTRL_BUFF")); //buff
		case COLOR_BASE_RES_10: return CTPBaseCtrl::m_stuSBDictionary.SBLoadString(_T("IDS_COMMONCTRL_LAVENDER")); //lavender
		case COLOR_BASE_RES_11: return CTPBaseCtrl::m_stuSBDictionary.SBLoadString(_T("IDS_COMMONCTRL_WHITE")); //white
		case COLOR_BASE_RES_DEF: return CTPBaseCtrl::m_stuSBDictionary.SBLoadString(_T("IDS_COMMONCTRL_NORMAL")); //Normal
		default:
			return CTPBaseCtrl::m_stuSBDictionary.SBLoadString(_T("IDS_COMMONCTRL_NORMAL")); //Normal
		}
	}

	COLORREF  TP_GetSysColor(TP_SYSTEM_COLOR dwIndex)
	{
		if(dwIndex>=MAX_COLOR) return RGB(0,0,0);
		return g_tpSysColor[dwIndex];
	}
	void      TP_SetSysColor(TP_SYSTEM_COLOR dwIndex,COLORREF clSet)
	{
		if(dwIndex>=MAX_COLOR) return ;
		g_tpSysColor[dwIndex] = clSet;
	}
	void      TP_SetSysColor(UINT dwIndex,COLORREF clSet)
	{
		if(dwIndex>=MAX_COLOR) return ;
		g_tpSysColor[dwIndex] = clSet;
	}
	CFont    *TP_GetSysFont(TP_SYSTEM_FONT eFontIndex)
	{
		return &g_tpSysFont[eFontIndex];
	}
	HBRUSH   TP_GetSysBrush(TP_BRUSH_INDEX eBrushIndex)
	{
		return  g_hBrush[eBrushIndex];
	}
	BOOL CALLBACK EnumChildWndForColor( HWND hwnd,  LPARAM lParam )
	{
		if(hwnd && ::IsWindow(hwnd))	::PostMessage(hwnd,WM_RESET_SYSCOLOR,0,0);
		return TRUE;
	}
	BOOL CALLBACK EnumWndForColor(HWND hwnd, LPARAM lParam  )
	{
		if(hwnd && ::IsWindow(hwnd)) ::EnumChildWindows(hwnd,(WNDENUMPROC)EnumChildWndForColor,(LPARAM)0); 
		if(hwnd && ::IsWindow(hwnd)) ::PostMessage(hwnd,WM_RESET_SYSCOLOR,0,0);
		return TRUE;
	}
	void  TP_ResetSysColor()
	{
		::EnumThreadWindows(AfxGetApp()->m_nThreadID,(WNDENUMPROC)EnumWndForColor,(LPARAM)0); 
	}

	BOOL CALLBACK EnumChildWndForFont( HWND hwnd,  LPARAM lParam )
	{
		if(hwnd && ::IsWindow(hwnd))	::PostMessage(hwnd,WM_RESET_SYSFONT,0,0);
		return TRUE;
	}
	BOOL CALLBACK EnumWndForFont(HWND hwnd, LPARAM lParam  )
	{
		if(hwnd && ::IsWindow(hwnd)) ::EnumChildWindows(hwnd,(WNDENUMPROC)EnumChildWndForFont,(LPARAM)0); 
		if(hwnd && ::IsWindow(hwnd))	::PostMessage(hwnd,WM_RESET_SYSFONT,0,0);
		return TRUE;
	}
	void  TP_ResetSysFont()
	{
		::EnumThreadWindows(AfxGetApp()->m_nThreadID,(WNDENUMPROC)EnumWndForFont,(LPARAM)0); 
	}

	BOOL CALLBACK EnumChildWndForFace( HWND hwnd,  LPARAM lParam )
	{
		if(hwnd && ::IsWindow(hwnd))	::PostMessage(hwnd,WM_RESET_SYSFACE,0,0);
		return TRUE;
	}
	BOOL CALLBACK EnumWndForFace(HWND hwnd, LPARAM lParam  )
	{
		if(hwnd && ::IsWindow(hwnd)) ::EnumChildWindows(hwnd,(WNDENUMPROC)EnumChildWndForFace,(LPARAM)0); 
		if(hwnd && ::IsWindow(hwnd))	::PostMessage(hwnd,WM_RESET_SYSFACE,0,0);
		return TRUE;
	}
	void  TP_ResetSysFace()
	{
		::EnumThreadWindows(AfxGetApp()->m_nThreadID,(WNDENUMPROC)EnumWndForFace,(LPARAM)0); 
	}
	 
};
