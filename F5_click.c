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
	WndClass.lpszClassName = "win"; // CreateWindowA�� ù ��° ���ڿ��� ������ �̸��� ���ƾ���.
	WndClass.hInstance = hinstance;
	WndClass.lpfnWndProc = WndProc; // ���� ���α׷� �޽��� ť���� ������ ó���ϴ� ������ ���α׷��Ӱ� ���� �������� �ϴµ� �ٷ� �̷��� ����� �ϴ� �Լ��̴�.
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.lpszMenuName = NULL;




	RegisterClassA(&WndClass);


	// �����츦 ����
	CreateWindowA("win", "F5_click", WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_BORDER, 300, 200, 400, 100, NULL, NULL, hinstance, NULL);

	while (GetMessageA(&msg, 0, 0, 0)) // �޽����� �ݺ������� ������. �� �Լ��� ��� �޽���ť���� �޽����� ���� ���� �ִ�.
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg); // �޽����� �� ������ ������ �Ʒ� �޽��� ó�� �Լ��� ȣ��� �� �ֵ��� Dispatch ���ش�.
	}
	return 0;

}
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) // �޽����� �� ������ �Լ�ó���� �� �ִ� �Լ�
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
		CreateWindowA("button", "F5���� Ŭ�� ����(F1)", WS_VISIBLE | WS_CHILD, 30, 15, 200, 30, hwnd, (HMENU)start, g_inst, NULL);
		CreateWindowA("button", "�ߴ�(F2)", WS_VISIBLE | WS_CHILD, 250, 15, 100, 30, hwnd, (HMENU)stop, g_inst, NULL);
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
	return DefWindowProcA(hwnd, uMsg, wParam, lParam); //�⺻���� �޽���(�ִ�, �ּ�ȭ ��)�� �����찡 ��� ���ִ� �Լ�
}