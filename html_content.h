// html_content.h - Contenido HTML, CSS y JavaScript predeterminado

#ifndef HTML_CONTENT_H
#define HTML_CONTENT_H

// HTML principal por defecto
const char* DEFAULT_INDEX_HTML = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32-CAM Portal</title>
  <link rel="stylesheet" href="/styles.css">
</head>
<body>
  <div class="container">
    <h1>ESP32-CAM Portal</h1>
    
    <div class="tabs">
      <div class="tab active" onclick="openTab('camera-tab')">Cámara</div>
      <div class="tab" onclick="openTab('gallery-tab')">Galería</div>
      <div class="tab admin-tab" onclick="location.href='/admin'">Admin</div>
    </div>
    
    <div id="camera-tab" class="content active">
      <img id="stream" src="/stream">
      
      <div class="controls">
        <div class="control-group">
          <button onclick="capturePhoto()">Tomar Foto</button>
          <button onclick="toggleStream(this)">Pausar</button>
        </div>
        
        <div class="control-group">
          <button onclick="changeResolution('VGA')">Resolución: VGA</button>
          <button onclick="changeResolution('SVGA')">Resolución: SVGA</button>
          <button onclick="changeResolution('XGA')">Resolución: XGA</button>
        </div>
        
        <div class="control-group">
          <button onclick="toggleEffect('normal')">Normal</button>
          <button onclick="toggleEffect('negative')">Negativo</button>
          <button onclick="toggleEffect('grayscale')">Escala de grises</button>
        </div>
      </div>
      
      <div id="status" class="status"></div>
    </div>
    
    <div id="gallery-tab" class="content">
      <button onclick="loadGallery()" class="refresh-btn">Actualizar Galería</button>
      <div id="gallery" class="gallery">
        <!-- Las imágenes se cargarán aquí -->
      </div>
    </div>
  </div>
  
  <!-- Modal para visualizar la imagen completa -->
  <div id="imageModal" class="modal">
    <span class="close" onclick="closeModal()">&times;</span>
    <img class="modal-content" id="modalImg">
    <div class="modal-buttons">
      <button onclick="downloadImage()">Descargar</button>
    </div>
  </div>
  
  <script src="/script.js"></script>
</body>
</html>
)rawliteral";

// HTML de administración por defecto
const char* DEFAULT_ADMIN_HTML = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32-CAM Admin</title>
  <link rel="stylesheet" href="/styles.css">
  <style>
    .admin-container {
      max-width: 800px;
      margin: 0 auto;
      padding: 20px;
    }
    .file-editor {
      margin-bottom: 20px;
    }
    .file-editor textarea {
      width: 100%;
      height: 300px;
      font-family: monospace;
      margin-bottom: 10px;
      padding: 10px;
      border: 1px solid #ccc;
    }
    .admin-section {
      background-color: #f5f5f5;
      border-radius: 5px;
      padding: 20px;
      margin-bottom: 20px;
    }
    .admin-section h2 {
      margin-top: 0;
      color: #333;
    }
    .file-list {
      border: 1px solid #ddd;
      border-radius: 5px;
      padding: 10px;
      background-color: white;
      max-height: 200px;
      overflow-y: auto;
    }
    .file-item {
      padding: 5px;
      border-bottom: 1px solid #eee;
      cursor: pointer;
    }
    .file-item:hover {
      background-color: #f0f0f0;
    }
    .firmware-upload {
      border: 2px dashed #3498db;
      border-radius: 5px;
      padding: 20px;
      text-align: center;
      margin-bottom: 20px;
    }
    .progress-bar {
      height: 20px;
      background-color: #eee;
      border-radius: 5px;
      margin-top: 10px;
      overflow: hidden;
      display: none;
    }
    .progress {
      height: 100%;
      background-color: #4CAF50;
      width: 0%;
      transition: width 0.3s;
    }
  </style>
</head>
<body>
  <div class="admin-container">
    <h1>Área Administrativa</h1>
    <a href="/" class="back-btn">← Volver a la Cámara</a>
    
    <div class="admin-section">
      <h2>Editor de Archivos Web</h2>
      <p>Selecciona un archivo para editar:</p>
      <div class="file-list" id="fileList">
        <div class="file-item" onclick="loadFile('/web/index.html')">index.html</div>
        <div class="file-item" onclick="loadFile('/web/admin.html')">admin.html</div>
        <div class="file-item" onclick="loadFile('/web/styles.css')">styles.css</div>
        <div class="file-item" onclick="loadFile('/web/script.js')">script.js</div>
        <div class="file-item" onclick="loadFile('/web/fallback.html')">fallback.html</div>
      </div>
      
      <div class="file-editor">
        <h3 id="currentFile">No hay archivo seleccionado</h3>
        <textarea id="fileContent" placeholder="El contenido del archivo aparecerá aquí"></textarea>
        <button onclick="saveFile()" class="save-btn">Guardar Cambios</button>
      </div>
    </div>
    
    <div class="admin-section">
      <h2>Actualización OTA</h2>
      <div class="firmware-upload">
        <p>Selecciona el archivo .bin con el nuevo firmware:</p>
        <input type="file" id="firmwareFile" accept=".bin">
        <button onclick="uploadFirmware()" class="upload-btn">Subir Firmware</button>
        <div class="progress-bar" id="progressBar">
          <div class="progress" id="progress"></div>
        </div>
      </div>
      <div id="otaStatus"></div>
    </div>
  </div>
  
  <script>
    let currentFilePath = '';
    
    // Cargar contenido de un archivo
    function loadFile(path) {
      currentFilePath = path;
      document.getElementById('currentFile').textContent = 'Archivo: ' + path;
      document.getElementById('fileContent').value = 'Cargando...';
      
      fetch(path)
        .then(response => {
          if (!response.ok) {
            throw new Error('Error al cargar el archivo');
          }
          return response.text();
        })
        .then(content => {
          document.getElementById('fileContent').value = content;
        })
        .catch(error => {
          document.getElementById('fileContent').value = 'Error: ' + error.message;
        });
    }
    
    // Guardar cambios en un archivo
    function saveFile() {
      if (!currentFilePath) {
        alert('No hay archivo seleccionado');
        return;
      }
      
      const content = document.getElementById('fileContent').value;
      const formData = new FormData();
      const blob = new Blob([content], { type: 'text/plain' });
      
      fetch('/update-web?file=' + encodeURIComponent(currentFilePath), {
        method: 'POST',
        body: content,
        headers: {
          'Content-Type': 'text/plain',
          'Authorization': 'Basic ' + btoa('admin:admin123') // Usar las credenciales configuradas
        }
      })
      .then(response => {
        if (!response.ok) {
          throw new Error('Error al guardar el archivo');
        }
        return response.json();
      })
      .then(data => {
        if (data.success) {
          alert('Archivo guardado correctamente');
        } else {
          alert('Error al guardar: ' + data.message);
        }
      })
      .catch(error => {
        alert('Error: ' + error.message);
      });
    }
    
    // Subir nuevo firmware
    function uploadFirmware() {
      const fileInput = document.getElementById('firmwareFile');
      if (!fileInput.files.length) {
        alert('Selecciona un archivo de firmware');
        return;
      }
      
      const file = fileInput.files[0];
      if (!file.name.endsWith('.bin')) {
        alert('El archivo debe tener extensión .bin');
        return;
      }
      
      // Mostrar barra de progreso
      const progressBar = document.getElementById('progressBar');
      const progress = document.getElementById('progress');
      progressBar.style.display = 'block';
      progress.style.width = '0%';
      
      const xhr = new XMLHttpRequest();
      xhr.open('POST', '/update-ota');
      xhr.setRequestHeader('Authorization', 'Basic ' + btoa('admin:admin123'));
      
      xhr.upload.onprogress = (e) => {
        if (e.lengthComputable) {
          const percentage = (e.loaded / e.total) * 100;
          progress.style.width = percentage + '%';
        }
      };
      
      xhr.onreadystatechange = function() {
        if (xhr.readyState === 4) {
          if (xhr.status === 200) {
            document.getElementById('otaStatus').textContent = 'Firmware actualizado correctamente. El dispositivo se reiniciará...';
            setTimeout(() => {
              alert('El dispositivo se está reiniciando. La página se recargará en 15 segundos.');
              setTimeout(() => {
                window.location.href = '/';
              }, 15000);
            }, 2000);
          } else {
            document.getElementById('otaStatus').textContent = 'Error al actualizar el firmware: ' + xhr.statusText;
          }
        }
      };
      
      xhr.send(file);
    }
  </script>
</body>
</html>
)rawliteral";

// CSS por defecto
const char* DEFAULT_CSS = R"rawliteral(
body {
  font-family: Arial, sans-serif;
  margin: 0;
  padding: 0;
  background-color: #f4f4f4;
  color: #333;
}
.container {
  max-width: 800px;
  margin: 0 auto;
  padding: 20px;
}
h1 {
  color: #2c3e50;
  text-align: center;
}
.tabs {
  display: flex;
  margin-bottom: 20px;
  border-bottom: 1px solid #ddd;
}
.tab {
  padding: 10px 20px;
  cursor: pointer;
  background-color: #eee;
  margin-right: 5px;
  border-radius: 5px 5px 0 0;
}
.tab.active {
  background-color: #3498db;
  color: white;
}
.admin-tab {
  margin-left: auto;
  background-color: #f39c12;
  color: white;
}
.content {
  display: none;
}
.content.active {
  display: block;
}
#stream {
  width: 100%;
  max-width: 640px;
  height: auto;
  display: block;
  margin: 0 auto;
  border: 1px solid #ddd;
}
.controls {
  margin-top: 20px;
  display: flex;
  justify-content: center;
  flex-wrap: wrap;
}
.control-group {
  margin: 10px;
}
button {
  background-color: #3498db;
  color: white;
  border: none;
  padding: 10px 15px;
  border-radius: 5px;
  cursor: pointer;
  margin: 5px;
}
button:hover {
  background-color: #2980b9;
}
.refresh-btn {
  display: block;
  margin: 0 auto 20px;
}
.gallery {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(150px, 1fr));
  gap: 10px;
  margin-top: 20px;
}
.gallery-item {
  cursor: pointer;
  border: 1px solid #ddd;
  border-radius: 5px;
  overflow: hidden;
}
.gallery-img {
  width: 100%;
  height: 100px;
  object-fit: cover;
}
.modal {
  display: none;
  position: fixed;
  z-index: 1000;
  left: 0;
  top: 0;
  width: 100%;
  height: 100%;
  background-color: rgba(0,0,0,0.9);
}
.modal-content {
  display: block;
  margin: 10% auto;
  max-width: 80%;
  max-height: 80%;
}
.close {
  color: white;
  position: absolute;
  top: 20px;
  right: 30px;
  font-size: 30px;
  cursor: pointer;
}
.modal-buttons {
  text-align: center;
  margin-top: 20px;
}
.modal-buttons button {
  background-color: #27ae60;
}
.modal-buttons button:hover {
  background-color: #2ecc71;
}
.status {
  margin-top: 20px;
  padding: 10px;
  text-align: center;
  color: #333;
}
.back-btn {
  display: inline-block;
  margin-bottom: 20px;
  color: #3498db;
  text-decoration: none;
}
.save-btn {
  background-color: #2ecc71;
}
.upload-btn {
  background-color: #e74c3c;
  margin-top: 10px;
}
@media (max-width: 600px) {
  .gallery {
    grid-template-columns: repeat(auto-fill, minmax(100px, 1fr));
  }
  .controls {
    flex-direction: column;
    align-items: center;
  }
}
)rawliteral";

// JavaScript por defecto
const char* DEFAULT_JS = R"rawliteral(
// Variables globales
let streamPaused = false;
let currentImagePath = '';

// Función para cambiar entre pestañas
function openTab(tabId) {
  const tabs = document.querySelectorAll('.tab');
  const contents = document.querySelectorAll('.content');
  
  tabs.forEach(tab => {
    if (tab.classList.contains('admin-tab')) return;
    tab.classList.remove('active');
  });
  
  contents.forEach(content => content.classList.remove('active'));
  
  document.querySelector(`[onclick="openTab('${tabId}')"]`).classList.add('active');
  document.getElementById(tabId).classList.add('active');
  
  if (tabId === 'gallery-tab') {
    loadGallery();
  }
}

// Función para pausar/reanudar el stream
function toggleStream(button) {
  const stream = document.getElementById('stream');
  
  if (streamPaused) {
    stream.src = "/stream";
    button.innerText = "Pausar";
  } else {
    stream.src = "";
    button.innerText = "Reanudar";
  }
  
  streamPaused = !streamPaused;
}

// Función para tomar una foto
function capturePhoto() {
  fetch('/capture')
    .then(response => response.text())
    .then(data => {
      document.getElementById('status').innerText = "Foto capturada: " + data;
      // Actualizar la galería si estamos en esa pestaña
      if (document.getElementById('gallery-tab').classList.contains('active')) {
        loadGallery();
      }
    })
    .catch(error => {
      document.getElementById('status').innerText = "Error al capturar foto: " + error;
    });
}

// Función para cambiar la resolución
function changeResolution(resolution) {
  fetch('/control?var=resolution&val=' + resolution)
    .then(response => response.text())
    .then(data => {
      document.getElementById('status').innerText = "Resolución cambiada a: " + resolution;
    })
    .catch(error => {
      document.getElementById('status').innerText = "Error al cambiar resolución: " + error;
    });
}

// Función para cambiar efectos
function toggleEffect(effect) {
  const effectMap = {
    'normal': 0,
    'negative': 1,
    'grayscale': 2
  };
  
  fetch('/control?var=special_effect&val=' + effectMap[effect])
    .then(response => response.text())
    .then(data => {
      document.getElementById('status').innerText = "Efecto cambiado a: " + effect;
    })
    .catch(error => {
      document.getElementById('status').innerText = "Error al cambiar efecto: " + error;
    });
}

// Función para cargar la galería de imágenes
function loadGallery() {
  document.getElementById('gallery').innerHTML = "Cargando...";
  
  fetch('/list')
    .then(response => response.json())
    .then(data => {
      const galleryDiv = document.getElementById('gallery');
      galleryDiv.innerHTML = '';
      
      if (data.length === 0) {
        galleryDiv.innerHTML = "No hay fotos guardadas.";
        return;
      }
      
      data.forEach(photo => {
        const item = document.createElement('div');
        item.className = 'gallery-item';
        
        const img = document.createElement('img');
        img.className = 'gallery-img';
        img.src = photo.thumb;
        img.alt = photo.name;
        img.onclick = function() {
          openModal(photo.path);
        };
        
        item.appendChild(img);
        galleryDiv.appendChild(item);
      });
    })
    .catch(error => {
      document.getElementById('gallery').innerHTML = "Error al cargar las imágenes: " + error;
    });
}

// Funciones para el modal
function openModal(imagePath) {
  const modal = document.getElementById('imageModal');
  const modalImg = document.getElementById('modalImg');
  
  currentImagePath = imagePath;
  modal.style.display = "block";
  modalImg.src = imagePath;
}

function closeModal() {
  document.getElementById('imageModal').style.display = "none";
}

function downloadImage() {
  if (!currentImagePath) return;
  
  const link = document.createElement('a');
  link.href = currentImagePath;
  link.download = currentImagePath.split('/').pop();
  document.body.appendChild(link);
  link.click();
  document.body.removeChild(link);
}

// Cerrar modal al hacer clic fuera de la imagen
window.onclick = function(event) {
  const modal = document.getElementById('imageModal');
  if (event.target === modal) {
    closeModal();
  }
};

// Inicializar al cargar la página
document.addEventListener('DOMContentLoaded', function() {
  // Si estamos en la página principal, cargar la galería
  if (document.getElementById('gallery-tab')) {
    // Ya estamos en la página principal
  }
});
)rawliteral";

// HTML de respaldo por defecto
const char* DEFAULT_FALLBACK_HTML = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32-CAM Portal</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 0;
      padding: 20px;
      text-align: center;
    }
    h1 {
      color: #2c3e50;
    }
    #stream {
      width: 100%;
      max-width: 640px;
      margin: 20px auto;
      border: 1px solid #ddd;
    }
    button {
      background-color: #3498db;
      color: white;
      border: none;
      padding: 10px 15px;
      border-radius: 5px;
      cursor: pointer;
      margin: 5px;
    }
  </style>
</head>
<body>
  <h1>ESP32-CAM Portal</h1>
  <p>Modo de respaldo: La tarjeta SD no está disponible o los archivos web no se encontraron.</p>
  <img id="stream" src="/stream">
  <div>
    <button onclick="location.href='/capture'">Tomar Foto</button>
  </div>
</body>
</html>
)rawliteral";

#endif // HTML_CONTENT_H