#include "GameLib/Framework.h"
using namespace GameLib;
#include "State.h"
#include <fstream>
using namespace std;

//����ԭ��
void readFile(char** buffer, int* size, const char* filename);
void mainLoop();

//ȫ�ֱ���
State* gState = 0;

//�û���װ�ĺ��������崦�����mainLoop()��
namespace GameLib {
	void Framework::update() {
		mainLoop();
	}
}

void mainLoop() {
	//��һ֡�����ʼ�������ƺ������״̬�����
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
		//����
		delete[] stageData;
		stageData = 0;
		//��һ�λ���
		gState->draw();
		return; //ֱ�ӽ������δ���
	}
	bool cleared = false;
	//��ѭ��
	//ͨ�ؼ��
	if (gState->hasCleared()) {
		cleared = true;
	}
	//��ȡ����ֵ
	cout << "a:left s:right w:up z:down. command?" << endl; //��ʾ��Ϸ��β���
	char input;
	cin >> input;
	//ˢ��
	gState->update(input);
	//����
	gState->draw();

	if (cleared) {
		//ͨ��ף����Ϣ
		cout << "Congratulation! you win." << endl;
		delete gState;
		gState = 0;
	}
	//�����ж�
	if (input == 'q') {
		Framework::instance().requestEnd();
	}
	//x���Ƿ񱻰����ˣ�
	if (Framework::instance().isEndRequested()) {
		if (gState) {
			delete gState;
			gState = 0;
		}
		return;
	}
}

//---------------------�����Ǻ�������--------------

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

