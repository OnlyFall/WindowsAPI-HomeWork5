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

	hWnd = CreateWindow(lpszClass, "Windows Program 1-1", WS_OVERLAPPEDWINDOW, 0, 0, 1100, 350, NULL, (HMENU)NULL, hinstance, NULL);
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
	HBITMAP myBitmap, mybitmap2, mybitmap3, mybitmap4, oldBitmap, oldBitmap2, oldBitmap3, oldBitmap4;
	static RECT rect;
	static BOOL drag;
	static int size = 0;

	HPEN hPen, oldPen;

	switch (iMessage) //�޽����� ��ȣ
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
	case WM_PAINT: //�޽����� ó��
		hdc = BeginPaint(hWnd, &ps);
		hmemdc = CreateCompatibleDC(hdc);
		myBitmap = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP1));
		oldBitmap = (HBITMAP)SelectObject(hmemdc, myBitmap);

	
		StretchBlt(hdc, 0, 0, 520, 320, hmemdc, 0, 0, 530, 320, SRCCOPY);  //���ȭ��
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
	//case���� ���ǵ��� ���� �޽����� Ŀ���� ó���ϵ��� �޽��� ����
}