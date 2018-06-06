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

	hWnd = CreateWindow(lpszClass, "Windows Program 1-1", WS_OVERLAPPEDWINDOW, 0, 0, 520, 330, NULL, (HMENU)NULL, hinstance, NULL);
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
	switch (iMessage) //�޽����� ��ȣ
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
		else if (wParam == 'e') { // ���
			startx += 5;
			endx -= 5;
			starty += 5;
			endy -= 5;
		}
		else if (wParam == 'h') { // Ȯ��
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
		{ // �� ���� 
			endx = LOWORD(lParam);
			endy = HIWORD(lParam);
			SetROP2(hdc, R2_XORPEN); // ���� XOR ����
			SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));
			Rectangle(hdc, startx, starty, endx, endy);
			Rectangle(hdc, startx, starty, ox, oy);
			//Rectangle(hmemdc, startx, starty, ox, oy);
			ox = endx;
			oy = endy; // �� ������ ���� �������� ����
			
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
	case WM_PAINT: //�޽����� ó��
		hdc = BeginPaint(hWnd, &ps);
		hmemdc = CreateCompatibleDC(hdc);
		myBitmap = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP1));
		oldBitmap = (HBITMAP)SelectObject(hmemdc, myBitmap);
		
		
		

		StretchBlt(hdc, 0, 0, 520, 320, hmemdc, 0, 0, 530, 320, SRCCOPY);  //���ȭ��
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
	//	StretchBlt(hdc, sx + mx, sy + my, ox - sx + mx + size, oy - sy + my + size, hmemdc, sx + mx, sy + my, ox - sx + mx + size - sizeview, oy - sy + my + size - sizeview, SRCCOPY); //�簢��
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
	//case���� ���ǵ��� ���� �޽����� Ŀ���� ó���ϵ��� �޽��� ����
}