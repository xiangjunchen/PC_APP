/*########################################################################
	【文件名】: 	WebHost.cpp
	【名  称】:		解析web连接的封装类.
	----------------------------------------------------
	Remarks:	...
	----------------------------------------------------
	Author:		huawenNie
	Email:		nie173@vip.sina.com
	MSN:		nie173@msn.com
	Created:	03/05/2004
  ########################################################################*/

#include "stdafx.h"

TCHAR *sz_SpecChar[][2] = { {_L("&lt"), _L("<")}, {_L("&gt"),_L(">")}};

//构造函数
CTPWebHost::CTPWebHost(const CString& m_str_webcode ,  vector<HyperLink>& m_vec_All_URL ,HyperLink& str_URL)
{
   str_Page_URL = str_URL.str_Hyperlink;
   str_pagetitle = str_URL.str_HyperlinkText;
   mb_ifFream = FALSE;
   OnRetrunWebContent(  m_str_webcode , m_vec_All_URL );
}

//=============================================================
//
// 返回所有合法链接
//
//=============================================================
void CTPWebHost::OnRetrunWebContent(const  CString& str_htmlcode ,  vector<HyperLink>& m_vec_All_URL)
{
   // 处理JAVASCRIPT代码
   OnAnalyseJavascrript( str_htmlcode , m_vec_All_URL);
   //处理嵌套代码的URL
   OnReturnFrameURL( str_htmlcode , m_vec_All_URL );
   //
   int pagesize = str_htmlcode.GetLength();
   //获取html的url
	OnGetHtmlURL( str_htmlcode, m_vec_All_URL, pagesize);
   //获取跳转的url
	if(m_vec_All_URL.size()<5)
	{
       OnGetJumpURL( str_htmlcode, m_vec_All_URL,pagesize);
	}
   
}
//////////////////////////////////////////////////////////////////////////
//返回Javascript代码里面合要求的URL 参数说明
//[页面代码],[查重URL容器],[返回的URL容器]
void CTPWebHost::OnAnalyseJavascrript(const CString& str_htmlcode ,  vector<HyperLink>& m_vec_All_URL  )
{
	CStringArray str_javascript;
	int pos1 = 0;
	int pos2 = 0;
	//int iLinkTextpos=0;
	//获取页面所有javascipt代码
	int isafety=0;
	while( pos1 != -1&&isafety<100 )
	{
		pos1 = str_htmlcode.Find(_L("<script") , pos1 );
		if( pos1!=-1 )
		{
			pos2 = str_htmlcode.Find(_L("</script>") , pos1 );
			pos2 +=9;
			str_javascript.Add( str_htmlcode.Mid( pos1 , pos2-pos1 ) );
			pos1 = pos2-1;
		}
		isafety++;
	}
	//返回页面Javascrript里面的URL
	CString str_link;
	for(int a = 0 ; a < str_javascript.GetSize() ; a++)
	{

		pos1=0;
		pos2=0;
		for(int b=0;pos1!=-1&&b<str_javascript[a].GetLength();b++)
		{
			pos1 = str_javascript[a].Find(_L("('") , pos1 );
			if( pos1!=-1 )
			{
				pos1 +=1 ;
				pos2 = str_javascript[a].Find(_L("')"),pos1);
				pos2 -=pos1;

				str_link = str_javascript[a].Mid(pos1,pos2);

				if(str_link.Find(_L(".htm"),0)!=-1||str_link.Find(_L(".html"),0)!=-1||
					str_link.Find(_L(".php"),0)!=-1||str_link.Find(_L(".asp"),0)!=-1||
					str_link.Find(_L(".xml"),0)!=-1||str_link.Find(_L("http://"),0)!=-1)
			 {
				 str_link.Replace(_L(" "),_L(""));
				 str_link.Replace(_L("'"),_L(""));
				 str_link.Replace(_L("\""),_L(""));
				 //,逗号处理
				 if(str_link.Find(_L(","),0)!=-1)
				 {
					 str_link=str_link.Left(str_link.Find(_L(","),0));
				 }
				 //=号处理
				 if(str_link.Find(_L("="),0)!=-1)
				 {
					 str_link=str_link.Right(str_link.Find(_L("="),0));
				 }
				 //>处理
				 if(str_link.Find(_L(">"),0)!=-1)
				 {
					 str_link=str_link.Left(str_link.Find(_L(">"),0));
				 }
				 //-----------------------------------------------------------------
				 if(str_link.Find(_L("http://"),0)<-1)//URL如果是相对地址的话
				 { 
					 str_link = OnConversionURL ( str_Page_URL , str_link);
				 }	

					 if( str_link.GetLength()>5)
					 {
						 m_HyperLink.str_Hyperlink=str_link;
						 m_HyperLink.str_HyperlinkText.Empty();
						 m_vec_All_URL.push_back( m_HyperLink );
					     
					 }
				 
			 }
			}
		}
	}
}
//====================================================
//
// ../表示向上一层
// /表示根目录下的
// XX.htm表示当前目录下的
// 把URL转换成绝对地址
CString CTPWebHost::OnConversionURL(CString sURL,CString str_fafURL)
{
	if(sURL.Find(_L("/"),8)<0)
	{
		sURL +="/";
	}
	str_fafURL.Replace(_L("'"),_L(""));
	CString str_activeURL;
	int int_j = 0;
	int i=0;
	str_activeURL = str_fafURL;
	if(str_fafURL.Find(_L("../"),0)!=-1&&str_fafURL[0]!='/')
	{
		while( i<=str_fafURL.GetLength() )
		{
			if( str_fafURL[i] == '.' && str_fafURL[i+1] == '.' && str_fafURL[i+2] == '/' )
			{ int_j++;}
			i++;
		}
		if(str_fafURL[0]=='/')
		{
			str_fafURL.Delete(0,1);
		}
		str_fafURL.Replace(_L("../"),_L(""));
		i=0;
		int int_i=0;
		while( i <= sURL.GetLength() )
		{
			if( sURL[i]=='/' )
			{ 
				int_i++;
			}
			i++;
		}
		int_i -= int_j;

		if( int_i<3 )
		{
			int_i = 3;
		}

		int int_cour=0;
		for( i=0; i<=sURL.GetLength(); i++)
		{
			if( sURL[i]=='/' )
			{ 
				int_cour++;
			}
			if( int_cour==int_i )
			{			 
				sURL= sURL.Left(i+1);
				break;
			}
		}
		//容错处理
		if( sURL[sURL.GetLength()-1]!='/' )
		{	
			sURL +="/";
		}
		sURL += str_fafURL;
		//   sURL += "[转换] 1";
		return sURL;
	}
	else
	{
		if( str_fafURL[0] =='/' )
		{
			int int_b = 0 ;
			for( int a=0; int_b<3 && a<sURL.GetLength(); a++)
			{
				if( sURL[a]=='/' )
			 {
				 int_b++;
			 }
			 if( int_b==3 )
			 {
				 sURL = sURL.Left(a);
				 break;
			 }
			}
			sURL += str_fafURL;
			//sURL += "[转换] 2";
		}
		else
		{
			for( int i=sURL.GetLength() ; i> 0 ; i-- )
			{
				if( sURL[i] =='/' )
				{
					sURL = sURL.Left( i+1 );
					break;
				}
			}
			sURL += str_fafURL;
			// sURL += "[转换 3]";
		}
		//	 sURL += "[转换]";
		return sURL;
	}
}
//////////////////////////////////////////////////////////////////////////
//获取html的url
void CTPWebHost::OnGetHtmlURL(const CString& str_htmlcode, vector<HyperLink>& m_vec_All_URL,int pagesize)
{
	int tag3 = 0;
	int tag0 = 0;
	int iLinkTextpos=0;
	int tag1 = 0;

	for(int i=0;i<pagesize&&tag0!=-1;i++)
	{
		tag0=str_htmlcode.Find(_L("href="),tag0);
		if(tag0!=-1)
		{tag0 +=5;}
		if(tag0 <= -1)	continue;

		tag1=str_htmlcode.Find(_L(">"),tag0);
		iLinkTextpos = tag1;
		tag1 -=tag0;
		str_link=str_htmlcode.Mid(tag0,tag1);
		str_link.Replace(_L("\""),_L(""));
		 
		if(!str_link.IsEmpty())
		{
			tag3=str_link.Find(_L(" "),0);
			if(tag3!=-1)
			{	
				str_link=str_link.Left(tag3);
			}
			else 
				//URL可能含有换行符合
			if(str_link.Find(_L("\n"),0)!=-1)
			{
				str_link=str_link.Left(str_link.Find(_L("\n"),0));
			}
	       
			//-----------------------------------------------------------------
			if(str_link.Find(_L("http://"),0)<0)//URL如果是相对地址的话
			{ 
				str_link= OnConversionURL(str_Page_URL,str_link);
			}			    			
				str_link.Replace( _L("\'") , _L(""));
				m_HyperLink.str_Hyperlink=str_link;
				//AfxMessageBox(str_link);
				m_HyperLink.str_HyperlinkText=OnGetLinkText(iLinkTextpos+1,str_htmlcode);
               m_vec_All_URL.push_back(m_HyperLink);
		}//if结束
	}//if over
}
//////////////////////////////////////////////////////////////////////////
//获取跳转的url
void CTPWebHost::OnGetJumpURL(const CString& str_htmlcode, vector<HyperLink>& m_vec_All_URL, int pagesize)
{
	//处理跳转
	int tag0=0;
	int tag1=0;
	int tag3=0;
	int tag4=0;
	int i;
	int iLinkTextpos=0;
	for( i=0;i<pagesize&&tag0!=-1;i++)
	{
		tag0=str_htmlcode.Find(_L("url="),tag0);
		if(tag0!=-1)
		{tag0 +=4;}
		tag1=str_htmlcode.Find(_L(">"),tag0);
		iLinkTextpos = tag1;
		tag1 -=tag0;
		str_link=str_htmlcode.Mid(tag0,tag1);

		if(!str_link.IsEmpty())
		{

			tag3=str_link.Find(_L("\""),0);
			if(tag3!=-1)
			{ str_link.Replace(_L("\""),_L(""));}
			tag3=str_link.Find(_L(" "),0);
			if(tag3!=-1)
			{	
				str_link=str_link.Left(tag3);
			}
			else
				//URL可能含有换行符合
				if(str_link.Find(_L("\n"),0)!=-1)
				{
					str_link=str_link.Left(str_link.Find(_L("\n"),0));
				}

				if(str_link.Find(_L("http://"),0)==-1)//URL如果是相对地址的话
				{ 
					str_link=OnConversionURL(str_Page_URL,str_link);
				}	
				if( !tag4 && str_link.GetLength() > 5)//URL未被处理
				{
					str_link.Replace( _L("\'") , _L(""));
					mb_ifFream=TRUE;
					m_HyperLink.str_Hyperlink=str_link;
					m_HyperLink.str_HyperlinkText=OnGetLinkText(iLinkTextpos+1,str_htmlcode);
					m_vec_All_URL.push_back(m_HyperLink);

				}
		}//if结束
	}//if over
}

//////////////////////////////////////////////////////////////////////////

//取URL连接文字
CString CTPWebHost::OnGetLinkText(int iIn,const CString& htmlcode)
{
	CString str_return;
	int ipos1=0;
	int ipos2=0;
	int iSafety=0;
	ipos1=htmlcode.Find(_L("</a>"),iIn);
    ipos2 = htmlcode.Find(_L("<a>"),iIn);
    if(ipos2>0)
     {
       if(ipos2<ipos1)
          ipos1 = ipos2;
     }
	if(ipos1>-1)
	{
        str_return =htmlcode.Mid(iIn,ipos1-iIn);
		if(str_return.Find(_L("href="),0)<0)
		{
			while(ipos1>-1&&iSafety<500)
			{
				iSafety++;
				//
				ipos1 = -1;
				ipos1 = str_return.Find(_L("<"),0);
				if(ipos1 > -1)
				{
					ipos2 = str_return.Find(_L(">"),ipos1);
					if(ipos2>-1)
					{
						str_return.Delete(ipos1,ipos2-ipos1+1);
					}
				}
			}
		}
		else
		{
			str_return.Empty();
		}
	}
    
	if(str_return.GetLength()==1||str_return.GetLength()==2)
	{
		str_return =str_pagetitle;
	}
	return str_return;
}
//处理嵌套代码的URL
void CTPWebHost::OnReturnFrameURL(const CString& str_htmlcode ,vector<HyperLink>& m_vec_All_URL  )
{
   CStringArray str_javascript;
   int pos1 = 0;
   int pos2 = 0;
   str_javascript.RemoveAll();
   //获取页面所有 frame 代码
    CString str_link;
   while( pos1 != -1 )
   {
    pos1 = str_htmlcode.Find( _L("<frame") , pos1 );
    if( pos1!=-1 )
    {
	pos2 = str_htmlcode.Find( _L("</frameset>") , pos1);
    pos1 += 6;
    pos2 -= pos1;
	str_javascript.Add( str_htmlcode.Mid( pos1 , pos2 ) );
    }
   }
   for(int a = 0 ; a < str_javascript.GetSize() ; a++)
   {
	   
       //AfxMessageBox( str_javascript[a] );
	   pos1=0;
	   pos2=0;
	   for(int b=0;pos1!=-1&&b<str_javascript[a].GetLength();b++)
	   {
		   
           pos1 =str_javascript[a].Find( _L("src") , pos1 );
		   if( pos1!=-1 )
		   {
			   pos1 = str_javascript[a].Find( _L("=") , pos1 );
			   pos2 = str_javascript[a].Find( _L(">") , pos1 );
			   pos1 += 1;
			   pos2 -= pos1;
               str_link = str_javascript[a].Mid(pos1,pos2);
			  
               if(str_link.Find(_L("http://"),0)<0 
				   && str_link.Find(_L(">"),0)<0 && str_link.Find(_L("<"),0)<0 )//URL如果是相对地址的话
				 { 
				   
			      str_link = OnConversionURL ( str_Page_URL , str_link);
				 }		  
			    str_link.Replace( _L("\'") , _L(""));
				
				if( str_link.GetLength() > 5)//URL未被处理
				{
					mb_ifFream =TRUE;		  
				m_HyperLink.str_Hyperlink=str_link;
                m_HyperLink.str_HyperlinkText.Empty();
			    m_vec_All_URL.push_back(m_HyperLink);
				}
		   }
	   }
   }
}
void CTPWebHost::ReplaceSpecChar(CString &sHtml)
{
	//AfxMessageBox(sHtml);
 // 	int nSpec = sizeof(sz_SpecChar[][2])/sizeof(TCHAR);
 //	for (int l = 0 ; l < nSpec; l++)
 //	{
 //		sHtml.Replace(sz_SpecChar[l][0] , sz_SpecChar[l][1]);
 //	}
	//AfxMessageBox(sHtml);

}


//====================================================
//URL模板,有4个通配符,$ 表示 一个数字,^ 表示一串数字,# 表示一个字符,!表示一串字符 文本
//判断URL是否是
// http://news.sina.com.cn
//====================================================


