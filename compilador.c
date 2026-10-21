
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
int siguienteUbicacion = 99;

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
        if (actual->type == 'V')
        {
            printf("%c\t", actual->symbol);
        }
        else
        {
            printf("%d\t", actual->symbol);
        }

        printf("%c\t", actual->type);

        if (actual->location == -1)
        {
            printf("-\n");
        }
        else
        {
            printf("%d\n", actual->location);
        }

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


void registrarSimbolo(int symbol, char type)
{
    tableEntry *encontrado;
    int location = -1;

    encontrado = buscarSimbolo(symbol, type);

    if (encontrado != NULL)
    {
        return;
    }

    if (type == 'V' || type == 'C')
    {
        if (siguienteUbicacion < 0)
        {
            printf("Error: no hay memoria disponible.\n");
            return;
        }

        location = siguienteUbicacion;
        siguienteUbicacion--;
    }

    insertarSimbolo(symbol, type, location);
}

int main()
{
    registrarSimbolo(10, 'L');
    registrarSimbolo('x', 'V');
    registrarSimbolo(123, 'C');
    registrarSimbolo('y', 'V');
    registrarSimbolo('x', 'V');

    mostrarTabla();

    return 0;
}