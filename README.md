## Solar alarm clock
#### Video Demo:  <URL HERE>
#### Description:
An alarm clock that mimics the sunrise, featuring an OLED screen, a simple GUI for adjusting the alarm time and a script for cycling through different sunrise colors.

#### Hardware
Hardware for this project consists of an Arduino Pro Mini on a custom circuit made on a perfboard.
The circuit is powered with a 12v power supply, and has a 5v regulator to power the Arduino.
The microcontroller is connected to an OLED screen, a buzzer, encoder, toggle switch, real-time-clock module, and a driver circuit made with MOSFETs to drive the RGB LED strip that will mimic the sunrise.
The brains of the project are enclosed in a 3D printed container with a lid that houses all the electronics and has holes for the encoder and screen, so the user can interact with the clock.
The container sits on top of an old lamp that now contains the RGB LED strip.

#### Software
The code starts including the main libraries for communicating with the sensors and other componnents connected to the microcontroller. After that, the pins and some of the variables are declared and given its initial values and the prototypes of the functions are also declared.
At the void setup function, we configure the pins as inputs or outputs and initialize some of the peripherals.
Void loop checks the state of a button on the encoder and switches between two main functions: display_time() or set_alarm(). A long press on the button will switch between the functions.

display_time():
this function reads the time stored in the real-time-clock and displays it in the OLED screen. Inside this function, we also read the state of a button on the side of the enclosure in order to light up or not the lamp as a regular lamp, as long as the sunrise() function is inactive.

set_alarm(timer):
this functions turns the OLED screen into a menu where the user can set the hour and minute when the lamp will start its sunrise() sequence. It takes the timer that is reset when we press the encoder button as a parameter to check if the user performed a short or long button press.

sunrise():
this function takes the set alarm time, and when the clock gets there, the RGB LEDs will cycle between 6 predetermined sunrise colors throughout time. The transition between the colors is made with a linear interpolation between the previous and next RGB colors, as a function of time.

setColor(rval, gval, bval):
this function is a short acessory function to make setting the colors easier and more concise.
