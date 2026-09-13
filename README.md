Para executar o codigo, basta compilar o arquivo `relacao.c`:
    $ gcc relacao.c -o a.out

e rodar o executavel, enviando a entrada como input, para que ela seja lida pelo STDIN:
    $ ./a.out < entradas/in1.txt
alternativamente (usando comandos de leitura de arquivo):
    $ cat entradas/in1.txt | ./a.out
    $ more entradas/in1.txt | ./a.out

Note que foi usado a funcao `feof(stdin)` no codigo. Portanto, somente rodar o executavel (`$ ./a.out`) nao funciona;
uma vez que a flag EOF so eh reconhecida pelo programa se o conteudo for passado usando o pipeing-system (|).
No entanto, outros programas de leitura de arquivo (como o `more`) tambem funcionam.

No Windows,
    $ type entradas/in1.txt | a.out