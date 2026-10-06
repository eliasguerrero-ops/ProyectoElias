#include <stdio.h>

#define TAM_PILA 100

char pila[TAM_PILA];
int cima = -1;

void push(char elemento)
{
    cima++;
    pila[cima] = elemento;
}

int main()
{
    printf("Prueba de pila\n");

    push('A');
    push('B');
    push('C');

    printf("Elemento en la cima: %c\n", pila[cima]);

    return 0;
}