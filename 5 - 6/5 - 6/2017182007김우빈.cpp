#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <atlImage.h>
#include <math.h>

#pragma comment(lib,"winmm.lib")

#define IDC_BOARD_SIZE1 760
#define IDC_BOARD_SIZE2 761
#define IDC_BOARD_SIZE3 762

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


HINSTANCE g_hinst;
LPCTSTR lpszClass = TEXT("First");

BOOL CollisionCheck(int x, int y, int mx, int my);


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) //h의 의미? = 핸들
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hinst = hinstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hinstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground =
		(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, NULL, WS_OVERLAPPEDWINDOW, 0, 0, 1280, 1080, NULL, (HMENU)NULL, hinstance, NULL);

	//WndClass.hCursor = LoadCursor(NULL, IDC_HELP);
	//WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	//WndClass.lpszClassName = "ChildClass"; // 차일드 윈도우 클래스 이름
	//WndClass.lpfnWndProc = (WNDPROC)ChildProc; // 차일드 윈도우 프로시저 지정
	//RegisterClassEx(&WndClass);
	//WndClass.lpfnWndProc = (WNDPROC)ChildProc2; // 차일드 윈도우 프로시저 지정

	//WndClass.hCursor = LoadCursor(NULL, IDC_HELP);
	//WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	//WndClass.lpszClassName = "ChildClass2"; // 차일드 윈도우 클래스 이름
	//WndClass.lpfnWndProc = (WNDPROC)ChildProc2; // 차일드 윈도우 프로시저 지정
	//RegisterClassEx(&WndClass);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

struct Drop {
	int x;
	int y;
	int xpos;
	int ypos;
	int index;
	int check;
};

static int bubble;
static int xSize = 10, ySize = 20;
static int CheckMateCount = 0;
int checkStart;
BOOL STOP = FALSE;
static POINT CheckMate[100];
static Drop Back[30][60];
//struct BackGroung {
//	int x;
//	int y;
//	int index = -1;
//	int check = 0;
//};



BOOL Critical(int destination, int x, int y) {
	BOOL result;
	CheckMateCount++;
	// 1 = ㅡ 모양
	// 2 = ㅣ 모양
	// 0 = ㄴ 반대로 돌린거
	// 3 =  ㄴ
	// 4 = ㄱ 반대로 돌린거
	// 5 = ㄱ
	if (destination == 0) {
		if (x == 0) {
			if (STOP == TRUE)
				return TRUE;

			if (bubble == 0)
				bubble = CheckMateCount;

			STOP = TRUE;

			if (checkStart == 0 || checkStart == 3)
				return (Critical(1, CheckMate[0].x, CheckMate[0].y));
			else if (checkStart == 1)
				return (Critical(0, CheckMate[0].x, CheckMate[0].y));
			else if (checkStart == 2)
				return (Critical(2, CheckMate[0].x, CheckMate[0].y));
			else if (checkStart == 4 || checkStart == 5)
				return (Critical(3, CheckMate[0].x, CheckMate[0].y));
		}

		if (Back[x - 1][y].check == 1 && (Back[x - 1][y].index == 1 || Back[x - 1][y].index == 3 || Back[x - 1][y].index == 4)) {
			CheckMate[CheckMateCount].x = x - 1;
			CheckMate[CheckMateCount].y = y;

			if (CheckMate[0].x == CheckMate[CheckMateCount].x && CheckMate[0].y == CheckMate[CheckMateCount].y)
				return TRUE;

			if (Back[x - 1][y].index == 1)
				result = Critical(0, x - 1, y);
			else if (Back[x - 1][y].index == 3)
				result = Critical(1, x - 1, y);
			else if (Back[x - 1][y].index == 4)
				result = Critical(3, x - 1, y);
		}
		else
			return FALSE;
	}

	else if (destination == 1) {
		if (y == 0)
		{
			if (STOP == TRUE)
				return TRUE;

			if (bubble == 0)
				bubble = CheckMateCount;

			STOP = TRUE;
			if (checkStart == 0 || checkStart == 3)
				return (Critical(1, CheckMate[0].x, CheckMate[0].y));
			else if (checkStart == 1)
				return (Critical(0, CheckMate[0].x, CheckMate[0].y));
			else if (checkStart == 2)
				return (Critical(2, CheckMate[0].x, CheckMate[0].y));
			else if (checkStart == 4 || checkStart == 5)
				return (Critical(3, CheckMate[0].x, CheckMate[0].y));

		}
		// 1 = ㅡ 모양
		// 2 = ㅣ 모양
		// 0 = ㄴ 반대로 돌린거
		// 3 =  ㄴ
		// 4 = ㄱ 반대로 돌린거
		// 5 = ㄱ
		if (Back[x][y - 1].check == 1 && (Back[x][y - 1].index == 2 || Back[x][y - 1].index == 4 || Back[x][y - 1].index == 5))
		{
			CheckMate[CheckMateCount].x = x;
			CheckMate[CheckMateCount].y = y - 1;
			if (CheckMate[0].x == CheckMate[CheckMateCount].x && CheckMate[0].y == CheckMate[CheckMateCount].y)
				return TRUE;

			if (Back[x][y - 1].index == 2)
				result = Critical(1, x, y - 1);
			else if (Back[x][y - 1].index == 4)
				result = Critical(2, x, y - 1);
			else if (Back[x][y - 1].index == 5)
				result = Critical(0, x, y - 1);

		}
		else
			return FALSE;
	}

	else if (destination == 2) {
		if (x == xSize - 1) {
			if (STOP == TRUE)
				return TRUE;
			if (bubble == 0)
				bubble = CheckMateCount;

			STOP = TRUE;
			if (checkStart == 0 || checkStart == 3)
				return (Critical(1, CheckMate[0].x, CheckMate[0].y));
			else if (checkStart == 1)
				return (Critical(0, CheckMate[0].x, CheckMate[0].y));
			else if (checkStart == 2)
				return (Critical(2, CheckMate[0].x, CheckMate[0].y));
			else if (checkStart == 4 || checkStart == 5)
				return (Critical(3, CheckMate[0].x, CheckMate[0].y));
		}
		// 1 = ㅡ 모양
		// 2 = ㅣ 모양
		// 0 = ㄴ 반대로 돌린거
		// 3 =  ㄴ
		// 4 = ㄱ 반대로 돌린거
		// 5 = ㄱ
		if (Back[x + 1][y].check == 1 && (Back[x + 1][y].index == 1 || Back[x + 1][y].index == 0 || Back[x + 1][y].index == 5))
		{
			CheckMate[CheckMateCount].x = x + 1;
			CheckMate[CheckMateCount].y = y;

			if (CheckMate[0].x == CheckMate[CheckMateCount].x && CheckMate[0].y == CheckMate[CheckMateCount].y)
				return TRUE;

			if (Back[x + 1][y].index == 1)
				result = Critical(2, x + 1, y);
			else if (Back[x + 1][y].index == 0)
				result = Critical(1, x + 1, y);
			else if (Back[x + 1][y].index == 5)
				result = Critical(3, x + 1, y);
		}
		else
			return FALSE;
	}

	else if (destination == 3) {
		if (y == ySize - 1) {
			if (STOP == TRUE)
				return TRUE;

			if (bubble == 0)
				bubble = CheckMateCount;

			STOP = TRUE;
			if (checkStart == 0 || checkStart == 3)
				return (Critical(1, CheckMate[0].x, CheckMate[0].y));
			else if (checkStart == 1)
				return (Critical(0, CheckMate[0].x, CheckMate[0].y));
			else if (checkStart == 2)
				return (Critical(2, CheckMate[0].x, CheckMate[0].y));
			else if (checkStart == 4 || checkStart == 5)
				return (Critical(3, CheckMate[0].x, CheckMate[0].y));
		}
		if (Back[x][y + 1].check == 1 && (Back[x][y + 1].index == 2 || Back[x][y + 1].index == 0 || Back[x][y + 1].index == 3))
		{
			// 1 = ㅡ 모양
			// 2 = ㅣ 모양
			// 0 = ㄴ 반대로 돌린거
			// 3 =  ㄴ
			// 4 = ㄱ 반대로 돌린거
			// 5 = ㄱ
			CheckMate[CheckMateCount].x = x;
			CheckMate[CheckMateCount].y = y + 1;


			if (CheckMate[0].x == CheckMate[CheckMateCount].x && CheckMate[0].y == CheckMate[CheckMateCount].y)
				return TRUE;

			if (Back[x][y + 1].index == 2)
				result = Critical(3, x, y + 1);
			else if (Back[x][y + 1].index == 0)
				result = Critical(0, x, y + 1);
			else if (Back[x][y + 1].index == 3)
				result = Critical(2, x, y + 1);
		}
		else
			return FALSE;
	}
	return result;
}


//LRESULT CALLBACK ChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
//
//	PAINTSTRUCT ps;
//	HDC hdc;
//	HBITMAP Block[7];
//	static HBITMAP hbit, oldBit1, oldBit2;;
//
//	static int sizeX, sizeY;
//
//	switch (uMsg) {
//
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	}
//	return DefWindowProc(hWnd, uMsg, wParam, lParam);
//}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK(윈도우 핸들, 처리해야 할 메시지의 값,입력장치의 값1,2)
{
	PAINTSTRUCT ps;
	HDC hdc, memdc, memdc2, chardc; //화면 출력을 위한 DC들 chardc는 캐릭터dc입니다.
	HBRUSH hBrush, oldBrush;		//전부다 합쳐서 출력을 준비하는 DC들
	HBITMAP block[6], BackGround, Animation[6], cup[4];
	//	CImage g_cimgTest;
	//	HDC adddc = g_cimgTest; 

	//HDC gdc = g_cimgTest.GetDC();
	static HBITMAP hbit, hbit2, oldBit1, oldBit2;
	static RECT rtChar;                              //주인공의 캐릭터 영역
	static RECT rtplane;							//충돌체크할 비행기 바나나 영역
	static RECT rtCup;			//충돌체크할 컵 바나나 영역
	static int x = 200 + 300;
	static Drop play;
	static int xpos = 3;
	static int ypos = 0;
	static int NextBlock;
	static int y = 0;
	static int count = 0;
	static int speedRate = 300;
	static int temp;
	static int inputX, inputY;
	static int resetTimer = 0;
	static BOOL STOPCHECK = FALSE;
	static BOOL ASSISTANT = FALSE;
	static BOOL CLOSECHECK = FALSE;
	static BOOL RESET = FALSE;
	static int CHANGE = 0;
	static int assistant = 0;
	static int checkTimer = 0;
	static int check = 0;
	static BOOL StopCheck = FALSE;
	static BOOL Ani = FALSE;
	static char text[100];
	static int dir;
	static int startPos = 5;
	static BOOL tempCheck = FALSE;
	static int tempX, tempY, tempX2 = 0, tempY2 = 0, tempCount = 0, charterCount = 0;
	static int Resize = 0;
	static int musicCount = 0;
	//static int Mapcount[20] = { 0 };
	//HWND STOP, RESET, CLOSE,GO;
	HWND hButton;
	HWND MAP1, MAP2, MAP3;
	switch (iMessage) //메시지의 번호
	{
	case WM_CREATE:
		play.x = x;
		play.y = y;
		play.xpos = 5;
		play.ypos = 0;
		play.index = rand() % 6;
		//SetTimer(hWnd, 6, 1000, NULL);
		/*	STOP = CreateWindow("button", "일시 정지", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 800, 450, 100, 30, hWnd, (HMENU)IDC_STOP, g_hinst, NULL);
			GO = CreateWindow("button", "재 개", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 800, 500, 100, 30, hWnd, (HMENU)IDC_GO, g_hinst, NULL);
			RESET = CreateWindow("button", "다시 시작", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 800, 550, 100, 30, hWnd, (HMENU)IDC_RESET, g_hinst, NULL);
			CLOSE = CreateWindow("button", "종 료", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 800, 600, 100, 30, hWnd, (HMENU)IDC_CLOSE, g_hinst, NULL);*/
		MAP1 = CreateWindow("button", "5 X 10 크기", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 800, 460, 100, 30, hWnd, (HMENU)IDC_BOARD_SIZE1, g_hinst, NULL);
		MAP2 = CreateWindow("button", "10 X 20 크기", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 800, 500, 100, 30, hWnd, (HMENU)IDC_BOARD_SIZE2, g_hinst, NULL);
		MAP3 = CreateWindow("button", "20 X 40 크기", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 800, 540, 100, 30, hWnd, (HMENU)IDC_BOARD_SIZE3, g_hinst, NULL);
		SetTimer(hWnd, 1, 300, NULL);
		
		SetTimer(hWnd, 6, 1000, NULL);
		SetTimer(hWnd, 7, 1000, NULL);
		break;

	case WM_COMMAND:
		switch (wParam) {
		case IDC_BOARD_SIZE1:
			xSize = 5;
			ySize = 10;
			for (int i = 0; i < xSize; ++i) {
				for (int j = 0; j < ySize; ++j) {
					Back[i][j].check = 0;
					Back[i][j].index = 0;
				}
			}
			CHANGE = 2;
			startPos = 2;
			Resize = 50;
			assistant = 1;
			play.x = x;
			play.y = y;
			play.xpos = startPos;
			play.ypos = 0;
			play.index = rand() % 6;
			speedRate = 300;
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 6);
			SetTimer(hWnd, 1, speedRate, NULL);
			SetTimer(hWnd, 6, 1000, NULL);
			STOPCHECK = FALSE;
			SetFocus(hWnd);
			break;

		case IDC_BOARD_SIZE2:
			xSize = 10;
			ySize = 20;
			for (int i = 0; i < xSize; ++i) {
				for (int j = 0; j < ySize; ++j) {
					Back[i][j].check = 0;
					Back[i][j].index = 0;
				}
			}
			CHANGE = 0;
			Resize = 0;
			assistant = 0;
			play.x = x;
			play.y = y;
			startPos = 5;
			play.xpos = startPos;
			play.ypos = 0;
			play.index = rand() % 6;
			speedRate = 300;
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 6);
			SetTimer(hWnd, 1, speedRate, NULL);
			SetTimer(hWnd, 6, 1000, NULL);
			STOPCHECK = FALSE;
			SetFocus(hWnd);
			break;

		case IDC_BOARD_SIZE3:
			xSize = 20;
			ySize = 40;
			for (int i = 0; i < xSize; ++i) {
				for (int j = 0; j < ySize; ++j) {
					Back[i][j].check = 0;
					Back[i][j].index = 0;
				}
			}
			CHANGE = 1;
			Resize = 0;
			assistant = -1;
			play.x = x;
			play.y = y;
			startPos = 10;
			play.xpos = startPos;
			play.ypos = 0;
			play.index = rand() % 6;
			speedRate = 300;
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 6);
			SetTimer(hWnd, 1, speedRate, NULL);
			SetTimer(hWnd, 6, 1000, NULL);
			STOPCHECK = FALSE;
			SetFocus(hWnd);
			break;
		}
		break;

	case WM_KEYDOWN:
		if (wParam == VK_LEFT) {
			if (play.xpos >= 0) {
				if (Back[play.xpos][play.ypos].check != 1 && Back[play.xpos][play.ypos + 1].check != 1) {
					play.x -= 50;
					play.xpos -= 1;
				}
			}
		}

		else if (wParam == VK_RIGHT) {
			if (play.xpos <= xSize - 3) {
				if (Back[play.xpos + 2][play.ypos].check != 1 && Back[play.xpos + 2][play.ypos + 1].check != 1) {
					play.x += 50;
					play.xpos += 1;
				}
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

		else if (wParam == VK_SPACE) {
			for (int i = ySize - 1; i >= 0; --i) {
				if (Back[play.xpos + 1][i].check != 1) {
					Back[play.xpos + 1][i].check = 1;
					Back[play.xpos + 1][i].index = play.index;

					if (i != ySize - 1) {
						if ((play.index == 0 || play.index == 5) && i != ySize - 1) {
							for (int i = 0; i < 100; i++)
							{
								CheckMate[i].x = -1;
								CheckMate[i].y = -1;
							}
							CheckMateCount = 0;
							CheckMate[0].x = play.xpos + 1;
							CheckMate[0].y = i;
							bubble = 0;
							checkStart = play.index;
							STOP = FALSE;
							if (Critical(0, play.xpos + 1, i)) {
								for (int j = 0; j < CheckMateCount; j++) {
									Back[CheckMate[j].x][CheckMate[j].y].check = 0;
									Back[CheckMate[j].x][CheckMate[j].y].index = 0;
								}
								if (bubble > 1) {
									for (int p = 0; p < bubble; p++) {
										CheckMate[CheckMateCount + p].x = CheckMate[p].x;
										CheckMate[CheckMateCount + p].y = CheckMate[p].y;
									}
									for (int p = bubble; p < CheckMateCount; p++) {
										CheckMate[p - bubble].x = CheckMate[CheckMateCount - p + bubble - 1].x;
										CheckMate[p - bubble].y = CheckMate[CheckMateCount - p + bubble - 1].y;
									}
									for (int p = CheckMateCount; p < CheckMateCount + bubble; p++) {
										CheckMate[CheckMateCount - bubble + p - CheckMateCount].x = CheckMate[p].x;
										CheckMate[CheckMateCount - bubble + p - CheckMateCount].y = CheckMate[p].y;
									}
								}
								dir = play.index;
								SetTimer(hWnd, 5, 100, NULL);
								KillTimer(hWnd, 1);
								KillTimer(hWnd, 6);
							}
						}
						else if ((play.index == 1 || play.index == 3 || play.index == 4) && i != ySize - 1) {
							for (int i = 0; i < 100; i++)
							{
								CheckMate[i].x = -1;
								CheckMate[i].y = -1;
							}
							CheckMateCount = 0;
							CheckMate[0].x = play.xpos + 1;
							CheckMate[0].y = i;
							checkStart = play.index;
							bubble = 0;
							STOP = FALSE;
							if (Critical(2, play.xpos + 1, i)) {
								for (int j = 0; j < CheckMateCount; j++) {
									Back[CheckMate[j].x][CheckMate[j].y].check = 0;
									Back[CheckMate[j].x][CheckMate[j].y].index = 0;
								}
								if (bubble > 1) {
									for (int p = 0; p < bubble; p++) {
										CheckMate[CheckMateCount + p].x = CheckMate[p].x;
										CheckMate[CheckMateCount + p].y = CheckMate[p].y;
									}
									for (int p = bubble; p < CheckMateCount; p++) {
										CheckMate[p - bubble].x = CheckMate[CheckMateCount - p + bubble - 1].x;
										CheckMate[p - bubble].y = CheckMate[CheckMateCount - p + bubble - 1].y;
									}
									for (int p = CheckMateCount; p < CheckMateCount + bubble; p++) {
										CheckMate[CheckMateCount - bubble + p - CheckMateCount].x = CheckMate[p].x;
										CheckMate[CheckMateCount - bubble + p - CheckMateCount].y = CheckMate[p].y;
									}
								}
								dir = play.index;
								SetTimer(hWnd, 5, 100, NULL);
								KillTimer(hWnd, 1);
								KillTimer(hWnd, 6);
							}
						}
					}
					else {
						if (play.index == 0) {
							for (int i = 0; i < 100; i++)
							{
								CheckMate[i].x = -1;
								CheckMate[i].y = -1;
							}
							CheckMate[0].x = play.xpos + 1;
							CheckMate[0].y = i;
							checkStart = play.index;
							bubble = 0;
							STOP = FALSE;
							CheckMateCount = 0;
							if (Critical(0, play.xpos + 1, i)) {
								for (int j = 0; j < CheckMateCount; j++) {
									Back[CheckMate[j].x][CheckMate[j].y].check = 0;
									Back[CheckMate[j].x][CheckMate[j].y].index = 0;
								}
								if (bubble > 1) {
									for (int p = 0; p < bubble; p++) {
										CheckMate[CheckMateCount + p].x = CheckMate[p].x;
										CheckMate[CheckMateCount + p].y = CheckMate[p].y;
									}
									for (int p = bubble; p < CheckMateCount; p++) {
										CheckMate[p - bubble].x = CheckMate[CheckMateCount - p + bubble - 1].x;
										CheckMate[p - bubble].y = CheckMate[CheckMateCount - p + bubble - 1].y;
									}
									for (int p = CheckMateCount; p < CheckMateCount + bubble; p++) {
										CheckMate[CheckMateCount - bubble + p - CheckMateCount].x = CheckMate[p].x;
										CheckMate[CheckMateCount - bubble + p - CheckMateCount].y = CheckMate[p].y;
									}
								}
								dir = play.index;
								SetTimer(hWnd, 5, 100, NULL);
								KillTimer(hWnd, 1);
								KillTimer(hWnd, 6);
							}
						}
						else if (play.index == 5) {
							for (int i = 0; i < 100; i++)
							{
								CheckMate[i].x = -1;
								CheckMate[i].y = -1;
							}
							CheckMate[0].x = play.xpos + 1;
							CheckMate[0].y = i;
							checkStart = play.index;
							bubble = 0;
							STOP = FALSE;
							CheckMateCount = 0;
							if (Critical(0, play.xpos + 1,i)) {
								for (int j = 0; j < CheckMateCount; j++) {
									Back[CheckMate[j].x][CheckMate[j].y].check = 0;
									Back[CheckMate[j].x][CheckMate[j].y].index = 0;
								}
								if (bubble > 1) {
									for (int p = 0; p < bubble; p++) {
										CheckMate[CheckMateCount + p].x = CheckMate[p].x;
										CheckMate[CheckMateCount + p].y = CheckMate[p].y;
									}
									for (int p = bubble; p < CheckMateCount; p++) {
										CheckMate[p - bubble].x = CheckMate[CheckMateCount - p + bubble - 1].x;
										CheckMate[p - bubble].y = CheckMate[CheckMateCount - p + bubble - 1].y;
									}
									for (int p = CheckMateCount; p < CheckMateCount + bubble; p++) {
										CheckMate[CheckMateCount - bubble + p - CheckMateCount].x = CheckMate[p].x;
										CheckMate[CheckMateCount - bubble + p - CheckMateCount].y = CheckMate[p].y;
									}
								}
								dir = play.index;
								SetTimer(hWnd, 5, 100, NULL);
								KillTimer(hWnd, 1);
								KillTimer(hWnd, 6);
							}
						}
						else if (play.index == 1) {
							for (int i = 0; i < 100; i++)
							{
								CheckMate[i].x = -1;
								CheckMate[i].y = -1;
							}
							CheckMate[0].x = play.xpos + 1;
							CheckMate[0].y = i;
							checkStart = play.index;
							bubble = 0;
							STOP = FALSE;
							CheckMateCount = 0;
							if (Critical(2, play.xpos + 1, i)) {
								for (int j = 0; j < CheckMateCount; j++) {
									Back[CheckMate[j].x][CheckMate[j].y].check = 0;
									Back[CheckMate[j].x][CheckMate[j].y].index = 0;
								}
								if (bubble > 1) {
									for (int p = 0; p < bubble; p++) {
										CheckMate[CheckMateCount + p].x = CheckMate[p].x;
										CheckMate[CheckMateCount + p].y = CheckMate[p].y;
									}
									for (int p = bubble; p < CheckMateCount; p++) {
										CheckMate[p - bubble].x = CheckMate[CheckMateCount - p + bubble - 1].x;
										CheckMate[p - bubble].y = CheckMate[CheckMateCount - p + bubble - 1].y;
									}
									for (int p = CheckMateCount; p < CheckMateCount + bubble; p++) {
										CheckMate[CheckMateCount - bubble + p - CheckMateCount].x = CheckMate[p].x;
										CheckMate[CheckMateCount - bubble + p - CheckMateCount].y = CheckMate[p].y;
									}
								}
								dir = play.index;
								SetTimer(hWnd, 5, 100, NULL);
								KillTimer(hWnd, 1);
								KillTimer(hWnd, 6);
							}
						}
						else if (play.index == 3) {
							for (int i = 0; i < 100; i++)
							{
								CheckMate[i].x = -1;
								CheckMate[i].y = -1;
							}
							CheckMate[0].x = play.xpos + 1;
							CheckMate[0].y = i;
							bubble = 0;
							checkStart = play.index;
							STOP = FALSE;
							CheckMateCount = 0;
							if (Critical(2, play.xpos + 1, i)) {
								for (int j = 0; j < CheckMateCount; j++) {
									Back[CheckMate[j].x][CheckMate[j].y].check = 0;
									Back[CheckMate[j].x][CheckMate[j].y].index = 0;
								}
								if (bubble > 1) {
									for (int p = 0; p < bubble; p++) {
										CheckMate[CheckMateCount + p].x = CheckMate[p].x;
										CheckMate[CheckMateCount + p].y = CheckMate[p].y;
									}
									for (int p = bubble; p < CheckMateCount; p++) {
										CheckMate[p - bubble].x = CheckMate[CheckMateCount - p + bubble - 1].x;
										CheckMate[p - bubble].y = CheckMate[CheckMateCount - p + bubble - 1].y;
									}
									for (int p = CheckMateCount; p < CheckMateCount + bubble; p++) {
										CheckMate[CheckMateCount - bubble + p - CheckMateCount].x = CheckMate[p].x;
										CheckMate[CheckMateCount - bubble + p - CheckMateCount].y = CheckMate[p].y;
									}
								}
								dir = play.index;
								SetTimer(hWnd, 5, 100, NULL);
								KillTimer(hWnd, 1);
								KillTimer(hWnd, 6);
							}
						}
						else if (play.index == 4) {
							for (int i = 0; i < 100; i++)
							{
								CheckMate[i].x = -1;
								CheckMate[i].y = -1;
							}
							CheckMate[0].x = play.xpos + 1;
							CheckMate[0].y = i;
							bubble = 0;
							checkStart = play.index;
							STOP = FALSE;
							CheckMateCount = 0;
							if (Critical(2, play.xpos + 1, i)) {
								for (int j = 0; j < CheckMateCount; j++) {
									Back[CheckMate[j].x][CheckMate[j].y].check = 0;
									Back[CheckMate[j].x][CheckMate[j].y].index = 0;
								}
								if (bubble > 1) {
									for (int p = 0; p < bubble; p++) {
										CheckMate[CheckMateCount + p].x = CheckMate[p].x;
										CheckMate[CheckMateCount + p].y = CheckMate[p].y;
									}
									for (int p = bubble; p < CheckMateCount; p++) {
										CheckMate[p - bubble].x = CheckMate[CheckMateCount - p + bubble - 1].x;
										CheckMate[p - bubble].y = CheckMate[CheckMateCount - p + bubble - 1].y;
									}
									for (int p = CheckMateCount; p < CheckMateCount + bubble; p++) {
										CheckMate[CheckMateCount - bubble + p - CheckMateCount].x = CheckMate[p].x;
										CheckMate[CheckMateCount - bubble + p - CheckMateCount].y = CheckMate[p].y;
									}
								}
								dir = play.index;
								SetTimer(hWnd, 5, 100, NULL);
								KillTimer(hWnd, 1);
								KillTimer(hWnd, 6);
							}
						}
					}
					play.x = x;
					play.y = y;
					play.xpos = startPos;
					play.ypos = 0;
					play.index = NextBlock;
					NextBlock = rand() % 6;
					break;
				}
			}
		}

		InvalidateRect(hWnd, NULL, FALSE);
		break;


	case WM_CHAR:

		break;
	case WM_TIMER:
		switch (wParam) {
		case 1:
			play.y += 50;
			play.ypos += 1;
			if (Back[play.xpos + 1][play.ypos + 1].check == 1) {
				Back[play.xpos + 1][play.ypos].check = 1;
				Back[play.xpos + 1][play.ypos].index = play.index;

				if (play.index == 0 || play.index == 5) {
					for (int i = 0; i < 100; i++)
					{
						CheckMate[i].x = -1;
						CheckMate[i].y = -1;
					}
					CheckMateCount = 0;
					CheckMate[0].x = play.xpos + 1;
					CheckMate[0].y = play.ypos;
					checkStart = play.index;
					bubble = 0;
					STOP = FALSE;
					if (Critical(0, play.xpos + 1, play.ypos)) {
						for (int i = 0; i <= CheckMateCount; i++) {
							Back[CheckMate[i].x][CheckMate[i].y].check = 0;
							Back[CheckMate[i].x][CheckMate[i].y].index = 0;
						}

						if (bubble > 1){
							for (int p = 0; p < bubble; p++){
								CheckMate[CheckMateCount + p].x = CheckMate[p].x;
								CheckMate[CheckMateCount + p].y = CheckMate[p].y;
							}
							for (int p = bubble; p < CheckMateCount; p++){
								CheckMate[p - bubble].x = CheckMate[CheckMateCount - p + bubble - 1].x;
								CheckMate[p - bubble].y = CheckMate[CheckMateCount - p + bubble - 1].y;
							}
							for (int p = CheckMateCount; p < CheckMateCount + bubble; p++){
								CheckMate[CheckMateCount - bubble + p - CheckMateCount].x = CheckMate[p].x;
								CheckMate[CheckMateCount - bubble + p - CheckMateCount].y = CheckMate[p].y;
							}
						}
						dir = play.index;
						SetTimer(hWnd, 5, 100, NULL);
						KillTimer(hWnd, 1);
						KillTimer(hWnd, 6);
					}
				}
				else if (play.index == 1 || play.index == 3 || play.index == 4) {
					for (int i = 0; i < 100; i++)
					{
						CheckMate[i].x = -1;
						CheckMate[i].y = -1;
					}
					CheckMateCount = 0;
					CheckMate[0].x = play.xpos + 1;
					CheckMate[0].y = play.ypos;
					checkStart = play.index;
					bubble = 0;
					STOP = FALSE;
					if (Critical(2, play.xpos + 1, play.ypos)) {
						for (int i = 0; i <= CheckMateCount; i++) {
							Back[CheckMate[i].x][CheckMate[i].y].check = 0;
							Back[CheckMate[i].x][CheckMate[i].y].index = 0;
						}
						if (bubble > 1) {
							for (int p = 0; p < bubble; p++) {
								CheckMate[CheckMateCount + p].x = CheckMate[p].x;
								CheckMate[CheckMateCount + p].y = CheckMate[p].y;
							}
							for (int p = bubble; p < CheckMateCount; p++) {
								CheckMate[p - bubble].x = CheckMate[CheckMateCount - p + bubble - 1].x;
								CheckMate[p - bubble].y = CheckMate[CheckMateCount - p + bubble - 1].y;
							}
							for (int p = CheckMateCount; p < CheckMateCount + bubble; p++) {
								CheckMate[CheckMateCount - bubble + p - CheckMateCount].x = CheckMate[p].x;
								CheckMate[CheckMateCount - bubble + p - CheckMateCount].y = CheckMate[p].y;
							}
						}
						dir = play.index;
						SetTimer(hWnd, 5, 100, NULL);
						KillTimer(hWnd, 1);
						KillTimer(hWnd, 6);
					}
				}
				play.x = x;
				play.y = y;
				play.xpos = startPos;
				play.ypos = 0;
				play.index = NextBlock;
				NextBlock = rand() % 6;
			}

			if (play.ypos == ySize - 1) {
				Back[play.xpos + 1][ySize - 1].check = 1;
				Back[play.xpos + 1][ySize - 1].index = play.index;

				if (play.index == 0 ) {
					for (int i = 0; i < 100; i++)
					{
						CheckMate[i].x = -1;
						CheckMate[i].y = -1;
					}
					CheckMate[0].x = play.xpos + 1;
					CheckMate[0].y = play.ypos;
					checkStart = play.index;
					bubble = 0;
					STOP = FALSE;
					CheckMateCount = 0;
					if (Critical(0, play.xpos + 1, play.ypos)) {
						for (int i = 0; i <= CheckMateCount; i++) {
							Back[CheckMate[i].x][CheckMate[i].y].check = 0;
							Back[CheckMate[i].x][CheckMate[i].y].index = 0;
						}
						if (bubble > 1) {
							for (int p = 0; p < bubble; p++) {
								CheckMate[CheckMateCount + p].x = CheckMate[p].x;
								CheckMate[CheckMateCount + p].y = CheckMate[p].y;
							}
							for (int p = bubble; p < CheckMateCount; p++) {
								CheckMate[p - bubble].x = CheckMate[CheckMateCount - p + bubble - 1].x;
								CheckMate[p - bubble].y = CheckMate[CheckMateCount - p + bubble - 1].y;
							}
							for (int p = CheckMateCount; p < CheckMateCount + bubble; p++) {
								CheckMate[CheckMateCount - bubble + p - CheckMateCount].x = CheckMate[p].x;
								CheckMate[CheckMateCount - bubble + p - CheckMateCount].y = CheckMate[p].y;
							}
						}
						dir = play.index;
						SetTimer(hWnd, 5, 100, NULL);
						KillTimer(hWnd, 1);
						KillTimer(hWnd, 6);
					}
				}
				else if (play.index == 5) {
					for (int i = 0; i < 100; i++)
					{
						CheckMate[i].x = -1;
						CheckMate[i].y = -1;
					}
					CheckMate[0].x = play.xpos + 1;
					CheckMate[0].y = play.ypos;
					checkStart = play.index;
					bubble = 0;
					STOP = FALSE;
					CheckMateCount = 0;
					if (Critical(0, play.xpos + 1, play.ypos)) {
						for (int i = 0; i <= CheckMateCount; i++) {
							Back[CheckMate[i].x][CheckMate[i].y].check = 0;
							Back[CheckMate[i].x][CheckMate[i].y].index = 0;
						}
						if (bubble > 1) {
							for (int p = 0; p < bubble; p++) {
								CheckMate[CheckMateCount + p].x = CheckMate[p].x;
								CheckMate[CheckMateCount + p].y = CheckMate[p].y;
							}
							for (int p = bubble; p < CheckMateCount; p++) {
								CheckMate[p - bubble].x = CheckMate[CheckMateCount - p + bubble - 1].x;
								CheckMate[p - bubble].y = CheckMate[CheckMateCount - p + bubble - 1].y;
							}
							for (int p = CheckMateCount; p < CheckMateCount + bubble; p++) {
								CheckMate[CheckMateCount - bubble + p - CheckMateCount].x = CheckMate[p].x;
								CheckMate[CheckMateCount - bubble + p - CheckMateCount].y = CheckMate[p].y;
							}
						}
						dir = play.index;
						SetTimer(hWnd, 5, 100, NULL);
						KillTimer(hWnd, 1);
						KillTimer(hWnd, 6);
					}
				}
				else if (play.index == 1) {
					for (int i = 0; i < 100; i++)
					{
						CheckMate[i].x = -1;
						CheckMate[i].y = -1;
					}
					CheckMate[0].x = play.xpos + 1;
					CheckMate[0].y = play.ypos;
					checkStart = play.index;
					bubble = 0;
					STOP = FALSE;
					CheckMateCount = 0;
					if (Critical(2, play.xpos + 1, play.ypos)) {
						for (int i = 0; i <= CheckMateCount; i++) {
							Back[CheckMate[i].x][CheckMate[i].y].check = 0;
							Back[CheckMate[i].x][CheckMate[i].y].index = 0;
						}
						if (bubble > 1) {
							for (int p = 0; p < bubble; p++) {
								CheckMate[CheckMateCount + p].x = CheckMate[p].x;
								CheckMate[CheckMateCount + p].y = CheckMate[p].y;
							}
							for (int p = bubble; p < CheckMateCount; p++) {
								CheckMate[p - bubble].x = CheckMate[CheckMateCount - p + bubble - 1].x;
								CheckMate[p - bubble].y = CheckMate[CheckMateCount - p + bubble - 1].y;
							}
							for (int p = CheckMateCount; p < CheckMateCount + bubble; p++) {
								CheckMate[CheckMateCount - bubble + p - CheckMateCount].x = CheckMate[p].x;
								CheckMate[CheckMateCount - bubble + p - CheckMateCount].y = CheckMate[p].y;
							}
						}
						dir = play.index;
						SetTimer(hWnd, 5, 100, NULL);
						KillTimer(hWnd, 1);
						KillTimer(hWnd, 6);
					}
				}
				else if (play.index == 3) {
					for (int i = 0; i < 100; i++)
					{
						CheckMate[i].x = -1;
						CheckMate[i].y = -1;
					}
					CheckMate[0].x = play.xpos + 1;
					CheckMate[0].y = play.ypos;
					checkStart = play.index;
					bubble = 0;
					STOP = FALSE;
					CheckMateCount = 0;
					if (Critical(2, play.xpos + 1, play.ypos)) {
						for (int i = 0; i <= CheckMateCount; i++) {
							Back[CheckMate[i].x][CheckMate[i].y].check = 0;
							Back[CheckMate[i].x][CheckMate[i].y].index = 0;
						}
						if (bubble > 1) {
							for (int p = 0; p < bubble; p++) {
								CheckMate[CheckMateCount + p].x = CheckMate[p].x;
								CheckMate[CheckMateCount + p].y = CheckMate[p].y;
							}
							for (int p = bubble; p < CheckMateCount; p++) {
								CheckMate[p - bubble].x = CheckMate[CheckMateCount - p + bubble - 1].x;
								CheckMate[p - bubble].y = CheckMate[CheckMateCount - p + bubble - 1].y;
							}
							for (int p = CheckMateCount; p < CheckMateCount + bubble; p++) {
								CheckMate[CheckMateCount - bubble + p - CheckMateCount].x = CheckMate[p].x;
								CheckMate[CheckMateCount - bubble + p - CheckMateCount].y = CheckMate[p].y;
							}
						}
						dir = play.index;
						SetTimer(hWnd, 5, 100, NULL);
						KillTimer(hWnd, 1);
						KillTimer(hWnd, 6);
					}
				}
				else if (play.index == 4) {
					for (int i = 0; i < 100; i++)
					{
						CheckMate[i].x = -1;
						CheckMate[i].y = -1;
					}
					CheckMate[0].x = play.xpos + 1;
					CheckMate[0].y = play.ypos;
					checkStart = play.index;
					bubble = 0;
					STOP = FALSE;
					CheckMateCount = 0;
					if (Critical(2, play.xpos + 1, play.ypos)) {
						for (int i = 0; i <= CheckMateCount; i++) {
							Back[CheckMate[i].x][CheckMate[i].y].check = 0;
							Back[CheckMate[i].x][CheckMate[i].y].index = 0;
						}
						if (bubble > 1) {
							for (int p = 0; p < bubble; p++) {
								CheckMate[CheckMateCount + p].x = CheckMate[p].x;
								CheckMate[CheckMateCount + p].y = CheckMate[p].y;
							}
							for (int p = bubble; p < CheckMateCount; p++) {
								CheckMate[p - bubble].x = CheckMate[CheckMateCount - p + bubble - 1].x;
								CheckMate[p - bubble].y = CheckMate[CheckMateCount - p + bubble - 1].y;
							}
							for (int p = CheckMateCount; p < CheckMateCount + bubble; p++) {
								CheckMate[CheckMateCount - bubble + p - CheckMateCount].x = CheckMate[p].x;
								CheckMate[CheckMateCount - bubble + p - CheckMateCount].y = CheckMate[p].y;
							}
						}
						dir = play.index;
						SetTimer(hWnd, 5, 100, NULL);
						KillTimer(hWnd, 1);
						KillTimer(hWnd, 6);
					}
				}
				play.x = x;
				play.y = y;
				play.xpos = startPos;
				play.ypos = 0;
				play.index = NextBlock;
				NextBlock = rand() % 6;
			}

			InvalidateRect(hWnd, NULL, FALSE);
			break;

			case 2:
				RESET = TRUE;
				resetTimer++;
				if (resetTimer >= 10) {
					RESET = FALSE;
					resetTimer = 0;
					KillTimer(hWnd, 2);
				}
				break;

			case 3:
				Ani = TRUE;
				if (checkTimer == 7) {
					for (int i = xSize - 1; i >= 0; i--) {
						for (int j = ySize - 2; j >= 0; j--) {
							while (j < ySize - 1 && Back[i][j].check == 1 && Back[i][j + 1].check == 0) {
								Back[i][j + 1].check = Back[i][j].check;
								Back[i][j].check = 0;
								Back[i][j + 1].index = Back[i][j].index;
								Back[i][j + 1].x = Back[i][j].x;
								Back[i][j + 1].y = Back[i][j].y;
								j++;
							}
						}
					}
					checkTimer = 0;
					KillTimer(hWnd, 3);
					Ani = FALSE;
					SetTimer(hWnd, 1, speedRate, NULL);
					SetTimer(hWnd, 6, 1000, NULL);
				}
				else
					checkTimer++;
				InvalidateRect(hWnd, NULL, FALSE);
				break;

			case 4:
				Ani = TRUE;
				if (checkTimer == 7) {
					for (int i = xSize - 1; i >= 0; i--) {
						for (int j = ySize - 2; j >= 0; j--) {
							while (j < ySize - 1 && Back[i][j].check == 1 && Back[i][j + 1].check == 0) {
								Back[i][j + 1].check = Back[i][j].check;
								Back[i][j].check = 0;
								Back[i][j + 1].index = Back[i][j].index;
								Back[i][j + 1].x = Back[i][j].x;
								Back[i][j + 1].y = Back[i][j].y;
								j++;
							}
						}
					}
					checkTimer = 0;
					KillTimer(hWnd, 4);
					Ani = FALSE;
					SetTimer(hWnd, 1,speedRate, NULL);
					SetTimer(hWnd, 6, 1000, NULL);
				}
				else
					checkTimer++;
				InvalidateRect(hWnd, NULL, FALSE);
				break;

			case 5:
				if (tempX2 == CheckMateCount) {
					tempX2 = 0;
					tempCheck = FALSE;
					KillTimer(hWnd, 5);
					if (dir == 0 || dir == 5)
						SetTimer(hWnd, 3, 100, NULL);
					else if(dir == 1 || dir == 3 || dir == 4)
						SetTimer(hWnd, 4, 100, NULL);
				}
				else {
					tempCheck = TRUE;
					tempX = CheckMate[tempX2].x;
					tempY = CheckMate[tempX2].y;
					tempX2++;
					charterCount++;
					charterCount = charterCount % 4;
				}
				InvalidateRect(hWnd, NULL, FALSE);
				break;

			case 6:
				speedRate -= 1;
				KillTimer(hWnd, 1);
				SetTimer(hWnd, 1, speedRate, NULL);
				break;

			case 7:
				if (musicCount % 204 == 0) {
					PlaySound(MAKEINTRESOURCE(119), g_hinst, SND_RESOURCE | SND_ASYNC); // 204초짜리 브금임
					musicCount++;
				}
				else
					musicCount++;
				break;
		}
		break;

		case WM_LBUTTONDOWN:
			inputX = LOWORD(lParam);
			inputY = HIWORD(lParam);

			if (inputX > 800 && inputX < 900 && inputY > 300 && inputY < 330) {
				if (STOPCHECK == FALSE) {
					STOPCHECK = TRUE;
					KillTimer(hWnd, 1);
					KillTimer(hWnd, 6);
				}
				else {
					STOPCHECK = FALSE;
					SetTimer(hWnd, 1, speedRate, NULL);
					SetTimer(hWnd, 6, 1000, NULL);
				}
			}

			else if (inputX > 800 && inputX < 900 && inputY > 340 && inputY < 370) {
				if (ASSISTANT == FALSE) 
					ASSISTANT = TRUE;
				else
					ASSISTANT = FALSE;
			}

			else if (inputX > 800 && inputX < 900 && inputY > 380 && inputY < 410) {
				if (RESET == FALSE) {
					SetTimer(hWnd, 2, 100, NULL);
					for (int i = 0; i < xSize; ++i) {
						for (int j = 0; j < ySize; ++j) {
							Back[i][j].check = 0;
							Back[i][j].index = 0;
						}
					}
					play.x = x;
					play.y = y;
					play.xpos = startPos;
					play.ypos = 0;
					play.index = rand() % 6;
					speedRate = 300;
					KillTimer(hWnd, 1);
					KillTimer(hWnd, 6);
					SetTimer(hWnd, 1, speedRate, NULL);
					SetTimer(hWnd, 6, 1000, NULL);
					STOPCHECK = FALSE;
					//SetTimer(hWnd, 6, 500, NULL);
				}
			}

			//else if (inputX > 800 & inputX < 900 && inputY > 420 && inputY < 450) {
			//	if (xSize == 10) {
			//		xSize = 20;
			//		ySize = 40;
			//		assistant = -1;
			//		for (int i = 0; i < xSize; ++i) {
			//			for (int j = 0; j < ySize; ++j) {
			//				Back[i][j].check = 0;
			//				Back[i][j].index = 0;
			//			}
			//		}
			//		play.x = x;
			//		play.y = y;
			//		play.xpos = 5;
			//		play.ypos = 0;
			//		play.index = rand() % 6;
			//		CHANGE = 1;
			//	}
			//	else if (xSize == 20) {
			//		xSize = 10;
			//		ySize = 20;
			//		assistant = 0;
			//		for (int i = 0; i < xSize; ++i) {
			//			for (int j = 0; j < ySize; ++j) {
			//				Back[i][j].check = 0;
			//				Back[i][j].index = 0;
			//			}
			//		}
			//		play.x = x;
			//		play.y = y;
			//		play.xpos = startPos;
			//		play.ypos = 0;
			//		play.index = rand() % 6;
			//		CHANGE = 0;
			//	}
			//}

			else if (inputX > 800 && inputX < 900 && inputY > 580 && inputY < 610) {
				PostQuitMessage(0);
			}
			InvalidateRect(hWnd, NULL, FALSE);
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

		Animation[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(108));
		Animation[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(109));
		Animation[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(110));
		Animation[3] = LoadBitmap(g_hinst, MAKEINTRESOURCE(111));
		Animation[4] = LoadBitmap(g_hinst, MAKEINTRESOURCE(112));
		Animation[5] = LoadBitmap(g_hinst, MAKEINTRESOURCE(113));

		cup[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(114));
		cup[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(115));
		cup[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(116));
		cup[3] = LoadBitmap(g_hinst, MAKEINTRESOURCE(117));

		BackGround = LoadBitmap(g_hinst, MAKEINTRESOURCE(107));

		oldBit1 = (HBITMAP)SelectObject(memdc, hbit);
		oldBit2 = (HBITMAP)SelectObject(memdc2, NULL);
		
		if (STOPCHECK == FALSE) {
			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			oldBrush = (HBRUSH)SelectObject(memdc, hBrush);
		}
		else {
			hBrush = CreateSolidBrush(RGB(0, 255, 0));
			oldBrush = (HBRUSH)SelectObject(memdc, hBrush);
		}
		Rectangle(memdc, 800, 300, 900, 330);
		wsprintf(text, TEXT("일시 정지"));
		if (STOPCHECK == FALSE)
			SetBkColor(memdc, RGB(255, 0, 0));
		else
			SetBkColor(memdc, RGB(0, 255, 0));
		TextOut(memdc, 816, 305, text, strlen(text));

	

		if (ASSISTANT == FALSE) {
			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			oldBrush = (HBRUSH)SelectObject(memdc, hBrush);
			Rectangle(memdc, 800, 340, 900, 370);
			wsprintf(text, TEXT("어시스턴트"));
			SetBkColor(memdc, RGB(255, 0, 0));
			TextOut(memdc, 811, 345, text, strlen(text));
		}
		else {
			hBrush = CreateSolidBrush(RGB(0, 255, 0));
			oldBrush = (HBRUSH)SelectObject(memdc, hBrush);
			Rectangle(memdc, 800, 340, 900, 370);
			wsprintf(text, TEXT("어시스턴트"));
			SetBkColor(memdc, RGB(0, 255, 0));
			TextOut(memdc, 811, 345, text, strlen(text));
		}

		if (RESET == FALSE) {
			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			oldBrush = (HBRUSH)SelectObject(memdc, hBrush);
			Rectangle(memdc, 800, 380, 900, 410);
			wsprintf(text, TEXT("RESET"));
			SetBkColor(memdc, RGB(255, 0, 0));
			TextOut(memdc, 827, 385, text, strlen(text));
		}
		else {
			hBrush = CreateSolidBrush(RGB(0, 255, 0));
			oldBrush = (HBRUSH)SelectObject(memdc, hBrush);
			Rectangle(memdc, 800, 380, 900, 410);
			wsprintf(text, TEXT("RESET"));
			SetBkColor(memdc, RGB(0, 255, 0));
			TextOut(memdc, 827, 385, text, strlen(text));
		}

	

		if (CHANGE == 0) {
			hBrush = CreateSolidBrush(RGB(255, 255, 0));
			oldBrush = (HBRUSH)SelectObject(memdc, hBrush);
			Rectangle(memdc, 800, 420, 900, 450);
			wsprintf(text, TEXT("크기 조절"));
			SetBkColor(memdc, RGB(255, 255, 0));
			TextOut(memdc, 816, 425, text, strlen(text));
		}
		else if (CHANGE == 1) {
			hBrush = CreateSolidBrush(RGB(0, 255, 255));
			oldBrush = (HBRUSH)SelectObject(memdc, hBrush);
			Rectangle(memdc, 800, 420, 900, 450);
			wsprintf(text, TEXT("크기 조절"));
			SetBkColor(memdc, RGB(0, 255, 255));
			TextOut(memdc, 816, 425, text, strlen(text));
		}
		else if (CHANGE == 2) {
			hBrush = CreateSolidBrush(RGB(255, 0, 255));
			oldBrush = (HBRUSH)SelectObject(memdc, hBrush);
			Rectangle(memdc, 800, 420, 900, 450);
			wsprintf(text, TEXT("크기 조절"));
			SetBkColor(memdc, RGB(255, 0, 255));
			TextOut(memdc, 816, 425, text, strlen(text));
		}
		

		if (CLOSECHECK == FALSE) {
			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			oldBrush = (HBRUSH)SelectObject(memdc, hBrush);
		}
		Rectangle(memdc, 800, 580, 900, 610);
		wsprintf(text, TEXT("종료"));
		SetBkColor(memdc, RGB(255, 0, 0));
		TextOut(memdc, 834, 585, text, strlen(text));

		//for (int i = 0; i < 10; ++i) {
		//	for (int j = 0; j < 10; ++j) {
		//		Back[j][i].x = 200 + (i * 50);
		//		Back[j][i].y = 200 + (j * 50);
		//	}
		//}

		Rectangle(memdc, 200, 0, 700, 1000);
		(HBITMAP)SelectObject(memdc2, BackGround);
		StretchBlt(memdc, 200, 0, 500, 1000, memdc2, 0, 0, 259,194, SRCCOPY);
		(HBITMAP)SelectObject(memdc2, block[play.index]);
		TransparentBlt(memdc, 200 + (play.xpos + 1) * (500 / xSize), play.ypos * (1000 / ySize), 500 / xSize, 1000 / ySize, memdc2, 0, 0, 50, 50, RGB(255, 255, 255)); //크기확대는 첫번쨰에서 결정함.
		SelectObject(memdc2, NULL);

		for (int i = 0; i < xSize; ++i) {
			for (int j = 0; j < ySize; ++j) {
				if (Back[i][j].check == 1) {
					(HBITMAP)SelectObject(memdc2, block[Back[i][j].index]);
					TransparentBlt(memdc, 200 + (500 / xSize * i), (1000 / ySize * j), 500 / xSize, 1000 / ySize, memdc2, 0, 0, 50, 50, RGB(255, 255, 255));
					SelectObject(memdc2, NULL);
				}
			}
		}

		if (ASSISTANT == TRUE) {
			for (int j = ySize - 1; j >= 0; j--) {
				if (Back[play.xpos + 1][j].check == 0) {
					(HBITMAP)SelectObject(memdc2, block[play.index]); 
					TransparentBlt(memdc, (play.xpos + assistant) * (500 / xSize) + 250 - Resize, j * (1000 / ySize), 500 / xSize, 1000 / ySize, memdc2, 0, 0, 50, 50, RGB(0, 0, 0)); //크기확대는 첫번쨰에서 결정함.
					SelectObject(memdc2, NULL);
					break;
				}

			}
		}

		if(Ani == TRUE)
		for (int i = 0; i < CheckMateCount; ++i) {
			(HBITMAP)SelectObject(memdc2, Animation[checkTimer]);
			TransparentBlt(memdc, 200 + (500 / xSize) * CheckMate[i].x, (1000 / ySize) * CheckMate[i].y, 500 / xSize, 1000 / ySize, memdc2, 0, 0, 150, 150, RGB(0, 0, 255));
			SelectObject(memdc2, NULL);
		}

		if (tempCheck == TRUE) {
			(HBITMAP)SelectObject(memdc2, Animation[charterCount]);
			TransparentBlt(memdc, 200 + (500 / xSize) * tempX, (1000 / ySize) * tempY, 500 / xSize, 1000 / ySize, memdc2, 0, 0, 150, 150, RGB(0, 0, 255));
			SelectObject(memdc2, NULL);
		}

		for (int i = 0; i <= xSize; i++) {
			MoveToEx(memdc, 200 + (500 / xSize) * (i), 0, NULL);
			LineTo(memdc, 200 + (500 / xSize) * (i), 1000);
		}
		for (int i = 0; i <= ySize; i++) {
			MoveToEx(memdc, 200, (1000 / ySize) * (i), NULL);
			LineTo(memdc, 700, (1000 / ySize) * (i));
		}

		(HBITMAP)SelectObject(memdc2, block[NextBlock]);
		TransparentBlt(memdc, 800, 30, 50, 50, memdc2, 0, 0, 50, 50, RGB(0, 0, 0));
		SelectObject(memdc2, NULL);

		BitBlt(hdc, 0, 0, 1280, 1080, memdc, 0, 0, SRCCOPY);



		SelectObject(memdc, oldBit1);

		for (int i = 0; i < 6; ++i) {
			DeleteObject(block[i]);
			DeleteObject(Animation[i]);
		}

		for (int i = 0; i < 4; ++i) {
			DeleteObject(cup[i]);
		}

		DeleteObject(oldBit1);
		DeleteObject(oldBit2);
		DeleteObject(hBrush);
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



//BOOL CollisionCheck(int x, int y, int mx, int my)
//{
//	if (mx > x&&mx<x + 150 && my>y&&my < y + 150)
//	{
//		return TRUE;
//	}
//	else
//	{
//		return FALSE;
//	}
//}
