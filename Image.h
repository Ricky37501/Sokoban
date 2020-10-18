#pragma once
#include"PictureFile.h"
class Image
{
public:
	Image(const char*);
	~Image();
	int getWidth()const;
	int getHeight()const;
	const unsigned* getImageData()const;

private:
	int mImageWidth;
	int mImageHeight;
	unsigned* mImageData;
	PictureFile mFile;
};

