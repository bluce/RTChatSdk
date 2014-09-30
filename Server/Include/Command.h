#ifndef _STRT_COMMAND_H
#define _STRT_COMMAND_H

#include "Type.h"

namespace Cmd
{
#pragma pack(push,1)

	struct stTaskId
	{
		union
		{
			QWORD taskid;
			struct
			{
				DWORD server_id;
				DWORD order_id;
			};
		};

		stTaskId()
		{
			server_id = 0;
			order_id = 0;
		}

		stTaskId(QWORD tempid)
		{
			taskid = tempid;
		}

		stTaskId & operator= (const QWORD temp_id)
		{
			taskid = temp_id;
			return (*this);
		}
			
		operator unsigned long long () const
		{
			return taskid;
		}
		
		bool IsVaild()
		{
			if(server_id == 0 || order_id == 0)
			{
				return false;
			}
			return true;
		}
	};

	struct stLogicSvrInfo
	{
		stLogicSvrInfo()
		{
			id = 0;
			memset(ip, 0, sizeof(ip));
			port = 0;
		}

		WORD id;
		char ip[16];
		WORD port;
	};


#pragma pack(pop)
};





#endif
