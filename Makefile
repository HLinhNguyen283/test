CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -pedantic

SRCS := main.cpp
OBJS := $(SRCS:.cpp=.o)
TARGET := my_program

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
