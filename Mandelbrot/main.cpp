#include <stdio.h>
#include <thread>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "ControlImage.h"
#include "MakeText.h"

using namespace std;
using namespace sf;

const int W = 1920;
const int H = 1080;

int max_iter = 128;
double zoom = 1.0;
double min_re = -2.5, max_re = 1;
double min_im = -1, max_im = 1;


// computes v + t(u - v)
// t should be a value between 0 and 1
Color linear_interpolation(const Color& v, const Color& u, double a)
{
	const double b = 1 - a;
	return Color(b * v.r + a * u.r, b * v.g + a * u.g, b * v.b + a * u.b);
}


void SetImage(Image& image, int start_W, int end_W, int start_H, int end_H) {
	
	for (int y = start_H; y < end_H; y++) {
		for (int x = start_W; x < end_W; x++)
		{
			double cr = min_re + (max_re - min_re) * x / W;
			double ci = min_im + (max_im - min_im) * y / H;
			double re = 0, im = 0;
			int iter;
			for (iter = 0; iter < max_iter; iter++)
			{
				double tr = re * re - im * im + cr;
				im = 2 * re * im + ci;
				re = tr;
				if (re * re + im * im > 2 * 2) break;
			}
			int r = 1.0 * (max_iter - iter) / max_iter * 0xff;
			int g = r, b = r;

			/*
			//color pallet similar to Ultra Fractal and Wikipedia
			static const vector<Color> colors{
				{0,7,100},
				{32,107,203},
				{237,255,255},
				{255,170,0},
				{0,2,0},
			};
			*/
			static const vector<Color> colors{
				{0,0,0},
				{213,67,31},
				{251,255,121},
				{62,223,89},
				{43,30,218},
				{0,255,247}
			};

			static const auto max_color = colors.size() - 1;
			if (iter == max_iter)iter = 0;
			
			double mu = 1.0 * iter / max_iter;
			//scale mu to be in the range of colors
			mu *= max_color;
			auto i_mu = static_cast<size_t>(mu);
			auto color1 = colors[i_mu];
			auto color2 = colors[min(i_mu + 1, max_color)];
			Color c = linear_interpolation(color1, color2, mu - i_mu);


			image.setPixel(x, y, Color(c));
		}
	}
}


int main(void)
{
	RenderWindow window(VideoMode(W, H), "Mandelbrot");
	Image image; image.create(W, H);
	Texture texture;
	Sprite sprite;
	
	bool flag = 1;
	while (window.isOpen())
	{
		if (flag) {

			thread th1(SetImage, std::ref(image), 0, W / 2, 0, H / 2);
			thread th2(SetImage, std::ref(image), 0, W / 2, H / 2, H);
			thread th3(SetImage, std::ref(image), (W / 2), W, 0, (H / 2) + 1);
			thread th4(SetImage, std::ref(image), (W / 2), W, (H / 2) + 1, H);

			th1.join();
			th2.join();
			th3.join();
			th4.join();
			
			texture.loadFromImage(image);
			sprite.setTexture(texture);
			window.draw(sprite);
			PrintText(window, max_iter, zoom);

			window.display();
		}

		flag = ControlImage(window, min_re, max_re, min_im, max_im, max_iter, zoom, W, H);
	}
	return 0;
}