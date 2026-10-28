#include "milan/milan.h"
#include <string.h>

void milan_cart_init(MilanCart *cart)
{
    if (cart)
        memset(cart, 0, sizeof *cart);
}
static size_t find_line(const MilanCart *cart, int id)
{
    for (size_t i = 0; i < cart->count; i++)
        if (cart->lines[i].product_id == id)
            return i;
    return cart->count;
}
MilanResult milan_cart_add(MilanCart *cart, const MilanCatalog *catalog, int id, int quantity)
{
    if (quantity < 1 || quantity > 99)
        return MILAN_INVALID_QUANTITY;
    if (!milan_catalog_find(catalog, id))
        return MILAN_PRODUCT_NOT_FOUND;
    size_t i = find_line(cart, id);
    if (i < cart->count)
    {
        if (cart->lines[i].quantity + (unsigned)quantity > 99)
            return MILAN_QUANTITY_LIMIT;
        cart->lines[i].quantity += (unsigned)quantity;
        return MILAN_OK;
    }
    if (cart->count >= MILAN_PRODUCT_CAPACITY)
        return MILAN_DATA_ERROR;
    cart->lines[cart->count++] = (MilanCartLine){id, (unsigned)quantity};
    return MILAN_OK;
}
MilanResult milan_cart_set_quantity(MilanCart *cart, int id, int quantity)
{
    if (quantity < 1 || quantity > 99)
        return MILAN_INVALID_QUANTITY;
    size_t i = find_line(cart, id);
    if (i == cart->count)
        return MILAN_ITEM_NOT_FOUND;
    cart->lines[i].quantity = (unsigned)quantity;
    return MILAN_OK;
}
MilanResult milan_cart_remove(MilanCart *cart, int id)
{
    size_t i = find_line(cart, id);
    if (i == cart->count)
        return MILAN_ITEM_NOT_FOUND;
    for (size_t j = i + 1; j < cart->count; j++)
        cart->lines[j - 1] = cart->lines[j];
    --cart->count;
    return MILAN_OK;
}
MilanResult milan_cart_summary(const MilanCart *cart, const MilanCatalog *catalog,
                               MilanCartSummary *out)
{
    if (!cart || !catalog || !out)
        return MILAN_DATA_ERROR;
    MilanCartSummary s = {0};
    for (size_t i = 0; i < cart->count; i++)
    {
        const MilanCartLine *line = &cart->lines[i];
        const MilanProduct *p = milan_catalog_find(catalog, line->product_id);
        if (!p || line->quantity < 1 || line->quantity > 99)
            return MILAN_DATA_ERROR;
        size_t j = s.line_count++;
        while (j > 0 && s.items[j - 1].line.product_id > line->product_id)
        {
            s.items[j] = s.items[j - 1];
            --j;
        }
        s.items[j] = (MilanCartItem){*line, p, p->price_cents * (int64_t)line->quantity};
        s.total_units += line->quantity;
        s.total_cents += p->price_cents * (int64_t)line->quantity;
    }
    *out = s;
    return MILAN_OK;
}
