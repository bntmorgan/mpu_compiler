sp              := $(sp).x
dirstack_$(sp)  := $(d)
d               := $(dir)

TARGET					:= $(call SRC_2_BIN, $(d)/mpu_disassembler.elf)
TARGETS 				+= $(TARGET)
OBJS_$(d)				:= $(call SRC_2_OBJ, $(d)/main.o $(d)/opt.o)

OBJECTS 				+= $(OBJS_$(d))

$(OBJS_$(d))		:  OBJ_CFLAGS	:= -I$(d) -Isources/includes

$(TARGET)				:  TARGET_LDFLAGS	:=
$(TARGET)				:  $(OBJS_$(d)) binary/mpu_lib/mpu_lib.so

d               := $(dirstack_$(sp))
sp              := $(basename $(sp))
