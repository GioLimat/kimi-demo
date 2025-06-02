# Demonstração da Linguagem “Orion”

> **Aviso:** Esta versão está em desenvolvimento, possui limitações e pode gerar bugs inconsistentes.

Uma linguagem de programação com pipeline completo de execução, incluindo:

- **Lexer**
- **Parser**
- **Semantic Validator**
- **Intermediate Representation (IR)**
- **Gerador de Bytecode**
- **Máquina Virtual (VM)**

---

## Recursos Suportados

1. **Tipos Primitivos**
    - `char` (UTF-32)
    - `int8`
    - `int16`
    - `int32`
    - `int64`
    - `float`
    - `double`
    - `boolean`

2. **Funções**
    - Parâmetros e retorno de valor
    - Funções aninhadas (nested functions)
    - Suporte à recursividade

3. **Operadores**
    - **Aritméticos**: `+`, `-`, `*`, `/`, `%`
    - **Bitwise**: `&`, `|`, `^`, `~`, `<<`, `>>`
    - **Lógicos**: `&&`, `||`, `!`
    - **Comparação**: `==`, `!=`, `<`, `>`, `<=`, `>=`
    - **Incremento / Decremento**: `++`, `--`

4. **Controle de Fluxo**
    - **If / Else If / Else**
    - **While Loop**

---

## Exemplo de Código: Fatorial (Recursão)

```c
fn fact(b: Int) : Int {
    if (b == 1) {
        return 1;
    }
    return b * fact(b - 1);
}

println(fact(5));
