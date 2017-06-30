#include <IRremote.h>
#define LED 13
#define BAUDRATE 57600
#define DEBUGOUTPUT 0


#define powercontrol 10

// checksum variables
byte generatedChecksum = 0;
byte checksum = 0; 
int payloadLength = 0;
byte payloadData[64] = {
  0};
byte poorQuality = 0;
byte attention = 0;
byte meditation = 0;

// system variables
long lastReceivedPacket = 0;
boolean bigPacket = false;


IRsend irsend;

//command variables

unsigned long turnon=0x0001;
unsigned long intensity=0x0000;

//////////////////////////
// Microprocessor Setup //
//////////////////////////
void setup()
{
  pinMode(LED, OUTPUT);
  Serial.begin(BAUDRATE);           // USB
}

////////////////////////////////
// Read data from Serial UART //
////////////////////////////////
byte ReadOneByte() {
  int ByteRead;

  while(!Serial.available());
  ByteRead = Serial.read();

#if DEBUGOUTPUT  
  Serial.print((char)ByteRead);   // echo the same byte out the USB serial (for debug purposes)
#endif

  return ByteRead;
}
/////////////
//MAIN LOOP//
/////////////
void loop() {


  // Look for sync bytes
  if(ReadOneByte() == 170) {
    if(ReadOneByte() == 170) {

      payloadLength = ReadOneByte();
      if(payloadLength > 169)                      //Payload length can not be greater than 169
          return;

      generatedChecksum = 0;        
      for(int i = 0; i < payloadLength; i++) {  
        payloadData[i] = ReadOneByte();            //Read payload into memory
        generatedChecksum += payloadData[i];
      }   

      checksum = ReadOneByte();                      //Read checksum byte from stream      
      generatedChecksum = 255 - generatedChecksum;   //Take one's compliment of generated checksum

        if(checksum == generatedChecksum) {    

        poorQuality = 200;
        attention = 0;
        meditation = 0;

        for(int i = 0; i < payloadLength; i++) {    // Parse the payload
          switch (payloadData[i]) {
          case 2:
            i++;            
            poorQuality = payloadData[i];
            bigPacket = true;            
            break;
          case 4:
            i++;
            attention = payloadData[i];                        
            break;
          case 5:
            i++;
            meditation = payloadData[i];
            break;
          case 0x80:
            i = i + 3;
            break;
          case 0x83:
            i = i + 25;      
            break;
          default:
            break;
          } // switch
        } // for loop

#if !DEBUGOUTPUT


        if(bigPacket) {
          if(poorQuality != 0)
          {
            digitalWrite(LED, HIGH);//turn a red LED on if the quality is poor
            Serial.print("the quality is poor");
            Serial.println(poorQuality, DEC);
          }
          
          else
          {
             digitalWrite(LED, LOW);
          Serial.print(" Attention: ");
          Serial.print(attention, DEC);
          Serial.print(" Meditation: ");
          Serial.print(meditation, DEC);
          Serial.print(" Time since last packet: ");
          Serial.print(millis() - lastReceivedPacket, DEC);
          lastReceivedPacket = millis();
          Serial.print("\n");
      
   

           for(int i=0;i<2;i++)
            {
 
              irsend.sendSony(0x0000+attention,20);//send the command from pin 3
              delay(15);
    
            } 
          delay(1000); //5 second delay between each signal burst
          }
           

          
                           
        }
#endif        
        bigPacket = false;        
      }
      else {
        // Checksum Error
      }  // end if else for checksum
    } // end if read 0xAA byte
  }//end if read 0xAA byte
}//end of main loop






  

 

