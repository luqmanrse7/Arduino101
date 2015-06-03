/*
for this tutorial a common cathode RGB must be used to directly connect it to arduino digital pin 9,10,11. The longest one must
be connected to a ground source. a tactile switch is then connected to digitalpin 4

*/


float col1 = 0, col2 = 0, col3 = 0, newCol;
int sw = 1;

void setup() {
  // put your setup code here, to run once
  pinMode(4, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(4) == 0)
  {
    while (digitalRead(4) == 0)
    {
      //debounce
    }
    if (sw < 4)
    {
      sw = sw + 1;
    }
    else
    {
      sw = 1;
    }
  }
  Serial.print(sw);
  Serial.print(" : ");
  Serial.print(col1);
  Serial.print(" : ");
  Serial.print(col2);
  Serial.print(" : ");
  Serial.println(col3);

  newCol = (analogRead(0) / 1023.0) * 255.0;
  analogWrite(9, col1 );
  analogWrite(10, col2 );
  analogWrite(11, col3 );

  if (sw == 1) {
    col1 = newCol;
    digitalWrite(13, 0);
  }
  else if (sw == 2) {
    col2 = newCol;
    digitalWrite(13, 0);
  }
  else if (sw == 3) {
    col3 = newCol;
    digitalWrite(13, 0);
  }
  else {
    digitalWrite(13, 1);
  }
  //delay(10);
}
