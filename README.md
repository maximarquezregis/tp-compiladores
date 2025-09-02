# Compilador Mini (Flex/Bison)

## Descripción
Compilador e intérprete simple para un lenguaje básico con tipos enteros y booleanos. Usa Flex (léxico) y Bison (sintaxis) para generar un AST; luego:

- Genera pseudo-assembly (el assembly se genera en assembly/gen_assembly.txt).
- Interpreta el código directamente desde el AST.
- Imprime el AST y la tabla de símbolos.

## Características del lenguaje
- Estructura: `int main() { ... }`, `void main() { ... }` o `bool main() { ... }`.
- Tipos: `int`, `bool` (y `void` sólo para `main`).
- Literales: enteros, `true`, `false`.
- Declaraciones: `int x;`, `bool flag;`.
- Asignación: `x = expr;`.
- Expresiones aritméticas: `+ - * /` y unario `-`.
- Expresiones lógicas: `&&`, `||`, `!` (precedencia y asociatividad estándar, `!` como prefijo).
- Retorno: `return expr;` o `return;` (según `int|bool`/`void`).
- Evaluación estricta izquierda→derecha.

(No hay funciones adicionales, control de flujo, condicionales, ni arrays.)

## Dependencias
- flex
- bison
- gcc
- make

## Compilación
```sh
make
```
Genera el binario `parser`.

## Uso
Desde archivo:
```sh
./parser input.txt
```
O por stdin (Ctrl+D para finalizar):
```sh
./parser
```
Salida generada:
- Pseudo-assembly: `assembly/gen_assembly.txt`.
- AST y tabla de símbolos en consola.

## Ejemplo mínimo
Archivo `input.txt`:
```
int main() {
  int x;
  bool b;
  x = 3 + 4 * 2;
  b = true && false;
  return x;
}
```
Ejecución:
```sh
make && ./parser input.txt
```
Pseudo-assembly ilustrativo (extracto):
```
DECL x
DECL b
LOADVAL 3
LOADVAL 4
LOADVAL 2
MUL
ADD
STORE x, EAX
LOADVAL 1
LOADVAL 0
AND
STORE b, EAX
LOAD x
RET
```

## Notas
- Errores léxicos/sintácticos se reportan con la línea.
- El tamaño del buffer de instrucciones está limitado a 1000 instrucciones (ajustable en `pas.c`).
- El intérprete maneja errores en tiempo de ejecución (división por cero, variables no inicializadas).

## Autores 
- Máximo Pablo Marquez Regis 
- Jeremías José Avaro
- Mateo Andrés Cornejo