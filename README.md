Para executar o código, basta compilar o arquivo `relacao.c`:
```
$ gcc relacao.c -o a.out
```

e rodar o executável, enviando a entrada como input, para que ela seja lida pelo STDIN:
```
$ ./a.out < entradas/in1.txt
```
alternativamente (usando comandos de leitura de arquivo):
```
$ cat entradas/in1.txt | ./a.out
$ more entradas/in1.txt | ./a.out
```

Note que foi usada a função `feof(stdin)` no código. Portanto, somente rodar o executável (`$ ./a.out`) não funciona;
uma vez que a flag `EOF` so é reconhecida pelo programa se o conteúdo for passado usando pipe.
No entanto, outros programas de leitura de arquivo (como o `more`) também funcionam.

No Windows,
```
$ type entradas/in1.txt | a.out
```
