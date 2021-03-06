#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <atlImage.h>
#include <math.h>
#include <time.h>
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

	hWnd = CreateWindow(lpszClass, "Windows Program 1-1", WS_OVERLAPPEDWINDOW, 0, 0, 615, 630, NULL, (HMENU)NULL, hinstance, NULL);
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

struct map {
	int hdcX;
	int hdcY;
	int memdcX;
	int memdcY;
	int index = -1;
	int memdcindex = -1;
	int check = 0;
};
static int num[9];

struct Drop {
	int x;
	int y;
	int xpos;
	int ypos;
	int index;
};
int ran(int n, int select) {
	int i = 0;
	int rNum = 0;

	srand(time(NULL));

	for (i = 0; i < n; ++i) {
		if (num[i] == rNum) {
			rNum = rand() % select;
			i = -1;
		}
	}
	return rNum;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK(윈도우 핸들, 처리해야 할 메시지의 값,입력장치의 값1,2)
{
	PAINTSTRUCT ps;
	HDC hdc, memdc, memdc2;
	//	CImage g_cimgTest;
	//	HDC adddc = g_cimgTest; 
	HBITMAP runMap;
	//HDC gdc = g_cimgTest.GetDC();
	static HBITMAP hbit, hbit2, oldBit1, oldBit2; //hbit는 hdc에 호환되는  판을 그림, hbit2는 이미지를 가져온다(더블버퍼링)
	static int count = 0;
	static int temp;
	static int select;
	static int cnt = 0;
	static map Map[5][5];
	static map randMap[5][5];
	static BOOL gameStart = FALSE;
	static int deleteX, deleteY; // 맵에서 랜덤으로 몆번째칸을 지울지를 정함
	static int tempX = 0, tempY = 0; // tempX, tempY는 클릭한 공간의 x, y좌표를 저장후 사용한다.
	static int inputX, inputY;  // 마우스가 어딜 입력 받았는지? 저장
	static int xCount = 0;      //이동하는 카운트이다. 왜 xCount인지는 나도모름...;;
	static int imHdcX, imHdcY, imX, imY, imX2, imY2, imX3, imY3, imX4, imY4, imCheck2, imCheck3, imCheck4, imCheck; // 이동하고나서 SWAP(빈공간과 내가 움직이는중인 그림을 서로 위치를 바꾸기위해..)을 할때 사용
	static BOOL Click = FALSE;  // 이동중에는 클릭이 막히도록 설정
	static BOOL complete = FALSE; // F키를 누르면 완성된 모양이 보이므로 누른다음 약 1초간 보이게 설정중이다.
	static int completeCount = 0; // F키를 눌러서 완성된 모양을 볼때 다시 돌리기위한 카운트
	srand((unsigned)time(NULL));
	//srand((unsigned)time(NULL));

	switch (iMessage) //메시지의 번호
	{
	case WM_CREATE:

		break;

	case WM_LBUTTONDOWN:


		break;

	case WM_KEYDOWN:

		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_TIMER:

		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		memdc2 = CreateCompatibleDC(memdc);
		//	gdc = CreateCompatibleDC(memdc);
		//		hbit2 = LoadBitmap(g_hinst, MAKEINTRESOURCE(BackGroundSky));
		hbit = CreateCompatibleBitmap(hdc, 630, 630);

		oldBit1 = (HBITMAP)SelectObject(memdc, hbit);
		oldBit2 = (HBITMAP)SelectObject(memdc2, runMap);


		//	BitBlt(hdc, 0, 0, 1280, 1080, memdc, 0, 0, SRCCOPY);


		BitBlt(hdc, 0, 0, 1280, 1080, memdc, 0, 0, SRCCOPY);
		SelectObject(memdc2, NULL);
		SelectObject(memdc, oldBit1);


		for (int i = 0; i < 6; ++i) {
			DeleteObject(runMap);
		}

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
