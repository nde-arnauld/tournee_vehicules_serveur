CXX = g++

CXXFLAGS = -Wall -std=c++17 -g

# -lws2_32 : sockets sous Windows
LDFLAGS = -lws2_32

TARGET = lancer_serveur.exe

SRCS = a_element.cpp \
       distance.cpp \
       evaluateur.cpp \
       foncteur.cpp \
       main_serveur.cpp \
       serveur.cpp \
       ville.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /Q $(OBJS) $(TARGET)

run: $(TARGET)
	.\$(TARGET)