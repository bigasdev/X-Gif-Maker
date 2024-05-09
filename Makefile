# set the App name
NAME = twitter-gif-maker
# set compiler
CC = g++ -std=c++17
# set all the files needed
#CPP = .\src\Entity/*.cpp .\src\Scenes/*.cpp
# imgui dependencies
BIN = bin/*.o
DEBUG_FLAGS = -DF_ENABLE_ASSERTS -DF_ENABLE_DEBUG

bin_dir:
	mkdir bin

imgui_o: $(patsubst src/ImGui/%.cpp,bin/%.o,$(wildcard src/ImGui/*.cpp))
app_o: $(patsubst src/Core/%.cpp,bin/%.o,$(wildcard src/Core/*.cpp))
entity_o : $(patsubst src/Entity/%.cpp,bin/%.o,$(wildcard src/Entity/*.cpp))
renderer_o : $(patsubst src/Renderer/%.cpp,bin/%.o,$(wildcard src/Renderer/*.cpp))
resources_o : $(patsubst src/Resources/%.cpp,bin/%.o,$(wildcard src/Resources/*.cpp))
scenes_o : $(patsubst src/Scenes/%.cpp,bin/%.o,$(wildcard src/Scenes/*.cpp))
tools_o : $(patsubst src/Tools/%.cpp,bin/%.o,$(wildcard src/Tools/*.cpp))
utils_o : $(patsubst src/Utils/%.cpp,bin/%.o,$(wildcard src/Utils/*.cpp))

# Rule to build all .cpp files in the src/ImGui folder
bin/%.o: src/ImGui/%.cpp
	$(CC) $(if $(filter true,$(DEBUG)),-g $(DEBUG_FLAGS)) -Iinclude -Iinclude/sdl -Iinclude/headers -Llib -c $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $< -o $@

bin/%.o: src/Core/%.cpp
	$(CC) $(if $(filter true,$(DEBUG)),-g $(DEBUG_FLAGS)) -Iinclude -Iinclude/sdl -Iinclude/headers -Llib -c $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $< -o $@

bin/%.o: src/Entity/%.cpp
	$(CC) $(if $(filter true,$(DEBUG)),-g $(DEBUG_FLAGS)) -Iinclude -Iinclude/sdl -Iinclude/headers -Llib -c $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $< -o $@

bin/%.o: src/Renderer/%.cpp
	$(CC) $(if $(filter true,$(DEBUG)),-g $(DEBUG_FLAGS)) -Iinclude -Iinclude/sdl -Iinclude/headers -Llib -c $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $< -o $@

bin/%.o: src/Resources/%.cpp
	$(CC) $(if $(filter true,$(DEBUG)),-g $(DEBUG_FLAGS)) -Iinclude -Iinclude/sdl -Iinclude/headers -Llib -c $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $< -o $@

bin/%.o: src/Scenes/%.cpp
	$(CC) $(if $(filter true,$(DEBUG)),-g $(DEBUG_FLAGS)) -Iinclude -Iinclude/sdl -Iinclude/headers -Llib -c $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $< -o $@

bin/%.o: src/Tools/%.cpp
	$(CC) $(if $(filter true,$(DEBUG)),-g $(DEBUG_FLAGS)) -Iinclude -Iinclude/sdl -Iinclude/headers -Llib -c $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $< -o $@

bin/%.o: src/Utils/%.cpp
	$(CC) $(if $(filter true,$(DEBUG)),-g $(DEBUG_FLAGS)) -Iinclude -Iinclude/sdl -Iinclude/headers -Llib -c $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $< -o $@

debug: imgui_o app_o entity_o renderer_o resources_o scenes_o tools_o utils_o
	${CC} -g -Wall -static-libstdc++ -static-libgcc -Iinclude -Iinclude/sdl -Iinclude/headers -Llib -o .build/${NAME}_debug.exe ${BIN} res/icon/icon.res -lstdc++fs -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lole32 -mwindows -mconsole

build: imgui_o app_o entity_o renderer_o resources_o scenes_o tools_o utils_o
	${CC} -s -finline-functions -flto -static-libstdc++ -static-libgcc -Iinclude -Iinclude/sdl -Iinclude/headers -Llib -o .release/${NAME}.exe ${BIN} res/icon/icon.res -lstdc++fs -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lole32 -mwindows -O2

compile: bin_dir imgui_o app_o entity_o renderer_o resources_o scenes_o tools_o utils_o
	${CC} -Wall -static-libstdc++ -static-libgcc -Iinclude -Iinclude/sdl -Iinclude/headers -Llib -o ${NAME}_debug ${BIN} res/icon/icon.res -lstdc++fs -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lole32 -mwindows -mconsole