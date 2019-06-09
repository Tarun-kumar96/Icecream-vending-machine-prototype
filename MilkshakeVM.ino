#include <Servo.h>

#include <EEPROM.h>

//------------------------------------ OUTPUTS --------------------------------

//---------------------- Icecream Handling ---------------------
const int electric_piston_f = 22;
const int electric_piston_b = 24;
const int icecream_door_o = 26;
const int icecream_door_c = 28;

//----------------------- Liquids Handling ---------------------
const int watervalve_cleaning = 30;
const int watervalve_icechamber = 32;
const int watervalve_dispensing = 34;

//----------------------- Solids Handling ----------------------
const int dcmotor_milkpowder = 36;
const int dcmotor_kitkat = 38;
const int dcmotor_oreo = 40;

//---------------------- Blender Handling ----------------------
const int servomotor_blender = 42;

//--------------------- Traversing Handling --------------------
const int steppermotor_traversing_x_f = 44;
const int steppermotor_traversing_x_b = 46;
const int steppermotor_traversing_y_f = 48;
const int steppermotor_traversing_y_b = 50;

//-------------------------Door Handling ----------------------
const int door_o = 52;
const int door_c = 53;

//-------------------- Crushed Ice Handling-------------------
const int steppermotor_ice_f = 51;
const int steppermotor_ice_b = 49;



//----------------------------------- INPUTS ---------------------------------------
const int kitkat_shake = 23;
const int oreo_shake = 25;
const int vanilla_icecream = 27;
const int water = 29;

//--------------------tech_input---------------------
const int door_irsensor = 31;
const int cup_irsensor = 33;
//const int wgt_sensor = ;
//const int wgt_sensor = ;
//const int wgt_sensor = ;



//----------------------------------- VARIABLES -------------------------------------

long t_KKS_W = 5000;
long t_KKS_IC = 5000;
long t_KKS_CI = 5000;
long t_KKS_KK = 5000;
long t_OS_W = 5000;
long t_OS_IC = 5000;
long t_OS_CI = 5000;
long t_OS_O = 5000;
long t_VIC_IC = 5000;
long t_ICD_O = 2000;
long t_ICD_C = 2000;
long t_W = 5000;
long t_blend = 5000;
long t_clean_blend = 5000;
int x_coord;
int y_coord;

//---------has individual conversion ratio but this time its ideal for all---------------
int conv_x;
int conv_y;


int milkpowder_x;
int milkpowder_y;
int water_x;
int water_y;
int crushedice_x;
int crushedice_y;
int kitkat_x;
int kitkat_y;
int oreo_x;
int oreo_y;
int blender_x;
int blender_y;
int dispense_x;
int dispense_y;
int icecream_x;
int icecream_y;



//---------------------------------- INTERNAL FUNCTIONS -------------------------------

bool sense(){
  if(digitalRead(door_irsensor)==HIGH){
    return true;
  }
  return false;
}

void wait(long t){  
  long r = 0;  
  while(r<=t){
    r += 50;    
    delay(50);
  }  
}

long convert_traverse_x(int x){
  long temp;
  temp = conv_x*x*1000;
  return temp;
  }

long convert_traverse_y(int x){
  long temp;
  temp = conv_y*x*1000;
  return temp;
}

void traverse(int x,int y){  
  int px = x_coord;
  int py = y_coord;  
  Serial.print(x_coord);
  if(x-px>0){
    delay(500);
    digitalWrite(steppermotor_traversing_x_f,HIGH);
    wait(convert_traverse_x(x-px));
    digitalWrite(steppermotor_traversing_x_f,LOW);
    delay(500);
  }
  else{
    delay(500);
    digitalWrite(steppermotor_traversing_x_b,HIGH);
    wait(convert_traverse_x(px-x));
    digitalWrite(steppermotor_traversing_x_b,LOW);
    delay(500);
  }  
  if(y-py>0){
    delay(500);
    digitalWrite(steppermotor_traversing_y_f,HIGH);
    wait(convert_traverse_y(y-py));
    digitalWrite(steppermotor_traversing_y_f,LOW);
    delay(500);
  }
  else{
    delay(500);
    digitalWrite(steppermotor_traversing_y_b,HIGH);
    wait(convert_traverse_y(py-y));
    digitalWrite(steppermotor_traversing_y_b,LOW);
    delay(500);
  }
  EEPROM.write(1,x);
  EEPROM.write(2,y);
  x_coord = x;
  y_coord = y;
}

void dispense_icecream(long t){
  delay(500);
  digitalWrite(icecream_door_o,LOW);
  wait(t_ICD_O);
  digitalWrite(icecream_door_o,HIGH);
  delay(500);
  digitalWrite(electric_piston_f,LOW);
  wait(t);
  digitalWrite(electric_piston_f,HIGH);
  delay(500);
  digitalWrite(icecream_door_c,LOW);
  wait(t_ICD_C);
  digitalWrite(icecream_door_c,HIGH);
  delay(500);
}

void dispense_crushedice(long t){
  delay(500);
  digitalWrite(steppermotor_ice_f,HIGH);
  wait(t);
  digitalWrite(steppermotor_ice_f,LOW);
  delay(500);
}

void dispense_water(long t){
  delay(500);
  digitalWrite(watervalve_dispensing,LOW);
  wait(t);
  digitalWrite(watervalve_dispensing,HIGH);
  delay(500);
}

void dispense_ingredient(int ingredient,long t){
  delay(500);
  digitalWrite(ingredient,HIGH);
  wait(t);
  digitalWrite(ingredient,LOW);
  delay(500);
}

void blend(){
  delay(500);
  digitalWrite(servomotor_blender,HIGH);
  wait(t_blend);
  digitalWrite(servomotor_blender,LOW);
  delay(500);
}

bool insert_base(){
  delay(500);
  long count = 0;
  bool flag = false;
  long flag1 = 0;
  Serial.println("              .........Hold your horses till the door opens shiva bitch.........          ");
  digitalWrite(door_o,HIGH);
  delay(5000);
  digitalWrite(door_o,LOW);
  Serial.println("                     .........Insert the fucking Cup shiva bitch.........          ");
  while(digitalRead(door_irsensor)==LOW){
    delay(50);
    count+=50;
  }
  if(count<=20000){
    Serial.println("            .........Remove your fucking hand shiva bitch.........          ");
    while(digitalRead(door_irsensor)==HIGH){
      delay(50);
    }
    digitalWrite(door_c,HIGH);
    delay(5000);
    digitalWrite(door_c,LOW);
    delay(1000);
    if(confirm_base()){
      Serial.println("            .........You have successfully placed the cup. Good job shiva Doggie.........          ");
      delay(2000);
      return true;
    }  
  }
  Serial.println("       .........Bitch!! Shiva!!! You fucked up your money. You didn't place the cup MF.........          ");
  return false;
}

bool confirm_base(){
  bool flag = false;
  long count = 0;
  while(digitalRead(cup_irsensor)==HIGH){
    delay(50);
    count+=50;
    if(count==2000){
      flag = true;
      break;
    }
  }
  return flag;
}


void dispense_order(){
  Serial.println("       .........Hey! Asshole. Your order is ready.........          ");
  long count = 0;
  delay(500);
  digitalWrite(door_o,HIGH);
  delay(5000);
  digitalWrite(door_o,LOW);
  delay(500);
  while(digitalRead(cup_irsensor)==HIGH){
    delay(50);
    count+=50;
  }
  if(count<30000){
    while(digitalRead(door_irsensor)==HIGH){
      delay(50);
    }
    Serial.println("       .........Come Again! Dont be a bitch.........          ");
    digitalWrite(door_c,HIGH);
    delay(5000);
    digitalWrite(door_c,LOW);
  }
  else{
    Serial.println("       .........Dont look here and there MF. Follow the procedure to get your order.........          ");
    //---------------------------Procedure to be added------------------------------
    delay(3000);
  }
  Serial.println("       .........Ready to use fucker.........          ");
}


void clean_blender(){
  delay(500);
  Serial.println("       .........Processing.... Please Wait.........          ");
  digitalWrite(watervalve_cleaning,LOW);
  digitalWrite(servomotor_blender,HIGH);
  wait(t_clean_blend);
  digitalWrite(servomotor_blender,LOW);
  digitalWrite(watervalve_cleaning,HIGH);
  delay(500);  
  Serial.println("       .........Ready to use fucker.........          ");
}


//-----------------------------------------###########  111111  ############-------------------------------------
//-----------------------------------------###########  111111  ############-------------------------------------
//-----------------------------------------###########  111111  ############-------------------------------------
//-----------------------------------------###########  111111  ############-------------------------------------


void setup() {  

  Serial.begin(9600);
  
//-----------OUTPUT PINS----------------   
  pinMode(electric_piston_f,OUTPUT);
  pinMode(electric_piston_b,OUTPUT);
  pinMode(icecream_door_o,OUTPUT);
  pinMode(icecream_door_c,OUTPUT);
  pinMode(watervalve_cleaning,OUTPUT);
  pinMode(watervalve_icechamber,OUTPUT);
  pinMode(watervalve_dispensing,OUTPUT);
  pinMode(dcmotor_milkpowder,OUTPUT);
  pinMode(dcmotor_kitkat,OUTPUT);
  pinMode(dcmotor_oreo,OUTPUT);
  pinMode(servomotor_blender,OUTPUT);
  pinMode(steppermotor_traversing_x_f,OUTPUT);
  pinMode(steppermotor_traversing_x_b,OUTPUT);
  pinMode(steppermotor_traversing_y_f,OUTPUT);
  pinMode(steppermotor_traversing_y_b,OUTPUT);
  pinMode(door_o,OUTPUT);
  pinMode(door_c,OUTPUT);
  pinMode(steppermotor_ice_f,OUTPUT);
  pinMode(steppermotor_ice_b,OUTPUT);
  
//----------INPUT PINS------------------
  pinMode(kitkat_shake,INPUT);
  pinMode(oreo_shake,INPUT);
  pinMode(vanilla_icecream,INPUT);
  pinMode(water,INPUT);
  pinMode(door_irsensor,INPUT);
  pinMode(cup_irsensor,INPUT);

//-----------SET TO HIGH------------------
  digitalWrite(electric_piston_f,HIGH);
  digitalWrite(electric_piston_b,HIGH);
  digitalWrite(icecream_door_o,HIGH);
  digitalWrite(icecream_door_c,HIGH);
  digitalWrite(watervalve_cleaning,HIGH);
  digitalWrite(watervalve_icechamber,HIGH);
  digitalWrite(watervalve_dispensing,HIGH);
  digitalWrite(dcmotor_milkpowder,HIGH);
  digitalWrite(dcmotor_kitkat,LOW);
  digitalWrite(dcmotor_oreo,LOW);
  digitalWrite(servomotor_blender,LOW);
  digitalWrite(steppermotor_traversing_x_f,LOW);
  digitalWrite(steppermotor_traversing_x_b,LOW);
  digitalWrite(steppermotor_traversing_y_f,LOW);
  digitalWrite(steppermotor_traversing_y_b,LOW);
  digitalWrite(door_o,LOW);
  digitalWrite(door_c,LOW);
  digitalWrite(steppermotor_ice_f,LOW);
  digitalWrite(steppermotor_ice_b,LOW);
  
//-----------SET TO LOW------------------
  digitalWrite(kitkat_shake,LOW);
  digitalWrite(oreo_shake,LOW);
  digitalWrite(vanilla_icecream,LOW);
  digitalWrite(water,LOW);
  digitalWrite(door_irsensor,LOW);
  digitalWrite(cup_irsensor,LOW);

  conv_x = EEPROM.read(3);
  conv_y = EEPROM.read(4);
  milkpowder_x = EEPROM.read(11);
  milkpowder_y = EEPROM.read(12);
  water_x = EEPROM.read(13);
  water_y = EEPROM.read(14);
  crushedice_x = EEPROM.read(15);
  crushedice_y = EEPROM.read(16);
  kitkat_x = EEPROM.read(17);
  kitkat_y = EEPROM.read(18);
  oreo_x = EEPROM.read(19);
  oreo_y = EEPROM.read(20);
  blender_x = EEPROM.read(21);
  blender_y = EEPROM.read(22);
  dispense_x = EEPROM.read(23);
  dispense_y = EEPROM.read(24);
  icecream_x = EEPROM.read(25);
  icecream_y = EEPROM.read(26);
  
  Serial.println("       .........Ready to use fucker.........          ");
}


//-----------------------------------------###########  222222  ############-------------------------------------
//-----------------------------------------###########  222222  ############-------------------------------------
//-----------------------------------------###########  222222  ############-------------------------------------
//-----------------------------------------###########  222222  ############-------------------------------------


void loop() {
  
  int b1 = digitalRead(kitkat_shake);
  int b2 = digitalRead(oreo_shake);
  int b3 = digitalRead(vanilla_icecream);
  int b4 = digitalRead(water);
  int b5 = digitalRead(door_irsensor);
  bool trig_blend = false;
  x_coord = EEPROM.read(1);
  y_coord = EEPROM.read(2);

  //------------------------------------------------------Kitkat Shake------------------------------------------------
  if(b1 == HIGH){
    while(b1 == HIGH){
      b1 = digitalRead(kitkat_shake);
    }
    
    //---------------###### Door Handling ######--------------
    if(insert_base()){
          
      //---------------###### 1.Icecream ######--------------
      Serial.println("Traversing icecream");
      Serial.print((String)"("+x_coord+","+y_coord+")");Serial.print("   -->   ");Serial.print((String)"("+icecream_x+","+icecream_y+")");
      traverse(icecream_x,icecream_y);
      delay(1000);
      Serial.println("Dispensing icecream");
      dispense_icecream(t_KKS_IC);
      delay(1000);
      
      //---------------###### 2.Crushedice ######--------------
      Serial.println("Traversing crushed ice");
      Serial.print((String)"("+x_coord+","+y_coord+")");Serial.print("   -->   ");Serial.print((String)"("+crushedice_x+","+crushedice_y+")");
      traverse(crushedice_x,crushedice_y);
      delay(1000);
      Serial.println("Dispensing crushed ice");
      dispense_crushedice(t_KKS_CI);
      delay(1000);
      
      //---------------###### 3.Water ######--------------
      Serial.println("Traversing water");
      Serial.print((String)"("+x_coord+","+y_coord+")");Serial.print("   -->   ");Serial.print((String)"("+water_x+","+water_y+")");
      traverse(water_x,water_y);
      delay(1000);
      Serial.println("Dispensing water");
      dispense_water(t_KKS_W);
      delay(1000);
      
      //---------------###### 4.Kitkat ######--------------
      Serial.println("Traversing kitkat");
      Serial.print((String)"("+x_coord+","+y_coord+")");Serial.print("   -->   ");Serial.print((String)"("+kitkat_x+","+kitkat_y+")");
      traverse(kitkat_x,kitkat_y);
      delay(1000);
      Serial.println("Dispensing kitkat");
      dispense_ingredient(dcmotor_kitkat,t_KKS_KK);
      delay(1000);
      
      //---------------###### 5.Blend ######--------------
      Serial.println("Traversing blender");
      Serial.print((String)"("+x_coord+","+y_coord+")");Serial.print("   -->   ");Serial.print((String)"("+blender_x+","+blender_y+")");
      traverse(blender_x,blender_y);
      delay(1000);
      Serial.println("Blender is blending..");
      blend();
      delay(1000);
      trig_blend = true;

      //-------------------Dispense-------------------
      dispense_order();
    }
  }



  //---------------------------------------------------- Oreo Shake ---------------------------------------------------------
  if(b2 == HIGH){
    while(b2 == HIGH){
      b2 = digitalRead(oreo_shake);
    }

    //---------------###### Door Handling ######--------------
    if(insert_base()){
    
      //---------------###### 1.Icecream ######--------------
      
      traverse(icecream_x,icecream_y);
      delay(1000);
      dispense_icecream(t_OS_IC);
      delay(1000);
      
      //---------------###### 2.Crushedice ######--------------
      
      traverse(crushedice_x,crushedice_y);
      delay(1000);
      dispense_crushedice(t_OS_CI);
      delay(1000);
      
      //---------------###### 3.Water ######--------------
      
      traverse(water_x,water_y);
      delay(1000);
      dispense_water(t_OS_W);
      delay(1000);
      
      //---------------###### 4.Oreo ######--------------
      
      traverse(oreo_x,oreo_y);
      delay(1000);
      dispense_ingredient(dcmotor_oreo,t_OS_O);
      delay(1000);
      
      //---------------###### 5.Blend ######--------------
      
      traverse(blender_x,blender_y);
      delay(1000);
      blend();
      delay(1000);
      trig_blend = true;

      //-------------------Dispense-------------------
      dispense_order();
      
    }
  }

  

  //------------------------------------------------------- Vanilla Icecream --------------------------------------------------  
  if(b3 == HIGH){
    while(b3 == HIGH){
      b3 = digitalRead(vanilla_icecream);
    }

    //---------------###### Door Handling ######--------------
    if(insert_base()){
    
      //---------------###### 1.Icecream ######--------------
      
      traverse(icecream_x,icecream_y);
      delay(1000);
      dispense_icecream(t_VIC_IC);
      delay(1000);
      
      //---------------###### 2.Toppings ######--------------
      //------------------Work in progress-----------------
      traverse(kitkat_x,kitkat_y);
      delay(1000);
      dispense_ingredient(dcmotor_oreo,t_OS_O);
      delay(1000);  

      //-------------------Dispense-------------------
      dispense_order();
      
    }
    Serial.println("       .........Ready to use fucker.........          ");
  }



  //-------------------------------------------------------------- Water ------------------------------------------------------  
  if(b4 == HIGH){
    while(b4 == HIGH){
      b4 = digitalRead(water);
    }

    //---------------###### Door Handling ######--------------
    if(insert_base()){
    
      //---------------###### 1.Water ######--------------
          
      traverse(water_x,water_y);
      delay(1000);
      dispense_water(t_W);
      delay(1000); 

      //-------------------Dispense-------------------
      dispense_order();
      
    }    
    Serial.println("       .........Ready to use fucker.........          ");
  }



 //---------------------------------------------------------- Blender Cleaning -------------------------------------------------- 
  if(trig_blend){
    clean_blender();
  }
    
}
