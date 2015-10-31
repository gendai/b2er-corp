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
	int x;
	int y;
	int w;
	int h;
};
typedef struct Feature Feature;


Feature *FeatureNew(int type, int x, int y, int w, int h);
void FeatureFree(Feature *feature);

int FeatureGetValue(Feature *feature, IntegralImage *image,  int x, int y, double scale);

Feature **FeatureListAll(size_t *size);


#endif
