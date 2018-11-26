# CNCells
Neste repositório encontram-se todos os códigos utilizados na construção do projeto CNCells

Este documento técnico apresenta os detalhes da construção do firmware da CNCells, uma máquina CNC (Comando numérico computadorizado), com dois eixos de liberdade XY, que tem o objetivo de junto à uma câmera microscópica agir na automatização para análise de amostras em lâminas, com interface gráfica na web.
A projeto CNCells em sua versão 1.0, tem três principais objetivos: Ler um arquivo G-Code, enviar os dados do caminho percorrido para um serviço na nuvem e gerar um gráfico com tais dados.
Com o propósito de ter maior precisão no projeto mecânico, para a sua construção, foi utilizado o software de modelagem 3D Inventor Profissional 2019. Uma máquina de corte a laser e peças em impressão 3D. 
Para sua movimentação foi utilizado motores NEMA17, padrão para CNC de pequeno porte, sendo estes integrados por drives de passo A4988 que por sua vez eram controlados por um ESP8266. A escolha do ESP em relação a outros microcontroladores do mercado vem por sua extrema facilidade em se conectar com a internet via Wi-fi.
O serviço de nuvem utilizado neste projeto foi o Google Drive Sheets, este, além de armazenar os dados gera um gráfico em tempo real do caminho percorrido pela aplicação.
Todo o firmware foi programado em linguagem C++, através da IDE do Arduino. 

