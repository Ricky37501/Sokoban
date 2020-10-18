#include "PictureFile.h"
#include<fstream>
using namespace std;

PictureFile::PictureFile(const char* filename):mSize(0),mData(0)
{
	ifstream in(filename, ifstream::binary);
	in.seekg(0, ifstream::end);
	mSize = static_cast<int>(in.tellg());
	in.seekg(0, ifstream::beg);
	mData = new char[mSize];
	in.read(mData, mSize);
}

PictureFile::~PictureFile()
{
	delete[] mData;
	mData = 0;
}

unsigned PictureFile::getUnsigned(const char* p) const
{
	const unsigned char* temp = reinterpret_cast<const unsigned char*>(p);
	unsigned ret = temp[0];
	ret |= ( temp[1] << 8 );
	ret |= ( temp[2] << 16 );
	ret |= ( temp[3] << 24 );

	return ret;
}

char * PictureFile::getData() const
{
	return mData;
}


