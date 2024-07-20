#include "Player.h"

// 렌더링 파이프라인
//  -> 그래픽을 출력하기까지의 과정
// 디테일한 부분은 그래픽스 라이브러리마다, 그리고 동일한 그래픽스 라이브러리라도
// 버전마다 디테일한 부분은 약간씩 다르지만, 핵심적인 개념은 모두 동일함

// 모델(로컬) 스페이스 -> 월드 스페이스 

// 3d 모델(객체)들은 각자 자기 자신만의 좌표체계를 가지고 있음
// 이 객체들을 월드 공간 상(하나의 공간)에 올리게 되면
// 월드 좌표를 기준으로 한 좌표 체계의 변환이 발생 됨

// 이 때, 가장 깔끔한 방식은 선형 변환
// 선형 변환을 하는 이유?
// 행렬을 이용한 선형 변환을 통해, 빠른 연산 및 변환의 누적이 가능해짐
// -> 실제로는 아핀 변환

// 크기 (선형 변환), 회전 (선형 변환), 위치 (선형 변환 시, 문제가 생김)
// -> 위치 변환으로 인해 변환의 누적이 깨지면서, 행렬 하나로 크기/회전/위치를 나타낼 수 없게 됨



void Player::Initialize()
{
	speed = 3.f;
	info.pos = D3DXVECTOR3(WIN_SIZE_X / 2.f, WIN_SIZE_Y / 2.f, 0);
	info.dir = D3DXVECTOR3(0, 0, 0);
	info.look = D3DXVECTOR3(1.f, 0, 0);
	info.size = D3DXVECTOR3(50.f, 50.f, 0);
	moveDisp = D3DXVECTOR3(0, 0, 0);

	bodyAngle = 0;
	gunBarrelAngle = 0;
	gunBarrelLength = 100.f;

	// 위, 왼
	originPoint[0].x = info.pos.x - info.size.x;
	originPoint[0].y = info.pos.y - info.size.y;
	// 위, 오
	originPoint[1].x = info.pos.x + info.size.x;
	originPoint[1].y = info.pos.y - info.size.y;
	// 아래, 오
	originPoint[2].x = info.pos.x + info.size.x;
	originPoint[2].y = info.pos.y + info.size.y;
	// 아래, 왼
	originPoint[3].x = info.pos.x - info.size.x;
	originPoint[3].y = info.pos.y + info.size.y;
}

void Player::Update()
{
	// 탱크 몸체 회전 A, D키
	if (GetAsyncKeyState('A'))
	{
		bodyAngle -= D3DXToRadian(5.f);
	}

	if (GetAsyncKeyState('D'))
	{
		bodyAngle += D3DXToRadian(5.f);
	}

	// 몸체 회전 각에 따라 해당 각으로 전진 W키
	if (GetAsyncKeyState('W'))
	{
		D3DXVECTOR3 forwordDir;
		forwordDir.x = info.look.x * cosf(bodyAngle) - info.look.y * sinf(bodyAngle);
		forwordDir.y = info.look.x * sinf(bodyAngle) + info.look.y * cosf(bodyAngle);

		moveDisp += forwordDir * speed;
	}

	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(g_hWnd, &mouse);

	// 벡터 타입끼리 연산을 위해 포인트 좌표를 벡터로 변환
	D3DXVECTOR3 mousePos(mouse.x, mouse.y, 0);

	// 플레이어가 마우스쪽을 바라보게 하려고 함
	
	// 플레이어에서 마우스 쪽으로 향하는 벡터를 구함
	info.dir = mousePos - info.pos;
	// 방향벡터로 만들기 위해, 벡터의 크기를 1로 만들어야 함
	//  -> 벡터의 크기를 구해서, 벡터의 각 원소를 크기로 나눈다.

	// 벡터의 크기를 구한다. 
	/*float length = sqrt(info.dir.x * info.dir.x + info.dir.y * info.dir.y);
	if (length < 3.f)
	{
		return;
	}
	info.dir.x /= length;
	info.dir.y /= length;*/

	// 벡터를 단위벡터화 한다.
	D3DXVec3Normalize(&info.dir, &info.dir);

	// 벡터의 각 성분끼리 곱하고, 더하는 작업
	// -> 벡터의 내적
	// D3DXVec3Dot
	// angle = acosf(info.dir.x * info.look.x + info.dir.y * info.look.y);
	// 포신의 각도를 방향벡터 쪽으로 변경

	// look 벡터는 기저 방향(현재 2d x축)으로의 값만 갖는 크기가 1인 벡터
	// 해당 벡터의 성분과 캐릭터가 바라보는 방향 성분끼리 곱한 후,
	// 결과를 더했을 때, 결과적으로 x축 성분 값만 남게 됨
	// 이 때, dir은 크기가 1인 단위 벡터이므로, 단위 원에 존재한다고 볼 수 있음
	// 단위 원에 존재한다면, x의 값은 cos 세타를 의미함
	// cos 세타는 밑변/빗변이고, 해당 값을 역함수에 넣을 시, 결과적으로
	// 원하는 세타 각을 구할 수 있음
	gunBarrelAngle = acosf(D3DXVec3Dot(&info.dir, &info.look));

	// 역함수는 180도까지 밖에 표현을 못하므로, 예외 처리
	if (mouse.y > info.pos.y)
	{
		gunBarrelAngle = 2 * D3DX_PI - gunBarrelAngle;
	}

	// 포신 끝점 설정
	gunBarrelPoint = D3DXVECTOR2(cosf(gunBarrelAngle) * gunBarrelLength + info.pos.x + moveDisp.x,
		-sinf(gunBarrelAngle) * gunBarrelLength + info.pos.y + moveDisp.y);

	// 플레이어를 나타낼 4개의 점을 연산
	POINT temp;
	for (int i = 0; i < 4; ++i)
	{
		// 원점 정보 가져오기
		calPoint[i] = originPoint[i];

		// 점에서 플레이어의 위치를 제거
		// -> 크기에 관한 값만 남게 됨
		//    크기만 남았다는 것은, 플레이어가 원점 상에 존재하는 것과 동일함
		// -> 플레이어의 로컬 좌표를 표현
		calPoint[i].x -= info.pos.x;
		calPoint[i].y -= info.pos.y;

		// 왜 위치를 없애고, 크기만 남겼는지?
		// -> 변환의 누적이 깨짐, 대표적으로 회전 시 객체의 위치까지 변경됨 
		//    제자리 회전을 표현하려면 위치가 없어야 함 
		// 삼각함수의 덧셈 정리
		// x를 회전한 좌표 = cosf * cosf - sinf * sinf;
		// y를 회전한 좌표 = cosf * sinf + sinf * cosf;
		temp = calPoint[i];
		calPoint[i].x = temp.x * cosf(bodyAngle) + (-temp.y * sinf(bodyAngle));
		calPoint[i].y = temp.x * sinf(bodyAngle) + (temp.y * cosf(bodyAngle));

		// 로컬 회전이 끝났으므로, 다시 플레이어의 위치를 더해 최종적으로 회전된 위치를 나타냄
		// -> 로컬 좌표를 월드 좌표로 바꿈
		calPoint[i].x += info.pos.x + moveDisp.x;
		calPoint[i].y += info.pos.y + moveDisp.y;
	}
}

void Player::Render(HDC hdc)
{
	DisplayText(hdc);

	// 플레이어 그리기
	MoveToEx(hdc, calPoint[0].x, calPoint[0].y, NULL);
	LineTo(hdc, calPoint[1].x, calPoint[1].y);
	LineTo(hdc, calPoint[2].x, calPoint[2].y);
	LineTo(hdc, calPoint[3].x, calPoint[3].y);
	LineTo(hdc, calPoint[0].x, calPoint[0].y);

	// 포신 그리기
	MoveToEx(hdc, info.pos.x + moveDisp.x, info.pos.y + moveDisp.y, NULL);
	LineTo(hdc, gunBarrelPoint.x, gunBarrelPoint.y);
}

void Player::Release()
{
}

void Player::DisplayText(HDC hdc)
{
	// 마우스 좌표
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(g_hWnd, &mouse);
	DisplayVector(hdc, L"마우스", D3DXVECTOR2(mouse.x, mouse.y), D3DXVECTOR2(10, 10));

	// 방향 벡터 (플레이어가 마우스쪽을 바라보는 방향)
	DisplayVector(hdc, L"방향 벡터", D3DXVECTOR2(info.dir.x, info.dir.y), D3DXVECTOR2(10, 30));

	// 포신각 (라디안, 디그리)
	DisplayVector(hdc, L"포신각 (r, d)", D3DXVECTOR2(gunBarrelAngle, D3DXToDegree(gunBarrelAngle)), D3DXVECTOR2(10, 50));

	// 몸체각 (라디안, 디그리)
	DisplayVector(hdc, L"몸체각 (r, d)", D3DXVECTOR2(bodyAngle, D3DXToDegree(bodyAngle)), D3DXVECTOR2(10, 70));

	// 포신 끝점 위치
	DisplayVector(hdc, L"포신 끝점", D3DXVECTOR2(gunBarrelPoint.x, gunBarrelPoint.y), D3DXVECTOR2(10, 90));

	// 플레이어의 연산된 4점의 위치
	for (int i = 0; i < 4; ++i)
	{
		wchar_t str[32];
		swprintf_s(str, L"CalPoint %d", i);
		DisplayVector(hdc, str, D3DXVECTOR2(calPoint[i].x, calPoint[i].y),
			D3DXVECTOR2(10, 110 + i * 20));
	}
}

void Player::DisplayVector(HDC hdc, const wchar_t* text, D3DXVECTOR2 value, D3DXVECTOR2 displayPt)
{
	wchar_t temp[128];
	swprintf_s(temp, L"%s : %.2f, %.2f", text, value.x, value.y);
	TextOut(hdc, displayPt.x, displayPt.y, temp, wcslen(temp));
}

Player::Player()
{
}

Player::~Player()
{
}
