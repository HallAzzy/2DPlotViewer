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
            Axes(dc);	// отрисовка осей координат

        const double step = 0.001;			// шаг изменения угла
        const double startTheta = 0.0;		// начальный угол в радианах
        const double endTheta = 2 * M_PI;	// конечный угол в радианах

        // предыдущие координаты
        double previousX = 0.0,
            previousY = 0.0;

        bool firstPoint = true; // для установки начальной точки

        for (double theta = startTheta; theta <= endTheta; theta += step)
        {
            double r = f(theta); // вычисление радиуса

            // преобразование в мировые декартовы координаты
            double xWorld = PolarToWorldX(theta, r);
            double yWorld = PolarToWorldY(theta, r);

            // преобразование в экранные координаты.
            int xScreen = WorldToScreenX(xWorld);
            int yScreen = WorldToScreenY(yWorld); // учитываем инверсию оси Y в экранных координатах

            if (firstPoint)
            {
                // устанавливаем начальную точку
                MoveTo(xWorld, yWorld);
                firstPoint = false;
            }
            else
            {
                // рисуем линию от предыдущей точки
                LineTo(dc, xWorld, yWorld);
            }
        }
    }
};
#endif SCENE_2D_H