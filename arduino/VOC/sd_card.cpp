// sd_card.cpp - Implementación de funciones para manejo de tarjeta SD y archivos

#include "sd_card.h"
#include "html_content.h"

// Variables globales definidas en config.h
extern uint8_t tempBuffer[BUFFER_SIZE];
extern bool hasSD;
extern size_t photoCount;

// Inicialización de la tarjeta SD
bool initSD() {
  if(!SD_MMC.begin()) {
    Serial.println("Fallo al montar la tarjeta SD");
    return false;
  }
  
  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE) {
    Serial.println("No hay tarjeta SD insertada");
    return false;
  }
  
  // Crear directorios necesarios si no existen
  if(!SD_MMC.exists(PHOTOS_DIR)) {
    SD_MMC.mkdir(PHOTOS_DIR);
  }
  
  if(!SD_MMC.exists(WEB_DIR)) {
    SD_MMC.mkdir(WEB_DIR);
  }
  
  // Crear archivos web predeterminados si no existen
  initWebFiles();
  
  // Contar fotos existentes para continuar la numeración
  File root = SD_MMC.open(PHOTOS_DIR);
  while (true) {
    File entry = root.openNextFile();
    if (!entry) break;
    if (!entry.isDirectory() && String(entry.name()).endsWith(".jpg")) {
      photoCount++;
    }
    entry.close();
  }
  root.close();
  
  Serial.println("Tarjeta SD inicializada correctamente");
  Serial.print("Fotos existentes: ");
  Serial.println(photoCount);
  return true;
}

// Función para crear archivos web predeterminados si no existen
void initWebFiles() {
  // Crear archivos HTML, CSS y JS básicos si no existen
  if (!fileExists(HTML_INDEX)) {
    createDefaultIndexHtml();
  }
  
  if (!fileExists(HTML_ADMIN)) {
    createDefaultAdminHtml();
  }
  
  if (!fileExists(CSS_MAIN)) {
    createDefaultCss();
  }
  
  if (!fileExists(JS_MAIN)) {
    createDefaultJs();
  }
  
  if (!fileExists(HTML_FALLBACK)) {
    createDefaultFallbackHtml();
  }
}

// Función para verificar si un archivo existe en la SD
bool fileExists(const char* path) {
  return SD_MMC.exists(path);
}

// Función para leer un archivo completo de la SD
String readFile(const char* path) {
  if (!fileExists(path)) {
    Serial.printf("Archivo no encontrado: %s\n", path);
    return "";
  }
  
  File file = SD_MMC.open(path, FILE_READ);
  if (!file) {
    Serial.printf("Error al abrir archivo: %s\n", path);
    return "";
  }
  
  String content = "";
  while (file.available()) {
    content += (char)file.read();
  }
  file.close();
  
  return content;
}

// Función para escribir un archivo en la SD
bool writeFile(const char* path, const char* content) {
  File file = SD_MMC.open(path, FILE_WRITE);
  if (!file) {
    Serial.printf("Error al abrir archivo para escritura: %s\n", path);
    return false;
  }
  
  size_t written = file.print(content);
  file.close();
  
  return (written == strlen(content));
}

// Función para crear la página HTML principal por defecto
void createDefaultIndexHtml() {
  writeFile(HTML_INDEX, DEFAULT_INDEX_HTML);
}

// Función para crear la página HTML de administración por defecto
void createDefaultAdminHtml() {
  writeFile(HTML_ADMIN, DEFAULT_ADMIN_HTML);
}

// Función para crear el archivo CSS por defecto
void createDefaultCss() {
  writeFile(CSS_MAIN, DEFAULT_CSS);
}

// Función para crear el archivo JavaScript por defecto
void createDefaultJs() {
  writeFile(JS_MAIN, DEFAULT_JS);
}

// Función para crear una página HTML básica de respaldo
void createDefaultFallbackHtml() {
  writeFile(HTML_FALLBACK, DEFAULT_FALLBACK_HTML);
}