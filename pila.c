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

int prioridad(char operador)
{
    if (operador == '*' || operador == '/' || operador == '%')
    {
        return 2;
    }

    if (operador == '+' || operador == '-')
    {
        return 1;
    }

    return 0;
}

void convertirPostfijo(char expresion[], char resultado[])
{
    int i = 0;
    int j = 0;
    char caracter;

    cima = -1;

    while (expresion[i] != '\0' && expresion[i] != '\n')
    {
        caracter = expresion[i];

        // Acepta mayúsculas, minúsculas y números
        if ((caracter >= 'A' && caracter <= 'Z') || 
            (caracter >= 'a' && caracter <= 'z') || 
            (caracter >= '0' && caracter <= '9'))
        {
            resultado[j] = caracter;
            j++;
        }
        else if (caracter == '(')
        {
            push(caracter);
        }
        else if (caracter == ')')
        {
            while (cima != -1 && pila[cima] != '(')
            {
                resultado[j] = pop();
                j++;
            }

            if (cima != -1)
            {
                pop();
            }
        }
        else if (caracter == '+' || caracter == '-' ||
                 caracter == '*' || caracter == '/' || caracter == '%')
        {
            while (cima != -1 && pila[cima] != '(' &&
                   prioridad(pila[cima]) >= prioridad(caracter))
            {
                resultado[j] = pop();
                j++;
            }

            push(caracter);
        }

        i++;
    }

    while (cima != -1)
    {
        resultado[j] = pop();
        j++;
    }

    resultado[j] = '\0';
}

int main()
{
    char expresion[100];
    char resultado[100];
    int resultadoProfundidad;

    printf("Escribe una expresion: ");
    fgets(expresion, sizeof(expresion), stdin);

    resultadoProfundidad = profundidad(expresion);

    if (resultadoProfundidad == -1)
    {
        printf("Error: los parentesis no estan balanceados.\n");
    }
    else
    {
        convertirPostfijo(expresion, resultado);
        printf("Expresion postfija: %s\n", resultado);
        printf("Profundidad maxima: %d\n", resultadoProfundidad);
    }

    return 0;
}