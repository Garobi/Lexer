Para rodar o programa basta rodar o makefile. Admito que não testamos com um MacOS, então pode não funcionar.

Caso não funcione, pode usar o g++ para compilar. O comando utilizado que funcionou na minha máquina foi o:

g++ -std=c++11 main.cpp lexer.cpp parser.cpp definitions.cpp -o Lexer

Foram retiradas todas as dependencias do windows então deve funcionar normalmente no MacOS.