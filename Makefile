# Compiler
CC :=g++ -std=c++11

# Where and what to include dependencies
LDFLAGS      := -L/usr/local/opt/opencv3/lib
INCLUDE      := -I/usr/local/opt/opencv3/include -I include
INCLUDE_TEST := -I/usr/local/opt/opencv3/include -I include test
LIB          := -lopencv_core -lopencv_imgproc -lopencv_video \
						    -lopencv_videoio -lopencv_tracking -lopencv_optflow \
						    -lopencv_features2d -lopencv_ml -lopencv_highgui \
						    -lopencv_imgcodecs -lopencv_objdetect

# Take all CPP source files
BUILD_DIR     := build
BUILDTEST_DIR := buildtest
SRC_DIR       := src
TEST_DIR      := test
SOURCES       := $(shell find $(SRC_DIR) -name "*.cpp")
TESTSOURCES   := $(shell find $(TEST_DIR) -name "*.cpp")
OBJS          := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SOURCES:.cpp=.o))
OBJS_FOR_TEST := $(shell find $(BUILD_DIR) -name "*.o" | sed '/.*main.o/d')
TESTOBJS      := $(patsubst $(TEST_DIR)/%,$(BUILDTEST_DIR)/%,$(TESTSOURCES:.cpp=.o))
TARGET        := bin/track
TEST          := bin/test

#$(CC) $(INCLUDE) $(LDFLAGS) $(LIB) -o $(TARGET) $(OBJS)

all:$(TARGET)

test:$(TEST)

$(TEST): $(TESTOBJS) $(OBJS_FOR_TEST)
	@echo "••••••••••••••••••••••••••••••"
	@echo "Link Test : $(TESTOBJS)"
	@echo "••••••••••••••••••••••••••••••"
	$(CC) $(LDFLAGS) $(LIB) $^ -o $(TEST)

$(TARGET): $(OBJS)
	@echo "••••••••••••••••••••••••••••••"
	@echo "Link Target : $(OBJS)"
	@echo "••••••••••••••••••••••••••••••"
	@mkdir -p bin/
	$(CC) $(LDFLAGS) $(LIB) $^ -o $(TARGET)

$(BUILDTEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	@echo "••••••••••••••••••••••••••••••"
	@echo "Build test : $@"
	@echo "••••••••••••••••••••••••••••••"
	@mkdir -p $(BUILDTEST_DIR)
	$(CC) $(INCLUDE) -c -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "••••••••••••••••••••••••••••••"
	@echo "Build : $@"
	@echo "••••••••••••••••••••••••••••••"
	@mkdir -p $(BUILD_DIR)
	$(CC) $(INCLUDE) -c -o $@ $<

clean:
	rm -r $(BUILD_DIR)/*.o $(TARGET)
	rm -r $(BUILDTEST_DIR)/*.o $(TEST)
	@make
	@make test