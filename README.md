New README (2011)
---

Cmod is a text-mode music player which plays files in IT, XM, S3M and MOD formats.

It uses the excellent libbassmod (I honestly think this is the best one to play Impulse Tracker songs).

I wrote this back in 2006, originally in Linux, but I have now ported it to Mac OSX Lion because I ran across with an Intel 32-bits version of libbassmod.dylib. If you have an afternoon it must be very easy to compile this in a Linux box with the appropiate libbassmod.so

In the dist/ folder you'll find a precompiled binary for Lion. Xcode project is also there.

Find a snapshot here: http://herotyc.untergrund.net/l/cmod.png

---

Original README (2006) below:

---

[ cmod 0.1    :: herotyc^asynkro    ]

[ herotyc.com :: jlg.hrtc@gmail.com ]

[ 02-11-2006 ]

cmod is a textmode module player based on
ncurses and bassmod for playback

info:

I recommend you to copy libbassmod.so to /usr/lib.

If it is not possible, run
$export LD_LIBRARY_PATH='path/to/lib'
before running. If you dont wish to do this
everytime, add the path to /etc/ld.so.conf

For macosx, copy the .dylib somewhere in
the path(maybe /usr/local/lib).

--

keys:

space or enter - play

q - quit

s - stop

, - seek backward

. - seek forward

\+ - volume up (0 to 100, default 100)

\- - volume down

\> - amplify up (0 to 100, default 50)

< - amplify down

l - toggle loop mode (default: off) (stops playback)

z - toggle random mode

a - add song to playlist

e - add all the songs in the current directory to playlist

c - add recursive

d - remove song from playlist

r - remove all songs from playlist

n - play next song in playlist

p - play previous song in playlist

g - center cursor on current song playing

left/right arrows - toggle between browser and playlist windows

up/down, home, end, ppage, npage - navigation

thanks to:

slack, shash and merlucin.

dsteele, sole for testing.

tonka for testing and macosx tiger compiling.
