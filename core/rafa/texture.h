#ifndef TEXTURE_H
#define TEXTURE_H
#pragma once

#include "header.h"

using namespace std;

class Texture
{
public:

    unsigned char* data;
    unsigned int texture;
    int width, height, nrChannels;

    // constructor reads and builds the shader
    Texture(const char* pathToImage);
    // use/activate the shader
    unsigned int generateTexture();
    unsigned int generateTexture(bool dontRepeat);

};


#endif
