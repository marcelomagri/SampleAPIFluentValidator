use actix_web::{web, App, HttpServer, HttpResponse};
use odbc::*;
use std::ptr;

async fn hello() -> HttpResponse {
    HttpResponse::Ok().body("Hello, World!")
}

#[actix_web::main]
async fn main() -> std::io::Result<()> {
    // Inicializa o servidor na porta 8080
    HttpServer::new(|| {
        App::new()
            .route("/", web::get().to(hello)) // Define a rota para a função hello
            .route("/version", web::get().to(get_version))
    })
    .bind("127.0.0.1:8080")? // Liga o servidor ao endereço e porta
    .run() // Executa o servidor
    .await
}
