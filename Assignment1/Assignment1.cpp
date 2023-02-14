// Assignment1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Assignment1.h"
#include <deque>
#include <iostream>
#include <string>

using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

const int SWITCH_TRAFFIC_LIGHT = 100;
const int MOVE_CAR = 200;
const int ADD_CAR = 300;

TrafficController trafficController = TrafficController(0.45, 0.45, 0.55, 0.55);


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					  _In_opt_ HINSTANCE hPrevInstance,
					  _In_ LPWSTR    lpCmdLine,
					  _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_ASSIGNMENT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ASSIGNMENT1));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASSIGNMENT1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_ASSIGNMENT1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
							  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	RECT client;
	GetClientRect(hWnd, &client);

	trafficController.addLight(0, 20, 40, true);
	trafficController.addLight(2, 95, 40, false);

	SetTimer(hWnd, SWITCH_TRAFFIC_LIGHT, 1000, (TIMERPROC) NULL);
	SetTimer(hWnd, MOVE_CAR, 6, (TIMERPROC) NULL);
	SetTimer(hWnd, ADD_CAR, 1000, (TIMERPROC) NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_LBUTTONDOWN:
		{
			trafficController.addCar(true);
			InvalidateRect(hWnd, NULL, false);
		}
		break;
		case WM_RBUTTONDOWN:
		{
			trafficController.addCar(false);
			InvalidateRect(hWnd, NULL, false);
		}
		break;
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case VK_LEFT:
					trafficController.setCarFlow(-0.1, true);
					break;
				case VK_RIGHT:
					trafficController.setCarFlow(0.1, true);
					break;
				case VK_UP:
					trafficController.setCarFlow(0.1, false);
					break;
				case VK_DOWN:
					trafficController.setCarFlow(-0.1, false);
					break;
			}
			break;
		}
		break;
		case WM_TIMER:
		{
			switch (wParam)
			{
				case SWITCH_TRAFFIC_LIGHT:
				{
					trafficController.progressLights();
					InvalidateRect(hWnd, NULL, false);
				}
				break;
				case MOVE_CAR:
				{
					trafficController.progressCars(0.0025);
					InvalidateRect(hWnd, NULL, false);
				}
				break;
				case ADD_CAR:
				{
					if ((float) (rand() % 100) / 100 < trafficController.getProbability(true))
					{
						trafficController.addCar(true);
					}
					if ((float) (rand() % 100) / 100 < trafficController.getProbability(false))
					{
						trafficController.addCar(false);
					}
				}
				break;
			}
		}
		break;
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			RECT screen;
			if (GetClientRect(hWnd, &screen))
			{
				int wWidth = (&screen)->right;
				int wHeight = (&screen)->bottom;
				float roadWidth = 0.03;

				//create roads
				HBRUSH hRoad = CreateSolidBrush(RGB(126, 126, 126));
				HPEN hDeletePen = CreatePen(PS_NULL, 0, 0);
				SelectObject(hdc, hRoad);
				SelectObject(hdc, hDeletePen);
				Rectangle(hdc, -1, wHeight / 2 - wHeight * roadWidth, wWidth + 1, wHeight / 2 + wHeight * roadWidth);
				Rectangle(hdc, wWidth / 2 - wWidth * roadWidth, -1, wWidth / 2 + wWidth * roadWidth, wHeight + 1);

				// cleanup objects
				DeleteObject(hDeletePen);
				DeleteObject(hRoad);
			}

			trafficController.draw(hWnd, hdc);


			EndPaint(hWnd, &ps);
		}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		case WM_INITDIALOG:
			return (INT_PTR) TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR) TRUE;
			}
			break;
	}
	return (INT_PTR) FALSE;
}
