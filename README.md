# Console-Image-Display
Console Image Display (IDS) is an image viewer that was built entirely within the windows console in C++.
All of the image processing is done with OpenCV 3.4.3.

<b>Features:</b>

    Works with all image types
    Greyscale / Ascii mode
    Automatic image resizing (for really big / small images)
    Adjustable font size


IDS works by taking command line arguments specifying the image to be displayed, the font size of the image, and image options.
<b>Usage:</b>

	options / flags:
		a,	ascii			displays the image using ascii characters [0x20 (32) - 0x7E (127)]
		g,	greyscale		displays the image as greyscale
		
	command format:
		*.exe <image path> [font size] [options]

if flag -a is left out of the arguments the image will be displayed using ascii character 0xDB (219)
