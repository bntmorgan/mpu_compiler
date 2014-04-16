sp              := $(sp).x
dirstack_$(sp)  := $(d)
d               := $(dir)

TARGET					:= $(call SRC_2_BIN, $(d)/mpu_assembler.elf)
TARGETS 				+= $(TARGET)
OBJS_$(d)				:= $(call SRC_2_OBJ, $(d)/main.o $(d)/lexer.o $(d)/parser.o \
	$(d)/sem.o $(d)/opt.o)

# Extra dependencies
$(call SRC_2_OBJ, $(d)/main.o): $(call SRC_2_OBJ, $(d)/lexer.o) 
$(call SRC_2_OBJ, $(d)/lexer.o): $(call SRC_2_OBJ, $(d)/parser.o) 

OBJECTS 				+= $(OBJS_$(d))

$(OBJS_$(d))		:  OBJ_CFLAGS	:= -I$(d) -I$(call SRC_2_OBJ, $(d)) \
	-Isources/includes

$(TARGET)				:  TARGET_LDFLAGS	:= -ly -lfl
$(TARGET)				:  $(OBJS_$(d)) binary/mpu_lib/mpu_lib.so

d               := $(dirstack_$(sp))
sp              := $(basename $(sp))
