# 通用 Makefile

# $(info "######## Makefile #######")
 
# PROJECT_ROOT_DIR := $(shell git worktree list | cut -d \  -f1)
# PROJECT_ROOT_DIR := $(shell dirname `git rev-parse --git-dir`)

# 其他变量
OPTIMIZE :=
WARNINGS := -Wall -Wno-unused -Wno-format
DEFS     := -DDEBUG -DUSE_SHARED_PTR -DSIM_SUITE

CURRENT_DIR := $(shell pwd)

CLIPS_DIR := /workspace/clips/learn/clips_core_source_630/core
JSON_DIR  := /data/source/rapidjson

UTILS_DIR := ../src/utils
ENGINE_DIR:= ../src/rule-engine
CLIPS_LIB := $(CLIPS_DIR)

CLIPSCPP_DIR:= $(ENGINE_DIR)/Clipscpp
DRIVER_DIR  := $(ENGINE_DIR)/RuleDriver
PAYLOAD_DIR := $(ENGINE_DIR)/Payload
MISC_DIR 	:= $(UTILS_DIR)/Misc
MESSAGE_DIR := $(UTILS_DIR)/Message
LOG_DIR 	:= $(UTILS_DIR)/Log
SQLITE_DIR  := $(UTILS_DIR)/SQLite

# 初始化编译工具以及编译选项
CROSS_COMPILE =
GCC 	:= $(CROSS_COMPILE)gcc
CXX 	:= $(CROSS_COMPILE)g++
CC		:=
AR		:= $(CROSS_COMPILE)ar
CFLAGS  := $(OPTIMIZE) $(WARNINGS) $(DEFS)
CPPFLAGS:= -std=c++11 -lRE_engine -lRE_driver -lrt -lClipscpp -lRE_payload -lclips -lUtils_sqlite -lUtils_log -lUtils_message -lUtils_misc -lpthread  -lsqlite3
LDFLAGS := -L$(ENGINE_DIR)/output -L$(DRIVER_DIR)/output -L$(CLIPSCPP_DIR)/output -L$(PAYLOAD_DIR)/output -L$(CLIPS_LIB) -L$(MISC_DIR)/output -L$(MESSAGE_DIR)/output -L$(LOG_DIR)/output -L$(SQLITE_DIR)/output
INCLUDE := -I$(JSON_DIR)/include -I$(CLIPSCPP_DIR)/src -I$(MISC_DIR)/src -I$(MESSAGE_DIR)/src -I$(LOG_DIR)/src -I$(SQLITE_DIR)/src -I$(PAYLOAD_DIR)/src -I$(ENGINE_DIR) -I$(DRIVER_DIR)/src -I$(DRIVER_DIR)/src/tables

# 源文件可能的后缀
SRCEXTS := c C cc cpp CPP c++ cxx cp
HDREXTS := h H hh hpp HPP h++ hxx hp

# 指定源文件目录, 以及目标文件生成目录
SRC_DIR = . src
INC_DIR = . src
OUT_DIR = output
OBJ_DIR = $(OUT_DIR)/obj

# 指定运行环境(-Wl,-rpath, 多个使用:)
RPATH 	= ".:$(OUT_DIR)"

# 额外增加的源文件或者排除不编译的源文件
SPECIAL_SRC :=
EXCLUDE_SRC := %UnitTest.cpp

# 设置目标类型(exe, a, so), 及目标名字
TARGET_TYPE := exe
TARGET_TYPE := $(strip $(TARGET_TYPE))
TARGET_NAME := re

ifeq ($(TARGET_NAME), )
	TARGET_NAME := $(shell basename $(PWD))
endif

ifneq ($(TARGET_TYPE), exe)
	TARGET_NAME := $(join $(OUT_DIR)/lib, $(TARGET_NAME))
	TARGET_NAME := $(join $(TARGET_NAME), .$(TARGET_TYPE))
else
	TARGET_NAME := $(join $(OUT_DIR), /$(TARGET_NAME))
	LDFLAGS += -L$(OUT_DIR)
endif

# 追加搜索目录
SRC_DIR	+= $(dir $(SPECIAL_SRC))
INC_DIR += $(SRC_DIR)

# 源文件, 头文件, 链接文件
SOURCES = $(filter-out $(EXCLUDE_SRC), $(foreach d, $(SRC_DIR), $(wildcard $(addprefix $(d)/*., $(SRCEXTS)))))
HEADERS = $(foreach d, $(INC_DIR), $(wildcard $(addprefix $(d)/*., $(HDREXTS))))
OBJECTS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))

# 配置编译选项
INCLUDE += $(foreach d, $(sort $(INC_DIR)), -I$d)
ifeq ($(TARGET_TYPE), so)
	CFLAGS  += -fpic -shared
	LDFLAGS += -shared
endif

# 定义伪目标
PHONY = all .mkdir clean

all: .dep .mkdir $(TARGET_NAME)

# 函数: 添加%.x依赖文件的路径
define add_vpath
$1

endef

# 函数: 生成中间文件
define gen_o_cmd
$2/%.o: %.$1
ifeq ($3 , $(GCC))
	$(3) $(INCLUDE) -Wp,-MT,$$@ -Wp,-MMD,$$@.d $(CFLAGS) -c -o $$@ $$<
else
	$(3) $(INCLUDE) -Wp,-MT,$$@ -Wp,-MMD,$$@.d $(CFLAGS) $(CPPFLAGS) -c -o $$@ $$<
endif

endef

# 执行函数, make会将函数里的内容当Makefile解析, 注意$$符号
$(eval $(foreach i, $(SRCEXTS), $(foreach d, $(SRC_DIR), $(call add_vpath,vpath %.$i $d))))
$(eval $(foreach i, $(SRCEXTS), $(call gen_o_cmd,$i,$(OBJ_DIR),$(if $(filter-out c C, $(i)),$(CXX),$(GCC)))))

# 静态, 动态, 可执行
ifeq ($(TARGET_TYPE), a)
$(TARGET_NAME): .mkdir $(OBJECTS)
	rm -f $@
	$(AR) rcvs $@ $(OBJECTS)
else
$(TARGET_NAME): LD=$(if $(filter-out %.c %.C, $(SOURCES)),$(CXX),$(GCC))
$(TARGET_NAME): .mkdir $(OBJECTS)
	$(LD) $(OBJECTS) $(CPPFLAGS) -o $@ $(LDFLAGS) -Wl,-rpath=$(RPATH)
endif

.dep:
	@cd ../src/rule-engine; make;

.mkdir:
	@if [ ! -d $(OBJ_DIR) ]; then mkdir -p $(OBJ_DIR); fi

clean:
	@cd ../src/rule-engine; make clean;
	rm -rf $(OUT_DIR)

run:$(TARGET_NAME)
	$(TARGET_NAME)


.PHONY: $(PHONY)
