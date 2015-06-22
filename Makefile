# Utilities
RM              := rm -rf
CHDIR           := cd
EXEC            := exec

# Test artifacts
TEST_DIR        := tests
TEST_OUT        := $(TEST_DIR)/all
TEST_SRCS       := all.cpp
TEST_DEPS       := $(TEST_SRCS:%.cpp=$(TEST_DIR)/%.d)
TEST_OBJS       := $(TEST_SRCS:%.cpp=$(TEST_DIR)/%.o)

# Example artifacts
EXAMPLE_DIR     := examples
EXAMPLE_SRCS    := types.cpp stack.cpp functions.cpp usertypes.cpp
EXAMPLE_DEPS    := $(EXAMPLE_SRCS:%.cpp=$(EXAMPLE_DIR)/%.d)
EXAMPLE_OBJS    := $(EXAMPLE_SRCS:%.cpp=$(EXAMPLE_DIR)/%.out)

# Compiler
CXX             ?= clang++
CXXFLAGS        += -std=c++14 -O2 -g -DDEBUG -fmessage-length=0 -Wall -Wextra -pedantic -Ilib
LDLIBS          += -llua

# Default targets
all: test examples

clean:
	$(RM) $(EXAMPLE_OBJS) $(EXAMPLE_DEPS) $(TEST_OUT) $(TEST_OBJS) $(TEST_DEPS)

# Tests
test: $(TEST_OUT)
	./$(TEST_OUT)

$(TEST_OUT): $(TEST_OBJS)
	$(CXX) $(LDFLAGS) -o$@ $(TEST_OBJS) $(LDLIBS)

$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp Makefile
	$(CXX) -c $(CXXFLAGS) -MMD -MF$(@:%.o=%.d) -MT$@ -o$@ $<

# Examples
examples: $(EXAMPLE_OBJS)
	@for ex in $(EXAMPLE_OBJS); do echo "> Example '$$ex'"; ./$$ex; done

-include $(EXAMPLE_DEPS)

$(EXAMPLE_DIR)/%.out: $(EXAMPLE_DIR)/%.cpp Makefile
	$(CXX) $(CXXFLAGS) -MMD -MF$(<:%.cpp=%.d) -MT$@ -o$@ $< $(LDLIBS)

# Phony
.PHONY: all clean test examples