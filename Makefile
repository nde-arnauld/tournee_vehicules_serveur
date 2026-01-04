IDIR = include
ODIR = obj
SDIR = src

CXX = g++
CXXFLAGS = -Wall -std=c++17 -g -I$(IDIR)
LDFLAGS =

SRCS = $(wildcard $(SDIR)/*.cpp)
OBJS = $(SRCS:$(SDIR)/%.cpp=$(ODIR)/%.o)

ifeq ($(OS), Windows_NT)
    SHELL := cmd
    LDFLAGS += -lws2_32
    TARGET = lancer_serveur.exe
    MKDIR = if not exist $(ODIR) mkdir $(ODIR)
    # Commande pour supprimer (convertit les / en \ pour 'del')
    FIX_PATH = $(subst /,\,$1)
    RM = del /Q /F
else
    TARGET = lancer_serveur
    MKDIR = mkdir -p $(ODIR)
    RM = rm -f
    FIX_PATH = $1
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(ODIR)/%.o: $(SDIR)/%.cpp
	@$(MKDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(call FIX_PATH,$(OBJS)) $(call FIX_PATH,$(TARGET))

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run