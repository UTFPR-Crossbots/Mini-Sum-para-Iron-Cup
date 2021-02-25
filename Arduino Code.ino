/*******PINOUT DEFINES*********/
// it is not recommended to make changes
// nao e recomendado que se faca alteracoes
// no se recomienda hacer cambios


// LED
#define LED 6

// left motor
#define pwmL 9
#define leftMotor1 7
#define leftMotor2 8

// right motor
#define pwmR 3
#define rightMotor1 5
#define rightMotor2 4

// DIP switch
#define DIP1 10
#define DIP2 11
#define DIP3 12
#define DIP4 13

// Robocore's line sensor
#define lineL A0
#define lineR A1

// Jsumo's distance sensor
#define distL A2
#define distR A3

// Jsumo's micro-start
#define microST 2
/*******PINOUT DEFINES - END*********/

/*******FUNCTIONS*******/
void MotorL(int pwm); // left motor / motor esquerdo / motor izquierdo
void MotorR(int pwm); // right motor / motor direito / motor derecho
int readDIP(); // read DIP switch / ler chave DIP / leer el interruptor DIP
/*******FUNCTIONS - END*******/

void setup() {

  /****************PINOUT CONFIG****************/
  // OUTPUTS
  pinMode(LED, OUTPUT);         // led

  // right motor
  pinMode(pwmR, OUTPUT);        // right motor power
  pinMode(rightMotor1, OUTPUT); // right motor dir.
  pinMode(rightMotor2, OUTPUT); // right motor dir.

  // left motor
  pinMode(pwmL, OUTPUT);        // left motor power
  pinMode(leftMotor1, OUTPUT);  // left motor dir.
  pinMode(leftMotor2, OUTPUT);  // left motor dir.


  // INPUTS: DO NOT CHANGE / NAO MUDAR / NO CAMBIAR
  // DIP switch
  pinMode(DIP1, INPUT_PULLUP);  // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR
  pinMode(DIP2, INPUT_PULLUP);  // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR
  pinMode(DIP3, INPUT_PULLUP);  // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR
  pinMode(DIP4, INPUT_PULLUP);  // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR

  // line sensor
  pinMode(lineL, INPUT); // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR
  pinMode(lineR, INPUT); // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR

  // distance sensor
  pinMode(distR, INPUT); // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR
  pinMode(distL, INPUT); // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR

  // micro-start
  pinMode(microST, INPUT); // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR
  /****************PINOUT CONFIG - END***************/

  /***************INITIAL CONDITIONS*****************/
  digitalWrite(LED, LOW); // LED off / LED desligado / LED apagado
  MotorL(0); // left motor stopped / motor esquerdo parado / motor izquierdo parado
  MotorR(0); // right motor stopped / motor direito parado / motor derecho parado
  /*************INITIAL CONDITIONS - END*************/
}

//line 0 é sem ver a linha
//line 1 é linha na esquerda
//line 2 é linha na direita
//line 3 é linha dos dois lados
int senliL, senliR, sendiL, sendiR, line = 0, blPart = 626, rotacao = 1, strat = 0, valorDIP = 15;

//obtem o valor dos dois sensores de distancia
void SenDi(){
     sendiL = digitalRead(distL);
     sendiR = digitalRead(distR);
}

//verifica onde a linha esta
void SenLi(){
     senliL = digitalRead(lineL);
     senliR = digitalRead(lineR);
     //linha em lugar nenhum
     if((senliL >= blPart) && (senliR >= blPart)){
          line = 0;
          //linha na esquerda apenas
     }else if((senliL <= blPart) && (senliR >= blPart)){
          line = 1;
          //linha na direita apenas
     }else if((senliL >= blPart) && (senliR <= blPart)){
          line = 2;
          //linha dos dois lados
     }else{
          line = 3;
     }
}

//função retirada do código do Marco Schneider
//faz a leitura do valor do dip
int readDIP (){
     int n = 0;
     if(digitalRead(DIP4)==HIGH)
          n = 1;
     if(digitalRead(DIP3)==HIGH)
          n = n + 2; //Bit shift (0001 --> 0010), ou seja, n =  n + 2
     if(digitalRead(DIP2)==HIGH)
          n = n + 4; //Bit shift (0001 --> 0100), ou seja, n =  n + 4
     if(digitalRead(DIP1)==HIGH)
          n = n + 8; //Bit shift (0001 --> 1000), ou seja, n =  n + 8
     return n; // Valor retornado de n
}

void loop() {
     valorDIP = readDIP();
     //caso  o botão para iniciar seja apertado
     if((digitalRead(microST)) == 1){
          digitalWrite(LED, HIGH);
          switch (valorDIP) {
               case 0: // 0000 -
                    break;

               case 1: // 0001 -
                    break;

               case 2: // 0010 -
                    break;

               case 3: // 0011 -
                    break;

               case 4: // 0100 -
                    break;

               case 5: // 0101 - B1 ou B5: de ladinho (meme)
                    //by Marco Przybysz
                    //o robô anda para trás e para frente antes de começar
                    MotorL(255);
                    MotorR(255);
                    delay(100);
                    MotorL(-255);
                    MotorR(-255);
                    delay(140);
                    rotacao = 1;
                    break;

               case 6: // 0110 - B5: ronaldinho (meme)
                    //by Marco Przybysz
                    //espelhamento da 7
                    while((digitalRead(microST)) == 1){
                         if(linha == 0){
                              MotorL(120);
                              MotorR(160);
                         }else{
                              MotorL(-120);
                              MotorR(-100);
                         }
                         SenLi();
                    }
                    break;

               case 7: // 0111 - B1: dibraldinho (meme)
                    //by Marco Przybysz
                    while((digitalRead(microST)) == 1){
                         if(linha == 0){
                              MotorL(160);
                              MotorR(120);
                         }else{
                              MotorL(-100);
                              MotorR(-120);
                         }
                         SenLi();
                    }
                    break;

               case 8: // 1000 - A2: Abertura em U
                    // by Marco Schneider
                    //o robô começa com um U
                    MotorL(128);
                    MotorR(128);
                    delay(100);
                    MotorL(128);
                    MotorR(-64);
                    delay(150);
                    rotacao = -1; //gira no sentido horario quando busca o oponente
                    SenDi(); //distancia
                    SenLi(); //linha
                    break;

               case 9: // 1001 - A4: Abertura em U espelho
                    // by Marco Schneider
                    //espelhamento da 8
                    MotorL(128);
                    MotorR(128);
                    delay(100);
                    MotorL(-64);
                    MotorR(-128);
                    delay(150);
                    rotacao = 1; //gira no sentido horario quando busca o oponente
                    SenDi(); //distancia
                    SenLi(); //linha
                    break;

               case 10: //1010 - A4: running in the 9s
                    //espelhamento da 12
                    MotorL(155);
                    MotorR(64);
                    delay(64);
                    rotacao = 1; //gira no sentido anti-horário quando busca o oponente
                    SenDi(); //distancia
                    SenLi(); //linha
                    break;

               case 11: //1011 - B3: roda roda jequiti
                    //robô começa girando no próprio eixo no sentido horário
                    //para buscar o oponente
                    SenLi();
                    SenDi(); //distancia
                    rotacao = 1; //gira no sentido anti-horário quando busca o oponente
                    break;

               case 12: //1100 - A4: running in the 90s
                    //by Marco Przybysz
                    //robô faz uma curva em 45° antes de começar a buscar o oponente
                    MotorL(64);
                    MotorR(155);
                    delay(64);
                    rotacao = -1; //gira no sentido horario quando busca o oponente
                    SenDi(); //distancia
                    SenLi(); //linha
                    break;

               case 13: //1101 - B3: desroda jequiti
                    //espelhamento da 11
                    SenLi();
                    SenDi(); //distancia
                    rotacao = -1; //gira no sentido horario quando busca o oponente
                    break;

               case 14: //1110 - B3: the clockwise suicidal
                    //como o robô já ta virado pro oponente, ele liga os motores
                    //antes de qualquer coisa,  para sair mais rápido
                    MotorL(255);
                    MotorR(255);
                    rotacao = -1; //gira no sentido horario quando busca o oponente
                    delay(200);
                    break;

               case 15: //1111 - B3: the ultimate suicidal robot
                    //padrão
                    //estratégia 14, mas gira anti horário
                    MotorL(255);
                    MotorR(255);
                    rotacao = 1; //gira no sentido anti-horário quando busca o oponente
                    delay(200);
                    break;
          }
          //a ideia é que depois da parte inicial da estratégia, a movimentação
          //seja a mesma
          while((digitalRead(microST)) == 1){ //repete até desligar
               //essa primeira parte detecta se o robô esta em cima de alguma
               //linha usando o valor resultante da função SenLi()
               if((line != 0) && (sendiL == 0) && (sendiR == 0)){
                    if(line == 3){ //ré caso tenha linha dos dois lados
                         MotorL(-255);
                         MotorR(-255);
                         delay(150);
                    }else if(line == 2){ //vai para direita em ré
                         MotorL(-255);
                         MotorR(-255);
                         delay(80);
                         MotorL(-128);
                         MotorR(-255);
                         delay(50);
                    }else{ //vai para a esquerda em ré
                         MotorL(-255);
                         MotorR(-255);
                         delay(80);
                         MotorL(-255);
                         MotorR(-128);
                         delay(50);
                    }
               //caso o robô esteja na área certa...
               //procura o adversário girando no sentido anti-horário
               }else if((sendiL == 0) && (sendiR == 0)){
                    if (rotacao > 0){
                         MotorL(72);
                         MotorR(255);
                    }else{
                         MotorL(255);
                         MotorR(75);
                    }
               //avança em direção ao adversário
               }else if((sendiL == 1) && (sendiR == 1)){
                    MotorL(255);
                    MotorR(255);
               //ajusta a rota um pouco para a esquerda
               }else if((sendiL == 1) && (sendiR == 0)){
                    MotorL(64);
                    MotorR(255);
               //ajusta a rota um pouco para a direita
               }else if((sendiL == 0) && (sendiR == 1)){
                    MotorL(255);
                    MotorR(64);
               }
               //verifica os dados dos sensores
               SenDi(); //distancia
               SenLi(); //linha
          }
     //desliga os motores
     }else{
          digitalWrite(LED, LOW);
          MotorL(0);
          MotorR(0);
     }

     //essa parte vai se repetir até que o robô comece a andar,
     //os valores já vão ser medidos para evitar atraso na hora que iniciar
     SenDi();
     SenLi();

}

/**LEFT MOTOR CONTROL / CONTROLE DO MOTOR ESQUERDO / CONTROL DEL MOTOR IZQUIERDO**/
// pwm = 0 -> stopped / parado / parado
// 0<pwm<=255 -> forward / para frente / seguir adelante
// -255<=pwm<0 -> backward / para tras / seguir espalda
void MotorL(int pwm){
  // leftMotor1=0 and leftMotor2=0 -> stopped / parado / parado
  // leftMotor1=0 and leftMotor2=1 -> moves forward / avanca / avanzar
  // leftMotor1=1 and leftMotor2=0 -> moves back / recua / retrocede
  // leftMotor1=1 and leftMotor2=1 -> stopped (braked) / parado (travado) / parado (frenado)

  if(pwm==0){
    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, LOW);
  }
  else if(pwm<0)
  {
    analogWrite(pwmL, -pwm);
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
  }
  else
  {
    analogWrite(pwmL, pwm);
    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, HIGH);
  }
}


/**RIGHT MOTOR CONTROL / CONTROLE DO MOTOR DIREITO / CONTROL DEL MOTOR DERECHO**/
// pwm = 0 -> stopped / parado / parado
// 0<pwm<=255 -> forward / frente / adelante
// -255<=pwm<0 -> backward / tras / espalda
void MotorR(int pwm){
  // rightMotor1=0 and rightMotor2=0 -> stopped / parado / parado
  // rightMotor1=0 and rightMotor2=1 -> moves forward / avanca / avanzar
  // rightMotor1=1 and rightMotor2=0 -> moves back / recua / retrocede
  // rightMotor1=1 and rightMotor2=1 -> stopped (braked) / parado (travado) / parado (frenado)

  if(pwm==0){
    digitalWrite(rightMotor1, LOW);
    digitalWrite(rightMotor2, LOW);
  }
  else if(pwm<0)
  {
    analogWrite(pwmR, -pwm);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
  }
  else
  {
    analogWrite(pwmR, pwm);
    digitalWrite(rightMotor1, LOW);
    digitalWrite(rightMotor2, HIGH);
  }
}
