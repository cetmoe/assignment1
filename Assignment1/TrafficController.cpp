#include "TrafficController.h"
#include <iostream>
#include <string>

TrafficController::TrafficController()
{
}

TrafficController::TrafficController(float _left, float _top, float _right, float _bottom)
{
	left = _left;
	top = _top;
	right = _right;
	bottom = _bottom;
}

void TrafficController::addLight(int state, int x, int y, bool horizontal)
{
	trafficLights.push_back(TrafficLight(state, x, y, horizontal));
}

void TrafficController::addCar(bool horizontal)
{
	cars.push_front(Car(horizontal));
}

void TrafficController::cycle()
{
	if (cycleLength[cycleState] == cycleProgress)
	{
		cycleState = (cycleState + 1) % 4;
		cycleProgress = 0;
	}
	else
	{
		cycleProgress += 1;
	}

	OutputDebugStringW(L"state: ");
	OutputDebugStringW(std::to_wstring(cycleState).c_str());
	OutputDebugStringW(L"\n");

	OutputDebugStringW(L"state: ");
	OutputDebugStringW(std::to_wstring(cycleProgress).c_str());
	OutputDebugStringW(L"\n");
}

void TrafficController::progressLights()
{
	bool progressable = true;
	for (auto& car : cars)
	{
		if (withinBounds(car))
		{
			progressable = false;
		}
	}

	if (progressable)
	{
		cycle();

		if (cycleProgress == 0)
		{
			for (auto& light : trafficLights)
			{
				light.progressState();
			}
		}
	}
}

TrafficLight TrafficController::findLight(Car car)
{
	for (auto& light : trafficLights)
	{
		if (light.isHorizontal() == car.isHorizontal())
		{
			return light;
		}
	}
}

bool TrafficController::withinBounds(Car car)
{
	if ((car.getProgress() >= left && car.getProgress() <= right)
		|| (car.getProgress() >= top && car.getProgress() <= bottom))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool TrafficController::withinStoppingBounds(Car car)
{
	if ((car.getProgress() >= left - 0.02 && car.getProgress() <= right + 0.02)
		|| (car.getProgress() >= top - 0.02 && car.getProgress() <= bottom + 0.02))
	{
		return true;
	}
	else
	{
		return false;
	}
}


void TrafficController::progressCars(float amount)
{
	for (auto car = cars.begin(); car != cars.end(); ++car)
	{
		if (!withinStoppingBounds(*car) || findLight(*car).getState() == 2)
		{
			bool movable = true;
			for (auto carInFront = car + 1; carInFront != cars.end(); ++carInFront)
			{
				if (car->isHorizontal() == carInFront->isHorizontal()
					&& (carInFront->getProgress() - car->getProgress()) < 0.1)
				{
					movable = false;
				}
			}
			if (movable)
			{
				car->move(amount);
			}
		}

		if (car->getProgress() > 1)
		{
			car = cars.erase(car);

			if (cars.end() == car)
			{
				break;
			}
		}
	}
}

void TrafficController::draw(HWND hWnd, HDC hdc)
{
	for (auto& light : trafficLights)
	{
		light.draw(hdc);
	}
	for (auto& car : cars)
	{
		car.draw(hWnd, hdc);
	}
}