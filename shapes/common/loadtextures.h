#pragma once

#include <iostream>
#include "../../core/lib.h"

unsigned int loadTexture(char const *path, int flip);
unsigned int loadCubemap(vector<std::string> faces, int flip);