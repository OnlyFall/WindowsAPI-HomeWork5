#include <Windows.h>
#include "resource.h"
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hinst;
LPCTSTR lpszClass = TEXT("First");

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

	hWnd = CreateWindow(lpszClass, "Windows Program 1-1", WS_OVERLAPPEDWINDOW, 0, 0, 810, 630, NULL, (HMENU)NULL, hinstance, NULL);
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK
																				 //(윈도우 핸들, 처리해야 할 메시지의 값,입력장치의 값1,2)
{
	PAINTSTRUCT ps;
	HDC hdc, hmemdc;
	static HBITMAP hbmOld, hbmMem, hbmMemOld;
	static HDC memDC;
	static HBITMAP hBitmap;
	HBITMAP myBitmap, oldBitmap;
	HBRUSH hBrush, oldBrush;
	HPEN hPen, oldPen;
	static DWORD inverse;
	static int menu;
	static int Right = 0;
	static int turn = 0;
	static RECT rect;
	static int select = 1;
	static int inputX, inputY;
	static int stress1 = 0, stress2 = 0, stress3 = 0, stress4 = 0;

	switch (iMessage) //메시지의 번호
	{
	case WM_CREATE:
		inverse = (DWORD)0x00CC0020;
		break;
	case WM_SIZE:
		GetClientRect(hWnd, &rect);
		break;

	case WM_LBUTTONDOWN:
		inputX = LOWORD(lParam);
		inputY = HIWORD(lParam);

		if ((inputX >= 0 && inputX < 400) && (inputY >= 0 && inputY < 300)) {
			select = 1;
		}
		else if ((inputX >= 400 && inputX < 800) && (inputY >= 0 && inputY < 300)) {
			select = 2;
		}
		else if ((inputX >= 0 && inputX < 400) && (inputY >= 300 && inputY < 600)){
			select = 3;
		}
		else if ((inputX >= 400 && inputX < 800) && (inputY >= 300 && inputY < 600)) {
			select = 4;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_CHAR:
		if (wParam == 'a')
		{
			menu = 1;
			select = 0;
		}
		else if (wParam == 'r')
		{
			turn++;
			if (turn % 2 == 0) {
				inverse = (DWORD)0x00CC0020;
			}
			else {
				inverse = (DWORD)0x00660046;
			}
		}
		else if (wParam == '2') {
			menu = 3;
		}

		
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_KEYDOWN:
		// 전체화면일때 좌우 이동
		if (select == 0) {
			if (wParam == VK_RIGHT)
			{
				if (Right >= 800) {
					Right = 0;
				}
				Right += 20;
			}
			else if (wParam == VK_LEFT) {
				if (Right <= -800) {
					Right = 0;
				}
				Right -= 20;
			}
		}
		
		// 4분할일때 화면 이동
		else {
			if (select == 1) { //1사분면
				if (wParam == VK_RIGHT)
				{
					if (stress1 >= 530) {
						stress1 = 0;
					}
					stress1 += 10;
				}
				else if (wParam == VK_LEFT) {
					if (stress1 <= -530) {
						stress1 = 0;
					}
					stress1 -= 10;
				}
			}

			else if (select == 2) { //2사분면
				if (wParam == VK_RIGHT)
				{
					if (stress2 >= 530) {
						stress2 = 0;
					}
					stress2 += 10;
				}
				else if (wParam == VK_LEFT) {
					if (stress2 <= -530) {
						stress2 = 0;
					}
					stress2 -= 10;
				}
			}

			else if (select == 4) { //3사분면
				if (wParam == VK_RIGHT)
				{
					if (stress3 >= 530) {
						stress3 = 0;
					}
					stress3 += 10;
				}
				else if (wParam == VK_LEFT) {
					if (stress3 <= -530) {
						stress3 = 0;
					}
					stress3 -= 10;
				}
			}
			else if (select == 3) { //4사분면
				if (wParam == VK_RIGHT)
				{
					if (stress4 >= 530) {
						stress4 = 0;
					}
					stress4 += 10;
				}
				else if (wParam == VK_LEFT) {
					if (stress4 <= -530) {
						stress4 = 0;
					}
					stress4 -= 10;
				}
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT: //메시지의 처리
		hdc = BeginPaint(hWnd, &ps);
		hmemdc = CreateCompatibleDC(hdc);
		myBitmap = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP1));
		oldBitmap = (HBITMAP)SelectObject(hmemdc, myBitmap);
		if (menu == 0)
		{
			BitBlt(hdc, 0, 0, 530, 320, hmemdc, 0, 0, inverse);
		}
		else if (menu == 1) {
			StretchBlt(hdc, 0 + Right, 0, 800, 600, hmemdc, 0, 0, 530, 320, inverse);
			StretchBlt(hdc, -800 + Right, 0, 800, 600, hmemdc, 0, 0, 530, 320, inverse);
			StretchBlt(hdc, 800 + Right, 0, 800 , 600, hmemdc, 0, 0, 530, 320, inverse);

		}
		else if (menu == 2)
		{
			StretchBlt(hdc, 0, 0, 800, 600, hmemdc, 0, 0, 800, 600, inverse);
		}
		else if (menu == 3)
		{
			
			//1사분면
			//StretchBlt(hdc, 0 + stress1, 0, 400, 300, hmemdc, 0, 0, 530, 320, (DWORD)0x00CC0020);
			StretchBlt(hdc, 0, 0, 400, 300, hmemdc, 0 - stress1, 0, 530, 320, (DWORD)0x00CC0020);
			StretchBlt(hdc, 0, 0, 400, 300, hmemdc, 530 - stress1, 0, 530, 320, (DWORD)0x00CC0020);
			StretchBlt(hdc, 0, 0, 400, 300, hmemdc, -530 - stress1, 0, 530, 320, (DWORD)0x00CC0020);
			//2사분면
			StretchBlt(hdc, 400 , 0, 400, 300, hmemdc, 0 - stress2, 0, 530, 320, (DWORD)0x00660046);
			StretchBlt(hdc, 400, 0, 400, 300, hmemdc, 530 - stress2, 0, 530, 320, (DWORD)0x00660046);
			StretchBlt(hdc, 400, 0, 400, 300, hmemdc, -530 - stress2, 0, 530, 320, (DWORD)0x00660046);
			//3사분면
			StretchBlt(hdc, 400, 300, 400, 300, hmemdc, 0 - stress3, 0, 530, 320, (DWORD)0x00CC0020);
			StretchBlt(hdc, 400 , 300, 400, 300, hmemdc, 530 - stress3, 0, 530, 320, (DWORD)0x00CC0020);
			StretchBlt(hdc, 400 , 300, 400, 300, hmemdc, -530 - stress3, 0, 530, 320, (DWORD)0x00CC0020);
			//4사분면
			StretchBlt(hdc, 0 , 300, 400, 300, hmemdc, 0 - stress4, 0, 530, 320, (DWORD)0x00660046);
			StretchBlt(hdc, 0 , 300, 400, 300, hmemdc, 530 - stress4, 0, 530, 320, (DWORD)0x00660046);
			StretchBlt(hdc, 0 , 300, 400, 300, hmemdc, -530 - stress4, 0, 530, 320, (DWORD)0x00660046);


			//1사분면
			if (select == 1) {
				hPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
				oldPen = (HPEN)SelectObject(hdc, hPen);
				MoveToEx(hdc, 400, 0, NULL);
				LineTo(hdc, 400, 300);
				MoveToEx(hdc, 0, 300, NULL);
				LineTo(hdc, 400, 300);
			}
			else {
				hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				oldPen = (HPEN)SelectObject(hdc, hPen);
				MoveToEx(hdc, 400, 0, NULL);
				LineTo(hdc, 400, 300);
				MoveToEx(hdc, 0, 300, NULL);
				LineTo(hdc, 400, 300);
			}
			//2사분면
			if (select == 2) {
				hPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
				oldPen = (HPEN)SelectObject(hdc, hPen);
				MoveToEx(hdc, 400, 0, NULL);
				LineTo(hdc, 400, 300);
				MoveToEx(hdc, 400, 300, NULL);
				LineTo(hdc, 800, 300);
			}
			else {
				hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				oldPen = (HPEN)SelectObject(hdc, hPen);
				MoveToEx(hdc, 400, 0, NULL);
				LineTo(hdc, 400, 300);
				MoveToEx(hdc, 400, 300, NULL);
				LineTo(hdc, 800, 300);
			}
			//3사분면
			if (select == 3) {
				hPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
				oldPen = (HPEN)SelectObject(hdc, hPen);
				MoveToEx(hdc, 0, 300, NULL);
				LineTo(hdc, 400, 300);
				MoveToEx(hdc, 400, 300, NULL);
				LineTo(hdc, 400, 600);
			}
			else {
				hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				oldPen = (HPEN)SelectObject(hdc, hPen);
				MoveToEx(hdc, 0, 300, NULL);
				LineTo(hdc, 400, 300);
				MoveToEx(hdc, 400, 300, NULL);
				LineTo(hdc, 400, 600);
			}
			//4사분면
			if (select == 4) {
				hPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
				oldPen = (HPEN)SelectObject(hdc, hPen);
				MoveToEx(hdc, 400, 300, NULL);
				LineTo(hdc, 800, 300);
				MoveToEx(hdc, 400, 300, NULL);
				LineTo(hdc, 400, 600);
			}
			else {
				hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				oldPen = (HPEN)SelectObject(hdc, hPen);
				MoveToEx(hdc, 400, 300, NULL);
				LineTo(hdc, 800, 300);
				MoveToEx(hdc, 400, 300, NULL);
				LineTo(hdc, 400, 600);
			}

		}
		BitBlt(hdc, 0, 0, 800, 600, memDC, 0, 0, SRCCOPY);

		SelectObject(hmemdc, oldBitmap);
		DeleteObject(myBitmap);
		DeleteDC(hmemdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
	//case에서 정의되지 않은 메시지는 커널이 처리하도록 메시지 전달
}