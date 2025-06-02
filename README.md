# Demo da Linguagem de Programação “Orion”

Uma linguagem de programação com pipeline completo de execução, incluindo **lexer**, **parser**, **semantic validator**, **intermediate representation**, **bytecode** e **máquina virtual (VM)**.  

---

## Recursos Suportados

1. **Tipos de Dados**  
   - `char` (UTF-32)  
   - `int8`  (signed 8 bits)  
   - `int16` (signed 16 bits)  
   - `int32` (signed 32 bits)  
   - `int64` (signed 64 bits)  
   - `float` (32 bits)  
   - `double` (64 bits)  
   - `boolean` (true / false)  

2. **Funções**  
   - Parâmetros e retorno de valor  
   - Funções aninhadas (nested functions)  
   - Suporte à recursividade  

3. **Operadores**  
   - Operações **aritméticas**: `+`, `-`, `*`, `/`, `%`  
   - Operações **bitwise**: `&`, `|`, `^`, `~`, `<<`, `>>`  
   - Operações **lógicas**: `&&`, `||`, `!`  
   - Operadores de **comparação**: `==`, `!=`, `<`, `>`, `<=`, `>=`  
   - Incremento / Decremento: `++`, `--`  

4. **Controle de Fluxo**  
   - **If / Else If / Else**  
   - **While Loop**  

---

## Exemplo de Sintaxe

```c
// Declaração de função principal
fn main() : int32 {
    var i : int32 = 0;

    // Laço while
    while (i < 10) {
        print(i);
        i++;
    }

    return 0;
}

// Função recursiva aninhada
fn fib(n : int32) : int32 {
    if (n <= 1) {
        return n;
    } else {
        return fib(n - 1) + fib(n - 2);
    }
}
