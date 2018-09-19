// postavuvanje konekcii za Keypad
char keypadPort at PORTD;

// postavuvanje konekcii za LCD

sbit LCD_RS at RB4_bit;
sbit LCD_EN at RB5_bit;
sbit LCD_D4 at RB0_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D7 at RB3_bit;
sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D4_Direction at TRISB0_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D7_Direction at TRISB3_bit;

int cnt ;
char kp;


void main() {
    unsigned short crveno;
    unsigned short zolto;
    unsigned short zeleno;
    short svetlo_vrednost ;
    short counter_podeseni = 0;
    short svetlo_flag = 0;
    short countms = 0;
    short enter_flag = 0;
    
    ANSEL = 0;
    ANSELH = 0;
    
    TRISB = 0b00000000;
    PORTB = 0b00000000;
    TRISB.B0 = 1;  // prekinuvach vlezen pin!
    PORTB.B0 = 1; // eksplicitno definiranje deka prekidachot e ON
    // semaforot na pochetokot ne raboti ! (treba da se vnesat vred vo EEPROM
    
    cnt = 0 ;
    Keypad_Init();
    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);
    
    
    
    while(1){
    
    if (PORTB.B0 == 1) {     // ako semaforot e isklucen , probaj vo slednata
   // Delay_1sec();            // iteracija
   // continue;

      do{
      kp = 0;
      kp = Keypad_Key_Click();
      
      switch(kp){

       case 0: kp = 0; break;
       case 1: kp = 1; break;
       case 2: kp = 2; break;
       case 3: kp = 3; break;
       case 4: kp = 4; break;
       case 5: kp = 5; break;
       case 6: kp = 6; break;
       case 7: kp = 7; break;
       case 8: kp = 8; break;
       case 9: kp = 9; break;
       case 10: kp = 100; break; // crveno
       case 11: kp = 101; break; // zolto
       case 12: kp = 102; break; // zeleno
       case 13: kp = 103; break;  // enter
       case 14: ; break;    // prazno (dva pati ima nula vo tabelata
       case 15: ; break; // prazno
       

      
      }
      if ( svetlo_flag == 0 ) {
       if (kp >= 100 && kp <=102 ){
       svetlo_flag = 1;
       svetlo_vrednost = kp;
       }
       else{
       continue;
       }
      
      }
      
      if (svetlo_flag == 1 && kp < 100 ){
            countms = countms * 10;
            countms = countms + kp;
      
      }
      
      if (svetlo_flag == 1 && kp == 103){
      EEPROM_Write(svetlo_vrednost - 100, countms); // crveno na lokacija 0x00
                                           // zolto na lokacija 0x01
                                          // zeleno na 0x02
      
      counter_podeseni = counter_podeseni + 1;
      // resetiraj gi ostanatite parametri

      svetlo_flag = 0;
      countms = 0;
      svetlo_vrednost = 0 ;
      
      
      }
      
      
      

   } while(counter_podeseni < 3);
	}
   counter_podeseni = 0;
    // semaforot e vkluchen

    crveno = EEPROM_Read(0x00);
    zolto = EEPROM_Read(0x01);
    zeleno = EEPROM_Read(0x02);
           // 76543210  -> redosled na bitovi
    PORTB = 0b00010000; // crveno
    Delay_ms(crveno);
    PORTB = 0b00100000; // zolto
    Delay_ms(zolto);
    PORTB = 0b01000000; // zeleno
    Delay_ms(zeleno);


    
    
    
    }

}