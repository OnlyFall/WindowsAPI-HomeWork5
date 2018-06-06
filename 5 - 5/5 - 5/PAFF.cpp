#include <Windows.h>
#include "resource.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <atlImage.h>
#include <math.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hinst;
LPCTSTR lpszClass = TEXT("First");

BOOL CollisionCheck(int x, int y, int mx, int my);


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) //h의 의미? = 핸들
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hinst = hinstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //배경색
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //프로그램 내에서 보이는 커서
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //아이콘 
	WndClass.hInstance = hinstance; //현재 실행되고 있는 객체의 핸들
	WndClass.lpfnWndProc = WndProc; //프로시저함수의 이름 
	WndClass.lpszClassName = lpszClass; //윈도우 클래스 내용의 이름 
	WndClass.lpszMenuName = NULL; // 메뉴바 이름
	WndClass.style = CS_HREDRAW | CS_VREDRAW; //윈도우 출력 스타일
											  // 여기까지 WNDCLASS구조체의 변수들에 값을 대입

	RegisterClass(&WndClass); // 윈도우 클래스를 운영체제에 등록

	hWnd = CreateWindow(lpszClass, "Windows Program 1-1", WS_OVERLAPPEDWINDOW, 0, 0, 1600, 800, NULL, (HMENU)NULL, hinstance, NULL);
	//CreateWindow(클래스 이름,타이틀 이름,윈도우 스타일, 윈도우 위치좌표x , y,윈도우 가로크기, 윈도우 세로크기, 부모 윈도우 핸들, 메뉴 핸들, 응용프로그램 인스턴스, 생성 윈도우 정보(NULL))
	ShowWindow(hWnd, nCmdShow); //윈도우의 화면 출력
	UpdateWindow(hWnd); //OS에 WM_PAINT메시지 전송
	while (GetMessage(&Message, NULL, 0, 0))
	{
		//윈도우 프로시저에서 PostQuitMessage()를 호출할때 종료됨.
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}//메시지루프를 돌림.
	return (int)Message.wParam;
}










static BOOL JUMP = FALSE;
static int moveSkySpeed = 5;
static int moveGroundSpeed = 2;
static int zoom = 0;
static BOOL charter1 = FALSE;
static BOOL CharterText = FALSE;
static BOOL MobText = FALSE;
BOOL CALLBACK Dlg6_3Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hButton;
	HDC hdc;
	static int x = 0, y = 100;
	static int count = 0;
	static int dir = 0;
	static int xdir = 0;
	static BOOL DownCheck = FALSE;
	static HBITMAP hBitmap;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	switch (iMsg)
	{
	case WM_INITDIALOG:
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_STOP:
			EndDialog(hDlg, 0);
			break;
			
		case IDC_JUMP:
			JUMP = TRUE;
			break;
			
		case IDC_FLAT:
			if (DownCheck == FALSE) {
				DownCheck = TRUE;
				WndProc(hDlg, WM_KEYDOWN, VK_DOWN, lParam);
			}
			else if (DownCheck == TRUE) {
				DownCheck = FALSE;
				WndProc(hDlg, WM_KEYUP, VK_DOWN, lParam);
			}
			break;

		case IDC_FRONT:
			WndProc(hDlg, WM_KEYUP, VK_RIGHT, lParam);
			WndProc(hDlg, WM_KEYDOWN, VK_LEFT, lParam);
			break;

		case IDC_BACK:
			WndProc(hDlg, WM_KEYDOWN, VK_RIGHT, lParam);

			break;

		case IDC_SPEEDUP:
			moveSkySpeed += 2;
			moveGroundSpeed += 2;
			break;
			
		case IDC_SPEEDDOWN:
			moveSkySpeed -= 2;
			moveGroundSpeed -= 2;
			break;

		case IDC_ZOOM:
			zoom = 200 * 0.2;
			break;

		case IDC_RESIZE:
			zoom = -(200 * 0.2);
			break;

		case IDC_RESET:
			zoom = 0;
			break;

		case IDC_CHARTER1:
			charter1 = FALSE;
			break;

		case IDC_CHARTER2:
			charter1 = TRUE;
			break;

		case IDC_CHECK1:
			if (CharterText == FALSE)
				CharterText = TRUE;
			else
				CharterText = FALSE;
			break;
		case IDC_CHECK2:
			if (MobText == FALSE)
				MobText = TRUE;
			else
				MobText = FALSE;
			break;
		}
		break;

	case WM_TIMER:

		break;
	case WM_PAINT:

		break;
	}
	return 0;
}









LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK(윈도우 핸들, 처리해야 할 메시지의 값,입력장치의 값1,2)
{
	PAINTSTRUCT ps;
	HDC hdc, memdc, memdc2, chardc; //화면 출력을 위한 DC들 chardc는 캐릭터dc입니다.
	 //전부다 합쳐서 출력을 준비하는 DC들
	HBITMAP runBit[6], airPlane[4], cupBanana[4], boom[3];
//	CImage g_cimgTest;
//	HDC adddc = g_cimgTest; 
	HBITMAP backGroundSky, backGroundGround, groundBit, oldgroundBit, skyBit, oldSkyBit, charBit, oldCharBit, addBit;
	//HDC gdc = g_cimgTest.GetDC();
	static HBITMAP hbit, hbit2, oldBit1, oldBit2; 
	static RECT rtChar;                              //주인공의 캐릭터 영역
	static RECT rtplane;							//충돌체크할 비행기 바나나 영역
	static RECT rtCup;								//충돌체크할 컵 바나나 영역
	static int x = 1300;
	static int y = 500;
	static int yPos = 0;
	static int jump;
	static int size;
	char text[100] = "과제좀 줄여주세요ㅠㅠㅠㅠㅠ";
	static int mx, my;
	static int planeX, planeY;
	static int cupX, cupY;
	static int count = 0;
	static int mobCount = 0;
	static int down = 0;
	static int upcount = 0;
	static BOOL jumpCheck = FALSE;
	static CString str;
	static BOOL right = FALSE;
	static BOOL crash = FALSE;
	static BOOL boomTrue = FALSE;
	static int boomCount = 0;
	static int boomC = 0;
	static int moveSky = 0, moveGround = 0;
	switch (iMessage) //메시지의 번호
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 1, NULL);
		SetTimer(hWnd, 2, 10, NULL);
		planeX = -150;
		cupX = -100;
		planeY = rand() % 550 + 150;
		cupY = rand() % 100 + 550;
		rtChar.left = x + 45;
		rtChar.top = y + 35;
		rtChar.right = x + 150;
		rtChar.bottom = y + 140;
	
		rtplane.left = planeX + 10;
		rtplane.top = planeY + 10;
		rtplane.right = planeX + 110;
		rtplane.bottom = planeY + 110;

		rtCup.left = cupX + 10;
		rtCup.top = cupY + 30;
		rtCup.right = cupX + 100;
		rtCup.bottom = cupY + 110;
		break;
	case WM_KEYDOWN:
		if (wParam == VK_LEFT)
		{
			x -= 10;
			rtChar.left = x + 45;
			rtChar.top = y + 35;
			rtChar.right = x + 150;
			rtChar.bottom = y + 140;
		}
		else if (wParam == VK_RIGHT)
		{
			x += 10;
			rtChar.left = x + 45;
			rtChar.top = y + 35;
			rtChar.right = x + 150;
			rtChar.bottom = y + 140;
			right = TRUE;
		}
		else if (wParam == VK_DOWN)
		{
			down = 50;
		}
		else if (wParam == VK_UP)
		{
			if (jumpCheck == FALSE) {
				jumpCheck = TRUE;
				SetTimer(hWnd, 3, 10, NULL);
			}
		}
		break;

	case WM_KEYUP:
		if (wParam == VK_RIGHT)
			right = FALSE;
		else if (wParam == VK_DOWN)
			down = 0;
		break;

	case WM_CHAR:
		switch (wParam)
		{
			if (wParam == 'p' || wParam == 'P')
				DialogBox(g_hinst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dlg6_3Proc);
		}
		break;

	case WM_LBUTTONDOWN:
		DialogBox(g_hinst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dlg6_3Proc);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case 1: // 그냥 애니메이션 때문에 있음... 묻지마....
			cupX += 20;
			planeX += 7;
			rtplane.left = planeX + 10;
			rtplane.top = planeY + 10;
			rtplane.right = planeX + 120;
			rtplane.bottom = planeY + 110;

			rtCup.left = cupX + 10;
			rtCup.top = cupY + 30;
			rtCup.right = cupX + 100;
			rtCup.bottom = cupY + 110;

			if (planeX >= 1750) {
				planeX = 0;
				planeX = planeX - rand() % 500;
				planeY = rand() % 150 + 150;
				rtplane.left = planeX + 10;
				rtplane.top = planeY + 10;
				rtplane.right = planeX + 120;
				rtplane.bottom = planeY + 110;
			}

			if (cupX >= 1750) {
				cupX = 0;
				cupX = cupX - rand() % 500;
				cupY = rand() % 100 + 550;
				rtCup.left = cupX + 10;
				rtCup.top = cupY + 30;
				rtCup.right = cupX + 100;
				rtCup.bottom = cupY + 110;
			}
			if (rtCup.left < rtChar.right && rtCup.right > rtChar.left &&
				rtCup.top < rtChar.bottom && rtCup.bottom > rtChar.top)
			{
				// 충돌
				crash = TRUE;
				boomTrue = TRUE;
				SetTimer(hWnd, 5, 10, NULL);
				cupX = 0;
				cupX = cupX - rand() % 500;
				cupY = rand() % 100 + 550;
				rtCup.left = cupX + 10;
				rtCup.top = cupY + 30;
				rtCup.right = cupX + 100;
				rtCup.bottom = cupY + 110;
				SetTimer(hWnd, 4, 100, NULL);
			}
			else
			{
				crash = FALSE;
			}

			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case 2: // 자동 이동중인 맵들
			moveSky += moveSkySpeed;
			moveGround += moveGroundSpeed;
			if (moveSky >= 1600)
				moveSky = 0;
			if (moveGround >= 1600)
				moveGround = 0;
			break;

		case 3:
			if (upcount < 20) {
				yPos -= 10;
				rtChar.left = x + 35;
				rtChar.top = y + 35 - yPos;
				rtChar.right = x + 150;
				rtChar.bottom = y + 150 - yPos;
				upcount++;
			}
			else {
				yPos += 10;
				rtChar.left = x + 45;
				rtChar.top = y + 35 + yPos;
				rtChar.right = x + 150;
				rtChar.bottom = y + 150 + yPos;
				upcount++;
			}
			if (upcount == 40) {
				upcount = 0;
				jumpCheck = FALSE;
				JUMP = FALSE;
				KillTimer(hWnd, 3);

			}
			break;

		case 5:
			if (boomC > 6) {
				boomTrue = FALSE;
				boomC = 0;
				KillTimer(hWnd, 5);
			}
			else
				boomC++;

		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		memdc2 = CreateCompatibleDC(memdc);
	//	gdc = CreateCompatibleDC(memdc);

		if (JUMP == TRUE) {
			if (jumpCheck == FALSE) {
				jumpCheck = TRUE;
				SetTimer(hWnd, 3, 10, NULL);
			}
		}

		hbit2 = LoadBitmap(g_hinst, MAKEINTRESOURCE(BackGroundSky));
		hbit = CreateCompatibleBitmap(hdc, 1600, 800);
		count++;
		mobCount++;
		boomCount++;
		boomCount = boomCount % 3;
		mobCount = mobCount % 4;
		count = count % 6;
		//str.Format("D:\5-5~1");

		//g_cimgTest.Load(_T("boom.gif"));
		if (right == FALSE && charter1 == FALSE) {
			runBit[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(Nomally1));
			runBit[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(Nomally2));
			runBit[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(Nomally3));
			runBit[3] = LoadBitmap(g_hinst, MAKEINTRESOURCE(Nomally4));
			runBit[4] = LoadBitmap(g_hinst, MAKEINTRESOURCE(Nomally5));
			runBit[5] = LoadBitmap(g_hinst, MAKEINTRESOURCE(Nomally6));
		}
		else if (right == TRUE && charter1 == FALSE) {
			runBit[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(ChangeChar1));
			runBit[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(ChangeChar2));
			runBit[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(ChangeChar3));
			runBit[3] = LoadBitmap(g_hinst, MAKEINTRESOURCE(ChangeChar4));
			runBit[4] = LoadBitmap(g_hinst, MAKEINTRESOURCE(ChangeChar5));
			runBit[5] = LoadBitmap(g_hinst, MAKEINTRESOURCE(ChangeChar6));
		}

		else if(charter1 == TRUE) {
			runBit[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(AirPlaneBanana1));
			runBit[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(AirPlaneBanana2));
			runBit[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(AirPlaneBanana3));
			runBit[3] = LoadBitmap(g_hinst, MAKEINTRESOURCE(AirPlaneBanana4));
		}

		airPlane[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(AirPlaneBanana1));
		airPlane[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(AirPlaneBanana2));
		airPlane[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(AirPlaneBanana3));
		airPlane[3] = LoadBitmap(g_hinst, MAKEINTRESOURCE(AirPlaneBanana4));

		cupBanana[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(CupBanana1));
		cupBanana[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(CupBanana2));
		cupBanana[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(CupBanana3));
		cupBanana[3] = LoadBitmap(g_hinst, MAKEINTRESOURCE(CupBanana4));

		if (boomTrue == TRUE) {
			boom[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(Crash1));
			boom[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(Crash2));
			boom[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(Crash3));
		}
		oldBit1 = (HBITMAP)SelectObject(memdc, hbit);
		oldBit2 = (HBITMAP)SelectObject(memdc2,NULL);
		skyBit = LoadBitmap(g_hinst, MAKEINTRESOURCE(BackGroundSky));
		oldSkyBit = (HBITMAP)SelectObject(memdc2, skyBit);
		StretchBlt(memdc, 0 + moveSky, 0, 1600, 800, memdc2, 0, 0, 2000,1000, SRCCOPY);
		StretchBlt(memdc, -1600 + moveSky, 0, 1600, 800, memdc2, 0, 0, 2000,1000,SRCCOPY);
		SelectObject(memdc2, skyBit);
		groundBit = LoadBitmap(g_hinst, MAKEINTRESOURCE(BackGroundGround));
		oldgroundBit = (HBITMAP)SelectObject(memdc2, groundBit);
		TransparentBlt(memdc, 0 + moveGround, 0, 1600, 800, memdc2, 0, 0, 2000, 1000, RGB(0, 0, 0)); //크기확대는 첫번쨰에서 결정함.
		TransparentBlt(memdc, -1600 + moveGround, 0, 1600, 800, memdc2, 0, 0, 2000, 1000, RGB(0, 0, 0));
		SelectObject(memdc2, oldgroundBit);
		
		if (charter1 == FALSE) {
			(HBITMAP)SelectObject(memdc2, runBit[count]);
			TransparentBlt(memdc, x, y + yPos + down, 200 + zoom, (200 - down) + zoom, memdc2, 0, 0, 220, 220, RGB(0, 255, 0)); //크기확대는 첫번쨰에서 결정함.
			SelectObject(memdc2, NULL);// 캐릭터
		}
		else if (charter1 == TRUE) {
			(HBITMAP)SelectObject(memdc2, runBit[mobCount]);
			TransparentBlt(memdc, x, y + yPos + down, 200 + zoom, (200 - down) + zoom, memdc2, 0, 0, 150, 150, RGB(0, 0, 255)); //크기확대는 첫번쨰에서 결정함.
			SelectObject(memdc2, NULL);// 캐릭터
		}

		if (CharterText == TRUE) {
			SetBkColor(memdc, RGB(255, 255, 0));
			TextOut(memdc, x, y - 30 + yPos + down, text, strlen(text));
		}

		
		
		(HBITMAP)SelectObject(memdc2, airPlane[mobCount]);  // 비행기탄 바나나
		TransparentBlt(memdc, planeX, planeY , 150, 150, memdc2, 0, 0, 150, 150, RGB(0, 0, 255)); //크기확대는 첫번쨰에서 결정함.
		SelectObject(memdc2, NULL);// 캐릭터

		if (MobText == TRUE) {
			(HBITMAP)SelectObject(memdc2, airPlane[mobCount]);  // 비행기탄 바나나
			TransparentBlt(memdc, planeX - 200, planeY, 150, 150, memdc2, 0, 0, 150, 150, RGB(0, 0, 255)); //크기확대는 첫번쨰에서 결정함.
			SelectObject(memdc2, NULL);// 캐릭터
		}
		
		(HBITMAP)SelectObject(memdc2, cupBanana[mobCount]); // 돌아가는 컵케잌 바나나
		TransparentBlt(memdc, cupX, cupY, 150, 150 , memdc2, 0, 0, 150, 150, RGB(0, 0, 255)); //크기확대는 첫번쨰에서 결정함.
		SelectObject(memdc2, NULL);// 캐릭터

		if (MobText == TRUE) {
			(HBITMAP)SelectObject(memdc2, cupBanana[mobCount]); // 돌아가는 컵케잌 바나나
			TransparentBlt(memdc, cupX - 200, cupY, 150, 150, memdc2, 0, 0, 150, 150, RGB(0, 0, 255)); //크기확대는 첫번쨰에서 결정함.
			SelectObject(memdc2, NULL);// 캐릭터
		}
		if (boomTrue == TRUE) {
			(HBITMAP)SelectObject(memdc2, boom[boomCount]);  // 비행기탄 바나나
			TransparentBlt(memdc, x, y, 150, 150, memdc2, 0, 0, 100, 100, RGB(0, 255, 0)); //크기확대는 첫번쨰에서 결정함.
			SelectObject(memdc2, NULL);// 캐릭터
		}
	//	g_cimgTest.Load(str);
	//	g_cimgTest.Draw(adddc, 0, 0, 165, 217);
	//	g_cimgTest.TransparentBlt(memdc, x, y, 150, 150, memdc2, 0, 0, 168, 217,(RGB(0,0,0)));
		BitBlt(hdc, 0, 0, 1600, 800, memdc, 0, 0, SRCCOPY);



		SelectObject(memdc, oldBit1);

		for (int i = 0; i < 3; ++i)
			DeleteObject(boom[i]);

		for (int i = 0; i < 6; ++i)
		{
			DeleteObject(runBit[i]);
		}
		for (int k = 0; k < 4; ++k)
		{
			DeleteObject(cupBanana[k]);
			DeleteObject(airPlane[k]);
		}
		DeleteObject(skyBit);
		DeleteObject(groundBit);
		DeleteObject(hbit);
		DeleteDC(memdc);
		DeleteDC(memdc2);
		DeleteObject(hbit2);

		

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
	//case에서 정의되지 않은 메시지는 커널이 처리하도록 메시지 전달
}



BOOL CollisionCheck(int x, int y, int mx, int my)
{
	if (mx > x&&mx<x + 150 && my>y&&my < y + 150)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
