IDIR = include
ODIR = obj
SDIR = src
SCDIR = script

CXX = g++
CXXFLAGS = -Wall -std=c++17 -g -I$(IDIR)
LDFLAGS =
ifeq ($(OS), Windows_NT)
    LDFLAGS += -lws2_32
endif

TARGET = lancer_serveur

SRCS = $(wildcard $(SDIR)/*.cpp)
OBJS = $(SRCS:$(SDIR)/%.cpp=$(ODIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(ODIR)/%.o: $(SDIR)/%.cpp
	@mkdir -p $(ODIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
