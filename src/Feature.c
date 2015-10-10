#include "Feature.h"
#include <stdlib.h>

Feature *featureNew(int type, int size)
{
	Feature *feature = malloc(sizeof(Feature));
	feature->type = type;
	feature->size = size;
	return feature;
}

void featureFree(Feature *feature)
{
	free(feature);
}

static int getFourRectValue(Feature *feature, IntegralImage *image, int x, int y)
{
	int sumTL = IntegralImageGetSum(image, x, y, feature->size, feature->size);
	int sumTR = IntegralImageGetSum(image, x + feature->size, y, feature->size, feature->size);
	int sumBL = IntegralImageGetSum(image, x, y + feature->size, feature->size, feature->size);
	int sumBR = IntegralImageGetSum(image, x + feature->size, y + feature->size, feature->size, feature->size);
	return sumTL - sumTR - sumBL + sumBR;
}

static int getTwoVerticalValue(Feature *feature, IntegralImage *image, int x, int y)
{
	return IntegralImageGetSum(image, x, y + feature->size, feature->size, feature->size)
		- IntegralImageGetSum(image, x, y, feature->size, feature->size);
}

static int getTwoHorizontalValue(Feature *feature, IntegralImage *image, int x, int y)
{
	return IntegralImageGetSum(image, x, y, feature->size, feature->size)
		- IntegralImageGetSum(image, x, y+feature->size, feature->size, feature->size);
}


static int getThreeVerticalValue(Feature *feature, IntegralImage *image, int x, int y)
{
	return IntegralImageGetSum(image, x, y, feature->size, feature->size)
		- IntegralImageGetSum(image, x + feature->size, y, feature->size, feature->size)
		+ IntegralImageGetSum(image, x + 2 * feature->size, y, feature->size, feature->size);
}

static int getThreeHorizontalValue(Feature *feature, IntegralImage *image, int x, int y)
{
	return IntegralImageGetSum(image, x, y, feature->size, feature->size)
		- IntegralImageGetSum(image, x, y+feature->size, feature->size, feature->size)
		+ IntegralImageGetSum(image, x, y + 2 * feature->size, feature->size, feature->size);
}


int getFeatureValue(Feature *feature, IntegralImage *image,  int x, int y)
{
	if(feature->type == FEATURE_FOUR)
		return getFourRectValue(feature, image, x, y);
	else if(feature->type == FEATURE_TWO_VERTICAL)
		return getTwoVerticalValue(feature, image, x, y);
	else if(feature->type == FEATURE_TWO_HORIZONTAL)
		return getTwoHorizontalValue(feature, image, x, y);
	else if(feature->type == FEATURE_THREE_VERTICAL)
		return getThreeVerticalValue(feature, image, x, y);
	else if(feature->type == FEATURE_THREE_HORIZONTAL)
		return getThreeHorizontalValue(feature, image, x, y);

	return 0;
}
