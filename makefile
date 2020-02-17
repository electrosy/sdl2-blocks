#OBJS defines files to compile as part of the project
OBJS = sdl2-blocks.cpp Video.cpp Input.cpp Sprite.cpp Block.cpp GameModel.cpp Clock.cpp Window.cpp

#OBJ_NAME name of executable
OBJ_NAME = sdl2-blocks

#
all : $(OBJS)
	g++ $(OBJS) -w -lSDL2 -lSDL2_image -o $(OBJ_NAME)
