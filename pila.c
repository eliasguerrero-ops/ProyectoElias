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

int validarExpresion(char expresion[])
{
    int i = 0;
    int esperaOperando = 1;
    char caracter;

    while (expresion[i] != '\0' && expresion[i] != '\n')
    {
        caracter = expresion[i];

        if (caracter == ' ')
        {
            i++;
            continue;
        }

        /* Revisar numeros completos */
        if (caracter >= '0' && caracter <= '9')
        {
            if (esperaOperando == 0)
            {
                return 0;
            }

            while (expresion[i] >= '0' && expresion[i] <= '9')
            {
                i++;
            }

            esperaOperando = 0;
            continue;
        }

        /* Revisar variables de una letra */
        if ((caracter >= 'A' && caracter <= 'Z') ||
            (caracter >= 'a' && caracter <= 'z'))
        {
            if (esperaOperando == 0)
            {
                return 0;
            }

            i++;
            esperaOperando = 0;
            continue;
        }

        if (caracter == '(')
        {
            if (esperaOperando == 0)
            {
                return 0;
            }

            esperaOperando = 1;
        }
        else if (caracter == ')')
        {
            if (esperaOperando == 1)
            {
                return 0;
            }

            esperaOperando = 0;
        }
        else if (caracter == '+' || caracter == '-' ||
                 caracter == '*' || caracter == '/' ||
                 caracter == '%')
        {
            if (esperaOperando == 1)
            {
                return 0;
            }

            esperaOperando = 1;
        }
        else
        {
            return 0;
        }

        i++;
    }

    if (i == 0 || esperaOperando == 1)
    {
        return 0;
    }

    return 1;
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

        if (caracter == ' ')
        {
            i++;
            continue;
        }

        /* Copiar el numero completo */
        if (caracter >= '0' && caracter <= '9')
        {
            while (expresion[i] >= '0' && expresion[i] <= '9')
            {
                resultado[j] = expresion[i];
                j++;
                i++;
            }

            resultado[j] = ' ';
            j++;
            continue;
        }

        /* Copiar una variable */
        if ((caracter >= 'A' && caracter <= 'Z') ||
            (caracter >= 'a' && caracter <= 'z'))
        {
            resultado[j] = caracter;
            j++;
            resultado[j] = ' ';
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
                resultado[j] = ' ';
                j++;
            }

            if (cima != -1)
            {
                pop();
            }
        }
        else if (caracter == '+' || caracter == '-' ||
                 caracter == '*' || caracter == '/' ||
                 caracter == '%')
        {
            while (cima != -1 && pila[cima] != '(' &&
                   prioridad(pila[cima]) >= prioridad(caracter))
            {
                resultado[j] = pop();
                j++;
                resultado[j] = ' ';
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
        resultado[j] = ' ';
        j++;
    }

    /* Quitar el espacio final */
    if (j > 0 && resultado[j - 1] == ' ')
    {
        j--;
    }

    resultado[j] = '\0';
}


