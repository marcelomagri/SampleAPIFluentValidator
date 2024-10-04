#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <conio.h> // Necessário para _getch()
#include <windows.h> // Necessário para SetConsoleTextAttribute
#include <sql.h>
#include <sqlext.h>
#include <crow.h>

void checkError(SQLHANDLE handle, SQLSMALLINT type, RETCODE retCode) {
    if (retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO) {
        SQLSMALLINT i = 0;
        SQLINTEGER nativeError;
        SQLWCHAR state[7];
        SQLWCHAR message[256];
        SQLSMALLINT messageLen;

        while (SQLGetDiagRec(type, handle, ++i, state, &nativeError, message, sizeof(message), &messageLen) == SQL_SUCCESS)
        {
            std::cerr << "Erro: " << message << " (SQL State: " << state << ", Native Error: " << nativeError << ")" << std::endl;
        }
    }
}

int main() {
    // Muda a cor do texto
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Verde brilhante

    std::cout << "CppSampleAPI!\n";

    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]() {
        return "Hello World !";
        });

    CROW_ROUTE(app, "/version")([]() {
        SQLHENV envHandle;
        SQLHDBC dbcHandle;
        SQLHSTMT stmtHandle;
        SQLRETURN retCode;

        char vl_parametro[256]; // Ajuste o tamanho do buffer conforme necessário

        // Alocar o ambiente ODBC
        SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &envHandle);
        SQLSetEnvAttr(envHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

        // Alocar a conexão
        SQLAllocHandle(SQL_HANDLE_DBC, envHandle, &dbcHandle);

        // String de conexão
        SQLWCHAR connectionString[] = L"DRIVER={SQL Server Native Client 11.0};SERVER=db_sh0743_des.prodam;DATABASE=sh0743;UID=user_sh0743;PWD=pwd_sh0743;";

        // Conectar ao banco de dados
        retCode = SQLDriverConnectW(dbcHandle, NULL, connectionString, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);

        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {
            std::wcout << L"Conexão bem-sucedida!" << std::endl;

            // Alocar o manipulador de instrução
            SQLAllocHandle(SQL_HANDLE_STMT, dbcHandle, &stmtHandle);

            // Executar a consulta SQL
            retCode = SQLExecDirectW(stmtHandle, (SQLWCHAR*)L"SELECT vl_parametro FROM parametro", SQL_NTS);
            checkError(stmtHandle, SQL_HANDLE_STMT, retCode);

            // Manipular o resultado

            int columnNumber = 1; // A primeira coluna (vl_parametro)
            SQLLEN paramLength;

            while ((retCode = SQLFetch(stmtHandle)) != SQL_NO_DATA) {
                if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {
                    // Obter o valor da coluna vl_parametro
                    retCode = SQLGetData(stmtHandle, columnNumber, SQL_C_CHAR, vl_parametro, sizeof(vl_parametro), &paramLength);
                    if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {
                        std::wcout << L"Valor de vl_parametro: " << vl_parametro << std::endl;
                    }
                    else {
                        checkError(stmtHandle, SQL_HANDLE_STMT, retCode);
                    }
                }
                else {
                    checkError(stmtHandle, SQL_HANDLE_STMT, retCode);
                    break; // Sai do loop se houver erro
                }
            }

            // Liberar o manipulador de instrução
            SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);
        }
        else {
            checkError(dbcHandle, SQL_HANDLE_DBC, retCode);
        }

        // Desconectar do banco de dados e liberar os handles
        SQLDisconnect(dbcHandle);
        SQLFreeHandle(SQL_HANDLE_DBC, dbcHandle);
        SQLFreeHandle(SQL_HANDLE_ENV, envHandle);

        return vl_parametro;

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

    app.port(32500).multithreaded().bindaddr("10.10.154.40").run();

    // Espera que o usuário pressione uma tecla
    // std::cout << "Pressione qualquer tecla para sair...";
    // _getch(); // Espera que uma tecla seja pressionada

    // Restaura a cor original (opcional)
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Branco

    return 0;
}
