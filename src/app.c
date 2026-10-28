#include "milan/milan.h"
const char *milan_result_message(MilanResult r)
{
    switch (r)
    {
    case MILAN_OK:
        return "Operación realizada.";
    case MILAN_PRODUCT_NOT_FOUND:
        return "Producto no encontrado.";
    case MILAN_ITEM_NOT_FOUND:
        return "Artículo no encontrado en el carrito.";
    case MILAN_INVALID_QUANTITY:
        return "Cantidad inválida: ingresa un entero de 1 a 99.";
    case MILAN_QUANTITY_LIMIT:
        return "La cantidad acumulada no puede exceder 99.";
    case MILAN_IO_ERROR:
        return "No se pudo leer el archivo del catálogo.";
    default:
        return "Los datos no son válidos.";
    }
}
