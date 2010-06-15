#include "stdafx.h"
#include "DisplayList.h"

// 기본 DiplayList 객체 생성자입니다.
DisplayList::DisplayList()
{
	this->R = 0;
	this->G = 0;
	this->B = 0;
	this->nNodes = 0;
}

// 기본 소멸자입니다.
DisplayList::~DisplayList()
{

}

// 모든 속성을 0으로 초기화합니다.
void DisplayList::reset()
{
	this->R = 0;
	this->G = 0;
	this->B = 0;
	this->nNodes = 0;
	this->XPos.clear();
	this->YPos.clear();
}

// 색상을 R, G, B 순서로 지정합니다. 색상 범위는 0-255입니다.
void DisplayList::SetRGB(short Red, short Green, short Blue)
{
	this->R = Red;
	this->G = Green;
	this->B = Blue;
}

// R 색상 데이터를 반환합니다.
short DisplayList::getR()
{
	return this->R;
}

// G 색상 데이터를 반환합니다.
short DisplayList::getG()
{
	return this->G;
}

// B 색상 데이터를 반환합니다.
short DisplayList::getB()
{
	return this->B;
}

// 노드 갯수를 반환합니다.
unsigned int DisplayList::GetNodes()
{
	return (this->nNodes);
}

// 노드 갯수를 지정합니다. 지정된 노드 갯수만큼 좌표값을 저장할 메모리를 할당합니다.
void DisplayList::setNodes(unsigned int numOfNodes)
{
	// 노드 갯수를 저장함
	this->nNodes = numOfNodes;

	// XPos, YPos vector 컨테이너의 공간을 미리 확보함
	this->XPos.reserve(numOfNodes);
	this->YPos.reserve(numOfNodes);

	// 확보된 공간만큼 0.0으로 채움
	for( unsigned int i = 0; i < numOfNodes; ++i) {
		this->XPos.push_back(0.0);
		this->YPos.push_back(0.0);
	}
}

// 지정된 인덱스에 X좌표를 입력합니다.
void DisplayList::setXPos(unsigned int index, double coord)
{
	this->XPos[index] = coord;
}

// 지정된 인덱스에 Y좌표를 입력합니다.
void DisplayList::setYPos(unsigned int index, double coord)
{
	this->YPos[index] = coord;
}

//지정된 벡터 인덱스의 X좌표값을 반환합니다.
double DisplayList::getXPos(unsigned int index)
{
	return ( this->XPos[index] );
}

//지정된 벡터 인덱스의 Y좌표값을 반환합니다.
double DisplayList::getYPos(unsigned int index)
{
	return ( this->YPos[index] );
}

// 해당 DisplayList의 X좌표값을 마지막에 추가합니다.
void DisplayList::addXPos(double coord)
{
	this->XPos.push_back(coord);
}

// 해당 DisplayList의 Y좌표값을 마지막에 추가합니다.
void DisplayList::addYPos(double coord)
{
	this->YPos.push_back(coord);
}

// 지정된 방향으로 Xdelta, Ydelta만큼 점을 이동합니다.
void DisplayList::Translate(double Xdelta, double Ydelta)
{
	for(unsigned int i = 0; i < this->nNodes; ++i) {
		this->XPos[i] += Xdelta;
		this->YPos[i] += Ydelta;
	}
}

// scalefactor 만큼 원점을 기준으로 확대/축소 연산을 수행합니다.
void DisplayList::Scale(double scalefactor)
{
	for(unsigned int i = 0; i < this->nNodes; ++i) {
		// 수치 오차를 줄이기 위해 1E-5를 곱함
		this->XPos[i] *= 1e3;
		this->YPos[i] *= 1e3;

		this->XPos[i] *= scalefactor;
		this->YPos[i] *= scalefactor;

		// 다시 원래 스케일로 복귀
		this->XPos[i] /= 1e3;
		this->YPos[i] /= 1e3;
	}
}

// scalefactor만큼 (x, y)를 기준으로 확대/축소 연산을 수행합니다.
void DisplayList::Scale(double scalefactor, double x, double y)
{
	// 모든 점에 대해 scaling 연산을 수행
	for(unsigned int i = 0; i < this->nNodes; ++i) {
		// 복합 변환 행렬 계산 수행
		// (x, y, 1) T(-x, -y) S(scalefactor, scalefactor) T(x, y)
		this->XPos[i] = x + ( x - this->XPos[i] ) * scalefactor;
		this->YPos[i] = y + ( y - this->YPos[i] ) * scalefactor;
	}
}

// x방향, Y 방향으로 각각 다른 축척값을 적용하여 확대합니다.
void DisplayList::Scale(double scale_x, double scale_y)
{
		// 모든 점에 대해 scaling 연산을 수행
	for(unsigned int i = 0; i < this->nNodes; ++i) {
		// 수치 오차를 줄이기 위해 1E-5를 곱함
		this->XPos[i] *= 1e5;
		this->YPos[i] *= 1e5;

		// scalefactor를 곱함
		this->XPos[i] *= scale_x;
		this->YPos[i] *= scale_y;

		// 다시 원래 스케일로 복귀
		this->XPos[i] /= 1e5;
		this->YPos[i] /= 1e5;
	}
}

// 기준점을 통해 x방향, Y 방향으로 각각 다른 축척값을 적용하여 확대합니다.
void DisplayList::Scale(double scale_x, double scale_y, double x, double y)
{
		// 모든 점에 대해 scaling 연산을 수행
	for(unsigned int i = 0; i < this->nNodes; ++i) {
		// 복합 변환 행렬 계산 수행
		// (x, y, 1) T(-x, -y) S(scale_x, scale_y) T(x, y)
		this->XPos[i] = x + ( x - this->XPos[i] ) * scale_x;
		this->YPos[i] = y + ( y - this->YPos[i] ) * scale_y;
	}
}

// 지정된 각도만큼 원점을 기준으로 회전합니다. 반시계방향이 +입니다.
void DisplayList::rot(double degree)
{
	double tempX, tempY;
	degree *= ( atan(1.0) / 45.0 ); // 입력받은 degree를 라디안으로 변환

	for(unsigned int i = 0; i < this->nNodes; ++i) {
		// 수치 오차를 줄이기 위해 1E-5를 곱함
		this->XPos[i] *= 1e5;
		this->YPos[i] *= 1e5;

		// 회전 변환 공식 적용
		tempX = cos(degree) * this->XPos[i] - sin(degree) * this->YPos[i];
		tempY = sin(degree) * this->XPos[i] + cos(degree) * this->YPos[i];

		// XPos, YPos 벡터에 대입
		this->XPos[i] = tempX;
		this->YPos[i] = tempY;

		// 다시 원래 스케일로 복귀
		this->XPos[i] /= 1e5;
		this->YPos[i] /= 1e5;
	}
}

// 지정된 각도만큼 (x, y)를 기준으로 회전합니다. 반시계방향이 +입니다.
void DisplayList::rot(double degree, double x, double y)
{
	double tempX, tempY;
	degree *= ( atan(1.0) / 45.0 ); // 입력받은 degree를 라디안으로 변환

	// 각도만큼 회전
	for(unsigned int i = 0; i < this->nNodes; ++i) {
		// 복합 변환 행렬 계산 수행
		// (x, y, 1) T(-x, -y) R(degree) T(x, y)
		tempX = x + (this->XPos[i] - x) * cos(degree) + (-this->YPos[i] + y) * sin(degree);
		tempY = y + (this->YPos[i] - y) * cos(degree) + ( this->XPos[i] - x) * sin(degree);

		this->XPos[i] = tempX;
		this->YPos[i] = tempY;
	}
}

// X축 혹은 Y축을 기준으로 반사 변환을 수행합니다.
void DisplayList::reflect(bool isX, bool isY)
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
	else if ( isX && isY ) { // isX, isY 모두 true일 경우 X, Y 좌표값의 부호를 모두 뒤집는다.
		for(unsigned int i = 0; i < this->nNodes; ++i) {
			this->XPos[i] -= this->XPos[i];
			this->YPos[i] -= this->YPos[i];
		}
	}
	else { // 모두 false일 경우
		return;
	}
}

