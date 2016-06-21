/*****************************************************************************
* ArduboyTones
*
* A library to play tone sequences.
*
*****************************************************************************/

#ifndef ARDUBOY_TONES_H
#define ARDUBOY_TONES_H

#include <Arduino.h>

#ifndef AB_DEVKIT
// Arduboy speaker pin 1 = Arduino pin 5 = ATmega32u4 PC6
#define TONE_PIN_PORT PORTC
#define TONE_PIN_DDR DDRC
#define TONE_PIN PORTC6
#define TONE_PIN_MASK _BV(TONE_PIN)
#else
// DevKit speaker pin 1 = Arduino pin A2 = ATmega32u4 PF5
#define TONE_PIN_PORT PORTF
#define TONE_PIN_DDR DDRF
#define TONE_PIN PORTF5
#define TONE_PIN_MASK _BV(TONE_PIN)
#endif

// The minimum frequency that can be produced without a clock prescaler.
#define MIN_NO_PRESCALE_FREQ ((uint16_t)(((F_CPU / 2L) + (1L << 16) - 1L) / (1L << 16)))

// Dummy frequency used to for silent tones (rests).
#define SILENT_FREQ 250

class ArduboyTones
{
 public:
  /// The class constructor.
  /**
   * The function passed to the constructor must return "true" if sounds
   * should be played, or "false" if all sound should be muted.
   */
  ArduboyTones(bool (*outEn)());

  /// Play a single tone.
  /**
   * A duration of 0 means play forever or until noTone() is called or
   * a new tone or sequence is started.
   */
  static void tone(uint16_t freq, uint16_t dur);

  /// Play two tones in sequence.
  static void tone(uint16_t freq1, uint16_t dur1,
                   uint16_t freq2, uint16_t dur2);

  /// Play three tones in sequence.
  static void tone(uint16_t freq1, uint16_t dur1,
                   uint16_t freq2, uint16_t dur2,
                   uint16_t freq3, uint16_t dur3);

  /// Play a tone sequence from values in an array.
  /**
   * The array must be placed in code space using PROGMEM.
   */
  static void tone(const uint16_t *tones);

  /// Stop playing the tone or sequence.
  static void noTone();

  /// Check if a tone or tone sequence is playing.
  /**
   * returns "true" if playing.
   */
  static bool playing();
};

#endif
