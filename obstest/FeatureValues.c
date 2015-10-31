#include "FeatureValues.h"
#include <stdlib.h>
#include <string.h>

static void featuresSort(FeatureValue *a, size_t n)
{
	if(n > 1)
	{
		
		size_t q = n/2;
		featuresSort(a, q);
		featuresSort(a+q, n-q);

		size_t i = 0, j = q;
		FeatureValue *temp = malloc(sizeof(FeatureValue) * n);
		memcpy(temp, a, sizeof(FeatureValue)*n);
		
		for(size_t k = 0; k < n; ++k)
			a[k] =  j == n ||  (i < q && temp[i].value < temp[j].value) ? temp[i++] : temp[j++];
		free(temp);
	}
}

FeatureValues *FeatureValuesNew(Feature *feature, TrainingImage **images, size_t size)
{
	FeatureValues *fv = malloc(sizeof(FeatureValues));
	fv->size = size;
	fv->feature = feature;
	fv->values = malloc(sizeof(FeatureValue) * size);

	for(size_t i = 0; i < size; ++i)
	{
		int val = FeatureGetValue(feature, images[i]->image, 0, 0, 1);
		fv->values[i].value = (val * 40) / images[i]->deviation;
		fv->values[i].i = i;
	}

	featuresSort(fv->values, size);
	return fv;
}

void FeatureValuesFree(FeatureValues *fv)
{
	free(fv->values);
	free(fv);
}
