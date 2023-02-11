#include "TrafficLight.h"

using namespace std;

TrafficLight::TrafficLight(int _state, int _x, int _y, bool _horizontal)
{
	state = _state;
	x = _x;
	y = _y;
	horizontal = _horizontal;
}

int TrafficLight::getState()
{
	return state;
}

COLORREF TrafficLight::getStateColor(int position)
{
	return colorStates[state][position];
}

bool TrafficLight::isHorizontal()
{
	return horizontal;
}

void TrafficLight::progressState()
{
	state = (state + 1) % 4;
}

void TrafficLight::draw(HDC hdc)
{
	// draws the black rectangle
	HBRUSH hBackground = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hdc, hBackground);
	RoundRect(hdc, x, y, x + 50, y + 140, 25, 25);

	HBRUSH redBrush = CreateSolidBrush(getStateColor(RED));
	HBRUSH yellowBrush = CreateSolidBrush(getStateColor(YELLOW));
	HBRUSH greenBrush = CreateSolidBrush(getStateColor(GREEN));

	SelectObject(hdc, redBrush);
	Ellipse(hdc, x + 5, y + 5, x + 45, y + 45);

	SelectObject(hdc, yellowBrush);
	Ellipse(hdc, x + 5, y + 50, x + 45, y + 90);

	SelectObject(hdc, greenBrush);
	Ellipse(hdc, x + 5, y + 95, x + 45, y + 135);

	DeleteObject(hBackground);
	DeleteObject(redBrush);
	DeleteObject(yellowBrush);
	DeleteObject(greenBrush);
}
