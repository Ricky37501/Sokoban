#pragma once
class PictureFile
{
public:
	PictureFile(const char* filename);
	~PictureFile();
	unsigned getUnsigned(const char* p) const;
	char* getData()const;

private:
	int mSize;
	char* mData;
};

