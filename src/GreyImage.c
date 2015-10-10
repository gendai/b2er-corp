#include "GreyImage.h"
#include <stdlib.h>


GreyImage *GreyImageNew(int width, int height)
{
	GreyImage *image = malloc(sizeof(GreyImage));
	image->width = width;
	image->height = height;
	image->pixels = malloc(width * height * sizeof(guchar));
	return image;
}


static guchar greyPixel(guchar *pixel)
{
	guchar val = (guchar) ( ( (int) pixel[0] + (int) pixel[1] + (int) pixel[2]  )/3);
	//pixel[0] = pixel[1] = pixel[2] = val;
	return val;
}

GreyImage *GreyImageNewFromImage(GtkImage *image)
{

	GdkPixbuf *pixBuf = gtk_image_get_pixbuf(image);
	int width = gdk_pixbuf_get_width(pixBuf);
	int height = gdk_pixbuf_get_height(pixBuf);
	int rowStride = gdk_pixbuf_get_rowstride(pixBuf);
	int nChannels = gdk_pixbuf_get_n_channels(pixBuf);
	guchar *pixels = gdk_pixbuf_get_pixels(pixBuf);
	GreyImage *grey = GreyImageNew(width, height);
	
	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			grey->pixels[i*width+j] = greyPixel(pixels + i * rowStride + j * nChannels);
		}
	}
	
	return grey;
}

void GreyImageFree(GreyImage *image)
{
	free(image->pixels);
	free(image);
}

GtkImage *GreyImageToImage(GreyImage *image)
{
	int width = image->width;
	int height = image->height;
	
	GdkPixbuf *pixBuf = gdk_pixbuf_new(
		GDK_COLORSPACE_RGB,
		0,
		8,
		width,
		height
		);
	
	int rowStride = gdk_pixbuf_get_rowstride(pixBuf);
	int nChannels = gdk_pixbuf_get_n_channels(pixBuf);
	guchar *pixels = gdk_pixbuf_get_pixels(pixBuf);
	
	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			guchar *pixel = pixels + i * rowStride + j * nChannels;
			pixel[0] = pixel[1] = pixel[2] = image->pixels[i*width+j];
		}
	}

	GtkImage *out = (GtkImage *) gtk_image_new_from_pixbuf(pixBuf);
	return out;
}

GreyImage *GreyImageResize(GreyImage *image, int outW, int outH)
{
	GreyImage *scaled = GreyImageNew(outW, outH);
	guchar *inP = image->pixels;
	guchar *outP = scaled->pixels;
	int inW = image->width;
	int inH = image->height;
	int xRatio = ((inW - 1) << 16) / outW;
	int yRatio = ((inH - 1) << 16) / outH;
	long y = 0;
	
	for (int i = 0; i < outH; ++i)
	{
		int yr = y >> 16;
		long y_diff = y - (yr << 16);
		long one_min_y_diff = 65536 - y_diff;
		int y_index = yr * inW;
		long x = 0;
		
		for (int j = 0; j < outW; ++j)
		{
			int xr = x >> 16;
			long x_diff = x - (xr << 16);
			long one_min_x_diff = 65536 - x_diff;
			int index = y_index + xr;

			outP[i * outW + j] = (
				inP[index]*one_min_x_diff*one_min_y_diff +
				inP[index+1]*x_diff*one_min_y_diff +
				inP[index+inW]*y_diff*one_min_x_diff +
				inP[index+inW+1]*x_diff*y_diff
				) >> 32;
			
			x += xRatio;
		}
		
		y += yRatio;
	}	


	return scaled;
}

GreyImage *GreyImageScale(GreyImage *image, float scale)
{
	return GreyImageResize(image, image->width * scale, image->height * scale);
}
