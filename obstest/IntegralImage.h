#ifndef DEF_INTEGRALIMAGE
#define DEF_INTEGRALIMAGE

#include "GreyImage.h"

struct IntegralImage
{
	int width;
	int height;
	int *pixels;
};
typedef struct IntegralImage IntegralImage;

IntegralImage *IntegralImageNew(GreyImage *image);
void IntegralImageFree(IntegralImage *image);
int IntegralImageGetSum(IntegralImage *image, int x, int y, int w, int h);
int IntegralImageGetDeviation(IntegralImage *image, IntegralImage *squaredImage, double scale ,int x, int y, int w, int h);


#endif
