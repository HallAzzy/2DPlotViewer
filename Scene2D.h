#pragma once
#ifndef SCENE_2D_H
#define SCENE_2D_H

#include <cmath>
#include "Camera2D.h"

class Scene2D : public Camera2D
{
private:
    typedef double (*Func)(double);
public:
    Scene2D(double X0, double Y0, double px, double py) : Camera2D(X0, Y0, px, py)
    {
    }

    void Plot(HDC dc, Func f, bool axes = true)
    {
        if (axes)
            Axes(dc);	// ��������� ���� ���������

        const double step = 0.001;			// ��� ��������� ����
        const double startTheta = 0.0;		// ��������� ���� � ��������
        const double endTheta = 2 * M_PI;	// �������� ���� � ��������

        // ���������� ����������
        double previousX = 0.0,
            previousY = 0.0;

        bool firstPoint = true; // ��� ��������� ��������� �����

        for (double theta = startTheta; theta <= endTheta; theta += step)
        {
            double r = f(theta); // ���������� �������

            // �������������� � ������� ��������� ����������
            double xWorld = PolarToWorldX(theta, r);
            double yWorld = PolarToWorldY(theta, r);

            // �������������� � �������� ����������.
            int xScreen = WorldToScreenX(xWorld);
            int yScreen = WorldToScreenY(yWorld); // ��������� �������� ��� Y � �������� �����������

            if (firstPoint)
            {
                // ������������� ��������� �����
                MoveTo(xWorld, yWorld);
                firstPoint = false;
            }
            else
            {
                // ������ ����� �� ���������� �����
                LineTo(dc, xWorld, yWorld);
            }
        }
    }
};
#endif SCENE_2D_H