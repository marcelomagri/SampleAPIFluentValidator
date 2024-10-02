#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <conio.h> // Necessário para _getch()
#include <windows.h> // Necessário para SetConsoleTextAttribute
#include <crow.h>

int main() {
    // Muda a cor do texto
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Verde brilhante

    std::cout << "CppSampleAPI!\n";

    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]() {
        return "Hello World !";
        });

    // Rota POST que recebe um ID e retorna o mesmo ID
    CROW_ROUTE(app, "/produtos/id").methods("POST"_method)([](const crow::request& req) {
        // Extrair o ID do corpo da requisição
        auto json = crow::json::load(req.body);
        if (!json) {
            return crow::response(400); // Bad Request
        }

        // Verifique se o ID está presente no JSON
        if (!json.has("id")) {
            return crow::response(400); // Bad Request
        }

        int id = json["id"].i(); // Extrai o ID

        crow::json::wvalue response_json;
        response_json["id_recebido"] = "O id recebido pela aplicacao foi :" + std::to_string(id); // Atribui o ID
        return crow::response{ response_json }; // Retorna a resposta JSON
    });

    app.port(32500).multithreaded().bindaddr("127.0.0.1").run();

    // Espera que o usuário pressione uma tecla
    // std::cout << "Pressione qualquer tecla para sair...";
    // _getch(); // Espera que uma tecla seja pressionada

    // Restaura a cor original (opcional)
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Branco

    return 0;
}
