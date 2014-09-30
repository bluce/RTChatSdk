#ifndef ST_NULLCMD_H_
#define ST_NULLCMD_H_

#include "Define.h"
#include "Command.h"

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>


#define defCheckCmdLen(id, count, cmd_len)		\
 	bool is_check = true; \
	defCmdLen recv_len  =  sizeof(Cmd::stProtoBufCmd) + count;	\
	if(cmd_len != recv_len) \
	{ \
		Global::logger->warn("cmd len error kick <id=%u,recv_len=%u,calc_len=%u", id, cmd_len, recv_len); \
		is_check = false; \
	} \

namespace Cmd
{
	const unsigned char NULL_CMD = 0x00;

	const unsigned char NULL_PARA = 0x00;	   

	struct stNullCmd
	{
		stNullCmd()
		{
			byCmd = NULL_CMD;
			byParam = NULL_PARA;
		}
		
		unsigned char byCmd;
		unsigned char byParam;				
	}__attribute__((packed));


	struct stProtoBufCmd
	{
		defCmdLen len()
		{
			return (cmdlen + sizeof(*this));
		}

		defCmdId cmdid;
		defCmdLen cmdlen;
		char data[0];
	}__attribute__((packed));


	struct stInternalCmd
	{
		defCmdLen len()
		{
			return (cmdlen + sizeof(*this));
		}

		defTempId tempid;
		defCmdLen cmdlen;
		char data[0];
	}__attribute__((packed));


	extern void Serialize(Cmd::stProtoBufCmd * send_buf, const ::google::protobuf::Message * p_send_cmd);
};




template<typename buf_type>
inline void AutoConstruct(buf_type * ptr)
{
	new (static_cast<void *>(ptr)) buf_type();
}

#endif

