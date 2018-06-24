/*****************************************************************************
*
* File: BmpImage.cpp
* Project: OpenGL test: Torus.exe
*
* Description: This is the Class implementation of the BmpImage object.
* This class is designed to generate handle a few bmp format.
*
* Author: Vedder Bruno
* Date: 15/04/2004
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#ifndef _BMPIMAGE_H
#define _BMPIMAGE_H

#include <iostream>
#include <fstream>

class BmpImage
{
    public:
        BmpImage(std::string file);
        ~BmpImage();
        int getWidth();
        int getHeight();
        unsigned char* getBitmap();
        
    protected:
        int w;
        int h;
        unsigned char *bitmap;
};

#endif
