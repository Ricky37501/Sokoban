#pragma once
#include "Array2D.h"
#include"Image.h"
//状态类
class State {
public:
	State(const char* stageData, int size,const char* imagefilename);
	void update(char input);
	void draw() const;
	bool hasCleared() const;
	void State::drawWinPicture() const;
private:
	enum Object {
		OBJ_SPACE,
		OBJ_WALL,
		OBJ_BLOCK,
		OBJ_MAN,

		OBJ_UNKNOWN,
	};
	void setSize(const char* stageData, int size);
	//网格绘制函数
	static void drawCell(int x, int y, unsigned color);
	void drawPicture(int x, int y, int index) const;

	int mWidth;
	int mHeight;
	Image mImage;
	Array2D< Object > mObjects;
	Array2D< bool > mGoalFlags;
};