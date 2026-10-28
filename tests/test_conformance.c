#include "milan/milan.h"
#include <stdio.h>
#include <string.h>

static int check(const MilanCart *cart, const MilanCatalog *cat, size_t lines, unsigned units,
                 int64_t cents)
{
    MilanCartSummary s;
    if (milan_cart_summary(cart, cat, &s) != MILAN_OK)
        return 0;
    return s.line_count == lines && s.total_units == units && s.total_cents == cents;
}
static int expect(MilanResult got, MilanResult expected)
{
    return got == expected;
}
int main(int argc, char **argv)
{
    if (argc != 2)
        return 1;
    MilanCatalog c;
    if (milan_catalog_load(argv[1], &c) != MILAN_OK)
        return 1;
    MilanCart x;
    milan_cart_init(&x);
    if (!check(&x, &c, 0, 0, 0))
        return 1;
    if (!expect(milan_cart_add(&x, &c, 1, 2), MILAN_OK) || !check(&x, &c, 1, 2, 3900))
        return 1;
    milan_cart_init(&x);
    if (!expect(milan_cart_add(&x, &c, 2, 3), MILAN_OK) || !check(&x, &c, 1, 3, 2550))
        return 1;
    milan_cart_init(&x);
    if (!expect(milan_cart_add(&x, &c, 999, 1), MILAN_PRODUCT_NOT_FOUND) ||
        !expect(milan_cart_add(&x, &c, 1, 0), MILAN_INVALID_QUANTITY) ||
        !expect(milan_cart_add(&x, &c, 1, -1), MILAN_INVALID_QUANTITY) ||
        !expect(milan_cart_add(&x, &c, 1, 100), MILAN_INVALID_QUANTITY) ||
        !expect(milan_cart_set_quantity(&x, 1, 2), MILAN_ITEM_NOT_FOUND) ||
        !expect(milan_cart_remove(&x, 1), MILAN_ITEM_NOT_FOUND) || !check(&x, &c, 0, 0, 0))
        return 1;
    milan_cart_init(&x);
    if (!expect(milan_cart_add(&x, &c, 1, 2), MILAN_OK) ||
        !expect(milan_cart_add(&x, &c, 2, 3), MILAN_OK) || !check(&x, &c, 2, 5, 6450) ||
        !expect(milan_cart_set_quantity(&x, 1, 1), MILAN_OK) || !check(&x, &c, 2, 4, 4500) ||
        !expect(milan_cart_remove(&x, 2), MILAN_OK) || !check(&x, &c, 1, 1, 1950) ||
        !expect(milan_cart_remove(&x, 1), MILAN_OK) || !check(&x, &c, 0, 0, 0))
        return 1;
    milan_cart_init(&x);
    if (!expect(milan_cart_add(&x, &c, 1, 99), MILAN_OK) || !check(&x, &c, 1, 99, 193050) ||
        !expect(milan_cart_add(&x, &c, 1, 1), MILAN_QUANTITY_LIMIT) ||
        !check(&x, &c, 1, 99, 193050))
        return 1;
    milan_cart_init(&x);
    if (!expect(milan_cart_add(&x, &c, 2, 1), MILAN_OK) ||
        !expect(milan_cart_add(&x, &c, 2, 2), MILAN_OK) || !check(&x, &c, 1, 3, 2550))
        return 1;
    milan_cart_init(&x);
    if (!expect(milan_cart_add(&x, &c, 1, 2), MILAN_OK) ||
        !expect(milan_cart_set_quantity(&x, 1, 0), MILAN_INVALID_QUANTITY) ||
        !check(&x, &c, 1, 2, 3900))
        return 1;
    puts("Conformidad T01-T13: OK");
    return 0;
}
