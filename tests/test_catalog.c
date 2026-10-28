#include "milan/milan.h"
#include <stdio.h>
int main(int argc, char **argv)
{
    MilanCatalog c;
    if (argc != 2 || milan_catalog_load(argv[1], &c) != MILAN_OK)
        return 1;
    if (c.count != 12 || c.products[0].id != 1 || c.products[11].id != 12 ||
        c.products[0].price_cents != 1950 || !milan_catalog_find(&c, 5) ||
        milan_catalog_find(&c, 999))
        return 1;
    puts("Catálogo: OK");
    return 0;
}
