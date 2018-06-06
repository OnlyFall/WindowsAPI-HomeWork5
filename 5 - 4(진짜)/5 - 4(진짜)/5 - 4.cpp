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


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) //h�� �ǹ�? = �ڵ�
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hinst = hinstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //����
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //���α׷� ������ ���̴� Ŀ��
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //������ 
	WndClass.hInstance = hinstance; //���� ����ǰ� �ִ� ��ü�� �ڵ�
	WndClass.lpfnWndProc = WndProc; //���ν����Լ��� �̸� 
	WndClass.lpszClassName = lpszClass; //������ Ŭ���� ������ �̸� 
	WndClass.lpszMenuName = NULL; // �޴��� �̸�
	WndClass.style = CS_HREDRAW | CS_VREDRAW; //������ ��� ��Ÿ��
											  // ������� WNDCLASS����ü�� �����鿡 ���� ����

	RegisterClass(&WndClass); // ������ Ŭ������ �ü���� ���

	hWnd = CreateWindow(lpszClass, "Windows Program 1-1", WS_OVERLAPPEDWINDOW, 0, 0, 800, 800, NULL, (HMENU)NULL, hinstance, NULL);
	//CreateWindow(Ŭ���� �̸�,Ÿ��Ʋ �̸�,������ ��Ÿ��, ������ ��ġ��ǥx , y,������ ����ũ��, ������ ����ũ��, �θ� ������ �ڵ�, �޴� �ڵ�, �������α׷� �ν��Ͻ�, ���� ������ ����(NULL))
	ShowWindow(hWnd, nCmdShow); //�������� ȭ�� ���
	UpdateWindow(hWnd); //OS�� WM_PAINT�޽��� ����
	while (GetMessage(&Message, NULL, 0, 0))
	{
		//������ ���ν������� PostQuitMessage()�� ȣ���Ҷ� �����.
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}//�޽��������� ����.
	return (int)Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK(������ �ڵ�, ó���ؾ� �� �޽����� ��,�Է���ġ�� ��1,2)
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
	switch (iMessage) //�޽����� ��ȣ
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
	//case���� ���ǵ��� ���� �޽����� Ŀ���� ó���ϵ��� �޽��� ����
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
		dir2Bit[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(126)); // �޹�� ���
		dir2Bit[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(127));
		dir2Bit[3] = LoadBitmap(g_hinst, MAKEINTRESOURCE(128));
		dir2Bit[4] = LoadBitmap(g_hinst, MAKEINTRESOURCE(129));
		dir2Bit[5] = LoadBitmap(g_hinst, MAKEINTRESOURCE(130));
	}
	else if (other2 == TRUE && dir == 1) {
		dir2Bit2[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(119));
		dir2Bit2[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(120)); // �޹�� �ʷϻ�
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
			TransparentBlt(hdc, xPos, yPos, 200 + s, 200 + s, memdc, 0, 0, 220, 220, RGB(0, 255, 0)); //ũ��Ȯ��� ù�������� ������.
			if (Second == TRUE) {
				if(other2 == FALSE)
					TransparentBlt(hdc, x3 + 200, y3, 200 + s, 200 + s, tempdc, 0, 0, 220, 220, RGB(0, 255, 0)); //ũ��Ȯ��� ù�������� ������.
				else
					TransparentBlt(hdc, x3 + 200, y3, 200 + s, 200 + s, tempdc, 0, 0, 220, 220, RGB(255, 255, 255)); //ũ��Ȯ��� ù�������� ������.
				
			}
		}
		else if (other == TRUE) {
			TransparentBlt(hdc, xPos, yPos, 200 + s, 200 + s, memdc, 0, 0, 220, 220, RGB(255, 255, 255)); //ũ��Ȯ��� ù�������� ������.
			if (Second == TRUE) {
				if (other2 == FALSE)
					TransparentBlt(hdc, x3 + 200, y3, 200 + s, 200 + s, tempdc, 0, 0, 220, 220, RGB(0, 255, 0)); //ũ��Ȯ��� ù�������� ������.
				else
					TransparentBlt(hdc, x3 + 200, y3, 200 + s, 200 + s, tempdc, 0, 0, 220, 220, RGB(255, 255, 255)); //ũ��Ȯ��� ù�������� ������.

			}
		}
	}
	else if (dir == 1) {		
		if (other == FALSE) {
			TransparentBlt(hdc, xPos, yPos, 200 + s, 200 + s, memdc, 0, 0, 220, 220, RGB(0, 255, 0)); //ũ��Ȯ��� ù�������� ������.
			if (Second == TRUE) {
				if (other2 == FALSE)
					TransparentBlt(hdc, x3 + 200, y3, 200 + s, 200 + s, tempdc, 0, 0, 220, 220, RGB(0, 255, 0)); //ũ��Ȯ��� ù�������� ������.
				else
					TransparentBlt(hdc, x3 + 200, y3, 200 + s, 200 + s, tempdc, 0, 0, 220, 220, RGB(255, 255, 255)); //ũ��Ȯ��� ù�������� ������.

			}
		}
		else if (other == TRUE) {
			TransparentBlt(hdc, xPos, yPos, 200 + s, 200 + s, memdc, 0, 0, 220, 220, RGB(255, 255, 255)); //ũ��Ȯ��� ù�������� ������.
			if (Second == TRUE) {
				if (other2 == FALSE)
					TransparentBlt(hdc, x3 + 200, y3, 200 + s, 200 + s, tempdc, 0, 0, 220, 220, RGB(0, 255, 0)); //ũ��Ȯ��� ù�������� ������.
				else
					TransparentBlt(hdc, x3 + 200, y3, 200 + s, 200 + s, tempdc, 0, 0, 220, 220, RGB(255, 255, 255)); //ũ��Ȯ��� ù�������� ������.

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
