// test_DL.cpp: Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "test_DL.h"
#include "MachineLearning.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <cstring>

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

MachineLearning getML() {
	return MachineLearning(3, 2, 2, 1);
}
MachineLearning ml = getML();

ofstream log1("c:/log/dl_test.log");
bool stop = false;
LPRECT rect = new RECT();
// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Mapper(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TESTDL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTDL));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
		if (stop) break;
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
	
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTDL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TESTDL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 960, 960, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void PaintMap(HDC hdc, MachineLearning ml, RECT rect) {
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	double step_w = 1.0 / (double)width;
	double step_h = 1.0 / (double)height;
	for (int w = 0; w < width; w++) {
		for (int h = 0; h < height; h++) {
			double color = ml.setInput(vector<double>({ (double)w*step_w,(double)h*step_h })).front();
			SetPixel(hdc, w, h, RGB(color>0.5?255:0,0, color<=0.5 ? 255 : 0));
		}
	}
}

void PaintNeuro(HDC hdc, MachineLearning ml,RECT rect) {
	//Ellipse(hdc)
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	int max_size_level = 0;
	for (auto level : *ml.levels) {
		max_size_level = max(level->size(), max_size_level);
	}
	int l=0, t = 0, r = 0, b = 0;
	int level_h = 0, delta_level_h_before = 0, delta_level_h_current = 0;
	int level_h_before = 0,level_h_current = 0;
	int radius = round(min(height / (2 * max_size_level), width / (2 * (ml.levels->size() + 1))));
	int step_h = height / (1 * max_size_level);
	int step_w = width / (1 * (ml.levels->size() + 1));
	int X = 0, Y = round(step_h*max_size_level/2);



	for (auto level_it = ml.levels->begin(); level_it != ml.levels->end(); level_it++) {
		vector<Neuron*>* level = *level_it;
		level_h_before = level_h_current;
		delta_level_h_before = delta_level_h_current;
		level_h_current = level->size()*(step_h)-step_h;
		delta_level_h_current = round((level_h_before - level_h_current) / 2 + delta_level_h_before);
		t = Y + delta_level_h_current;
		l += step_w;
		for (Neuron* current_neuron : *level) {
			r = l + radius;
			b = t + radius;
			current_neuron->X = l + radius / 2;
			current_neuron->Y = t + radius / 2;
			Ellipse(hdc, l, t, r, b);
			char str[255] = {'\0'};
			
			
			sprintf_s(str,255,"out: %f bias: %f", current_neuron->output, current_neuron->bias);
			TextOutA(hdc, current_neuron->X, current_neuron->Y, str,28);
			t += step_h;
			for (Edge* current_edge : *current_neuron->managers) {
				HGDIOBJ original = SelectObject(hdc, GetStockObject(DC_PEN));
				HPEN dc_pen_line = CreatePen(PS_SOLID, max(4, min(15,abs(current_edge->weight))), current_edge->weight>0 ? RGB(0, 0, 255) : RGB(255, 0, 0));
				SelectObject(hdc, dc_pen_line);
				MoveToEx(hdc, round(current_edge->left->X), round(current_edge->left->Y), NULL);
				LineTo(hdc, round(current_neuron->X), round(current_neuron->Y));
				SelectObject(hdc, original);
				DeleteObject(dc_pen_line);
			}
		}
	}

}

double ML_XOR(HWND hwnd) {
	std::vector<double> input1;
	std::vector<double> input2;
	std::vector<double> input3;
	std::vector<double> input4;
	std::vector<double> expected1;
	std::vector<double> expected2;
	std::vector<double> expected3;
	std::vector<double> expected4;

	input1.insert(input1.end(), 0.0);
	input1.insert(input1.end(), 0.0);
	input2.insert(input2.end(), 0.0);
	input2.insert(input2.end(), 1.0);
	input3.insert(input3.end(), 1.0);
	input3.insert(input3.end(), 0.0);
	input4.insert(input4.end(), 1.0);
	input4.insert(input4.end(), 1.0);

	expected1.insert(expected1.end(), 0.0);
	expected2.insert(expected2.end(), 1.0);
	expected3.insert(expected3.end(), 1.0);
	expected4.insert(expected4.end(), 0.0);

	//expected1.insert(expected1.end(), 1.0);
	//expected2.insert(expected2.end(), 0);
	//expected3.insert(expected3.end(), 0);
	//expected4.insert(expected4.end(), 1.0);
	double eps = 0;
	double eps_current = 0;
	LPRECT rect = new RECT();

	if (!log1.is_open()) {
		terminate();
	}
	GetClientRect(hwnd, rect);
	int length = 2000;
	for (int i = 0; i < length; i++) {
		if (stop) break;
		eps = 0;
		
		log1 << (eps_current = ml.assert(input1, expected1)) << " ";
		eps += eps_current;
		//InvalidateRect(hwnd, rect, 1.0);

		log1 << (eps_current = ml.assert(input2, expected2)) << " ";
		eps += eps_current;
		//InvalidateRect(hwnd, rect, 1.0);

		log1 << (eps_current = ml.assert(input3, expected3)) << " ";
		eps += eps_current;
		//InvalidateRect(hwnd, rect, 1.0);

		log1 << (eps_current = ml.assert(input4, expected4)) << " ";
		eps += eps_current;

		if (i % 100 == 0) { InvalidateRect(hwnd, rect, 1.0); }

		log1 << eps << endl;
		
		if (eps < 0.01) {
			break;
		}

		MSG msg;
		while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			if (stop) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}


	return eps;
}

void ML_XOR_MULTY(HWND hwnd) {
	int count_good = 0;
	for (size_t i = 0; i < 100; i++)
	{
		ml = getML();
		if (ML_XOR(hwnd) < 0.01) {
			count_good++;
		}
		log1 << "OK: " << count_good << endl;
	}
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND — обработать меню приложения
//  WM_PAINT — отрисовать главное окно
//  WM_DESTROY — отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
			case IDM_EXIT:
				stop = true;
				Sleep(1000);
				DestroyWindow(hWnd);
				break; 
			case IDM_Start_Machine_Learning:
				ML_XOR(hWnd);
				break;
			case IDM_Restart_Machine_Learning:
				GetClientRect(hWnd, rect);
				InvalidateRect(hWnd, rect, 1.0);
				stop = true;
				Sleep(1000);
				stop = false;
				ml = getML();
				ML_XOR(hWnd);
				break;
			case IDM_Start_Multy_Machine_Learning:
				GetClientRect(hWnd, rect);
				InvalidateRect(hWnd, rect, 1.0);
				stop = true;
				Sleep(1000);
				stop = false;
				ML_XOR_MULTY(hWnd);
				break;
			case IDM_Get_Map_Machine_Learning:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Mapper);
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
			GetClientRect(hWnd, rect);
			PaintNeuro(hdc, ml, *rect);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		stop = true;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDlg, &ps);
		GetClientRect(hDlg, rect);
		PaintNeuro(hdc, ml, *rect);
		// TODO: Добавьте сюда любой код прорисовки, использующий HDC...
		EndPaint(hDlg, &ps);
	}
	break;
	}
	return (INT_PTR)FALSE;
}
// Обработчик сообщений для окна "Mapper".
INT_PTR CALLBACK Mapper(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDlg, &ps);
		GetClientRect(hDlg, rect);
		PaintMap(hdc, ml, *rect);
		// TODO: Добавьте сюда любой код прорисовки, использующий HDC...
		EndPaint(hDlg, &ps);
	}
	break;
	}
	return (INT_PTR)FALSE;
}
