/*****************************************************************************
* ArduboyTones
*
* A library to play tone sequences.
*
*****************************************************************************/

#include "ArduboyTones.h"

// pointer to a function that indicates if sound is enabled
static bool (*outputEnabled)();

static volatile long durationToggleCount = 0;
static volatile bool tonesPlaying = false;
static volatile bool toneSilent;

ArduboyTones::ArduboyTones(boolean (*outEn)())
{
  outputEnabled = outEn; // TODO handle mute.

  bitClear(TONE_PIN_PORT, TONE_PIN); // set the pin low
  bitSet(TONE_PIN_DDR, TONE_PIN); // set the pin to output mode
}

void ArduboyTones::tone(uint16_t freq, uint16_t dur)
{
  long toggleCount = 0;
  uint32_t ocrValue;
  uint8_t tccrxbValue;

  tonesPlaying = true; 

  if (freq >= MIN_NO_PRESCALE_FREQ) {
    tccrxbValue = _BV(WGM32) | _BV(CS30); // no prescaling
    ocrValue = F_CPU / freq / 2 - 1;
    toneSilent = false;
  }
  else {
    tccrxbValue = _BV(WGM32) | _BV(CS31); // prescaler /8
    if (freq == 0) {
      ocrValue = F_CPU / SILENT_FREQ / 2 / 8 - 1; // dummy tone for silence
      freq = SILENT_FREQ;
      toneSilent = true;
    }
    else {
      ocrValue = F_CPU / freq / 2 / 8 - 1;
      toneSilent = false;
    }
  }

  if (dur > 0) {
    toggleCount = (long)freq * (long)dur * 2 / 1000;
  }
  else {
    toggleCount = -1; // indicate infinite duration
  }

  TCCR3A = 0;
  TCCR3B = tccrxbValue;
  OCR3A = ocrValue;
  durationToggleCount = toggleCount;
  bitWrite(TIMSK3, OCIE3A, 1); // enable the output compare match interrupt
}

void ArduboyTones::noTone()
{
  bitWrite(TIMSK3, OCIE3A, 0); // disable the output compare match interrupt
  TCCR3B = 0; // stop the counter
  bitClear(TONE_PIN_PORT, TONE_PIN); // set the pin low
  tonesPlaying = false;
}

bool ArduboyTones::playing() {
  return tonesPlaying;
}

ISR(TIMER3_COMPA_vect)
{
  if (durationToggleCount != 0) {
    if (!toneSilent) {
      *(&TONE_PIN_PORT) ^= TONE_PIN_MASK; // toggle the pin
    }
    if (durationToggleCount > 0) {
      durationToggleCount--;
    }
  }
  else {
    ArduboyTones::noTone();
  }
}

