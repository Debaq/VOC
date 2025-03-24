// oled.h - Funciones para manejar la pantalla OLED de 0.89"

#ifndef OLED_H
#define OLED_H

#include "config.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Inicializar la pantalla OLED
bool initOLED();

// Mostrar texto en la pantalla OLED
void displayText(const String& text, int size = 1, bool clear = true);

// Mostrar IP en la pantalla OLED
void displayIP(const IPAddress& ip, const String& ssid = "");

// Mostrar mensaje de modo AP en la pantalla OLED
void displayAPMode(const IPAddress& ip, const String& ssid);

#endif // OLED_H