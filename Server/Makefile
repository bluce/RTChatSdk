export CompilerTool = /usr/bin/g++
export CompilerOption

export LinkTool = /usr/bin/g++
export LinkOption

CompilerOptionTemp = -c -I../Include -I../ -I../Command -Wall -Werror -Wno-deprecated -I/usr/include/ -I../../../Public \
`xml2-config --cflags` `mysql_config --cflags` `pkg-config --cflags --libs protobuf`
LinkOptionTemp = -Wall -Werror -Wno-deprecated -L../Include -lgd -llog4cxx -lpthread -lrt -lz -ldl -lm `xml2-config --libs` `mysql_config --libs_r` `pkg-config --cflags --libs protobuf`

DebugOption = -ggdb -D_GNU_SOURCE -D_REENTRANT -D_HRX_DEBUG  -D_DEBUG -D_LINUX 
ReleaseOption = -g -O2 -D_GNU_SOURCE -D_REENTRANT -D_LINUX 

export CubeLib = ../Include/rtchatlib.a ../Command/command.a

SubDir = Command LoginService GateService InfoService TransService


.PHONY : debug release clean 

AllDir = $(SubDir) 

AllTarget = outdebug
AllObject: $(AllTarget) 

release: clean upcode makerelease rdir
debug: clean outdebug releasedir

$(AllTarget):
	@for dir in $(AllDir);\
	do\
		CompilerOption='$(DebugOption) $(CompilerOptionTemp)' \
		LinkOption='$(DebugOption) $(LinkOptionTemp)' \
		$(MAKE) -C $$dir || exit 1;\
	done

makerelease:
	@for dir in $(AllDir);\
	do\
		CompilerOption='$(ReleaseOption) $(CompilerOptionTemp)' \
		LinkOption='$(ReleaseOption) $(LinkOptionTemp)' \
		$(MAKE)  -C $$dir || exit 1;\
	done

rdir:
	@rm -rf VoiceServer
	@mkdir -p VoiceServer/
	@mkdir -p VoiceServer//TransService
	@mkdir -p VoiceServer/GateService
	@mkdir -p VoiceServer/InfoService
	@mkdir -p VoiceServer/LoginService
	@mkdir -p VoiceServer/Config
	@cp -af ./stopserver ./VoiceServer
	@cp -af ./restart-dev ./VoiceServer
	@cp -af ./InfoService/InfoService VoiceServer/InfoService
	@cp -af ./TransService/TransService VoiceServer/TransService
	@cp -af ./GateService/GateService VoiceServer/GateService
	@cp -af ./LoginService/LoginService VoiceServer/LoginService

clean:
	@for dir in $(AllDir);\
	do\
		CompilerOption='$(CompilerOptionTemp)' \
		LinkOption='$(LinkOptionTemp)' \
		$(MAKE) clean -C $$dir || exit 1;\
	done
	

