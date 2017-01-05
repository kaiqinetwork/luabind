APP_STL := gnustl_static
APP_CPPFLAGS := -frtti -fexceptions -std=c++11
APP_MODULES := luabind_static
APP_LDFLAGS := -latomic


ifeq ($(NDK_DEBUG),1)
  APP_OPTIM := debug
else
  APP_CPPFLAGS += -DNDEBUG
  APP_OPTIM := release
endif