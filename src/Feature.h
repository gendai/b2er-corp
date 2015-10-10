#ifndef DEF_FEATURE
#define DEF_FEATURE

#define FEATURE_FOUR 1
#define FEATURE_TWO_VERTICAL 2
#define FEATURE_TWO_HORIZONTAL 3
#define FEATURE_THREE_VERTICAL 4
#define FEATURE_THREE_HORIZONTAL 5

#include "IntegralImage.h"

struct Feature
{
	int type;
	int size;
};
typedef struct Feature Feature;

Feature *featureNew(int type, int size);
void featureFree(Feature *feature);

int getFeatureValue(Feature *feature, IntegralImage *image,  int x, int y);


#endif
