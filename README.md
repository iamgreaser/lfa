LFA: Lard-Free Audio - A wrapper for the PulseAudio API

Currently this just wraps PA to OSS. I would suggest chucking in ezjack and wrapping it to JACK instead, because after all that's what you should be using instead of PulseAudio, and JACK is less painful than ALSA to get working properly even without ezjack.

Stuff that's confirmed to work:

* pasimple 0.9.23
* paplay 0.9.23
* SDL 1.2.15

Crucial features that are missing:

* JACK support
* Recording support
* Enough stuff to make ALSA's module work

"Features" that are crucially missing:

* The PulseAudio Stutter(TM)

