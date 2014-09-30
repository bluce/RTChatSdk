#include "NullCmd.h"
#include "AssertEx.h"
#include "Global.h"
#include "internal.pb.h"
#include "game.pb.h"

namespace Cmd
{	
#define cmdSerialize(name)							\
	const name *  p_cmd = (const name * )p_send_cmd;				\
	p_cmd->google::protobuf::MessageLite::SerializeToArray(send_buf->data, send_buf->cmdlen);
	
	void Serialize(Cmd::stProtoBufCmd * send_buf, const ::google::protobuf::Message * p_send_cmd)
	{
		defCmdId id = send_buf->cmdid;
		switch(id)
		{
			case enNotifyLogicInfo:
			{
				cmdSerialize(cmdNotifyLogicInfo);
			}
			break;

		case enNotifyLoginResult:
			{
				cmdSerialize(cmdNotifyLoginResult);
			}
			break;


		case enNotifyCreateResult:
			{
				cmdSerialize(cmdNotifyCreateResult);
			}
			break;

		case enNotifyEnterResult:
			{
				cmdSerialize(cmdNotifyEnterResult);
			}
			break;

		case enNotifyRoomList:
			{
				cmdSerialize(cmdNotifyRoomList);
			}
			break;

		case enNotifyAddVoiceUser:
			{
				cmdSerialize(cmdNotifyAddVoiceUser);
			}
			break;

		case enNotifyDelVoiceUser:
			{
				cmdSerialize(cmdNotifyDelVoiceUser);
			}
			break;

		case enNotifyMicQueue:
			{
				cmdSerialize(cmdNotifyMicQueue);
			}
			break;

		case enNotifyTakeMic:
			{
				cmdSerialize(cmdNotifyTakeMic);
			}
			break;



		/// -<<<<<<<<<<<<<内部协议 >>>>>>>>>>>>>>>>>>
			
		case enUnRegisterUser:
			{
				cmdSerialize(cmdUnRegisterUser);
			}
			break;


		case enRegisterUser:
			{
				cmdSerialize(cmdRegisterUser);
			}
			break;

		case enDelRoom:
			{
				cmdSerialize(cmdDelRoom);
			}
			break;
			
		case enRegisterSvr:
			{
				cmdSerialize(cmdRegisterSvr);
			}
			break;

		case enRemoveRoomUser:
			{
				cmdSerialize(cmdRemoveRoomUser);
			}
			break;

		case enNotifySomeEnterRoom:
			{
				cmdSerialize(cmdNotifySomeEnterRoom);
			}
			break;

		case enNotifyRandChat:
			{
				cmdSerialize(cmdNotifyRandChat);
			}
			break;

		case enReturnRandChat:
			{
				cmdSerialize(cmdReturnRandChat);
			}
			break;

		case enNotifySomeLeaveRoom:
			{
				cmdSerialize(cmdNotifySomeLeaveRoom);
			}
			break;

		case enNotifyUpdatePower:
			{
				cmdSerialize(cmdNotifyUpdatePower);
			}
			break;
		case enNotifyRandPlay:
			{
				cmdSerialize(cmdNotifyRandPlay);
			}
			break;

		case enNotifyAssignResult:
			{
				cmdSerialize(cmdNotifyAssignResult);
			}
			break;
		case enNotifyUpdatePowerResult:
			{
				cmdSerialize(cmdNotifyUpdatePowerResult);
			}
			break;
		default:
			{
				Global::logger->fatal("[serial_protobuf] not register serialize <cmdid=%u>", id);
				stAssert(0);
				break;
			}
				
		}
	}
};
