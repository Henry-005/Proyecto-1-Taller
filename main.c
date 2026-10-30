/*
 * Taller de programación imperativa en C
 * Compilar: gcc -std=c11 -Wall -Wextra -pedantic main.c -o taller
 * Ejecutar: ./taller
 */
#include <stdio.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Ejercicio 1: intercambio de valores                                */
/* ------------------------------------------------------------------ */
void swap(int *a, int *b)
{
    int tmp = *a;   /* tmp es local: solo existe dentro de swap */
    *a = *b;
    *b = tmp;
}

/* ------------------------------------------------------------------ */
/* Ejercicio 2: estadísticas de un arreglo (size >= 1)                */
/* ------------------------------------------------------------------ */
long long sumArray(const int *values, int size)
{
    long long total = 0;
    for (int i = 0; i < size; i++) {
        total += values[i];
    }
    return total;
}

int minArray(const int *values, int size)
{
    int min = values[0];
    for (int i = 1; i < size; i++) {
        if (values[i] < min) min = values[i];
    }
    return min;
}

int maxArray(const int *values, int size)
{
    int max = values[0];
    for (int i = 1; i < size; i++) {
        if (values[i] > max) max = values[i];
    }
    return max;
}

int countNegatives(const int *values, int size)
{
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (values[i] < 0) count++;
    }
    return count;
}

void printReverse(const int *values, int size)
{
    for (int i = size - 1; i >= 0; i--) {
        printf("%d%s", values[i], i > 0 ? " " : "");
    }
    printf("\n");
}

/* ------------------------------------------------------------------ */
/* Utilidad: imprimir arreglo                                         */
/* ------------------------------------------------------------------ */
static void printArray(const char *label, const int *values, int size)
{
    printf("%s[", label);
    for (int i = 0; i < size; i++) {
        printf("%d%s", values[i], i < size - 1 ? ", " : "");
    }
    printf("]\n");
}

/* ------------------------------------------------------------------ */
/* Ejercicio 3: copia independiente                                   */
/* ------------------------------------------------------------------ */
int *copyArray(const int *values, int size)
{
    if (size <= 0) return NULL;
    int *copy = malloc((size_t)size * sizeof *copy);
    if (copy == NULL) return NULL;
    for (int i = 0; i < size; i++) {
        copy[i] = values[i];
    }
    return copy;
}

/* ------------------------------------------------------------------ */
/* Ejercicio 4: ordenamiento por inserción (ascendente)               */
/* ------------------------------------------------------------------ */
void sortAscending(int *values, int size)
{
    for (int i = 1; i < size; i++) {
        int key = values[i];
        int j = i - 1;
        while (j >= 0 && values[j] > key) {
            values[j + 1] = values[j];
            j--;
        }
        values[j + 1] = key;
    }
}

/* ------------------------------------------------------------------ */
/* Ejercicio 5: dos valores que suman un objetivo (arreglo ordenado)  */
/* ------------------------------------------------------------------ */
int findPair(const int *values, int size, int target)
{
    if (size < 2) return 0;
    int left = 0;
    int right = size - 1;
    while (left < right) {
        long long sum = (long long)values[left] + values[right];
        if (sum == target) {
            printf("(%d, %d)\n", values[left], values[right]);
            return 1;
        }
        if (sum < target) left++;   /* necesitamos una suma mayor */
        else right--;               /* necesitamos una suma menor */
    }
    return 0;
}

/* ------------------------------------------------------------------ */
/* Ejercicio 6: 3Sum                                                  */
/* Devuelve cantidad de ternas, 0 si no hay, -1 si falla la reserva.  */
/* ------------------------------------------------------------------ */
int threeSum(const int *values, int size)
{
    if (size < 3) return 0;

    int *sorted = copyArray(values, size);
    if (sorted == NULL) return -1;
    sortAscending(sorted, size);

    int count = 0;
    for (int i = 0; i < size - 2; i++) {
        if (sorted[i] > 0) break;                          /* ya no puede sumar 0 */
        if (i > 0 && sorted[i] == sorted[i - 1]) continue; /* primer elemento repetido */

        int left = i + 1;
        int right = size - 1;
        while (left < right) {
            long long sum = (long long)sorted[i] + sorted[left] + sorted[right];
            if (sum < 0) {
                left++;
            } else if (sum > 0) {
                right--;
            } else {
                printf("(%d, %d, %d)\n", sorted[i], sorted[left], sorted[right]);
                count++;
                int a = sorted[left];
                int b = sorted[right];
                /* saltar valores repetidos en ambos lados */
                while (left < right && sorted[left] == a) left++;
                while (left < right && sorted[right] == b) right--;
            }
        }
    }

    free(sorted);
    return count;
}

/* ------------------------------------------------------------------ */
/* Ejercicio 7: máximos acumulados desde ambos extremos               */
/* ------------------------------------------------------------------ */
void buildLeftMax(const int *height, int size, int *leftMax)
{
    if (size <= 0) return;
    leftMax[0] = height[0];
    for (int i = 1; i < size; i++) {
        leftMax[i] = height[i] > leftMax[i - 1] ? height[i] : leftMax[i - 1];
    }
}

void buildRightMax(const int *height, int size, int *rightMax)
{
    if (size <= 0) return;
    rightMax[size - 1] = height[size - 1];
    for (int i = size - 2; i >= 0; i--) {
        rightMax[i] = height[i] > rightMax[i + 1] ? height[i] : rightMax[i + 1];
    }
}

/* ------------------------------------------------------------------ */
/* Ejercicio 8: Trapping Rain Water                                   */
/* Si verbose != 0 imprime alturas, máximos y agua por posición.      */
/* ------------------------------------------------------------------ */
static long long trappedWaterImpl(const int *height, int size, int verbose)
{
    if (size < 3) return 0;

    int *leftMax = malloc((size_t)size * sizeof *leftMax);
    int *rightMax = malloc((size_t)size * sizeof *rightMax);
    if (leftMax == NULL || rightMax == NULL) {
        free(leftMax);   /* free(NULL) es seguro */
        free(rightMax);
        return -1;
    }

    buildLeftMax(height, size, leftMax);
    buildRightMax(height, size, rightMax);

    long long total = 0;
    if (verbose) {
        printArray("  alturas  : ", height, size);
        printArray("  leftMax  : ", leftMax, size);
        printArray("  rightMax : ", rightMax, size);
        printf("  agua     : [");
    }
    for (int i = 0; i < size; i++) {
        int lower = leftMax[i] < rightMax[i] ? leftMax[i] : rightMax[i];
        int water = lower - height[i];
        total += water;
        if (verbose) printf("%d%s", water, i < size - 1 ? ", " : "");
    }
    if (verbose) printf("]\n");

    free(leftMax);
    free(rightMax);
    return total;
}

long long trappedWater(const int *height, int size)
{
    return trappedWaterImpl(height, size, 0);
}

/* ------------------------------------------------------------------ */
/* Pruebas                                                            */
/* ------------------------------------------------------------------ */
static void testExercise1(void)
{
    printf("=== Ejercicio 1: swap ===\n");
    int a = 8, b = -3;
    printf("Antes: a = %d, b = %d\n", a, b);
    swap(&a, &b);
    printf("Despues: a = %d, b = %d\n\n", a, b);
}

static void testExercise2(void)
{
    printf("=== Ejercicio 2: estadisticas ===\n");
    int v[] = {4, -2, 0, 7, -5};
    int n = (int)(sizeof v / sizeof v[0]);
    printArray("Entrada: ", v, n);
    printf("Suma: %lld Minimo: %d Maximo: %d\n", sumArray(v, n), minArray(v, n), maxArray(v, n));
    printf("Negativos: %d Inverso: ", countNegatives(v, n));
    printReverse(v, n);
    printArray("Despues (sin cambios): ", v, n);
    printf("\n");
}

static void testExercise3(void)
{
    printf("=== Ejercicio 3: copia independiente ===\n");
    int v[] = {4, -2, 0, 7, -5};
    int n = (int)(sizeof v / sizeof v[0]);
    int *copy = copyArray(v, n);
    if (copy == NULL) {
        printf("Fallo la reserva de memoria\n\n");
        return;
    }
    copy[0] = 99;
    printArray("Original: ", v, n);
    printArray("Copia: ", copy, n);
    free(copy);
    printf("\n");
}

static void testExercise4(void)
{
    printf("=== Ejercicio 4: ordenar una copia ===\n");
    int v[] = {4, -2, 0, 7, -5};
    int n = (int)(sizeof v / sizeof v[0]);
    int *copy = copyArray(v, n);
    if (copy == NULL) {
        printf("Fallo la reserva de memoria\n\n");
        return;
    }
    sortAscending(copy, n);
    printArray("Original: ", v, n);
    printArray("Ordenado: ", copy, n);
    free(copy);
    printf("\n");
}

static void testExercise5(void)
{
    printf("=== Ejercicio 5: findPair ===\n");
    int a[] = {-4, -1, 0, 2, 5};
    int b[] = {0, 0};
    int c[] = {3};
    struct { const int *arr; int size; int target; int expected; } tests[] = {
        {a, 5, 1, 1}, {a, 5, 8, 0}, {b, 2, 0, 1}, {c, 1, 6, 0}
    };
    int total = (int)(sizeof tests / sizeof tests[0]);
    for (int i = 0; i < total; i++) {
        printArray("Arreglo: ", tests[i].arr, tests[i].size);
        printf("Objetivo %d -> ", tests[i].target);
        int r = findPair(tests[i].arr, tests[i].size, tests[i].target);
        if (r == 0) printf("No existe\n");
        printf("  [%s]\n", r == tests[i].expected ? "OK" : "FALLO");
    }
    printf("\n");
}

static void testExercise6(void)
{
    printf("=== Ejercicio 6: threeSum ===\n");
    int a[] = {-1, 0, 1, 2, -1, -4};
    int b[] = {0, 1, 1};
    int c[] = {0, 0, 0};
    int d[] = {0, 0, 0, 0};
    int e[] = {-2, 0, 0, 2, 2};
    int f[] = {1, -1};   /* tamano insuficiente */
    struct { const int *arr; int size; int expected; } tests[] = {
        {a, 6, 2}, {b, 3, 0}, {c, 3, 1}, {d, 4, 1}, {e, 5, 1}, {f, 2, 0}
    };
    int total = (int)(sizeof tests / sizeof tests[0]);
    for (int i = 0; i < total; i++) {
        printArray("Entrada: ", tests[i].arr, tests[i].size);
        int r = threeSum(tests[i].arr, tests[i].size);
        printf("  Cantidad: %d [%s]\n", r, r == tests[i].expected ? "OK" : "FALLO");
    }
    printf("\n");
}

static void testExercise7(void)
{
    printf("=== Ejercicio 7: maximos acumulados ===\n");
    int h[] = {3, 0, 2, 0, 4};
    int n = (int)(sizeof h / sizeof h[0]);
    int *left = malloc((size_t)n * sizeof *left);
    int *right = malloc((size_t)n * sizeof *right);
    if (left == NULL || right == NULL) {
        free(left);
        free(right);
        printf("Fallo la reserva de memoria\n\n");
        return;
    }
    buildLeftMax(h, n, left);
    buildRightMax(h, n, right);
    printArray("height:   ", h, n);
    printArray("leftMax:  ", left, n);
    printArray("rightMax: ", right, n);
    free(left);
    free(right);
    printf("\n");
}

static void testExercise8(void)
{
    printf("=== Ejercicio 8: trappedWater ===\n");
    int t1[] = {5};
    int t2[] = {0, 1, 2, 3};
    int t3[] = {3, 2, 1, 0};
    int t4[] = {2, 2, 2};
    int t5[] = {3, 0, 3};
    int t6[] = {3, 0, 2, 0, 4};
    int t7[] = {4, 2, 0, 3, 2, 5};
    int t8[] = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
    int own[] = {2, 0, 1, 0, 3};  /* caso propio */
    struct { const int *arr; int size; long long expected; } tests[] = {
        {t1, 1, 0}, {t2, 4, 0}, {t3, 4, 0}, {t4, 3, 0},
        {t5, 3, 3}, {t6, 5, 7}, {t7, 6, 9}, {t8, 12, 6}
    };
    int total = (int)(sizeof tests / sizeof tests[0]);
    for (int i = 0; i < total; i++) {
        printArray("Alturas: ", tests[i].arr, tests[i].size);
        long long r = trappedWater(tests[i].arr, tests[i].size);
        printf("  Agua total: %lld [%s]\n", r, r == tests[i].expected ? "OK" : "FALLO");
    }

    printf("\nDetalle por posicion (caso del enunciado):\n");
    long long r = trappedWaterImpl(t6, 5, 1);
    printf("  Total: %lld\n", r);

    /* Caso propio: columnas 2,0,1,0,3.
     * Pos 1: min(2,3)-0 = 2; pos 2: min(2,3)-1 = 1; pos 3: min(2,3)-0 = 2.
     * Total esperado = 5. */
    printf("\nCaso propio:\n");
    r = trappedWaterImpl(own, 5, 1);
    printf("  Total: %lld (esperado 5) [%s]\n\n", r, r == 5 ? "OK" : "FALLO");
}

int main(void)
{
    testExercise1();
    testExercise2();
    testExercise3();
    testExercise4();
    testExercise5();
    testExercise6();
    testExercise7();
    testExercise8();
    return 0;
}
