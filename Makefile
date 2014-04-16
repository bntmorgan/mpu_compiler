################################## FUNCTIONS ###################################

define SRC_2_OBJ
  $(foreach src,$(1),$(patsubst sources/%,build/%,$(src)))
endef

define SRC_2_BIN
  $(foreach src,$(1),$(patsubst sources/%,binary/%,$(src)))
endef

################################## STARTING RULE ###############################

all: targets

################################## GLOBALS  ####################################

CC=gcc
CFLAGS=-Wall -g -Werror
YFLAGS=-v -d
LD=gcc
LEX=lex
YACC=yacc

################################## INCLUDES ####################################

# Overriden in rules.mk
TARGETS :=
OBJECTS :=

dir	:= sources
include	$(dir)/rules.mk

################################## RULES #######################################

targets: $(TARGETS)

%.elf:
	@mkdir -p $(dir $@)
	@echo [LD] $@
	@$(LD) -o $@ $^ $(TARGET_LDFLAGS)

%.so:
	@mkdir -p $(dir $@)
	@echo [LD] $@
	@$(LD) -shared -o $@ $^ $(TARGET_LDFLAGS)

build/%.o: build/%.c
	@mkdir -p $(dir $@)
	@echo [CC] $@
	@$(CC) -o $@ -c $< $(CFLAGS) $(OBJ_CFLAGS)

build/%.c: sources/%.l
	@mkdir -p $(dir $@)
	@echo [LX] $@
	@$(LEX) -o $@ $<

build/%.c: sources/%.y
	@mkdir -p $(dir $@)
	@echo [YC] $@
	@$(YACC) $(YFLAGS) -o $@ $<

build/%.o: sources/%.c
	@mkdir -p $(dir $@)
	@echo [CC] $@
	@$(CC) -o $@ -c $< $(CFLAGS) $(OBJ_CFLAGS)

info:
	@echo TARGETS [$(TARGETS)]
	@echo OBJECTS [$(OBJECTS)]

clean:
	@echo [CLR] $(TARGETS)
	@echo [CLR] $(OBJECTS)
	@rm -fr $(dir $(TARGETS)) $(OBJECTS)

mr-proper: mr-proper-vim clean

mr-proper-vim:
	@echo [CLR] *.swp
	@find . | grep .swp | xargs rm -f
