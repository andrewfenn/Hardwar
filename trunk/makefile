APP=./bin/hardwar
SF= ./src/InputManager.o ./src/GameManager.o ./src/GameState.o \
	 ./src/TerrainManager.o	\
	 ./src/EditorState.o \
	 ./src/Main.o \

CC=g++
LINKER=gcc
WFLAGS=-Wall -Wuninitialized -Wwrite-strings -Wcast-qual  -Wcomment -Wcast-align -Wno-deprecated
OGRE_CFLAGS = -DOGRE_GUI_GLX -DOGRE_CONFIG_LITTLE_ENDIAN -I/usr/local/include -I/usr/local/include/OGRE  
OGRE_LIBS = -L/usr/local/lib -lOgreMain  
OIS_CFLAGS = -I/usr//include/OIS -I/usr//include  
OIS_LIBS = -L/usr//lib -lOIS  
LIBS = -lalut -lopenal -lopenal -lalut  -logg -lvorbis -lvorbisfile -lvorbisenc
COPTS=$(WFLAGS) -g -march=i686 -O -pthread -pipe -DFX -D_GNU_SOURCE -DINVERSEXY $(OGRE_CFLAGS) $(OIS_CFLAGS)  -I./include
LINKOPTS=-rdynamic -L. -lstdc++ -lm -lz -ldl $(OIS_LIBS) $(OGRE_LIBS) $(LIBS)
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

