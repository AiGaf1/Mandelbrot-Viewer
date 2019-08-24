#pragma once
#include <SFML/Graphics.hpp>
#include <Windows.h>

using namespace sf;

bool ControlImage(RenderWindow& window, double&  minRe, double& maxRe, double& minIm, double& maxIm,
					int& maxIter, double& zoom, const int& width, const int& height);