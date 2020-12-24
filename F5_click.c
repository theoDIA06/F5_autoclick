#define _CRT_OBSOLETE_NO_WARNINGS
#include <Windows.h>
#include <stdbool.h>
#include <stdlib.h>

POINT ptMouse;
HINSTANCE g_inst;
MSG msg;
WNDCLASSA WndClass;
HANDLE autokeybdthread;
bool autoF5toogle = false;
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#define start 100
#define stop  200

DWORD WINAPI autoF5(void* lpVoid)
{
	while (true)
	{
		if (autoF5toogle)
		{
				keybd_event(VK_F5, 0, 0x22, 0);
				keybd_event(VK_F5, 0, 0x00, 0);
				Sleep(5);
		}
	}
}


int CALLBACK WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WndClass.lpszClassName = "win"; // CreateWindowA의 첫 번째 인자에서 정해준 이름과 같아야함.
	WndClass.hInstance = hinstance;
	WndClass.lpfnWndProc = WndProc; // 응용 프로그램 메시지 큐에서 꺼내서 처리하는 과정을 프로그래머가 직접 만들어줘야 하는데 바로 이러한 기능을 하는 함수이다.
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.lpszMenuName = NULL;




	RegisterClassA(&WndClass);


	// 윈도우를 생성
	CreateWindowA("win", "F5_click", WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_BORDER, 300, 200, 400, 100, NULL, NULL, hinstance, NULL);

	while (GetMessageA(&msg, 0, 0, 0)) // 메시지를 반복적으로 꺼낸다. 이 함수를 써야 메시지큐에서 메시지를 꺼낼 수가 있다.
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg); // 메시지가 올 때마다 꺼내서 아래 메시지 처리 함수가 호출될 수 있도록 Dispatch 해준다.
	}
	return 0;

}
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) // 메시지가 올 때마다 함수처리할 수 있는 함수
{
	if (uMsg == WM_HOTKEY)
	{
		if (wParam == 1)
		{
			autoF5toogle = true;
		}
		else if (wParam == 0)
		{
			autoF5toogle = false;
		}
	}
	switch (uMsg)
	{
	case WM_CLOSE:
		CloseHandle(autokeybdthread);
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		autokeybdthread = CreateThread(NULL, 0, autoF5, &autoF5toogle, 0, NULL);
		RegisterHotKey(hwnd, 1, 0, VK_F1);
		RegisterHotKey(hwnd, 0, 0, VK_F2);
		CreateWindowA("button", "F5오토 클릭 시작(F1)", WS_VISIBLE | WS_CHILD, 30, 15, 200, 30, hwnd, (HMENU)start, g_inst, NULL);
		CreateWindowA("button", "중단(F2)", WS_VISIBLE | WS_CHILD, 250, 15, 100, 30, hwnd, (HMENU)stop, g_inst, NULL);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case start:
			autoF5toogle = true;
			break;
		case stop:
			autoF5toogle = false;
			break;
		default:
			break;
		}
	default:
		break;
	}
	return DefWindowProcA(hwnd, uMsg, wParam, lParam); //기본적인 메시지(최대, 최소화 등)를 윈도우가 모두 해주는 함수
}