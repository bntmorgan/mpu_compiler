sp              := $(sp).x
dirstack_$(sp)  := $(d)
d               := $(dir)

TARGET					:= $(call SRC_2_BIN, $(d)/mpu_lib.so)
TARGETS 				+= $(TARGET)
OBJS_$(d)				:= $(call SRC_2_OBJ, $(d)/mpu_lib.o)

OBJECTS 				+= $(OBJS_$(d))

$(OBJS_$(d))		:  OBJ_CFLAGS	:= -Isources/includes -fPIC

$(TARGET)				:  TARGET_LDFLAGS	:=
$(TARGET)				:  $(OBJS_$(d))

d               := $(dirstack_$(sp))
sp              := $(basename $(sp))
