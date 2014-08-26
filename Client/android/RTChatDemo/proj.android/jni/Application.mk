APP_STL := gnustl_static 

APP_CPPFLAGS := -frtti -DCOCOS2D_DEBUG=1 -std=c++11 -Wno-literal-suffix -fsigned-char



APP_DEBUG := $(strip $(NDK_DEBUG))
ifeq ($(APP_DEBUG),1)
  APP_CPPFLAGS += -DCOCOS2D_DEBUG=1
  APP_OPTIM := debug
else
  APP_CPPFLAGS += -DNDEBUG
  APP_OPTIM := release
endif