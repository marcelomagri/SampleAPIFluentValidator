using FluentValidation;
using FluentValidation.AspNetCore;
using SampleAPI.Models;
using System.Globalization;

var builder = WebApplication.CreateBuilder(args);

builder.Services.AddValidatorsFromAssemblyContaining<Program>();

var app = builder.Build();

app.MapPost("/pedido/incluir", (Pedido pedido, IValidator<Pedido> validator) => {

    var validationResults = validator.Validate(pedido);
    if (!validationResults.IsValid)
    {
        return Results.BadRequest(validationResults.Errors);
    }

    var PedidoID = Guid.NewGuid().ToString();

    return Results.Ok(new 
    { 
        Mensagem = $"Pedido {PedidoID} efetuado com sucesso em {DateTime.Now.ToString("D", new CultureInfo("pt-BR"))}. Obrigado!"
    });

});

app.MapGet("/", () => "Hello World!");

app.MapGet("/sample/{client?}/{id?}", (CLIENT_TYPE? client, int? id) => {

    if (id == null || (client == null || client.GetType() != typeof(CLIENT_TYPE)))
    {
        //if (client == CLIENT_TYPE.WEB || client == null)
        //    return Results.Ok(new
        //    {
        //        Error = "Id não informado",
        //        Message = "Id não informado"
        //    });
        //else
            return Results.BadRequest();
    }

    if (id > 999)
    {
        return Results.Ok(new
        {
            Error = "Id inválido",
            Message = $"Id {id} maior que 999 é inválido"
        });
    }
    else
    {
        return Results.Ok(new
        {
            Message = $"Sample {id}",
            Guid = Guid.NewGuid(),
        });
    }
});

app.Run();

enum CLIENT_TYPE
{
    WEB,
    DISPENSER
};