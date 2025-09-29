# Sistema de Autenticación con Microservicios

Un sistema completo de autenticación desarrollado con C++ y Qt6 que implementa patrones de microservicios, APIs REST y autenticación JWT.

## 🏗️ Arquitectura

```
[Cliente Qt6] ←→ HTTP/REST ←→ [Servidor C++ (Crow)] ←→ [Base de Datos]
```

## ✨ Características

- 🔐 **Autenticación JWT** - Tokens seguros con expiración
- 📱 **Cliente Qt6** - Interfaz gráfica moderna y responsiva
- 🌐 **API REST** - Endpoints estándar con JSON
- 🚀 **Servidor C++** - Alta performance con Crow framework
- 📊 **Logging completo** - Trazabilidad en tiempo real
- 🔧 **Arquitectura modular** - Fácil mantenimiento y escalabilidad

## 🛠️ Tecnologías

### Backend (Servidor C++)
- **Framework**: Crow (REST API)
- **Autenticación**: jwt-cpp
- **JSON**: nlohmann/json
- **Compilador**: C++17/20
- **Build System**: CMake

### Frontend (Cliente)
- **Framework**: Qt6 (Core, Widgets, Network)
- **HTTP Client**: QNetworkAccessManager
- **JSON**: QJsonDocument
- **Build System**: CMake + MOC

## 📦 Instalación

### Prerrequisitos

#### Para el Servidor C++
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install build-essential cmake libcrow-dev nlohmann-json3-dev

# Instalar jwt-cpp (manual)
git clone https://github.com/Thalhammer/jwt-cpp.git
cd jwt-cpp && mkdir build && cd build
cmake .. -DJWT_BUILD_EXAMPLES=OFF
make && sudo make install
```

#### Para el Cliente Qt6
```bash
# Ubuntu/Debian
sudo apt install qt6-base-dev qt6-network-dev cmake

# Windows: Descargar Qt6 desde https://www.qt.io/download
# macOS: brew install qt6
```

### Clonar el Repositorio
```bash
git clone https://github.com/tu-usuario/sistema-autenticacion.git
cd sistema-autenticacion
```

## 🚀 Uso

### 1. Compilar y Ejecutar el Servidor

```bash
cd servidor/
mkdir build && cd build
cmake ..
make

# Ejecutar servidor
./AuthServer
```

**Salida esperada:**
```
🚀 Servidor iniciando en puerto 8080...
📍 Endpoints disponibles:
   POST /register - Registrar usuario
   POST /login    - Iniciar sesión
   GET  /users    - Ver usuarios (debug)
[INFO] Crow server is running at http://0.0.0.0:8080
```

### 2. Compilar y Ejecutar el Cliente Qt6

```bash
cd cliente-qt/
mkdir build && cd build
cmake ..
make

# Ejecutar cliente
./AuthClient
```

### 3. Probar el Sistema

1. **Registrar Usuario:**
   - Abrir cliente Qt6
   - Usuario: `juan`, Password: `1234`
   - Click "🚀 Registrar"

2. **Iniciar Sesión:**
   - Usuario: `juan`, Password: `1234`
   - Click "🔐 Iniciar Sesión"

3. **Ver Usuarios:**
   - Click "👥 Ver Usuarios"

## 📡 API Endpoints

### Autenticación

#### POST `/register`
Registra un nuevo usuario y devuelve JWT token.

**Request:**
```json
{
  "username": "juan",
  "password": "1234"
}
```

**Response (201):**
```json
{
  "success": true,
  "message": "Usuario registrado exitosamente",
  "user": {
    "id": 1,
    "username": "juan"
  },
  "token": "eyJhbGciOiJIUzI1NiIs...",
  "expires_in": 86400
}
```

#### POST `/login`
Autentica usuario existente y devuelve JWT token.

**Request:**
```json
{
  "username": "juan",
  "password": "1234"
}
```

**Response (200):**
```json
{
  "success": true,
  "message": "Login exitoso",
  "user": {
    "id": 1,
    "username": "juan"
  },
  "token": "eyJhbGciOiJIUzI1NiIs..."
}
```

### Usuarios

#### GET `/users`
Obtiene lista de usuarios registrados (debug).

**Response (200):**
```json
{
  "success": true,
  "users": [
    {
      "id": 1,
      "username": "juan"
    }
  ]
}
```

## 🎯 Estructura del Proyecto

```
sistema-autenticacion/
├── servidorCrow/                    # Microservicio C++ (Crow)
│   ├── src/
│   │   └── main.cpp
│   ├── CMakeLists.txt
│   └── README.md
├── UI/                  # Cliente Qt6
│   ├── src/
│   │   ├── main.cpp
│   │   ├── mainwindow.cpp
│   │   ├── mainwindow.h
│   │   └── mainwindow.ui
│   ├── CMakeLists.txt
│   └── README.md
└── README.md
```

## 🔐 Seguridad

### JWT Tokens
- **Algoritmo**: HMAC SHA-256
- **Expiración**: 24 horas
- **Claims**: user_id, username, issuer
- **Secret**: Configurable (cambiar en producción)

### Validaciones
- ✅ Campos requeridos (username, password)
- ✅ Usuarios únicos
- ✅ Credenciales válidas
- ✅ Manejo de errores robusto

## 🧪 Testing

### Testing Manual con cURL

```bash
# Verificar servidor
curl http://localhost:8080/users

# Registrar usuario
curl -X POST http://localhost:8080/register \
  -H "Content-Type: application/json" \
  -d '{"username": "test", "password": "123"}'

# Hacer login
curl -X POST http://localhost:8080/login \
  -H "Content-Type: application/json" \
  -d '{"username": "test", "password": "123"}'
```

### Testing con Postman
Importar la colección desde `tools/postman/auth-collection.json`

## 📊 Monitoreo y Logs

### Logs del Servidor
```
📝 Solicitud de registro recibida
🔍 Intentando registrar usuario: juan
✅ Usuario creado: juan con ID: 1
🔑 Solicitud de login recibida
```

### Logs del Cliente Qt
```
[18:30:15] 🚀 Registrando usuario: juan
[18:30:15] ✅ Usuario registrado: juan
[18:30:15] 🎫 Token recibido
```

## 🔧 Configuración

### Variables de Entorno (Servidor)
```bash
export JWT_SECRET="tu_secreto_super_seguro"
export SERVER_PORT=8080
export LOG_LEVEL=INFO
```

### Configuración Qt Cliente
```cpp
// En mainwindow.cpp
m_baseUrl("http://localhost:8080")  // Cambiar para producción
```

## 🚀 Roadmap

### Versión 1.1
- [ ] Base de datos PostgreSQL
- [ ] Hash de passwords con bcrypt
- [ ] Middleware de validación JWT
- [ ] Rate limiting
- [ ] CORS configuración

### Versión 1.2
- [ ] Microservicio de transacciones
- [ ] Database migrations
- [ ] Docker containerization
- [ ] CI/CD pipeline

### Versión 2.0
- [ ] gRPC communication
- [ ] Redis para sessions
- [ ] Prometheus metrics
- [ ] Load balancer

## 🤝 Contribuir

1. Fork el proyecto
2. Crear feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit cambios (`git commit -m 'Add some AmazingFeature'`)
4. Push a la branch (`git push origin feature/AmazingFeature`)
5. Abrir Pull Request



## 👥 Autores

- **Cristian David** - *Desarrollo inicial* - [tu-github](https://github.com/crislord987)

## 🙏 Agradecimientos

- [Crow Framework](https://github.com/CrowCpp/Crow) - Por el excelente framework REST
- [jwt-cpp](https://github.com/Thalhammer/jwt-cpp) - Por la implementación JWT
- [nlohmann/json](https://github.com/nlohmann/json) - Por el parser JSON
- [Qt6](https://www.qt.io/) - Por el framework de interfaz gráfica

## 📞 Soporte

- Crear un [Issue](https://github.com/tu-usuario/sistema-autenticacion/issues)
- Email: crislor1050@gmail.com
- Discord: crislord987

---