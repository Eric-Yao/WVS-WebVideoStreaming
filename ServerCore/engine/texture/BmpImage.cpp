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
#include "BmpImage.h"

/*
* Constructor, load bmp file and setup class info.
*/
BmpImage::BmpImage(std::string filename)
{
    unsigned int uival;
    unsigned short usval;
    unsigned int size;
    
    bitmap = NULL;
    std::ifstream in(filename.c_str(), std::ios::binary );
    if (!in)
    {
        std::cerr << "File not found: " << filename << std::endl;
        throw std::string("BmpImage: File not found.");
    }
    
    /* Move to Width/Height position */
    in.seekg( 18 );
    in.read( (char *) &uival, 4);
    //cout << "Loading texture: " << filename << endl;
    //cout << "width = " << uival << endl;
    w = uival;
    in.read( (char *) &uival, 4);
    //cout << "height = " << uival << endl;
    h = uival;

    /* Allocate buffer, assumed we are in 24bits */
    size = w * h * 3;
    bitmap = new unsigned char[ size ];

    /* read plane number */
    in.read((char *) &usval, 2);
    //cout << "plane = " << usval << endl;
    if (usval != 1)
        throw std::string("BmpImage: Invalid plane number.");

    /* read bpp number */
    in.read((char *) &usval, 2);
    //cout << "bpp = " << usval << endl;
    if (usval != 24)
        throw std::string("BmpImage: Invalid BPP file.");
    
    /* Skip end of header */
    in.seekg( 24, std::ios::cur );

    /* Read values */
    in.read((char *) bitmap , size);
    
    for (int i=0;i<size;i+=3) 
    { // reverse all of the colors. (bgr -> rgb)
	    int temp = bitmap[i];
    	bitmap[i] = bitmap[i+2];
	    bitmap[i+2] = temp;
    }
}

int BmpImage::getWidth()
{
    return w;
}

int BmpImage::getHeight()
{
    return h;
}

unsigned char* BmpImage::getBitmap()
{
    return bitmap;
}

/*
* Destructor, should free bitmap memory.
*/
BmpImage::~BmpImage()
{
    if(bitmap != NULL)
    {
         delete []bitmap;
    }
}

