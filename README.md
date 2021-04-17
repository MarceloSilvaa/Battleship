#### Autores do projeto

Elisabete Pinto\
Marcelo Silva

#### Compilação
1\. Extrair todos os ficheiros\
2\. Mudar para o diretório que contém os ficheiros resultantes da extração\
3\. Executar o comando ```make```

##### É possível escolher entre duas estruturas de suporte durante a compilação
1\. ```make matrix``` para utilizar a matrix como suporte para o jogo\
2\. ```make quad``` para utilizar a quadtree como suporte para o jogo\
3\. ```make``` irá compilar o jogo com a estrutura predefinida que é a quadtree

#### Execução
Depois de concluir a compilação do programa, para executar o jogo deve usar o comando ```./battleship ```

Durante a execução do jogo é possível escolher se quer que ambos os jogadores joguem no mesmo terminal, ou se quer que joguem em terminais diferentes.

##### Execução do jogo em terminais diferentes

Nesta situação, é importante referir que o jogo deve ser executado a partir do mesmo diretório, por ambos os jogadores.

O jogo não deve ser interrompido antes do seu fim. (Exemplo: Não deve ser utilizado ctrl-C após o início do jogo) 

O incumprimento destas duas condições leva ao mal funcionamento do jogo e poderá prejudicar jogos futuros, caso os semáforos não tenham sido libertados. Seria necessário limpar os semáforos do diretório atual, ou alterar o nome do semáforo a utilizar na nova execução do programa.

#### Relação entre o tamanho da matriz e o número de barcos
Quando recebemos instruções sobre como determinar o número de barcos permitido para uma determinada matriz, já tinhamos estudado uma fórmula mais complexa de o fazer, e com a devida autorização decidimos manter essa fórmula.

O objetivo desta fórmula é manter o jogo estável para todos os possíveis tamanhos da matriz, ao mesmo tempo que é dada flexibilidade ao utilizador para configurar o jogo.

Para isto, baseamo-nos numa configuração base o mais parecida possível com a configuração do jogo regular, mas tendo em conta que essa configuração terá de ser adaptada para os diversos possíveis tamanhos da matriz.

Essa configuração base foi a seguinte:
- Tamanho da matriz: 10x10
- Número de barcos: 10
- Número de células preenchidas: 30

Para oferecer flexibilidade na escolha do número de barcos, foi importante destacar uma percentagem mínima e máxima da ocupação do tabuleiro.


A percentagem mínima de ocupação do tabuleiro corresponde à percentagem de ocupação na configuração base, ou seja, uma ocupação de 30%.

Para determinar uma percentagem máxima que fizesse sentido, decidimos adicionar 3 barcos de cada tipo básico (barcos rectangulares) à configuração base. Nesta situação a configuração ficou com uma percentagem de ocupação de 72% e é este o valor da percentagem máxima utilizada nas fórmulas.

Valores utilizados nas fórmulas:
- L, corresponde à largura do tabuleiro escolhida pelo utilizador;
- NB, número de navios na configuração base;
- AB, área do tabuleiro na configuração base; 
- CB, número de células ocupadas se utilizar apenas os quatro navios básicos;
- PMAX, percentagem máxima de ocupação do tabuleiro;
- PMIN, percentagem mínima de ocupação do tabuleiro.

##### Fórmula para calcular o número mínimo de barcos
```
NB + (L * L - AB) / CB
```
##### Fórmula para calcular o número máximo de barcos
```
NB + (L * L - AB) / CB * PMAX / PMIN
```

#### Explicação detalhada dos inputs

##### Forma de jogar

Este jogo pode ser jogado de duas maneiras. Ambos os jogadores jogam no mesmo terminal, ou então, cada jogador joga num terminal diferente. Neste caso, são usados ficheiros e semáforos como comunicação interna entre os jogadores.

Deve inserir o valor 1 se pretende que ambos os jogadores joguem no mesmo terminal. Caso queira que os jogadores joguem em dois terminais diferentes então deve inserir o valor 2.

##### Tamanho do tabuleiro

Tendo em conta os valores escritos entre parênteses (x1-x2), deve inserir um número entre x1 e x2.

##### Número de barcos

Tendo em conta os valores escritos entre parênteses (x1-x2), deve inserir um número entre x1 e x2.

##### Nomes de jogadores

Qualquer string com tamanho >= 1 e <= 15.

Strings que não cumpram com estes requisitos não dão problemas no programa, mas os nomes dos utilizadores serão alterados.


No caso da string ter tamanho < 1, a string será substituída por "Player 1" ou "Player 2", consoante o jogador que escolheu o nome.

No caso da string ter tamanho > 15, os caracteres que estão depois do limite são cortados.

##### Observações sobre os inputs anteriores quando o jogo é jogado em dois terminais diferentes

O tamanho do tabuleiro é escolhido pelo primeiro jogador que inicie o programa. 

O número de barcos é escolhido pelo primeiro jogador que inicie o programa.

Os nomes dos jogadores não podem ser idênticos. Se o segundo jogador escolher um nome idêntico ao primeiro jogador, então o nome do segundo jogador será alterado automaticamente.

##### Escolha do tipo de posicionamento

As escolhas possíveis são 0 e 1. O posicionamento manual corresponde à escolha 0, enquanto o posicionamento aleatório corresponde à escolha 1.

##### Posicionamento manual

Sendo C o indicador da coluna e L o indicador da linha, o posicionamento manual deve ser feito como indicado a seguir.


Barcos rectangulares devem ser inseridos na forma CL:CL. Por exemplo, A1:A4 seria um barco que ocupava as células A1, A2, A3 e A4.


Barcos do tipo L devem ser inseridos na forma CL:CL,CL. Por exemplo, A1:A2,B1 seria um barco que ocupava as células A1, A2 e B1.

##### Escolha da coordenada a atingir

As coordenadas devem ser representadas na forma CL. Onde C corresponde à coluna e L corresponde à linha.

#### Representação do jogo

##### Símbolos
Para presentar o jogo são utilizados 4 símbolos diferentes: S, X, ? e ~ :
- S corresponde a uma parte de barco;
- X corresponde a uma parte de barco atingida;
- ? corresponde a uma célula desconhecida;
- ~ corresponde a água.

##### Cores
Para tornar a representação do jogo mais intuitiva foram utilizadas as seguintes cores :
- Branco, no início do jogo todas as peças têm esta cor;
- Azul, uma célula com água foi atingida;
- Amarelo, uma parte de navio foi atingida;
- Vermelho, todas as partes de um navio foram atingidas;
- Roxo, serve para destacar o símbolo de desconhecido.
