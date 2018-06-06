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
void Animation(int xPos, int yPos, int x3, int y3, HDC hdc, int s, int dir, BOOL other, BOOL other2,BOOL Second, int select);
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

	hWnd = CreateWindow(lpszClass, "Windows Program 1-1", WS_OVERLAPPEDWINDOW, 0, 0, 800, 800, NULL, (HMENU)NULL, hinstance, NULL);
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK(윈도우 핸들, 처리해야 할 메시지의 값,입력장치의 값1,2)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static int x, x3;
	static int y, y3;
	static int jump;
	static int size;
	static int x1 = 0, x2 = 0;
	static int select = 0;
	static int other = 0;
	static int count = 0;
	static int dir = 0;
	static int k;
	static int mx, my;
	static BOOL otherani, otherani2, Second;
	switch (iMessage) //메시지의 번호
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 70, NULL);
		Second = FALSE;
		x = 100;
		x3 = 100;
		y3 = 100;
		y = 100;
		break;
	case WM_KEYDOWN:
		if (wParam == VK_LEFT)
		{
			x -= 10;
			x3 -= 10;
			dir = 0;
		}
		else if (wParam == VK_RIGHT)
		{
			x3 += 10;
			x += 10;
			dir = 1;
		}
		else if (wParam == VK_DOWN)
		{
			y3 += 10;
			y += 10;
		}
		else if (wParam == VK_UP)
		{
			y3 -= 10;
			y -= 10;
		}
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (CollisionCheck(x, y, mx, my) == TRUE)
		{
			select = 1;
			SetTimer(hWnd, 5, 1, NULL);
		}
		else if((CollisionCheck(x3 + 200, y3, mx, my) == TRUE))
		{
			select = 2;
			SetTimer(hWnd, 5, 1, NULL);
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_CHAR:
		switch (wParam)
		{
		case 'j':
			SetTimer(hWnd, 2, 50, NULL);
			break;
		case 'J':
			SetTimer(hWnd, 2, 50, NULL);
			break;
		case 'e':
			SetTimer(hWnd, 3, 50, NULL);
			break;
		case 'E':
			SetTimer(hWnd, 3, 50, NULL);
			break;
		case 's':
			SetTimer(hWnd, 4, 50, NULL);
			break;
		case 'S':
			SetTimer(hWnd, 4, 50, NULL);
			break;
		case 't':
			if (Second == FALSE)
				Second = TRUE;
			else
				Second = FALSE;
			break;
		case 'T':
			if (Second == FALSE)
				Second = TRUE;
			else
				Second = FALSE;
		}
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case 2:
			jump++;
			if (jump < 10)
			{
				y3 -= 5;
				y -= 5;
			}
			else
			{
				y3 += 5;
				y += 5;
			}
			if (jump == 20)
			{
				jump = 0;
				KillTimer(hWnd, 2);
			}
			break;
		case 3:
			k++;
			if (k < 10)
			{
				size += 5;
			}
			else
			{
				size -= 5;
			}
			if (k == 20)
			{
				k = 0;
				KillTimer(hWnd, 3);
			}
			break;
		case 4:
			k++;
			if (k < 10)
			{
				size -= 5;
			}
			else
			{
				size += 5;
			}
			if (k == 20)
			{
				k = 0;
				KillTimer(hWnd, 4);
			}
			break;

		case 5:
			if (select == 1) {
				if (count > 5) {
					count = 0;
					otherani = FALSE;
					KillTimer(hWnd, 5);
				}
				else {
					otherani = TRUE;
					x += 5;
					y += 5;

					count++;
				}
			}
			else if (select == 2) {
				if (count > 5) {
					count = 0;
					otherani2 = FALSE;
					KillTimer(hWnd, 5);
				}
				else {
					otherani2 = TRUE;
					x3 += 5;
					y3 += 5;
					count++;
				}
			}
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		Animation(x, y,x3,y3, hdc, size, dir, otherani, otherani2,Second, select);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
	//case에서 정의되지 않은 메시지는 커널이 처리하도록 메시지 전달
}

void Animation(int xPos, int yPos,int x3, int y3, HDC hdc, int s, int dir, BOOL other, BOOL other2, BOOL Second, int select)
{
	HDC memdc, changedc, tempdc, kdc;
	HBITMAP runBit[6], moveBit[6],runBit2[6], moveBit2[6] ,hbit, hbit2,oldbit, oldbit2, hbit3, oldbit3, hbit4, oldbit4;
	HBITMAP dir1Bit[6], dir1Bit2[6], dir2Bit[6], dir2Bit2[6];
	static int count;
	count++;
	count = count % 6;
	if (other == FALSE && dir == 0) {
		runBit[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(102));
		runBit[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(103));
		runBit[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(104));
		runBit[3] = LoadBitmap(g_hinst, MAKEINTRESOURCE(105));
		runBit[4] = LoadBitmap(g_hinst, MAKEINTRESOURCE(106));
		runBit[5] = LoadBitmap(g_hinst, MAKEINTRESOURCE(107));
	}
	else if (other == TRUE && dir == 0) {
		moveBit[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(109));
		moveBit[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(110));
		moveBit[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(111));
		moveBit[3] = LoadBitmap(g_hinst, MAKEINTRESOURCE(112));
		moveBit[4] = LoadBitmap(g_hinst, MAKEINTRESOURCE(113));
		moveBit[5] = LoadBitmap(g_hinst, MAKEINTRESOURCE(114));
	}

	if (other2 == FALSE && dir == 0) {
		runBit2[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(102));
		runBit2[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(103));
		runBit2[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(104));
		runBit2[3] = LoadBitmap(g_hinst, MAKEINTRESOURCE(105));
		runBit2[4] = LoadBitmap(g_hinst, MAKEINTRESOURCE(106));
		runBit2[5] = LoadBitmap(g_hinst, MAKEINTRESOURCE(107));
	}
	else if (other2 == TRUE && dir == 0) {
		moveBit2[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(109));
		moveBit2[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(110));
		moveBit2[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(111));
		moveBit2[3] = LoadBitmap(g_hinst, MAKEINTRESOURCE(112));
		moveBit2[4] = LoadBitmap(g_hinst, MAKEINTRESOURCE(113));
		moveBit2[5] = LoadBitmap(g_hinst, MAKEINTRESOURCE(114));
	}

	if (other == FALSE && dir == 1) {
		dir1Bit[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(125));
		dir1Bit[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(126));
		dir1Bit[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(127));
		dir1Bit[3] = LoadBitmap(g_hinst, MAKEINTRESOURCE(128));
		dir1Bit[4] = LoadBitmap(g_hinst, MAKEINTRESOURCE(129));
		dir1Bit[5] = LoadBitmap(g_hinst, MAKEINTRESOURCE(130));
	}
	else if (other == TRUE && dir == 1) {
		dir1Bit2[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(119));
		dir1Bit2[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(120));
		dir1Bit2[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(121));
		dir1Bit2[3] = LoadBitmap(g_hinst, MAKEINTRESOURCE(122));
		dir1Bit2[4] = LoadBitmap(g_hinst, MAKEINTRESOURCE(123));
		dir1Bit2[5] = LoadBitmap(g_hinst, MAKEINTRESOURCE(124));
	}

	if (other2 == FALSE && dir == 1) {
		dir2Bit[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(125));
		dir2Bit[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(126)); // 뒷배경 흰색
		dir2Bit[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(127));
		dir2Bit[3] = LoadBitmap(g_hinst, MAKEINTRESOURCE(128));
		dir2Bit[4] = LoadBitmap(g_hinst, MAKEINTRESOURCE(129));
		dir2Bit[5] = LoadBitmap(g_hinst, MAKEINTRESOURCE(130));
	}
	else if (other2 == TRUE && dir == 1) {
		dir2Bit2[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(119));
		dir2Bit2[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(120)); // 뒷배경 초록색
		dir2Bit2[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(121));
		dir2Bit2[3] = LoadBitmap(g_hinst, MAKEINTRESOURCE(122));
		dir2Bit2[4] = LoadBitmap(g_hinst, MAKEINTRESOURCE(123));
		dir2Bit2[5] = LoadBitmap(g_hinst, MAKEINTRESOURCE(124));
	}


	memdc = CreateCompatibleDC(hdc);
	tempdc = CreateCompatibleDC(hdc);
	changedc = CreateCompatibleDC(memdc);
	hbit = LoadBitmap(g_hinst, MAKEINTRESOURCE(108));
	oldbit = (HBITMAP)SelectObject(memdc, hbit);
	hbit2 = LoadBitmap(g_hinst, MAKEINTRESOURCE(108));
	oldbit2 = (HBITMAP)SelectObject(changedc, NULL);
	hbit3 = LoadBitmap(g_hinst, MAKEINTRESOURCE(108));
	oldbit3 = (HBITMAP)SelectObject(tempdc, NULL);
	hbit3 = LoadBitmap(g_hinst, MAKEINTRESOURCE(108));
	BitBlt(hdc, 0, 0, 800, 600, memdc, 0, 0, SRCCOPY);


	if (dir == 0) {
		if(other == FALSE)
			(HBITMAP)SelectObject(memdc, runBit[count]);
		else if(other == TRUE)
			(HBITMAP)SelectObject(memdc, moveBit[count]);

		if (Second == TRUE) {
			if (other2 == FALSE)
				(HBITMAP)SelectObject(tempdc, runBit2[count]);
			else
				(HBITMAP)SelectObject(tempdc, moveBit2[count]);
		}
	}
	else if (dir == 1) {
		if (other == FALSE)
			(HBITMAP)SelectObject(memdc, dir1Bit[count]);
		else if (other == TRUE)
			(HBITMAP)SelectObject(memdc, dir1Bit2[count]);

		if (Second == TRUE) {
			if (other2 == FALSE)
				(HBITMAP)SelectObject(tempdc, dir2Bit[count]);
			else
				(HBITMAP)SelectObject(tempdc, dir2Bit2[count]);
		}
	}
	
	if (dir == 0) {
		if (other == FALSE) {
			TransparentBlt(hdc, xPos, yPos, 200 + s, 200 + s, memdc, 0, 0, 220, 220, RGB(0, 255, 0)); //크기확대는 첫번쨰에서 결정함.
			if (Second == TRUE) {
				if(other2 == FALSE)
					TransparentBlt(hdc, x3 + 200, y3, 200 + s, 200 + s, tempdc, 0, 0, 220, 220, RGB(0, 255, 0)); //크기확대는 첫번쨰에서 결정함.
				else
					TransparentBlt(hdc, x3 + 200, y3, 200 + s, 200 + s, tempdc, 0, 0, 220, 220, RGB(255, 255, 255)); //크기확대는 첫번쨰에서 결정함.
				
			}
		}
		else if (other == TRUE) {
			TransparentBlt(hdc, xPos, yPos, 200 + s, 200 + s, memdc, 0, 0, 220, 220, RGB(255, 255, 255)); //크기확대는 첫번쨰에서 결정함.
			if (Second == TRUE) {
				if (other2 == FALSE)
					TransparentBlt(hdc, x3 + 200, y3, 200 + s, 200 + s, tempdc, 0, 0, 220, 220, RGB(0, 255, 0)); //크기확대는 첫번쨰에서 결정함.
				else
					TransparentBlt(hdc, x3 + 200, y3, 200 + s, 200 + s, tempdc, 0, 0, 220, 220, RGB(255, 255, 255)); //크기확대는 첫번쨰에서 결정함.

			}
		}
	}
	else if (dir == 1) {		
		if (other == FALSE) {
			TransparentBlt(hdc, xPos, yPos, 200 + s, 200 + s, memdc, 0, 0, 220, 220, RGB(0, 255, 0)); //크기확대는 첫번쨰에서 결정함.
			if (Second == TRUE) {
				if (other2 == FALSE)
					TransparentBlt(hdc, x3 + 200, y3, 200 + s, 200 + s, tempdc, 0, 0, 220, 220, RGB(0, 255, 0)); //크기확대는 첫번쨰에서 결정함.
				else
					TransparentBlt(hdc, x3 + 200, y3, 200 + s, 200 + s, tempdc, 0, 0, 220, 220, RGB(255, 255, 255)); //크기확대는 첫번쨰에서 결정함.

			}
		}
		else if (other == TRUE) {
			TransparentBlt(hdc, xPos, yPos, 200 + s, 200 + s, memdc, 0, 0, 220, 220, RGB(255, 255, 255)); //크기확대는 첫번쨰에서 결정함.
			if (Second == TRUE) {
				if (other2 == FALSE)
					TransparentBlt(hdc, x3 + 200, y3, 200 + s, 200 + s, tempdc, 0, 0, 220, 220, RGB(0, 255, 0)); //크기확대는 첫번쨰에서 결정함.
				else
					TransparentBlt(hdc, x3 + 200, y3, 200 + s, 200 + s, tempdc, 0, 0, 220, 220, RGB(255, 255, 255)); //크기확대는 첫번쨰에서 결정함.

			}
		}		
	}

	SelectObject(memdc, oldbit);
	for (int i = 0; i < 6; i++)
	{
		DeleteObject(runBit[i]);
		DeleteObject(runBit2[i]);
		DeleteObject(moveBit[i]);
		DeleteObject(moveBit2[i]);
		DeleteObject(dir1Bit[i]);
		DeleteObject(dir1Bit[i]);
		DeleteObject(dir1Bit2[i]);
		DeleteObject(dir2Bit[i]);
		DeleteObject(dir2Bit[i]);
	}
	DeleteDC(memdc);
	DeleteObject(hbit);
	DeleteDC(changedc);
	DeleteObject(hbit2);
}


BOOL CollisionCheck(int x, int y, int mx, int my)
{
	if (mx > x&&mx<x + 220 && my>y&&my < y + 220)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
