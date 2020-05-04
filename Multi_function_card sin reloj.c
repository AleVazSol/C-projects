#INCLUDE <16f887.h>
#device ADC=8  
#fuses NOWDT,NOPROTECT,NOLVP,PUT,BROWNOUT  
#use delay(clock=/*1843200 2000000*/4000000)  
#INCLUDE <stdlib.h>  
#INCLUDE <math.h>
#include <string.h>
#use rs232(baud=9600, xmit=pin_c6, rcv=pin_c7, bits=8, parity=N)
 
#define i1 PIN_A0 
#define i2 PIN_A1
#define i3 PIN_A2
#define i4 PIN_A3
#define i5 PIN_A4
#define i6 PIN_A5
#define i7 PIN_E0 
#define i8 PIN_E1
#define i9 PIN_E2
#define i10 PIN_C3
#define i11 PIN_D0
#define i12 PIN_D1

#define o1 PIN_B5 
#define o2 PIN_B4
#define o3 PIN_B3
#define o4 PIN_B2
#define o5 PIN_B1
#define o6 PIN_B0
#define o7 PIN_D7 
#define o8 PIN_D6
#define o9 PIN_D5
#define o10 PIN_D4
#define o11 PIN_D3
#define o12 PIN_D2

float carga_del_timer,ciclo_de_trabajo,dc1,dc2;
float freq,DC1f,DC2f;
char frecuencia[6];
char serial[15];//variable donde se almacena lo que se recibio del serial
int PWM,recibi;//variable para seleccionar cual PWM se va a encender
char duty[5];
int32 cristal=/*1843200 2000000*/4 000 000;
int16 porcentaje1,porcentaje2; //valor final que se ingresa en el pwm
char salida[2];
char constante="";
char nombreentrada[3];
int n_salida;
int n_entrada;
int estado_salida;
int estado_entrada;
int b,r,caso_valido;
/*
      
                                  CCCCCCCCCC  AAAAAAAAA   VVV   VVV  SSSSSSSSS
                                  CCCCCCCCCC  AAAAAAAAA   VVV   VVV  SSSSSSSSS
                                  CCC         AAA   AAA   VVV   VVV  SSS
                                  CCC         AAAAAAAAA   VVV   VVV  SSSSSSSSS
                                  CCC         AAAAAAAAA   VVV   VVV       SSSS
                                  CCCCCCCCCC  AAA   AAA    VVV VVV   SSSSSSSSS
                                  CCCCCCCCCC  AAA   AAA     VVVVV    SSSSSSSSS

*/
void setup_pwm(){                                     // rutina para calcular los valores del PWM
carga_del_timer=64*freq;
carga_del_timer=(cristal/carga_del_timer)-1;
DC1f=dc1*(4*carga_del_timer+4);
DC2f=dc2*(4*carga_del_timer+4);
porcentaje1=(int16)DC1f;                              //el valor del ciclo de trabajo se convierte a entero ya que es el valor que acepta la funcion set_pwmx_duty
porcentaje2=(int16)DC2f;
setup_timer_2(T2_DIV_BY_16,(int)carga_del_timer,1);
set_pwm1_duty(porcentaje2);
set_pwm2_duty(porcentaje1);
}
 
 
void recepcion_frecuencia(){     //rutina para manejar los caracteres de la frecuencia
if(serial[1]=='?'){printf("F=%f\n\r",freq);}
else{
for (int o=0;o<5;o++){
 frecuencia[o]=serial[1+o];
 }
 freq=atof(frecuencia);

 if(freq<10001 && freq>244){
  setup_pwm();
   printf("F=%f\n\r",freq);
  
 }
 else{
 printf("error FREQ\n\r");
}}}



void get_DC(){                               //rutina para preguntar el valor del dc de un PWM
switch (PWM){
      case 1: printf("DC=%1.3f\n\r",dc1); // se camb
      break;
      case 2: printf("DC=%1.3f\n\r",dc2);
      break;
}}


void recepcion_DC()                                   //rutina para manejar los caracteres del ciclo de trabajo ingresado
{
PWM=serial[3]-'0';                                    //se transforma el valor del PWM a numero y se guarda en la variable PWM
if (PWM>0 && PWM<3){
 if (serial[4]=='D'&& (PWM==1||PWM==2) )               //se llena el string de duty con el valor del ciclo de trabajo
 {
 if(serial[5]=='?'){get_DC();}
 else{
  for (int i=0;i<3;i++){
   duty[i]=serial[5+i];
   }
   ciclo_de_trabajo=atof(duty);                       //se convierte a valor numerico el ciclo de trabajo
   if(ciclo_de_trabajo>100){ciclo_de_trabajo=100;}
   if(ciclo_de_trabajo <101 && ciclo_de_trabajo>=0){    //si es un valor de 0 a 100
    ciclo_de_trabajo/=100;                            //se divide entre 100 porque es un porcentaje
    switch (PWM){
      case 1: dc1=ciclo_de_trabajo;
      break;
      case 2: dc2=ciclo_de_trabajo;
      break;
     }
    printf("PWM=%i DC=%1.3f\n\r",PWM,ciclo_de_trabajo); ////se manda una respuesta al comando 
    setup_pwm();                                        //se llama a la rutina para hacer setup del pwm
   }                                                    //
   else {printf("errorSELECPWM\n\r"); }
 }
 }
 else{printf("errorPWM\n\r");}                                //comando para errores
 
 }
else{ printf("error\n\r"); }
}



void set_salidas(){                            //activa o sesactiva la salida dependiendo del comando enviado

if (estado_salida==0){                        //si el comando que se envio fue para apagar 
  switch (n_salida){
   case 1: output_low(o1);
   break;
   case 2: output_low(o2);
   break;
   case 3: output_low(o3);
   break;
   case 4: output_low(o4);
   break;
   case 5: output_low(o5);
   break;
   case 6: output_low(o6);
   break;
   case 7: output_low(o7);
   break;
   case 8: output_low(o8);
   break;
   case 9: output_low(o9);
   break;
   case 10: output_low(o10);
   break;
   case 11: output_low(o11);
   break;
   case 12: output_low(o12);
   break;
   }
}
if (estado_salida==1){                                 //si el comando que envio fue para encender
  switch (n_salida){
   case 1: output_High(o1);
   break;
   case 2: output_high(o2);
   break;
   case 3: output_high(o3);
   break;
   case 4: output_high(o4);
   break;
   case 5: output_high(o5);
   break;
   case 6: output_high(o6);
   break;
   case 7: output_high(o7);
   break;
   case 8: output_high(o8);
   break;
   case 9: output_high(o9);
   break;
   case 10: output_high(o10);
   break;
   case 11: output_high(o11);
   break;
   case 12: output_high(o12);
   break;
   }   

}

}


void salidas(){                    // determina cual salida se selecciono y a que estado se va a cambiar
r=1;                              // variable que ayuda a determinar si la salida que se va a activar es de 1 o 2 digitos
for( b=1;b<3;b++){                //for para almacenar el numero de salida seleccionada
 if (serial[b]=='O'){
  salida[b-1]=constante;    //la constante es un caracter vacio para borrar dato anterior
 }
 else{
  salida[b-1]=serial[b];
  r++;
 }
}
n_salida=atoi(salida);
 if (n_salida>0 && n_salida<13){
  switch (serial[r+1]){
   case 'F':estado_salida=0;        //si es F quiere decir que usuario ingreso OFF
    printf("%s OFF\n\r",salida);
    set_salidas();
   break;
  case 'N':estado_salida=1;      //si es N quiere decir que usuario ingreso ON
   printf("%s ON\n\r",salida);
   set_salidas();
  break;
   default:printf("errorCASESALID\n\r");
  break;
}
}
else {printf("errorSALID\n\r");}
}



void set_entrada(){                           //lee el valor de la entrada seleccionada y la almacena en la variable estado_entrada
  switch (n_entrada){
   case 1: estado_entrada=input_state(i1);
   break;
   case 2: estado_entrada=input_state(i2);
   break;
   case 3: estado_entrada=input_state(i3);
   break;
   case 4: estado_entrada=input_state(i4);
   break;
   case 5: estado_entrada=input_state(i5);
   break;
   case 6: estado_entrada=input_state(i6);
   break;
   case 7: estado_entrada=input_state(i7);
   break;
   case 8: estado_entrada=input_state(i8);
   break;
   case 9: estado_entrada=input_state(i9);
   break;
   case 10: estado_entrada=input_state(i10);
   break;
   case 11: estado_entrada=input_state(i11);
   break;
   case 12: estado_entrada=input_state(i12);
   break;
   }
 printf("IN_%i=%i\n\r",n_entrada,estado_entrada);
}

void entradas(){
nombreentrada[0]=serial[1];
nombreentrada[1]=serial[2];
n_entrada=atoi(nombreentrada);
if (n_entrada>0 && n_entrada<13){
set_entrada();
}
else{printf("errorENTR\n\r");}
}


void reset(){    //apaga todas las salidas y manda un 0 al duty cicle de los PWM
output_low(o1);
output_low(o2);
output_low(o3);
output_low(o4);
output_low(o5);
output_low(o6);
output_low(o7);
output_low(o8);
output_low(o9);
output_low(o10);
output_low(o11);
output_low(o12);
dc1=0;
dc2=0;
printf("Reset\n\r");
setup_pwm();
}

#int_RDA
void RDA_isr()
{
 gets(serial);
 recibi=1;
 //printf("esto es lo que recibi%s\n\r",serial);
}


void main(){
 freq=0;
 dc1=0;
 dc2=0;
 enable_interrupts(INT_RDA);
 enable_interrupts(GLOBAL);
 setup_ccp1(CCP_PWM); 
 setup_ccp2(CCP_PWM);
 
while(True){

 if(recibi==1){
  if(serial[0]=='P' && serial[1]=='W' && serial[2]=='M'){  //checa si el comando es para el pwm
   caso_valido=1;
   recepcion_DC();
   }  
  if(serial[0]=='F'){
   recepcion_frecuencia();
   caso_valido=1;
   }
  if(serial[0]=='I' && serial[1]=='D'){  
   printf("MFC1.0.0\n\r");
   caso_valido=1;
   } else{
     if(serial[0]=='I'){
     entradas();
     caso_valido=1;
   }
   }
  if(serial[0]=='R' && serial[1]=='S' && serial[2]=='T'){  
   caso_valido=1;
   reset();
   }
  if(serial[0]=='O'){
   caso_valido=1;
   salidas();
   }
  if(caso_valido==0){printf("errorCASOWHILE\n\r");}
  caso_valido=0;
  recibi=0;
  serial="";
  } 
 delay_ms(10);

 
}
}

