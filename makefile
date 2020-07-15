##sdl2-blocks
##Copyright (C) 2020 Steven Philley

#OBJS defines files to compile as part of the project
OBJS = sdl2-blocks.cpp Video.cpp Input.cpp Sprite.cpp Block.cpp GameModel.cpp \
		Clock.cpp Winlet.cpp Texture.cpp Renderable.cpp Renderables.cpp \
		GameController.cpp SimpleShape.cpp Timer.cpp Textures.cpp Font.cpp
		

#OBJ_NAME name of executable
OBJ_NAME = sdl2-blocks

#
all : $(OBJS)
	g++ $(OBJS) -w -lSDL2 -lSDL2_image -lSDL2_ttf -o $(OBJ_NAME)
