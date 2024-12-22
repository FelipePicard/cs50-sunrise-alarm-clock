## Solar alarm clock
#### Video Demo: [Watch it here!!] (https://youtu.be/estjCFrhNYo)
#### Description:
An alarm clock that mimics the sunrise, featuring an OLED screen, a simple GUI for adjusting the alarm time and a script for cycling through different sunrise colors.

---

## Hardware  
This project uses:  
- **Arduino Pro Mini** on a custom circuit made on a perfboard.  
- **Power supply**: A 12V source with a 5V regulator for the Arduino.  
- **Components**:  
  - OLED screen  
  - Buzzer  
  - Encoder  
  - Toggle switch  
  - Real-time-clock module  
  - Driver circuit with MOSFETs to drive the RGB LED strip  
- **Enclosure**:  
  - 3D-printed container with:  
    - Holes for the encoder and screen for user interaction.  
    - Electronics housed inside.  
  - RGB LED strip mounted inside an old lamp base.  

---

## Features  
- Real-time clock for precise timekeeping.  
- Mimics the sunrise for a natural wake-up experience.  
- Smooth color transitions via **linear interpolation**.  
- Interactive GUI to adjust alarm settings.  
- Dual functionality: alarm lamp and regular lamp.

---

## Software
The code starts including the main libraries for communicating with the sensors and other componnents connected to the microcontroller. After that, the pins and some of the variables are declared and given its initial values and the prototypes of the functions are also declared.
At the void setup function, we configure the pins as inputs or outputs and initialize some of the peripherals.
Void loop checks the state of a button on the encoder and switches between two main functions: display_time() or set_alarm(). A long press on the button will switch between the functions.

---

### Core Functions  

#### `void display_time()`  
- **Reads** the real-time clock and displays the current time on the OLED screen.  
- **Toggle Functionality**: Checks a side button to use the lamp as a regular light when the `sunrise()` sequence is inactive.  

#### `void set_alarm(timer)`  
- Turns the OLED screen into a **menu** for setting the alarm time (hour and minute).  
- Detects **short** or **long button presses** using the `timer` parameter.  

#### `void sunrise()`  
- Activates when the current time matches the set alarm.  
- Gradually cycles through **6 predefined sunrise colors** using **linear interpolation** for smooth transitions.  

#### `void setColor(rval, gval, bval)`  
- A helper function to easily set the RGB LED color values.
