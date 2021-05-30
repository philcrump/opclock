# OpClock

UTC Date and Time clock with event feed designed for co-ordinating spacecraft operations.

This software has been developed with the official Raspberry Pi 7" touchscreen and a Raspberry Pi 4.

<p align="center">
  <img src="/img/eventcountdown.gif" width="50%" />
</p>

## Dependencies

`sudo apt install build-essential libcurl4-openssl-dev`

## Configuration

Copy _config.ini.template_ to _config.ini_ and customise.

## Compile and Install 

```
make
sudo systemctl enable systemd-time-wait-sync
sudo ./install
```

### Raspberry Pi Modifications for fast & graphically-clean boot

`sudo systemctl disable getty@tty1.service`

#### /boot/config.txt:

* Comment out the line: `dtoverlay=vc4-fkms-v3d`
* Append the following lines:
```
disable_splash=1
boot_delay=0
dtoverlay=pi3-disable-bt
```
* Optional, requires RPi4(?) and UHS-1 SD Card, append the line: `dtoverlay=sdtweak,overclock_50=100`

#### /boot/cmdline.txt

* Remove `console=tty1`
* Append `vt.global_cursor_default=0 logo.nologo`

## Usage

Reboot to apply the boot modifications and let the clock start automatically.

### Waiting for Network & Time Sync on boot

As the Raspberry Pi has no RTC the clock will not start until systemd reports that the system time has been synchronised by NTP. This means that networking and NTP need to be configured correctly and be available for the clock application to start.

<p float="left">
  <img src="/img/waitfornetwork.png" width="49%" />
  <img src="/img/waitfortimesync.png" width="49%" />
</p>

### Backlight On/Off Control

You can switch off the display backlight by double-tapping the touchscreen anywhere. A single tap anywhere will then switch the backlight back on.

### HTTP Events Endpoint

When configured for the HTTP/HTTPS endpoint, the application expects the endpoint to return a JSON structure containing:
```
{
       "events": [
              {
                     'description': 'AoS ISS',
                     'type': 2,
                     'time_unix': 1524382363
              },
              ...
       ]
}

```

An example endpoint is served on <https://localhost:4443/> by the python3 script in _test-http-endpoint_, this can be run by:
```
cd test-http-endpoint
./test-http-endpoint.py
```
This test script will generate events for the next few minutes when first started, so needs to be restarted regularly for fresh events to be available for the display.

### Event Types

A few numeric event types are implemented to render simple highlight graphics on the display.

* TYPE_NONE = 0 (No graphic)
* TYPE_DOWNLINK_START = 1 (blue ramp-up)
* TYPE_DOWNLINK_STOP = 2 (blue ramp-down)
* TYPE_UPLINK_START = 3 (yellow ramp-up)
* TYPE_UPLINK_STOP = 4 (yellow ramp-down)

## Licensing

Unless otherwise specified at the top of the relevant file, all materials of this project are licensed under the BSD 3-clause License (New BSD License).

Copyright (c) Phil Crump 2021. All rights reserved.