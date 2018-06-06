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

	hWnd = CreateWindow(lpszClass, "Windows Program 1-1", WS_OVERLAPPEDWINDOW, 0, 0, 520, 330, NULL, (HMENU)NULL, hinstance, NULL);
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
	HBITMAP myBitmap, mybitmap2, mybitmap3, mybitmap4,oldBitmap , oldBitmap2, oldBitmap3, oldBitmap4;
	static int menu;
	static int turn = 1;
	static RECT rect;
	static BOOL drag;
	static int mx, my, size;
	static int sizeview;
	HPEN hPen, oldPen;
	static int copy = 0;
	static int cx1, cy1, cx2, cy2;
	static double viewx = 0;
	static double viewy = 0;
	static int count = 0;
	static int starttempx, starttempy, endtempx, endtempy;
	static int startx, starty, ox, oy, endx, endy;
	static int change = 0;
	switch (iMessage) //메시지의 번호
	{
	case WM_CREATE:
		break;
	case WM_SIZE:
		GetClientRect(hWnd, &rect);
		break;
	case WM_CHAR:
		if (wParam == '1') {
			viewx = abs(startx - endx) * 0.2;
			viewy = abs(starty - endy) * 0.2;
		}
		else if (wParam == '2') {
			viewx = abs(startx - endx) * 0.4;
			viewy = abs(starty - endy) * 0.4;
		}
		else if (wParam == '0') {
			viewx = 0;
			viewy = 0;
		}
		else if (wParam == 'w') {
			starty -= 5;
		}
		else if (wParam == 'a') {
			startx -= 5;
		}
		else if (wParam == 's') {
			endy += 5;
		}
		else if (wParam == 'd') {
			endx += 5;
		}
		else if (wParam == 'e') { // 축소
			startx += 5;
			endx -= 5;
			starty += 5;
			endy -= 5;
		}
		else if (wParam == 'h') { // 확장
			startx -= 5;
			endx += 5;
			starty -= 5;
			endy += 5;
		}
		else if (wParam == 'c') {
			starttempx = startx + viewx;
			starttempy = starty + viewy;
			endtempx = endx - startx - viewx;
			endtempy = endy - starty - viewy;
		}
		else if (wParam == 'p') {
			count++;
		}
		else if (wParam == 'n') {
				change = 1;
		}
		else if (wParam == 'm') {
				change = 2;
		}
		else if (wParam == 'r') {
			change = 0;
			startx = 0;
			starty = 0;
			endx = 0;
			endy = 0;
			starttempx = 0;
			starttempy = 0;
			endtempx = 0;
			endtempy = 0;
			change = 0;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_LBUTTONDOWN:
		startx = LOWORD(lParam);
		starty = HIWORD(lParam);
		drag = TRUE;
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hWnd);
		hmemdc = CreateCompatibleDC(hdc);
		if (drag)
		{ // 흰 바탕 
			endx = LOWORD(lParam);
			endy = HIWORD(lParam);
			SetROP2(hdc, R2_XORPEN); // 펜의 XOR 연산
			SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));
			Rectangle(hdc, startx, starty, endx, endy);
			Rectangle(hdc, startx, starty, ox, oy);
			//Rectangle(hmemdc, startx, starty, ox, oy);
			ox = endx;
			oy = endy; // 현 지점을 이전 지점으로 설정
			
		}
		ReleaseDC(hWnd, hdc);
		ReleaseDC(hWnd, hmemdc);
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_DOWN:
			starty += 5;
			endy += 5;
			break;
		case VK_UP:
			starty -= 5;
			endy -= 5;
			break;
		case VK_LEFT:
			endx -= 5;
			startx -= 5;
			break;
		case VK_RIGHT:
			endx += 5;
			startx += 5;
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_LBUTTONUP:
		viewx = 0;
		viewy = 0;
		count = 0;
		drag = FALSE;
		break;
	case WM_PAINT: //메시지의 처리
		hdc = BeginPaint(hWnd, &ps);
		hmemdc = CreateCompatibleDC(hdc);
		myBitmap = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP1));
		oldBitmap = (HBITMAP)SelectObject(hmemdc, myBitmap);
		
		
		

		StretchBlt(hdc, 0, 0, 520, 320, hmemdc, 0, 0, 530, 320, SRCCOPY);  //배경화면
		if (count == 0) {
			if (change == 0) {
				StretchBlt(hdc, startx, starty, endx - startx, endy - starty, hmemdc, startx + viewx, starty + viewy, endx - startx - viewx, endy - starty - viewy, SRCCOPY);
			}
			else if (change == 1) {
				StretchBlt(hdc, startx, starty, endx - startx, endy - starty, hmemdc, endx - startx - viewx, starty + viewy, -(endx - startx - viewx), endy - starty - viewy, SRCCOPY);
			}
			else if (change == 2) {
				StretchBlt(hdc, startx, starty, endx - startx, endy - starty, hmemdc, startx + viewx, endy - starty - viewy, endx - startx - viewx, -(endy - starty - viewy), SRCCOPY);
			}
		}
		else {
			StretchBlt(hdc, startx, starty, endx - startx, endy - starty, hmemdc, starttempx, starttempy, endtempx, endtempy, SRCCOPY);
		}
	//	StretchBlt(hdc, sx + mx, sy + my, ox - sx + mx + size, oy - sy + my + size, hmemdc, sx + mx, sy + my, ox - sx + mx + size - sizeview, oy - sy + my + size - sizeview, SRCCOPY); //사각형
		if (copy == 1)
		{
			StretchBlt(hdc, 0, 0, cx2, cy2, hmemdc, cx1, cy1, cx2, cy2, SRCCOPY);
		}
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