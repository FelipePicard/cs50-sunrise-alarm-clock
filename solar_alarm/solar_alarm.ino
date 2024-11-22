// GUI for setting alarm time for the sunrise alarm clock
#include <Encoder.h>
#include "RTClib.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// defining the pins for the inputs and outputs
Encoder myEnc(11, 12);
#define btnPin 10
#define r 3
#define g 5
#define b 6

bool btnState = HIGH;
bool prev_btnState = HIGH;
int correction = 0;

RTC_DS1307 rtc;
DateTime now;

void set_alarm(long);
void setColor(int, int, int);

int timer_thresh = 800;
int menu_counter = 0;
int enc_counter = 0;
long prev_pos = 0;
unsigned long pt = 0;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// wake up hours
int h = 6;
// wake up minutes
int m = 0;

int time_set = 0;
int hour_set = 0;
int min_set = 0;
int prev_h = 0;
int prev_m = 0;

// time till alarm is fully lit (in seconds)
int timeout = 1*60;

int k = 0;

// color values for red green and blue LEDs
int rval;
int gval;
int bval;

// initial values for each color
int ri = 0;
int gi = 0;
int bi = 0;

// threshold values for each color
int rf = 255;
int gf = 40;
int bf = 20;

char hour_set_text[3];
char min_set_text[3];

void setup() {
  pinMode(btnPin, INPUT_PULLUP);
  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);
  // put your setup code here, to run once:
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  
  // uncomment this line to adjust time manually, if needed
  //rtc.adjust(DateTime(2023, 7, 15, 8, 21, 0));
  
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  display.clearDisplay();
  Serial.begin(9600);
}

void loop() {
  display.clearDisplay();
  unsigned long ct = millis();

  btnState = digitalRead(btnPin);
  
  long timer = ct - pt;

  if (timer > timer_thresh && prev_btnState == LOW)
  {
    menu_counter++;
    btnState = HIGH;
    tone(9, 5600, 100);
  }

  if (btnState == LOW && prev_btnState == HIGH)
  {
    pt = ct;
    prev_btnState = btnState;
  }

  //Serial.println(timer);

  if (btnState == HIGH)
  {
    pt = ct;
    prev_btnState = btnState;
  }


  if (menu_counter % 2 == 0)
  {
    display_time();
  }

  if (menu_counter % 2 != 0)
  {
    set_alarm(timer);
  }

  display.display();
  // Serial.println(enc_counter);
}

void set_alarm(long timer)
{
  long newPosition = (myEnc.read())/2;
  long position = newPosition - correction;
  long enc_change = position - prev_pos;

  display.setTextSize(1.5);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Adjust alarm time:");

  sprintf(hour_set_text, "%02d", hour_set);
  sprintf(min_set_text, "%02d", min_set);

  if (hour_set > 24)
  {
    prev_h = 0;
  }

  if (min_set > 60)
  {
    prev_m = 0;
  }

  //Serial.println(min_set_text);

  display.setTextSize(1.5);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 15);
  display.print(hour_set_text);
  display.print(':');
  display.print(min_set_text);

  if (enc_counter % 2 == 0)
  {
    display.setCursor(0, 18);
    display.print("__");
    hour_set = prev_h + enc_change;
  }

  if (enc_counter % 2 != 0)
  {
    display.setCursor(18, 18);
    display.print("__");
    min_set = prev_m + enc_change;
  }

  display.display();

  if (min_set < 0)
  {
    min_set = 0;
    prev_m = min_set;
    prev_pos = position;
    tone(9, 600, 30);
  }

  if (hour_set < 0)
  {
    hour_set = 0;
    prev_h = hour_set;
    prev_pos = position;
    tone(9, 600, 30);
  }

  if (btnState == LOW && timer < 150)
  {
    digitalWrite(13, HIGH);
    tone(9, 3600, 10);
    enc_counter++;
    prev_h = hour_set;
    prev_m = min_set;
    prev_pos = position;
    delay(200);
  }

  else
  {
    digitalWrite(13, LOW);
  }
}

void display_time()
{
  now = rtc.now();

  display.setTextSize(1.5);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(now.hour(), DEC);
  display.print(':');
  display.print(now.minute(), DEC);
  display.print(':');
  display.print(now.second(), DEC);
  
  display.setTextSize(1.5);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 15);
  display.print(daysOfTheWeek[now.dayOfTheWeek()]);

  sunrise();
}

// sunrise colors function
void sunrise()
{
  h = hour_set;
  m = min_set;
  // elapsed time since the sunrise routine started, in seconds
  long elapsed_time = (now.hour() - h)*360 + (now.minute() - m)*60 + (now.second());
  // elapsed time, in percent
  int percentage = map(elapsed_time, 0, timeout, 0, 100);
  
    if(elapsed_time < timeout){
      k = 0;
      rval = map(percentage, 0, 60, ri, rf);
      gval = map(percentage, 25, 100, gi, gf);
      bval = map(percentage, 10, 70, bi, bf);

      if(rval<0){
        rval=0;
      }
      if(gval<0){
        gval=0;
      }
      if(bval<0){
        bval=0;
      }

      if(rval>rf){
        rval=rf;
      }
      if(gval>gf){
        gval=gf;
      }
      if(bval>bf){
        bval=bf;
      }
      digitalWrite(13, HIGH);
  }

  else if (elapsed_time >= timeout && elapsed_time < (timeout + 1*60)){
    rval = rf;
    gval = gf;
    bval = bf;
  }

  else if (elapsed_time >= (timeout + 1*60)){
    //count = 1;
    k = 1;
    digitalWrite(13, LOW);
  }
  //Serial.println("elapsed time", elapsed_time);
  setColor(rval, gval, bval);
}

// set RGB LEDs color function
void setColor(int red, int green, int blue)
{
  analogWrite(r, red);
  analogWrite(g, green);
  analogWrite(b, blue);
}