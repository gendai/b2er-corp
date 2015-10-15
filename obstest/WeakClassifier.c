#include "WeakClassifier.h"
#include <stdlib.h>

WeakClassifier *WeakClassifierNew(double alpha, int threshold, int parity, Feature *feature)
{
	WeakClassifier *wc = malloc(sizeof(WeakClassifier));
	wc->alpha = alpha;
	wc->threshold = threshold;
	wc->parity = parity;
	wc->feature = feature;
	return wc;
}

void WeakClassifierFree(WeakClassifier *wc)
{
	FeatureFree(wc->feature);
	free(wc);
}

int WeakClassifierCheck(WeakClassifier *wc, IntegralImage *image, int x, int y, double scale, int deviation)
{
	int featureValue = FeatureGetValue(wc->feature, image, x, y, scale);
	int sizedValue = featureValue / (scale*scale);
	int normalizedValue = (sizedValue * 40) / deviation;
	return wc->parity * normalizedValue < wc->parity * wc->threshold;
}
