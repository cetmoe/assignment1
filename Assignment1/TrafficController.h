#pragma once
#include "TrafficLight.h"
#include "Car.h"
#include <vector>
#include <deque>

using namespace std;

class TrafficController
{
private:
	float left = 0;
	float top = 0;
	float right = 0;
	float bottom = 0;

	float pw = 0;
	float pn = 0;

	int cycleState = 0;
	int cycleProgress = 0;

	int cycleLength[4] = { 4,1,4,1 };

	deque<TrafficLight> trafficLights = {};
	deque<Car> cars = {};

public:
	TrafficController();

	TrafficController(float _left, float _top, float _right, float _bottom);

	void addLight(int state, int x, int y, bool horizontal);
	void addCar(bool horizontal);

	bool withinBounds(Car car);
	bool withinStoppingBounds(Car car);

	void cycle();

	void setCarFlow(float change, bool horizontal);
	float getProbability(bool horizontal);

	void progressLights();
	void progressCars(float amount);

	TrafficLight findLight(Car car);

	void draw(HWND hWnd, HDC hdc);
};

