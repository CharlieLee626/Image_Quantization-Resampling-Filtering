//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include "Image.h"
#include <iostream>
#include <cmath>
using namespace std;
#define PI 3.1415926

// Constructor and Desctructors
MyImage::MyImage() 
{
	Data = NULL;
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
}

MyImage::~MyImage()
{
	if ( Data )
		delete Data;
}


// Copy constructor
MyImage::MyImage( MyImage *otherImage)
{
	Height = otherImage->Height;
	Width  = otherImage->Width;
	Data   = new char[Width*Height*3];
	strcpy(otherImage->ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage->Data[i];
	}
}



// = operator overload
MyImage & MyImage::operator= (const MyImage &otherImage)
{
	//MyImage output;
	Height = otherImage.Height;
	Width  = otherImage.Width;
	Data   = new char[Width*Height*3];
	strcpy( (char *)otherImage.ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage.Data[i];
	}
	
	return *this;

}


// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage()
{

	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if ( IN_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Width*Height; i ++)
	{
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Bbuf[i] = fgetc(IN_FILE);
	}
	
	// Allocate Data structure and copy
	Data = new char[Width*Height*3];
	for (i = 0; i < Height*Width; i++)
	{
		Data[3*i]	= Bbuf[i];
		Data[3*i+1]	= Gbuf[i];
		Data[3*i+2]	= Rbuf[i];

		/*cout << int(Bbuf[i]) << int(Gbuf[i]) << int(Rbuf[i]) << endl;*/
	}

	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(IN_FILE);

	return true;

}



// MyImage functions defined here
bool MyImage::WriteImage()
{
	// Verify ImagePath
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *OUT_FILE;
	OUT_FILE = fopen(ImagePath, "wb");
	if ( OUT_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Height*Width; i++)
	{
		Bbuf[i] = Data[3*i];
		Gbuf[i] = Data[3*i+1];
		Rbuf[i] = Data[3*i+2];
	}

	
	// Write data to file
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Rbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Gbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Bbuf[i], OUT_FILE);
	}
	
	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(OUT_FILE);

	return true;

}

void averagefilter(const unsigned char* input_data, unsigned char* output_data, int input_width, int input_height)
{
	for (int m = 0; m < input_height; m++) {
		for (int n = 0; n < input_width; n++) {
			// Simply ignore the border
			if (n == 0 || n == input_width - 1 || m == 0 || m == input_height - 1) {
				output_data[m * input_width + n] = input_data[m * input_width + n];
			}
			else {
				output_data[m * input_width + n] =
						(input_data[(m - 1) * input_width + n - 1] +
						input_data[(m - 1) * input_width + n] +
						input_data[(m - 1) * input_width + n + 1] +
						input_data[m * input_width + n - 1] +
						input_data[m * input_width + n] +
						input_data[m * input_width + n + 1] +
						input_data[(m + 1) * input_width + n - 1] +
						input_data[(m + 1) * input_width + n] +
						input_data[(m + 1) * input_width + n + 1]) / 9;
			}
		}
	}
}

void quantization(unsigned char* input_data, unsigned char* output_data, int input_width, int input_height, int quan, int mode) {

	// If quantization parameter == 8, then there is no need to re quantize
	if (quan == 8) {
		for (int m = 0; m < input_height; m++) {
			for (int n = 0; n < input_width; n++) {
				output_data[m * input_width + n] = input_data[m * input_width + n];
			}
		}
	}

	// If quantization parameter != 8
	else {
		//uniform quantization
		if (mode == -1) {
			int interval = 256 / pow(2, quan);

			for (int m = 0; m < input_height; m++) {
				for (int n = 0; n < input_width; n++) {
					output_data[m * input_width + n] = int(input_data[m * input_width + n] / interval) * interval + int(interval / 2);
				}
			}
		}

		//a pivot for smarter logarithmic quantization
		else {
			int num_interval = pow(2, quan);
			int interval = 256 / num_interval;
			double* log_interval = new double[num_interval + 1];
			int split;
			if (mode % interval == 0)
				split = mode - (int(mode / interval)) * interval;
			else
				split = mode - (int(mode / interval) + 1) * interval;
			int _count = 0;
			log_interval[0] = split;
			_count++;
			//cout << log_interval[0] << endl;

			// Evenly split 0~255 to 2^quan intervals
			while (split < 255) {
				split += interval;
				log_interval[_count] = split;
				//cout << _count << ": " << split << ": " << log_interval[_count] << endl;
				_count++;
			}
			//cout << "Num of interval: " << _count - 1 << endl;

			// Shift the pivot num(mode) to 0
			for (int i = 0; i < _count; i++) {
				log_interval[i] -= mode;
				//cout << "Intervals: " << log_interval[i] << endl;
			}

			// Scale the larger part to 0~60 degree
			// Ex:	pivot = 90, then the interval would be -90~165 -> scale the (0~165) to (0 degree to 60 degree)
			//		Why 60 rather than 90? Because when degree is close to 90, the slope would rise too fast 
			double num_to_degree;
			int origin_num;
			if (mode < 128) {
				num_to_degree = abs(60.0 / log_interval[_count - 1]);
				origin_num = abs(log_interval[_count - 1]);
			}
			else {
				num_to_degree = abs(60.0 / log_interval[0]);
				origin_num = abs(log_interval[0]);
			}
			//cout << "origin num: " << origin_num << endl;
			//cout << "num_to_degree factor: " << num_to_degree << endl;
			for (int i = 0; i < _count; i++) {
				if (log_interval[i] > 0)
					log_interval[i] = log_interval[i] * num_to_degree;
				else
					log_interval[i] = log_interval[i] * num_to_degree;
				//cout << "Intervals in degree: " << log_interval[i] << endl;
			}

			// Tangent each interval to make them "Logarithmic"
			for (int i = 0; i < _count; i++) {
				log_interval[i] = tan(log_interval[i] * PI / 180.0);
				//cout << "Intervals after tangent: " << log_interval[i] << endl;
			}

			// Scale the tangented intervals back, and shift from (-pivot ~ 255-pivot) back to (0~255)
			double degree_to_num;
			if (mode < 128) {
				degree_to_num = abs(origin_num / log_interval[_count - 1]);
			}
			else {
				degree_to_num = abs(origin_num / log_interval[0]);
			}
			//cout << "degree_to_num factor: " << degree_to_num << endl;
			for (int i = 0; i < _count; i++) {
				if (log_interval[i] > 0)
					log_interval[i] = log_interval[i] * degree_to_num + mode;
				else
					log_interval[i] = log_interval[i] * degree_to_num + mode;
				//cout << "Intervals scaled back and shifted back: " << log_interval[i] << endl;
			}

			// Cut off the extra head and tail of log_interval
			if (log_interval[0] != 0)
				log_interval[0] = 0;
			if (log_interval[_count - 1] != 255)
				log_interval[_count - 1] = 255;

			/*for (int i = 0; i < _count; i++) {
				cout << "Intervals after cut off: " << log_interval[i] << endl;
			}*/

			// Quantize the input_data
			for (int m = 0; m < input_height; m++) {
				for (int n = 0; n < input_width; n++) {
					// Find out the data of this pixel is located at which interval
					for (int i = 0; i < _count; i++) {
						if (input_data[m * input_width + n] < log_interval[i]) {
							// the data of this pixel belongs to interval i-1~i
							output_data[m * input_width + n] = round((log_interval[i - 1] + log_interval[i]) / 2);
							break;
							//cout << "Between (" << i - 1 << ": " << log_interval[i - 1] << ", " << i << ": " << log_interval[i] << ")" << endl;
						}
					}
				}
			}
		}
	}
}

// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::Modify(float scale, int quan, int mode)
{
	// Store the R,G,B of each pixel from the origin image
	unsigned char* origin_Rbuf = new unsigned char[Width * Height];
	unsigned char* origin_Gbuf = new unsigned char[Width * Height];
	unsigned char* origin_Bbuf = new unsigned char[Width * Height];
	for (int i = 0; i < Height * Width; i++)
	{
		origin_Bbuf[i] = Data[3 * i];
		origin_Gbuf[i] = Data[3 * i + 1];
		origin_Rbuf[i] = Data[3 * i + 2];
	}
	cout << "Finish reading data to origin_buf" << endl;

	// Filter origin_RGBbuf to filtered_RGBbuf
	unsigned char* filtered_Rbuf = new unsigned char[Width * Height];
	unsigned char* filtered_Gbuf = new unsigned char[Width * Height];
	unsigned char* filtered_Bbuf = new unsigned char[Width * Height];
	averagefilter(origin_Bbuf, filtered_Bbuf, Width, Height);
	averagefilter(origin_Gbuf, filtered_Gbuf, Width, Height);
	averagefilter(origin_Rbuf, filtered_Rbuf, Width, Height);

	char* filtered_data;
	filtered_data = new char[Width * Height * 3];
	for (int i = 0; i < Width * Height; i++) {
		filtered_data[3 * i] = filtered_Bbuf[i];
		filtered_data[3 * i + 1] = filtered_Gbuf[i];
		filtered_data[3 * i + 2] = filtered_Rbuf[i];
	}
	cout << "Finish filtering data" << endl;

	// Quantize filtered_RGBbuf to quantized_RGBbuf
	unsigned char* quantized_Rbuf = new unsigned char[Width * Height];
	unsigned char* quantized_Gbuf = new unsigned char[Width * Height];
	unsigned char* quantized_Bbuf = new unsigned char[Width * Height];
	quantization(filtered_Bbuf, quantized_Bbuf, Width, Height, quan, mode);
	quantization(filtered_Gbuf, quantized_Gbuf, Width, Height, quan, mode);
	quantization(filtered_Rbuf, quantized_Rbuf, Width, Height, quan, mode);

	char* quantized_data;
	quantized_data = new char[Width * Height * 3];
	for (int i = 0; i < Width * Height; i++) {
		quantized_data[3 * i] = quantized_Bbuf[i];
		quantized_data[3 * i + 1] = quantized_Gbuf[i];
		quantized_data[3 * i + 2] = quantized_Rbuf[i];
	}
	cout << "Finish quantizing data" << endl;

	// Scale the filtered image depends on "scale" factor
	// Why doing this? It's for making the width and height of the new image exactly an integer !!!!!
	double scale_altered = round(scale * 64) / 64;
	int scaled_width = round(Width * scale_altered);
	int scaled_height = round(Height * scale_altered);
	cout << "scaled width: " << scaled_width << endl << "scaled height: " << scaled_height << endl;

	MyImage scaledImage;
	char* scaled_data;
	scaled_data = new char[scaled_width * scaled_height * 3];
	
	for (int i = 0; i < scaled_width * scaled_height; i++)
	{
		int scaled_x = i % scaled_width;
		int scaled_y = i / scaled_width;
		double x_portion = (scaled_x + 0.0) / scaled_width;
		double y_portion = (scaled_y + 0.0) / scaled_height;
		int origin_x = round(x_portion * Width);
		int origin_y = round(y_portion * Height);
		scaled_data[3 * i] = quantized_Bbuf[origin_y * Width + origin_x];
		scaled_data[3 * i + 1] = quantized_Gbuf[origin_y * Width + origin_x];
		scaled_data[3 * i + 2] = quantized_Rbuf[origin_y * Width + origin_x];
	}
	cout << "Finish scaling data" << endl;

	// Output the filtered / Quantized image of origin_image
	/*Width = scaled_width;
	Height = scaled_height;
	Data = quantized_data;*/

	// Output the scaled image of filtered and quantized data
	Width = scaled_width;
	Height = scaled_height;
	Data = scaled_data;
	//ImagePath = "output_image";

	/*scaledImage.setWidth(scaled_width);
	scaledImage.setHeight(scaled_height);
	scaledImage.setImageData(scaled_data);
	scaledImage.setImagePath("scaled_image");*/

	// Clean up and return
	delete origin_Rbuf;
	delete origin_Gbuf;
	delete origin_Bbuf;
	delete filtered_Rbuf;
	delete filtered_Gbuf;
	delete filtered_Bbuf;
	delete quantized_Rbuf;
	delete quantized_Gbuf;
	delete quantized_Bbuf;

	return false;
}