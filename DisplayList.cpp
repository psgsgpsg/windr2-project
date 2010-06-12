#include "stdafx.h"
#include "DisplayList.h"

DisplayList::DisplayList() // �⺻ DiplayList ��ü �������Դϴ�.
{
	this->R = 0;
	this->G = 0;
	this->B = 0;
	this->nNodes = 0;
}

DisplayList::~DisplayList() // �⺻ �Ҹ����Դϴ�.
{

}

void DisplayList::reset()  // ��� �Ӽ��� 0���� �ʱ�ȭ�մϴ�.
{
	this->R = 0;
	this->G = 0;
	this->B = 0;
	this->nNodes = 0;
	this->XPos.clear();
	this->YPos.clear();
}

void DisplayList::SetRGB(short Red, short Green, short Blue) // ������ R, G, B ������ �����մϴ�. ���� ������ 0-255�Դϴ�.
{
	this->R = Red;
	this->G = Green;
	this->B = Blue;
}

short DisplayList::getR() // R ���� �����͸� ��ȯ�մϴ�.
{
	return this->R;
}

short DisplayList::getG() // G ���� �����͸� ��ȯ�մϴ�.
{
	return this->G;
}

short DisplayList::getB() // B ���� �����͸� ��ȯ�մϴ�.
{
	return this->B;
}

unsigned int DisplayList::GetNodes() // ��� ������ ��ȯ�մϴ�.
{
	return (this->nNodes);
}

void DisplayList::setNodes(unsigned int numOfNodes) // ��� ������ �����մϴ�.
{
	// ��� ������ ������
	this->nNodes = numOfNodes;

	// XPos, YPos vector �����̳��� ������ �̸� Ȯ����
	this->XPos.reserve(numOfNodes);
	this->YPos.reserve(numOfNodes);
}

void DisplayList::setXPos(int index, double coord) // ������ �ε����� X��ǥ�� �Է��մϴ�.
{
	this->XPos[index] = coord;
}

void DisplayList::setYPos(int index, double coord) // ������ �ε����� Y��ǥ�� �Է��մϴ�.
{
	this->YPos[index] = coord;
}

double DisplayList::getXPos(int index) //������ ���� �ε����� X��ǥ���� ��ȯ�մϴ�.
{
	return ( this->XPos[index] );
}

double DisplayList::getYPos(int index) //������ ���� �ε����� Y��ǥ���� ��ȯ�մϴ�.
{
	return ( this->YPos[index] );
}

void DisplayList::addXPos(double coord) // X��ǥ���� �߰��մϴ�.
{
	this->XPos.push_back(coord);
}

void DisplayList::addYPos(double coord) // Y��ǥ���� �߰��մϴ�.
{
	this->YPos.push_back(coord);
}

void DisplayList::Translate(double Xdelta, double Ydelta) // ������ �������� Xdelta, Ydelta��ŭ ���� �̵��մϴ�.
{
	for(unsigned int i = 0; i < this->nNodes; ++i) {
		this->XPos[i] += Xdelta;
		this->YPos[i] += Ydelta;
	}
}

void DisplayList::Scale(double scalefactor) // scalefactor ��ŭ ������ �������� Ȯ��/��� ������ �����մϴ�.
{
	for(unsigned int i = 0; i < this->nNodes; ++i) {
		this->XPos[i] *= scalefactor;
		this->YPos[i] *= scalefactor;
	}
}

void DisplayList::Scale(double scalefactor, double x, double y) // scalefactor��ŭ (x, y)�� �������� Ȯ��/��� ������ �����մϴ�.
{
	// ��� ���� -x, -y��ŭ �̵���
	this->Translate(-x, -y);

	// ��� ���� ���� scaling ������ ����
	for(unsigned int i = 0; i < this->nNodes; ++i) {
		this->XPos[i] *= scalefactor;
		this->YPos[i] *= scalefactor;
	}

	// �ٽ� ������ ������ ����
	this->Translate(x, y);
}

void DisplayList::rot(double degree) // ������ ������ŭ ������ �������� ȸ���մϴ�. �ݽð������ +�Դϴ�.
{
	double tempX, tempY;
	degree *= ( atan(1.0) / 180.0 ); // �Է¹��� degree�� �������� ��ȯ

	for(unsigned int i = 0; i < this->nNodes; ++i) {
		// ȸ�� ��ȯ ���� ����
		tempX = cos(degree) * this->XPos[i] - sin(degree) * this->YPos[i];
		tempY = sin(degree) * this->XPos[i] + cos(degree) * this->YPos[i];

		// XPos, YPos ���Ϳ� ����
		this->XPos[i] = tempX;
		this->YPos[i] = tempY;
	}
}
void DisplayList::rot(double degree, double x, double y) // ������ ������ŭ (x, y)�� �������� ȸ���մϴ�. �ݽð������ +�Դϴ�.
{
	double tempX, tempY;
	degree *= ( atan(1.0) / 180.0 ); // �Է¹��� degree�� �������� ��ȯ

	// �Է¹��� x, y ��ŭ �������� �̵���
	this->Translate(-x, -y);

	// ������ŭ ȸ��
	for(unsigned int i = 0; i < this->nNodes; ++i) {
		// ȸ�� ��ȯ ���� ����
		tempX = cos(degree) * this->XPos[i] - sin(degree) * this->YPos[i];
		tempY = sin(degree) * this->XPos[i] + cos(degree) * this->YPos[i];

		// XPos, YPos ���Ϳ� ����
		this->XPos[i] = tempX;
		this->YPos[i] = tempY;
	}

	// �ٽ� ���� ��ġ�� ����
	this->Translate(x, y);
}

void DisplayList::reflect(bool isX, bool isY) // X�� Ȥ�� Y���� �������� �ݻ� ��ȯ�� �����մϴ�.
{
	/*
	isX�� true�� ��쿡�� X���� �������� �ݻ�, isY�� true�� ��쿡�� Y���� �������� �ݻ�
	 �Ѵ� true�� ��쿡�� ������ �������� �ݻ�, �Ѵ� false�� ��쿡�� �ƹ� ���۵� ���� ����
	*/

	if ( isX && !isY) { // isX�� true�� ��� Y��ǥ���� ��ȣ�� ��� �����´�.
		for(unsigned int i = 0; i < this->nNodes; ++i) {
			this->YPos[i] -= this->YPos[i];
		}
	}
	else if ( !isX && isY) { // isY�� true�� ��� X��ǥ���� ��ȣ�� ��� �����´�.
		for(unsigned int i = 0; i < this->nNodes; ++i) {
			this->XPos[i] -= this->XPos[i];
		}
	}
	else if ( isX && isY ) { // isX, isY ��� true�� ��� X, Y ��ǥ���� ��ȣ�� ��� �����´�.
		for(unsigned int i = 0; i < this->nNodes; ++i) {
			this->XPos[i] -= this->XPos[i];
			this->YPos[i] -= this->YPos[i];
		}
	}
	else { // ��� false�� ���
		return;
	}
}

