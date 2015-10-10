#include "IntegralImage.h"
#include <stdlib.h>

IntegralImage *IntegralImageNew(GreyImage *image)
{
	int width = image->width;
	int height = image->height;
	IntegralImage *integral = malloc(sizeof(GreyImage));
	integral->width = width;
	integral->height = height;
	integral->pixels = malloc(width * height * sizeof(int));

	integral->pixels[0] = image->pixels[0];
	for(int i = 1; i < width; ++i)
		integral->pixels[i] = integral->pixels[i-1] + image->pixels[i];
	for(int i = 1; i < height; ++i)
		integral->pixels[i*width] = integral->pixels[(i-1)*width] + image->pixels[i*width];

	for(int i = 1; i < height; ++i)
	{
		for(int j = 1; j < width; ++j)
		{
			integral->pixels[i*width + j] = image->pixels[i*width+j] + integral->pixels[(i-1)*width + j] + integral->pixels[i*width + j-1] - integral->pixels[(i-1)*width + j-1];
		}
	}

	return integral;
}

void IntegralImageFree(IntegralImage *image)
{
	free(image->pixels);
	free(image);
}

int IntegralImageGetSum(IntegralImage *image, int x, int y, int w, int h)
{
	int width = image->width;
	int r1 = !x || !y ? 0 : (int) image->pixels[(y-1)*width +  x-1];
	int r2 = !x ? 0 : (int) image->pixels[(y+h-1)*width + x-1];
	int r3 = !y ? 0 : (int) image->pixels[(y-1)*width + x+w-1];
	int r4 = (int) image->pixels[(y+h-1)*width + x+w-1];
	return r1 + r4 - r2 - r3;
}
