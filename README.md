# MX8650-reading-and-generating-image-from-mouse-sensor
Reading and writing data from registers of MX8650 mouse sensor | Generating image from mouse sensor | Using MX8650 as an interferometer to detect minor vibrations

Kindly refer the datasheet before modifying the code.

The transistor is necessary to act as CS pin for the MX8560 since the display device also communicates via SPI. If not used, commands to the display will also be recieved by the MX8560. If you are not using the display, remove or comment out the specific codes which are marked in the code.

Regards
