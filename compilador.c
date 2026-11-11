#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pila.c"

typedef struct tableEntry
{
    int symbol;
    char type;
    int location;
    struct tableEntry *next;
} tableEntry;

tableEntry *symbolTable = NULL;
int siguienteUbicacion = 99;
int codigo[100];
int contadorCodigo = 0;
int memoriaSML[100];

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
int obtenerUbicacionOperando(char token[])
{
    tableEntry *encontrado;
    int numero;
    int i;

    /* Revisar si es una variable */
    if (strlen(token) == 1 &&
        ((token[0] >= 'a' && token[0] <= 'z') ||
         (token[0] >= 'A' && token[0] <= 'Z')))
    {
        encontrado = buscarSimbolo(token[0], 'V');

        if (encontrado == NULL)
        {
            if (siguienteUbicacion < contadorCodigo)
            {
                printf("Error: memoria llena.\n");
                return -1;
            }

            registrarSimbolo(token[0], 'V');
            encontrado = buscarSimbolo(token[0], 'V');
        }

        if (encontrado == NULL)
        {
            return -1;
        }

        return encontrado->location;
    }

    /* Revisar si es un numero entero */
    for (i = 0; token[i] != '\0'; i++)
    {
        if (token[i] < '0' || token[i] > '9')
        {
            printf("Error: operando no valido: %s\n", token);
            return -1;
        }
    }

    numero = atoi(token);

    encontrado = buscarSimbolo(numero, 'C');

    if (encontrado == NULL)
    {
        if (siguienteUbicacion < contadorCodigo)
        {
            printf("Error: memoria llena.\n");
            return -1;
        }

        registrarSimbolo(numero, 'C');
        encontrado = buscarSimbolo(numero, 'C');
    }

    if (encontrado == NULL)
    {
        return -1;
    }

    memoriaSML[encontrado->location] = numero;

    return encontrado->location;
}
int generarLet(char expresion[], char variableDestino)
{
    char postfijo[100];
    char token[20];
    int pilaUbicaciones[100];
    int cimaUbicaciones = -1;
    int ubicacion;
    int izquierda;
    int derecha;
    int temporal;
    int i = 0;
    int j;
    char operador;

    tableEntry *destino;

    /* Buscar la variable donde se guardara el resultado */

    registrarSimbolo(variableDestino, 'V');

    destino = buscarSimbolo(variableDestino, 'V');

    if (destino == NULL)
    {
        return 0;
    }

    /* Convertir la expresion a postfijo */

    if (!validarExpresion(expresion))
    {
        printf("Error: expresion matematica no valida.\n");
        return 0;
    }

    convertirPostfijo(expresion, postfijo);

    printf("Expresion postfija: %s\n", postfijo);

    /* Recorrer la expresion postfija */

    while (postfijo[i] != '\0')
    {
        while (postfijo[i] == ' ')
        {
            i++;
        }

        if (postfijo[i] == '\0')
        {
            break;
        }

        j = 0;

        while (postfijo[i] != ' ' && postfijo[i] != '\0')
        {
            token[j] = postfijo[i];
            j++;
            i++;
        }

        token[j] = '\0';

        /* Revisar si el token es un operador */

        if (strlen(token) == 1 &&
            (token[0] == '+' || token[0] == '-' ||
             token[0] == '*' || token[0] == '/' ||
             token[0] == '%'))
        {
            operador = token[0];

            if (cimaUbicaciones < 1)
            {
                printf("Error en la expresion postfija.\n");
                return 0;
            }

            derecha = pilaUbicaciones[cimaUbicaciones];
            cimaUbicaciones--;

            izquierda = pilaUbicaciones[cimaUbicaciones];
            cimaUbicaciones--;

            /* Revisar espacio para instrucciones y temporal */

            if (contadorCodigo + 3 >= siguienteUbicacion)
            {
                printf("Error: memoria insuficiente para la operacion.\n");
                return 0;
            }

            temporal = siguienteUbicacion;
            siguienteUbicacion--;

            /* LOAD izquierda */

            codigo[contadorCodigo] = 2000 + izquierda;
            contadorCodigo++;

            /* Ejecutar operacion */

            if (operador == '+')
            {
                codigo[contadorCodigo] = 3000 + derecha;
            }
            else if (operador == '-')
            {
                codigo[contadorCodigo] = 3100 + derecha;
            }
            else if (operador == '/')
            {
                codigo[contadorCodigo] = 3200 + derecha;
            }
            else if (operador == '*')
            {
                codigo[contadorCodigo] = 3300 + derecha;
            }
            else
            {
                codigo[contadorCodigo] = 3400 + derecha;
            }

            contadorCodigo++;

            /* STORE temporal */

            codigo[contadorCodigo] = 2100 + temporal;
            contadorCodigo++;

            cimaUbicaciones++;
            pilaUbicaciones[cimaUbicaciones] = temporal;
        }
        else
        {
            ubicacion = obtenerUbicacionOperando(token);

            if (ubicacion == -1)
            {
                return 0;
            }

            cimaUbicaciones++;
            pilaUbicaciones[cimaUbicaciones] = ubicacion;
        }
    }

    /* Debe quedar un solo resultado */

    if (cimaUbicaciones != 0)
    {
        printf("Error al evaluar la expresion.\n");
        return 0;
    }

    /* Guardar el resultado en la variable destino */

    if (contadorCodigo + 2 >= siguienteUbicacion)
    {
        printf("Error: memoria insuficiente.\n");
        return 0;
    }

    codigo[contadorCodigo] = 2000 + pilaUbicaciones[0];
    contadorCodigo++;

    codigo[contadorCodigo] = 2100 + destino->location;
    contadorCodigo++;

    return 1;
}

void leerArchivo()
{
    FILE *archivo;
    char linea[100];

    archivo = fopen("programa.simple", "r");

    if (archivo == NULL)
    {
        printf("Error: no se pudo abrir programa.simple\n");
        return;
    }

    printf("--- CONTENIDO DEL PROGRAMA ---\n");

    while (fgets(linea, sizeof(linea), archivo) != NULL)
    {
        printf("%s", linea);
    }

    fclose(archivo);
}

void analizarArchivo()
{
    FILE *archivo;
    FILE *salida;
    char linea[100];
    char comando[20];
    char variable;
    int numeroLinea;
    int i;
    int destino;
    int referencias[100];
    int lineasDestino[100];
    tableEntry *entradaDestino;

    tableEntry *entradaLinea;
    tableEntry *entradaVariable;

    contadorCodigo = 0;
    for (i = 0; i < 100; i++)
{
    memoriaSML[i] = 0;
}
    for (i = 0; i < 100; i++)
{
    referencias[i] = -1;
    lineasDestino[i] = -1;
}

    archivo = fopen("programa.simple", "r");

    if (archivo == NULL)
    {
        printf("Error: no se pudo abrir programa.simple\n");
        return;
    }

    while (fgets(linea, sizeof(linea), archivo) != NULL)
    {
        if (sscanf(linea, "%d %19s", &numeroLinea, comando) != 2)
        {
            continue;
        }

        registrarSimbolo(numeroLinea, 'L');

        entradaLinea = buscarSimbolo(numeroLinea, 'L');

        if (entradaLinea != NULL)
        {
            entradaLinea->location = contadorCodigo;
        }

        if (strcmp(comando, "rem") == 0)
        {
            continue;
        }

        if (strcmp(comando, "input") == 0 ||
            strcmp(comando, "print") == 0)
        {
            if (sscanf(linea, "%d %19s %c",
                       &numeroLinea, comando, &variable) != 3)
            {
                printf("Error en la linea %d\n", numeroLinea);
                continue;
            }

            registrarSimbolo(variable, 'V');

            entradaVariable = buscarSimbolo(variable, 'V');

            if (entradaVariable == NULL)
            {
                printf("Error: no se encontro la variable.\n");
                continue;
            }

            if (contadorCodigo >= 100)
            {
                printf("Error: memoria de instrucciones llena.\n");
                break;
            }

            if (strcmp(comando, "input") == 0)
            {
                codigo[contadorCodigo] =
                    1000 + entradaVariable->location;
            }
            else
            {
                codigo[contadorCodigo] =
                    1100 + entradaVariable->location;
            }

            contadorCodigo++;
        }
else if (strcmp(comando, "goto") == 0)
{
    if (sscanf(linea, "%d %19s %d",
               &numeroLinea, comando, &destino) != 3)
    {
        printf("Error en el comando goto.\n");
        continue;
    }

    if (contadorCodigo >= 100)
    {
        printf("Error: memoria de instrucciones llena.\n");
        break;
    }

    
    codigo[contadorCodigo] = 4000;

    
    lineasDestino[contadorCodigo] = destino;


    referencias[contadorCodigo] = contadorCodigo;

    contadorCodigo++;
}
else if (strcmp(comando, "end") == 0)
{
    if (contadorCodigo >= 100)
    {
        printf("Error: memoria de instrucciones llena.\n");
        break;
    }

    codigo[contadorCodigo] = 4300;
    contadorCodigo++;
}
else if (strcmp(comando, "let") == 0)
{
    char variableDestino;
    char expresion[100];
    char *igual;

    /* Buscar el signo igual */

    igual = strchr(linea, '=');

    if (igual == NULL)
    {
        printf("Error: falta el signo igual en let.\n");
        continue;
    }

    /* Obtener la variable destino */

    if (sscanf(linea, "%d %19s %c",
               &numeroLinea, comando, &variableDestino) != 3)
    {
        printf("Error en la variable destino.\n");
        continue;
    }

    /* Copiar la expresion despues del signo igual */

    strcpy(expresion, igual + 1);

    printf("\nProcesando LET: %c = %s",
           variableDestino, expresion);

    if (!generarLet(expresion, variableDestino))
    {
        printf("Error al generar la instruccion LET.\n");
    }
}
else
{
    printf("Comando aun no implementado: %s\n", comando);
}
    }

    fclose(archivo);
    /* SEGUNDA PASADA: resolver referencias goto */

printf("\n--- RESOLVIENDO SALTOS ---\n");

for (i = 0; i < contadorCodigo; i++)
{
    if (referencias[i] != -1)
    {
        entradaDestino = buscarSimbolo(lineasDestino[i], 'L');

        if (entradaDestino != NULL)
        {
            codigo[i] = 4000 + entradaDestino->location;

            printf("Salto a linea %d resuelto: posicion %d\n",
                   lineasDestino[i],
                   entradaDestino->location);
        }
        else
        {
            printf("Error: la linea destino %d no existe.\n",
                   lineasDestino[i]);
            return;
        }
    }
}

    salida = fopen("programa.sml", "w");

    if (salida == NULL)
    {
        printf("Error: no se pudo crear programa.sml\n");
        return;
    }

/* Copiar las instrucciones a la memoria completa */

for (i = 0; i < contadorCodigo; i++)
{
    memoriaSML[i] = codigo[i];
}

/* Mostrar solamente las instrucciones */

printf("\n--- CODIGO SML GENERADO ---\n");

for (i = 0; i < contadorCodigo; i++)
{
    printf("%02d %+05d\n", i, memoriaSML[i]);
}

/* Escribir las 100 posiciones de memoria */

for (i = 0; i < 100; i++)
{
    fprintf(salida, "%+05d\n", memoriaSML[i]);
}

    fclose(salida);

    printf("\nArchivo programa.sml creado correctamente.\n");

    mostrarTabla();
}

int main()
{
    analizarArchivo();

    return 0;
}