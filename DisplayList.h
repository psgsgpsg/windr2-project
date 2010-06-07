// 점 데이터를 저장하기 위한 구조체
// R, G, B : 색상 데이터
// nNodes : 형상의 노드 갯수
// *Xpos, *Ypos : 데이터의 x, y좌표
using namespace std;

typedef struct DisplayList {
	int	R, G, B;                    // 색상 정보
	int	nNodes;                     // 노드 갯수
	vector<double> XPos, YPos;      // X, Y 노드 좌표 벡터

	// 구조체 생성자
	DisplayList() {
		this->R = 0;
		this->G = 0;
		this->B = 0;
		this->nNodes = 0;
	}

	// 색상을 지정하는 부분
	void SetRGB(int Red, int Green, int Blue) {
		this->R = Red;
		this->G = Green;
		this->B = Blue;
	}

	// delta만큼 모든 점을 이동시킨다.
	void Translate(double Xdelta, double Ydelta) {
		for(int i = 0; i < this->nNodes; ++i) {
			this->XPos[i] += Xdelta;
			this->YPos[i] += Ydelta;
		}
	}

	// scalefactor 만큼 원점을 기준으로 스케일링을 함
	void Scale(double scalefactor) {
		for(int i = 0; i < this->nNodes; ++i) {
			this->XPos[i] *= scalefactor;
			this->YPos[i] *= scalefactor;
		}
	}
} DisplayList;