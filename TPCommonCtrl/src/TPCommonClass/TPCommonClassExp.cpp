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
	if(25 == uTye)      TP_WriteProfileInt(_T("1"),_T("32"),bSet); //ǿ��ɾ���ز�
	else if(uTye == 29) TP_WriteProfileInt(_T("PathSet"),_T("TeamMode"),bSet); // TeamMode
	else if(uTye == 30) TP_WriteProfileInt(_T("PathSet"),_T("AutoMessage"),bSet); // TeamMode
	else if(uTye == 32) TP_WriteProfileInt(_T("PathSet"),_T("AutoLogin"),bSet); // TeamMode
}
BOOL         TP_GetState(UINT uTye)
{
	if(uTye == 0)       return  TP_GetProfileInt(_L("1"),_L("12"),0);//�ز��Ƿ�ʹ�����ݿ�
	else if(uTye == 1)  return  TP_GetProfileInt(_T("1"),_T("13"),0);//�Ƿ�ʹ�ù��°�༭	
	else if(uTye == 3)  return  TP_GetProfileInt(_T("1"),_T("8"),0); //�Ƿ�����ģ��
	else if(uTye == 4)  return  TP_GetProfileInt(_T("1"),_T("10"),0);//�Ƿ���زĲʵ�
	else if(uTye == 5)  return  TP_GetProfileInt(_T("1"),_T("9"),0); //�زĿɲ���ϵͳ��Դ
	else if(uTye == 6)  return  TP_GetProfileInt(_T("1"),_T("7"),0); //����ǰ���
	else if(uTye == 7)  return  TP_GetProfileInt(_T("1"),_T("14"),0); //�Ƿ�ʹ���û�
	else if(uTye == 8)  return  TP_GetProfileInt(_T("1"),_T("15"),0); //PreEdit
	else if(uTye == 9)  return  TP_GetProfileInt(_T("1"),_T("19"),0); //TP_CaptureRender_TestUsed
	else if(uTye == 10) return  TP_GetProfileInt(_T("1"),_T("17"),0); //Seek�Ƿ�ϳ�
	else if(uTye == 11) return  TP_GetProfileInt(_T("1"),_T("18"),0); //DeckLink ProEdit
	else if(uTye == 12) return  TP_GetProfileInt(_T("1"),_T("19"),0); //ʱ�����ϻ��仯��Χ
	else if(uTye == 13) return  TP_GetProfileInt(_T("1"),_T("20"),0); //���ڶ���λ�ô���
	else if(uTye == 14) return  TP_GetProfileInt(_T("1"),_T("21"),0); //�ɼ�����ʱ�Ƿ�ɾ���ļ�
	else if(uTye == 15) return  TP_GetProfileInt(_T("1"),_T("22"),0); //Sam Fam
	else if(uTye == 16) return  TP_GetProfileInt(_T("1"),_T("23"),0); //CustoSet
	else if(uTye == 17) return  TP_GetProfileInt(_T("1"),_T("24"),0); //Clip Audio Ctrl
	else if(uTye == 18) return  TP_GetProfileInt(_T("1"),_T("25"),0); //ͳ���زĸ���
	else if(uTye == 19) return  TP_GetProfileInt(_T("1"),_T("26"),0); //p2����fangshi
	else if(uTye == 20) return  TP_GetProfileInt(_T("1"),_T("27"),0); //ʱ�����ϻ���Ƶ���
	else if(uTye == 21) return  TP_GetProfileInt(_T("1"),_T("28"),0); //X1 Render �� ʱ�� �Ƿ��Output
	else if(uTye == 22) return  TP_GetProfileInt(_T("1"),_T("29"),0); //SmartEdit Ȩ��
	else if(uTye == 23) return  TP_GetProfileInt(_T("1"),_T("30"),0); //ȡ��SmartRender
	else if(uTye == 24) return  TP_GetProfileInt(_T("1"),_T("31"),0); //�ϳɴ�Key
	else if(uTye == 25) return  TP_GetProfileInt(_T("1"),_T("32"),0); //ǿ��ɾ���ز�
	else if(uTye == 26) return  TP_GetProfileInt(_T("1"),_T("33"),0); //�ؼ�����ģʽ
	else if(uTye == 27) return  TP_GetProfileInt(_T("1"),_T("34"),0); //AI3
	else if(uTye == 28) return  TP_GetProfileInt(_T("1"),_T("35"),0); // �����ö��û�SaveData
	else if(uTye == 29) return  TP_GetProfileInt(_T("PathSet"),_T("TeamMode"),0); // TeamMode
	else if(uTye == 30) return  TP_GetProfileInt(_T("PathSet"),_T("AutoMessage"),0); // TeamTalk
	else if(uTye == 31) return  TP_GetProfileInt(_T("1"),_T("36"),0); // ǿ��ɾ��
	else if(uTye == 32) return  TP_GetProfileInt(_T("PathSet"),_T("AutoLogin"),1); // TeamTalk
	else if(uTye == 33) return  TP_GetProfileInt(_T("1"),_T("37"),1); //  Search No Node
	else if(uTye == 34) return  TP_GetProfileInt(_T("1"),_T("38"),0); // DWMStatus
	else if(uTye == 35) return  TP_GetProfileInt(_T("1"),_T("39"),0); // DWMStatus

	else if(uTye == 40) return  TP_GetProfileInt(_T("1"),_T("40"),0); //����������� 
	else if(uTye == 41) return  TP_GetProfileInt(_T("1"),_T("41"),0); //���������´��� 

	return TRUE;
}
}