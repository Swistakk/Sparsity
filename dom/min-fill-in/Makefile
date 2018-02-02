//DEBUG = -g -fsanitize=address
DEBUG = -O3

LFLAGS = -Wall $(DEBUG)
CVERSION = -std=c++14
BOOST = /usr/local/boost_1_62_0/

INCLUDE_DIR := ./include
SRC_DIR := ./src
BUILD_DIR := ./build
TARGET_DIR := ./bin

HPPS := $(shell find $(INCLUDE_DIR) -name '*.hpp')

APPR_TW_SRCS := apprTW.cpp
APPR_TW_SOBJS:=$(patsubst %.cpp, %.o, $(APPR_TW_SRCS))
APPR_TW_OBJS_PREFIX:=$(patsubst %.cpp, $(BUILD_DIR)/%.o, $(APPR_TW_SRCS))


UNAME := $(shell uname)
 #OSX
ifeq ($(UNAME), Darwin)
LLIB = -L/usr/local/lib/ -lboost_program_options
ILIB = -I/usr/local/include/
CCC = g++
# -stdlib=libc++ 
endif

# LINUX
ifeq ($(UNAME), Linux)
LLIB = -L/usr/local/lib/ -lboost_program_options
ILIB = -I$BOOST
CCC = g++
endif

CFLAGS = -Wall -c $(DEBUG)



SHELL := /bin/bash

.PHONY:  clean cleanall make_dirs make_build_dir make_target_dir

all: apprTW


make_build_dir:
	@if [ ! -d $(BUILD_DIR) ]; then \
		echo -e "\033[33mCreating '$(BUILD_DIR)'.\033[0m";\
		mkdir $(BUILD_DIR); \
	fi

make_target_dir:
	@if [ ! -d $(TARGET_DIR) ]; then \
		echo -e "\033[33mCreating '$(TARGET_DIR)'.\033[0m";\
		mkdir $(TARGET_DIR); \
	fi

make_dirs: make_build_dir make_target_dir 

apprTW: make_dirs $(APPR_TW_OBJS_PREFIX) 
	$(CCC) $(LFLAGS) $(CVERSION) $(APPR_TW_OBJS_PREFIX)  -o $(TARGET_DIR)/apprTW $(LLIB)


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HPPS)
	$(CCC) $(CFLAGS) $(CVERSION) $(ILIB) $< -o $@




clean:
	rm -f $(BUILD_DIR)/*
	rm -f $(SRC_DIR)/*~

cleanall: clean
	rm -f $(TARGET_DIR)/*
