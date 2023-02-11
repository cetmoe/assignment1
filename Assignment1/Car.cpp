#include "Car.h"

#include <string>

using namespace std;

Car::Car(bool _horizontal, COLORREF _color)
{
	progress = 0;
	horizontal = _horizontal;
	color = _color;
}

float Car::getProgress() { return progress; }

bool Car::isHorizontal() { return horizontal; }

void Car::move(float amount)
{
	progress += (float) amount;
}

void Car::draw(HWND hWnd, HDC hdc)
{
	RECT screen;
	if (GetClientRect(hWnd, &screen))
	{
		int wWidth = (&screen)->right;
		int wHeight = (&screen)->bottom;
		float carWidth = 0.02;
		float carHeight = 0.02;

		HBRUSH carColor = CreateSolidBrush(color);

		float left, top, right, bottom = 0;

		if (horizontal)
		{

			left = wWidth * (progress - carWidth);
			top = wHeight * ((float) 1 / 2 - carHeight);
			right = wWidth * (progress + carWidth);
			bottom = wHeight * ((float) 1 / 2 + carHeight);

		}
		else
		{
			left = wWidth * ((float) 1 / 2 - carHeight / 2);
			top = wHeight * (progress - carWidth);
			right = wWidth * ((float) 1 / 2 + carHeight / 2);
			bottom = wHeight * (progress + carWidth);
		}

		SelectObject(hdc, carColor);
		Rectangle(hdc, left, top, right, bottom);
		DeleteObject(carColor);
	}
}