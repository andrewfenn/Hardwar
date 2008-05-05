APP=./bin/hardwar
SF= ./src/GUIManager.o ./src/InputManager.o ./src/GameManager.o ./src/GameState.o \
	 ./src/WorldManager.o	\
	 ./src/EditorState.o \
	 ./src/Main.o \

CC=g++
LINKER=gcc
WFLAGS=-Wall -Wuninitialized -Wwrite-strings -Wcast-qual  -Wcomment -Wcast-align -Wno-deprecated
OGRE_CFLAGS = `pkg-config OGRE --cflags` `pkg-config CEGUI-OGRE --cflags`
OGRE_LIBS = `pkg-config OGRE --libs` `pkg-config CEGUI-OGRE --libs`
CEGUI_CFLAGS = `pkg-config CEGUI --cflags`
CEGUI_LIBS = `pkg-config CEGUI --libs` 
OIS_CFLAGS = `pkg-config OIS --cflags`
OIS_LIBS = `pkg-config OIS --libs`
SQLITE3_CFLAGS = -I/usr/include
SQLITE3_LIBS = `pkg-config sqlite3 --libs`
LIBS = -lalut -lopenal -lopenal -lalut  -logg -lvorbis -lvorbisfile -lvorbisenc -lboost_filesystem
COPTS=$(WFLAGS) -g -march=i686 -O -pthread -pipe -DFX -D_GNU_SOURCE -DINVERSEXY $(OGRE_CFLAGS) $(CEGUI_CFLAGS) $(OIS_CFLAGS) $(SQLITE_CFLAGS) -I./include -I./usr/include/boost/
LINKOPTS=-rdynamic -L. -lstdc++ -lm -lz -ldl $(OIS_LIBS) $(CEGUI_LIBS) $(OGRE_LIBS) $(LIBS) $(SQLITE3_LIBS)
all: $(APP)

$(APP): $(SF)
	$(LINKER) $(COPTS) $(OBJS) $^ $(LINKOPTS) -o $@

clean:
	rm -f $(SF) $(APP)

%.o: %.cpp
	$(CC) -c $(COPTS) $< -o $@

.obj/%.o: ../src/%.cpp ../include/%.h
	mkdir -p .obj
	$(CC) $(COPTS) -c $< -o $@

