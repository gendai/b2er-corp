#include "Detector.h"
#include "GreyImage.h"
#include "IntegralImage.h"
#include "dimensions.h"
#include <math.h>
#include <stdlib.h>

static int facesInSameRegion(DetectedFace a, DetectedFace b)
{
	return a.x < b.x + b.w &&
		a.x + a.w > b.x &&
		a.y < b.y + b.h &&
		a.y + a.h > b.y;
}

static void sortFacesBySize(DetectedFace *faces, int n)
{
	int swapped;
	do
	{
		swapped = 0;
		--n;
		for(int i = 0; i < n; ++i)
		{
			if(faces[i].w > faces[i+1].w)
			{
				DetectedFace temp = faces[i];
				faces[i] = faces[i+1];
				faces[i+1] = temp;
				swapped = 1;
			}
  		}
	} while(swapped);
}

static DetectedFace **getRegions(DetectedFace *faces, int nbFaces, int **regionsInfos)
{
	DetectedFace **regions = malloc(nbFaces * sizeof(DetectedFace *));
	int size = 0;
	*regionsInfos = malloc((nbFaces+1)*sizeof(int));

	for(int i = 0; i < nbFaces; ++i)
	{
		DetectedFace face = faces[i];
		int inRegion = 0;

		for(int j = 0; j < size; ++j)
		{
			DetectedFace *region = regions[j];
			if(face.w == region->w)
			{
				int regionSize = (*regionsInfos)[j+1];
				for(int k = 0; k < regionSize; ++k)
				{
					if(facesInSameRegion(face, region[k]))
					{
						inRegion = 1;
						(*regionsInfos)[j+1]++;
						region[regionSize] = face;
						break;
					}
				}
				if(inRegion)
					break;
			}
		}

		
		if(!inRegion)
		{
			regions[size] = malloc(nbFaces * sizeof(DetectedFace));
			regions[size][0] = face;
			(*regionsInfos)[++size] = 1;
		}
	
	}

	**regionsInfos = size;
	return regions;
}

static DetectedFace *postProcessing(DetectedFace *faces, int size, int *newsize)
{
	int size1 = 0;
	DetectedFace *trueFaces1 = malloc(size * sizeof(DetectedFace));
	int *regionsInfos;
	DetectedFace **regions = getRegions(faces, size, &regionsInfos);
	int nbRegions = *regionsInfos;
	double *confidenceVals = malloc(nbRegions * sizeof(double));

	for(int i = 0; i < nbRegions; ++i)
	{
		DetectedFace *region = regions[i];
		int nbFacesInRegion = regionsInfos[i+1];
		double confidence = (double)nbFacesInRegion / (double)region->w;
		if(confidence > CONFIDENCE_THRESHOLD)
		{
			trueFaces1[size1] = region[nbFacesInRegion/2];
			confidenceVals[size1++] = confidence;
		}
	}

	sortFacesBySize(trueFaces1, size1);

	DetectedFace *trueFaces2 = malloc(size1 * sizeof(DetectedFace));
	int size2 = 0;

	for(int i = 0; i < size1; ++i)
	{
		DetectedFace face = trueFaces1[i];
		int faceX = face.x + face.w/2;
		int faceY = face.y + face.h/2;
		
		for(int j = i+1; j < size1; ++j)
		{
			DetectedFace face2 = trueFaces1[j];
			if(face2.x >= 0
			   && faceX > face2.x && faceX < face2.x + face2.w
			   && faceY > face2.y && faceY < face2.y + face2.h)
			{
				if(confidenceVals[i] > confidenceVals[j])
				{
					trueFaces1[j].x = -1;
				}
				else
				{
					trueFaces1[i].x = -1;
					break;
				}
			}
		}
	}

	for(int i = 0; i < size1; ++i)
	{
		if(trueFaces1[i].x >= 0)
			trueFaces2[size2++] = trueFaces1[i];
	}

	
	*newsize = size2;
	return trueFaces2;
}


static int getDeviation(IntegralImage *image, IntegralImage *squaredImage, double scale ,int x, int y, int w, int h)
{
	int nbPixs = (int)((24*scale) * (24*scale));
	int sum = IntegralImageGetSum(image, x, y, w, h);
	int squaredSum = IntegralImageGetSum(squaredImage, x, y, w, h);
	int average = sum / nbPixs;
	int variance = squaredSum / nbPixs - average*average;
	if(variance > 0)
        {
          return sqrt(variance);
        }else{
          return 1;
        }
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

	static DetectedFace faces[200000];
	int nbFaces = 0;

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
				int deviation = getDeviation(intImage, squaredImage, scale ,x, y, width, height);
				
				if(StrongClassifierCheck(sc, intImage, x, y, scale, deviation))
				{
					faces[nbFaces].x = x;
					faces[nbFaces].y = y;
					faces[nbFaces].w = width;
					faces[nbFaces].h = height;
					++nbFaces;
				}

				scale *= WIN_RATIO;
				width = WIN_WIDTH * scale;
				height = WIN_HEIGHT * scale;
			}
			
			
			
		}
	}
	
	
	DetectedFace *trueFaces = postProcessing(faces, nbFaces, &nbFaces);
	for(int i = 0; i < nbFaces; ++i)
		addRect(pixbuf, trueFaces[i].x, trueFaces[i].y, trueFaces[i].w, trueFaces[i].h);

	
	
}
