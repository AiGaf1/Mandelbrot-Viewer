#include "ControlImage.h"

bool ControlImage(RenderWindow& window, double& minRe, double& maxRe, double& minIm,
	double& maxIm, int& maxIter, double& zoom, const int& width, const int& height)
{
	double prevMinRe = minRe, prevMaxRe = maxRe, prevMinIm = minIm, prevMaxIm = maxIm, prevZoom = zoom;
	int prevMaxIter = maxIter;
	Event e;

	while (window.pollEvent(e))
	{
		if (e.type == Event::Closed) window.close();
		if (e.type == Event::KeyPressed)
		{
			//move delta
			double w = (maxRe - minRe) * 0.3;
			double h = (maxIm - minIm) * 0.3;

			if (e.key.code == Keyboard::Left) { minRe -= w, maxRe -= w; }
			if (e.key.code == Keyboard::Right) { minRe += w, maxRe += w; }
			if (e.key.code == Keyboard::Up) { minIm -= h, maxIm -= h; }
			if (e.key.code == Keyboard::Down) { minIm += h, maxIm += h; }

		}
		//Set Iteration level by mouse wheel
		//the more iteration level the better image result
		if (e.type == Event::MouseWheelScrolled)
		{
			if (e.MouseWheelScrolled)
			{
				if (e.mouseWheelScroll.wheel == Mouse::VerticalWheel)
				{
					if (e.mouseWheelScroll.delta > 0)maxIter *= 2;
					else maxIter /= 2;
					if (maxIter < 1)maxIter = 1;
				}
			}
		}
		if (e.type == Event::MouseButtonPressed)
		{
			auto zoom_x = [&](double z)
			{
				//mouse point will be new center point
				double cr = minRe + (maxRe - minRe) * e.mouseButton.x / width;
				double ci = minIm + (maxIm - minIm) * e.mouseButton.y / height;

				//zoom
				double tminr = cr - (maxRe - minRe) / 2 / z;
				maxRe = cr + (maxRe - minRe) / 2 / z;
				minRe = tminr;

				double tmini = ci - (maxIm - minIm) / 2 / z;
				maxIm = ci + (maxIm - minIm) / 2 / z;
				minIm = tmini;
			};
			//Left Click to ZoomIn
			if (e.mouseButton.button == Mouse::Left)
			{
				zoom_x(5);
				zoom *= 5;
			}
			//Right Click to ZoomOut
			if (e.mouseButton.button == Mouse::Right)
			{
				zoom_x(1.0 / 5);
				zoom /= 5;
			}
		}
	}


	if (minRe != prevMinRe || maxRe != prevMaxRe || minIm != prevMinIm ||
		maxIm != prevMaxIm || zoom != prevZoom || maxIter != prevMaxIter) {
		prevMinRe = minRe;
		prevMaxRe = maxRe;
		prevMinIm = minIm;
		prevMaxIm = maxIm;
		prevZoom = zoom;
		prevMaxIter = maxIter;
		return 1;
	}

	
	return 0;
}
