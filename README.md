# EnigmArduino – On the road to an Arduino-powered ciphering machine
Etienne Lesot, Hugo Kyhm

CentraleSupélec, Rennes campus, 2016/2017
![montage](https://raw.githubusercontent.com/pcotret/enigmarduino/master/img/montage.png)

This project has been realized by Etienne Lesot and Hugo Kyhm (1st year students 16/17 at CentraleSupélec, Rennes campus) for a short-term project under the supervision of Dr. Pascal Cotret.

## Goals of this project
We were inspired by the project realized by Marion Georges and Tanguy xxx who made a mechanical replica of a 2-rotor Enigma ciphering machine with lots of 3D-printed parts (details available at: https://pascalr2blog.wordpress.com/3d-printing/enigma-replica/). However, the replica made by Marion and Tanguy had some drawbacks… The main one is the rotor itself:
* Metal connectors were not well fixed.
* All the circuitry inside a rotor has been completely hand-made: quite hard to assemble, risks of short-circuits.
  That is the reason why we decided to replace rotors by a single microcontroller-based board while keeping a physical keyboard: our EnigmArduino was born!

## Introduction
Here is the overall structure of EnigmArduino.
![structure](https://raw.githubusercontent.com/pcotret/enigmarduino/master/img/schematic.png)
EnigmArduino is composed of three main parts:

* A basic 1602 LCD screen.
* An Arduino Uno which replaces the rotors logic.
* A keyboard (26 letters for this first version).

## Keyboard with the Arduino
Connecting 26 letters to a single Arduino is impossible as it does not have enough pins… We had to use another technique: in fact, using several voltage dividers, we were able to connect 26 buttons to a single Arduino pin!
![voltage](https://raw.githubusercontent.com/pcotret/enigmarduino/master/img/voltage_divider.png)
The above schematic shows the electric wiring for the first five letters (all resistors are 6.8 kohms but we could use other values !). We also enable the internal pull up resistor of the Arduino pin : it puts a +5V (in reality, a bit lower) on the pin. As a consequence, when no button is pressed, the Arduino pin reads about 1023 (2^10 as we have a 10-bit ADC in the Arduino Uno). 
You may notice that the first button switch causes a direct short from A0 to GND. The pullup resistor on the pin protects the Arduino from a short and the actual current flowing is only around 1 mA.
When a single button is pressed, we get a voltage divider with a different resistor ratio. A simple Arduino sketch can give us a projection of those voltage divider values :
```C
const byte keypadPin=A0;
int val=0;

void setup() 
{
  Serial.begin(9600); 
  pinMode(keypadPin, INPUT_PULLUP); 
}

void loop()
{
    int pinVal;
    pinVal = analogRead(keypadPin);
    Serial.println(val);
    delay(100);
} 
```
![matlab](https://raw.githubusercontent.com/pcotret/enigmarduino/master/img/chart.png)
The full schematic with the Arduino, push buttons and a LCD screen:
![full](https://raw.githubusercontent.com/pcotret/enigmarduino/master/img/fritzing.png)
We assumed that push buttons were placed as a classic AZERTY keyboard (for the Arabic alphabet only). 

## The Arduino brain

**Last version: 12/07/2017**

* 3 rotors
* Only encrypts a single character
* Dependencies between rotors is taken into account.
* This version only implement a dummy logic inside rotors.

**Description:**

Each rotor is an object of a class called Rotor with all variables and methods needed. EnigmArduino works on ASCII code to encode each input character. Each rotor is described by a 2D-array where the first column is the input while the second one is the output. When the main loop asks for a character, it is processed through all 3 rotors to give the ciphertext.

Todo list (non-exhaustive!):

* Write a "real" logic inside rotors.
* Implement a method to initialize each rotor position.
* For the CodeBlocks versions, make something more automatic.
* Integrate the plugboard feature.
* Create a daughter board in a tool such as KiCad to create a custom enigmArduino board :)

In [code/codeblocks](https://github.com/pcotret/enigmarduino/tree/master/code/codeblocks), you will find a simple C++ application describing the enigmArduino brain.

In [code/enigmarduino](https://github.com/pcotret/enigmarduino/tree/master/code/enigmarduino), you will find a working Arduino code. Just use the schematic given above for the wiring.


