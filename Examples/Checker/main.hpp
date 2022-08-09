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

#define LGFX_AUTODETECT
#include <LovyanGFX.hpp>

#include <ESP_Color.h>
#include <Image_Scaling.h>

const uint8_t origSize = 64;
const uint8_t destSize = 16;

TFT_eSprite sOriginal = TFT_eSprite(&M5.Lcd);
TFT_eSprite sScaled = TFT_eSprite(&M5.Lcd);
TFT_eSprite sFinal = TFT_eSprite(&M5.Lcd);

UGFX::UnifiedSprite_TFTeSPI Original(&sOriginal);
UGFX::UnifiedSprite_TFTeSPI Scaled(&sScaled);
UGFX::UnifiedSprite_TFTeSPI Final(&sFinal);

void setup() {
	Serial.begin(115200);
	M5.begin();
	M5.Lcd.setRotation(0);

	sOriginal.setColorDepth(16);
	sOriginal.createSprite(origSize, origSize);
	sOriginal.fillScreen(TFT_BLACK);

	auto color = TFT_CYAN;
	const int grid = 6;
	for (auto x = 0; x < grid; x++)
	{
		for (auto y = 0; y < grid; y++)
		{
			sOriginal.fillRect(x * origSize / grid, y * origSize / grid, origSize / grid, origSize / grid, color);
			color = color == TFT_RED ? TFT_CYAN : TFT_RED;
		}
		color = color == TFT_RED ? TFT_CYAN : TFT_RED;
	}

	sScaled.setColorDepth(16);
	sScaled.createSprite(destSize, destSize);
	sScaled.fillScreen(TFT_BLACK);

	sFinal.setColorDepth(16);
	sFinal.createSprite(origSize, origSize);
	sFinal.fillScreen(TFT_BLACK);
}

void Render()
{
	ImageScaling::Average(Original, Scaled);
	ImageScaling::Bilineal(Scaled, Final);

	sOriginal.pushSprite(0, 0);
	sScaled.pushSprite(30, 64 + 10);
	sFinal.pushSprite(0, 160 - 64);
}

void loop()
{
	M5.update();

	Render();
	delay(500);
}