cmake_minimum_required(VERSION 3.15)
project(ServidorCrow CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencias
find_package(Crow CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(libpqxx CONFIG REQUIRED)
find_package(jwt-cpp CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

# Ejecutable
add_executable(ServidorCrow src/main.cpp)

# Vinculación
target_link_libraries(ServidorCrow
  PRIVATE
    Crow::Crow
    nlohmann_json::nlohmann_json
    libpqxx::pqxx          # libpqxx suele exportar este target
    jwt-cpp::jwt-cpp    # este target puede variar según versión (lo verificamos si falla)
    OpenSSL::SSL
    OpenSSL::Crypto
)
