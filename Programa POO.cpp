/* Programación Orientada a Objetos */

/* Semestre Agosto - Diciembre 2024 */ 

/* Mantenimiento a un Catálogo de Datos y Foros */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Estructura para representar un Producto o Servicio */
typedef struct {
    char Clave[8];  // 6 caracteres + terminador nulo
    char Nombre[50];
    char Descripcion[100];
    char Medida[10];  // Chico, Mediano, Grande
    float Costo;
    char Disponibilidad;  // 'S' (Sí) o 'N' (No)
} Producto;

int Encontrado = 0;

/* Prototipos de las funciones */
void VerificarYCrearArchivo();
int ValidarOpcion(int *Opcion);
void IngresarClave(Producto *producto);
void IngresarNombre(Producto *producto);
void IngresarDimension(Producto *producto);
void IngresarCosto(float *Costo);
char IngresarDisponibilidad();
int ValidarClave(const char *Clave);
int ValidarNombre(const char *Nombre);
int ValidarDimension(const char *Medida);
int ValidarCosto(const char *CostoSt);
int ValidarDisponibilidad(char Disponibilidad);
void AltaDeProducto(Producto *Productos, int *NumerodeRegistros);
int ClaveExistenteEnArreglo(const Producto *Productos, int NumerodeRegistros, const char *clave);
void BajaDeProductoServicio();
void RealizarBaja(FILE *GuardarArchivo, FILE *BajaArchivo, const char *ClaveEliminar);
void CerrarArchivos(FILE *GuardarArchivo, FILE *BajaArchivo);
void ManejarResultado(const char *ClaveEliminar);
void CambioDeProductoServicio();
int ProcesarCambio(FILE *GuardarArchivo, FILE *TemporalArchivo, const char *ClaveModificar);
int RealizarCambio(const char *ClaveModificar);
void ModificarProducto(FILE *TemporalArchivo, char *Linea);
void MostrarProductosServicios();
void GuardarDatosEnArchivo(Producto *Productos, int NumerodeRegistros);
void IngresarNuevoNombre(char *Dato, const char *mensaje);
void IngresarNuevoNombre(char *Dato, const char *mensaje);
void IngresarNuevaDescripcion(char *descripcion, const char *mensaje);
void IngresarNuevaDimension(char *dimension, const char *mensaje);
void IngresarNuevoCosto(float *costo, const char *mensaje);
void IngresarNuevaDisponibilidad(char *disponibilidad, const char *mensaje);
void OrdenarRegistrosPorNombre();
int CompararPorNombre(const void *a, const void *b);


int main() {
    /* Declaración de variables y estructuras */
    int Opcion;

    /* Llamar a la función para verificar y crear el archivo */
    VerificarYCrearArchivo();

    /* Declarar un arreglo de Producto para almacenar los registros */
    Producto Productos[100]; // Se Puedes ajustar el tamaño
    int NumerodeRegistros = 0; // Contador de registros

    /* Menú de Opciones */
    while (1) {
        printf("\nCat%clogo de datos (PRODUCTOS Y SERVICIOS):\n", 160);
        printf("Este programa funciona como una base de datos y para comenzar seleccione una opci%cn, recuerde utilizar may%csculas y seguir las instrucciones:\n\n", 162, 163);
        printf("1. Alta de Producto o Servicio\n");
        printf("2. Baja de Producto o Servicio\n");
        printf("3. Cambio de Producto o Servicio\n");
        printf("4. Consulta de Producto o Servicio\n");
        printf("5. Reporte en archivo\n");
        printf("6. Salir\n");
        printf("Elija una opci%cn: ", 162);

        /* Llamada a la función de validación de la opción */
        if (!ValidarOpcion(&Opcion)) {
            continue; // Regresar al inicio del bucle
        }

        switch (Opcion) {
            case 1: /* Alta de Producto */
                AltaDeProducto(Productos, &NumerodeRegistros);
                printf("\nPara confirmar que quiere ingresar esos datos al archivo seleccione el case 5 una vez haya terminado de dar de alta todos los productos o servicios\n");
                break;

            case 2: /* Baja de Producto */
                BajaDeProductoServicio();
                break;

            case 3: /* Cambio de Producto */
                CambioDeProductoServicio();
                OrdenarRegistrosPorNombre();
                break;

            case 4: /* Consulta de Producto o Servicio en monitor (es decir en el cmd) */
                MostrarProductosServicios();
                break;

            case 5: /* Guardar datos en el archivo de texto */
                GuardarDatosEnArchivo(Productos, NumerodeRegistros);
                OrdenarRegistrosPorNombre();

                /* Reiniciar el contador de registros a cero para eliminar los productos de la memoria */
                NumerodeRegistros = 0;
                break;

            case 6: /* Salir */
                exit(0);

            default:
                printf("Opci%cn no v%clida. Intente de nuevo.\n", 162, 160);
        }
    }
    return 0;
}

	
// Función para verificar y crear el archivo "productos.txt" si no existe
void VerificarYCrearArchivo() {
    FILE *VerificacionArchivo = fopen("productos.txt", "r");

    if (VerificacionArchivo == NULL) {
        FILE *CreacionArchivo = fopen("productos.txt", "w");

        if (CreacionArchivo == NULL) {
            printf("Error al crear el archivo 'productos.txt'.\n");
            exit(1); // Terminar el programa con código de error
        }

        fclose(CreacionArchivo);
    } else {
        fclose(VerificacionArchivo);
    }
}

// Implementación de la función de validación de la opción
int ValidarOpcion(int *Opcion) {
    int c;

    if (scanf("%d", Opcion) != 1) {
        // Limpiar el búfer de entrada
        while ((c = getchar()) != '\n' && c != EOF);
        printf("Opci%cn no v%clida. Debe seleccionar un n%cmero del 1 al 6.\n", 162, 160, 163);
        return 0; // Opción no válida
    }

    if (*Opcion < 1 || *Opcion > 6) {
    	// Limpiar el búfer de entrada
        while ((c = getchar()) != '\n' && c != EOF);
        printf("Opci%cn no v%clida. Debe seleccionar un n%cmero del 1 al 6.\n", 162, 160, 163);
        return 0; // Opción no válida
    }

    return 1; // Opción válida
}

// Función para ingresar la Clave del Producto
void IngresarClave(Producto *producto) {
    int ValidacionExitosa = 1;
    int ClaveExistente = 0;
    FILE *BajaArchivo;

    do {
        ValidacionExitosa = 1;
        ClaveExistente = 0;
        printf("Clave del producto o servicio (debe ser de la forma '#XXXXXX' con 6 caracteres alfan%cmericos y '#'): ", 130);
        scanf("%s", producto->Clave);

        if (!ValidarClave(producto->Clave)) {
            printf("Clave no v%clida. Debe ser de la forma '#XXXXXX' con 6 caracteres alfan%cmericos y no olvide el '#'. Intente de nuevo.\n", 160, 130);
            ValidacionExitosa = 0;
        } else {
            BajaArchivo = fopen("productos.txt", "r");
            if (BajaArchivo == NULL) {
                printf("Error al abrir el archivo.\n");
                break;
            }

            char Linea[256];

            while (fgets(Linea, sizeof(Linea), BajaArchivo) != NULL) {
                if (strstr(Linea, producto->Clave) != NULL) {
                    printf("La clave del producto o servicio ya existe en el archivo. Intente con una clave diferente.\n");
                    fclose(BajaArchivo);
                    ClaveExistente = 1;
                    ValidacionExitosa = 0;
                    break;
                }
            }

            fclose(BajaArchivo);
        }
    } while (!ValidacionExitosa);
}

// Función para validar la clave del producto o servicio
int ValidarClave(const char *Clave) {
    // Verificar la longitud de la clave
    if (strlen(Clave) != 7) {
        return 0;  // Clave no válida
    }

    // Verificar el primer carácter
    if (Clave[0] != '#') {
        return 0;  // Clave no válida
    }

    // Verificar los caracteres restantes
    for (int i = 1; i < 7; i++) {
        if (!isalnum(Clave[i]) && !isupper(Clave[i])) {
            return 0;  // Clave no válida
        }
    }

    // Si todas las condiciones se cumplen, la clave es válida
    return 1;
}

// Función para ingresar el nombre del producto o servicio
void IngresarNombre(Producto *producto) {
    int ValidacionExitosa = 1;

    do {
        ValidacionExitosa = 1;
        printf("Nombre del producto o servicio (solo caracteres de texto): ");
        scanf(" %49[^\n]", producto->Nombre);
        if (!ValidarNombre(producto->Nombre)) {
            printf("Nombre no v%clido (solo caracteres de texto). Intente de nuevo.\n", 160);
            ValidacionExitosa = 0;
        }
    } while (!ValidacionExitosa);
}

// Función para validar el Nombre (solo caracteres de texto)
int ValidarNombre(const char *Nombre) {
    int longitud = strlen(Nombre);
    for (int i = 0; i < longitud; i++) {
        if (!isalpha(Nombre[i]) && !isspace(Nombre[i])) {
            return 0;  // Nombre no válido
        }
    }
    
    // Si todas las condiciones se cumplen, el nombre es válido
    return 1;
}
	
// Función para ingresar la dimensión del producto o servicio
void IngresarDimension(Producto *producto) {
    int ValidacionExitosa = 1;

    do {
        ValidacionExitosa = 1;
        printf("Dimensi%cn del producto o servicio (Chico, Mediano, Grande, N/A): ", 162);
        scanf(" %9s", producto->Medida);
        if (!ValidarDimension(producto->Medida)) {
            printf("Dimensi%cn no v%clida, debe ser Chico, Mediano, Grande, N/A. Intente de nuevo.\n", 162, 160);
            ValidacionExitosa = 0;
        }
    } while (!ValidacionExitosa);
}

// Función para validar la dimensión (Chico, Mediano, Grande)
int ValidarDimension(const char *Medida) {
    if (strcmp(Medida, "Chico") == 0 || strcmp(Medida, "Mediano") == 0 || strcmp(Medida, "Grande") == 0 || strcmp(Medida, "N/A") == 0) {
        return 1;  // Dimensión válida
    }

    return 0;  // Dimensión no válida
}
	
// Función para ingresar el costo del producto o servicio
void IngresarCosto(float *Costo) {
    char CostoSt[100];
    int CostoValido = 0;

    while (!CostoValido) {
        printf("Costo del Producto o Servicio: $");
        if (scanf("%s", CostoSt) != 1 || !ValidarCosto(CostoSt)) {
            printf("Costo no v%clido. Debe ser un n%cmero decimal positivo. Intente de nuevo.\n", 160, 163);
            CostoValido = 0;
            while (getchar() != '\n'); // Limpia el búfer de entrada
        } else {
            CostoValido = 1;
            *Costo = atof(CostoSt);
        }
    }
}
	
// Función para validar el costo
int ValidarCosto(const char *CostoSt) {
    int DotCount = 0;  // Contador de puntos decimales
    int DigitCount = 0;  // Contador de dígitos

    if (strlen(CostoSt) == 0) {
        return 0; // La cadena está vacía, no es válida
    }

    for (int i = 0; i < strlen(CostoSt); i++) {
        if (!isdigit(CostoSt[i])) {
            if (CostoSt[i] == '.') {
                DotCount++;
                if (DotCount > 1) {
                    return 0; // Más de un punto decimal, no es válido
                }
            } else {
                return 0; // Caracter no válido
            }
        } else {
            DigitCount++;
        }
    }

    // Verificar si hay al menos un dígito y si el valor es positivo
    if (DigitCount == 0 || atof(CostoSt) <= 0) {
        return 0; // No es un número decimal positivo
    }

    return 1; // La entrada es un costo válido
}
	
// Función para ingresar la disponibilidad del producto o servicio 
char IngresarDisponibilidad() {
    char disponibilidad;
    int ValidacionExitosa = 0;
    int c;

    do {
        ValidacionExitosa = 1;
        printf("Disponibilidad del producto o servicio (Seleccione 'S' para Si / Seleccione 'N' para No): ");
        scanf(" %c", &disponibilidad);
        if (!ValidarDisponibilidad(disponibilidad)) {
            printf("Disponibilidad no v%clida. Debe ser 'S' o 'N'. Intente de nuevo.\n", 160);
            ValidacionExitosa = 0;
        }

        // Limpiar el búfer de entrada
        while ((c = getchar()) != '\n' && c != EOF);
    } while (!ValidacionExitosa);

    return disponibilidad;
}

// Función para validar la Disponibilidad (Sí o No)
int ValidarDisponibilidad(char Disponibilidad) {
    if (Disponibilidad != 'S' && Disponibilidad != 'N') {
        return 0;  // Disponibilidad no válida
    }
    return 1;  // Disponibilidad válida
}

// Función para el "Case 1: Alta de Producto"
void AltaDeProducto(Producto *Productos, int *NumerodeRegistros) {
    Producto NuevoProducto;
    printf("Ingrese los datos del Producto o Servicio:\n");

    // Llamada a la función para ingresar la claves
    IngresarClave(&NuevoProducto);

    // Verificar si la clave ya existe en el arreglo
    if (ClaveExistenteEnArreglo(Productos, *NumerodeRegistros, NuevoProducto.Clave)) {
        printf("La clave del producto o servicio ya existe en el arreglo. Intente con una clave diferente.\n");
        return;
    }

    // Llamada a la función para ingresar el nombre
    IngresarNombre(&NuevoProducto);

    // Solicitar y validar la descripción del Producto
    printf("Descripci%cn del Producto o Servicio (hasta 99 caracteres): ", 162);
    scanf(" %99[^\n]", NuevoProducto.Descripcion);

    // Llamada a la función para ingresar y validar la dimensión
    IngresarDimension(&NuevoProducto);

    // Llamada a la función para ingresar y validar el costo
    IngresarCosto(&NuevoProducto.Costo);

    // Llamar a la función para ingresar y validar la disponibilidad
    NuevoProducto.Disponibilidad = IngresarDisponibilidad();

    // Agregar el Producto al arreglo y aumentar el contador
    if (*NumerodeRegistros < 100) {
        Productos[*NumerodeRegistros] = NuevoProducto;
        (*NumerodeRegistros)++;
        printf("Producto agregado correctamente.\n");
    } else {
        printf("No se pueden agregar más productos, el arreglo está lleno.\n");
    }
}

// Función para verificar si la clave ya existe en el arreglo
int ClaveExistenteEnArreglo(const Producto *Productos, int NumerodeRegistros, const char *clave) {
    for (int i = 0; i < NumerodeRegistros; i++) {
        if (strcmp(Productos[i].Clave, clave) == 0) {
            return 1; // La clave ya existe en el arreglo
        }
    }
    return 0; // La clave no existe en el arreglo
}


// Función para realizar la "Baja de Producto o Servicio" (Case 2)
void BajaDeProductoServicio() {
    char ClaveEliminar[8];
    printf("Ingrese la clave del producto o servicio a eliminar (debe ser de la forma '#XXXXXX' con 6 caracteres alfanum%cricos y su '#'): ", 130);
    scanf("%s", ClaveEliminar);
    if (!ValidarClave(ClaveEliminar)) {
        printf("clave no v%clida. Debe ser de la forma '#XXXXXX' con 6 caracteres alfanum%cricos y no olvide el '#'. Intente de nuevo.\n", 160, 130);
        return;
    }

    FILE *GuardarArchivo = fopen("productos.txt", "r");
    if (GuardarArchivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    FILE *BajaArchivo = fopen("temp_productos.txt", "w");
    if (BajaArchivo == NULL) {
        printf("Error al abrir el archivo temporal.\n");
        fclose(GuardarArchivo);
        return;
    }

    RealizarBaja(GuardarArchivo, BajaArchivo, ClaveEliminar);
    CerrarArchivos(GuardarArchivo, BajaArchivo);
    ManejarResultado(ClaveEliminar);
}

// Función para realizar la operación de baja
void RealizarBaja(FILE *GuardarArchivo, FILE *BajaArchivo, const char *ClaveEliminar) {
    char Linea[256];
    Encontrado = 0; // Reiniciar el estado de Encontrado antes de iniciar la búsqueda

    while (fgets(Linea, sizeof(Linea), GuardarArchivo) != NULL) {
        if (strstr(Linea, ClaveEliminar) != NULL) {
            Encontrado = 1; // Se ha Encontrado la clave a eliminar
        } else {
            fputs(Linea, BajaArchivo);
        }
    }
}

// Función para cerrar los archivos
void CerrarArchivos(FILE *GuardarArchivo, FILE *BajaArchivo) {
    fclose(GuardarArchivo);
    fclose(BajaArchivo);
}

// Función para manejar el resultado de la operación de baja
void ManejarResultado(const char *ClaveEliminar) {
    if (Encontrado) {
        remove("productos.txt");
        rename("temp_productos.txt", "productos.txt");
        printf("El producto o servicio con clave %s ha sido eliminado.\n", ClaveEliminar);
    } else {
        printf("No se encontr%c un producto o servicio con la clave %s.\n", 162, ClaveEliminar);
        remove("temp_productos.txt"); // Eliminar el archivo temporal si no se encontró la clave
    }
}

// Función para realizar el "Cambio de Producto o Servicio" (Case 3)
void CambioDeProductoServicio() {
    char ClaveModificar[8];
    printf("Ingrese la clave del producto o servicio a modificar: ");
    scanf("%s", ClaveModificar);

    if (!ValidarClave(ClaveModificar)) {
        printf("Clave no v%clida. Intente de nuevo.\n", 160);
        return;
    }

    if (RealizarCambio(ClaveModificar)) {
        printf("Los datos del producto o servicio con clave %s han sido modificados.\n", ClaveModificar);
    } else {
        printf("No se encontr%c un producto o servicio con la clave %s.\n", 162, ClaveModificar);
    }
}

// Función principal para realizar el cambio en el archivo
int RealizarCambio(const char *ClaveModificar) {
    // Abrir el archivo original y un archivo temporal
    FILE *GuardarArchivo = fopen("productos.txt", "r");
    FILE *TemporalArchivo = fopen("temp_productos.txt", "w");

    // Verificar la apertura de archivos
    if (GuardarArchivo == NULL || TemporalArchivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return 0;
    }

    // Variable para indicar si se encontró la clave a modificar
    int Encontrado = 0;

    // Procesar el cambio en el archivo
    Encontrado = ProcesarCambio(GuardarArchivo, TemporalArchivo, ClaveModificar);

    // Cerrar los archivos
    fclose(GuardarArchivo);
    fclose(TemporalArchivo);

    // Si se encontró la clave, reemplazar el archivo original con el temporal
    if (Encontrado) {
        remove("productos.txt");
        rename("temp_productos.txt", "productos.txt");
    }

    // Devolver si se encontró o no la clave
    return Encontrado;
}

// Función para procesar el cambio en el archivo
int ProcesarCambio(FILE *GuardarArchivo, FILE *TemporalArchivo, const char *ClaveModificar) {
    char Linea[256];
    int Encontrado = 0;

    // Recorrer cada línea del archivo original
    while (fgets(Linea, sizeof(Linea), GuardarArchivo) != NULL) {
        // Verificar si la línea contiene la clave a modificar
        if (strstr(Linea, ClaveModificar) != NULL) {
            Encontrado = 1;
            // Modificar el producto y escribir en el archivo temporal
            ModificarProducto(TemporalArchivo, Linea);
        } else {
            // Si no es la línea a modificar, escribir en el archivo temporal
            fputs(Linea, TemporalArchivo);
        }
    }

    // Devolver si se encontró o no la clave
    return Encontrado;
}

// Función para modificar un producto
void ModificarProducto(FILE *TemporalArchivo, char *Linea) {
    // Definir una estructura para almacenar los datos del producto
    Producto Producto;

    // Extraer los datos del producto de la línea
    sscanf(Linea, "%s | %19[^|] | %99[^|] | %9[^|] | $%f | %c",
           Producto.Clave, Producto.Nombre, Producto.Descripcion, Producto.Medida, &Producto.Costo, &Producto.Disponibilidad);

    // Almacenar la descripción, medida, costo y disponibilidad originales
    char DescripcionOriginal[100];
    strncpy(DescripcionOriginal, Producto.Descripcion, sizeof(Producto.Descripcion));

    char MedidaOriginal[10];
    strncpy(MedidaOriginal, Producto.Medida, sizeof(Producto.Medida));
    
    float CostoOriginal = Producto.Costo;

    char DisponibilidadOriginal;
    DisponibilidadOriginal = Producto.Disponibilidad;

    // Solicitar al usuario los nuevos datos del producto
    IngresarNuevoNombre(Producto.Nombre, "Nuevo nombre del producto o servicio (solo caracteres de texto)");
    IngresarNuevaDescripcion(Producto.Descripcion, "Nueva descripcion del producto o servicio (hasta 99 caracteres)");
    IngresarNuevaDimension(Producto.Medida, "Nueva dimension del producto o servicio");
    IngresarNuevoCosto(&Producto.Costo, "Nuevo costo del producto o servicio");
    IngresarNuevaDisponibilidad(&Producto.Disponibilidad, "Nueva disponibilidad del producto o servicio");

    // Restaurar la descripción, medida, costo y disponibilidad original si es necesario
    if (Producto.Descripcion[0] == '\0') {
        strncpy(Producto.Descripcion, DescripcionOriginal, sizeof(Producto.Descripcion));
    }
    if (Producto.Medida[0] == '\0') {
        strncpy(Producto.Medida, MedidaOriginal, sizeof(Producto.Medida));
    }
    if (Producto.Costo == 0.0) {
        Producto.Costo = CostoOriginal;
    }
    if (Producto.Disponibilidad == '\0') {
        Producto.Disponibilidad = DisponibilidadOriginal;
    }

    // Escribir el producto modificado en el archivo temporal
    fprintf(TemporalArchivo, "%-8s | %-20s | %-99s | %-10s | $%-14.2f | %c\n",
            Producto.Clave, Producto.Nombre, Producto.Descripcion, Producto.Medida, Producto.Costo, Producto.Disponibilidad);
}


// Función para mostrar los productos y servicios desde un archivo (Case 4)
void MostrarProductosServicios() {
	
    FILE *GuardarArchivo = fopen("productos.txt", "r"); // Abrir el archivo en modo lectura

    if (GuardarArchivo == NULL) {
        printf("Error al abrir el archivo.\n");
    } else {
        printf("Listado de Productos y Servicios:\n\n");

        // Leer y mostrar cada línea del archivo
        char Linea[256];
        while (fgets(Linea, sizeof(Linea), GuardarArchivo) != NULL) {
            printf("%s", Linea);
        }

        fclose(GuardarArchivo);
    }
}


// Función para guardar los datos en un archivo (case 5)
void GuardarDatosEnArchivo(Producto* Productos, int NumerodeRegistros) {
    // Abrir el archivo en modo apertura y escritura (append)
    FILE* GuardarArchivo = fopen("productos.txt", "a+");

    // Verificar si se pudo abrir el archivo
    if (GuardarArchivo == NULL) {
        printf("Error al abrir el archivo.\n");
    } else {
        // Colocar el puntero al final del archivo y verificar si el archivo está vacío
        fseek(GuardarArchivo, 0, SEEK_END);
        if (ftell(GuardarArchivo) == 0) {
            // Si el archivo está vacío, agregar encabezados al archivo
            fprintf(GuardarArchivo, "%-8s | %-20s | %-99s | %-10s | %-15s | %-15s\n", "Clave", "Nombre", "Descripción", "Medida", "Costo", "Disponibilidad");
        }

        // Iterar a través de los registros de productos
        for (int i = 0; i < NumerodeRegistros; i++) {
            char claveProducto[8];
            strcpy(claveProducto, Productos[i].Clave);

            // Abrir el archivo en modo lectura para verificar si la clave ya existe
            FILE* BajaArchivo = fopen("productos.txt", "r");
            if (BajaArchivo == NULL) {
                printf("Error al abrir el archivo.\n");
                break;
            }

            char Linea[256];
            int claveExisteEnArchivo = 0;

            // Verificar si la clave ya existe en el archivo
            while (fgets(Linea, sizeof(Linea), BajaArchivo) != NULL) {
                if (strstr(Linea, claveProducto) != NULL) {
                    claveExisteEnArchivo = 1;
                    break;
                }
            }

            // Cerrar el archivo después de la verificación
            fclose(BajaArchivo);

            // Si la clave no existe en el archivo, agregar el nuevo producto
            if (!claveExisteEnArchivo) {
                fprintf(GuardarArchivo, "%-8s | %-20s | %-99s | %-10s | $%-14.2f | %c\n",
                    Productos[i].Clave, Productos[i].Nombre, Productos[i].Descripcion, Productos[i].Medida, Productos[i].Costo, Productos[i].Disponibilidad);
            }
        }

        // Cerrar el archivo después de agregar los datos
        fclose(GuardarArchivo);

        // Imprimir mensaje de éxito
        printf("Datos guardados en el archivo correctamente.\n\n");
    }
}



// Función para ingresar el nuevo nombre
void IngresarNuevoNombre(char *Dato, const char *mensaje) {
    int EsTextoValido = 0;
    char Input[100];

    while (!EsTextoValido) {
        printf("%s ( o 'x' para mantener sin cambios): ", mensaje);
        fflush(stdin);
        fgets(Input, sizeof(Input), stdin);
        Input[strcspn(Input, "\n")] = '\0';

        // Verificar si la entrada es "x" o está vacía (se presionó Enter)
        if (Input[0] == '\0' || (strlen(Input) == 1 && Input[0] == 'x')) {
            // Mantener sin cambios (no se hace nada)
            EsTextoValido = 1;
        } else {
            // Validar que la entrada solo contenga caracteres de texto
            EsTextoValido = 1;
            for (int i = 0; i < strlen(Input); i++) {
                if (!isalpha(Input[i]) && !isspace(Input[i])) {
                    EsTextoValido = 0;
                    break;
                }
            }

            if (EsTextoValido) {
                // Copiar la entrada en el Dato
                strncpy(Dato, Input, sizeof(Input));
            } else {
                printf("Entrada no v%clida. Debe contener solo caracteres de texto o 'x' para mantener sin cambios. No se realiz%c ning%cn cambio.\n", 160, 162, 163);
            }
        }
    }
}

// Función para ingresar y validar la nueva descripción
void IngresarNuevaDescripcion(char *descripcion, const char *mensaje) {
    char Input[100];
    int ValidacionExitosa = 0;

    do {
        ValidacionExitosa = 1;
        printf("%s (o 'x' para mantener sin cambios): ", mensaje);
        fflush(stdin);
        fgets(Input, sizeof(Input), stdin);
        Input[strcspn(Input, "\n")] = '\0'; // Eliminar el salto de línea

        // Verificar si la entrada es "x" o está vacía (se presionó Enter)
        if (Input[0] == '\0' || (strlen(Input) == 1 && Input[0] == 'x')) {
            // Mantener sin cambios (no se hace nada)
        } else {
            // Copiar la entrada en la descripción
            strncpy(descripcion, Input, sizeof(Input) - 1);  // Copiar máximo sizeof(Input) - 1 caracteres
            descripcion[sizeof(Input) - 1] = '\0';  // Asegurar que la cadena esté terminada correctamente
        }
    } while (!ValidacionExitosa);
}




// Función para ingresar y validar la nueva dimensión
void IngresarNuevaDimension(char *dimension, const char *mensaje) {
    char Input[100];
    int ValidacionExitosa = 0;

    do {
        ValidacionExitosa = 1;
        printf("%s (Chico, Mediano, Grande, N/A o 'x' para mantener sin cambios): ", mensaje);
        fflush(stdin);
        fgets(Input, sizeof(Input), stdin);
        Input[strcspn(Input, "\n")] = '\0';  // Eliminar el carácter de nueva línea

        // Verificar si la entrada es "x" o está vacía (se presionó Enter)
        if (Input[0] == '\0' || (strlen(Input) == 1 && Input[0] == 'x')) {
            // Mantener sin cambios (no se hace nada)
        } else {
            // Validar que la entrada sea una dimensión válida
            if (strcmp(Input, "Chico") == 0 || strcmp(Input, "Mediano") == 0 || strcmp(Input, "Grande") == 0 || strcmp(Input, "N/A") == 0) {
                // Copiar la entrada en la dimensión sin incluir caracteres no deseados
                strncpy(dimension, Input, sizeof(dimension) - 1);  // Restar 1 para dejar espacio para el carácter nulo
            } else {
                printf("Entrada no v%clida. Debe ser 'Chico', 'Mediano', 'Grande', 'N/A' o 'x' para mantener sin cambios. No se realiz%c ning%cn cambio.\n", 160, 162, 163);
                ValidacionExitosa = 0;
            }
        }
    } while (!ValidacionExitosa);
}


// Función para ingresar y validar el nuevo costo
void IngresarNuevoCosto(float *costo, const char *mensaje) {
    char Input[100];
    int CostoValido;

    do {
        CostoValido = 1;  // Inicializar en 1 por defecto

        // Solicitar la entrada al usuario
        printf("%s ($ o 'x' para mantener sin cambios): ", mensaje);
        fflush(stdin);

        // Leer la entrada del usuario
        if (scanf("%s", Input) != 1) {
            printf("Entrada no v%clida. Debe ser un n%cmero decimal positivo o 'x' para mantener sin cambios. No se realiza ning%cn cambio.\n", 160, 163, 163);

            // Limpiar el búfer de entrada
            while (getchar() != '\n');
            CostoValido = 0;  // Establecer en 0 para repetir el bucle
        } else {
            // Verificar si la entrada es "x"
            if (strcmp(Input, "x") == 0) {
                CostoValido = 1;  // Mantener sin cambios
            } else if (!ValidarCosto(Input)) {
                // Validar si la entrada es un número decimal positivo
                printf("Costo no v%clido. Debe ser un n%cmero decimal positivo. No se realiza ning%cn cambio.\n", 160, 163, 163);

                // Limpiar el búfer de entrada
                while (getchar() != '\n');
                CostoValido = 0;  // Establecer en 0 para repetir el bucle
            } else {
                *costo = atof(Input);  // Convertir la entrada a flotante y asignarla al costo
            }
        }
    } while (!CostoValido);
}


// Función para ingresar y validar la nueva disponibilidad
void IngresarNuevaDisponibilidad(char *disponibilidad, const char *mensaje) {
    char Input[2];
    int ValidacionExitosa = 0;

    do {
        ValidacionExitosa = 1;
        printf("%s ('S' para Si / 'N' para No, o 'x' para mantener sin cambios): ", mensaje);
        fflush(stdin);
        fgets(Input, sizeof(Input), stdin);
        Input[strcspn(Input, "\n")] = '\0';

        // Verificar si la entrada es 'S', 'N', o 'x'
        if (strcmp(Input, "S") == 0 || strcmp(Input, "N") == 0 || strcmp(Input, "x") == 0) {
            if (strcmp(Input, "x") != 0) {
                // Copiar la entrada en la disponibilidad si no es 'x'
                strncpy(disponibilidad, Input, sizeof(Input));
            }
        } else {
            printf("Entrada no v%clida. Debe ser 'S', 'N', o 'x' para mantener sin cambios. No se realiza ning%cn cambio.\n", 160, 163);
            ValidacionExitosa = 0;
        }

        // Limpieza del búfer de entrada
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
            // Consumir caracteres no deseados en el búfer
        }

    } while (!ValidacionExitosa);
}


// Función para ordenar los registros por nombre
void OrdenarRegistrosPorNombre() {
    FILE* archivo = fopen("productos.txt", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    // Leer todos los registros en un arreglo
    Producto productos[100];
    int NumeroRegistros = 0;

    char Linea[256];
    fgets(Linea, sizeof(Linea), archivo); // Leer la primera línea de encabezados

    while (fgets(Linea, sizeof(Linea), archivo) != NULL) {
    sscanf(Linea, "%s | %19[^|] | %99[^|] | %9[^|] | $%f | %c",
	productos[NumeroRegistros].Clave, productos[NumeroRegistros].Nombre, productos[NumeroRegistros].Descripcion, productos[NumeroRegistros].Medida, &productos[NumeroRegistros].Costo, &productos[NumeroRegistros].Disponibilidad);

        NumeroRegistros++;
    }

    fclose(archivo);
	
	// Ordenar los registros por nombre usando la función de comparación
	qsort(productos, NumeroRegistros, sizeof(Producto), CompararPorNombre);

    // Escribir los registros ordenados de vuelta al archivo
    archivo = fopen("productos.txt", "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    fprintf(archivo, "%-8s | %-20s | %-99s | %-10s | %-15s | %-15s\n", "Clave", "Nombre", "Descripción", "Medida", "Costo", "Disponibilidad");

    for (int i = 0; i < NumeroRegistros; i++) {
        fprintf(archivo, "%-8s | %-20s | %-99s | %-10s | $%-14.2f | %c\n",
            productos[i].Clave, productos[i].Nombre, productos[i].Descripcion, productos[i].Medida, productos[i].Costo, productos[i].Disponibilidad);
    }
	
    fclose(archivo);

}

// Función de comparación para qsort
int CompararPorNombre(const void *a, const void *b) {
    return strcasecmp(((Producto *)a)->Nombre, ((Producto *)b)->Nombre);
}
