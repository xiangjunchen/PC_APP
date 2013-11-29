#pragma once

class CTPBaseNetInterface
{
public:
	CTPBaseNetInterface(void);
	~CTPBaseNetInterface(void);

public:
	virtual BOOL IsNetConnect();
};
