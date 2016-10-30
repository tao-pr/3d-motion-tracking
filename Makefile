# Compiler
CC :=g++ -std=c++11

# Where and what to include dependencies
LDFLAGS   := 
CFLAGS    := -L/usr/local/opt/opencv3/lib
INCLUDE   := -I/usr/local/opt/opencv3/include -I include
LIB       := -lopencv_core -lopencv_imgproc -lopencv_video \
						-lopencv_videoio -lopencv_tracking -lopencv_optflow \
						-lopencv_features2d -lopencv_ml -lopencv_highgui \
						-lopencv_imgcodecs -lopencv_objdetect

# Take all CPP source files
SRC_DIR   := src
BUILD_DIR := build
SOURCES   := $(shell find $(SRC_DIR) -name "*.cpp")
OBJS      := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SOURCES:.cpp=.o))
TARGET    := bin/track

#$(CC) $(INCLUDE) $(LDFLAGS) $(CFLAGS) $(LIB) -o $(TARGET) $(OBJS)

all:$(TARGET)

$(TARGET): $(OBJS)
	@echo "••••••••••••••••••••••••••••••"
	@echo "Link Target : $(OBJS)"
	@echo "••••••••••••••••••••••••••••••"
	@mkdir -p bin/
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIB) $^ -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "••••••••••••••••••••••••••••••"
	@echo "Build : $@"
	@echo "••••••••••••••••••••••••••••••"
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) $(LDFLAGS) -c -o $@ $<

clean:
	rm -r $(BUILD_DIR)/*.o $(TARGET)
	@make