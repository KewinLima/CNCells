/*################################################################################
                      CNCells v1.0
por: Kewin Lima

Este código controla uma CNC e envia a posição atual para 
a nuvem em uma planilha que gera um gráfico de pontos
                      
Recife, 26/11/2018
##############################################################################*/
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

//Liga ou desliga o debug pela porta serial
#define DEBUG 1

//Numero de tentativas para reconectar ao wi-fi
#define N_TENTATIVAS 5

//Define a direção linear
#define FRENTE HIGH
#define TRAS   LOW

//Define o tamanho máximo da área de trabalho
#define X_MAX 70
#define Y_MAX 40

//Definicao de pinos
#define PIN_STEP_X D1
#define PIN_DIR_X  D4
#define PIN_STEP_Y D5
#define PIN_DIR_Y  D6

//Define os tamanhos da entrada
#define TAM_STRING 19
#define TAM_DADOS  52

//Número de pulsos necessários para 1 volta completa no Nema 17
#define N_PULSOS 800

//Define o tamanho do raio da polia em milímetros
#define RAIO 6

WiFiClientSecure client;
 
// -- Variáveis e constantes --
 
const char *ssid = "Cris";
const char *password = "21061996";
const char* host = "script.google.com";
const int httpsPort = 443;
const char* fingerprint = "46 B2 C3 44 9C 59 09 8B 01 B6 F8 BD 4C FB 00 74 91 2F EF F6";
String googleSheetsID = "AKfycbxlmcZQcVsqD-BLzMkB-n4GStg_ZTj6_E2aOQQCo6TrkLw-PLU";
 
//Rotina
char entrada[TAM_DADOS][TAM_STRING] =
{"G1 X0.00 Y0.00", "G4 P150", "G1 X1.00 Y0.00", "G4 P150", "G1 X2.00 Y0.00", "G4 P150", "G1 X3.00 Y0.00", "G4 P150", "G1 X4.00 Y0.00", "G4 P150",
 "G1 X0.00 Y1.00", "G4 P150", "G1 X1.00 Y1.00", "G4 P150", "G1 X2.00 Y1.00", "G4 P150", "G1 X3.00 Y1.00", "G4 P150", "G1 X4.00 Y1.00", "G4 P150",
 "G1 X0.00 Y2.00", "G4 P150", "G1 X1.00 Y2.00", "G4 P150", "G1 X2.00 Y2.00", "G4 P150", "G1 X3.00 Y2.00", "G4 P150", "G1 X4.00 Y2.00", "G4 P150",
 "G1 X0.00 Y3.00", "G4 P150", "G1 X1.00 Y3.00", "G4 P150", "G1 X2.00 Y3.00", "G4 P150", "G1 X3.00 Y3.00", "G4 P150", "G1 X4.00 Y3.00", "G4 P150",
 "G1 X0.00 Y4.00", "G4 P150", "G1 X1.00 Y4.00", "G4 P150", "G1 X2.00 Y4.00", "G4 P150", "G1 X3.00 Y4.00", "G4 P150", "G1 X4.00 Y4.00", "G4 P150"};

// Declaração dos pontos de origem
float origem_x = -1; 
float origem_y = -1; 

//Declaração da posição atual
float posAtual_x = origem_x; 
float posAtual_y = origem_y; 

//Declaração da velocidade
int velocidade = 70;  //Escalda de 1 - 100

//Declaração de variáveis para tratar a string do Gcode
char gcodeComando[3] = {};
float gcode_x = 0;
float gcode_y = 0;

void setup() 
{
  // Configuracao dos pinos
  pinMode(PIN_STEP_X,OUTPUT); 
  pinMode(PIN_DIR_X, OUTPUT);
  pinMode(PIN_STEP_Y,OUTPUT); 
  pinMode(PIN_DIR_Y, OUTPUT);
  if(DEBUG) Serial.begin(115200);
  
  //Tenta se conectar com a internet
  connectToWiFi();
}
void loop() 
{  
    inicializarMesa();
    lerGcode();
    finalizarMesa();
}

void connectToWiFi() 
{
  Serial.println("Conectando a rede: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  WiFi.mode(WIFI_STA);
  Serial.println("");
  Serial.println("Conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  delay(1000);
}

void lerGcode()
{  
   for(int i = 0; i < TAM_DADOS ; i++)
   {
      for(int j = 0 ; entrada[i][j] != ' '; j++) //Procurando a primeira parte do Gcode, que corresponde ao tipo de comando
      {
          gcodeComando[j] = entrada[i][j];
      }
      if(gcodeComando[0] == 'G' && gcodeComando[1] == '1') //Comando para avanço da mesa
      {
          Serial.print("G01");
          Serial.print(" ");
          gcode_x = ((int)entrada[i][4]-48) + ((int)entrada[i][6]-48)/10 + ((int)entrada[i][7]-48)/100;
          Serial.print("(");
          Serial.print(gcode_x);
          Serial.print(" , ");
          
          gcode_y = ((int)entrada[i][10]-48) + ((int)entrada[i][12]-48)/10 + ((int)entrada[i][13]-48)/100;           
          Serial.print(gcode_y);
          Serial.println(")");
          moverPara(gcode_x,gcode_y);

          sendDataToGoogleSheets();
      }
      else if(gcodeComando[0] == 'G' && gcodeComando[1] == '4')
      {
          Serial.println("G04 P105");
          delay(150);
      }
   }
}

// Função que move a mesa para uma posição ( X , Y )
void moverPara(float novo_x, float novo_y)
{
    passo('x', novo_x - posAtual_x);
    posAtual_x = novo_x;
    passo('y', novo_y - posAtual_y);
    posAtual_y = novo_y;
}

void sendDataToGoogleSheets() {
  if (client.connect(host, httpsPort) != true) 
  {
      Serial.println("Falha na conexão ao Google Sheets");
      Serial.println("Tentando reconectar...");
      for(int i = 0; i < N_TENTATIVAS ; i++)
      {
        
          Serial.print("Tentativa numero ");
          Serial.println(i);
          if(client.connect(host, httpsPort))
            goto inicio;
          delay(500);
      }
      Serial.println("Falha na reconexão");
      return;
  }
  inicio:
  
  String stringPos_x = String(posAtual_x/10000000000, DEC);
  String stringPos_y = String(posAtual_y/10000000000, DEC);
  String url = "/macros/s/" + googleSheetsID + "/exec?X=" + stringPos_x + "&Y=" + stringPos_y;
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "User-Agent: BuildFailureDetectorESP8266\r\n" + "Connection: close\r\n\r\n");
  Serial.print("Request enviada");
  Serial.print(" (");
  Serial.print(stringPos_x); 
  Serial.print(" , ");
  Serial.print(stringPos_y); 
  Serial.println(")");
}

/* Função responsável por mover um determinado comprimento linearmente em milímetros em uma determinada direção 
  eixo ==  ' x 'ou ' y '
  1 < velocidade < 100
*/
void passo(char eixo, float distancia) { 
    
    limitadorVelocidade();
    if(eixo == 'x')
    {  
        digitalWrite(PIN_DIR_X,direcionador(distancia));  
        for(float i=0;i < (N_PULSOS*abs(distancia)/(2*3.14159*RAIO)) ;i++) 
        {
            digitalWrite(PIN_STEP_X,HIGH); 
            delayMicroseconds(500); 
            digitalWrite(PIN_STEP_X,LOW);
            delay(10 - velocidade/10);
        }
    }
    else if(eixo ==  'y')
    {
        digitalWrite(PIN_DIR_Y,direcionador(distancia));
        for(float i=0;i < (N_PULSOS*abs(distancia)/(2*3.14159*RAIO)) ;i++) 
        {
            digitalWrite(PIN_STEP_Y,HIGH); 
            delayMicroseconds(500); 
            digitalWrite(PIN_STEP_Y,LOW);
            delay(10 - velocidade/10);
        }
     }
}

void limitadorVelocidade()
{
    //Limitador de velocidade
    if(velocidade < 1)
      velocidade = 1;
    if(velocidade > 100)
       velocidade = 100;    
}

bool direcionador(float distancia)
{
    if(distancia < 0) 
        return TRAS;
    else 
        return FRENTE;  
}
void printPosicao()
{
    Serial.print("(");
    Serial.print(posAtual_x);
    Serial.print(" , ");
    Serial.print(posAtual_y);
    Serial.println(")");
    
}
void inicializarMesa()
{
    moverPara(0,0);
    printPosicao();
    delay(500);
    moverPara(40,0);    
    printPosicao();
    delay(500);
    moverPara(40,40);
    printPosicao();   
    delay(500);    
    moverPara(0,40);
    printPosicao();    
    delay(500);
    moverPara(0,0);
    printPosicao();    
    delay(500);
}
void finalizarMesa()
{
    moverPara(origem_x,origem_y);
    printPosicao();    
    delay(10000);
}
