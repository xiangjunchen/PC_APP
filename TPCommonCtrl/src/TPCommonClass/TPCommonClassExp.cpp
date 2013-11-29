#include "StdAfx.h"
#include "TPCommonClassExp.h"

namespace _tp_commonclass
{
BOOL         TP_WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry,int nValue)
{
	CTPRegUtils stu;
	if(ERROR_SUCCESS != stu.Open(lpszSection)) 
	{
		if(ERROR_SUCCESS != stu.Create(lpszSection)) return 0;
	}
	UINT uType = stu.SetDword(lpszEntry,nValue);
	stu.Close();
	return uType == ERROR_SUCCESS;
}
UINT         TP_GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry,int nDefault)
{
	CTPRegUtils stu;
	if(ERROR_SUCCESS !=stu.Open(lpszSection)) return nDefault;
	UINT uType = stu.GetDword(lpszEntry,nDefault);
	stu.Close();
	return uType;
}
void         TP_SetState(UINT uTye,BOOL bSet)
{
	if(25 == uTye)      TP_WriteProfileInt(_T("1"),_T("32"),bSet); //强制删除素材
	else if(uTye == 29) TP_WriteProfileInt(_T("PathSet"),_T("TeamMode"),bSet); // TeamMode
	else if(uTye == 30) TP_WriteProfileInt(_T("PathSet"),_T("AutoMessage"),bSet); // TeamMode
	else if(uTye == 32) TP_WriteProfileInt(_T("PathSet"),_T("AutoLogin"),bSet); // TeamMode
}
BOOL         TP_GetState(UINT uTye)
{
	if(uTye == 0)       return  TP_GetProfileInt(_L("1"),_L("12"),0);//素材是否使用数据库
	else if(uTye == 1)  return  TP_GetProfileInt(_T("1"),_T("13"),0);//是否使用故事板编辑	
	else if(uTye == 3)  return  TP_GetProfileInt(_T("1"),_T("8"),0); //是否打开清除模班
	else if(uTye == 4)  return  TP_GetProfileInt(_T("1"),_T("10"),0);//是否打开素材彩旦
	else if(uTye == 5)  return  TP_GetProfileInt(_T("1"),_T("9"),0); //素材可操作系统资源
	else if(uTye == 6)  return  TP_GetProfileInt(_T("1"),_T("7"),0); //下载前解包
	else if(uTye == 7)  return  TP_GetProfileInt(_T("1"),_T("14"),0); //是否使用用户
	else if(uTye == 8)  return  TP_GetProfileInt(_T("1"),_T("15"),0); //PreEdit
	else if(uTye == 9)  return  TP_GetProfileInt(_T("1"),_T("19"),0); //TP_CaptureRender_TestUsed
	else if(uTye == 10) return  TP_GetProfileInt(_T("1"),_T("17"),0); //Seek是否合场
	else if(uTye == 11) return  TP_GetProfileInt(_T("1"),_T("18"),0); //DeckLink ProEdit
	else if(uTye == 12) return  TP_GetProfileInt(_T("1"),_T("19"),0); //时间线上画变化范围
	else if(uTye == 13) return  TP_GetProfileInt(_T("1"),_T("20"),0); //存在读盘位置错误
	else if(uTye == 14) return  TP_GetProfileInt(_T("1"),_T("21"),0); //采集错误时是否删除文件
	else if(uTye == 15) return  TP_GetProfileInt(_T("1"),_T("22"),0); //Sam Fam
	else if(uTye == 16) return  TP_GetProfileInt(_T("1"),_T("23"),0); //CustoSet
	else if(uTye == 17) return  TP_GetProfileInt(_T("1"),_T("24"),0); //Clip Audio Ctrl
	else if(uTye == 18) return  TP_GetProfileInt(_T("1"),_T("25"),0); //统计素材个数
	else if(uTye == 19) return  TP_GetProfileInt(_T("1"),_T("26"),0); //p2上载fangshi
	else if(uTye == 20) return  TP_GetProfileInt(_T("1"),_T("27"),0); //时间线上画音频打包
	else if(uTye == 21) return  TP_GetProfileInt(_T("1"),_T("28"),0); //X1 Render 的 时候 是否带Output
	else if(uTye == 22) return  TP_GetProfileInt(_T("1"),_T("29"),0); //SmartEdit 权限
	else if(uTye == 23) return  TP_GetProfileInt(_T("1"),_T("30"),0); //取消SmartRender
	else if(uTye == 24) return  TP_GetProfileInt(_T("1"),_T("31"),0); //合成带Key
	else if(uTye == 25) return  TP_GetProfileInt(_T("1"),_T("32"),0); //强制删除素材
	else if(uTye == 26) return  TP_GetProfileInt(_T("1"),_T("33"),0); //特技调整模式
	else if(uTye == 27) return  TP_GetProfileInt(_T("1"),_T("34"),0); //AI3
	else if(uTye == 28) return  TP_GetProfileInt(_T("1"),_T("35"),0); // 可以用多用户SaveData
	else if(uTye == 29) return  TP_GetProfileInt(_T("PathSet"),_T("TeamMode"),0); // TeamMode
	else if(uTye == 30) return  TP_GetProfileInt(_T("PathSet"),_T("AutoMessage"),0); // TeamTalk
	else if(uTye == 31) return  TP_GetProfileInt(_T("1"),_T("36"),0); // 强制删除
	else if(uTye == 32) return  TP_GetProfileInt(_T("PathSet"),_T("AutoLogin"),1); // TeamTalk
	else if(uTye == 33) return  TP_GetProfileInt(_T("1"),_T("37"),1); //  Search No Node
	else if(uTye == 34) return  TP_GetProfileInt(_T("1"),_T("38"),0); // DWMStatus
	else if(uTye == 35) return  TP_GetProfileInt(_T("1"),_T("39"),0); // DWMStatus

	else if(uTye == 40) return  TP_GetProfileInt(_T("1"),_T("40"),0); //启动参数检测 
	else if(uTye == 41) return  TP_GetProfileInt(_T("1"),_T("41"),0); //蓝光属性新窗口 

	return TRUE;
}
}