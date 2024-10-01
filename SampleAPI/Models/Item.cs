using FluentValidation;

namespace SampleAPI.Models
{
    public class Item
    {
        public string CodigoProduto { get; set; }
        public string Descricao { get; set; }
        public int Quantidade { get; set; }
        public decimal ValorUnitario { get; set; }
        public decimal ValorTotal => Quantidade * ValorUnitario;
    }

    public class ItemValidator : AbstractValidator<Item>
    {
        public ItemValidator()
        {
            RuleFor(i => i.CodigoProduto)
                .NotEmpty().WithMessage("O código do produto é obrigatório.");

            RuleFor(i => i.Descricao)
                .NotEmpty().WithMessage("A descrição é obrigatória.");

            RuleFor(i => i.Quantidade)
                .GreaterThan(0).WithMessage("A quantidade deve ser maior que zero.");

            RuleFor(i => i.ValorUnitario)
                .GreaterThan(0).WithMessage("O valor unitário deve ser maior que zero.");
        }
    }
}
