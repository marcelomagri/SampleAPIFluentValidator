using FluentValidation;

namespace SampleAPI.Models
{
    public class Pedido
    {
        public string NomeCliente { get; set; }
        public string EnderecoCliente { get; set; }
        public decimal ValorPedido { get; set; }
        public char TipoVenda { get; set; }
        public List<Item> Itens { get; set; }
    }

    public class PedidoValidator : AbstractValidator<Pedido>
    {
        public PedidoValidator()
        {
            RuleFor(p => p.NomeCliente)
                .NotEmpty().WithMessage("O nome do cliente é obrigatório.")
                .MinimumLength(3).WithMessage("O nome deve ter pelo menos 3 caracteres.");

            RuleFor(p => p.EnderecoCliente)
                .NotEmpty().WithMessage("O endereço é obrigatório.");

            RuleFor(p => p.Itens)
                .NotEmpty().WithMessage("O pedido deve ter pelo menos um item.");

            // Validação personalizada para garantir que o valor do pedido seja igual à soma dos itens
            RuleFor(p => p.ValorPedido)
                .Equal(p => p.Itens.Sum(i => i.Quantidade * i.ValorUnitario))
                .WithMessage("O valor do pedido deve ser igual à soma dos valores dos itens.");

            // Nova validação para tipo de venda Atacado
            RuleFor(p => p.Itens)
                .Must((pedido, itens) =>
                    pedido.TipoVenda != 'A' || itens.Count >= 5)
                .WithMessage("Para vendas no atacado, é necessário um mínimo de 5 itens.")
                .When(p => p.TipoVenda == 'A'); // Aplicar somente se a venda for Atacado

            // Validação para garantir que TipoVenda seja 'A' ou 'V'
            RuleFor(p => p.TipoVenda)
                .Must(tipo => tipo == 'A' || tipo == 'V')
                .WithMessage("O tipo de venda deve ser 'A' (Atacado) ou 'V' (Varejo).");


            RuleForEach(p => p.Itens).SetValidator(new ItemValidator());
        }
    }

    function VaiCorinthians()
    {
        console.log('Vai Corinthians');
    }
}
