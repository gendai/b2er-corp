#ifndef DEF_WEAKCLASSIFIER
#define DEF_WEAKCLASSIFIER

#include "Feature.h"

struct WeakClassifier
{
	double alpha;
	int threshold;
	int parity;
	Feature *feature;
};
typedef struct WeakClassifier WeakClassifier;

WeakClassifier *WeakClassifierNew(double alpha, int threshold, int parity, Feature *feature);
void WeakClassifierFree(WeakClassifier *wc);

int WeakClassifierCheck(WeakClassifier *wc, IntegralImage *image, int x, int y, double scale, int deviation);


#endif
