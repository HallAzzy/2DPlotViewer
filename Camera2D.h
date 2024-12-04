#pragma once
#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include <windef.h>
#include <windows.h>

class Camera2D
{
protected:
	double X0, Y0;						// экранные координаты начала мировой системы координат
	double px, py;						// единицы масштаба мировой системы координат, выраженные в пикселях
	int W, H;							// разрешение рабочей области окна

	double PolarToWorldX(double theta, double r)
	{
		return r * cos(theta);
	}

	double PolarToWorldY(double theta, double r)
	{
		return r * sin(theta);
	}

	int WorldToScreenX(double X)		// переход от мировых координат к экранным (для абсциссы)
	{
		return static_cast<int>(X0 + px * X);
	}

	int WorldToScreenY(double Y)		// переход от мировых координат к экранным (для ординаты)
	{
		return static_cast<int>(Y0 - py * Y);
	}

	double ScreenToWorldX(int X)		// переход от экранных координат к мировым (для абсциссы)
	{
		return (X - X0) / px;
	}

	double ScreenToWorldY(int Y)		// переход от экранных координат к мировым (для ординаты)
	{
		return (Y0 - Y) / py;
	}

	double L()							// абсцисса левой границы рабочей области окна (в мировых координатах)
	{
		return -X0 / px;
	}

	double R()							// абсцисса правой границы рабочей области окна (в мировых координатах)
	{
		return (W - X0) / px;
	}

	double T()       // ордината левой границы рабочей области окна (в мировых координатах)
	{
		return ScreenToWorldY(0);
	}
	double B()       // ордината правой границы рабочей области окна (в мировых координатах)
	{
		return ScreenToWorldY(H);
	}

private:
	double posX, posY;					// позиция графического курсора в мировых координатах (для функций MoveTo и LineTo)

public:
	Camera2D(double X0, double Y0, double px, double py) : X0(X0), Y0(Y0), px(px), py(py)
	{
	}

	void Clear(HDC dc)
	{
		// очистка рабочей области окна
		RECT rect;
		GetClientRect(WindowFromDC(dc), &rect); // получаем размеры окна
		PatBlt(dc, 0, 0, rect.right, rect.bottom, WHITENESS); // заполняем область белым цветом
	}

	void SetResolution(HDC dc)
	{
		// данная процедура вызывается при изменении размеров окна
		// в ней задаются значения величин W, H, а также настраиваются значения параметров X0, Y0, px, py таким образом, чтобы обеспечить одинаковые масштабы по координатным осям
		RECT rect;
		GetClientRect(WindowFromDC(dc), &rect);

		H = rect.bottom + 1;
		W = rect.right + 1;

		double BT = (B() + T()) / 2;
		double RL = (R() - L()) / 2 * (H / W);
		double Bn = BT + RL;
		double Tn = BT - RL;
		double previousY = Y0;

		Y0 = WorldToScreenY((Tn + Bn) / 2);

		py *= Y0 / previousY;
	}

	void MoveTo(double X, double Y)
	{
		// перемещение графического курсора (posX, posY)
		posX = X;
		posY = Y;
	}

	void LineTo(HDC dc, double X, double Y)
	{
		// отрисовка линии из текущей позиции графического курсора в точку (X, Y) и его перемещение в эту точку
		// преобразование мировых координат в экранные
		int Xs1 = WorldToScreenX(posX);
		int Ys1 = WorldToScreenY(posY);
		int Xs2 = WorldToScreenX(X);
		int Ys2 = WorldToScreenY(Y);

		// отрисовка линии
		MoveToEx(dc, Xs1, Ys1, nullptr);
		::LineTo(dc, Xs2, Ys2);

		// обновление позиции графического курсора
		posX = X;
		posY = Y;
	}

	void Axes(HDC dc)
	{
		// отрисовка координатных осей
		// ось X
		MoveTo(L(), 0.0);
		LineTo(dc, R(), 0.0);

		// ось Y
		MoveTo(0.0, ScreenToWorldY(H));
		LineTo(dc, 0.0, ScreenToWorldY(0));
	}
};

#endif CAMERA_2D_H
