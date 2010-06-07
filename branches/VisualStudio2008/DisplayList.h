// �� �����͸� �����ϱ� ���� ����ü
// R, G, B : ���� ������
// nNodes : ������ ��� ����
// *Xpos, *Ypos : �������� x, y��ǥ
using namespace std;

typedef struct DisplayList {
	int	R, G, B;                    // ���� ����
	int	nNodes;                     // ��� ����
	vector<double> XPos, YPos;      // X, Y ��� ��ǥ ����

	// ����ü ������
	DisplayList() {
		this->R = 0;
		this->G = 0;
		this->B = 0;
		this->nNodes = 0;
	}

	// ������ �����ϴ� �κ�
	void SetRGB(int Red, int Green, int Blue) {
		this->R = Red;
		this->G = Green;
		this->B = Blue;
	}

	// delta��ŭ ��� ���� �̵���Ų��.
	void Translate(double Xdelta, double Ydelta) {
		for(int i = 0; i < this->nNodes; ++i) {
			this->XPos[i] += Xdelta;
			this->YPos[i] += Ydelta;
		}
	}

	// scalefactor ��ŭ ������ �������� �����ϸ��� ��
	void Scale(double scalefactor) {
		for(int i = 0; i < this->nNodes; ++i) {
			this->XPos[i] *= scalefactor;
			this->YPos[i] *= scalefactor;
		}
	}
} DisplayList;