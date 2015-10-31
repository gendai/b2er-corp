#ifndef DEF_FEATUREVALUES
#define DEF_FEATUREVALUES

#include <stddef.h>
#include "Feature.h"
#include "TrainingImage.h"

struct FeatureValue
{
	size_t i;
	int value;
};
typedef struct FeatureValue FeatureValue;

struct FeatureValues
{
	size_t size;
	Feature *feature;
	FeatureValue *values;
};
typedef struct FeatureValues FeatureValues;

FeatureValues *FeatureValuesNew(Feature *feature, TrainingImage **images, size_t size);
void FeatureValuesFree(FeatureValues *fv);

#endif
