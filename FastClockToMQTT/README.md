The jython script runs in JMRI and publishes three topics.
- full date and time
- hour
- minute

The ino file is an arduino sketch that subscribes the JMRI fastclock topic and displays the time on the TM1637 LED display.
