// camera.h - Funciones de configuración y manejo de la cámara ESP32-CAM

#ifndef CAMERA_H
#define CAMERA_H

#include "config.h"
#include "esp_camera.h"
#include "img_converters.h"
#include "esp_timer.h"

// Inicialización de la cámara
bool initCamera();

// Capturar una foto y guardarla en la SD
String capturePhotoSaveSD();

// Callback para enviar las imágenes JPEG por chunks
size_t jpg_encode_stream(void * arg, size_t index, const void* data, size_t len);

#endif // CAMERA_H