/*########################################################################
	���ļ�����: 	WebHost.h
	����  �ơ�:		����web���ӵķ�װ��.
	----------------------------------------------------
	Remarks:	...
	----------------------------------------------------
	Author:		huawenNie
	Email:		nie173@vip.sina.com
				nie173@msn.com
	Created:	03/05/2004
  ########################################################################*/
#pragma once
// #include <iostream>
//#include <vector>
//using namespace std;


//���������ݽṹ

#ifndef _____HyperLinkTag_h____
#define _____HyperLinkTag_h____

//���������ݼ�
typedef struct tagHyperLinkTag{
	//link address;
	CString str_Hyperlink;
	//link text;
	CString str_HyperlinkText;
}HyperLink;

#endif
class CTPWebHost
{
public:
   //���캯�� ����˵��
   //[ҳ�����],[����URL����]
   //[���ص�URL����],��ҳ��URL]
   //[��ҳ�ı���]
   CTPWebHost( const CString& m_str_webcode ,  vector<HyperLink>& m_vec_All_URL  ,HyperLink& str_URL);
   //�Ƿ��п�ܻ���ת
   BOOL mb_ifFream;
protected:

	//�������кϷ����� ����˵��
	//[ҳ�����],[����URL����],[���ص�URL����],[ץȡURL�Ƚ�ģ������],[����URL�Ƚ�ģ������]
    void OnRetrunWebContent(const  CString& m_str_webcode ,  vector<HyperLink>& m_vec_All_URL );
	//��ȡhtml��url
	void OnGetHtmlURL(const CString& str_htmlcode, vector<HyperLink>& m_vec_All_URL, 
		int pagesize);
    //��ȡ��ת��url
	void OnGetJumpURL(const CString& str_htmlcode, vector<HyperLink>& m_vec_All_URL, int pagesize);
	//���Ƕ�״����URL
	void OnReturnFrameURL(const CString& str_htmlcode ,vector<HyperLink>& m_vec_All_URL) ;

	//��URLת���ɾ��Ե�ַ
	CString OnConversionURL(CString SeedURL,CString RelativeURL) ;
    
	//����Javascript���������Ҫ���URL ����˵��
    //[ҳ�����],[���ص�URL����]
	void OnAnalyseJavascrript(const CString& m_str_webcode ,  vector<HyperLink>& m_vec_All_URL  );
  
	
	
    //ȡURL��������
	CString OnGetLinkText(int iIn,const CString& htmlcode);
	
protected:
	 //��ҳ��URL
     CString str_Page_URL;
	 //��ҳ�ı���
	 CString str_pagetitle;
     HyperLink m_HyperLink;
	 //������ת
	 CString str_link;
};
