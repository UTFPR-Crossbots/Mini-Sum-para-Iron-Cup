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
int calibrateLine();
/*******FUNCTIONS - END*******/
 
/*******VARIÁVEIS*******/
int start;            //Variáveis para indicar se o robô deve ser acionado ou permanecer parado
int frontR,frontL;    //Variáveis para indicar o valor mais recente retornado pelo sensor frontal de direita e da esquerda
int infR,infL;        //Variáveis para indicar o valor mais recente retornado pelo sensor inferior de direita e da esquerda
int infR_min;     //Variáveis para definir o valor mínimo aceitável pelo sensor de linha de direita
int infL_min;     //Variáveis para definir o valor mínimo aceitável pelo sensor de linha de direita
bool aberturaFoiFeita = false; //Variável para identificar se o movimento de abertura foi feito

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

  //Calibração da linha//
  infL_min = calibrateLine();
  infR_min = calibrateLine();
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
 
/** read DIP switch / ler chave DIP / leer el interruptor DIP **/
// returns a value between 0 and 15
// retorna um valor entre 0 e 15
// devuelve un valor entre 0 y 15

//Segundo o regulamento da competição, a posição padrão da chave DIP será 1111
//Ou seja, a função readDIP retornará o valor de n como sendo 15
//A partir disso e com a estratégia base funcionando, desenvolver as demais
int readDIP(){
  int n=0;
  if(digitalRead(DIP4)==HIGH)
    n=1;
  if(digitalRead(DIP3)==HIGH)
    n|= (1<<1); //Bit shift (0001 --> 0010), ou seja, n =  n + 2
  if(digitalRead(DIP2)==HIGH)
    n|= (1<<2); //Bit shift (0001 --> 0100), ou seja, n =  n + 4
  if(digitalRead(DIP1)==HIGH)
    n|= (1<<3); //Bit shift (0001 --> 1000), ou seja, n =  n + 8
  return n; // Valor retornado de n
}

int calibrateLine(){

  int valorLimite;

  valorLimite = (analogRead(lineR) + analogRead(lineL))/2;

  return valorLimite;
}

/*ACERCA DOS SENSORES DE LINHA*/
//Os sensores de linha retornam um valor de 0 à 1000
//Quanto mais próximo de 0, mais a refletância -> superfície clara
//Assim, para detectar a linha do dojô -> infR < Limite e infL < Limite

void loop() {
  start = digitalRead(microST);           //verifica se o micro-start foi ligadopleo controle do juiz, iniciando luta
  if(start==1){                           //se foi ligado o rb6o de movimenta
    digitalWrite(LED, HIGH);              //liga o LED  
    infR = analogRead(lineR);             //O output do sensor de linha é do tipo analógico
    infL = analogRead(lineL);             //O output do sensor de linha é do tipo analógico
    switch (readDIP()) //Leitura da chave DIP por meio da função readDIP
    {
      case 15: //Chave DIP 1111 (Estratégia Padrão)
        if (infR > infR_min && infL > infL_min){            //caso os sensores IR inferiores não detectem a linha 
          frontR = digitalRead(distR);        //realiza a leitura dos 2 sensores frontais
          frontL = digitalRead(distL);
          if((frontR==1)&&(frontL==1)){        //se os dois sensores frontais detectam o adversário
            MotorL(255);                       //o robô avança na direção do adversário
            MotorR(255);
          }
          else if((frontR==0)&&(frontL==1)){   //caso o sensor frontal da direita pare de detectar o adversário
            MotorL(0);                         //o robô para o motor esquerdo e  
            MotorR(255);                       //gira o motor direito tentando identificar novamente o adversário
          }
          else if ((frontR==1)&&(frontL==0)){  //caso o sensor frontal da direita pare de detectar o adversário
            MotorR(0);                         //o robô para o motor direito e  
            MotorL(255);                       //gira o motor esquerdo tentando identificar novamente o adversário
          }
          else{
            MotorL(255);                       //caso o robô não esteja detectando o adversário ele irá girar em sentido horário
            MotorR(-255);                      //com ambos os motores em máxima potência
          }
        }
        else{                                  //caso o robô detecte a borda do dojo
          MotorL(-255);                        //ele irá recuar pra não cair para fora
          MotorR(-255);
        }
        break;
      case 14: //Chave DIP 1110 (Estratégia - A2 - Abertura em U)
        if(aberturaFoiFeita = false){          //Verifica se o movimento de abertura foi realizado
          //Avançar
          MotorL(255);
          MotorR(255);
          delay(20);
          //Girar
          MotorL(255);
          MotorR(-128);
          delay(30);
          aberturaFoiFeita = true;
        }
        if(frontL == 1 && frontR == 1){        //Adversário detectado na frente do robô (Seguir em frente)
          MotorL(255);
          MotorR(255);
        }
        else if(frontL == 1 && frontR == 0){   //Adversário detectado na esquerda (Virar para a esquerda)
          MotorL(-128);
          MotorR(255);
        }
        else if(frontL == 0 && frontR == 1){   //Adversário detectado na direita (Virar para a direita)
          MotorL(255);
          MotorR(-128);
        }
        //Adversário não encontrado, checar se o mini não está saindo do dojô
        else if(infL < infL_min && infR > infR_min){       //Linha do dojô detectada na esquerda
          MotorL(-255);
          MotorR(-255);
          delay(300);
          //Giro para sair da beirada
          MotorL(255);
          MotorR(-255);
          delay(400);
        }
         else if(infL > infL_min && infR < infR_min){      //Linha do dojô detectada na direita
          MotorL(-255);
          MotorR(-255);
          delay(300);
          //Giro para sair da beirada
          MotorL(-255);
          MotorR(-255);
          delay(400);
        }
         else if(infL < infL_min && infR < infR_min){      //Linha do dojô detectada pelos dois sensores
          MotorL(-255);
          MotorR(-255);
          delay(300);
          //Giro para sair da beirada
          MotorL(255);
          MotorR(-255);
          delay(400);
        }
        else{                                  //Nenhuma das condições anteriores foram satisfeitas (Andar para frente)
          MotorL(255);
          MotorR(255);
        }
        break;
      case 13://Chave DIP 1101 (Pe D Valsa) (Inclinação 4)
              //Aqui ele luta com movimento de curva e acelerações mais suaves para pegar o oponente na surpresa

        if (infR > infR_min && infL > infL_min)
        {                              //caso os sensores IR inferiores não detectem a linha
          frontR = digitalRead(distR); //realiza a leitura dos 2 sensores frontais
          frontL = digitalRead(distL);
          if ((frontR == 1) && (frontL == 1))
          {              //se os dois sensores frontais detectam o adversário
            MotorL(255); //o robô avança na direção do adversário
            MotorR(255);
          }
          else if ((frontR == 0) && (frontL == 1))
          {               //caso o sensor frontal da direita pare de detectar o adversário
            MotorL(-200); //o robô para o motor esquerdo e
            MotorR(200);  //gira o motor direito tentando identificar novamente o adversário
          }
          else if ((frontR == 1) && (frontL == 0))
          {               //caso o sensor frontal da direita pare de detectar o adversário
            MotorR(-200); //o robô para o motor direito e
            MotorL(200);  //gira o motor esquerdo tentando identificar novamente o adversário
          }
          else
          {
            MotorL(150); //caso o robô não esteja detectando o adversário ele irá stalkear em sentido antihorário
            MotorR(200); //dá pra achar valores de giro mais bacanas aqui
          }
        }
        else
        {               //caso o robô detecte a borda do dojo
          MotorL(-160); //ele irá recuar pra não cair para fora
          MotorR(-160);
        }
        break;
      case 12://Chave DIP 1100 (De ladinho) (Posição B1 ou B5) (meme)
              //Nisso vai pra frente e pra trás perpendicular ao oponente full meme
        MotorL(255);
        MotorR(255);

        delay(300); //tem que acertar o timing pra ele ficar dentro da arena

        MotorL(-255);
        MotorR(-255);

        delay(300);
        break;
      case 11://Chave DIP 1011 (Dibraldinho) (Inclinação 1) (Meme)
              //Esse só foge dando voltas antihorárias na arena
        if (infR > infR_min)
        { //caso os sensor IR da esquerda n detecte a linha

          MotorL(160); //ele ta livre pra dibrar, andando em circulos
          MotorR(120);
        }
        else
        { //caso o robô detecte a borda do dojo, faz uma baliza pra não morrer
          MotorL(-100);
          MotorR(-120);
        }
        break;
      case 10://Chave DIP 1010 (Assassino) (Heteroflex)
              //Essa aqui é uma abordagem diferente do padrao, com prioridade diferente. Isso é pra fazer com que ele prefira matar a se defender
        frontR = digitalRead(distR); //realiza a leitura dos 2 sensores frontais
        frontL = digitalRead(distL);
        if ((frontR == 1) && (frontL == 1))
        {              //se os dois sensores frontais detectam o adversário
          MotorL(255); //o robô avança puto na direção do adversário
          MotorR(255);
        }
        else if ((frontR == 0) && (frontL == 1) && (infL > infL_min))
        {              //caso o sensor frontal da direita pare de detectar o adversário e o sensor da linha esquerda nao apite
          MotorL(120); //o robô reduz a velocidade do motor esquerdo e
          MotorR(255); //gira o motor direito tentando identificar novamente o adversário
        }
        else if ((frontR == 1) && (frontL == 0) && (infR > infR_min))
        {              //caso o sensor frontal da direita pare de detectar o adversário e o sensor da linha direita nao apite
          MotorR(120); //o robô reduz a velocidade do motor direito e
          MotorL(255); //gira o motor esquerdo tentando identificar novamente o adversário
        }
        else
        {
          MotorL(-200); //caso o robô não esteja detectando o adversário ele irá girar de ré
          MotorR(-255); //em sentido horário com uma potência braba
        }
        break;
      case 9://Chave DIP 1001 (O Uzinho Invertido) (Posição b3)
            //Esse aqui é o uzinho do Marco só que bait, sai de ré na abertura pra ver se o cara se mata
        if (aberturaFoiFeita = false)
        { //Verifica se o movimento de abertura foi realizado
          //Recuar
          MotorL(-255);
          MotorR(-255);
          delay(20);
          //Girar
          MotorL(-255);
          MotorR(-120);
          delay(200);
          //Recuar
          MotorL(-255);
          MotorR(-255);
          delay(20);
          aberturaFoiFeita = true;
        }
        if (frontL == 1 && frontR == 1)
        { //Adversário detectado na frente do robô (Seguir em frente)
          MotorL(255);
          MotorR(255);
        }
        else if (frontL == 1 && frontR == 0)
        { //Adversário detectado na esquerda (Virar para a esquerda)
          MotorL(-128);
          MotorR(255);
        }
        else if (frontL == 0 && frontR == 1)
        { //Adversário detectado na direita (Virar para a direita)
          MotorL(255);
          MotorR(-128);
        }
        //Adversário não encontrado, checar se o mini não está saindo do dojô
        else if (infL < infL_min && infR > infR_min)
        { //Linha do dojô detectada na esquerda
          MotorL(-255);
          MotorR(-255);
          delay(300);
          //Giro para sair da beirada
          MotorL(255);
          MotorR(-255);
          delay(400);
        }
        else if (infL > infL_min && infR < infR_min)
        { //Linha do dojô detectada na direita
          MotorL(-255);
          MotorR(-255);
          delay(300);
          //Giro para sair da beirada
          MotorL(-255);
          MotorR(-255);
          delay(400);
        }
        else if (infL < infL_min && infR < infR_min)
        { //Linha do dojô detectada pelos dois sensores
          MotorL(-255);
          MotorR(-255);
          delay(300);
          //Giro para sair da beirada
          MotorL(255);
          MotorR(-255);
          delay(400);
        }
        else
        { //Nenhuma das condições anteriores foram satisfeitas (Andar para frente)
          MotorL(255);
          MotorR(255);
        }
        break;
    }
  }
  else{                                     // assume que o robô deve ficar parado e está sob piso preto
    digitalWrite(LED, LOW);                 // LED desligado 
    MotorL(0);                              // motores parados 
    MotorR(0);                              // depois cofere o menor valor detectado quando o sensor esta detectando a cor preta
    infR = analogRead(lineR);              // lê o valor atual do sensor de linha
    if(infR<infR_min){
      infR_min=infR;
    }
    infL = analogRead(lineL);
    if(infL<infL_min){
      infL_min=infL;
    }
  }
}