Overview
========

DRN provides an event loop which periodically sets the root window name in X to
a customisable string. The string is constructed from callbacks to a shared
library.

This project is a conrived example, written to practice the use of dynamically
loaded shared objects, linked lists, string manipulation, UNIX signal handling
and systemd unit files. However, it is useable as a status bar for dwm.

Installation
============

`# PREFIX=/usr/local make install`

Usage
=====

`$ drn <delimiter> <callback1> [callback2] ... [callbackN]`

e.g.

`$ drn " | " batt_capacity batt_status local_time &`

Would set the root window to:

` | Charging | 100% | Mon 09/12/18`

By defualt, the root window is updated every 5s.

drn can be stopped gracefully by sending SIGTERM or SIGINT

Systemd
=======

drn can be configured to autostart upon launch of an Xsession with using
systemd. An example unit file is


Customisation
=============

Callbacks can be added or modified in src/drn_cb.c, they should take no
arguements and return a pointer to a string.

`typedef char *(*getstr)(void);`

Once modified, the updated library must be updated and installed and drn restarted:

`# make libinstall
$ systemctl --user restart drn.service`

or

`$ kill $(YOURPID);
$ drn <delimiter> <callback1> [callback2] ... [callbackN]`

Uninstall

`# PREFIX=/usr/local make uninstall`








