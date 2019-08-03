# Arduino_MS6264L
Example code of using a MS6264 CMOS static RAM chip with an Arduino MEGA.

The MS6264L chip provides 8Kb of static RAM addressable through 13 address pins (A0-A12).
However, this example code only uses 8 of the address pins (A0-A7) so it only exposes 256 bytes of RAM.
This example code provides functions to write and read from RAM, and also includes instructions for extending the example code to include all 13 address pins.
