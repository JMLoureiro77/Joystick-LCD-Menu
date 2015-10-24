/*  
    Joytick & LCD & Menu   by JMLoureiro77

    http://jmloureiro77.blogspot.com.es

  328P Flash 5462 (17%) RAM 409(19%)

 - LCD SIN parpadeo
 - Sistema de menus SIN libreria
 - Joystick una pulsacion con retardo
 - editMode para introducir valores en Menu 1 

CC-BY 3.0 */

//---------------------------------------------------
// 1. Librerias
//---------------------------------------------------
#include <Wire.h>
#include <LiquidCrystal_I2C.h> 

//----------------------------------------------------
// 2. Pines
//----------------------------------------------------
#define xPin     A1   
#define yPin     A0   
#define kPin      7   
//SDA            A4
//SCL            A5

//----------------------------------------------------
// 3. Variables y Comandos
//----------------------------------------------------
int tCount1;
bool refresh;//lcd clear On/Off
//leerJoystick
int joyRead;
int joyPos; 
int lastJoyPos;
long lastDebounceTime = 0; 
long debounceDelay = 70;                 //user define
//Control Joystick
bool PQCP;
bool editMode;
//sistema de menus
int mNivel1;  
int mNivel2;  
//editmode
byte n[19];
int lastN;
int lcdX;
//int lcdY;
bool exiT;
//----------------------------------------------------
// 4. Objetos
//----------------------------------------------------
LiquidCrystal_I2C lcd(0x27,16,2); 

//====================================================
// SETUP
//====================================================
void setup() {
//----------------------------------------------------
// S1. Pines
//----------------------------------------------------
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(kPin, INPUT_PULLUP);
//----------------------------------------------------
// S2. Objetos
//----------------------------------------------------
  lcd.init();
  lcd.backlight(); 
}

//====================================================
// LOOP
//====================================================
void loop() {
  controlJoystick();
  menu();
/*  if (millis()%50==0){
    tCount1++;}
  if (tCount1>1000){tCount1=0;}*/
}

//====================================================
// Menu
//====================================================
void menu(){
  switch (mNivel1){
    case 0:
      menu0();//pantalla de inicio
    break;
    case 1:
        if(mNivel2==1){menu11();
        }else if (mNivel2==2){menu12();
        }else if (mNivel2==3){menu13();}
        else{menu1();}
    break;
    case 2:
        if(mNivel2==1){menu21();
        }else if (mNivel2==2){menu22();
        }else if (mNivel2==3){menu23();}
        else{menu2();}
    break;
    case 3:
      menu3(); //
    break;
  }//switch
}
//----------------------------------------------------
// Pantalla de inicio
//----------------------------------------------------
void menu0(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print("Pantalla Inicio");
}
//----------------------------------------------------
// Menu 1
//----------------------------------------------------
void menu1(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print("Menu 1");
  lcd.setCursor(0,1);
//++++++++++++++++++++
  while(editMode){
    controlJoystick();
    lcd.setCursor(lcdX,1);
    if(n[lcdX]!=lastN){
      lcd.print(n[lcdX]);
      lastN=n[lcdX];
    }
  }
}
//-------------------------------------------------1.1
void menu11(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print("Menu 1.1");
}
//-------------------------------------------------1.2
void menu12(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print("Menu 1.2");
}
//-------------------------------------------------1.3
void menu13(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print("Menu 1.3");
}
//----------------------------------------------------
// Menu 2
//----------------------------------------------------
void menu2(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print("Menu 2");
}
//-------------------------------------------------2.1
void menu21(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print("Menu 2.1");
}
//-------------------------------------------------2.2
void menu22(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print("Menu 2.2");
}
//-------------------------------------------------2.3
void menu23(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print("Menu 2.3");
}
//----------------------------------------------------
// Menu 3
//----------------------------------------------------
void menu3(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print("Menu 3");
  switch (mNivel2){
    case 0:
      return;
    case 1:
      menu31();
    break;
    case 2:
      menu32();
    break;
    case 3:
      menu33();
    break;
    default:
    break;
  }//switch
}
//-------------------------------------------------3.1
void menu31(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print("Menu 3.1");
}
//-------------------------------------------------3.2
void menu32(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print("Menu 3.2");
}
//-------------------------------------------------3.3
void menu33(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print("Menu 3.3");
}
//====================================================
// Control Joystic
//====================================================
void controlJoystick(){
  leeJoystick();
  if(PQCP) {
    PQCP=0;
    if (joyPos==5){editMode=!editMode;}
    switch (editMode){
       case 1: 
          lcd.blink();
          if (joyPos==4&&n[lcdX]<9){n[lcdX]++;   //arriba
              refresh=0;}
          if (joyPos==3&&n[lcdX]>0){n[lcdX]--;   //abajo
              refresh=0;} 
          if (joyPos==1&&lcdX<19){lcdX++;        //derecha
            refresh=0;}
          if (joyPos==2&&lcdX>0){lcdX--;         //izq
           refresh=0;}
        break;
        case 0:
          lcd.noBlink();
          if (mNivel1<3&&joyPos==3){mNivel1++;    //abajo
            refresh=1;
            mNivel2=0;}
          if (mNivel1>0&&joyPos==4){mNivel1--;    //arriba
            mNivel2=0;
            refresh=1;}
          if (mNivel2<3&&joyPos==1){mNivel2++;   //derecha
            refresh=1;}
          if (mNivel2>0&&joyPos==2){mNivel2--;    //izq
           refresh=1;}
        }//!edit
  }//PQCP
}
int leeJoystick(){
  int x = analogRead(xPin);
  int y = analogRead(yPin);
  int k = digitalRead(kPin);
    if(x>900){joyRead=1;        //x+
    }else if(x<100){joyRead=2;  //x-
    }else if(y>900){joyRead=3;  //y+
    }else if(y<100){joyRead=4;  //y-
    }else if(!k){joyRead=5;
    }else{joyRead=0;}

  if (joyRead != lastJoyPos){lastDebounceTime = millis();}
  if(((millis() - lastDebounceTime) > debounceDelay)&&(joyRead!=joyPos)){
    joyPos=joyRead;
    if(!PQCP){PQCP=1;}
    }
  if(((millis() - lastDebounceTime) > (5*debounceDelay))&&(joyPos==3||joyPos==4)){
    joyPos=joyRead;                     //repeat time
    if(!PQCP){PQCP=1;}
    }
  lastJoyPos=joyRead;
}