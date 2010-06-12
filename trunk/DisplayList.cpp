#include "stdafx.h"
#include "DisplayList.h"

DisplayList::DisplayList() // 기본 DiplayList 객체 생성자입니다.
{
	this->R = 0;
	this->G = 0;
	this->B = 0;
	this->nNodes = 0;
}

DisplayList::~DisplayList() // 기본 소멸자입니다.
{

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

void DisplayList::SetRGB(short Red, short Green, short Blue) // 색상을 R, G, B 순서로 지정합니다. 색상 범위는 0-255입니다.
{
	this->R = Red;
	this->G = Green;
	this->B = Blue;
}

short DisplayList::getR() // R 색상 데이터를 반환합니다.
{
	return this->R;
}

short DisplayList::getG() // G 색상 데이터를 반환합니다.
{
	return this->G;
}

short DisplayList::getB() // B 색상 데이터를 반환합니다.
{
	return this->B;
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

void DisplayList::Scale(double scalefactor, double x, double y) // scalefactor만큼 (x, y)를 기준으로 확대/축소 연산을 수행합니다.
{
	// 모든 점을 -x, -y만큼 이동함
	this->Translate(-x, -y);

	// 모든 점에 대해 scaling 연산을 수행
	for(unsigned int i = 0; i < this->nNodes; ++i) {
		this->XPos[i] *= scalefactor;
		this->YPos[i] *= scalefactor;
	}

	// 다시 원래의 점으로 복귀
	this->Translate(x, y);
}

void DisplayList::rot(double degree) // 지정된 각도만큼 원점을 기준으로 회전합니다. 반시계방향이 +입니다.
{
	double tempX, tempY;
	degree *= ( atan(1.0) / 180.0 ); // 입력받은 degree를 라디안으로 변환

	for(unsigned int i = 0; i < this->nNodes; ++i) {
		// 회전 변환 공식 적용
		tempX = cos(degree) * this->XPos[i] - sin(degree) * this->YPos[i];
		tempY = sin(degree) * this->XPos[i] + cos(degree) * this->YPos[i];

		// XPos, YPos 벡터에 대입
		this->XPos[i] = tempX;
		this->YPos[i] = tempY;
	}
}
void DisplayList::rot(double degree, double x, double y) // 지정된 각도만큼 (x, y)를 기준으로 회전합니다. 반시계방향이 +입니다.
{
	double tempX, tempY;
	degree *= ( atan(1.0) / 180.0 ); // 입력받은 degree를 라디안으로 변환

	// 입력받은 x, y 만큼 원점으로 이동함
	this->Translate(-x, -y);

	// 각도만큼 회전
	for(unsigned int i = 0; i < this->nNodes; ++i) {
		// 회전 변환 공식 적용
		tempX = cos(degree) * this->XPos[i] - sin(degree) * this->YPos[i];
		tempY = sin(degree) * this->XPos[i] + cos(degree) * this->YPos[i];

		// XPos, YPos 벡터에 대입
		this->XPos[i] = tempX;
		this->YPos[i] = tempY;
	}

	// 다시 원래 위치로 복귀
	this->Translate(x, y);
}

void DisplayList::reflect(bool isX, bool isY) // X축 혹은 Y축을 기준으로 반사 변환을 수행합니다.
{
	/*
	isX가 true일 경우에는 X축을 기준으로 반사, isY가 true일 경우에는 Y축을 기준으로 반사
	 둘다 true일 경우에는 원점을 기준으로 반사, 둘다 false일 경우에는 아무 동작도 하지 않음
	*/

	if ( isX && !isY) { // isX만 true일 경우 Y좌표값의 부호를 모두 뒤집는다.
		for(unsigned int i = 0; i < this->nNodes; ++i) {
			this->YPos[i] -= this->YPos[i];
		}
	}
	else if ( !isX && isY) { // isY만 true일 경우 X좌표값의 부호를 모두 뒤집는다.
		for(unsigned int i = 0; i < this->nNodes; ++i) {
			this->XPos[i] -= this->XPos[i];
		}
	}
	else if ( isX && isY ) { // isX, isY 모두 true일 경우 X, Y 좌표값의 부호는 모두 뒤집는다.
		for(unsigned int i = 0; i < this->nNodes; ++i) {
			this->XPos[i] -= this->XPos[i];
			this->YPos[i] -= this->YPos[i];
		}
	}
	else { // 모두 false일 경우
		return;
	}
}

