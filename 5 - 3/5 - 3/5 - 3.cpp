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

	hWnd = CreateWindow(lpszClass, "Windows Program 1-1", WS_OVERLAPPEDWINDOW, 0, 0, 1100, 350, NULL, (HMENU)NULL, hinstance, NULL);
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
	HBITMAP myBitmap, mybitmap2, mybitmap3, mybitmap4, oldBitmap, oldBitmap2, oldBitmap3, oldBitmap4;
	static RECT rect;
	static BOOL drag;
	static int size = 0;

	HPEN hPen, oldPen;

	switch (iMessage) //메시지의 번호
	{
	case WM_CREATE:
		break;
	case WM_SIZE:
		GetClientRect(hWnd, &rect);
		break;
	case WM_CHAR:
		if (wParam == '1') {
			size = 1;
		}
		else if (wParam == '2') {
			size = 2;
		}
		else if (wParam == '3') {
			size = 3;
		}
		else if (wParam == '4') {
			size = 4;
		}
		else if (wParam == '5') {
			size = 5;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONDOWN:

		break;
	case WM_MOUSEMOVE:
	
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_KEYDOWN:
		
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_LBUTTONUP:

		break;
	case WM_PAINT: //메시지의 처리
		hdc = BeginPaint(hWnd, &ps);
		hmemdc = CreateCompatibleDC(hdc);
		myBitmap = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP1));
		oldBitmap = (HBITMAP)SelectObject(hmemdc, myBitmap);

	
		StretchBlt(hdc, 0, 0, 520, 320, hmemdc, 0, 0, 530, 320, SRCCOPY);  //배경화면
		hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		oldPen = (HPEN)SelectObject(hdc, hPen);	
		MoveToEx(hdc, 110, 0, NULL);
		LineTo(hdc, 110, 330);
		MoveToEx(hdc, 220, 0, NULL);
		LineTo(hdc, 220, 330);

		MoveToEx(hdc, 0, 110, NULL);
		LineTo(hdc, 330, 110);
		MoveToEx(hdc, 0, 220, NULL);
		LineTo(hdc, 330, 220);

		hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		Rectangle(hdc, 600, 0, 930, 330);
		for (int i = 0; i < size; ++i) {
			
			MoveToEx(hdc, 700 + i * 110, 110, NULL);
			LineTo(hdc, 700, 110);
			MoveToEx(hdc, 0, 220, NULL);
			LineTo(hdc, 330, 220);
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