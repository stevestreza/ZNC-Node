CC=g++
INCLUDES = -I. -Ideps/node.js/src -Ideps/znc -Ideps/node.js/deps/libev -Ideps/node.js/deps/libeio
LIBS = -L. -Ldeps/node.js/build/default/ -lnode -lv8 -lssl -lcrypto -lz -ldl $(shell /usr/local/bin/znc-config --libs) deps/znc/*.o
CFLAGS = -fPIC $(INCLUDES) -DFUCKPITTSBURGH $(shell /usr/local/bin/znc-config --cflags)
LDFLAGS = -dynamiclib -cclib -lstdc++ $(shell /usr/local/bin/znc-config --libznc)
MODLINK = $(shell /usr/local/bin/znc-config --modlink)

SRC = src/ZNCNode.cpp
OBJ = obj/ZNCNode.o

all: zncnode.so

zncnode.so: setup $(OBJ)
	$(CC) $(LDFLAGS) $(LIBS) -o $@ $(OBJ)

obj/%.o: src/%.cpp
	$(CC) -c -o $@ src/$*.cpp ${MODLINK} $(CFLAGS)

setup:
	mkdir -p obj

clean:
	rm -r obj

test: clean all