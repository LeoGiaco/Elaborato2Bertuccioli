#pragma once

#include <iostream>
#include "../../core/lib.h"

unsigned int loadTexture1D(string path, int flip);
unsigned int loadTexture2D(string path, int flip);
unsigned int loadCubemap(vector<std::string> faces, int flip);