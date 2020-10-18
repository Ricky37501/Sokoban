#include "GameLib/Framework.h"
using namespace GameLib;
#include "State.h"
#include <fstream>
using namespace std;

//函数原型
void readFile(char** buffer, int* size, const char* filename);
void mainLoop();

//全局变量
State* gState = 0;

//用户封装的函数。具体处理放在mainLoop()中
namespace GameLib {
	void Framework::update() {
		mainLoop();
	}
}

void mainLoop() {
	//第一帧负责初始化。绘制好最初的状态后结束
	if (!gState) {
		const char* filename = "stageData.txt";
		char* stageData;
		int fileSize;
		readFile(&stageData, &fileSize, filename);
		if (!stageData) {
			cout << "stage file could not be read." << endl;
			return;
		}
		gState = new State(stageData, fileSize, "spirit.dds");
		//析构
		delete[] stageData;
		stageData = 0;
		//第一次绘制
		gState->draw();
		return; //直接结束本次处理
	}
	bool cleared = false;
	//主循环
	//通关检测
	if (gState->hasCleared()) {
		cleared = true;
	}
	//获取输入值
	cout << "a:left s:right w:up z:down. command?" << endl; //提示游戏如何操作
	char input;
	cin >> input;
	//刷新
	gState->update(input);
	//绘制
	gState->draw();

	if (cleared) {
		//通关祝贺信息
		cout << "Congratulation! you win." << endl;
		delete gState;
		gState = 0;
	}
	//结束判断
	if (input == 'q') {
		Framework::instance().requestEnd();
	}
	//x键是否被按下了？
	if (Framework::instance().isEndRequested()) {
		if (gState) {
			delete gState;
			gState = 0;
		}
		return;
	}
}

//---------------------下面是函数定义--------------

void readFile(char** buffer, int* size, const char* filename) {
	ifstream in(filename, ifstream::binary);
	if (!in) {
		*buffer = 0;
		*size = 0;
	}
	else {
		in.seekg(0, ifstream::end);
		*size = static_cast<int>(in.tellg());
		in.seekg(0, ifstream::beg);
		*buffer = new char[*size];
		in.read(*buffer, *size);
	}
}

