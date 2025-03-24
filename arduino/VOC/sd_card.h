// sd_card.h - Funciones para manejo de tarjeta SD y archivos

#ifndef SD_CARD_H
#define SD_CARD_H

#include "config.h"
#include "FS.h"
#include "SD_MMC.h"

// Inicialización de la tarjeta SD
bool initSD();

// Función para verificar si un archivo existe en la SD
bool fileExists(const char* path);

// Función para leer un archivo completo de la SD
String readFile(const char* path);

// Función para escribir un archivo en la SD
bool writeFile(const char* path, const char* content);

// Función para crear archivos web predeterminados si no existen
void initWebFiles();

// Declaraciones de funciones para crear archivos por defecto
void createDefaultIndexHtml();
void createDefaultAdminHtml();
void createDefaultCss();
void createDefaultJs();
void createDefaultFallbackHtml();

#endif // SD_CARD_H