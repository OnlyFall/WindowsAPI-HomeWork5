#include <Windows.h>
#include "resource.h"
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hinst;
LPCTSTR lpszClass = TEXT("First");

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

	hWnd = CreateWindow(lpszClass, "Windows Program 1-1", WS_OVERLAPPEDWINDOW, 0, 0, 810, 630, NULL, (HMENU)NULL, hinstance, NULL);
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK
																				 //(������ �ڵ�, ó���ؾ� �� �޽����� ��,�Է���ġ�� ��1,2)
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

	switch (iMessage) //�޽����� ��ȣ
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
		// ��üȭ���϶� �¿� �̵�
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
		
		// 4�����϶� ȭ�� �̵�
		else {
			if (select == 1) { //1��и�
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

			else if (select == 2) { //2��и�
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

			else if (select == 4) { //3��и�
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
			else if (select == 3) { //4��и�
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
	case WM_PAINT: //�޽����� ó��
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
			
			//1��и�
			//StretchBlt(hdc, 0 + stress1, 0, 400, 300, hmemdc, 0, 0, 530, 320, (DWORD)0x00CC0020);
			StretchBlt(hdc, 0, 0, 400, 300, hmemdc, 0 - stress1, 0, 530, 320, (DWORD)0x00CC0020);
			StretchBlt(hdc, 0, 0, 400, 300, hmemdc, 530 - stress1, 0, 530, 320, (DWORD)0x00CC0020);
			StretchBlt(hdc, 0, 0, 400, 300, hmemdc, -530 - stress1, 0, 530, 320, (DWORD)0x00CC0020);
			//2��и�
			StretchBlt(hdc, 400 , 0, 400, 300, hmemdc, 0 - stress2, 0, 530, 320, (DWORD)0x00660046);
			StretchBlt(hdc, 400, 0, 400, 300, hmemdc, 530 - stress2, 0, 530, 320, (DWORD)0x00660046);
			StretchBlt(hdc, 400, 0, 400, 300, hmemdc, -530 - stress2, 0, 530, 320, (DWORD)0x00660046);
			//3��и�
			StretchBlt(hdc, 400, 300, 400, 300, hmemdc, 0 - stress3, 0, 530, 320, (DWORD)0x00CC0020);
			StretchBlt(hdc, 400 , 300, 400, 300, hmemdc, 530 - stress3, 0, 530, 320, (DWORD)0x00CC0020);
			StretchBlt(hdc, 400 , 300, 400, 300, hmemdc, -530 - stress3, 0, 530, 320, (DWORD)0x00CC0020);
			//4��и�
			StretchBlt(hdc, 0 , 300, 400, 300, hmemdc, 0 - stress4, 0, 530, 320, (DWORD)0x00660046);
			StretchBlt(hdc, 0 , 300, 400, 300, hmemdc, 530 - stress4, 0, 530, 320, (DWORD)0x00660046);
			StretchBlt(hdc, 0 , 300, 400, 300, hmemdc, -530 - stress4, 0, 530, 320, (DWORD)0x00660046);


			//1��и�
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
			//2��и�
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
			//3��и�
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
			//4��и�
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
	//case���� ���ǵ��� ���� �޽����� Ŀ���� ó���ϵ��� �޽��� ����
}