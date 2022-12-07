#pragma once

#include "lib.h"
#include "3dthings.h"
#include <assimp/Importer.hpp>	// C++ importer interface
#include <assimp/scene.h>		// Output data structure
#include <assimp/postprocess.h> // Post processing flags

bool loadAssImp(
	const char *path, vector<Shape3D> &meshVector);