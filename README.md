# ` Tarea 1 – Lenguajes de Programación 3 `

Implementación de los _Listings_ de los capítulos 1 al 5 del libro _"Advanced Linux Programming"_.  
Todos los programas están organizados en carpetas por capítulo y se compilan mediante **GNU Make**.

# Uso del `makefile`
## Compilar todo
    make all
Genera todos los ejecutables en bin/CapituloX/.

## Limpiar compilaciones
    make clean
Elimina binarios y objetos.

## Compilar un listing puntual
    make listing-3.5
Compila únicamente src/Capitulo3/Listing3.5.c → bin/Capitulo3/listing3.5.

## Compilar todos los de un capítulo
    make cap4
Compila todos los listados del Capítulo 4.

# Colaboradores
- [Leyda Fleitas](https://github.com/leydafleitasfp)
- [Daniela González](https://github.com/maquedani)
