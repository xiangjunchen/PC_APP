#pragma once

extern HMODULE g_hArticleManage ;
extern TPArticleManageProcess   g_stuPress;
extern TPArticleManageInterface g_stuArticleInterface;

LRESULT TP_InitArticleCenter();
LRESULT TP_ReleaseArticleCenter();
