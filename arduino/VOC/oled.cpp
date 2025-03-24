// oled.cpp - Implementación de funciones para la pantalla OLED

#include "oled.h"

// Definir el objeto para la pantalla OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Inicializar la pantalla OLED
bool initOLED() {
  // Inicializar I2C
  Wire.begin(OLED_SDA, OLED_SCL);
  
  // Inicializar pantalla OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println(F("Error al inicializar la pantalla OLED"));
    return false;
  }
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println(F("Iniciando..."));
  display.display();
  
  return true;
}

// Mostrar texto en la pantalla OLED
void displayText(const String& text, int size, bool clear) {
  if (clear) display.clearDisplay();
  
  display.setTextSize(size);
  display.setCursor(0, 0);
  display.println(text);
  display.display();
}

// Mostrar IP en la pantalla OLED
void displayIP(const IPAddress& ip, const String& ssid) {
  display.clearDisplay();
  display.setTextSize(1);
  
  // Mostrar título
  display.setCursor(0, 0);
  display.println(F("Conectado a WiFi:"));
  
  // Mostrar SSID
  display.setCursor(0, 10);
  display.println(ssid);
  
  // Mostrar IP
  display.setCursor(0, 25);
  display.println(F("IP:"));
  display.setCursor(20, 25);
  display.println(ip.toString());
  
  display.display();
}

// Mostrar mensaje de modo AP en la pantalla OLED
void displayAPMode(const IPAddress& ip, const String& ssid) {
  display.clearDisplay();
  display.setTextSize(1);
  
  // Mostrar título
  display.setCursor(0, 0);
  display.println(F("Modo AP activado:"));
  
  // Mostrar SSID
  display.setCursor(0, 10);
  display.println(ssid);
  
  // Mostrar IP
  display.setCursor(0, 25);
  display.println(F("IP:"));
  display.setCursor(20, 25);
  display.println(ip.toString());
  
  display.display();
}