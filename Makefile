SHELL=/bin/bash
FLAGS=-g -I ./core -I ./shapes -lglut -lGLU -lGL -lGLEW -lassimp

all: main.out

main.out: ./bin/main.o ./bin/Window.o ./bin/GLProgram.o ./bin/Scene.o ./bin/Camera.o ./bin/Shape3D.o ./bin/HasUniformValues.o ./bin/HasShader.o ./bin/HasCollider.o ./bin/ShapeBase.o ./bin/NoiseFilter.o ./bin/SimpleNoiseFilter.o ./bin/Terrain.o ./bin/Meshes.o ./bin/loadtextures.o
	g++ -o main.out ./bin/*.o ${FLAGS}

./bin/main.o: main.cpp
	g++ -c -o ./bin/main.o main.cpp ${FLAGS}

./bin/Window.o: ./core/Window.cpp ./core/Window.h
	g++ -c -o ./bin/Window.o ./core/Window.cpp ${FLAGS}

./bin/GLProgram.o: ./core/GLProgram.cpp ./core/GLProgram.h
	g++ -c -o ./bin/GLProgram.o ./core/GLProgram.cpp ${FLAGS}

./bin/Scene.o: ./core/Scene.cpp ./core/Scene.h
	g++ -c -o ./bin/Scene.o ./core/Scene.cpp ${FLAGS}

./bin/ShapeBase.o: ./shapes/common/ShapeBase.cpp ./shapes/common/ShapeBase.h
	g++ -c -o ./bin/ShapeBase.o ./shapes/common/ShapeBase.cpp ${FLAGS}

./bin/Shape3D.o: ./shapes/3D/Shape3D.cpp ./shapes/3D/Shape3D.h
	g++ -c -o ./bin/Shape3D.o ./shapes/3D/Shape3D.cpp ${FLAGS}

./bin/Terrain.o: ./shapes/3D/Terrain.cpp ./shapes/3D/Terrain.h
	g++ -c -o ./bin/Terrain.o ./shapes/3D/Terrain.cpp ${FLAGS}

./bin/ComplexShape3D.o: ./shapes/3D/ComplexShape3D.cpp ./shapes/3D/ComplexShape3D.h
	g++ -c -o ./bin/ComplexShape3D.o ./shapes/3D/ComplexShape3D.cpp ${FLAGS}

./bin/HasUniformValues.o: ./core/HasUniformValues.cpp ./core/HasUniformValues.h
	g++ -c -o ./bin/HasUniformValues.o ./core/HasUniformValues.cpp ${FLAGS}

./bin/HasShader.o: ./shapes/common/HasShader.cpp ./shapes/common/HasShader.h
	g++ -c -o ./bin/HasShader.o ./shapes/common/HasShader.cpp ${FLAGS}

./bin/HasCollider.o: ./shapes/common/HasCollider.cpp ./shapes/common/HasCollider.h
	g++ -c -o ./bin/HasCollider.o ./shapes/common/HasCollider.cpp ${FLAGS}

./bin/loadtextures.o: ./shapes/common/loadtextures.cpp ./shapes/common/loadtextures.h
	g++ -c -o ./bin/loadtextures.o ./shapes/common/loadtextures.cpp ${FLAGS}

./bin/Camera.o: ./core/Camera.cpp ./core/Camera.h
	g++ -c -o ./bin/Camera.o ./core/Camera.cpp ${FLAGS}

./bin/NoiseFilter.o: ./noise/NoiseFilter.cpp ./noise/NoiseFilter.h
	g++ -c -o ./bin/NoiseFilter.o ./noise/NoiseFilter.cpp ${FLAGS}

./bin/SimpleNoiseFilter.o: ./noise/SimpleNoiseFilter.cpp ./noise/SimpleNoiseFilter.h
	g++ -c -o ./bin/SimpleNoiseFilter.o ./noise/SimpleNoiseFilter.cpp ${FLAGS}

./bin/Meshes.o: ./core/load_meshes_assimp.cpp ./core/load_meshes_assimp.hpp
	g++ -c -o ./bin/Meshes.o ./core/load_meshes_assimp.cpp ${FLAGS}


.PHONY: clean

clean:
	-rm ./bin/*.o *.out