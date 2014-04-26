/*
  CoreTemp Arduino Code,
  by pvyParts (Aaron Kable)
  
  Current LCD layout
  
  16 x 2 LCD
  
              \/ Core Speed  
  AMD FX-8x #.#Ghz
  $##$ |||||||||||
  ^temp     ^load graph  
  
  TODO - Not Hard Code CPU Type
  TODO - make graph work a little better.
     
  started from the Serial Event Demo Code.
 */
#include <LiquidCrystal.h>

//my lcd is the freetronics LCD Shield
LiquidCrystal lcdGPU(7, 6, 5, 4, 3, 2);
LiquidCrystal lcdCPU(7, 8, 5, 4, 3, 2);
//setup the special chars for hte graph

  byte line5[8] =
  {// full
    B11111, B00000, B11111, B11111, B11111, B11111, B00000, B11111
  };

  byte line4[8] =
  {// 80%
    B11111, B00000, B11110, B11110, B11110, B11110, B00000, B11111
  };

  byte line3[8] =
  {// 60%
    B11111, B00000, B11100, B11100, B11100, B11100, B00000, B11111
  };

  byte line2[8] =
  {// 40%
    B11111, B00000, B11000, B11000, B11000, B11000, B00000, B11111
  };

  byte line1[8] =
  { //  20%
    B11111, B00000, B10000, B10000, B10000, B10000, B00000, B11111
  };

  byte line0[8] =
  { // empty
    B11111, B00000, B00000, B00000, B00000, B00000, B00000, B11111
  };
  
  byte loadchar[8] =
  { // % char to match graph
    B11111, B00000, B10010, B00100, B01000, B10010, B00000, B11111
  };
  byte tempchar[8] =
  { // termometer
    B00100, B01010, B01010, B01010, B01010, B10001, B10001, B01110
  };
  byte corechar[8] =
  { //  not used as the LCD can only do so many chars...
    B00000, B01010, B11111, B01110, B01110, B11111, B01010, B00000
  };

String inputString = "";  

boolean NewData = false;  // update Lcd flag

// holder strings
String gpu0tmp = "", gpu1tmp = "", ram = "", cputmp="";
int gpuLoad = 0, cpuLoad = 0;

void setup() {
  // setup LCD
  lcdGPU.begin(16, 2);
  lcdCPU.begin(16, 2);
  // initialize serial:
  Serial.begin(19200);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  
  //make the graph chars
  lcdGPU.createChar(6, line0); // cant use 0
  lcdGPU.createChar(1, line1);
  lcdGPU.createChar(2, line2);
  lcdGPU.createChar(3, line3);
  lcdGPU.createChar(4, line4);
  lcdGPU.createChar(5, line5);
  lcdGPU.createChar(7, loadchar);
  lcdGPU.createChar(8, tempchar);
  lcdCPU.createChar(6, line0); // cant use 0
  lcdCPU.createChar(1, line1);
  lcdCPU.createChar(2, line2);
  lcdCPU.createChar(3, line3);
  lcdCPU.createChar(4, line4);
  lcdCPU.createChar(5, line5);
  lcdCPU.createChar(7, loadchar);
  lcdCPU.createChar(8, tempchar);
  //lcd.createChar(9, corechar); // too many chars


}
int iHome = 1;
int diff = 1;
int count = 5; // delay for anim (move every *count* updates) // not used anymore
int dely = 0;


//draw the graph - load is a %, witdh is the length in lcd Chars
void graph(int load, int width, int screen){
  // cell = 100/size
  // show the % one
  // loop for full
  // show partial
  // loop for empty
  
  // weight of one cell in %
  float weight = 100/width;
  // total cells of current load
  float full_cells_f = load/weight;
  int full_cells = full_cells_f;
  // add fulls
  int i = 0;
  while(i<full_cells){
    if(screen == 0){lcdGPU.write(5);}
    else {lcdCPU.write(5);}
    i++;
  }
  // add scaled cell  
  int wt = weight;
  int test = (load-(wt*full_cells))/2;
  if (test == 0){
    test = 6;
  }
  //dont show a random blanck @ 100%
  if (load < 100){
    if(screen == 0){lcdGPU.write(test);}
    else{lcdCPU.write(test);}
  }
  i++;
  // add blanks
  while(i<width){
    if(screen == 0){lcdGPU.write(6);}
    else{lcdCPU.write(6);}
    i++;
  }
  
  }void loop() {
  // print the string when a newline arrives:
  if (NewData) {    
    //GPU
    lcdGPU.clear();
    lcdGPU.setCursor(0, 0);
    lcdGPU.print(gpuLoad);
    lcdGPU.print("%");
    lcdGPU.setCursor(4, 0);
    graph(gpuLoad,12, 0);
    lcdGPU.setCursor(0, 1);
    lcdGPU.write(8);
    lcdGPU.print(gpu0tmp);
    lcdGPU.print((char)223);
    lcdGPU.setCursor(9, 1);
    lcdGPU.write(8);
    lcdGPU.print(gpu1tmp);
    lcdGPU.print((char)223);
    //CPU
    lcdCPU.clear();
    lcdCPU.setCursor(0, 0);
    lcdCPU.print(cpuLoad);
    lcdCPU.print("%");
    lcdCPU.setCursor(4, 0);
    graph(cpuLoad,12, 1);
    lcdCPU.setCursor(0, 1);
    lcdCPU.print("RAM:");
    lcdCPU.print(ram);
    lcdCPU.print("%");
    lcdCPU.setCursor(12, 1);
    lcdCPU.write(8);
    lcdCPU.print(cputmp);
    inputString = "";
    NewData = false;
  }
}

int b[4];// b #0
// b0 -> b1 first break speed and avg load >> coreSpd
// b1+1 -> b2 max core temp >>coreTmp
// b2+1 -> end time >> coretime

// TODO - RAM usage and %
// Graphics LCD Graph each core load.

void serialEvent() {
  b[0] = 0;
    // clear the string:
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set the flag
    // so the main loop can do something about it:
    // update the strings to input.
    if (inChar == '\n') {
      b[1] = inputString.indexOf(';');
      b[2] = inputString.indexOf(';', b[1]+1);
      b[3] = inputString.indexOf(';', b[2]+1);
      b[4] = inputString.indexOf(';', b[3]+1);
      b[5] = inputString.indexOf(';', b[4]+1);
      b[6] = inputString.length()-1;
      gpu0tmp = inputString.substring(b[0],b[1]);
      gpu1tmp = inputString.substring(b[1]+1,b[2]);
      ram = inputString.substring(b[2]+1,b[3]);
      cputmp = inputString.substring(b[3]+1,b[4]);
      String loadHold = inputString.substring(b[4]+1,b[5]);
      gpuLoad = loadHold.toInt();
      loadHold = inputString.substring(b[5]+1,b[6]);
      cpuLoad = loadHold.toInt();
      
      NewData = true;
    } 
  }
  
}

