#include <stdio.h>

#define TAM_PILA 100

char pila[TAM_PILA];
int cima = -1;

void push(char elemento)
{
    if (cima == TAM_PILA - 1)
    {
        printf("Error: la pila esta llena.\n");
    }
    else
    {
        cima++;
        pila[cima] = elemento;
    }
}

char pop()
{
    char elemento;

    if (cima == -1)
    {
        printf("Error: la pila esta vacia.\n");
        return '\0';
    }

    elemento = pila[cima];
    cima--;

    return elemento;
}

int profundidad(char expresion[])
{
    int actual = 0;
    int maxima = 0;
    int i;

    for (i = 0; expresion[i] != '\0'; i++)
    {
        if (expresion[i] == '(')
        {
            actual++;

            if (actual > maxima)
            {
                maxima = actual;
            }
        }
        else if (expresion[i] == ')')
        {
            actual--;

            if (actual < 0)
            {
                return -1;
            }
        }
    }

    if (actual != 0)
    {
        return -1;
    }

    return maxima;
}

int main()
{
    char expresion[100];
    int resultado;

    printf("Escribe una expresion: ");
    fgets(expresion, 100, stdin);

    resultado = profundidad(expresion);

    if (resultado == -1)
    {
        printf("Error: los parentesis no estan balanceados.\n");
    }
    else
    {
        printf("Profundidad maxima: %d\n", resultado);
    }

    return 0;
}