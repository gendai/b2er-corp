#ifndef DEF_TRAININGIMAGE
#define DEF_TRAININGIMAGE

#include "IntegralImage.h"

struct TrainingImage
{
	IntegralImage *image;
	int valid;
	int deviation;
	double weight;
};
typedef struct TrainingImage TrainingImage;


TrainingImage *TrainingImageNew(GreyImage *grey, int valid, double weight);
void TrainingImageFree(TrainingImage *ti);

#endif
