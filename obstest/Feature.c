#include "Feature.h"
#include <stdlib.h>

Feature *FeatureNew(int type, int x, int y, int w, int h)
{
	Feature *feature = malloc(sizeof(Feature));
	feature->type = type;
	feature->x = x;
	feature->y = y;
	feature->w = w;
	feature->h = h;
	return feature;
}

void FeatureFree(Feature *feature)
{
	free(feature);
}

static int getFourRectValue(IntegralImage *image, int x, int y, int w, int h)
{
    int rectW = w/2;
	int rectH = h/2;
	
	int sumTL = IntegralImageGetSum(image, x, y, rectW, rectH);
	int sumTR = IntegralImageGetSum(image, x + rectW, y, rectW, rectH);
	int sumBL = IntegralImageGetSum(image, x, y + rectH, rectW, rectH);
	int sumBR = IntegralImageGetSum(image, x + rectW, y + rectH, rectW, rectH);
	return sumTL - sumTR - sumBL + sumBR;
}

static int getTwoVerticalValue(IntegralImage *image, int x, int y, int w, int h)
{
	int rectW = w;
	int rectH = h/2;
	return IntegralImageGetSum(image, x, y + rectH, rectW, rectH)
		- IntegralImageGetSum(image, x, y, rectW, rectH);
}

static int getTwoHorizontalValue(IntegralImage *image, int x, int y, int w, int h)
{
	int rectW = w/2;
	int rectH = h;
	return IntegralImageGetSum(image, x, y, rectW, rectH)
		- IntegralImageGetSum(image, x + rectW, y, rectW, rectH);
}


static int getThreeVerticalValue(IntegralImage *image, int x, int y, int w, int h)
{
	int rectW = w;
	int rectH = h/3;
	return IntegralImageGetSum(image, x, y, rectW, rectH)
		- IntegralImageGetSum(image, x, y + rectH, rectW, rectH)
		+ IntegralImageGetSum(image, x, y + 2*rectH, rectW, rectH);
}

static int getThreeHorizontalValue(IntegralImage *image, int x, int y, int w, int h)
{
	int rectW = w / 3;
	int rectH = h;
	return IntegralImageGetSum(image, x, y, rectW, rectH)
		- IntegralImageGetSum(image, x + rectW, y, rectW, rectH)
		+ IntegralImageGetSum(image, x + 2 * rectW, y, rectW, rectH);
}


int FeatureGetValue(Feature *feature, IntegralImage *image,  int x, int y, double scale)
{
	int w = feature->w * scale;
	int h = feature->h * scale;
	x += feature->x * scale;
	y += feature->y * scale;

	
	if(feature->type == FEATURE_FOUR)
		return getFourRectValue(image, x, y, w, h);
	else if(feature->type == FEATURE_TWO_VERTICAL)
		return getTwoVerticalValue(image, x, y, w, h);
	else if(feature->type == FEATURE_TWO_HORIZONTAL)
		return getTwoHorizontalValue(image, x, y, w, h);
	else if(feature->type == FEATURE_THREE_VERTICAL)
		return getThreeVerticalValue(image, x, y, w, h);
	else if(feature->type == FEATURE_THREE_HORIZONTAL)
		return getThreeHorizontalValue(image, x, y, w, h);

	return 0;
}
