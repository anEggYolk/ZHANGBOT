CXX = g++
CXXFLAGS = -Wall -Werror
SRCS = main.cpp tests.cpp

# FYI:
# $(patsubst %.cpp,%.o,$(SRCS)) converts .cpp to .o
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

TARGET = chess_bot

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
