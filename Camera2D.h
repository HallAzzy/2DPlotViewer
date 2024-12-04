#pragma once
#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include <windef.h>
#include <windows.h>

class Camera2D
{
protected:
	double X0, Y0;						// �������� ���������� ������ ������� ������� ���������
	double px, py;						// ������� �������� ������� ������� ���������, ���������� � ��������
	int W, H;							// ���������� ������� ������� ����

	double PolarToWorldX(double theta, double r)
	{
		return r * cos(theta);
	}

	double PolarToWorldY(double theta, double r)
	{
		return r * sin(theta);
	}

	int WorldToScreenX(double X)		// ������� �� ������� ��������� � �������� (��� ��������)
	{
		return static_cast<int>(X0 + px * X);
	}

	int WorldToScreenY(double Y)		// ������� �� ������� ��������� � �������� (��� ��������)
	{
		return static_cast<int>(Y0 - py * Y);
	}

	double ScreenToWorldX(int X)		// ������� �� �������� ��������� � ������� (��� ��������)
	{
		return (X - X0) / px;
	}

	double ScreenToWorldY(int Y)		// ������� �� �������� ��������� � ������� (��� ��������)
	{
		return (Y0 - Y) / py;
	}

	double L()							// �������� ����� ������� ������� ������� ���� (� ������� �����������)
	{
		return -X0 / px;
	}

	double R()							// �������� ������ ������� ������� ������� ���� (� ������� �����������)
	{
		return (W - X0) / px;
	}

	double T()       // �������� ����� ������� ������� ������� ���� (� ������� �����������)
	{
		return ScreenToWorldY(0);
	}
	double B()       // �������� ������ ������� ������� ������� ���� (� ������� �����������)
	{
		return ScreenToWorldY(H);
	}

private:
	double posX, posY;					// ������� ������������ ������� � ������� ����������� (��� ������� MoveTo � LineTo)

public:
	Camera2D(double X0, double Y0, double px, double py) : X0(X0), Y0(Y0), px(px), py(py)
	{
	}

	void Clear(HDC dc)
	{
		// ������� ������� ������� ����
		RECT rect;
		GetClientRect(WindowFromDC(dc), &rect); // �������� ������� ����
		PatBlt(dc, 0, 0, rect.right, rect.bottom, WHITENESS); // ��������� ������� ����� ������
	}

	void SetResolution(HDC dc)
	{
		// ������ ��������� ���������� ��� ��������� �������� ����
		// � ��� �������� �������� ������� W, H, � ����� ������������� �������� ���������� X0, Y0, px, py ����� �������, ����� ���������� ���������� �������� �� ������������ ����
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
		// ����������� ������������ ������� (posX, posY)
		posX = X;
		posY = Y;
	}

	void LineTo(HDC dc, double X, double Y)
	{
		// ��������� ����� �� ������� ������� ������������ ������� � ����� (X, Y) � ��� ����������� � ��� �����
		// �������������� ������� ��������� � ��������
		int Xs1 = WorldToScreenX(posX);
		int Ys1 = WorldToScreenY(posY);
		int Xs2 = WorldToScreenX(X);
		int Ys2 = WorldToScreenY(Y);

		// ��������� �����
		MoveToEx(dc, Xs1, Ys1, nullptr);
		::LineTo(dc, Xs2, Ys2);

		// ���������� ������� ������������ �������
		posX = X;
		posY = Y;
	}

	void Axes(HDC dc)
	{
		// ��������� ������������ ����
		// ��� X
		MoveTo(L(), 0.0);
		LineTo(dc, R(), 0.0);

		// ��� Y
		MoveTo(0.0, ScreenToWorldY(H));
		LineTo(dc, 0.0, ScreenToWorldY(0));
	}
};

#endif CAMERA_2D_H
