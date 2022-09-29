/***************************************************
Copyright (c) 2021 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses
****************************************************/

#pragma GCC optimize ("-Ofast")

#include <M5StickCPlus.h>
#include <ESP_Color.h>
#include <Image_Scaling.h>

const uint8_t origSize = 64;
const uint8_t destSize = 16;

TFT_eSprite Original = TFT_eSprite(&M5.Lcd);
TFT_eSprite Scaled = TFT_eSprite(&M5.Lcd);
TFT_eSprite Final = TFT_eSprite(&M5.Lcd);

void setup() {
	Serial.begin(115200);
	M5.begin();
	M5.Lcd.setRotation(0);

	Original.setColorDepth(16);
	Original.createSprite(origSize, origSize);
	Original.fillScreen(TFT_BLACK);

	auto color = TFT_CYAN;
	const int grid = 6;
	for (auto x = 0; x < grid; x++)
	{
		for (auto y = 0; y < grid; y++)
		{
			Original.fillRect(x * origSize / grid, y * origSize / grid, origSize / grid, origSize / grid, color);
			color = color == TFT_RED ? TFT_CYAN : TFT_RED;
		}
		color = color == TFT_RED ? TFT_CYAN : TFT_RED;
	}

	Scaled.setColorDepth(16);
	Scaled.createSprite(destSize, destSize);
	Scaled.fillScreen(TFT_BLACK);

	Final.setColorDepth(16);
	Final.createSprite(origSize, origSize);
	Final.fillScreen(TFT_BLACK);
}

void Render()
{
	ImageScaling::Average(Original, Scaled);
	ImageScaling::Bilineal(Scaled, Final);

	Original.pushSprite(0, 0);
	Scaled.pushSprite(30, 64 + 10);
	Final.pushSprite(0, 160 - 64);
}

void loop()
{
	M5.update();

	Render();
	delay(500);
}