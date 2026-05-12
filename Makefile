# ---------------------------------------------------------------
# Makefile — Hybrid Inventory Manager
# C files compiled as C, C++ files compiled as C++, linked together.
# ---------------------------------------------------------------

CXX      := g++
CC       := gcc
CFLAGS   := -Wall -Wextra -std=c11   -Iinclude
CXXFLAGS := -Wall -Wextra -std=c++17 -Iinclude
TARGET   := inventory

# Source files
C_SRCS   := src/inventory.c
CXX_SRCS := src/main.cpp

# Object files
C_OBJS   := $(C_SRCS:.c=.o)
CXX_OBJS := $(CXX_SRCS:.cpp=.o)

# ---------------------------------------------------------------
all: $(TARGET)

$(TARGET): $(C_OBJS) $(CXX_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f src/*.o $(TARGET) inventory.dat

.PHONY: all clean
