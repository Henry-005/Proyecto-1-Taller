# Taller de programación imperativa en C

**Curso:** Paradigmas de Programación — Ingeniería en Computadores, ITCR
**Integrante:** Henry Perez Sequeira — 2024138803
**Lenguaje:** C (C11)

## Ejecución

```
gcc -std=c11 -Wall -Wextra -pedantic main.c -o taller
./taller
```

El programa define todas las entradas en `main` y ejecuta una función de prueba por ejercicio. Las pruebas con resultado esperado imprimen `[OK]` o `[FALLO]`.

## Resultados de las pruebas

| Ejercicio | Resultado |
|---|---|
| 1. `swap` | a = 8, b = -3 pasa a a = -3, b = 8 |
| 2. Estadísticas | Suma 4, mín -5, máx 7, negativos 2, inverso `-5 7 0 -2 4`; arreglo sin cambios |
| 3. `copyArray` | Copia con primer elemento 99; el original no cambia; memoria liberada |
| 4. Ordenar copia | `[-5, -2, 0, 4, 7]`; original intacto |
| 5. `findPair` | 4 casos del enunciado correctos, incluido tamaño 1 |
| 6. `threeSum` | 5 casos del enunciado + tamaño insuficiente (0 ternas), todos correctos |
| 7. Máximos | leftMax `[3,3,3,3,4]`, rightMax `[4,4,4,4,4]` |
| 8. `trappedWater` | 8 casos del enunciado correctos (0, 0, 0, 0, 3, 7, 9, 6) + caso propio |

Verificado además con `-fsanitize=address,undefined` sin errores ni fugas.

**Caso propio (ejercicio 8):** alturas `[2, 0, 1, 0, 3]`. leftMax = `[2,2,2,2,3]`, rightMax = `[3,3,3,3,3]`. Agua por posición = `[0, 2, 1, 2, 0]`, total 5. Justificación: las columnas de los extremos (2 y 3) forman las paredes; el nivel del agua en las posiciones 1 a 3 lo fija la pared más baja (2), por eso en la posición 2 (altura 1) caben 1 unidad y en las posiciones 1 y 3 caben 2 unidades cada una.

## Explicaciones

### Ejercicio 1: punteros en `swap`
Se usan punteros porque C pasa los argumentos por valor: si `swap` recibiera `int a, int b` modificaría copias. Con las direcciones puede escribir en las variables originales. Modificar `a` dentro de la función cambia solo el puntero local (a dónde apunta), sin afectar al llamador; modificar `*a` cambia el valor almacenado en la dirección, que es la variable original. La variable auxiliar `tmp` tiene alcance local: solo existe dentro de `swap` y se destruye al retornar.

### Ejercicio 3: asignar punteros no copia elementos
`b = a` copia solo la dirección, así que ambos punteros apuntan al mismo bloque y modificar uno modifica el otro. Para tener datos independientes hay que reservar un bloque nuevo con `malloc` y copiar elemento por elemento.

### Ejercicio 5: por qué se descarta un extremo
Con el arreglo ordenado, si `values[left] + values[right] < target`, `values[left]` no puede formar el par con ningún valor del rango actual, porque `values[right]` ya es el mayor disponible; se descarta `left`. Si la suma es mayor que `target`, `values[right]` no puede formar par ni con el menor valor restante, y se descarta `right`. Cada paso descarta un candidato, por eso el recorrido es O(n).

### Ejercicio 6: cómo se evitan ternas duplicadas en 3Sum
Se ordena una copia del arreglo. Para el primer elemento se salta `sorted[i]` si es igual a `sorted[i-1]`, de modo que cada valor se usa una sola vez como primer elemento. Cuando se encuentra una terna, se avanzan `left` y `right` mientras apunten a valores iguales a los ya usados, de modo que el mismo par no se repite. Como todo está ordenado, las repeticiones quedan contiguas y la complejidad total es O(n²).

### Ejercicio 8: por qué el agua depende del menor de los dos máximos
El agua sobre una columna se sostiene entre la pared más alta de la izquierda y la más alta de la derecha. El nivel no puede superar la más baja de las dos, porque por encima de ella el agua se derrama por ese lado. Por eso `water[i] = min(leftMax[i], rightMax[i]) - height[i]`.

### Ejemplo de alcance de variables (de nuestro código)
En `swap`, la variable `tmp` es local: solo es visible dentro de la función y deja de existir al terminar. Otro ejemplo: en `threeSum`, `left`, `right` y `sum` se declaran dentro del bloque del `for`/`while`, y su alcance termina con ese bloque. No usamos variables globales.

### Manejo de memoria: C comparado con Java
En C la memoria dinámica es manual: se reserva con `malloc`, se debe comprobar si devolvió `NULL` y se debe liberar con `free`; olvidarlo produce fugas y liberarlo mal produce errores de acceso. En Java los objetos y arreglos se crean con `new` en el heap y el recolector de basura los libera cuando ya no hay referencias, así que no hay `free` ni aritmética de punteros, a cambio de menos control sobre cuándo se libera la memoria.
