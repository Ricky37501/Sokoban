#pragma once
//��ά������
//��֪�������Ƿ���Ϥģ����÷����������Ϥ������Ҫѧϰһ����ػ���
//�ඨ���г�����T������ʹ��ʱ������Ҫ�ᱻ�滻��int����bool�Ⱦ�������
template< class T > class Array2D {
public:
	Array2D() : mArray(0) {}
	~Array2D() {
		delete[] mArray;
		mArray = 0;  //Ϊ��ȫ�������ָ��ֵ��Ϊ0
	}
	void setSize(int size0, int size1) {
		mSize0 = size0;
		mSize1 = size1;
		mArray = new T[size0 * size1];
	}
	T& operator()(int index0, int index1) {
		return mArray[index1 * mSize0 + index0];
	}
	const T& operator()(int index0, int index1) const {
		return mArray[index1 * mSize0 + index0];
	}
private:
	T* mArray;
	int mSize0;
	int mSize1;
};

