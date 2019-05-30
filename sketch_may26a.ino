#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// WAV files converted to code by wav2sketch
#include "AudioSample808Kick.h"
#include "AudioSample707Snare.h"
#include "AudioSample808Hat.h"
#include "AudioSampleTrapBeat.h"
#include "AudioSampleDnbBeat.h"
#include "AudioSamplePopBeat.h"

// GUItool: begin automatically generated code
AudioPlayMemory          drumLoop;       //xy=160,313
AudioPlayMemory          kick;           //xy=166,209
AudioPlayMemory          snare;          //xy=166,244
AudioPlayMemory          hihat;          //xy=166,279
AudioMixer4              drumVerb;       //xy=329,228
AudioEffectReverb        reverb1;        //xy=450,321
AudioMixer4              mixer;         //xy=632,302
AudioOutputUSB           usb1;           //xy=795,280
AudioOutputAnalog        dac1;           //xy=795,319
AudioConnection          patchCord1(drumLoop, 0, drumVerb, 3);
AudioConnection          patchCord2(kick, 0, drumVerb, 0);
AudioConnection          patchCord3(snare, 0, drumVerb, 1);
AudioConnection          patchCord4(hihat, 0, drumVerb, 2);
AudioConnection          patchCord5(drumVerb, reverb1);
AudioConnection          patchCord6(drumVerb, 0, mixer, 0);
AudioConnection          patchCord7(reverb1, 0, mixer, 1);
AudioConnection          patchCord8(mixer, 0, usb1, 0);
AudioConnection          patchCord9(mixer, dac1);
// GUItool: end automatically generated code

int pot = 14; // we are using a pot to select which loop to be played
int potState = 0;
int mappedPotState;

int stretch = 15; // we are using a stretch sensor to manipulate effects attached to the loops
float stretchState = 0;
float mappedStretchState;

elapsedMillis kickTimer = 0;
elapsedMillis snareTimer = 0;
elapsedMillis hatTimer1 = 0;
elapsedMillis hatTimer2 = 0;

elapsedMillis loopTimer = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  AudioMemory(16);

  pinMode(pot, INPUT);
  pinMode(stretch, INPUT);
  
  mixer.gain(0, 0.7);

  mixer.gain(2, 0.0);
  mixer.gain(3, 0.0);

  drumVerb.gain(0, 0.7);
  drumVerb.gain(1, 0.5);
  drumVerb.gain(2, 0.5);
  drumVerb.gain(3, 0.7);

  reverb1.reverbTime(1.0);
}

void loop() {
  // put your main code here, to run repeatedly:
  mixer.gain(1, mappedStretchState);
  
  potState = analogRead(pot); // reads the pin that has pot connected, and saves data in potState
  mappedPotState = map(potState, 0, 1023, 1, 3); // maps the pot data, making it suitable for our project
  Serial.print("Pot: ");
  Serial.print(mappedPotState);
  delay(10);

  stretchState = analogRead(stretch); // reads the pin that has the stretch sensor connected, and saves data in stretchState
  mappedStretchState = map(stretchState, 170, 250, 0, 1);
  Serial.print("| Stretch: ");
  Serial.println(stretchState);
  delay(10);

  if (mappedPotState == 1) { // turned to the left
    // First Loop: House Drum Beat at 114BPM (programmed in Arduino using Roland TR-8 Samples)
    if (kickTimer >= 526) { // every beat
      kick.play(AudioSample808Kick); // playing the kick sample
      kickTimer = 0; // resets the timer for the next loop
    }

    if (snareTimer >= 1052) { // every second beat
      snare.play(AudioSample707Snare); // playing the snare sample
      snareTimer = 0; // resets the timer for the next loop
    }

    if (hatTimer1 >= 789) {
      hihat.play(AudioSample808Hat); // playing the hat sample
      hatTimer1 = 263; // resetting the timer to 263 allows the hihat to be played on the off-beat
    }
  }

  else if (mappedPotState == 2) { // in the middle
    // Second Loop: Trap Beat at 137BPM (programmed in Roland TR-8 then imported)
    if (loopTimer >= 3504) { // 2 bars
      drumLoop.play(AudioSampleTrapBeat); // playing the loop
      loopTimer = 0; // resets the timer for the next loop
    }
  }

  else if (mappedPotState == 3) { // turned to the right
    // Third Loop: Drum n Bass Beat at 170BPM (programmed in Ableton Drum Rack using Roland TR-8 samples then imported
    if (loopTimer >= 1412) { // 1 bar
      drumLoop.play(AudioSampleDnbBeat); // playing the loop
      loopTimer = 0; // resets the timer for the next loop
    }

    if (hatTimer2 >= 176) { // every eighth beat
      hihat.play(AudioSample808Hat); // playing the hat sample
      hatTimer2 = 0; // resets the timer for the next loop
    }
  }  

  // we hoped to have five loops stored but only had enough memory space for three
  /*      else if (mappedPotState == 4){
          // Fourth Loop: Pop Beat at 100BPM (programmed in Ableton Drum Rack using Roland TR-8 samples then imported
          if (loopTimer >= 4800) { // 2 bars
            drumLoop.play(AudioSamplePopBeat); // playing the loop
            loopTimer = 0; // resets the timer for the next loop
          }

          if (hatTimer >= 300) { // every eighth beat
            hihat.play(AudioSample808Hat); // playing the hat sample
            hatTimer = 0; // resets the timer for the next loop
          }
        }*/
}
