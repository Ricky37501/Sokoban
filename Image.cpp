#include "Image.h"
#include "PictureFile.h"
#include "State.h"

Image::Image(const char* filename):mFile(PictureFile(filename)){
	char* file = mFile.getData();
	mImageHeight = mFile.getUnsigned(&file[12]);
	mImageWidth = mFile.getUnsigned(&file[16]);
	mImageData = new unsigned[mImageHeight*mImageWidth];
	for (int i = 0; i < mImageHeight*mImageWidth;i++) {
		mImageData[i] = mFile.getUnsigned(&file[128 + 4 * i]);
	}
}

Image::~Image()
{
	delete[] mImageData;
	mImageData = 0;
}

int Image::getWidth()const
{
	return mImageWidth;
}

int Image::getHeight()const
{
	return mImageHeight;
}

const unsigned * Image::getImageData()const
{
	return mImageData;
}
