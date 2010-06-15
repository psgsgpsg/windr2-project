// 점 데이터를 저장하기 위한 클래스
// R, G, B : 색상 데이터
// nNodes : 형상의 노드 갯수
// *Xpos, *Ypos : 데이터의 x, y좌표
#pragma once
#include <cmath>
#include <vector>

using namespace std;

class DisplayList
{
	short R; // Red 색상 데이터 (0-255 사이의 정수입니다)
	short G; // Green 색상 데이터 (0-255 사이의 정수입니다)
	short B; // Blue 색상 데이터 (0-255 사이의 정수입니다)
	unsigned int nNodes; // 노드 갯수
	vector<double> XPos; // X 노드 좌표 벡터
	vector<double> YPos; // Y 노드 좌표 벡터

	// 멤버 함수
public:
	// 기본 생성자 및 소멸자 함수 정의
	// 기본 DiplayList 구조체를 생성합니다.
	DisplayList();
	// DisplayList 소멸자입니다.
	~DisplayList();
	// 모든 속성을 0으로 초기화합니다.
	void reset();

	// 색상 관련 함수 정의
	// 색상을 R, G, B 순서로 지정합니다. 색상 범위는 0-255입니다.
	void SetRGB(short Red = 0, short Green = 0, short Blue = 0);
	// R 색상 데이터를 반환합니다.
	short getR();
	// G 색상 데이터를 반환합니다.
	short getG();
	// B 색상 데이터를 반환합니다.
	short getB();

	// 노드 수 관련 함수 정의
	// 노드 갯수를 반환합니다.
	unsigned int GetNodes();
	// 노드 갯수를 지정합니다.
	void setNodes(unsigned int numOfNodes);

	// 좌표 데이터 관련 함수 정의
	//지정된 벡터 인덱스에 X좌표값을 입력합니다.
	void setXPos(unsigned int index, double coord);
	//지정된 벡터 인덱스에 Y좌표값을 입력합니다.
	void setYPos(unsigned int index, double coord);
	//지정된 벡터 인덱스의 X좌표값을 반환합니다.
	double getXPos(unsigned int index);
	//지정된 벡터 인덱스의 Y좌표값을 반환합니다.
	double getYPos(unsigned int index);
	// X 좌표 데이터를 추가합니다.
	void addXPos(double coord);
	// Y 좌표 데이터를 추가합니다.
	void addYPos(double coord);

	// 좌표 변환 관련 함수 정의
	// 지정된 방향으로 Xdelta, Ydelta만큼 점을 이동합니다.
	void Translate(double Xdelta, double Ydelta);
	// scalefactor 만큼 원점을 기준으로 확대/축소 연산을 수행합니다.
	void Scale(double scalefactor);
	// 오버로드 되었습니다.
	// scalefactor만큼 (x, y)를 기준으로 확대/축소 연산을 수행합니다.
	void Scale(double scalefactor, double x, double y);
	// 오버로드 되었습니다.
	// x방향, Y 방향으로 각각 다른 축척값을 적용하여 확대합니다.
	void Scale(double scale_x, double scale_y);
	// 오버로드 되었습니다.
	// 기준점을 통해 x방향, Y 방향으로 각각 다른 축척값을 적용하여 확대합니다.
	void Scale(double scale_x, double scale_y, double x, double y);
	// 지정된 각도만큼 원점을 기준으로 회전합니다. 반시계방향이 +입니다.
	void rot(double radian);
	// 지정된 각도만큼 (x, y)를 기준으로 회전합니다. 반시계방향이 +입니다.
	void rot(double radian, double x, double y);
	// X축 혹은 Y축을 기준으로 반사 변환을 수행합니다.
	void reflect(bool isX, bool isY);
};
