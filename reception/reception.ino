#include <IRremote.h>
#define LED_UTBM 3



int RECV_PIN = 10;

IRrecv irrecv(RECV_PIN);

decode_results results;

int intensity=0;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  results.decode_type=SONY;
}

void loop() {
  if (irrecv.decode(&results))
  {
    
    Serial.println(results.value,DEC);
    intensity=results.value;
    delay(50);
    irrecv.resume(); // Receive the next value
  }

   if(intensity<=255)
   {
    
    analogWrite(5,intensity);
   }
    
  if(intensity>=60)
  {
    analogWrite(6,255);
    delay(3000);
    analogWrite(6,0);
  }
}
