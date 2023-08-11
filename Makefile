# Compiler settings
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra

# Directories
INCLUDE_DIR := libs/fmt/include
LIB_DIR := libs/fmt/

# Libraries
LIBS := -lssl -lcrypto -lboost_system -lpthread -lfmt

# Source files
SRC := main3.cpp Order_Update.cpp Position.cpp

# Object files
OBJ := $(SRC:.cpp=.o)

# Targets
TARGET := main3

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -L$(LIB_DIR) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
