# Image Resampling, Quantization and Filtering
The program can run at the command line as: xxx.exe C:/myDir/myImage.rgb S Q M

1. The first parameter is the name of the image, which will be 8 bit per channel RGB format (Total 24 bits per pixel). 
2. The second parameter will be a Scale value. This will control by how much the image has to be scaled. It will be a floating-point number greater than 0.0 but less than or equal to 1.0.
3. The third parameter will be a Quantization number giving the number of bits each channel needs to be quantized down to. The initial rgb file has 8 bits per channel So this Q value will be a value such that 1<= Q <= 8.
4. The fourth parameter will suggest a Mode for quantization. Its value will be an integer. A -1 indicates uniform quantization (explained below) or a whole valued number (between 0 – 255) suggests a pivot for smarter logarithmic quantization.
