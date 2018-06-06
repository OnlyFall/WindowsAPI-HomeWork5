#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <atlImage.h>
#include <math.h>
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

	hWnd = CreateWindow(lpszClass, "Windows Program 1-1", WS_OVERLAPPEDWINDOW, 0, 0, 1280,1080, NULL, (HMENU)NULL, hinstance, NULL);
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

struct BackGroung {
	int x;
	int y;
	int index = -1;
	int check = 0;
};

struct Drop {
	int x;
	int y;
	int xpos;
	int ypos;
	int index;
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK(������ �ڵ�, ó���ؾ� �� �޽����� ��,�Է���ġ�� ��1,2)
{
	PAINTSTRUCT ps;
	HDC hdc, memdc, memdc2, chardc; //ȭ�� ����� ���� DC�� chardc�� ĳ����dc�Դϴ�.
									//���δ� ���ļ� ����� �غ��ϴ� DC��
	HBITMAP block[6];
	//	CImage g_cimgTest;
	//	HDC adddc = g_cimgTest; 
	HBITMAP backGroundSky, backGroundGround, groundBit, oldgroundBit, skyBit, oldSkyBit, charBit, oldCharBit, addBit;
	//HDC gdc = g_cimgTest.GetDC();
	static HBITMAP hbit, hbit2, oldBit1, oldBit2;
	static RECT rtChar;                              //���ΰ��� ĳ���� ����
	static RECT rtplane;							//�浹üũ�� ����� �ٳ��� ����
	static RECT rtCup;			//�浹üũ�� �� �ٳ��� ����
	static int x = 200 + 300;
	static Drop play;
	static int xpos = 5;
	static int ypos = 0;
	static int y = 0;
	static int count = 0;
	static int temp;
	static BackGroung Map[10][20];
	static int Mapcount[20] = { 0 };
	switch (iMessage) //�޽����� ��ȣ
	{
	case WM_CREATE:
		play.x = x;
		play.y = y;
		play.xpos = 5;
		play.ypos = 0;
		play.index = rand() % 6;

		SetTimer(hWnd, 1, 100, NULL);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_LEFT) {
			if (play.xpos >= 0) {
				play.x -= 50;
				play.xpos -= 1;
			}
		}
		
		else if(wParam == VK_RIGHT){
			if (play.xpos <= 7) {
				play.x += 50;
				play.xpos += 1;
			}
		}
		else if (wParam == VK_RETURN) {
			if (play.index == 0)
				play.index = 3;
			else if (play.index == 3)
				play.index = 4;
			else if (play.index == 4)
				play.index = 5;
			else if (play.index == 5)
				play.index = 0;
			else if (play.index == 1)
				play.index = 2;
			else if (play.index == 2)
				play.index = 1;
		}

		break;
	case WM_CHAR:

		break;
	case WM_TIMER:
		switch (wParam) {
		case 1:
			play.y += 50;
			play.ypos += 1;
			if (Map[play.xpos + 1][play.ypos].check == 1) {
				Map[play.xpos + 1][play.ypos - 1].check = 1;
				Map[play.xpos + 1][play.ypos - 1].index = play.index;
				if (play.index == 1)
					Mapcount[play.ypos - 1] += 1;
				if (Mapcount[play.ypos - 1] == 10) {
					temp = play.ypos - 1;
					for (int j = temp; j > 0; --j) {
						for (int i = 0; i < 10; ++i) {
							Map[i][j].check = Map[i][j - 1].check;
							Map[i][j].index = Map[i][j - 1].index;
							Map[i][j].x = Map[i][j - 1].x;
							Map[i][j].y = Map[i][j - 1].y;
						}
						Mapcount[j] = Mapcount[j - 1];
					}
				}
				play.x = x;
				play.y = y;
				play.xpos = 5;
				play.ypos = 0;
				play.index = rand() % 6;
			}

				if (play.ypos == 19) {
					Map[play.xpos + 1][play.ypos].check = 1;
					Map[play.xpos + 1][play.ypos].index = play.index;
					if (play.index == 1)
						Mapcount[play.ypos] += 1;
					if (Mapcount[play.ypos] == 10) {
						temp = play.ypos;
						for (int j = temp; j > 0; --j) {
							for (int i = 0; i < 10; ++i) {
								Map[i][j].check = Map[i][j - 1].check;
								Map[i][j].index = Map[i][j - 1].index;
								Map[i][j].x = Map[i][j - 1].x;
								Map[i][j].y = Map[i][j - 1].y;
							}
							Mapcount[j] = Mapcount[j - 1];
						}
					}
					play.x = x;
					play.y = y;
					play.xpos = 5;
					play.ypos = 0;
					play.index = rand() % 6;
				}
				
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		memdc2 = CreateCompatibleDC(memdc);
		//	gdc = CreateCompatibleDC(memdc);
//		hbit2 = LoadBitmap(g_hinst, MAKEINTRESOURCE(BackGroundSky));
		hbit = CreateCompatibleBitmap(hdc, 1280, 1080);

		block[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(101));
		block[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(102));
		block[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(103));
		block[3] = LoadBitmap(g_hinst, MAKEINTRESOURCE(104));
		block[4] = LoadBitmap(g_hinst, MAKEINTRESOURCE(105));
		block[5] = LoadBitmap(g_hinst, MAKEINTRESOURCE(106));

		oldBit1 = (HBITMAP)SelectObject(memdc, hbit);
		oldBit2 = (HBITMAP)SelectObject(memdc2, NULL);

		
		
		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 10; ++j) {
				Map[j][i].x = 200 + (i * 50);
				Map[j][i].y = 200 + (j * 50);
			}
		}

		Rectangle(memdc, 200, 0, 700, 1000);
		(HBITMAP)SelectObject(memdc2, block[play.index]);
		TransparentBlt(memdc, play.x, play.y, 50, 50, memdc2, 0, 0, 50, 50, RGB(255, 255, 255)); //ũ��Ȯ��� ù�������� ������.
		SelectObject(memdc2, NULL);

		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 20; ++j) {
				if (Map[i][j].check == 1) {
					(HBITMAP)SelectObject(memdc2, block[Map[i][j].index]);
					TransparentBlt(memdc, 200 + (50 * i), (50 * j), 50, 50, memdc2, 0, 0, 50, 50, RGB(255, 255, 255));
					SelectObject(memdc2, NULL);
				}
			}
		}

		for (int i = 0; i < 10; i++) {
			MoveToEx(memdc, 200 + 50 * (i + 1), 0, NULL);
			LineTo(memdc, 200 + 50 * (i + 1), 1000);
		}
		for (int i = 0; i < 20; i++) {
			MoveToEx(memdc, 200, 50 * (i + 1), NULL);
			LineTo(memdc, 700, 50 * (i + 1));
		}



		BitBlt(hdc, 0, 0, 1280, 1080, memdc, 0, 0, SRCCOPY);



		SelectObject(memdc, oldBit1);

		for (int i = 0; i < 6; ++i) {
			DeleteObject(block[i]);
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
