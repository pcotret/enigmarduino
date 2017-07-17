// Constants
#define ALPHALEN 26

// Libraries
#include <iostream>
using namespace std;

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
// Main program - One iteration only
int main()
{
    // Variables
    char plaintext,ciphertext;
    int plaincode,ciphercode;
    int temp_letter1,temp_letter2;
    // Main program
    InitRotors();
    cout << "Enter plaintext:" << endl;
    cin >> plaintext;
    plaincode=(int)plaintext;
    temp_letter1=ProcessRotor(rotor1,plaincode);
    temp_letter2=ProcessRotor(rotor2,temp_letter1);
    ciphercode=ProcessRotor(rotor3,temp_letter2);
    ciphertext=(char)ciphercode;
    cout << "Ciphertext: " << ciphertext << endl;
    RotateRotors();
    return 0;
}
