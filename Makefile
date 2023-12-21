
SYSCONF_LINK = /usr/bin/g++
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
	$(SYSCONF_LINK) -Wall -std=c++11 -g -I include -L lib $(CPPFLAGS) src/glad.c src/*.cpp lib/libglfw.3.dylib $< -o $(TARGET)

clean:
	-rm -f $(TARGET)
