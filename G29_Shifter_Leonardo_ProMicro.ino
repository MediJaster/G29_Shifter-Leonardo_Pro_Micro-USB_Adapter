// Logitech Driving Force Shifter USB Adapter
// Inspired by projects from Armandoiglesias and Jason Duncan
// Copyright (C) Jean-Romain Garnier, Constant Zion
// Modified by MediJaster to work on an Arduino Leonardo/Pro Micro
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <Joystick.h>

// Create the Joystick
Joystick_ Joystick;

// Shifter analog axis threshhold
// Change these values if the gears aren't detected correctly
#define XAXIS_LEFT_THRESHOLD 350
#define XAXIS_RIGHT_THRESHOLD 650
#define YAXIS_UP_THRESHOLD 700
#define YAXIS_DOWN_THRESHOLD 300

#define GEAR_1 1
#define GEAR_2 2
#define GEAR_3 3
#define GEAR_4 4
#define GEAR_5 5
#define GEAR_6 6
#define GEAR_REVERSE 7
#define GEAR_NEUTRAL 0

// Whether switching to neutral should register as a button (0 or 1)
#define REGISTER_NEUTRAL 0

int gear, old_gear;

void deactivate_all() {
  for (int i = 0; i <= 10; i++)
    Joystick.setButton(i, LOW);
}

int determine_gear(int x, int y, int is_reverse) {
  int gear = GEAR_NEUTRAL;

  // Stick is to the left
  if (x < XAXIS_LEFT_THRESHOLD) {

    if (y > YAXIS_UP_THRESHOLD)
      gear = GEAR_1;
    else if (y < YAXIS_DOWN_THRESHOLD) 
      gear = GEAR_2;
  }

  // Stick is to the right
  else if (x > XAXIS_RIGHT_THRESHOLD) {

    if (y > YAXIS_UP_THRESHOLD)
      gear = GEAR_5;

    // Is it in reverse?
    else if (y < YAXIS_DOWN_THRESHOLD) {
      if (is_reverse)
        gear = GEAR_REVERSE;
      else
        gear = GEAR_6;
    }
  }

  else {

    if (y > YAXIS_UP_THRESHOLD)
      gear = GEAR_3;

    else if (y < YAXIS_DOWN_THRESHOLD)
      gear = GEAR_4;
  }

  return gear;
}

void setup()
{
  // G29 Analog input configuration
  pinMode(A0, INPUT_PULLUP); // X axis
  pinMode(A2, INPUT_PULLUP); // Y axis

  // Reverse Switch
  pinMode(2, INPUT);

  // Initialize Joystick Library
  Joystick.begin();
}

void loop()
{
  int x = analogRead(0);           // X axis
  int y = analogRead(2);           // Y axis
  int is_reverse = digitalRead(2); // Reverse button

  int gear = determine_gear(x, y, is_reverse);

  if (gear == 0)
    deactivate_all();

  else if (gear != old_gear) {
    Joystick.setButton(old_gear - 1, LOW);
    Joystick.setButton(gear - 1, HIGH);

    if (gear == 7)
      Joystick.setButton(5, LOW);
  }

  old_gear = gear;
}