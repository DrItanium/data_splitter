This applications is meant to make it easy to split a binary image into
separate component files for uploading to multiple flash chips. Each of these
chips will contain an interleaved offset of the original data.


Given a 2 megabyte image that needs to be divided into four 512k blocks, the
program will do the following:

- Output 0 will contain bytes 0, 4, 8,  12, 16, 20, 24, 28, ... etc
- Output 1 will contain bytes 1, 5, 9,  13, 17, 21, 25, 29, ... etc 
- Output 2 will contain bytes 2, 6, 10, 14, 18, 22, 26, 30, ... etc
- Output 3 will contain bytes 3, 7, 11, 15, 19, 23, 27, 31, ... etc

If you were to make it a 64-bit bus then eight files will be generated. 

This tool has come about to solve the issue I have with my new i960
development board and its 32-bit data bus. I now have to take my boot firmware
and break it up into four 512k blocks for upload to flash chips. This program
handles the decomposition. 

Tools like objcopy will split a given binary file into contiguous parts but not
in an interleaved design like this tool does.

Theoretically, you can request splits from 2 to size_t-1. 

