/**
 * VOC - Visual Otoscopic Camera
 * Script principal para funcionalidades del portal web
 */

// Variables globales
let streamPaused = false;
let currentImagePath = '';
let originalFileContent = '';
let confirmCallback = null;
let wifiNetworks = [];

// ==========================
// FUNCIONES COMUNES
// ==========================

// Mostrar notificación tipo toast
function showToast(message, type = '') {
  const toast = document.getElementById('toast');
  toast.textContent = message;
  toast.className = 'toast show';
  
  if (type) {
    toast.classList.add(type);
  }
  
  setTimeout(() => {
    toast.classList.remove('show');
    setTimeout(() => {
      toast.className = 'toast';
    }, 300);
  }, 3000);
}

// Función para cambiar entre pestañas
function openTab(tabId) {
  // Ocultar todas las pestañas y contenido
  const tabs = document.querySelectorAll('.tab:not(.admin-tab)');
  const contents = document.querySelectorAll('.content');
  
  tabs.forEach(tab => tab.classList.remove('active'));
  contents.forEach(content => content.classList.remove('active'));
  
  // Activar la pestaña seleccionada
  document.querySelector(`[onclick="openTab('${tabId}')"]`).classList.add('active');
  document.getElementById(tabId).classList.add('active');
  
  // Cargar la galería si se selecciona esa pestaña
  if (tabId === 'gallery-tab') {
    loadGallery();
  }
}

// Función para abrir pestañas en el área de administración
function openAdminTab(tabId) {
  const tabs = document.querySelectorAll('.admin-tabs .tab');
  const contents = document.querySelectorAll('.admin-container .content');
  
  tabs.forEach(tab => tab.classList.remove('active'));
  contents.forEach(content => content.classList.remove('active'));
  
  document.querySelector(`[onclick="openAdminTab('${tabId}')"]`).classList.add('active');
  document.getElementById(tabId).classList.add('active');
  
  // Cargar información específica según la pestaña
  if (tabId === 'wifi-tab') {
    loadWifiStatus();
  }
}

// Modal de confirmación
function showConfirmModal(title, message, callback) {
  document.getElementById('confirmTitle').textContent = title;
  document.getElementById('confirmMessage').textContent = message;
  document.getElementById('confirmModal').style.display = 'block';
  confirmCallback = callback;
}

function closeConfirmModal() {
  document.getElementById('confirmModal').style.display = 'none';
  confirmCallback = null;
}

function confirmAction() {
  if (confirmCallback && typeof confirmCallback === 'function') {
    confirmCallback();
  }
  closeConfirmModal();
}

// ==========================
// FUNCIONES DE CÁMARA
// ==========================

// Función para pausar/reanudar el stream
function toggleStream(button) {
  const stream = document.getElementById('stream');
  const streamIcon = document.getElementById('streamIcon');
  const streamText = document.getElementById('streamText');
  
  if (streamPaused) {
    stream.src = "/stream";
    streamText.textContent = "Pausar";
    streamIcon.className = "icon icon-pause";
    document.getElementById('streamStatus').style.display = 'block';
  } else {
    stream.src = "";
    streamText.textContent = "Reanudar";
    streamIcon.className = "icon icon-play";
    document.getElementById('streamStatus').style.display = 'none';
  }
  
  streamPaused = !streamPaused;
}

// Función para tomar una foto
function capturePhoto() {
  const statusBox = document.getElementById('status');
  statusBox.textContent = "Capturando foto...";
  statusBox.className = "status-box";
  
  fetch('/capture')
    .then(response => {
      if (!response.ok) {
        throw new Error('Error al capturar foto');
      }
      return response.text();
    })
    .then(data => {
      statusBox.textContent = "Foto capturada con éxito";
      statusBox.className = "status-box success";
      showToast("Foto capturada con éxito", "success");
      
      // Si estamos en la galería, actualizarla
      if (document.getElementById('gallery-tab').classList.contains('active')) {
        loadGallery();
      }
    })
    .catch(error => {
      statusBox.textContent = "Error: " + error.message;
      statusBox.className = "status-box error";
      showToast("Error al capturar foto", "error");
    });
}

// Función para cambiar la resolución
function changeResolution(resolution) {
  const statusBox = document.getElementById('status');
  statusBox.textContent = "Cambiando resolución...";
  statusBox.className = "status-box";
  
  fetch(`/control?var=resolution&val=${resolution}`)
    .then(response => {
      if (!response.ok) {
        throw new Error('Error al cambiar resolución');
      }
      return response.text();
    })
    .then(data => {
      statusBox.textContent = `Resolución cambiada a ${resolution}`;
      statusBox.className = "status-box success";
      showToast(`Resolución: ${resolution}`, "success");
    })
    .catch(error => {
      statusBox.textContent = "Error: " + error.message;
      statusBox.className = "status-box error";
      showToast("Error al cambiar resolución", "error");
    });
}

// Función para cambiar efectos visuales
function toggleEffect(effect) {
  const statusBox = document.getElementById('status');
  statusBox.textContent = "Aplicando efecto...";
  statusBox.className = "status-box";
  
  // Mapeo de efectos a valores numéricos
  const effectMap = {
    'normal': 0,
    'negative': 1,
    'grayscale': 2
  };
  
  // Actualizar UI de botones
  document.querySelectorAll('[id^="effect-"]').forEach(button => {
    button.classList.remove('active');
  });
  document.getElementById(`effect-${effect}`).classList.add('active');
  
  fetch(`/control?var=special_effect&val=${effectMap[effect]}`)
    .then(response => {
      if (!response.ok) {
        throw new Error('Error al cambiar efecto');
      }
      return response.text();
    })
    .then(data => {
      statusBox.textContent = `Efecto cambiado a ${effect}`;
      statusBox.className = "status-box success";
      showToast(`Efecto: ${effect}`, "success");
    })
    .catch(error => {
      statusBox.textContent = "Error: " + error.message;
      statusBox.className = "status-box error";
      showToast("Error al cambiar efecto", "error");
    });
}

// ==========================
// FUNCIONES DE GALERÍA
// ==========================

// Cargar galería de imágenes
function loadGallery() {
  const gallery = document.getElementById('gallery');
  const galleryEmpty = document.getElementById('galleryEmpty');
  
  gallery.innerHTML = '<div class="gallery-loading"><span class="icon icon-spinner"></span> Cargando imágenes...</div>';
  galleryEmpty.style.display = 'none';
  
  fetch('/list')
    .then(response => {
      if (!response.ok) {
        throw new Error('Error al cargar galería');
      }
      return response.json();
    })
    .then(data => {
      if (data.length === 0) {
        gallery.innerHTML = '';
        galleryEmpty.style.display = 'block';
        return;
      }
      
      gallery.innerHTML = '';
      data.forEach(photo => {
        const item = document.createElement('div');
        item.className = 'gallery-item';
        
        const img = document.createElement('img');
        img.className = 'gallery-img';
        img.src = photo.path;
        img.alt = photo.name;
        img.loading = 'lazy'; // Carga perezosa para optimizar rendimiento
        
        img.onclick = function() {
          openImageModal(photo.path, photo.name);
        };
        
        item.appendChild(img);
        gallery.appendChild(item);
      });
    })
    .catch(error => {
      gallery.innerHTML = `<div class="gallery-loading"><span class="icon icon-exclamation"></span> Error: ${error.message}</div>`;
      showToast("Error al cargar galería", "error");
    });
}

// Abrir modal para visualizar imagen
function openImageModal(imagePath, fileName) {
  const modal = document.getElementById('imageModal');
  const modalImg = document.getElementById('modalImg');
  const modalTitle = document.getElementById('modalTitle');
  
  currentImagePath = imagePath;
  modalTitle.textContent = fileName || 'Vista previa';
  modalImg.src = imagePath;
  modal.style.display = 'block';
  
  // Bloquear scroll de fondo
  document.body.style.overflow = 'hidden';
}

// Cerrar modal de imagen
function closeModal() {
  document.getElementById('imageModal').style.display = 'none';
  document.body.style.overflow = '';
}

// Descargar imagen actual
function downloadImage() {
  if (!currentImagePath) return;
  
  const link = document.createElement('a');
  link.href = currentImagePath;
  link.download = currentImagePath.split('/').pop();
  document.body.appendChild(link);
  link.click();
  document.body.removeChild(link);
  
  showToast("Descargando imagen...", "success");
}

// Eliminar imagen (a implementar en backend)
function deleteImage() {
  if (!currentImagePath) return;
  
  showConfirmModal(
    "Confirmar eliminación", 
    "¿Estás seguro de que deseas eliminar esta imagen? Esta acción no se puede deshacer.",
    () => {
      // Aquí iría la implementación del endpoint de eliminación
      showToast("Función no implementada", "warning");
      closeModal();
    }
  );
}

// ==========================
// FUNCIONES DE ADMINISTRACIÓN - EDITOR DE ARCHIVOS
// ==========================

// Cargar contenido de un archivo
function loadFile(path) {
  const fileContent = document.getElementById('fileContent');
  const currentFileName = document.getElementById('currentFileName');
  const saveButton = document.getElementById('saveButton');
  const resetButton = document.getElementById('resetButton');
  
  fileContent.value = 'Cargando...';
  currentFileName.textContent = 'Cargando archivo...';
  saveButton.disabled = true;
  resetButton.disabled = true;
  
  fetch(path)
    .then(response => {
      if (!response.ok) {
        throw new Error(`Error ${response.status}: ${response.statusText}`);
      }
      return response.text();
    })
    .then(content => {
      originalFileContent = content;
      fileContent.value = content;
      currentFileName.textContent = `Archivo: ${path}`;
      saveButton.disabled = false;
      resetButton.disabled = false;
    })
    .catch(error => {
      fileContent.value = `Error al cargar el archivo: ${error.message}`;
      currentFileName.textContent = 'Error';
      showToast("Error al cargar archivo", "error");
    });
}

// Guardar cambios en un archivo
function saveFile() {
  const fileContent = document.getElementById('fileContent');
  const currentFileName = document.getElementById('currentFileName');
  const filePath = currentFileName.textContent.replace('Archivo: ', '');
  
  if (!filePath || filePath === 'Error' || filePath === 'Ningún archivo seleccionado') {
    showToast("Ningún archivo seleccionado", "error");
    return;
  }
  
  // Mostrar confirmación para prevenir cambios accidentales
  showConfirmModal(
    "Confirmar guardar", 
    "¿Estás seguro de que deseas guardar los cambios? Esto sobrescribirá el archivo actual.",
    () => {
      fetch(`/update-web?file=${encodeURIComponent(filePath)}`, {
        method: 'POST',
        body: fileContent.value,
        headers: {
          'Content-Type': 'text/plain'
        }
      })
      .then(response => {
        if (!response.ok) {
          throw new Error(`Error ${response.status}: ${response.statusText}`);
        }
        return response.json();
      })
      .then(data => {
        if (data.success) {
          originalFileContent = fileContent.value;
          showToast("Archivo guardado correctamente", "success");
        } else {
          showToast(`Error: ${data.message}`, "error");
        }
      })
      .catch(error => {
        showToast(`Error al guardar: ${error.message}`, "error");
      });
    }
  );
}

// Descartar cambios en el editor
function resetFile() {
  const fileContent = document.getElementById('fileContent');
  
  showConfirmModal(
    "Confirmar reinicio", 
    "¿Estás seguro de que deseas descartar todos los cambios?",
    () => {
      fileContent.value = originalFileContent;
      showToast("Cambios descartados", "warning");
    }
  );
}

// ==========================
// FUNCIONES DE ADMINISTRACIÓN - ACTUALIZACIÓN DE FIRMWARE
// ==========================

// Configuración del área de subida de firmware
function setupFirmwareUpload() {
  const dropzone = document.getElementById('firmwareDropzone');
  const fileInput = document.getElementById('firmwareFile');
  
  // Hacer clic en la zona para seleccionar archivo
  dropzone.addEventListener('click', () => {
    fileInput.click();
  });
  
  // Manejar selección de archivo
  fileInput.addEventListener('change', handleFirmwareSelection);
  
  // Manejar arrastrar y soltar
  dropzone.addEventListener('dragover', (e) => {
    e.preventDefault();
    dropzone.classList.add('drag-over');
  });
  
  dropzone.addEventListener('dragleave', () => {
    dropzone.classList.remove('drag-over');
  });
  
  dropzone.addEventListener('drop', (e) => {
    e.preventDefault();
    dropzone.classList.remove('drag-over');
    
    if (e.dataTransfer.files.length) {
      fileInput.files = e.dataTransfer.files;
      handleFirmwareSelection();
    }
  });
}

// Manejar la selección de archivo de firmware
function handleFirmwareSelection() {
  const fileInput = document.getElementById('firmwareFile');
  const uploadButton = document.getElementById('uploadButton');
  const firmwareInfo = document.getElementById('firmwareInfo');
  const selectedFileName = document.getElementById('selectedFileName');
  const selectedFileSize = document.getElementById('selectedFileSize');
  
  if (fileInput.files.length === 0) {
    firmwareInfo.style.display = 'none';
    uploadButton.disabled = true;
    return;
  }
  
  const file = fileInput.files[0];
  
  // Verificar si es un archivo .bin
  if (!file.name.toLowerCase().endsWith('.bin')) {
    showToast("El archivo debe tener extensión .bin", "error");
    fileInput.value = '';
    firmwareInfo.style.display = 'none';
    uploadButton.disabled = true;
    return;
  }
  
  // Mostrar información del archivo
  selectedFileName.textContent = file.name;
  
  // Formatear tamaño del archivo
  const sizes = ['Bytes', 'KB', 'MB', 'GB'];
  let fileSize = file.size;
  let sizeIndex = 0;
  while (fileSize > 1024 && sizeIndex < sizes.length - 1) {
    fileSize /= 1024;
    sizeIndex++;
  }
  selectedFileSize.textContent = `(${fileSize.toFixed(2)} ${sizes[sizeIndex]})`;
  
  firmwareInfo.style.display = 'block';
  uploadButton.disabled = false;
}

// Subir firmware
function uploadFirmware() {
  const fileInput = document.getElementById('firmwareFile');
  const uploadButton = document.getElementById('uploadButton');
  const cancelButton = document.getElementById('cancelButton');
  const progressContainer = document.getElementById('progressContainer');
  const progressBar = document.getElementById('progressBar');
  const progressPercent = document.getElementById('progressPercent');
  const progressText = document.getElementById('progressText');
  const otaStatus = document.getElementById('otaStatus');
  
  if (fileInput.files.length === 0) {
    showToast("Ningún archivo seleccionado", "error");
    return;
  }
  
  const file = fileInput.files[0];
  const formData = new FormData();
  formData.append('firmware', file);
  
  // Preparar UI para la subida
  uploadButton.disabled = true;
  cancelButton.style.display = 'block';
  progressContainer.style.display = 'block';
  progressBar.style.width = '0%';
  progressPercent.textContent = '0%';
  progressText.textContent = 'Iniciando...';
  otaStatus.textContent = '';
  otaStatus.className = 'ota-status';
  
  // Crear solicitud XHR para monitorear el progreso
  const xhr = new XMLHttpRequest();
  xhr.open('POST', '/update-ota', true);
  
  xhr.upload.onprogress = (e) => {
    if (e.lengthComputable) {
      const percent = Math.round((e.loaded / e.total) * 100);
      progressBar.style.width = percent + '%';
      progressPercent.textContent = percent + '%';
      
      if (percent < 100) {
        progressText.textContent = 'Subiendo...';
      } else {
        progressText.textContent = 'Procesando...';
      }
    }
  };
  
  xhr.onreadystatechange = function() {
    if (xhr.readyState === 4) {
      cancelButton.style.display = 'none';
      
      if (xhr.status === 200) {
        try {
          const response = JSON.parse(xhr.responseText);
          
          if (response.success) {
            progressText.textContent = 'Completado';
            otaStatus.textContent = 'Firmware actualizado correctamente. El dispositivo se reiniciará en breve...';
            otaStatus.className = 'ota-status success';
            
            showToast("Firmware actualizado con éxito", "success");
            
            // Programar redirección después de un tiempo para permitir reinicio
            setTimeout(() => {
              showToast("Reconectando al dispositivo...", "warning");
              setTimeout(() => {
                window.location.href = '/';
              }, 5000);
            }, 5000);
          } else {
            progressText.textContent = 'Error';
            otaStatus.textContent = response.message || 'Error al actualizar el firmware';
            otaStatus.className = 'ota-status error';
            showToast("Error en la actualización", "error");
            uploadButton.disabled = false;
          }
        } catch (e) {
          progressText.textContent = 'Error';
          otaStatus.textContent = 'Error al procesar la respuesta';
          otaStatus.className = 'ota-status error';
          showToast("Error en la actualización", "error");
          uploadButton.disabled = false;
        }
      } else {
        progressText.textContent = 'Error';
        otaStatus.textContent = `Error ${xhr.status}: ${xhr.statusText}`;
        otaStatus.className = 'ota-status error';
        showToast("Error en la actualización", "error");
        uploadButton.disabled = false;
      }
    }
  };
  
  xhr.onerror = function() {
    cancelButton.style.display = 'none';
    progressText.textContent = 'Error de conexión';
    otaStatus.textContent = 'Error de conexión al servidor';
    otaStatus.className = 'ota-status error';
    showToast("Error de conexión", "error");
    uploadButton.disabled = false;
  };
  
  // Enviar solicitud
  xhr.send(file);
}

// Cancelar subida de firmware
function cancelFirmwareUpload() {
  // Esta función necesitaría que el backend soporte cancelación
  showToast("Cancelando subida...", "warning");
  
  // Reiniciar UI
  document.getElementById('progressContainer').style.display = 'none';
  document.getElementById('cancelButton').style.display = 'none';
  document.getElementById('uploadButton').disabled = false;
  
  // En un caso real, aquí se abortaría la solicitud XHR
}

// ==========================
// FUNCIONES DE ADMINISTRACIÓN - CONFIGURACIÓN WIFI
// ==========================

// Cargar estado actual de WiFi
function loadWifiStatus() {
  const wifiMode = document.getElementById('wifiMode');
  const wifiSSID = document.getElementById('wifiSSID');
  const wifiIP = document.getElementById('wifiIP');
  
  // Aquí se debería implementar una llamada al backend para obtener el estado actual
  // Por ahora, mostraremos datos de ejemplo
  
  fetch('/wifi-status')
    .then(response => {
      if (!response.ok) {
        throw new Error('Error al cargar estado WiFi');
      }
      return response.json();
    })
    .then(data => {
      wifiMode.textContent = data.isAPMode ? 'Punto de Acceso' : 'Cliente';
      wifiSSID.textContent = data.ssid || 'VOC';
      wifiIP.textContent = data.ip || '192.168.4.1';
      
      // Actualizar formulario con datos actuales
      document.getElementById('ssidInput').value = data.configuredSsid || '';
      document.getElementById('apModeToggle').checked = data.apModeEnabled || false;
    })
    .catch(error => {
      // Si falla, mostrar valores por defecto
      wifiMode.textContent = 'Punto de Acceso';
      wifiSSID.textContent = 'VOC';
      wifiIP.textContent = '192.168.4.1';
      
      showToast("Error al cargar estado WiFi", "error");
    });
}

// Escanear redes WiFi disponibles
function scanNetworks() {
  const networkList = document.getElementById('networkList');
  const networkListContent = document.getElementById('networkListContent');
  const networkSelect = document.getElementById('wifiNetworkSelect');
  
  networkListContent.innerHTML = '<p class="text-center"><i class="fas fa-spinner fa-spin"></i> Buscando redes...</p>';
  networkList.style.display = 'block';
  
  // Limpiar selector de redes
  networkSelect.innerHTML = '<option value="" disabled selected>Seleccionar red...</option>';
  
  fetch('/scan-wifi')
    .then(response => {
      if (!response.ok) {
        throw new Error('Error al escanear redes WiFi');
      }
      return response.json();
    })
    .then(data => {
      wifiNetworks = data.networks || [];
      
      if (wifiNetworks.length === 0) {
        networkListContent.innerHTML = '<p class="text-center">No se encontraron redes WiFi</p>';
        return;
      }
      
      networkListContent.innerHTML = '';
      
      // Ordenar por intensidad de señal
      wifiNetworks.sort((a, b) => b.rssi - a.rssi);
      
      wifiNetworks.forEach((network, index) => {
        // Crear elemento en la lista
        const networkItem = document.createElement('div');
        networkItem.className = 'network-item';
        
        // Determinar icono basado en intensidad de señal
        const signalStrength = Math.min(Math.max(2 + Math.floor(network.rssi / 20), 0), 4);
        let signalIcon;
        
        if (signalStrength > 3) {
          signalIcon = 'fas fa-wifi';
        } else if (signalStrength > 2) {
          signalIcon = 'fas fa-wifi';
        } else if (signalStrength > 1) {
          signalIcon = 'fas fa-wifi';
        } else {
          signalIcon = 'fas fa-wifi';
        }
        
        // Crear contenido del elemento
        networkItem.innerHTML = `
          <div class="network-info">
            <i class="${signalIcon}"></i>
            <span>${network.ssid}</span>
            ${network.configured ? '<span class="badge">(Configurada)</span>' : ''}
          </div>
          <div class="network-details">
            <span class="network-security">${network.encryption ? '<i class="fas fa-lock"></i>' : '<i class="fas fa-lock-open"></i>'}</span>
            <div class="network-signal">
              ${Array(5).fill(0).map((_, i) => 
                `<div class="signal-bar ${i < signalStrength ? 'active' : ''}"></div>`
              ).join('')}
            </div>
          </div>
        `;
        
        // Agregar al contenedor
        networkListContent.appendChild(networkItem);
        
        // Agregar evento para seleccionar la red
        networkItem.addEventListener('click', () => {
          document.getElementById('ssidInput').value = network.ssid;
          document.getElementById('passwordInput').value = '';
          document.getElementById('passwordInput').focus();
        });
        
        // Agregar a la lista desplegable
        const option = document.createElement('option');
        option.value = index;
        option.textContent = network.ssid;
        option.selected = network.configured;
        networkSelect.appendChild(option);
      });
    })
    .catch(error => {
      networkListContent.innerHTML = `<p class="text-center">Error: ${error.message}</p>`;
      showToast("Error al escanear redes WiFi", "error");
    });
}

// Manejar selección de red del desplegable
function handleNetworkSelection() {
  const select = document.getElementById('wifiNetworkSelect');
  const index = parseInt(select.value);
  
  if (isNaN(index) || !wifiNetworks[index]) return;
  
  const network = wifiNetworks[index];
  document.getElementById('ssidInput').value = network.ssid;
  document.getElementById('passwordInput').value = '';
  document.getElementById('passwordInput').focus();
}

// Alternar visibilidad de la contraseña
function togglePasswordVisibility() {
  const passwordInput = document.getElementById('passwordInput');
  const passwordToggleIcon = document.getElementById('passwordToggleIcon');
  
  if (passwordInput.type === 'password') {
    passwordInput.type = 'text';
    passwordToggleIcon.className = 'icon eye-slash';
  } else {
    passwordInput.type = 'password';
    passwordToggleIcon.className = 'icon eye';
  }
}

// Guardar configuración WiFi
function saveWifiConfig() {
  const ssid = document.getElementById('ssidInput').value.trim();
  const password = document.getElementById('passwordInput').value;
  const apMode = document.getElementById('apModeToggle').checked;
  
  if (!ssid) {
    showToast("Debe ingresar un nombre de red", "error");
    return;
  }
  
  // Confirmar antes de guardar
  showConfirmModal(
    "Confirmar configuración WiFi", 
    "Al guardar la configuración WiFi, el dispositivo se reiniciará para aplicar los cambios. ¿Desea continuar?",
    () => {
      // Construir parámetros
      const params = new URLSearchParams({
        ssid,
        password,
        apMode: apMode ? '1' : '0'
      });
      
      fetch(`/config-wifi?${params.toString()}`)
        .then(response => {
          if (!response.ok) {
            throw new Error(`Error ${response.status}: ${response.statusText}`);
          }
          return response.json();
        })
        .then(data => {
          if (data.success) {
            showToast("Configuración WiFi guardada. El dispositivo se reiniciará...", "success");
            
            // Mostrar pantalla de espera
            document.querySelector('.wifi-config').innerHTML = `
              <div class="text-center" style="padding: 40px 20px;">
                <i class="fas fa-sync fa-spin" style="font-size: 3rem; margin-bottom: 20px; color: var(--primary-color);"></i>
                <h3>Reiniciando dispositivo...</h3>
                <p>El dispositivo está aplicando la nueva configuración WiFi.</p>
                <p>Por favor, espere un momento...</p>
                <div class="progress-container" style="margin-top: 30px;">
                  <div class="progress-bar">
                    <div class="progress" id="rebootProgress" style="width: 0%"></div>
                  </div>
                </div>
              </div>
            `;
            
            // Animar barra de progreso
            let progress = 0;
            const interval = setInterval(() => {
              progress += 1;
              document.getElementById('rebootProgress').style.width = `${progress}%`;
              
              if (progress >= 100) {
                clearInterval(interval);
                window.location.href = '/';
              }
            }, 300); // 30 segundos total para reiniciar
          } else {
            showToast(data.message || "Error al guardar configuración WiFi", "error");
          }
        })
        .catch(error => {
          showToast(`Error: ${error.message}`, "error");
        });
    }
  );
}

// Reiniciar formulario WiFi
function resetWifiForm() {
  document.getElementById('wifiForm').reset();
  showToast("Formulario reiniciado", "warning");
}

// ==========================
// INICIALIZACIÓN
// ==========================

// Inicializar cuando se carga la página
document.addEventListener('DOMContentLoaded', function() {
  // Comprobar si estamos en la página principal o en admin
  const isAdminPage = window.location.pathname.includes('/admin');
  
  if (isAdminPage) {
    // Inicializar página de administración
    setupFirmwareUpload();
    
    // Cargar estado WiFi si estamos en esa pestaña
    if (document.getElementById('wifi-tab').classList.contains('active')) {
      loadWifiStatus();
    }
  } else {
    // Inicializar página principal
    loadGallery();
  }
  
  // Cerrar modales al hacer clic fuera
  window.onclick = function(event) {
    const imageModal = document.getElementById('imageModal');
    const confirmModal = document.getElementById('confirmModal');
    
    if (event.target === imageModal) {
      closeModal();
    }
    
    if (event.target === confirmModal) {
      closeConfirmModal();
    }
  };
});