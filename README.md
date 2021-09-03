
# Broadcast entre threads usando um buffer compartilhado

## Pré-Requisitos
Certifique-se que os programas *make* e *gcc* estão instalados no ambiente linux, caso não instale com os comandos:

Ubuntu, Debian:
````bash
sudo apt-get install make gcc gdb
````

CentOS, Fedora:
````bash
yum install make gcc gdb
````

ou equivalentes.

## Executando o Programa

Basta executar um dos comandos
```bash
make inicializa.test PARAMS="N P C I"
make produz.test PARAMS="N P C I"
make consome.test PARAMS="N P C I"
make completo.test PARAMS="N P C I"
```
Ex:
```bash
make inicializa.test PARAMS="16 2 2 4"
```

## Compilando o Programa

*** Antes de começar, verifique se make e gcc estão instalados ***

Execute o comando
```bash
make tarefa1
```

## Testes

Descrição dos testes realizados, problemas identificados e não resolvidos. 

[Relatorio dos Testes](./docs/relatorio.md)

