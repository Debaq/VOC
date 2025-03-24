// VOC.ino - Programa principal para ESP32-CAM Portal

#include "config.h"
#include "camera.h"
#include "sd_card.h"
#include "wifi.h"
#include "web_server.h"
#include "oled.h"

// Definición de variables globales (declaradas como extern en config.h)
bool hasSD = false;
size_t photoCount = 0;
String lastPhotoFilename = "";
uint8_t tempBuffer[BUFFER_SIZE];
// isAPMode se define en wifi.cpp

// Definir las variables que faltan y causan errores
httpd_handle_t camera_httpd = NULL;
httpd_handle_t stream_httpd = NULL;
DNSServer dnsServer;

void setup() {
  // Iniciar puerto serial
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  
  // Inicializar cámara
  if (!initCamera()) {
    Serial.println("Fallo al inicializar la cámara, reiniciando...");
    delay(1000);
    ESP.restart();
  }
  
  // Inicializar pantalla OLED
  if (!initOLED()) {
    Serial.println("Fallo al inicializar la pantalla OLED");
  } else {
    displayText("ESP32-CAM\nIniciando...", 1, true);
  }
  
  // Inicializar tarjeta SD
  hasSD = initSD();
  if (!hasSD) {
    Serial.println("Advertencia: Sistema funcionará sin almacenamiento en SD");
    displayText("No SD Card\nDetected!", 1, true);
    delay(1000);
  }
  
  // Inicializar WiFi (intentará conectarse a la red guardada primero)
  if (!initWiFi()) {
    Serial.println("Funcionando en modo AP");
  }
  
  // Inicializar servidores web
  startCameraServer();
  
  Serial.println("Sistema listo");
}

void loop() {
  // Manejar peticiones DNS para el portal cautivo (solo en modo AP)
  if (isAPMode) {
    dnsServer.processNextRequest();
  }
  
  // No es necesario más código aquí, los servidores HTTP funcionan en segundo plano
  delay(10);
}