#pragma once

class CTPMemFile : public CMemFile
{
public:
	CTPMemFile(void);
	~CTPMemFile(void);

	LRESULT ReadFile(LPCTSTR sFileName);
	LRESULT WriteFile(LPCTSTR sFileName);
public:
	virtual void Close();

};
