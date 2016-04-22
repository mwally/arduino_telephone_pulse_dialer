/*  --------------------------------

     Arduino Telephone Pulse Dialer
      Version 0.9: April 22, 2016
          by: Michael R. Wally

        Michael.R.Wally@gmail.com

            This is free and
         unencumbered software
             released into
           the public domain.

    --------------------------------  */

/*  ========================
     HARDWARE CONFIGURATION
    ========================  */

/*

    Output Pins
    ===========

    The microcontroller pin numbers for the pins
    physically connected to the output device(s).

*/

const unsigned int outputPins  = 4;           // The quantity of output devices
const unsigned int outputPin[] = {4,5,6,7};   // Individual pin numbers for each device

/*  ========================
     SOFTWARE CONFIGURATION
    ========================  */

/*

    RNG Seed Value
    ==============

    The Seed Value for the Random Number Generator.
    Please change this before deploying your Dialer.

    Valid values are from 0 to 4,294,967,295.

*/

const unsigned long rngSeed = 1234567890;
                  // Maximum: 4294967295

/*

    Dial Strings
    ============

    The exact sequence(s) to be dialed.

    Valid characters are:

      0-9   Telephone Number Digits
      ,     Fixed Delay
      ~     Random Delay
      !     Hookswitch Flash

*/

const unsigned int dialStrings  = 2;   // The quantity of Dial Strings
const char*        dialString[] = {    // The individual  Dial Strings

  "14122458085",
  "00000000000"

};

/*

    Dial Random
    ===========

    If set to TRUE, execute the dialStrings
    in a pseudorandom order.

    If set to FALSE, execute the dialStrings
    in the exact order listed above.
 
*/

const boolean dialRandom = true;
               // Default: true


/*

    Pre-Dial Delay
    ==============
  
    How long to wait, after taking the phone offhook,
    before starting a dialing sequence.

    Default: 3-5 Seconds.

*/

const unsigned int preDialDelayMin =  3000;
                          // Default: 3000 (3 seconds)

const unsigned int preDialDelayMax =  5000;
                          // Default: 5000 (5 seconds)

/*

    Post-Dial Delay
    ===============
  
    How long to wait after dialing the
    sequence before going on-hook again.
    
*/

const unsigned long postDialDelayMin = 60000;
                           // Default: 60000  (1 minute)

const unsigned long postDialDelayMax = 120000;
                           // Default: 120000 (2 minutes)


/*

    Intercall Delay
    ===============
  
    The duration between calls.
  
*/

const unsigned long intercallDelayMin = 10000;
                            // Default: 10000 (10 seconds)

const unsigned long intercallDelayMax = 20000;
                            // Default: 20000 (20 seconds)


/*

    Pulse Break
    ===========
  
    The duration of dial pulses to transmit.
    
    This is normally 61ms in the United States.
    This is normally 67ms in Europe.

*/

const unsigned int pulseBreak = 61;
                 // US Default: 61 (EIA Standard RS-470)
                 // EU Default: 67


/*

    Pulse Make
    ==========
  
    The duration between dial pulses to transmit.

    This is normally 39ms in the United States.
    This is normally 33ms in Europe.
  
*/

const unsigned int pulseMake = 39;
                // US Default: 39 (EIA Standard RS-470)
                // EU Default: 33





/*

    Interdigit Delay
    ================
  
    The duration between dial pulse digits.
    This is normally between 300-800ms in the United States.
  
*/

const unsigned int interdigitDelayMin = 800;
                            // Default: 800

const unsigned int interdigitDelayMax = 800;
                            // Default: 800






/*

    Hookflash Timings
    =================
  
    How long to open the circuit to indicate
    a "hookflash" signal to the remote switch.

    Typical values range from 300 to 800ms.
  
*/

const unsigned int flashBreak = 500;
const unsigned int flashMake  = 500;
                   // Defaults: 500








/*

    Dial Delay (Fixed)
    ==================
  
    How long to delay the dialString
    sequence for the ',' entry.
  
*/

const unsigned int dialDelayFixed = 1000;
                        // Default: 1000  (1 second)




/*

    Dial Delay (Random)
    ==================
  
    How long to delay the dialString
    sequence for the '~' entry.
  
*/


const unsigned int dialDelayMin = 1000;
                      // Default: 1000  (1 second)

const unsigned int dialDelayMax = 3000;
                      // Default: 3000  (3 seconds)









/*  ======
     BOOT
    ======  */

void setup() {

  // Seed the Random Number Generator
  randomSeed(rngSeed);

  // Configure the OUTPUT Pins
  for (unsigned int i = 0; i < outputPins; i++) {
    pinMode(outputPin[i], OUTPUT);
  }

}






/*  ===============
     DIAL SEQUENCE
    ===============  */

void loop() {
  for (unsigned int i = 0; i < dialStrings; i++) {

    if (dialRandom) { i = random(0,(dialStrings-1)); }

    offHook();
    delay(random(preDialDelayMin,preDialDelayMax));

    for (unsigned int digit = 0; digit < strlen(dialString[i]); digit++) {
      dialDigit(dialString[i][digit]);
      delay(random(interdigitDelayMin,interdigitDelayMax));
    }

    delay(random(postDialDelayMin,postDialDelayMax));
    onHook();

    delay(random(intercallDelayMin,intercallDelayMax));
      
  }
}








/*  =============
     DIAL DIGITS
    =============  */

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

    case '!': flash(); break;
    case ',': delayFixed(); break;
    case '~': delayRandom(); break;
    
  }
  
}


/*  ============
     DIAL DELAY
    ============  */

void delayFixed() {
  delay(dialDelayFixed);
}

void delayRandom() {
  delay(random(dialDelayMin,dialDelayMax));
}


/*  ===========
     SIGNALING
    ===========  */

void pulse() {
  onHook();
  delay(pulseBreak);
  offHook();
  delay(pulseMake); 
}

void flash() {
  onHook();
  delay(flashBreak);
  offHook();
  delay(flashMake);
}



/*  ===========
     LINE HOOK
    ===========  */

void offHook() {
  for (unsigned int i = 0; i < outputPins; i++) {
    digitalWrite(outputPin[i], HIGH);
  }
}

void onHook() {
  for (unsigned int i = 0; i < outputPins; i++) {
    digitalWrite(outputPin[i], LOW);
  }
}
