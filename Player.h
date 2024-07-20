#pragma once
#include "Object.h"
class Player :
    public Object
{
private:
	POINT originPoint[4]; // 플레이어의 원점 정보를 갖는 배열
	POINT calPoint[4]; // 원점의 정보를 가지고 연산한 점들의 정보를 갖는 배열

	float speed; // 속력 
	float bodyAngle; // 탱크 몸체 회전 시, 사용할 각
	float gunBarrelLength; // 포신의 길이
	float gunBarrelAngle; // 포신 회전 시, 사용할 각
	D3DXVECTOR2 gunBarrelPoint; // 포신의 끝점 위치를 나타낼 벡터
	D3DXVECTOR3 moveDisp; // 키 입력으로 이동 시, 한 틱에 이동 변위량을 가질 벡터 

public:
    // Object을(를) 통해 상속됨
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

