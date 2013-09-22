/*  --------------------------------

     Arduino Telephone Pulse Dialer
          by: Michael R. Wally

        Michael.R.Wally@gmail.com

    --------------------------------  */





/*  ========================
     HARDWARE CONFIGURATION
    ========================  */

/*

    Relay Pin
    =========
  
    The Arduino pin number for the pin physically
    connected to the mechenical relay.

*/

int relayPin = 13;





/*  ========================
     SOFTWARE CONFIGURATION
    ========================  */

/*

    Dial String
    ===========
  
    The exact sequence to be dialed.

    Valid characters are:

      0-9   Dial Pulse Digits
      W     Wait (Fixed Delay)
      R     Random Wait (Random Delay)
      F     Flash (Hookswitch Flash)

*/

const char dialString[] = "18002255288";



/*

    Pre-Dial Delay
    ==============
  
    How long to wait, after taking the phone offhook,
    before starting the dialing sequence.

*/

int preDialDelay =  3000;
        // Default: 3000 (3 seconds)



/*

    Post-Dial Delay
    ===============
  
    How long to wait after dialing the phone
    number before disconnecting the call.

*/

long postDialDelay = 60000;
         // Default: 60000 (1 minute)



/*

    Pulse Break
    ===========
  
    The duration of dial pulses to transmit.
    This is normally 61ms in the United States.

*/

int pulseBreak = 61;
     // Default: 61 (EIA Standard RS-470)



/*

    Pulse Make
    ==========
  
    The duration between dial pulses to transmit.
    This is normally 39ms in the United States.
  
*/

int pulseMake = 39;
    // Default: 39 (EIA Standard RS-470)



/*

    Interdigit Time
    ===============
  
    The duration between dial pulse digits.
    This is normally between 300-700ms in the United States.
  
*/

int interdigitTime = 700;
         // Default: 700



/*

    Intercall Time
    =============
  
    The duration between calls.
  
*/

long intercallTime = 600000;
         // Default: 600000 (10 minutes)





/*  =====================
     MISC. CONFIGURATION
    =====================  */

const char version[] = "0.01";
int callCount = 0;





void setup() {

  // Set Relay PIN
  pinMode(relayPin, OUTPUT);

  // Ensure Phone Line is On Hook
  digitalWrite(relayPin, LOW);
  
  // Initialize the Serial Port
  Serial.begin(9600);

  // Boot Message
  Serial.println("\n\n\n\n\n");
  Serial.println("Arduino Telephone Pulse Dialer");
  Serial.print("Version ");
  Serial.println(version);
  Serial.println();

}



void loop() {

  // Tick Call Counter
  callCount++;
 
  // Announce Call
  serialPrintUptime();
  Serial.print("---------- CALL #");
  Serial.print(callCount);
  Serial.println(" ----------");

  // Take Phone Line Off Hook
  serialPrintUptime();
  Serial.print("Going offhook... ");

  digitalWrite(relayPin, HIGH);

  Serial.println("DONE");

  
  // Execute Pre-Dial-Delay
  serialPrintUptime();
  Serial.print("Executing pre-dial delay... ");
  
  delay(preDialDelay);
  
  Serial.println("DONE");


  for (int digit = 0; digit < (sizeof(dialString) - 1); digit++) {
    
    serialPrintUptime();
    Serial.print("NOW PULSING ");
    Serial.print(dialString[digit]);
    Serial.print(": ");

    dialDigit(dialString[digit]); 
    delay(interdigitTime);
    
    Serial.println();

  }
 
  // Execute Post-Dial-Delay
  serialPrintUptime();
  Serial.print("Executing post-dial delay... ");
  
  delay(postDialDelay);
  
  Serial.println("DONE");

  // Extra whitespace before loop
  Serial.println();
 
}



void dialDigit(char digit) {
  
  switch(digit) {
   
    case '1': pulse(); break;
    case '2': pulse(); pulse(); break;
    case '3': pulse(); pulse(); pulse(); break;
    case '4': pulse(); pulse(); pulse(); pulse(); break;
    case '5': pulse(); pulse(); pulse(); pulse(); pulse(); break;
    case '6': pulse(); pulse(); pulse(); pulse(); pulse(); pulse(); break;
    case '7': pulse(); pulse(); pulse(); pulse(); pulse(); pulse(); pulse(); break;
    case '8': pulse(); pulse(); pulse(); pulse(); pulse(); pulse(); pulse(); pulse(); break;
    case '9': pulse(); pulse(); pulse(); pulse(); pulse(); pulse(); pulse(); pulse(); pulse(); break;
    case '0': pulse(); pulse(); pulse(); pulse(); pulse(); pulse(); pulse(); pulse(); pulse(); pulse(); break;
    
  }
  
}


void pulse() {

  digitalWrite(relayPin, LOW);
  delay(pulseBreak);

  Serial.print("*");

  digitalWrite(relayPin, HIGH);
  delay(pulseMake);
  
}

void serialPrintUptime() {
  
  unsigned long uptime = millis();
  
  int days = uptime / 86400000;
  int hours = (uptime % 86400000) / 3600000;
  int minutes = ((uptime % 3600000) % 86400000) / 60000;
  int seconds = (((uptime % 3600000) % 86400000) % 60000) / 1000;

  char uptimeOutput[30];
  sprintf(uptimeOutput, "[%02dd %02d:%02d:%02d] ", days, hours, minutes, seconds);

  Serial.print(uptimeOutput);

}
