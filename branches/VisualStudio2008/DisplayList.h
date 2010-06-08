// 점 데이터를 저장하기 위한 클래스
// R, G, B : 색상 데이터
// nNodes : 형상의 노드 갯수
// *Xpos, *Ypos : 데이터의 x, y좌표
#pragma once

using namespace std;

class DisplayList
{
	int R; // Red 색상 데이터 (0-255 사이의 정수입니다)
	int G; // Green 색상 데이터 (0-255 사이의 정수입니다)
	int B; // Blue 색상 데이터 (0-255 사이의 정수입니다)
	unsigned int nNodes; // 노드 갯수
	vector<double> XPos; // X 노드 좌표 벡터
	vector<double> YPos; // Y 노드 좌표 벡터

	// 멤버 함수
public:
	// 기본 생성자 및 소멸자 함수 정의
	DisplayList();	// 기본 DiplayList 구조체를 생성합니다.
	~DisplayList(); // DisplayList 소멸자입니다.
	void reset(); // 모든 속성을 0으로 초기화합니다.

	// 색상 관련 함수 정의
	void SetRGB(int Red = 0, int Green = 0, int Blue = 0); // 색상을 R, G, B 순서로 지정합니다. 색상 범위는 0-255입니다.
	int getR(); // R 색상 데이터를 반환합니다.
	int getG(); // G 색상 데이터를 반환합니다.
	int getB(); // B 색상 데이터를 반환합니다.

	// 노드 수 관련 함수 정의
	unsigned int GetNodes(); // 노드 갯수를 반환합니다.
	void setNodes(unsigned int numOfNodes); // 노드 갯수를 지정합니다.

	// 좌표 데이터 관련 함수 정의
	void setXPos(int index, double coord); //지정된 벡터 인덱스에 X좌표값을 입력합니다.
	void setYPos(int index, double coord); //지정된 벡터 인덱스에 Y좌표값을 입력합니다.
	double getXPos(int index); //지정된 벡터 인덱스의 X좌표값을 반환합니다.
	double getYPos(int index); //지정된 벡터 인덱스의 Y좌표값을 반환합니다.
	void addXPos(double coord); // X 좌표 데이터를 추가합니다.
	void addYPos(double coord); // Y 좌표 데이터를 추가합니다.

	// 좌표 변환 관련 함수 정의
	void Translate(double Xdelta, double Ydelta); // 지정된 방향으로 Xdelta, Ydelta만큼 점을 이동합니다.
	void Scale(double scalefactor);	// scalefactor 만큼 원점을 기준으로 확대/축소 연산을 수행합니다.
};

DisplayList::DisplayList() // 기본 DiplayList 객체 생성자입니다.
{
	this->R = 0;
	this->G = 0;
	this->B = 0;
	this->nNodes = 0;
}

DisplayList::~DisplayList()
{
	// 동적으로 할당된 벡터 컨테이너를 초기화합니다.
	this->XPos.clear();
	this->YPos.clear();
}

void DisplayList::reset()  // 모든 속성을 0으로 초기화합니다.
{
	this->R = 0;
	this->G = 0;
	this->B = 0;
	this->nNodes = 0;
	this->XPos.clear();
	this->YPos.clear();
}

void DisplayList::SetRGB(int Red, int Green, int Blue) // 색상을 R, G, B 순서로 지정합니다. 색상 범위는 0-255입니다.
{
	this->R = Red;
	this->G = Green;
	this->B = Blue;
}

int DisplayList::getR() // R 색상 데이터를 반환합니다.
{
	return this->R;
}

int DisplayList::getG() // G 색상 데이터를 반환합니다.
{
	return this->G;
}

int DisplayList::getB() // B 색상 데이터를 반환합니다.
{
	return this->B;
}

void DisplayList::Translate(double Xdelta, double Ydelta) // 지정된 방향으로 Xdelta, Ydelta만큼 점을 이동합니다.
{
	for(unsigned int i = 0; i < this->nNodes; ++i) {
		this->XPos[i] += Xdelta;
		this->YPos[i] += Ydelta;
	}
}

void DisplayList::Scale(double scalefactor) // scalefactor 만큼 원점을 기준으로 확대/축소 연산을 수행합니다.
{
	for(unsigned int i = 0; i < this->nNodes; ++i) {
		this->XPos[i] *= scalefactor;
		this->YPos[i] *= scalefactor;
	}
}

unsigned int DisplayList::GetNodes() // 노드 갯수를 반환합니다.
{
	return (this->nNodes);
}

void DisplayList::setNodes(unsigned int numOfNodes) // 노드 갯수를 지정합니다.
{
	// 노드 갯수를 저장함
	this->nNodes = numOfNodes;

	// XPos, YPos vector 컨테이너의 공간을 미리 확보함
	this->XPos.reserve(numOfNodes);
	this->YPos.reserve(numOfNodes);
}

void DisplayList::setXPos(int index, double coord) // 지정된 인덱스에 X좌표를 입력합니다.
{
	this->XPos[index] = coord;
}

void DisplayList::setYPos(int index, double coord) // 지정된 인덱스에 Y좌표를 입력합니다.
{
	this->YPos[index] = coord;
}

double DisplayList::getXPos(int index) //지정된 벡터 인덱스의 X좌표값을 반환합니다.
{
	return ( this->XPos[index] );
}

double DisplayList::getYPos(int index) //지정된 벡터 인덱스의 Y좌표값을 반환합니다.
{
	return ( this->YPos[index] );
}

void DisplayList::addXPos(double coord) // X좌표값을 추가합니다.
{
	this->XPos.push_back(coord);
}

void DisplayList::addYPos(double coord) // Y좌표값을 추가합니다.
{
	this->YPos.push_back(coord);
}