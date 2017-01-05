LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := luabind_static

LOCAL_MODULE_FILENAME := libluabind

LOCAL_SRC_FILES := \
../../src/class.cpp \
../../src/class_info.cpp \
../../src/class_registry.cpp \
../../src/class_rep.cpp \
../../src/create_class.cpp \
../../src/error.cpp \
../../src/exception_handler.cpp \
../../src/function.cpp \
../../src/inheritance.cpp \
../../src/link_compatibility.cpp \
../../src/object_rep.cpp \
../../src/open.cpp \
../../src/pcall.cpp \
../../src/scope.cpp \
../../src/stack_content_by_name.cpp \
../../src/weak_ref.cpp \
../../src/wrapper_base.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)../..

LOCAL_C_INCLUDES := $(LOCAL_PATH)../..
                    
LOCAL_CPPFLAGS += -fexceptions

LOCAL_WHOLE_STATIC_LIBRARIES := \
    libboost_signals-gcc-mt-1_49 \
    libboost_system-gcc-mt-1_49 \
    libboost_thread-gcc-mt-1_49 \
    libboost_filesystem-gcc-mt-1_49 \
    libboost_date_time-gcc-mt-1_49 \
    libboost_program_options-gcc-mt-1_49 \
    luajit_static

include $(BUILD_STATIC_LIBRARY)

$(call import-add-path, $(KAIQI_PROJECT_ROOT)/include-android)

$(call import-module, boost)
$(call import-module, luajit)