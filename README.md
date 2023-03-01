# 2SNES2USB
An Arduino sketch that creates a dual SNES to dual HID USB gamepad adapter.  This was developed for use with a Seeeduino Xiao SAMD21 microcontroller, but it should work with any board compatible with [NicoHood's HID-Project Library](https://github.com/NicoHood/HID).  Up to 4 controllers should be possible with minor edits to the sketch, but this is untested.

My testing (with a SAMD21) shows a 1000hz polling rate, so there should be no perceptible input lag.
