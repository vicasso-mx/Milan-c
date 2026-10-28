#include "milan/milan.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int parse_record(const char *line, MilanProduct *p)
{
    char fields[7][MILAN_TEXT_CAPACITY];
    size_t field = 0, pos = 0;
    int quoted = 0;
    memset(fields, 0, sizeof fields);
    for (size_t i = 0;; ++i)
    {
        char c = line[i];
        if (quoted)
        {
            if (c == '"' && line[i + 1] == '"')
            {
                if (pos + 1 >= sizeof fields[0])
                    return 0;
                fields[field][pos++] = '"';
                ++i;
            }
            else if (c == '"')
                quoted = 0;
            else if (c == '\0')
                return 0;
            else
            {
                if (pos + 1 >= sizeof fields[0])
                    return 0;
                fields[field][pos++] = c;
            }
        }
        else if (c == '"' && pos == 0)
            quoted = 1;
        else if (c == ',' || c == '\0' || c == '\r' || c == '\n')
        {
            fields[field][pos] = '\0';
            if (c == ',')
            {
                if (++field >= 7)
                    return 0;
                pos = 0;
            }
            else
            {
                if (field != 6)
                    return 0;
                break;
            }
        }
        else
        {
            if (pos + 1 >= sizeof fields[0])
                return 0;
            fields[field][pos++] = c;
        }
    }
    char *end = NULL;
    long id = strtol(fields[0], &end, 10);
    if (!end || *end || id <= 0 || id > 2147483647L)
        return 0;
    long long price = strtoll(fields[5], &end, 10);
    if (!end || *end || price <= 0)
        return 0;
    if (!fields[1][0] || !fields[2][0] || strlen(fields[6]) >= MILAN_PATH_CAPACITY)
        return 0;
    p->id = (int)id;
    p->price_cents = (int64_t)price;
    strcpy(p->name, fields[1]);
    strcpy(p->description, fields[2]);
    strcpy(p->brand, fields[3]);
    strcpy(p->category, fields[4]);
    strcpy(p->image, fields[6]);
    return 1;
}

MilanResult milan_catalog_load(const char *path, MilanCatalog *catalog)
{
    if (!path || !catalog)
        return MILAN_DATA_ERROR;
    FILE *f = fopen(path, "rb");
    if (!f)
        return MILAN_IO_ERROR;
    MilanCatalog tmp = {0};
    char line[4096];
    if (!fgets(line, sizeof line, f))
    {
        fclose(f);
        return MILAN_DATA_ERROR;
    }
    while (fgets(line, sizeof line, f))
    {
        if (tmp.count == MILAN_PRODUCT_CAPACITY || !parse_record(line, &tmp.products[tmp.count]))
        {
            fclose(f);
            return MILAN_DATA_ERROR;
        }
        for (size_t i = 0; i < tmp.count; ++i)
            if (tmp.products[i].id == tmp.products[tmp.count].id)
            {
                fclose(f);
                return MILAN_DATA_ERROR;
            }
        ++tmp.count;
    }
    if (ferror(f) || tmp.count != MILAN_PRODUCT_CAPACITY)
    {
        fclose(f);
        return MILAN_DATA_ERROR;
    }
    fclose(f);
    for (size_t i = 0; i < tmp.count; ++i)
        for (size_t j = i + 1; j < tmp.count; ++j)
            if (tmp.products[j].id < tmp.products[i].id)
            {
                MilanProduct t = tmp.products[i];
                tmp.products[i] = tmp.products[j];
                tmp.products[j] = t;
            }
    *catalog = tmp;
    return MILAN_OK;
}

const MilanProduct *milan_catalog_find(const MilanCatalog *catalog, int id)
{
    if (!catalog)
        return NULL;
    for (size_t i = 0; i < catalog->count; ++i)
        if (catalog->products[i].id == id)
            return &catalog->products[i];
    return NULL;
}
