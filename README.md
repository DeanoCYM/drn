# Overview

DRN provides an event loop which periodically sets the root window name in X to
a customisable string. The string is constructed from callbacks to a shared
library.

This project is a contrived example, written to practice the use of dynamically
loaded shared objects, linked lists, string manipulation, UNIX signal handling
and systemd unit files. However, it is useable as a status bar for dwm.

# Installation

```
# PREFIX=/usr/local make install
```

# Usage

```
$ drn <delimiter> <callback> [callback] ... [callback]
```

e.g.


```
$ drn " | " batt_capacity batt_status local_time &
```

Would set the root window to:

```
 Charging | 100% | Mon 09/12/18 08:38PM
```

By defualt, the root window is updated every 5s.
drn can be stopped gracefully by sending SIGTERM or SIGINT

# systemd

drn can be configured to autostart upon launch of an Xsession with using
systemd. An example unit file is in `etc/drn.service`. The drn service file requires systemd to be aware when an Xsession is started. To acheive this, a target must be created; an example target `etc/xsession.target` is provided.

To use the provided unit files, modify to your requirements and copy into into your systemd user directory.

```
$ cp etc/{xsession.target,drn.service} $HOME/.config/systemd/user/
```

To provide the required hooks at start up, run the following commands. Append to your `~/.xinitrc` or `~/.xsession` to make persistant.

```
systemctl --user import-environment PATH DBUS_SESSION_BUS_ADDRESS DISPLAY
systemctl --no-block --user start xsession.target
```

Finally, enable and start drn.

```
$ systemctl --user enable drn.service
$ systemctl --user start drn.service
$ systemctl --user daemon-reload
```

Customisation
=============

Callbacks can be added or modified in src/drn_cb.c, they should take no
arguements and return a pointer to a string.

``` c
typedef char *(*getstr)(void);
```

Once modified, the updated library must be updated and installed and drn restarted:

```
# make libinstall
$ systemctl --user restart drn.service
```

or

```
$ kill $(YOURPID);
$ drn <delimiter> <callback> [callback] ... [callback]
```

# Uninstall

```
# PREFIX=/usr/local make uninstall
```










