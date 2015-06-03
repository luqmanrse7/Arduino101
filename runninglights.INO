/*

For this tutorial bunch of leds are connected from digital pin 7 to digital pin 13. A tactile switch is then connected to be used well as a switch
Please remember to connect a current limiting resistor to each and every single leds

*/

boolean state, pause;

void setup() {
  // put your setup code here, to run once:
  pause = 0;
  pinMode( 4, INPUT_PULLUP);
  for (int count = 7; count < 13; count++)
  {
    pinMode(count, OUTPUT);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  while (pause == 1)
  {
    //while pause do nothing
  }
  state = !state;
  for (int x = 7; x < 13; x ++)
  {
    digitalWrite( x, state);
    delay(250);
    if (digitalRead(4) == 0)
    {
      pause = 1;
      break;
    }
  }
  while (pause == 1)
  {
    delay(100);
    if (digitalRead(4) == 0)
    {
      pause = 0;
      break;
    }
  }
}
