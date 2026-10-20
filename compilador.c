
#include <stdio.h>
#include <stdlib.h>

typedef struct tableEntry
{
    int symbol;
    char type;
    int location;
    struct tableEntry *next;
} tableEntry;

tableEntry *symbolTable = NULL;

void insertarSimbolo(int symbol, char type, int location)
{
    tableEntry *nuevo;
    tableEntry *actual;

    nuevo = malloc(sizeof(tableEntry));

    if (nuevo == NULL)
    {
        printf("Error al reservar memoria.\n");
        return;
    }

    nuevo->symbol = symbol;
    nuevo->type = type;
    nuevo->location = location;
    nuevo->next = NULL;

    if (symbolTable == NULL)
    {
        symbolTable = nuevo;
    }
    else
    {
        actual = symbolTable;

        while (actual->next != NULL)
        {
            actual = actual->next;
        }

        actual->next = nuevo;
    }
}

void mostrarTabla()
{
    tableEntry *actual;

    actual = symbolTable;

    printf("\n--- TABLA DE SIMBOLOS ---\n");
    printf("Simbolo\tTipo\tUbicacion\n");

    while (actual != NULL)
    {
        printf("%c\t%c\t%d\n",
               actual->symbol,
               actual->type,
               actual->location);

        actual = actual->next;
    }
}

tableEntry *buscarSimbolo(int symbol, char type)
{
    tableEntry *actual;

    actual = symbolTable;

    while (actual != NULL)
    {
        if (actual->symbol == symbol && actual->type == type)
        {
            return actual;
        }

        actual = actual->next;
    }

    return NULL;
}


int main()
{
    tableEntry *resultado;

    insertarSimbolo('x', 'V', 99);
    insertarSimbolo('y', 'V', 98);
    insertarSimbolo('z', 'V', 97);

    mostrarTabla();

    resultado = buscarSimbolo('y', 'V');

    if (resultado != NULL)
    {
        printf("\nSe encontro la variable y.\n");
        printf("Ubicacion: %d\n", resultado->location);
    }
    else
    {
        printf("\nNo se encontro la variable.\n");
    }

    return 0;
}
