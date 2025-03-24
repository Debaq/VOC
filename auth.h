// auth.h - Funciones de autenticación para el área administrativa

#ifndef AUTH_H
#define AUTH_H

#include "config.h"
#include "esp_http_server.h"

// Función simple para codificar en base64 (para autenticación)
String base64Encode(String input);

// Función auxiliar para verificar credenciales de admin
bool isAuthenticated(httpd_req_t *req);

#endif // AUTH_H