unsigned long startingMillis = 0;
time_t startingEpochTime = 0; // Starting epoch time

void setup() {
  Serial.begin(115200);
  startingMillis = millis();
  startingEpochTime = getTimeFromMillis(startingMillis); // Get the starting epoch time
}

void loop() {
  // Get the current millis and calculate the elapsed milliseconds
  unsigned long currentMillis = millis();
  unsigned long elapsedMillis = currentMillis - startingMillis;

  // Calculate the current epoch time
  time_t currentEpochTime = startingEpochTime + getTimeFromMillis(elapsedMillis);

  // Print the current epoch time
  Serial.println(currentEpochTime);

  // Delay or perform other operations as needed
  delay(1000);
}

// Function to convert milliseconds to epoch time
time_t getTimeFromMillis(unsigned long millis) {
  unsigned long seconds = millis / 1000; // Convert milliseconds to seconds
  return seconds + 946684800; // Add the number of seconds between 1970-01-01 and 2000-01-01
}
