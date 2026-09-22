#include <stdio.h>
#include <string.h>

int main()
{
    int memory[1000];

    int accumulator = 0;
    int instructionCounter = 0;
    int operationCode = 0;
    int operand = 0;
    int instructionRegister = 0;

    int numero;
    int i;
    int ejecutando = 1;

    FILE *archivo;
    char linea[100];
    int archivoExiste = 0;

    /* Inicializar memoria */
    for (i = 0; i < 1000; i++)
    {
        memory[i] = 0;
    }

    /* Mensaje de bienvenida */
    printf("*** Bienvenido a Simpletron! ***\n");
    printf("*** Introduzca su programa una instruccion ***\n");
    printf("*** (o palabra de datos) a la vez en la linea ***\n");
    printf("*** de texto de entrada. Yo indicare el numero ***\n");
    printf("*** de posicion y una interrogacion (?). Usted ***\n");
    printf("*** tecleara entonces la palabra para esa ***\n");
    printf("*** posicion. Introduzca 9999 para terminar. ***\n\n");

    /* ============================= */
    /* CARGAR EL PROGRAMA            */
    /* ============================= */
i = 0;

archivo = fopen("programa.simp", "r");

if (archivo != NULL)
{
     archivoExiste = 1;
    printf("*** Se encontro programa.simp ***\n");
    printf("*** Cargando programa desde archivo ***\n\n");

    while (i < 1000 && fgets(linea, 100, archivo) != NULL)
    {
        if (sscanf(linea, "%d", &numero) != 1)
        {
            printf("\n*** ERROR ***\n");
            printf("La linea %d del archivo no es valida.\n", i);

            fclose(archivo);
            return 1;
        }

        if (numero == 9999)
        {
            break;
        }

        if (numero < -9999 || numero > 99999)
        {
            printf("\n*** ERROR ***\n");
            printf("El numero de la linea %d no es valido.\n", i);

            fclose(archivo);
            return 1;
        }

        memory[i] = numero;

        i++;
    }

    fclose(archivo);
}
else
{
    printf("*** No se encontro programa.simp ***\n");
    printf("*** Se utilizara la carga manual ***\n\n");

    while (i < 1000)
    {
        printf("%03d ? ", i);
        scanf("%d", &numero);

        if (numero == 9999)
        {
            break;
        }

        while (numero < -9999 || numero > 99999)
        {
            printf("Numero invalido.\n");
            printf("Introduzca un numero valido: ");
            scanf("%d", &numero);
        }

        memory[i] = numero;

        i++;
    }
}
    printf("*** Comienza la ejecucion del programa ***\n\n");


    /* ============================= */
    /* EJECUTAR EL PROGRAMA          */
    /* ============================= */

    while (ejecutando == 1)
    {
        /*
         * Traer la siguiente instruccion
         * desde memoria.
         */
        instructionRegister = memory[instructionCounter];

        /*
         * Separar la instruccion:
         *
         * 1009
         *  |
         *  +-- 10 = codigo de operacion
         *      09 = operando
         */
        operationCode = instructionRegister / 1000;
        operand = instructionRegister % 1000;


        /* ============================= */
        /* REVISAR INSTRUCCION            */
        /* ============================= */

        if (instructionRegister < 0 ||
            instructionRegister > 99999)
        {
            printf("\n*** ERROR FATAL ***\n");
            printf("La instruccion no es valida.\n");

            ejecutando = 0;
            break;
        }


        /* ============================= */
        /* EJECUTAR INSTRUCCION           */
        /* ============================= */

        switch (operationCode)
        {

            /* ------------------------- */
            /* 10 - LEER                 */
            /* ------------------------- */

            case 10:

                printf("? ");
                scanf("%d", &numero);

                while (numero < -9999 || numero > 9999)
                {
                    printf("Numero invalido.\n");
                    printf("? ");
                    scanf("%d", &numero);
                }

                memory[operand] = numero;

                instructionCounter++;

                break;


            /* ------------------------- */
            /* 11 - ESCRIBIR             */
            /* ------------------------- */

            case 11:

                printf("%d\n", memory[operand]);

                instructionCounter++;

                break;


            /* ------------------------- */
            /* 20 - CARGAR               */
            /* ------------------------- */

            case 20:

                accumulator = memory[operand];

                instructionCounter++;

                break;


            /* ------------------------- */
            /* 21 - ALMACENAR            */
            /* ------------------------- */

            case 21:

                memory[operand] = accumulator;

                instructionCounter++;

                break;


            /* ------------------------- */
            /* 30 - SUMAR                */
            /* ------------------------- */

            case 30:

                accumulator = accumulator + memory[operand];

                if (accumulator > 9999 || accumulator < -9999)
                {
                    printf("\n*** ERROR FATAL ***\n");
                    printf("Desbordamiento del acumulador.\n");

                    ejecutando = 0;
                }
                else
                {
                    instructionCounter++;
                }

                break;


            /* ------------------------- */
            /* 31 - RESTAR               */
            /* ------------------------- */

            case 31:

                accumulator = accumulator - memory[operand];

                if (accumulator > 9999 || accumulator < -9999)
                {
                    printf("\n*** ERROR FATAL ***\n");
                    printf("Desbordamiento del acumulador.\n");

                    ejecutando = 0;
                }
                else
                {
                    instructionCounter++;
                }

                break;


            /* ------------------------- */
            /* 32 - DIVIDIR              */
            /* ------------------------- */

            case 32:

                if (memory[operand] == 0)
                {
                    printf("\n*** ERROR FATAL ***\n");
                    printf("No se puede dividir entre cero.\n");

                    ejecutando = 0;
                }
                else
                {
                    accumulator = accumulator / memory[operand];

                    instructionCounter++;
                }

                break;


            /* ------------------------- */
            /* 33 - MULTIPLICAR          */
            /* ------------------------- */

case 33:

    accumulator = accumulator * memory[operand];

    instructionCounter++;

    break;


/* ------------------------- */
/* 34 - MODULO               */
/* ------------------------- */

case 34:

    if (memory[operand] == 0)
    {
        printf("\n*** ERROR FATAL ***\n");
        printf("No se puede calcular modulo entre cero.\n");

        ejecutando = 0;
    }
    else
    {
        accumulator = accumulator % memory[operand];

        instructionCounter++;
    }

    break;


/* ------------------------- */
/* 40 - BRANCH               */
/* ------------------------- */

case 40:

                instructionCounter = operand;

                break;


            /* ------------------------- */
            /* 41 - BIFURCAR SI CERO     */
            /* ------------------------- */

            case 41:

                if (accumulator == 0)
                {
                    instructionCounter = operand;
                }
                else
                {
                    instructionCounter++;
                }

                break;


            /* ------------------------- */
            /* 42 - BIFURCAR SI NEGATIVO */
            /* ------------------------- */

            case 42:

                if (accumulator < 0)
                {
                    instructionCounter = operand;
                }
                else
                {
                    instructionCounter++;
                }

                break;


            /* ------------------------- */
            /* 43 - ALTO                 */
            /* ------------------------- */

            case 43:

                printf("\n*** Termino la ejecucion de Simpletron ***\n");

                ejecutando = 0;

                break;


            /* ------------------------- */
            /* CODIGO INVALIDO           */
            /* ------------------------- */

            default:

                printf("\n*** ERROR FATAL ***\n");
                printf("Codigo de operacion invalido: %d\n",
                       operationCode);

                ejecutando = 0;

                break;
        }
    }


    /* ============================= */
    /* REGISTROS                     */
    /* ============================= */

    printf("\n");
    printf("========== REGISTROS ==========\n");

    printf("Acumulador: %d\n", accumulator);

    printf("Contador de instrucciones: %d\n",
           instructionCounter);

    printf("Registro de instrucciones: %d\n",
           instructionRegister);

    printf("Codigo de operacion: %d\n",
           operationCode);

    printf("Operando: %d\n",
           operand);


    /* ============================= */
    /* VACIADO DE MEMORIA            */
    /* ============================= */

    printf("\n");
    printf("========== MEMORIA ==========\n");

    for (i = 0; i < 1000; i++)
    {
        printf("%03d : %+06d\n", i, memory[i]);
    }

    return 0;
}