os := $(shell uname -s)
ifeq ($(os),Linux)
	SYSCONF_LINK = g++
endif
ifeq ($(os),Darwin)
	SYSCONF_LINK = clang++
endif
CPPFLAGS     =
LDFLAGS      =
LIBS         = -lm

DESTDIR = ./
TARGET  = main

# OBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

# all: $(DESTDIR)$(TARGET)

# $(DESTDIR)$(TARGET): $(OBJECTS)
# 	$(SYSCONF_LINK) -Wall -std=c++11 -g $(LDFLAGS) -o $(DESTDIR)$(TARGET) $(OBJECTS) $(LIBS)

all:
# $(OBJECTS): %.o: %.cpp
ifeq ($(os),Linux)
	$(SYSCONF_LINK) -Wall -std=c++11 -g -I include src/glad.c src/*.cpp -l glfw $< -o $(TARGET)
endif
ifeq ($(os),Darwin)
	$(SYSCONF_LINK) -Wall -std=c++11 -g -I include -L lib $(CPPFLAGS) src/glad.c src/*.cpp lib/libglfw.3.dylib $< -o $(TARGET)
endif

clean:
	-rm -f $(TARGET)
