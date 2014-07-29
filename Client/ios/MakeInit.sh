#!/bin/bash
#MakeInit

protoc --cpp_out=RTChat/Classes/proto --proto_path=../../Public/ ../../Public/game.proto
