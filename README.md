CMUS Lyrics Daemon
==================

This is a daemon program that can send current lines of the lrc lyrics which
is currently playing in CMUS to an FIFO pipe. The lyrics can be printed on 
the desktop by using Geektool.

How to Use
----------

Use *which* if you are not sure where cmus-remote is.

Please put your lrc files in the folder you specified and name them exactly 
the same as the music file related to that except for the filename extension.
For example, if your music file is "Enya - Only Time.ape", you should name 
your lrc file "Enya - Only Time.lrc".

When both cmus and this daemon are running, the current line of lyrics and a 
few lines of lyrics before and after it will be sent to the FIFO pipe.

Precision problem: 
The minimum precision of the position information given by cmus-remote is one
second, while the min precision of lrc is one centisecond, this difference 
may make the timing of the lyrics slightly imprecise, but it is not a serious
problem since there are five lines being shown at the same time.

Working with GeekTool
------------------

![ScreenShot](https://raw.github.com/equinox1993/CMUS-Lyrics-Daemon/master/screenshot.png)

If the FIFO pipe used by the daemon is */tmp/cmuslyricspipe*, let GeekTool 
run "cat < /tmp/cmuslyricspipe" every one second.

How It Works
------------

This daemon process periodically grabs status information of cmus by calling 
"cmus-remote -Q". It uses the file path information given by cmus-remote to 
form the path of the lrc file. If this path is different from the current path 
being stored and a file exists at that path, the file will be loaded into the 
Lyrics object. This process will pass current time position to the object and 
the lyrics at / near that time will be returned and sent through the pipe.

Classes Implemented
-------------------

Lyrics:
Line A is consider as the current lyrics if current time position is between 
A (inclusive) and B (exclusive).
A class that loads the lrc code into a string vector that stores the lines of 
lyrics and an integer time array that stores the relations from time 
(centisec) to index of lyrics array. The initial size of the time array is 
10 min * 60 min/sec * 100 centisec/sec * 2 bytes/short = 117 kb; searching for 
lyrics at any time positions will be constant time operation.

CmusStatus:
A class that grabs status information from cmus-remote.

These two classes are written in C++ so they can be used in any other projects
or unix-like systems.

Licensing
---------

CMUS Lyrics Daemon is licensed under the GNU General Public License version 3.

Yuwei Huang
2014.3.10