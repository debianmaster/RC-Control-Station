// **********************************************************
// ******************   Flysky Tx Code   *******************
//               by midelic on RCgroups.com 
//          Thanks to PhracturedBlue,ThierryRC
//          and Hasi for PPM code
// **********************************************************
// Serial interfacing by debianmaster on RCgroups.com
// Hardware:promini or aduino uno(M328)..or any setup with Atmega 328.
// this code it is not tested - for developement only.Use it on your own "risk".


#define Serial_BAUD_RATE 115200 //115.200 baud //Serial port speed for debugging

String splitString(String s, char parser,int index);

static const uint8_t A7105_regs[] = {
    0xff, 0x42, 0x00, 0x14, 0x00, 0xff, 0xff ,0x00, 0x00, 0x00, 0x00, 0x01, 0x21, 0x05, 0x00, 0x50,
    0x9e, 0x4b, 0x00, 0x02, 0x16, 0x2b, 0x12, 0x00, 0x62, 0x80, 0x80, 0x00, 0x0a, 0x32, 0xc3, 0x0f,
    0x13, 0xc3, 0x00, 0xff, 0x00, 0x00, 0x3b, 0x00, 0x17, 0x47, 0x80, 0x03, 0x01, 0x45, 0x18, 0x00,
    0x01, 0x0f, 0xff,
};
static const uint8_t tx_channels[16][16] = {
  {0x0a, 0x5a, 0x14, 0x64, 0x1e, 0x6e, 0x28, 0x78, 0x32, 0x82, 0x3c, 0x8c, 0x46, 0x96, 0x50, 0xa0},
  {0xa0, 0x50, 0x96, 0x46, 0x8c, 0x3c, 0x82, 0x32, 0x78, 0x28, 0x6e, 0x1e, 0x64, 0x14, 0x5a, 0x0a},
  {0x0a, 0x5a, 0x50, 0xa0, 0x14, 0x64, 0x46, 0x96, 0x1e, 0x6e, 0x3c, 0x8c, 0x28, 0x78, 0x32, 0x82},
  {0x82, 0x32, 0x78, 0x28, 0x8c, 0x3c, 0x6e, 0x1e, 0x96, 0x46, 0x64, 0x14, 0xa0, 0x50, 0x5a, 0x0a},
  {0x28, 0x78, 0x0a, 0x5a, 0x50, 0xa0, 0x14, 0x64, 0x1e, 0x6e, 0x3c, 0x8c, 0x32, 0x82, 0x46, 0x96},
  {0x96, 0x46, 0x82, 0x32, 0x8c, 0x3c, 0x6e, 0x1e, 0x64, 0x14, 0xa0, 0x50, 0x5a, 0x0a, 0x78, 0x28},
  {0x50, 0xa0, 0x28, 0x78, 0x0a, 0x5a, 0x1e, 0x6e, 0x3c, 0x8c, 0x32, 0x82, 0x46, 0x96, 0x14, 0x64},
  {0x64, 0x14, 0x96, 0x46, 0x82, 0x32, 0x8c, 0x3c, 0x6e, 0x1e, 0x5a, 0x0a, 0x78, 0x28, 0xa0, 0x50},
  {0x50, 0xa0, 0x46, 0x96, 0x3c, 0x8c, 0x28, 0x78, 0x0a, 0x5a, 0x32, 0x82, 0x1e, 0x6e, 0x14, 0x64},
  {0x64, 0x14, 0x6e, 0x1e, 0x82, 0x32, 0x5a, 0x0a, 0x78, 0x28, 0x8c, 0x3c, 0x96, 0x46, 0xa0, 0x50},
  {0x46, 0x96, 0x3c, 0x8c, 0x50, 0xa0, 0x28, 0x78, 0x0a, 0x5a, 0x1e, 0x6e, 0x32, 0x82, 0x14, 0x64},
  {0x64, 0x14, 0x82, 0x32, 0x6e, 0x1e, 0x5a, 0x0a, 0x78, 0x28, 0xa0, 0x50, 0x8c, 0x3c, 0x96, 0x46},
  {0x46, 0x96, 0x0a, 0x5a, 0x3c, 0x8c, 0x14, 0x64, 0x50, 0xa0, 0x28, 0x78, 0x1e, 0x6e, 0x32, 0x82},
  {0x82, 0x32, 0x6e, 0x1e, 0x78, 0x28, 0xa0, 0x50, 0x64, 0x14, 0x8c, 0x3c, 0x5a, 0x0a, 0x96, 0x46},
  {0x46, 0x96, 0x0a, 0x5a, 0x50, 0xa0, 0x3c, 0x8c, 0x28, 0x78, 0x1e, 0x6e, 0x32, 0x82, 0x14, 0x64},
  {0x64, 0x14, 0x82, 0x32, 0x6e, 0x1e, 0x78, 0x28, 0x8c, 0x3c, 0xa0, 0x50, 0x5a, 0x0a, 0x96, 0x46},
};
 
//Spi Comm.pins with A7105
  #define PPM_pin 2//PPM in 
  #define SDI_pin 5 //SDIO-D2 
  #define SCLK_pin 4 //SCK-D5
  //#define CS_pin 2//CS-D4
  #define CS_pin 6//CS-D6 //phil pinout
  
  
  //---------------------------------
  //#define  CS_on PORTD |= 0x04 //D2
  //#define  CS_off PORTD &= 0xFB //D2
  #define  CS_on PORTD |= 0x40 //D6 //phil pinout
  #define  CS_off PORTD &= 0xBF //D6 //phil pinout
  //
  #define  SCK_on PORTD |= 0x10//D4
  #define  SCK_off PORTD &= 0xEF//D4
  #define  SDI_on PORTD |= 0x20 //D5
  #define  SDI_off PORTD &= 0xDF //D5
 //
  #define  SDI_1 (PIND & 0x20) == 0x20 //D5
  #define  SDI_0 (PIND & 0x20) == 0x00 //D5
 //
  #define RED_LED_pin A3
  #define Red_LED_ON  PORTC |= _BV(3);
  #define Red_LED_OFF  PORTC &= ~_BV(3);
  #define NOP() __asm__ __volatile__("nop")
  
//########## Variables #################
static uint32_t id;//tx id, don't confuse with A7105 id
static uint8_t chanrow;
static uint8_t chancol;
static uint8_t chanoffset;
static uint8_t channel;
static byte counter=255;
static uint8_t aid[4];//for debug only
static uint8_t packet[21];//inside code there are 16....so don't bother
int mSpeed=1000;
int chnl=0;//ck
int ppm_test=1000;
String readString;
volatile uint16_t Servo_data[10] = {mSpeed,mSpeed,mSpeed,mSpeed,mSpeed,mSpeed,mSpeed,mSpeed};//8 channels//I think change to volatile if using in interrupt

void setup() {
  pinMode(RED_LED_pin, OUTPUT); 
  //RF module pins
  pinMode(PPM_pin, INPUT);//PPM input
  pinMode(SDI_pin, OUTPUT);//SDI   SDIO 
  pinMode(SCLK_pin, OUTPUT);//SCLK SCL 
  pinMode(CS_pin, OUTPUT);//CS output
  CS_on;//start CS high
  SDI_on;//start SDIO high
  SCK_off;//start sck low
  Serial.begin(Serial_BAUD_RATE);//for debugging
  
  uint8_t i;
  uint8_t if_calibration1;
  uint8_t vco_calibration0;
  uint8_t vco_calibration1;
  
  //
  //for debug 
  delay(10);//wait 10ms for A7105 wakeup
  //A7105_reset();//reset A7105
  _spi_write_adress(0x00,0x00);//reset A7105
  A7105_WriteID(0x5475c52A);//0x2Ac57554
  A7105_ReadID();//for debug only
  //Serial.print(aid[0],HEX);
  //Serial.print(aid[1],HEX);
  //Serial.print(aid[2],HEX);
  //Serial.print(aid[3],HEX);
  
  for (i = 0; i < 0x33; i++){
        if(A7105_regs[i] != 0xff)
            _spi_write_adress(i, A7105_regs[i]);

}
_spi_write_adress(0x02,0x01);
while(_spi_read_adress(0x02)){
if_calibration1=_spi_read_adress(0x22);
if(if_calibration1&0x10){//do nothing
}
}
//delay(10);// debug code wait for calib.
_spi_write_adress(0x24,0x13);
_spi_write_adress(0x26,0x3b);
_spi_write_adress(0x0F,0x00);//channel 0
_spi_write_adress(0x02,0x02);
while(_spi_read_adress(0x02)){
vco_calibration0=_spi_read_adress(0x25);
if(vco_calibration0&0x08){//do nothing
}
}
//delay(10);//debug code wait for calib.
_spi_write_adress(0x0F,0xA0);
_spi_write_adress(0x02,0x02);
while(_spi_read_adress(0x02)){
vco_calibration1=_spi_read_adress(0x25);
if(vco_calibration1&0x08){//do nothing
}
}
//delay(10);//debug code wait for calib.
_spi_write_adress(0x25,0x08);

id=0x30000006;//fixed TX ID(Thierry ID)
bind_Flysky();
chanrow=id % 16;
chanoffset=(id & 0xff) / 16;
chancol=0;
//PPM setup

attachInterrupt(0, read_ppm, CHANGE);
TCCR1A = 0;  //reset timer1
TCCR1B = 0;
TCCR1B |= (1 << CS11);  //set timer1 to increment every 0,5 us
//
}





//servodata timing range for flysky.
////-100% =~ 0x03e8//=1000us(min)
 //+100% =~ 0x07ca//=1994us(max)
 //Center = 0x5d9//=1497us(center)
//channel order AIL;ELE;THR;RUD;AUX1;AUX2;AUX3;AUX4

//############ MAIN LOOP ##############
void loop() {
  delayMicroseconds(1460);
  channel=tx_channels[chanrow][chancol]-chanoffset;
  _spi_strobe(0xA0);
  _spi_strobe(0xE0);
  _spi_write_adress(0x0F,channel);
  Write_Packet(0x55);//servo_data timing can be updated in interrupt(ISR routine for decoding PPM signal)
  _spi_strobe(0xD0);
  chancol = (chancol + 1) % 16;   
  while (Serial.available()) {
        delay(3);  //delay to allow buffer to fill 
        if (Serial.available() >0) {
          char c = Serial.read();  //gets one byte from serial buffer
          readString += c; //makes the string readString
        } 
  }   
  if (readString.length() >0) {
        if(readString.indexOf(",")>-1){ 
          String a=  splitString(readString,',',0);
          String e= splitString(readString,',',1);
          String t = splitString(readString,',',2);
          String r= splitString(readString,',',3);
          Servo_data[0]=a.toInt();
          Servo_data[1]=e.toInt();
          Servo_data[2]=t.toInt();
          Servo_data[3]=r.toInt();
          Serial.print("a=");Serial.println(a.toInt());
          Serial.print("e=");Serial.println(e.toInt());
          Serial.print("t=");Serial.println(t.toInt());
          Serial.print("r=");Serial.println(r.toInt());          
        }
        readString="";
  }       
}


void read_ppm(){  //leave this alone
  static unsigned int pulse;
  static unsigned long counterPPM;
  static byte chan;//channel
  Serial.println("in ppm");
  
  counterPPM = TCNT1;
  TCNT1 = 0;

  if(counter < 1020){  //must be a pulse if less than 510us
    pulse = counterPPM;
  }
  else if(counterPPM > 4000){  //sync pulses over 2000us
    chan = 0;
  }
  else{  //servo values between 510us and 2420us will end up here
    Servo_data[chan] = (counterPPM + pulse)/2;
    Serial.println(Servo_data[chan]);
    chan++;
  }
}

//BIND_TX
void bind_Flysky() {
  while(counter){//while counter//counter for 2.5 sec.
  _spi_strobe(0xA0);//strobe stand-by
  _spi_strobe(0xE0);//spi strobe reset tx fifo
  _spi_write_adress(0x0F,0x01);//sent on channel 1(Tx)
  Write_Packet(0xaa);//(bind packet)
  _spi_strobe(0xD0);//strobe Fifo Tx
  delay(10);//wait 10ms
  if (bitRead(counter,3)==1) 
  Red_LED_ON;
  if(bitRead(counter,3)==0)
  Red_LED_OFF;
  counter--;
  }
}

//-------------------------------
//-------------------------------
//A7105 SPI routines
//-------------------------------
//-------------------------------
void A7105_WriteID(uint32_t ida) {
 CS_off;
_spi_write(0x06);
_spi_write((ida>>24)&0xff); 
_spi_write((ida>>16)&0xff);
_spi_write((ida>>8)&0xff);
_spi_write((ida>>0)&0xff);
 CS_on;
}
void A7105_ReadID(){
uint8_t i;
 CS_off;
_spi_write(0x46);
for(i=0;i<4;i++){
aid[i]=_spi_read();
}
CS_on;
}
//----------------------
void Write_Packet(uint8_t init){//except adress(0x05)should sent to A7105 21 bytes totally)//I think this function can be improved
uint8_t i;
CS_off;
_spi_write(0x05);//TX/RX FIFO adress
_spi_write(init);//0xaa or 0x55(depend on bind packet or data packet)
_spi_write((id >>  0) & 0xff);
_spi_write((id >>  8) & 0xff);
_spi_write((id >>  16) & 0xff);
_spi_write((id >>  24) & 0xff);
for(i=0;i<8;i++){
packet[0+2*i]=lowByte(Servo_data[i]);//low byte of servo timing(1000-2000us)
packet[1+2*i]=highByte(Servo_data[i]);//high byte of servo timing(1000-2000us)
_spi_write(packet[0+2*i]);
_spi_write(packet[1+2*i]);
}
CS_on;
}

//---------------------------------
//-------------------------------------- 
void _spi_write(uint8_t command) {  
  uint8_t n=8; 
  SCK_off;
  SDI_off;
  while(n--) {
    if(command&0x80)
      SDI_on;
    else 
     SDI_off;
     SCK_on;
	 NOP();
	 SCK_off;
    command = command << 1;
  }
  SDI_on;
}  
void _spi_write_adress(uint8_t address, uint8_t data) {
   CS_off;
  _spi_write(address); 
   NOP();
  _spi_write(data);  
   CS_on;
} 
//-----------------------------------------
  uint8_t _spi_read(void) {
  uint8_t result;
  uint8_t i;
  result=0;
  pinMode(SDI_pin,INPUT);//make SDIO pin input
  //SDI_on;
  for(i=0;i<8;i++) {                    
	if(SDI_1)  ///if SDIO =1 
      result=(result<<1)|0x01;
	  else
	  result=result<<1;
    SCK_on;
    NOP();
	SCK_off;
	NOP();
  }
  pinMode(SDI_pin,OUTPUT);//make SDIO pin output again
  return result;
  }   
//--------------------------------------------
uint8_t _spi_read_adress(uint8_t address) { 
  uint8_t result;
  CS_off;
  address |=0x40; 
  _spi_write(address);
  result = _spi_read();  
  CS_on;
  return(result); 
} 
//------------------------
void _spi_strobe(uint8_t address) {
 CS_off;
_spi_write(address);
 CS_on;
}
//------------------------
void A7105_reset(void) {
  _spi_write_adress(0x00,0x00); 
}

String splitString(String s, char parser,int index){
  String rs='\0';
  int parserIndex = index;
  int parserCnt=0;
  int rFromIndex=0, rToIndex=-1;

  while(index>=parserCnt){
    rFromIndex = rToIndex+1;
    rToIndex = s.indexOf(parser,rFromIndex);

    if(index == parserCnt){
      if(rToIndex == 0 || rToIndex == -1){
        return '\0';
      }
      return s.substring(rFromIndex,rToIndex);
    }
    else{
      parserCnt++;
    }

  }
  return rs;
}












  



