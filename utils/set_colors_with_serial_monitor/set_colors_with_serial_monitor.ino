#define r 3
#define g 5
#define b 6

unsigned long pt = 0;

// Time till alarm is fully lit (in milliseconds)
int timeout = 5000;

// Color values for red, green, and blue LEDs
int rval;
int gval;
int bval;

// Initial values for each color
int ri = 0;
int gi = 0;
int bi = 0;

// Threshold values for each color
int rf = 255;
int gf = 255;
int bf = 0;

void setup() {
  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);
  Serial.begin(9600);
  Serial.println("Enter RGB values in the format: R,G,B (e.g., 255,167,0)");
}

void loop() {
  // Check if data is available from Serial Monitor
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    int comma1 = input.indexOf(',');
    int comma2 = input.lastIndexOf(',');

    if (comma1 != -1 && comma2 != -1 && comma1 != comma2) {
      // Parse the input into R, G, B values
      rval = input.substring(0, comma1).toInt();
      gval = input.substring(comma1 + 1, comma2).toInt();
      bval = input.substring(comma2 + 1).toInt();

      // Constrain values to 0-255
      rval = constrain(rval, 0, 255);
      gval = constrain(gval, 0, 255);
      bval = constrain(bval, 0, 255);

      // Update the LED colors
      setColor(rval, gval, bval);

      // Provide feedback in Serial Monitor
      Serial.print("Color set to R: ");
      Serial.print(rval);
      Serial.print(", G: ");
      Serial.print(gval);
      Serial.print(", B: ");
      Serial.println(bval);
    } else {
      Serial.println("Invalid format! Use: R,G,B");
    }
  }

  // Uncomment to test static color
  // setColor(255, 167, 0);
}

// Function to set color of RGB LED
void setColor(int red, int green, int blue) {
  analogWrite(r, red);
  analogWrite(g, green);
  analogWrite(b, blue);
}
