# client-server-work
Aplicação em C de Cliente-Servidor usando Threads e Sockets

**Type_A - Quantidade fixa de threads:** N threads criadas no momento da criação do processo.

**Type_C - Quantidade variável de threads:** Uma thread deverá ser criada para cada cliente.

Para compilar e executar o `client.c` usar os seguintes comandos no terminal:

**Compilar:**
```
 gcc -pthread -o client client.c
```
**Executar:**
```
 ./client
```

Para compilar e executar o `type_a_server.c` usar os seguintes comandos no terminal:

**Compilar:**
```
gcc -pthread -o type_a_server type_a_server.c
```

**Executar:**
```
./type_a_server
```

Para compilar e executar o `type_c_server.c` usar os seguintes comandos no terminal:

**Compilar:**
```
gcc -pthread -o type_c_server type_c_server.c
```

**Executar:**
```
./type_c_server
```
