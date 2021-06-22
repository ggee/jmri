The jython script runs in JMRI and publishes three topics.
- full date and time
- hour
- minute

The ino files are an arduino sketch that subscribes the JMRI fastclock topic and displays the time on the display.  There are 3 examples.
- ESP6288 with TM1637 4-Digit 7-Segment Display
- ESP8266 with 128x64 OLED showing digital clock (TBD)
- ESP8266 with 128x64 OLED showing analog clock (TBD)
