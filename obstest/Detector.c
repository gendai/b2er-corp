#include "Detector.h"
#include "GreyImage.h"
#include "IntegralImage.h"
#include "dimensions.h"
#include <math.h>

static int getDeviation(IntegralImage *image, IntegralImage *squaredImage)
{
	int nbPixs = image->width * image->height;
	int sum = image->pixels[nbPixs - 1];
	int squaredSum = squaredImage->pixels[nbPixs - 1];
	int average = sum / nbPixs;
	int variance = squaredSum / nbPixs - average*average;
	if(variance < 0)
		variance = -variance;
	printf("Deviation: %d\n", (int) sqrt(variance));
	return variance > 0 ? sqrt(variance) : 1;
}

static void pixelToRed(guchar *pixel)
{
	pixel[0] = 255;
	pixel[1] = pixel[2] = 0;
}

static void addRect(GdkPixbuf *pixbuf, int x, int y, int w, int h)
{
	int rowStride = gdk_pixbuf_get_rowstride(pixbuf);
	int nChannels = gdk_pixbuf_get_n_channels(pixbuf);
	guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);
	
	
	for(int i = x; i < x+w; ++i)
	{
		pixelToRed(pixels + y*rowStride + i*nChannels);
		pixelToRed(pixels + (y+h-1)*rowStride + i*nChannels);
	}

	for(int i = y; i < y+h; ++i)
	{
		pixelToRed(pixels + i*rowStride + x*nChannels);
		pixelToRed(pixels + i*rowStride + (x+w-1)*nChannels);
	}
}

void detectFaces(GtkImage *image, StrongClassifier *sc)
{

	GdkPixbuf *pixbuf = gtk_image_get_pixbuf(image);
	GreyImage *grey = GreyImageNewFromImage(image);
	IntegralImage *intImage = IntegralImageNew(grey);

	int imgW = intImage->width;
	int imgH = intImage->height;
	int maxX = imgW - WIN_WIDTH;
	int maxY = imgH - WIN_HEIGHT;

	GreyImage *squaredGrey = GreyImageNew(imgW, imgH);
	for(int i = 0, count = imgW*imgH; i < count; ++i)
		squaredGrey->pixels[i] = grey->pixels[i]*grey->pixels[i];

	IntegralImage *squaredImage = IntegralImageNew(squaredGrey);
	int deviation = getDeviation(intImage, squaredImage);

	int pos = 0;
	int neg = 0;

	for(int x = 0; x < maxX; ++x)
	{
		for(int y = 0; y < maxY; ++y)
		{

			double scale = 1;
			int maxWidth = imgW - x;
			int maxHeight = imgH - y;
			int width = WIN_WIDTH;
			int height = WIN_HEIGHT;

			while(width <= maxWidth && height <= maxHeight)
			{
				if(StrongClassifierCheck(sc, intImage, x, y, scale, deviation))
				{
					printf("(%d, %d) ! [%d * %d]\n", x, y, width, height);
					++pos;
					addRect(pixbuf, x, y, width, height);
				}
				else
					++neg;

				scale *= WIN_RATIO;
				width = WIN_WIDTH * scale;
				height = WIN_HEIGHT * scale;
			}
			
			
			
		}
	}

	printf("+ : %d , - : %d\n", pos, neg);
	
}
