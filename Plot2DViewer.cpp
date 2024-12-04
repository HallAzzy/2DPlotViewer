#include <windows.h>
#include <windowsx.h>
#include "Data.h"
#include "Scene2D.h"

LRESULT _stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);						// прототип оконной процедуры
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)		// основна€ процедура
{
	// ѕерва€ составл€юща€ часть основной процедуры - создание окна: сначала описываетс€ оконный класс wc, затем создаЄтс€ окно hWnd
	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;						// им€ оконной процедуры, закрепленной за данным классом
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;						// идентификатор приложени€, содержащий адрес начала выделенной ему области пам€ти
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(6);
	wc.lpszMenuName = 0;							// меню в оконном классе отсутствует
	wc.lpszClassName = TEXT("MainWindowClass");		// им€ оконного класса, используемое при создании экземпл€ров окна
	RegisterClass(&wc);								// регистраци€ класса wc

	HWND hWnd = CreateWindow(						// hWnd - дескриптор, идентифицирующий окно; функци€ создани€ окна заполн€ет дескриптор hWnd ненулевым значением
		TEXT("MainWindowClass"),					// им€ оконного класса
		TEXT("Plot2D Viewer"),						// заголовок окна
		WS_OVERLAPPEDWINDOW,						// стиль окна
		200, 200, 500, 300,							// координаты на экране левого верхнего угла окна, его ширина и высота
		nullptr, nullptr, hInstance, nullptr);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ¬тора€ составл€юща€ часть основной процедуры - основной цикл обработки системных сообщений, который ожидает сообщени€ и рассылает их соответствующим окнам
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))				// функци€ GetMessage выбирает из очереди сообщение и заносит его в структуру msg
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);						// функци€ DispatchMessage оповещает систему о необходимости вызова оконной процедуры
	}

	return 0;
}

// ¬ основном модуле объ€вл€етс€ только одна глобальна€ переменна€ - создаЄтс€ объект класса Scene2D
// ¬се дальнейшие действи€ осуществл€ютс€ посредством обращени€ к методам, реализованным в этом классе
Scene2D scene(X0, Y0, px, py);

LRESULT _stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)		// оконна€ процедура принимает и обрабатывает все сообщени€, отправленные окну
{
	switch (msg)
	{
		case WM_PAINT:
		{
			HDC dc = GetDC(hWnd);
			scene.Clear(dc);				// вызов реализованного в классе Camera2D метода, отвечающего за очистку рабочей области окна hWnd
			scene.Plot(dc, Cardioid);		// вызов реализованного в классе Scene2D метода, отвечающего за отрисовку графика синусоиды
			ReleaseDC(hWnd, dc);
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		case WM_SIZE:
		{
			HDC dc = GetDC(hWnd);
			scene.SetResolution(dc);
			ReleaseDC(hWnd, dc);
			InvalidateRect(hWnd, nullptr, false);
			return 0;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		default:
		{
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}
	return 0;
}
