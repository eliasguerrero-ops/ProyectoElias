
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    printf("\n--- CODIGO SML GENERADO ---\n");

    for (i = 0; i < contadorCodigo; i++)
    {
        printf("%02d %+05d\n", i, codigo[i]);
        fprintf(salida, "%+05d\n", codigo[i]);
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