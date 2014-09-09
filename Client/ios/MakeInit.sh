#!/bin/bash
#MakeInit

protoc --cpp_out=RTChat/Classes/proto --proto_path=../../Public/ ../../Public/game.proto
protoc --cpp_out=RTChat/Classes/proto --proto_path=../../Public/ ../../Public/public.proto

cp ./RTChat/Classes/RTChatCommonTypes.h ./IosHelper/
cp ./RTChat/Classes/interface/RTChatSdk.h ./IosHelper/

cp ../../../RTChatIOSDemo/RTChatIOSDemo/RTChat/RTChatSdkIosHelper.h ./IosHelper/
cp ../../../RTChatIOSDemo/RTChatIOSDemo/RTChat/RTChatSdkIosHelper.mm ./IosHelper/


cp ../../../flappyball/flappyball/Classes/RTChat/RTChatSdkCocos2dxHelper.cpp ./IosHelper/
cp ../../../flappyball/flappyball/Classes/RTChat/RTChatSdkCocos2dxHelper.h ./IosHelper/
