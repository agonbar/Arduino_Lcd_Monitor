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
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

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
String coreSpd = "";
String coretmp = "";
String coretime = "";
int coreLoad = 0;

void setup() {
  // setup LCD
  lcd.begin(16, 2);
  // initialize serial:
  Serial.begin(19200);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  
  //make the graph chars
  lcd.createChar(6, line0); // cant use 0
  lcd.createChar(1, line1);
  lcd.createChar(2, line2);
  lcd.createChar(3, line3);
  lcd.createChar(4, line4);
  lcd.createChar(5, line5);
  lcd.createChar(7, loadchar);
  lcd.createChar(8, tempchar);
  //lcd.createChar(9, corechar); // too many chars


}
int iHome = 1;
int diff = 1;
int count = 5; // delay for anim (move every *count* updates) // not used anymore
int dely = 0;


//draw the graph - load is a %, witdh is the length in lcd Chars
void graph(int load, int width){
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
    lcd.write(5);
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
    lcd.write(test);
  }
  i++;
  // add blanks
  while(i<width){
    lcd.write(6);
    i++;
  }
  
  }void loop() {
  // print the string when a newline arrives:
  if (NewData) {    
    //clear
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(8);
    lcd.print(coretmp.toInt());
    lcd.print((char)223);
    lcd.setCursor(5, 0);
    lcd.write(7);
    graph(coreLoad,10);
    // temps n stuffs
    lcd.setCursor(0, 1);
    lcd.write((char)402);
    lcd.print(coreSpd);
    lcd.setCursor(10, 1);
    lcd.write(8);
    lcd.print(coretmp);
    lcd.print((char)223);
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
      b[1] = inputString.indexOf(',');
      b[2] = inputString.indexOf(',', b[1]+1);
      b[3] = inputString.indexOf(',', b[2]+1);
      b[4] = inputString.length()-1;
      coreSpd = inputString.substring(b[0],b[1]);
      coretmp = inputString.substring(b[1]+1,b[2]);
      coretime = inputString.substring(b[2]+1,b[3]);
      String loadHold = inputString.substring(b[3]+1,b[4]);
      coreLoad = loadHold.toInt();
      NewData = true;
    } 
  }
  
}

