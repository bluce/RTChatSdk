#ifndef _ST_SERVICE_H
#define _ST_SERVICE_H

#include <string>
#include "Type.h"
#include "Global.h"

class stService: public stDisableCopy
{
 public:
	const char * GetName() const;

	virtual ~stService();

	static stService *  GetBaseInstancePtr();

	void Main();

	void Terminate();

	bool IsTerminate();

	void DealSignalUsr1();
	
	virtual void DealSignalUsr2();

	virtual void ReloadConfig();

	virtual void OnSignalUsr2();

 protected:
	stService(const char * base_name);
		
	virtual bool Init();

	virtual bool ServiceCallBack() = 0;

	virtual void Final() = 0;
	
 private:

	std::string m_strBaseName;

	static stService * m_pBaseInstance;

	bool m_blTerminate;
};

#endif
