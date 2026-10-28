#include "milan/milan.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void money(int64_t cents)
{
    printf("$%lld.%02lld MXN", (long long)(cents / 100), (long long)(cents % 100));
}
void milan_print_product(const MilanProduct *p, int detail)
{
    if (!p)
        return;
    printf("%d. %s — ", p->id, p->name);
    money(p->price_cents);
    putchar('\n');
    if (detail)
        printf("  Descripción: %s\n  Marca: %s\n  Categoría: %s\n", p->description, p->brand,
               p->category);
}
void milan_print_cart(const MilanCartSummary *s)
{
    printf("Carrito: %zu líneas, %u unidades\n", s->line_count, s->total_units);
    for (size_t i = 0; i < s->line_count; i++)
    {
        const MilanCartItem *x = &s->items[i];
        printf("%s | ", x->product->name);
        money(x->product->price_cents);
        printf(" × %u = ", x->line.quantity);
        money(x->subtotal_cents);
        putchar('\n');
    }
    printf("Total: ");
    money(s->total_cents);
    putchar('\n');
}
static int read_int(const char *prompt, int *out)
{
    char buf[128], *end;
    long n;
    printf("%s", prompt);
    if (!fgets(buf, sizeof buf, stdin))
        return 0;
    buf[strcspn(buf, "\r\n")] = 0;
    if (!buf[0])
        return -1;
    n = strtol(buf, &end, 10);
    if (*end || end == buf || n < (-2147483647L - 1) || n > 2147483647L)
        return -1;
    *out = (int)n;
    return 1;
}
int milan_console_run(const MilanCatalog *cat, MilanCart *cart)
{
    for (;;)
    {
        puts("\nMILAN — Mercado Interactivo en Línea de Artículos y Novedades\n1 Listar productos  "
             "2 Ver detalle  3 Agregar  4 Ver carrito\n5 Modificar cantidad  6 Eliminar  0 Salir");
        int op, rc = read_int("Opción: ", &op);
        if (!rc)
            return 0;
        if (rc < 0)
        {
            puts("Entrada incorrecta.");
            continue;
        }
        if (op == 0)
            return 0;
        if (op == 1)
        {
            for (size_t i = 0; i < cat->count; i++)
                milan_print_product(&cat->products[i], 0);
            continue;
        }
        if (op == 4)
        {
            MilanCartSummary s;
            milan_cart_summary(cart, cat, &s);
            milan_print_cart(&s);
            continue;
        }
        if (op < 2 || op > 6)
        {
            puts("Opción incorrecta.");
            continue;
        }
        int id, q;
        rc = read_int("ID de producto: ", &id);
        if (rc <= 0)
        {
            puts("Entrada incorrecta.");
            continue;
        }
        if (op == 2)
        {
            const MilanProduct *p = milan_catalog_find(cat, id);
            if (p)
                milan_print_product(p, 1);
            else
                puts(milan_result_message(MILAN_PRODUCT_NOT_FOUND));
            continue;
        }
        if (op == 6)
        {
            MilanResult r = milan_cart_remove(cart, id);
            puts(milan_result_message(r));
            continue;
        }
        rc = read_int("Cantidad (1-99): ", &q);
        if (rc <= 0)
        {
            puts("Entrada incorrecta.");
            continue;
        }
        MilanResult r =
            op == 3 ? milan_cart_add(cart, cat, id, q) : milan_cart_set_quantity(cart, id, q);
        puts(milan_result_message(r));
    }
}
