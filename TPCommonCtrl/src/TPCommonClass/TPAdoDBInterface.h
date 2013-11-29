#pragma once


class CTPAdoDB
{
public:
	CTPAdoDB();
	~CTPAdoDB();

	LRESULT OpenConnection(CString sDBPath);
	LRESULT CloseConnection();

	LRESULT GetItemDBData();
	BOOL    IsItemExist();
private:
	_ConnectionPtr		   m_pConnection ;

	CString   GetAdoError(_ConnectionPtr	pConFileCount);

protected:
};
class AFX_EXT_CLASS CTPAdoDBInterface
{
public:
	CTPAdoDBInterface(void);
	~CTPAdoDBInterface(void);
};
