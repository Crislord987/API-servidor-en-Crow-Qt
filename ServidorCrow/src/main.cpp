#include <crow.h>
#include <jwt-cpp/jwt.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using json = nlohmann::json;

// Estructura simple para almacenar usuarios en memoria
struct User {
    string username;
    string password;  // En la vida real, esto debería estar hasheado
    int id;
};

// "Base de datos" en memoria (se pierde al reiniciar)
vector<User> users_db;
int next_user_id = 1;

int main() {
    crow::SimpleApp app;
    
    // Endpoint de registro - POST /register
    CROW_ROUTE(app, "/register").methods("POST"_method)([](const crow::request& req) {
        cout << "📝 Solicitud de registro recibida" << endl;
        
        try {
            // 1. Parsear el JSON del body
            json request_data = json::parse(req.body);
            
            // 2. Verificar que tenga los campos necesarios
            if (!request_data.contains("username") || !request_data.contains("password")) {
                json error_response = {
                    {"success", false},
                    {"error", "Se requieren los campos: username y password"}
                };
                return crow::response(400, error_response.dump());
            }
            
            string username = request_data["username"];
            string password = request_data["password"];
            
            cout << "🔍 Intentando registrar usuario: " << username << endl;
            
            // 3. Verificar que el username no esté vacío
            if (username.empty() || password.empty()) {
                json error_response = {
                    {"success", false},
                    {"error", "Username y password no pueden estar vacíos"}
                };
                return crow::response(400, error_response.dump());
            }
            
            // 4. Verificar si el usuario ya existe
            for (const auto& user : users_db) {
                if (user.username == username) {
                    json error_response = {
                        {"success", false},
                        {"error", "El usuario ya existe"}
                    };
                    return crow::response(409, error_response.dump()); // 409 = Conflict
                }
            }
            
            // 5. "Crear" el usuario (guardar en memoria)
            User new_user = {
                username,
                password,  // ⚠️ En producción: hashear con bcrypt
                next_user_id++
            };
            users_db.push_back(new_user);
            
            cout << "✅ Usuario creado: " << username << " con ID: " << new_user.id << endl;
            
            // 6. Generar JWT token para el usuario recién creado
            auto token = jwt::create()
                .set_issuer("auth.transmi")
                .set_type("JWS")
                .set_payload_claim("user_id", jwt::claim(to_string(new_user.id)))
                .set_payload_claim("username", jwt::claim(username))
                .set_expires_at(chrono::system_clock::now() + chrono::hours{24}) // Expira en 24 horas
                .sign(jwt::algorithm::hs256{"mi_secreto_super_seguro"});
            
            // 7. Respuesta exitosa
            json success_response = {
                {"success", true},
                {"message", "Usuario registrado exitosamente"},
                {"user", {
                    {"id", new_user.id},
                    {"username", username}
                }},
                {"token", token},
                {"expires_in", 86400} // 24 horas en segundos
            };
            
            return crow::response(201, success_response.dump()); // 201 = Created
            
        } catch (const json::exception& e) {
            cout << "❌ Error de JSON: " << e.what() << endl;
            json error_response = {
                {"success", false},
                {"error", "JSON inválido"}
            };
            return crow::response(400, error_response.dump());
            
        } catch (const exception& e) {
            cout << "❌ Error interno: " << e.what() << endl;
            json error_response = {
                {"success", false},
                {"error", "Error interno del servidor"}
            };
            return crow::response(500, error_response.dump());
        }
    });
    
    // Endpoint para ver usuarios registrados (solo para debug)
    CROW_ROUTE(app, "/users")
    ([](const crow::request& req) {
        json response = {
            {"success", true},
            {"users", json::array()}
        };
        
        for (const auto& user : users_db) {
            response["users"].push_back({
                {"id", user.id},
                {"username", user.username}
                // ⚠️ NO enviamos la password por seguridad
            });
        }
        
        return crow::response(200, response.dump());
    });
    
    // Endpoint de login simple
    CROW_ROUTE(app, "/login").methods("POST"_method)
    ([](const crow::request& req) {
        cout << "🔑 Solicitud de login recibida" << endl;
        
        try {
            json request_data = json::parse(req.body);
            
            if (!request_data.contains("username") || !request_data.contains("password")) {
                json error_response = {
                    {"success", false},
                    {"error", "Se requieren username y password"}
                };
                return crow::response(400, error_response.dump());
            }
            
            string username = request_data["username"];
            string password = request_data["password"];
            
            // Buscar usuario
            for (const auto& user : users_db) {
                if (user.username == username && user.password == password) {
                    // ✅ Usuario encontrado, generar token
                    auto token = jwt::create()
                        .set_issuer("auth.transmi")
                        .set_type("JWS")
                        .set_payload_claim("user_id", jwt::claim(to_string(user.id)))
                        .set_payload_claim("username", jwt::claim(username))
                        .set_expires_at(chrono::system_clock::now() + chrono::hours{24})
                        .sign(jwt::algorithm::hs256{"mi_secreto_super_seguro"});
                    
                    json success_response = {
                        {"success", true},
                        {"message", "Login exitoso"},
                        {"user", {
                            {"id", user.id},
                            {"username", username}
                        }},
                        {"token", token}
                    };
                    
                    return crow::response(200, success_response.dump());
                }
            }
            
            // ❌ Usuario no encontrado o password incorrecta
            json error_response = {
                {"success", false},
                {"error", "Credenciales inválidas"}
            };
            return crow::response(401, error_response.dump()); // 401 = Unauthorized
            
        } catch (const exception& e) {
            json error_response = {
                {"success", false},
                {"error", "Error interno del servidor"}
            };
            return crow::response(500, error_response.dump());
        }
    });
    
    // Iniciar servidor
    cout << "🚀 Servidor iniciando en puerto 8080..." << endl;
    cout << "📍 Endpoints disponibles:" << endl;
    cout << "   POST /register - Registrar usuario" << endl;
    cout << "   POST /login    - Iniciar sesión" << endl;
    cout << "   GET  /users    - Ver usuarios (debug)" << endl;
    
    app.port(8080).multithreaded().run();
    
    return 0;
}