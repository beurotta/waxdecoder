# WaxDecoder
## Description
WaxDecoder is an implementation in Sensomusic Usine of open source xwax decoder for timecoded discs used in DJ software (xwax.org). This implementation ports xwax code (written in C) to Usine Hollyhock through the C++ SDK provided by Sensomusic (www.sensomusic.org).

This module takes a stereo input from a timecoded disc and outputs the pitch of the disc and the position of the needle, directly computed by xwax code.

## Installation
Just copy the user module file corresponding to your platform (WaxDecoder.usr-win32 or WaxDecoder.usr-win64) to your user module folder configured in Usine.

## Usage
This module takes a stereo input from a timecoded disc and outputs the pitch of the disc and the position of the needle, directly computed by xwax code.

There are three settings which account for:
- manufacturer of your timecoded disc (Serato, Traktor or MixVibes discs);
- rotational speed of the deck (33/45 rpm);
- a 'software preamp' if you use an unamplified phono signal connected to a line-level interface.

As it is a raw implementation of the library, you may encounter gaps in position read by the module or small variation in pitch. This raw implementation is intended to keep the inherent modularity of Usine. A patch example is given and contains useful subpatchs that can be used to filter WaxDecoder output signal.

## Versions 
- 2012/07/04
  - first release, implement the timecoder used in xwax v1.2 in Sensomusic Usine v5 with SDK v5.70
- 2018/11/16
  - updated to Usine SDK HH3-7.01.006
  - based on xwax v1.7 sources
  - project migrated to Visual Studio
  - code has been cleaned
  - xwax v1.7 sources implemented
  - hardware settings moved from parameters
  - optimisation of the onProcess callback
  - sample rate and block size modifications are now handled

## Contributing
You are free to use/modify this code. See the License.txt file for more details.

## Credits
- xwax timecoded disc decoder : Mark Hills
- Hollyhock SDK : Sensomusic team
- WaxDecoder Usine module : Arnaud Beurotte

Modifications made on xwax source files are labelled 'MODS'

## License
MIT and GNU GPL v2 licenses are used. See the License.txt file for more details.