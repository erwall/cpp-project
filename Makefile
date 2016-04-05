INC = inc/
LIBSRC = src/lib/
APPSRC = src/app/
LIBOBJ = obj/lib/
APPOBJ = obj/app/
BIN = bin/
LIB = libproject.a

LIBOBJS = $(patsubst $(LIBSRC)%.cc, $(LIBOBJ)%.o, $(wildcard $(LIBSRC)*.cc))
APPOBJS = $(patsubst $(APPSRC)%.cc, $(APPOBJ)%.o, $(wildcard $(APPSRC)*.cc))
OBJS = $(LIBOBJS) $(APPOBJS)
APPS = $(addprefix $(BIN), myserver)

CC = g++

CPPFLAGS = -std=c++11 -g -I$(INC)
#CXXFLAGS = -Wall -Wextra -pedantic-errors -Wold-style-cast
LDFLAGS = -L.
LDLIBS = -lproject

.PHONY : all clean
.PRECIOUS : $(APPOBJ)%.o

all : $(APPS)

$(BIN)% : $(APPOBJ)%.o $(LIB)
	$(CC) $(LDFLAGS) $(LDLIBS) $^ -o $@

$(LIB) : $(LIBOBJS)
	ar cr $(LIB) $^

$(APPOBJ)%.o : $(APPSRC)%.cc
	$(CC) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

$(LIBOBJ)%.o : $(LIBSRC)%.cc
	$(CC) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

clean :
	rm -f $(BIN)* $(LIBOBJ)* $(APPOBJ)* $(LIB)
