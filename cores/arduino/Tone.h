#ifndef _WIRING_TONE_
#define _WIRING_TONE_

#ifdef __cplusplus

    // Generate a tone to a pin.
    extern void tone(uint8_t _pin, unsigned int frequency, unsigned long duration = 0);
    // brief Stop tone generation on pin.
    extern void noTone(uint8_t _pin);

#endif

#endif /* _WIRING_TONE_ */
