mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS +=\

APPL_CXXOBJS +=\
Watch.o\
Secone.o\
Sectwo.o\
Walker.o\
LineTracer.o\
 

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION 
 
# Include libraries 
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile 


endif


APPL_LIBS += -lm

APPL_DIRS += $(mkfile_path)app

INCLUDES += \
	-I$(mkfile_path)app\
