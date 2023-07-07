#include <LedControl.h>

LedControl lc = LedControl(12, 11, 10, 2); // (dataPin, clockPin, csPin, totalDevices)

#define VRX_PIN  A0 // Arduino pin connected to VRX pin
#define VRY_PIN  A1 // Arduino pin connected to VRY pin

int speaker_pin   = 7;//pino sons
int start_button  = 6;//pino botão comecar


int x = 0; // To store value of the X axis
int y = 0; // To store value of the Y axis
#define lim_esq -1
#define lim_dir 8

#define lim_baix 16


//Espaco vazio
#define vazio 0
//Espaco ocupado
#define cheio 1

//Partes bordas peças
#define bAmbos 2
#define bEsq 3
#define bDir 4
#define nlado 5

//Matriz tela (Talvez mudar para int )
int Tela[16][8] = {  {0, 0, 0, 0, 0, 0, 0, 0}, //1
  {0, 0, 0, 0, 0, 0, 0, 0}, //2
  {0, 0, 0, 0, 0, 0, 0, 0}, //3
  {0, 0, 0, 0, 0, 0, 0, 0}, //4
  {0, 0, 0, 0, 0, 0, 0, 0}, //5
  {0, 0, 0, 0, 0, 0, 0, 0}, //6
  {0, 0, 0, 0, 0, 0, 0, 0}, //7
  {0, 0, 0, 0, 0, 0, 0, 0}, //8
  {0, 0, 0, 0, 0, 0, 0, 0}, //9
  {0, 0, 0, 0, 0, 0, 0, 0}, //10
  {0, 0, 0, 0, 0, 0, 0, 0}, //11
  {0, 0, 0, 0, 0, 0, 0, 0}, //12
  {0, 0, 0, 0, 0, 0, 0, 0}, //13
  {0, 0, 0, 0, 0, 0, 0, 0}, //14
  {0, 0, 0, 0, 0, 0, 0, 0}, //15
  {0, 0, 0, 0, 0, 0, 0, 0}
};//16

int formas[15][4][4] = {
  // T
  { {0, 0, 0, 0},
    {bEsq, nlado, bDir, 0},
    {0, bAmbos, 0, 0},
    {0, 0, 0, 0}
  },

  { {0, bAmbos, 0, 0},
    {bEsq, bDir, 0, 0},
    {0, bAmbos, 0, 0},
    {0, 0, 0, 0}
  },

  { {0, bAmbos, 0, 0},
    {bEsq, nlado, bDir, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },

  { {0, bAmbos, 0, 0},
    {0, bEsq, bDir, 0},
    {0, bAmbos, 0, 0},
    {0, 0, 0, 0}
  },
  // []
  { {0, bEsq, bDir, 0},
    {0, bEsq, bDir, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },
  // L
  { {0, bAmbos, 0, 0},
    {0, bAmbos, 0, 0},
    {0, bEsq, bDir, 0},
    {0, 0, 0, 0}
  },

  { {0, 0, 0, 0},
    {bEsq, nlado, bDir, 0},
    {bAmbos, 0, 0, 0},
    {0, 0, 0, 0}
  },

  { {0, bEsq, bDir, 0},
    {0, 0, bAmbos, 0},
    {0, 0, bAmbos, 0},
    {0, 0, 0, 0}
  },

  { {0, 0, bAmbos, 0},
    {bEsq, nlado, bDir, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },

  // _/-
  { {0, bEsq, bDir, 0},
    {bEsq, bDir, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },

  { {bAmbos, 0, 0, 0},
    {bEsq, bDir, 0},
    {0, bAmbos, 0, 0},
    {0, 0, 0, 0}
  },

  { {0, bAmbos, 0, 0},
    {bEsq, bDir, 0},
    {bAmbos, 0, 0, 0},
    {0, 0, 0, 0}
  },
  // -\_
  { {bEsq, bDir, 0, 0},
    {0, bEsq, bDir, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },

  // |
  { {0, bAmbos, 0, 0},
    {0, bAmbos, 0, 0},
    {0, bAmbos, 0, 0},
    {0, bAmbos, 0, 0}
  },

  { {0, 0, 0, 0},
    {bEsq, nlado, nlado, bDir},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  }
};

int rot;


short posx = 3;
short posy = -4;

//index forma atual
int fi = 0;

void NovaForma() {}
void ChecaLinha() {}

int delai = 200;

void desenharF() {
  Serial.print("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA ");
  int yt = posy;// variaveis auxiliares para impedir perda de dados de posição
  int xt = posx;
  Serial.print(yt); Serial.print(" ");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (formas[fi][i][j] > 1) {

        //Tela[posy + i][posx + j] = formas[fi][i][j];
        //checa se ta dentro da tela
        if (posy + i >= 0) {
          Tela[posy + i][posx + j] = 2;
        }

        //Caso não seja peça checa se é espaço vazio
      } else {
        if (posy + i >= 0 && posy + i <= 15 && Tela[posy + i][posx + j] > 1) {
          Tela[posy + i][posx + j] = 0;
        }
      }
    }
  } Serial.print(posy); Serial.print(" ");
  Serial.print(yt); Serial.print(" ");
  //LIMPAR LINHA DA TELA EMCIMA DA FORMA
  if (yt > 0) {
    for (int i = 0; i < 8; i++) {
      if (Tela[yt - 1][i] > 1) {
        Tela[yt - 1][i] = 0;
      }
    }
  } Serial.print(posy); Serial.print(" ))");
  //limpar lados a esquerda/direita da matriz
  for (int i = 0; i < 4; i++) {
    //dir
    if (posx + 4 <= 7 && Tela[posy + i][posx + 4] > 1) {
      Tela[yt + i][posx + 4] = 0;
    }
    //esq
    if (posx - 1 >= 0 && Tela[posy + i][posx - 1] > 1) {
      Tela[yt + i][posx - 1] = 0;
    }
  }
  posy = yt;
  posx = xt;
  Serial.print(posy); Serial.print(" ))");
  printScr();
  delayGame(delai, posx);

}

//Abaixa tudo apartir dessa linha pra cima
void AbaixarAcima(int dely) {
  for (int i = dely; i > 0; i--) {
    for (int j = 0; j < 8; j++) {
      Tela[i][j] =  Tela[i - 1][j];
    }
  }
}

int score = 0;
void(* resetFunc) (void) = 0;//declare reset function at address 0

void movedown() {
  int move = 1;

  //backup para peça removida
  int auxfi = fi;
  int aux[4][4];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      aux[i][j] = formas[fi][i][j];
    }
  }

  for (int i = 0; i < 4; i++) {

    for (int j = 0; j < 4; j++) {

      //Checa se embaixo tem peças ou se chegou no final da tela
      if (formas[fi][i][j] > 1) {
        //Serial.print(" DEBUG1 ");
        //Serial.print(Tela[posy + i + 1][posx + j]);
        if (posy + i + 1 > lim_baix - 1 || Tela[posy + i + 1][posx + j] == 1) {

          move = 0;
          if (posy > -1){
          for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 8; j++) {
              //Serial.print(" ");Serial.print(Tela[posy + i][posx + j]);

              if ( Tela[i][j] >= 1) {
                Tela[i][j] = 1;
              }
            }
          }


          /*          //Checar se fileira fez ponto(esperar proxima interação ou tocar animação)
            int countL = 0;
            for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
              if (formas[fi][i][j] >= 1) {
                for(int h=0;h < 8;h++){
                  if(Tela[posy + i][posx + h] ==1){
                  countL++;
                  }
                }
                if(countL==8){
                  //Mexer peças de cima
                  //AbaixarAcima(i+posy);
                  Serial.print("PONTO");
                  break;
                }
              }
            }
            }*/
          //Limpa tela direito
          for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
              if (Tela[posy + i][posx + j] >= 1) {
                Tela[posy + i][posx + j] = 1;
              }
            }
          }

          //Checak por ponto
          int countL = 0;
          for (int i = 0; i < 16; i++) {
            if (Tela[i][0] == 1) {

              for (int h = 0; h < 8; h++) {
                if (Tela[i][h] == 1) {
                  countL++;
                }
              }
              if (countL == 8) {
                //Mexer peças de cima
                AbaixarAcima(i);
                score+=100;
                delayGame(delai / 2, posx);
              }
              countL = 0;
            }

          }


          //Perdeu o jogo
          }else{
            Serial.print("<<<<<");
              Serial.print(score);
             Serial.print(">>>>>>");
              delay(1000);
              resetFunc();
          }
          //Criar nova forma========
          /*if (fi >= 14) {
            fi = 0;
            } else {
            //Serial.print("DEBUG2");
            fi++;
            }*/
          fi = random(15);
          //Serial.print("DEBUG3");


          posy = -4;
          posx = 3;


          break;
        }
      }
    }
    if (move == 0) {
      break;
    }
  }
  if (move == 1) {

    posy += 1;
  }
  desenharF();

  
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        formas[auxfi][i][j] = aux[i][j];
      }
    }
  
}

void printScr() {
  for (int j = 0; j < 8; j++) {
    Serial.print("=");
  }
  Serial.print("\n");

  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 8; j++) {
      if (Tela[i][j] > 1) {
        Serial.print("X");
      } else if (Tela[i][j] == 1) {
        Serial.print("O");
      } else {
        Serial.print(" ");
      }
    }
    Serial.print("\n");
  }
}

void moveEsq() {
  int move = 1;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (formas[fi][i][j] == bAmbos || formas[fi][i][j] == bEsq) {
        if ((j - 1 + posx) < 0 || Tela[posy + i][posx + j - 1] != 0) {
          move = 0;
          break;
        }
      }
    }
  }
  if (move == 1) {
    posx--;
    desenharF();
  }
  movedown();
}
void moveDir() {
  int move = 1;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (formas[fi][i][j] == bAmbos || formas[fi][i][j] == bDir) {
        if ((j + 1 + posx) > 7 || Tela[posy + i][posx + j + 1] != 0) {

          move = 0;
          break;
        }
      }
    }
  }
  if (move == 1) {
    posx++;
    desenharF();
  }
  movedown();

}

//Converter Tela em formato inteiro para bytes
byte Tbin[16];
void TelaBin() {
  for (int i = 0; i < 16; i++) {
    float som = 0;
    for (int j = 0; j < 8; j++) {
      int bit = 0;
      if (Tela[i][7 - j] > 0) {
        bit = 1;
      }
      //Formula para converter em byte o +0.1 é por conta do pow ser um float e truncar com 1 amenos
      som = som + bit * (pow(2, j) + + 0.1);
    }
    Tbin[i] = (int)som ;
    //Serial.println(Tbin[i]);
  }
}

//Atualiza tela display
void disp() {
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  for (int rownum = 0; rownum < 8; rownum++)
  {
    lc.setRow(0, rownum, Tbin[rownum]);
    lc.setRow(1, rownum, Tbin[rownum + 8]);
  }
}


//MUSICA
int length = 99;
char notes[] = "EbCDCbaaCEDCbbCDECaa DFAGFEECEDCbbCDECaa EbCDCbaaCEDCbbCDECaa DFAGFEECEDCbbCDECaa ECDbCab ECDbCEAJ ";
int beats[] =      // Som
{
  2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 2, 2, 2, 4, 2,
  2, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 2, 2, 2, 4, 1,
  2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 2, 2, 2, 4, 2,
  2, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 2, 2, 2, 4, 1,
  5, 5, 5, 5, 5, 5, 7, 2, 5, 5, 5, 5, 2, 2, 5, 5, 3
};
int tempo = 128;        // Tempo


void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speaker_pin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speaker_pin, LOW);
    delayMicroseconds(tone);
  }
}


void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' , 'D', 'E', 'F', 'G', 'J', 'A', 'B'};
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956, 850, 760, 716, 637, 603, 568, 507 };

  for (int i = 0; i < 14; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

int mi = 0;
//Delay atualizando constantemente leds do display
void delayGame(int del, int x) {
  Serial.print(x); Serial.print(" ");
  long int t1 = millis();
  Serial.print(x); Serial.print(" ");
  TelaBin();
  Serial.print(x); Serial.print(" ");
  disp();
   if (notes[mi] == ' ')
      {
        delay(beats[mi] * del/4);   //Pausa
      }
      else{
        playNote(notes[mi], beats[mi] * del/3);
      
      }/*
  while (millis() - t1 < del) {
    
  }*/
  Serial.println(x);
  mi+=1;
  if(mi > length){
    mi = 0;
    }
}


//Girar a peça
void rotate() {
  int nexti = -1;
  if (fi > -1 && fi < 3) {
    nexti = fi + 1;
  } else if (fi == 3) {
    nexti = 0;
  } else if (fi > 4 && fi < 8) {
    nexti = fi + 1;
  } else if (fi == 8) {
    nexti = 5;
  } else if (fi == 9) {
    nexti =  fi + 1;
  } else if (fi == 10) {
    nexti = 9;
  } else if (fi == 11) {
    nexti =  fi + 1;
  } else if (fi == 12) {
    nexti = 11;
  } else if (fi == 13) {
    nexti = fi + 1;
  } else if (fi == 14) {
    nexti = 13;
  }
  int rodar = 1;
  if (nexti != -1) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (formas[nexti][i][j] > 1) {
          if (posy + i < 0 || posy + i > 7 || Tela[posy + i][posx + j] == 1 ) {

            rodar = 0;
            break;
          }
        }
      }
      if (rodar == 0) {
        break;
      }
    }

  } else {
    nexti = fi;
  }


  if (rodar == 1) {
    fi = nexti;
    desenharF();
    delayGame(delai / 2, posx);
  }
  movedown();
}


void splashScreen()
{
     int up[] =
     {
         B11101110,   //   o o o   o o o
         B01001000,   //     o     o
         B01001100,   //     o     o o
         B01001000,   //     o     o
         B01001110,   //     o     o o o
         B00000000,   //
         B11101110,   //   o o o   o o o
         B01001010    //     o     o   o
     };
 
     int down[] =
     {
         B01001100,   //     o     o o
         B01001010,   //     o     o   o
         B01001001,   //     o     o     o
         B00000000,   //
         B01000111,   //     o       o o o
         B01000100,   //     o       o
         B01000010,   //     o         o
         B01001110    //     o     o o o
     };
 
     for(int rownum = 0; rownum < 8; rownum++)
     {
         lc.setRow(0,rownum,up[rownum]);
         lc.setRow(1,rownum,down[rownum]);
     }
}


void setup() {
  //speaker
  pinMode(speaker_pin,  OUTPUT);
 pinMode(start_button, INPUT_PULLUP);

  //config leds
  lc.shutdown(0, false);
  lc.shutdown(1, false);

  lc.setIntensity(0, 5);
  lc.setIntensity(1, 5);

  lc.clearDisplay(0);
  lc.clearDisplay(1);

  Serial.begin(9600);
  randomSeed(analogRead(A2));

}

int started = 0;
int sound = 1;
void loop() {
  if (started == 0)
  {
    splashScreen();
    for (int i = 0; i < length; i++)
    {

      if (digitalRead(start_button) == LOW)
      {
        started = 1;
        break;
      }
      if (notes[i] == ' ')
      {
        delay(beats[i] * tempo);   //Pausa
      }
      else
      {
        if (sound == 1) {
          playNote(notes[i], beats[i] * tempo);
        }
        else
        {
          digitalWrite(speaker_pin, LOW);
        }
      }


      delay(tempo / 2);
    }
  }



  else
  {
    x = analogRead(VRX_PIN);
    y = analogRead(VRY_PIN);

    // print data to Serial Monitor on Arduino IDE
    /*Serial.print("x = ");
      Serial.print(x);
      Serial.print(", y = ");
      Serial.println(y);*/

    if (y > 800 ) {
      moveDir();
    } else if (y < 100) {
      moveEsq();
    } else if (x < 100) {
      delai = 50;
      movedown();
    } else if (x > 800) {
      rotate();
    }
    else {
      movedown();
    }
    delai = 500;
  }
}








