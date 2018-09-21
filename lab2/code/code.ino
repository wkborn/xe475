void setup() {
  Serial.begin(9600);

  Serial.print("once\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("forever\n");

  delay(500);
}
