#include "MakeText.h"

void MakeText(RenderWindow& window, const int& max_iter, const double& zoom)
{
	Text text;
	Font font; font.loadFromFile("Font/arial.ttf");
	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(Color::White);

	char str[100];
	sprintf_s(str, "max iter:%d\nzoom:x%2.2lf", max_iter, zoom);
	text.setString(str);
	window.draw(text);

}
