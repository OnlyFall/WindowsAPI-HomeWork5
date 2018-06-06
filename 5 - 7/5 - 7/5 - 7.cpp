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
	static int imHdcX, imHdcY,imX, imY, imX2, imY2, imX3, imY3, imX4, imY4,imCheck2, imCheck3, imCheck4,imCheck; // �̵��ϰ��� SWAP(������� ���� �����̴����� �׸��� ���� ��ġ�� �ٲٱ�����..)�� �Ҷ� ���
	static BOOL Click = FALSE;  // �̵��߿��� Ŭ���� �������� ����
	static BOOL complete = FALSE; // FŰ�� ������ �ϼ��� ����� ���̹Ƿ� �������� �� 1�ʰ� ���̰� �������̴�.
	static int completeCount = 0; // FŰ�� ������ �ϼ��� ����� ���� �ٽ� ���������� ī��Ʈ
	srand(10);
	//srand((unsigned)time(NULL));

	switch (iMessage) //�޽����� ��ȣ
	{
	case WM_CREATE:

		break;

	case WM_LBUTTONDOWN:
		if (Click == FALSE) {
			inputX = LOWORD(lParam);
			inputY = HIWORD(lParam);
			// Ŭ���� ������ �׸��� ���ʿ� ����� �� �ֺ��� ������� �ֳ� Ȯ���� ������� ��ǥ�� �����Ѵ�.
			for (int i = 0; i < select; ++i) {
				for (int j = 0; j < select; ++j) {
					if (inputX > randMap[j][i].hdcX && inputX < randMap[j][i].hdcX + (600 / select) && inputY > randMap[j][i].hdcY && inputY < randMap[j][i].hdcY + (600 / select)) {
						tempX = j;
						tempY = i;
						if (randMap[tempX + 1][tempY].check == 1 && tempX + 1 < select) {
							imHdcX = randMap[tempX][tempY].hdcX;
							imHdcY = randMap[tempX][tempY].hdcY;
							imX = randMap[tempX + 1][tempY].memdcX;
							imY = randMap[tempX + 1][tempY].memdcY;
							imCheck = randMap[tempX + 1][tempY].check;
						}
						else if (randMap[tempX - 1][tempY].check == 1) {
							imHdcX = randMap[tempX][tempY].hdcX;
							imHdcY = randMap[tempX][tempY].hdcY;
							imX2 = randMap[tempX - 1][tempY].memdcX;
							imY2 = randMap[tempX - 1][tempY].memdcY;
							imCheck2 = randMap[tempX - 1][tempY].check;
						}
						else if (randMap[tempX][tempY - 1].check == 1) {
							imHdcX = randMap[tempX][tempY].hdcX;
							imHdcY = randMap[tempX][tempY].hdcY;
							imX3 = randMap[tempX][tempY - 1].memdcX;
							imY3 = randMap[tempX][tempY - 1].memdcY;
							imCheck3 = randMap[tempX][tempY - 1].check;
						}
						else if (randMap[tempX][tempY + 1].check == 1) {
							imHdcX = randMap[tempX][tempY].hdcX;
							imHdcY = randMap[tempX][tempY].hdcY;
							imX4 = randMap[tempX][tempY + 1].memdcX;
							imY4 = randMap[tempX][tempY + 1].memdcY;
							imCheck4 = randMap[tempX][tempY + 1].check;
						}
						Click = TRUE;
						SetTimer(hWnd, 1, 10, NULL);
					}
				}
			}
		}


		break;

	case WM_KEYDOWN:
		if (wParam == 'f' || wParam == 'F') {
			complete = TRUE;
			SetTimer(hWnd, 2, 100, NULL);
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_CHAR:
		if (wParam == '2') {
			select = 2;
			randMap[deleteX][deleteY].check = 0;
			deleteX = rand() % select;
			deleteY = rand() % select;
			randMap[deleteX][deleteY].check = 1;

			for (int i = 0; i < select; ++i) {
				for (int j = 0; j < select; ++j) {
					randMap[j][i].hdcX = j * (600 / select);
					randMap[j][i].hdcY = i * (600 / select);
					randMap[j][i].index = 1;
					randMap[j][i].memdcX = (select - j - 1) * (600 / select);
					randMap[j][i].memdcY = (select - i - 1) * (600 / select);
				}
			}
		}
		else if (wParam == '3') {
			select = 3;
			randMap[deleteX][deleteY].check = 0;
			deleteX = rand() % select;
			deleteY = rand() % select;
			randMap[deleteX][deleteY].check = 1;

			for (int i = 0; i < select; ++i) {
				for (int j = 0; j < select; ++j) {
					randMap[j][i].hdcX = j * (600 / select);
					randMap[j][i].hdcY = i * (600 / select);
					randMap[j][i].index = 1;
					randMap[j][i].memdcX = (select - j - 1) * (600 / select);
					randMap[j][i].memdcY = (select - i - 1) * (600 / select);
				}
			}
		}
		else if (wParam == '4') {
			select = 4;
			randMap[deleteX][deleteY].check = 0;
			deleteX = rand() % select;
			deleteY = rand() % select;
			randMap[deleteX][deleteY].check = 1;

			for (int i = 0; i < select; ++i) {
				for (int j = 0; j < select; ++j) {
					randMap[j][i].hdcX = j * (600 / select);
					randMap[j][i].hdcY = i * (600 / select);
					randMap[j][i].index = 1;
					randMap[j][i].memdcX = (select - j - 1) * (600 / select);
					randMap[j][i].memdcY = (select - i - 1) * (600 / select);
				}
			}
		}
		else if (wParam == '5') {
			select = 5;
			randMap[deleteX][deleteY].check = 0;
			deleteX = rand() % select;
			deleteY = rand() % select;
			randMap[deleteX][deleteY].check = 1;

			for (int i = 0; i < select; ++i) {
				for (int j = 0; j < select; ++j) {
					randMap[j][i].hdcX = j * (600 / select);
					randMap[j][i].hdcY = i * (600 / select);
					randMap[j][i].index = 1;
					randMap[j][i].memdcX = (select - j - 1) * (600 / select);
					randMap[j][i].memdcY = (select - i - 1) * (600 / select);
				}
			}
		}
		else if (wParam == 's' || wParam == 'S') {
			gameStart = TRUE;
			//checkingX = rand() % (select + 1);
			//checkingY = rand() % (select + 1);
			randMap[deleteX][deleteY].check = 0;
			deleteX = rand() % select;
			deleteY = rand() % select;
			randMap[deleteX][deleteY].check = 1;

			for (int i = 0; i < select; ++i) {
				for (int j = 0; j < select; ++j) {
					randMap[j][i].hdcX = j * (600 / select);
					randMap[j][i].hdcY = i * (600 / select);
					randMap[j][i].index = 1;
					randMap[j][i].memdcX = (select - j - 1) * (600 / select);
					randMap[j][i].memdcY = (select - i - 1) * (600 / select);
				}
			}
		}
		
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_TIMER:
		switch (wParam) {
		case 1:
			// ��������� �̵��� �����ϸ� ���� swap ���ش�.
			xCount += 1;
			if (randMap[tempX + 1][tempY].check == 1 && tempX + 1 < select) {
				if (xCount <= 600 / select)
					randMap[tempX][tempY].hdcX += 1;
				else {
					randMap[tempX + 1][tempY].memdcX = randMap[tempX][tempY].memdcX;
					randMap[tempX + 1][tempY].memdcY = randMap[tempX][tempY].memdcY;
					randMap[tempX + 1][tempY].check = randMap[tempX][tempY].check;
					
					randMap[tempX][tempY].hdcX = imHdcX;
					randMap[tempX][tempY].memdcX = imX;
					randMap[tempX][tempY].memdcY = imY;
					randMap[tempX][tempY].check = imCheck;
					xCount = 0;
					Click = FALSE;
					KillTimer(hWnd, 1);
				}
			}

			else if (randMap[tempX - 1][tempY].check == 1 && tempX - 1 >=0) {
				if (xCount <= 600 / select)
					randMap[tempX][tempY].hdcX -= 1;
				else {
					randMap[tempX - 1][tempY].memdcX = randMap[tempX][tempY].memdcX;
					randMap[tempX - 1][tempY].memdcY = randMap[tempX][tempY].memdcY;
					randMap[tempX - 1][tempY].check = randMap[tempX][tempY].check;

					randMap[tempX][tempY].hdcX = imHdcX;
					randMap[tempX][tempY].memdcX = imX2;
					randMap[tempX][tempY].memdcY = imY2;
					randMap[tempX][tempY].check = imCheck2;
					xCount = 0;
					Click = FALSE;
					KillTimer(hWnd, 1);
				}
			}

			else if (randMap[tempX][tempY - 1].check == 1 && tempY - 1 >= 0) {
				if (xCount <= 600 / select)
					randMap[tempX][tempY].hdcY -= 1;
				else {
					randMap[tempX][tempY - 1].memdcX = randMap[tempX][tempY].memdcX;
					randMap[tempX][tempY - 1].memdcY = randMap[tempX][tempY].memdcY;
					randMap[tempX][tempY - 1].check = randMap[tempX][tempY].check;

					randMap[tempX][tempY].hdcY = imHdcY;
					randMap[tempX][tempY].memdcY = imY3;
					randMap[tempX][tempY].memdcX = imX3;
					randMap[tempX][tempY].check = imCheck3;
					xCount = 0;
					Click = FALSE;
					KillTimer(hWnd, 1);
				}
			}
			else if (randMap[tempX][tempY + 1].check == 1 && tempY + 1 <=select) {
				if (xCount <= 600 / select)
					randMap[tempX][tempY].hdcY += 1;
				else {
					randMap[tempX][tempY + 1].memdcX = randMap[tempX][tempY].memdcX;
					randMap[tempX][tempY + 1].memdcY = randMap[tempX][tempY].memdcY;
					randMap[tempX][tempY + 1].check = randMap[tempX][tempY].check;

					randMap[tempX][tempY].hdcY = imHdcY;
					randMap[tempX][tempY].memdcX = imX4;
					randMap[tempX][tempY].memdcY = imY4;
					randMap[tempX][tempY].check = imCheck4;
					xCount = 0;
					Click = FALSE;
					KillTimer(hWnd, 1);
				}
			}
			else {
				xCount = 0;
				Click = FALSE;
				KillTimer(hWnd, 1);
			}
			break;

		case 2:
			if (completeCount <= 3)
				completeCount++;
			else {
				complete = FALSE;
				completeCount = 0;
			}
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		memdc2 = CreateCompatibleDC(memdc);
		//	gdc = CreateCompatibleDC(memdc);
		//		hbit2 = LoadBitmap(g_hinst, MAKEINTRESOURCE(BackGroundSky));
		hbit = CreateCompatibleBitmap(hdc, 630, 630);
		runMap = LoadBitmap(g_hinst, MAKEINTRESOURCE(101));

		oldBit1 = (HBITMAP)SelectObject(memdc, hbit);
		oldBit2 = (HBITMAP)SelectObject(memdc2, runMap);


	//	BitBlt(hdc, 0, 0, 1280, 1080, memdc, 0, 0, SRCCOPY);
		(HBITMAP)SelectObject(memdc2, runMap);
		if (gameStart == FALSE) {// ���� ������ ĭ ������ ������϶��� ȭ�� (���� �� ����)
			BitBlt(memdc, 0, 0, 600, 600, memdc2, 0, 0, SRCCOPY);
		}

		if (complete == TRUE) { // fŰ�� �������� ���̴� �ϼ��� ȭ��
			for (int i = 0; i < select; ++i) {
				for (int j = 0; j < select; ++j) {
					Map[j][i].hdcX = j * (600 / select);
					Map[j][i].hdcY = i * (600 / select);
					Map[j][i].index = cnt;
					cnt++;
				}
			}


			for (int i = 0; i < select; ++i) {
				for (int j = 0; j < select; ++j) {
					if (Map[j][i].check == 0)
						StretchBlt(memdc, Map[j][i].hdcX, Map[j][i].hdcY, 600 / select, 600 / select, memdc2, Map[j][i].hdcX, Map[j][i].hdcY, 600 / select, 600 / select, SRCCOPY);
				}
			}
		}
		else { //(SŰ�� ���� �������� ȭ���� ��ġ�ϰ� �Ѻκ��� ������)
			for (int i = 0; i < select; ++i) {
				for (int j = 0; j < select; ++j) {
					if (randMap[j][i].check == 0)
						StretchBlt(memdc, randMap[j][i].hdcX, randMap[j][i].hdcY, 600 / select, 600 / select, memdc2, randMap[j][i].memdcX, randMap[j][i].memdcY, 600 / select, 600 / select, SRCCOPY);
				}
			}
		}


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
