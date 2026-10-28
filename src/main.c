#include "milan/milan.h"
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char **argv)
{

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    const char *path = argc > 1 ? argv[1] : "data/PRODUCTOS.csv";
    MilanCatalog catalog;
    MilanResult r = milan_catalog_load(path, &catalog);
    if (r != MILAN_OK)
    {
        fprintf(stderr, "%s\n", milan_result_message(r));
        return 1;
    }
    MilanCart cart;
    milan_cart_init(&cart);
    return milan_console_run(&catalog, &cart);
}
