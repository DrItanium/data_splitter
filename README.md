This is a program meant to make it easy to carve a memory image up into multiple images by moving 
each individual byte to a separate split file. So for example, lets say that
you have two 8-bit flash chips on your design which hold onto upper and lower
bytes of a 16-bit word. This program will take a single input image and
generate two files. One will contain all of the bytes at even addresses (0, 2,
4, 6, 8, 10, 12, etc) and the other will contain bytes the odd addresses (1, 3, 5, 7, 9, 11).

Right now, the goal is to provide support for 16-bit and 32-bit busses but if I
am successful then it will become very easy to support more widths as time goes
on.
