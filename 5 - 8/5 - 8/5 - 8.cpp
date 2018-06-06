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

	hWnd = CreateWindow(lpszClass, "Windows Program 1-1", WS_OVERLAPPEDWINDOW, 0, 0, 615, 630, NULL, (HMENU)NULL, hinstance, NULL);
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK(������ �ڵ�, ó���ؾ� �� �޽����� ��,�Է���ġ�� ��1,2)
{
	PAINTSTRUCT ps;
	HDC hdc, memdc, memdc2;
	//	CImage g_cimgTest;
	//	HDC adddc = g_cimgTest; 
	HBITMAP runMap;
	//HDC gdc = g_cimgTest.GetDC();
	static HBITMAP hbit, hbit2, oldBit1, oldBit2; //hbit�� hdc�� ȣȯ�Ǵ�  ���� �׸�, hbit2�� �̹����� �����´�(������۸�)
	static int count = 0;
	static int temp;
	static int select;
	static int cnt = 0;
	static map Map[5][5];
	static map randMap[5][5];
	static BOOL gameStart = FALSE;
	static int deleteX, deleteY; // �ʿ��� �������� �p��°ĭ�� �������� ����
	static int tempX = 0, tempY = 0; // tempX, tempY�� Ŭ���� ������ x, y��ǥ�� ������ ����Ѵ�.
	static int inputX, inputY;  // ���콺�� ��� �Է� �޾Ҵ���? ����
	static int xCount = 0;      //�̵��ϴ� ī��Ʈ�̴�. �� xCount������ ������...;;
	static int imHdcX, imHdcY, imX, imY, imX2, imY2, imX3, imY3, imX4, imY4, imCheck2, imCheck3, imCheck4, imCheck; // �̵��ϰ��� SWAP(������� ���� �����̴����� �׸��� ���� ��ġ�� �ٲٱ�����..)�� �Ҷ� ���
	static BOOL Click = FALSE;  // �̵��߿��� Ŭ���� �������� ����
	static BOOL complete = FALSE; // FŰ�� ������ �ϼ��� ����� ���̹Ƿ� �������� �� 1�ʰ� ���̰� �������̴�.
	static int completeCount = 0; // FŰ�� ������ �ϼ��� ����� ���� �ٽ� ���������� ī��Ʈ
	srand((unsigned)time(NULL));
	//srand((unsigned)time(NULL));

	switch (iMessage) //�޽����� ��ȣ
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
	//case���� ���ǵ��� ���� �޽����� Ŀ���� ó���ϵ��� �޽��� ����
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
