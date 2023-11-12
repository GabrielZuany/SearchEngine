<div align="center">
  <h3 align="center">SearchEngine</h3>
</div>

## Sobre

SearchEngine foi o trabalho final desenvolvido ao longo da disciplina de Técnicas de Busca e Ordenação, ministrada pelo professor Giovanni.

## Começando

### Pré-requisitos

- Linux (sem suporte pro windows, sra. microsoft)
- [GNU Compiler Collection](https://gcc.gnu.org/)

### Instalando

1. Clone o repositório

   ```sh
   git clone https://github.com/GabrielZuany/SearchEngine.git
   ```

2. Dirija-se ao seu diretório

   ```sh
   cd SearchEngine/
   ```

3. Compile o código-fonte

   ```sh
   make
   ```

## Usando

Intruções de uso para o giovanni.

### Giovanni

giovanni.

```sh
./trab3 <diretório_entrada>
```

Onde,

- `diretório_entrada` - é o nome do diretório de entrada.

## Contribuindo

Este projeto está configurado e integrado com o Visual Studio Code.

### Valgrind

Há uma regra no [Makefile](./Makefile) para lançar rapidamente o Valgrind. O resultado será salvo em [valgrind-out.txt](./valgrind-out.txt).

```sh
make valgrind VALARGS=<args>
```

Onde,

- `args` - são os argumentos para passar ao programa. Opcional.

## Suporte

Esse programa não funciona em ambientes não-POSIX e pode até mesmo não funcionar em sistemas não-Linux.

Isso se deve ao fato de usar APIs que são extensões do POSIX e não fazem parte do padrão C.

## Licença

Licenciado sob [Unlicense](./LICENSE).
