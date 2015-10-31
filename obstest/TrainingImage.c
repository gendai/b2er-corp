#include "TrainingImage.h"
#include <stdlib.h>
#include "GreyImage.h"
#include "dimensions.h"


TrainingImage *TrainingImageNew(GreyImage *grey, int valid, double weight)
{
	TrainingImage *ti = malloc(sizeof(TrainingImage));	
	ti->image = IntegralImageNew(grey);

	GreyImage *squaredGrey = GreyImageNew(grey->width, grey->height);
	for(int i = 0, count = grey->width*grey->height; i < count; ++i)
		squaredGrey->pixels[i] = grey->pixels[i]*grey->pixels[i];
	IntegralImage *squaredImage = IntegralImageNew(squaredGrey);

	ti->deviation = IntegralImageGetDeviation(ti->image, squaredImage, (double) WIN_WIDTH / (double) grey->width , 0, 0, WIN_WIDTH, WIN_HEIGHT);
	ti->valid= valid;
	ti->weight = weight;
	
	GreyImageFree(grey);
	GreyImageFree(squaredGrey);
	IntegralImageFree(squaredImage);

	return ti;
}

void TrainingImageFree(TrainingImage *ti)
{
	free(ti->image);
	free(ti);
}
