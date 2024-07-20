#pragma once
#include "Object.h"
class Player :
    public Object
{
private:
	POINT originPoint[4]; // �÷��̾��� ���� ������ ���� �迭
	POINT calPoint[4]; // ������ ������ ������ ������ ������ ������ ���� �迭

	float speed; // �ӷ� 
	float bodyAngle; // ��ũ ��ü ȸ�� ��, ����� ��
	float gunBarrelLength; // ������ ����
	float gunBarrelAngle; // ���� ȸ�� ��, ����� ��
	D3DXVECTOR2 gunBarrelPoint; // ������ ���� ��ġ�� ��Ÿ�� ����
	D3DXVECTOR3 moveDisp; // Ű �Է����� �̵� ��, �� ƽ�� �̵� �������� ���� ���� 

public:
    // Object��(��) ���� ��ӵ�
    void Initialize() override;
    void Update() override;
    void Render(HDC hdc) override;
    void Release() override;

	void DisplayText(HDC hdc);
	void DisplayVector(HDC hdc, const wchar_t* text, D3DXVECTOR2 value, D3DXVECTOR2 displayPt);

public:
    Player();
    virtual ~Player();
};

