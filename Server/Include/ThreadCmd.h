#ifndef _ST_THREADCMD_H
#define _ST_THREADCMD_H

#include "NullCmd.h"
#include "Define.h"

namespace Cmd
{
#pragma pack(push,1)
	/// gateway断开，删除用户
	struct cmdGateDisconn: public Cmd::stNullCmd
	{
		cmdGateDisconn()
		{
			byCmd = 1;
			byParam = 1;
		}
			
		static const BYTE m_byParam = 1;
	};

#pragma pack(pop)
};


#endif

