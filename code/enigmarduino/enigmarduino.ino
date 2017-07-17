// Constants
#define ALPHALEN 26   // Length of the alphabet to be encrypted/decrypted
#define KEYBOARDPIN 0 // Analog port for the keyboard
#define LCDPIN 10     // Button for LCD screen reset
#define DELTA 2       // Keyboard uncertainty

// Libraries
#include <LiquidCrystal.h>

// LCD wiring
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Rotor class
// Classes
class Rotor
{
  public:
  int counter;                   // Counter to increment rotor positions
  int rotor_wiring[ALPHALEN][2]; // Rotors internal circuitry
  int i,j;
  Rotor()
  {
    for(i=0;i<ALPHALEN;i++)
    {
      for(j=0;j<2;j++)
      {
        rotor_wiring[i][j]=0;
      }
    }
  }
  void RotateRotor();
  void DisplayWiring();
};

// Rotor functions
/* Description: DisplayWiring. Basically println rotor values
 * Input: None
 * Output: None
 */
void Rotor::DisplayWiring()
{
  int i,j;
  for(i=0;i<ALPHALEN;i++) 
  {
    for (j=0;j<2;j++)
    {
      if (j==0)
      {
        Serial.print(rotor_wiring[i][j]);
        Serial.print(';');
      }
      else
      {
        Serial.println(rotor_wiring[i][j]); 
      }
    }
  }  
}
/* Description: RotateRotor. It basically makes 1/26 rotation for each rotor when needed
 * Input: None
 * Output: None
 */
void Rotor::RotateRotor()
{
  int i,j=0;
  for(i=0;i<ALPHALEN;i++)
  {
    for(j=0;j<2;j++)
    {
      rotor_wiring[i][j]++%ALPHALEN;
    }
  }
}
/* Description: DisplayWiring. Displays a rotor internal circuitry
 * Input: None
 * Output: None
 */
void Rotor::DisplayWiring()
{
  int i,j=0;
  for(i=0;i<ALPHALEN;i++)
  {
    for(j=0;j<2;j++)
    {
      cout << rotor_wiring[i][j] << " ";
    }
    cout << endl;
  }
  cout << endl;
}

// Other functions and global variables
Rotor rotor1; // Rotor #1
Rotor rotor2; // Rotor #2
Rotor rotor3; // Rotor #3
/* Description: InitRotors. Initialize rotors (with "dirty" stuff for the moment)
 * Input: None
 * Output: None
 */
void InitRotors()
{
  int i;
  for(i=0;i<ALPHALEN;i++)
  {
    rotor1.rotor_wiring[i][0]=i;
    rotor1.rotor_wiring[i][1]=ALPHALEN-1-i;
    rotor2.rotor_wiring[i][0]=i;
    rotor2.rotor_wiring[i][1]=(i+2)%ALPHALEN;
    rotor3.rotor_wiring[i][0]=i;
    rotor3.rotor_wiring[i][1]=ALPHALEN-1-i;
  }
  rotor1.counter=0;
  rotor2.counter=0;
  rotor3.counter=0;
}
/* Description: ProcessRotor.
 * Input: rotor, a rotor; key, the character to be ciphered
 * Output: None
 */
int ProcessRotor(Rotor rotor,int key)
{
  int input_index;
  int output_char;
  // Get input index
  input_index=key-97;
  // Get the rotor wiring
  output_char=rotor.rotor_wiring[input_index][1];
  return output_char+97;
}
/* Description: RotateRotors. Managing dependencies between all rotors
 * Input: None
 * Output: None
 */
void RotateRotors()
{
  rotor1.counter++;
  if(rotor1.counter==ALPHALEN)
  {
    if(rotor2.counter==ALPHALEN)
    {
      if(rotor3.counter==ALPHALEN)
      {
        rotor3.RotateRotor();
        rotor3.counter++;
      }
      rotor2.RotateRotor();
      rotor2.counter++;
    }
    rotor1.RotateRotor();
    rotor1.counter++;
  }
}

// Setup (executed only once)
void setup()
{
  Serial.begin(57600);
  lcd.begin(16, 2);
  pinMode(LCDPIN,INPUT);
  InitRotors();
}

// Main loop (infinite)
void loop()
{
  // Variables
  boolean button_state=false;
  int cursor_position=0;
  int keyboard_value;
  char plaintext,ciphertext;
  int plaincode,ciphercode;
  int temp_letter1,temp_letter2;
  // Stuff
  boolean button_state=digitalRead(LCDPIN);
  if(button_state)
  {
    lcd.clear();
    cursor_position=0;
  }
  else
  {
    keyboard_value=analogRead(KEYBOARDPIN);
    delay(50);
    plaintext=KeyDecode(keyboard_value);
    plaincode=(int)plaintext;
    temp_letter1=ProcessRotor(rotor1,plaincode);
    temp_letter2=ProcessRotor(rotor2,temp_letter1);
    ciphercode=ProcessRotor(rotor3,temp_letter2);
    ciphertext=(char)ciphercode;
    lcd.write(plaintext);
    lcd.setCursor(cursor_position,1);
    lcd.write(ciphertext);
    lcd.setCursor(cursor_position+1,0);
  }
}

char KeyDecode(int V)
{
  if((V>=0)&&(V<0+DELTA))
    return 'a';
  else if((V>122-DELTA)&&(V<122+DELTA))
    return 'z';
  else if((V>219-DELTA)&&(V<219+DELTA))
    return 'e';
  else if((V>296-DELTA)&&(V<296+DELTA))
    return 'r';
  else if((V>360-DELTA)&&(V<360+DELTA))
    return 't';
  else if((V>414-DELTA)&&(V<414+DELTA))
    return 'y';
  else if((V>460-DELTA)&&(V<460+DELTA))
    return 'u';
  else if((V>499-DELTA)&&(V<499+DELTA))
    return 'i';
  else if((V>533-DELTA)&&(V<553+DELTA))
    return 'o';
  else if((V>563-DELTA)&&(V<563+DELTA))
    return 'p';
  else if((V>590-DELTA)&&(V<590+DELTA))
    return 'q';
  else if((V>613-DELTA)&&(V<613+DELTA))
    return 's';
  else if((V>634-DELTA)&&(V<634+DELTA))
    return 'd';
  else if((V>653-DELTA)&&(V<653+DELTA))
    return 'f';
  else if((V>671-DELTA)&&(V<671+DELTA))
    return 'g';
  else if((V>686-DELTA)&&(V<686+DELTA))
    return 'h';
  else if((V>701-DELTA)&&(V<701+DELTA))
    return 'j';
  else if((V>714-DELTA)&&(V<714+DELTA))
    return 'k';
  else if((V>726-DELTA)&&(V<726+DELTA))
    return 'l';
  else if((V>738-DELTA)&&(V<738+DELTA))
    return 'm';
  else if((V>748-DELTA)&&(V<748+DELTA))
    return 'w';
  else if((V>758-DELTA)&&(V<758+DELTA))
    return 'x';
  else if((V>767-DELTA)&&(V<767+DELTA))
    return 'c';
  else if((V>775-DELTA)&&(V<775+DELTA))
    return 'v';
  else if((V>783-DELTA)&&(V<783+DELTA))
    return 'b';
  else if((V>790-DELTA)&&(V<790+DELTA))
    return 'n';
}


char typedKey (int V)
{

    if ((V<1025)&&(V>1010))
    {
      return 'a';
    }
    else if ((V<1010)&&(V>990))
    {
      return 'b';
    }
    else if ((V<990)&&(V>965))
    {
      return 'c';
    }
    else if ((V<965)&&(V>945))
    {
      return 'd';
    }
    else if ((V<945)&&(V>925))
    {
      return 'e';
    }
    else if ((V<925)&&(V>910))
    {
      return 'f';
    }
    else if ((V<910)&&(V>890))
    {
      return 'g';
    }
    else if ((V<890)&&(V>870))
    {
      return 'h';
    }
    else if ((V<870)&&(V>855))
    {
      return 'i';
    }
    else if ((V<855)&&(V>838))
    {
      return 'j';
    }
    else if ((V<838)&&(V>823))
    {
      return 'k';
    }
    else if ((V<823)&&(V>808))
    {
      return 'l';
    }
    else if ((V<808)&&(V>792))
    {
      return 'm';
    }
    else if ((V<792)&&(V>778))
    {
      return 'n';
    }
    else if ((V<778)&&(V>765))
    {
      return 'o';
    }
    else if ((V<765)&&(V>753))
    {
      return 'p';
    }
    else if ((V<753)&&(V>738))
    {
      return 'q';
    }
    else if ((V<738)&&(V>728))
    {
      return 'r';
    }
    else if ((V<728)&&(V>715))
    {
      return 's';
    }
    else if ((V<715)&&(V>705))
    {
      return 't';
    }
    else if ((V<705)&&(V>690))
    {
      return 'u';
    }
    else if ((V<690)&&(V>667))
    {                                 
      return 'v';
    }
    else if ((V<667)&&(V>648))
    {
      return 'w';
    }
    else if ((V<648)&&(V>628))
    {
      return 'x';
    }
    else if ((V<628)&&(V>610))
    {
      return 'y';
    }
    else if((V<610)&&(V>580))
    {
      return 'z';
    }
    
}

