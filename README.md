# Otoscopio Educativo con ESP32-CAM (VOC)

![Otoscopio Educativo](https://i.imgur.com/placeholder-otoscopio.jpg)

## Visión General

El Otoscopio Educativo VOC (Visual Otoscopic Camera) es un dispositivo médico educativo que combina un otoscopio tradicional con tecnología de cámara digital basada en ESP32-CAM, permitiendo que tanto el evaluador como el docente visualicen simultáneamente el canal auditivo y tímpano del paciente durante las prácticas clínicas.

### Características Principales

- **Visualización Dual:** Sistema óptico con prisma y espejo 50/50 que permite la visualización simultánea directa y por cámara
- **Transmisión en Tiempo Real:** Streaming de video en vivo a través de WiFi con múltiples resoluciones (VGA, SVGA, XGA)
- **Captura de Imágenes:** Capacidad para tomar y almacenar fotografías de alta resolución
- **Múltiples Efectos Visuales:** Normal, negativo y escala de grises para mejorar la visualización diagnóstica
- **Interfaz Web Responsiva:** Accesible desde cualquier dispositivo con navegador web, sin necesidad de aplicaciones adicionales
- **Pantalla OLED Integrada:** Muestra información de estado, IP y conexión para facilitar el uso
- **Modo de Punto de Acceso y Cliente WiFi:** Configurable para conectarse a redes existentes o crear su propia red
- **Portal Cautivo:** Conexión WiFi directa sin necesidad de configuración adicional
- **Almacenamiento en SD:** Guarda imágenes para su revisión posterior o documentación clínica
- **Galería Integrada:** Visualización, descarga y gestión de imágenes capturadas
- **Panel de Administración Protegido:** Interfaz para gestionar configuraciones y actualizar el firmware con autenticación segura
- **Actualización OTA:** Posibilidad de actualizar el firmware inalámbricamente
- **Sistema de Respaldo:** Funcionalidad básica garantizada incluso sin tarjeta SD
- **Personalización Web:** Editor de archivos web integrado para personalizar la interfaz

## Principio de Funcionamiento

![Diagrama de Funcionamiento](https://i.imgur.com/placeholder-diagrama.jpg)

El sistema utiliza un prisma con espejo 50/50 que divide la luz proveniente del canal auditivo:
- 50% de la luz se dirige al ocular para la visualización directa por el evaluador.
- El otro 50% se dirige hacia la cámara ESP32-CAM, que transmite la imagen en tiempo real a través de WiFi.

Esta configuración óptica permite que tanto el estudiante (evaluador) como el profesor puedan observar simultáneamente la misma estructura anatómica, facilitando la enseñanza práctica de técnicas de exploración otoscópica.

## Componentes del Hardware

- **ESP32-CAM:** Módulo con microcontrolador ESP32 y cámara OV2640
- **Pantalla OLED:** Visualización de estado e información de conexión (0.89")
- **Prisma con Espejo 50/50:** Componente óptico que divide la luz
- **Carcasa de Otoscopio:** Adaptada para integrar el sistema óptico y electrónico
- **Tarjeta MicroSD:** Para almacenamiento de imágenes capturadas y archivos web
- **Batería LiPo:** Alimentación portátil del dispositivo
- **LED de Iluminación:** Proporciona luz para la exploración
- **Conos Desechables:** Compatibles con estándares médicos

## Arquitectura del Software

El sistema está desarrollado con una arquitectura modular:

```
ESP32-CAM Portal (VOC)
│
├── Módulo de Cámara
│   ├── Inicialización y configuración
│   ├── Captura de imágenes
│   ├── Streaming de video
│   └── Efectos visuales (normal, negativo, escala de grises)
│
├── Sistema de Archivos (SD)
│   ├── Almacenamiento de imágenes
│   ├── Gestión de archivos web
│   ├── Archivos de configuración
│   └── Sistema de respaldo sin SD
│
├── Pantalla OLED
│   ├── Información de estado
│   ├── Detalles de conexión
│   └── Mensajes del sistema
│
├── Conectividad WiFi
│   ├── Modo punto de acceso
│   ├── Modo cliente (conexión a redes existentes)
│   ├── Modo dual (AP+STA)
│   ├── Configuración WiFi persistente
│   ├── Escaneo de redes disponibles
│   └── Servidor DNS (portal cautivo)
│
└── Servidor Web
    ├── Interfaz de usuario
    │   ├── Visualización en tiempo real
    │   ├── Galería de imágenes
    │   ├── Controles de cámara y efectos
    │   └── Diseño responsivo para múltiples dispositivos
    │
    └── Área administrativa
        ├── Autenticación segura
        ├── Edición de archivos web
        ├── Configuración WiFi
        ├── Escaneo de redes disponibles
        └── Actualización OTA
```

## Instrucciones de Uso

### Paso 1: Encendido y Conexión

1. Encienda el dispositivo mediante el interruptor ubicado en la base.
2. La pantalla OLED mostrará el estado de inicialización y la información de conexión.
3. Por defecto, el dispositivo intentará conectarse a la red WiFi configurada previamente. Si no es posible, iniciará automáticamente en modo punto de acceso.
4. En su dispositivo (teléfono, tablet o computadora), busque y conéctese a la red WiFi:
   - **SSID:** VOC
   - **Contraseña:** No requiere (red abierta)
5. Se abrirá automáticamente el portal web o navegue a la dirección que se muestra en la pantalla OLED (por defecto: `http://192.168.4.1`)

### Paso 2: Interfaz de Usuario

![Interfaz de Usuario](https://i.imgur.com/placeholder-interfaz.jpg)

La interfaz presenta tres secciones principales:

- **Cámara:** Visualización en tiempo real con las siguientes opciones:
  - Tomar foto (almacenamiento automático en la SD)
  - Pausar/reanudar transmisión
  - Cambiar resolución (VGA, SVGA, XGA)
  - Aplicar efectos visuales (normal, negativo, escala de grises)
  - Indicador de estado en pantalla

- **Galería:** Acceso a imágenes capturadas con funciones de:
  - Previsualización
  - Descarga
  - Visualización ampliada
  - Exploración de imágenes previas

- **Admin:** Área para configuraciones avanzadas (requiere autenticación).

### Paso 3: Exploración Otoscópica

1. Coloque un cono desechable en el extremo del otoscopio.
2. Proceda con la técnica estándar de exploración otoscópica:
   - Tracción del pabellón auricular hacia arriba y atrás (adultos).
   - Tracción del pabellón auricular hacia abajo y atrás (niños).
3. El evaluador observa directamente a través del ocular.
4. El docente y demás estudiantes visualizan en tiempo real la misma imagen a través de sus dispositivos.
5. Capture imágenes según sea necesario con el botón "Tomar Foto".
6. Puede pausar la transmisión para analizar mejor una imagen específica.
7. Los efectos visuales pueden ayudar a destacar diferentes estructuras anatómicas.

### Área Administrativa

Para acceder al área administrativa:
- **Usuario:** admin
- **Contraseña:** admin123 (se recomienda cambiar en primera instalación)

El panel de administración se divide en tres secciones:

#### 1. Editor de Archivos Web

Permite personalizar la interfaz de usuario:
- Edición de archivos HTML (index.html, admin.html, fallback.html)
- Modificación de estilos CSS (styles.css)
- Personalización de funcionalidades JavaScript (script.js)
- Vista previa de cambios en tiempo real
- Guardado seguro con confirmación

#### 2. Actualización de Firmware

Proporciona actualización inalámbrica (OTA):
- Arrastre y suelte archivos .bin de firmware
- Barra de progreso con información detallada
- Verificación de integridad
- Reinicio automático tras actualización exitosa
- Sistema de respaldo en caso de fallos

#### 3. Configuración WiFi

Gestión completa de conectividad:
- Escaneo automático de redes disponibles
- Visualización de intensidad de señal
- Configuración de credenciales WiFi
- Opción de modo dual (AP+Cliente simultáneo)
- Estado actualizado de conexión
- IP estática o DHCP configurable
- Reinicio controlado tras cambios

## Especificaciones Técnicas

- **Resoluciones Disponibles:** 
  - VGA (640x480)
  - SVGA (800x600)
  - XGA (1024x768)
  - Hasta UXGA (1600x1200)
- **Efectos Visuales:** Normal, Negativo, Escala de grises
- **Pantalla OLED:** 0.89", 128x32 píxeles, I2C
- **Alcance WiFi:** ~30 metros (línea de vista)
- **Formato de Imagen:** JPEG (calidad configurable)
- **Autenticación Admin:** Basic Auth con credenciales configurables
- **Duración de Batería:** ~3 horas de uso continuo
- **Capacidad de Almacenamiento:** Dependiente de la tarjeta SD (máximo 32GB)
- **Puerto Web:** 80 (HTTP)
- **Puerto Streaming:** 81
- **Dimensiones:** 140mm x 30mm (largo x diámetro)
- **Peso:** 180g (incluida batería)

## Diagrama de Bloques del Sistema

```
┌──────────────────┐      ┌───────────────┐
│                  │      │               │
│  Prisma 50/50    │◄────►│  Canal        │
│                  │      │  Auditivo     │
└───────┬──────────┘      └───────────────┘
        │
        ├─────────────┐
        │             │
┌───────▼──────┐      │
│              │      │
│  ESP32-CAM   │◄────►│  Pantalla OLED
│              │      │
└───────┬──────┘      │
        │             │
┌───────▼──────┐      │
│              │      │
│  Servidor    │      │
│  WiFi        │      │
│              │      │
└───────┬──────┘      │
        │             │
        │       ┌─────▼─────┐
┌───────▼──────┐│           │
│              ││  Visor    │
│  Dispositivo ││  Directo  │
│  Cliente     ││           │
│              ││           │
└──────────────┘└───────────┘
```

## Estructura de Archivos del Proyecto

```
VOC/
├── VOC.ino                 # Programa principal
├── auth.cpp                # Implementación de autenticación
├── auth.h                  # Cabecera de autenticación
├── camera.cpp              # Funciones de la cámara
├── camera.h                # Cabecera de la cámara
├── config.h                # Configuración global
├── html_content.h          # Contenido HTML predeterminado
├── oled.cpp                # Funciones de la pantalla OLED
├── oled.h                  # Cabecera de la pantalla OLED
├── sd_card.cpp             # Gestión de tarjeta SD
├── sd_card.h               # Cabecera de tarjeta SD
├── web_server.cpp          # Implementación del servidor web
├── web_server.h            # Cabecera del servidor web
├── wifi.cpp                # Funciones WiFi
└── wifi.h                  # Cabecera WiFi
```

## Instalación y Configuración

### Requisitos

- Arduino IDE (1.8.x o superior)
- Paquete ESP32 para Arduino
- Bibliotecas necesarias:
  - ESP32 Camera
  - ArduinoJson
  - Adafruit_GFX
  - Adafruit_SSD1306
  - Update
  - SD_MMC

### Compilación e Instalación

1. Clone el repositorio o descargue los archivos fuente.
2. Abra el archivo `VOC.ino` en Arduino IDE.
3. Seleccione la placa "ESP32 Wrover Module" o "AI Thinker ESP32-CAM".
4. Verifique la configuración en `config.h` según sus necesidades.
5. Compile y cargue el firmware (requiere adaptador FTDI para la carga inicial).

### Preparación de la Tarjeta SD

Antes de usar el dispositivo, formatee una tarjeta microSD (FAT32) y cree las siguientes carpetas:
- `/photos` - Para almacenar imágenes capturadas
- `/web` - Para archivos de la interfaz web
- `/config` - Para archivos de configuración

## Personalización

### Modificar Credenciales

Para cambiar el usuario y contraseña de administración, edite en `config.h`:
```cpp
#define ADMIN_USER "su_nuevo_usuario"
#define ADMIN_PASSWORD "su_nueva_contraseña"
```

### Personalizar Interfaz Web

La interfaz web puede personalizarse de dos formas:
1. **Desde el área administrativa:** Utilizando el editor web integrado
2. **Modificando los archivos en la SD:** Editando directamente los archivos en la carpeta `/web`
3. **Cambiando el código fuente:** Modificando los valores predeterminados en `html_content.h`

### Configuración WiFi Avanzada

Para habilitar IP estática, edite la configuración WiFi desde el panel de administración o modifique directamente el archivo `/config/wifi_config.json` en la SD:

```json
{
  "ssid": "su_red_wifi",
  "password": "su_contraseña",
  "use_static_ip": true,
  "static_ip": "192.168.1.200",
  "gateway": "192.168.1.1",
  "subnet": "255.255.255.0",
  "dns1": "8.8.8.8",
  "dns2": "8.8.4.4"
}
```

## Resolución de Problemas

| Problema | Posible Causa | Solución |
|----------|--------------|----------|
| No enciende el dispositivo | Batería descargada | Recargue la batería |
| Pantalla OLED no muestra información | Error de inicialización I2C | Verifique conexiones y dirección I2C |
| No se conecta al WiFi | Interferencia o credenciales incorrectas | Acérquese al dispositivo o verifique la configuración |
| No crea punto de acceso | Error de inicialización WiFi | Reinicie el dispositivo |
| Imagen borrosa | Lente sucia o desenfocada | Limpie la lente o ajuste el enfoque |
| No captura fotos | Tarjeta SD llena o ausente | Verifique la tarjeta SD y espacio disponible |
| Error en la galería | Archivos de imagen corruptos | Formatee la tarjeta SD |
| Error al actualizar firmware | Conexión interrumpida o archivo incorrecto | Intente nuevamente con conexión estable y archivo válido |
| No se puede acceder al área admin | Credenciales incorrectas | Utilice el usuario y contraseña correctos |
| Error al guardar archivos web | Permisos de escritura en SD | Verifique que la SD no esté protegida contra escritura |
| La pantalla de streaming se congela | Ancho de banda WiFi insuficiente | Reduzca la resolución o acérquese al dispositivo |

## Características Futuras en Desarrollo

- **Grabación de video:** Almacenamiento de secuencias de video en la SD
- **Mediciones en pantalla:** Herramientas de medición visual para estructuras anatómicas
- **Conectividad Bluetooth:** Para control remoto desde aplicaciones específicas
- **Marcadores y anotaciones:** Posibilidad de realizar anotaciones sobre las imágenes
- **Múltiples perfiles de usuario:** Gestión de varios usuarios con diferentes permisos
- **Exportación de datos:** Generación de informes en formato PDF
- **Modo de bajo consumo:** Para extender la duración de la batería
- **Integración con sistemas LMS:** Para entornos educativos

## Contribuir al Proyecto

El Otoscopio Educativo VOC es un proyecto abierto. Contribuciones son bienvenidas:
- Reporte de errores a través de Issues
- Sugerencias de mejoras
- Pull requests con nuevas características
- Documentación adicional
- Traducciones a otros idiomas

## Licencia

Este proyecto está licenciado bajo la Licencia MIT - ver el archivo LICENSE para más detalles.

---

*Desarrollado para mejorar la educación médica en técnicas de exploración otoscópic