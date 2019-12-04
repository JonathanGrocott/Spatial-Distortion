#Majority of makefile copied from https://spin.atomicobject.com/2016/08/26/makefile-c-projects/
TARGET_EXEC ?= spatial

CC = g++
BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
LDLIBS := -lboost_system -lboost_filesystem

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -std=c++11


$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CP_AR) Data ./build
	$(CC) $(OBJS) -o $@ $(LDLIBS) $(CPPFLAGS)


# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDLIBS) -c -g $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

CP_AR ?= cp -ar
MKDIR_P ?= mkdir -p