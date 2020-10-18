#include "GameLib/Framework.h"
using namespace GameLib;
#include "State.h"
#include <fstream>
#include <algorithm>
using namespace std;

State::State(const char* stageData, int size, const char* imagefilename) :mImage(Image(imagefilename)){
	//检测容量
	setSize(stageData, size);
	//确保容量
	mObjects.setSize(mWidth, mHeight);
	mGoalFlags.setSize(mWidth, mHeight);
	//预先设置好初始值
	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			mObjects(x, y) = OBJ_WALL; //先全部设置为墙壁
			mGoalFlags(x, y) = false; //非终点
		}
	}
	int x = 0;
	int y = 0;
	for (int i = 0; i < size; ++i) {
		Object t;
		bool goalFlag = false;
		switch (stageData[i]) {
		case '#': t = OBJ_WALL; break;
		case ' ': t = OBJ_SPACE; break;
		case 'o': t = OBJ_BLOCK; break;
		case 'O': t = OBJ_BLOCK; goalFlag = true; break;
		case '.': t = OBJ_SPACE; goalFlag = true; break;
		case 'p': t = OBJ_MAN; break;
		case 'P': t = OBJ_MAN; goalFlag = true; break;
		case '\n': x = 0; ++y; t = OBJ_UNKNOWN; break; //换行处理
		default: t = OBJ_UNKNOWN; break;
		}
		if (t != OBJ_UNKNOWN) { //这个if处理的意义在如果遇到未定义的元素值就跳过它
			mObjects(x, y) = t; //写入
			mGoalFlags(x, y) = goalFlag; //终点信息
			++x;
		}
	}
}

void State::setSize(const char* stageData, int size) {
	mWidth = mHeight = 0; //初始化
	//当前位置
	int x = 0;
	int y = 0;
	for (int i = 0; i < size; ++i) {
		switch (stageData[i]) {
		case '#': case ' ': case 'o': case 'O':
		case '.': case 'p': case 'P':
			++x;
			break;
		case '\n':
			++y;
			//更新最大值
			mWidth = max(mWidth, x);
			mHeight = max(mHeight, y);
			x = 0;
			break;
		}
	}
}

void State::draw() const {
	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			Object o = mObjects(x, y);
			bool goalFlag = mGoalFlags(x, y);
			//unsigned color = 0;
			int index = -1;
			//if (goalFlag) {
			//	switch (o) {
			//	case OBJ_SPACE: cout << '.'; color = 0x0000ff; break;
			//	case OBJ_WALL: cout << '#'; color = 0xffffff; break;
			//	case OBJ_BLOCK: cout << 'O'; color = 0xff00ff; break;
			//	case OBJ_MAN: cout << 'P'; color = 0x00ffff; break;
			//	}
			//}
			//else {
			//	switch (o) {
			//	case OBJ_SPACE: cout << ' '; color = 0x000000; break;
			//	case OBJ_WALL: cout << '#'; color = 0xffffff; break;
			//	case OBJ_BLOCK: cout << 'o'; color = 0xff0000; break;
			//	case OBJ_MAN: cout << 'p'; color = 0x00ff00; break;
			//	}			
			if (goalFlag) {
				switch (o) {
				case OBJ_SPACE: cout << '.'; index = 4; break;
				case OBJ_WALL: cout << '#'; index = 1; break;
				case OBJ_BLOCK: cout << 'O'; index = 3; break;
				case OBJ_MAN: cout << 'P'; index = 0; break;
				}
			}
			else {
				switch (o) {
				case OBJ_SPACE: cout << ' '; index = 5; break;
				case OBJ_WALL: cout << '#'; index = 1; break;
				case OBJ_BLOCK: cout << 'o'; index = 2; break;
				case OBJ_MAN: cout << 'p'; index = 0; break;
				}
			}
			//drawCell(x, y, color);
			drawPicture(x, y, index);
		}
		cout << endl;
	}
}

void State::drawCell(int x, int y, unsigned color) {
	unsigned* vram = Framework::instance().videoMemory();
	unsigned windowWidth = Framework::instance().width();
	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 16; ++j) {
			vram[(y * 16 + i) * windowWidth + (x * 16 + j)] = color;
		}
	}
}

void State::drawPicture(int x, int y, int index) const{
	unsigned* vram = Framework::instance().videoMemory();
	unsigned windowWidth = Framework::instance().width();
	for (int row = 0, i=0;row < mImage.getHeight();row++,i++) {
		for (int col = index * 32,j=0;col < (index + 1) * 32;col++,j++) {
			vram[(y*32 + i)*windowWidth + (x*32 + j)] = mImage.getImageData()[row*mImage.getWidth() + col];
		}
	}
}void State::drawWinPicture() const{
	unsigned* vram = Framework::instance().videoMemory();
	unsigned windowWidth = Framework::instance().width();
	Image win("test.dds");
	for (int i = 0;i < win.getHeight()*win.getWidth();i++) {
		vram[(i/win.getWidth())*windowWidth+i%win.getWidth()] = win.getImageData()[i];
	}
}

//更新myObject二维数组
void State::update(char input) {
	//移动量变换
	int dx = 0;
	int dy = 0;
	switch (input) {
	case 'a': dx = -1; break; //向左
	case 'd': dx = 1; break; //右
	case 'w': dy = -1; break; //上。Y朝下为正
	case 's': dy = 1; break; //下。
	}
	//使用较短的变量命名
	int w = mWidth;
	int h = mHeight;
	Array2D< Object >& o = mObjects;
	//查询小人的坐标
	int x, y;
	x = y = -1;
	bool found = false;
	for (y = 0; y < mHeight; ++y) {
		for (x = 0; x < mWidth; ++x) {
			if (o(x, y) == OBJ_MAN) {
				found = true;
				break;
			}
		}
		if (found) {
			break;
		}
	}
	//移动
	//移动后的坐标
	int tx = x + dx;
	int ty = y + dy;
	//判断坐标的极端值。不允许超出合理值范围
	if (tx < 0 || ty < 0 || tx >= w || ty >= h) {
		return;
	}
	//A.该方向上是空白或者终点。则小人移动
	if (o(tx, ty) == OBJ_SPACE) {
		o(tx, ty) = OBJ_MAN;
		o(x, y) = OBJ_SPACE;
		//B.如果该方向上是箱子。并且该方向的下下个格子是空白或者终点，则允许移动
	}
	else if (o(tx, ty) == OBJ_BLOCK) {
		//检测同方向上的下下个格子是否位于合理值范围
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) { //按键无效
			return;
		}
		if (o(tx2, ty2) == OBJ_SPACE) {
			//按顺序替换
			o(tx2, ty2) = OBJ_BLOCK;
			o(tx, ty) = OBJ_MAN;
			o(x, y) = OBJ_SPACE;
		}
	}
}

//只要还存在一个goalFlag值为false就不能判定为通关
bool State::hasCleared() const {
	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			if (mObjects(x, y) == OBJ_BLOCK) {
				if (mGoalFlags(x, y) == false) {
					return false;
				}
			}
		}
	}
	return true;
}