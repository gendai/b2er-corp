#include "TestWeakClassifier.h"
#include <stdlib.h>


TestWeakClassifier *TestWeakClassifierNew(FeatureValues *fv, int threshold, int parity, double errors)
{
	TestWeakClassifier *twc = malloc(sizeof(TestWeakClassifier));
	twc->fv = fv;
	twc->threshold = threshold;
	twc->parity = parity;
	twc->errors = errors;
	return twc;
}

void TestWeakClassifierFree(TestWeakClassifier *twc)
{
	free(twc);
}

int TestWeakClassifierCheck(TestWeakClassifier *twc, int val)
{
	return twc->parity * val < twc->parity * twc->threshold;
}

WeakClassifier *TestWeakClassifierGet(TestWeakClassifier *twc, double alpha)
{
	return WeakClassifierNew(alpha, twc->threshold, twc->parity, twc->fv->feature);
}

TestWeakClassifier * TestWeakClassifierTrain(TrainingImage **images, double validWeight, FeatureValues *fv)
{
	double positiveError = validWeight;

	TestWeakClassifier *best = TestWeakClassifierNew(fv, fv->values[0].value, 1, positiveError);
	for(size_t i = 0; i < fv->size; ++i)
	{
		if(images[fv->values[i].i]->valid)
		{
			positiveError -= images[fv->values[i].i]->weight;
			if(positiveError < best->errors)
			{
				TestWeakClassifierFree(best);
				best = TestWeakClassifierNew(fv, fv->values[i].value + 1.0, 1, positiveError);
			}
		}
		else
		{
			positiveError += images[fv->values[i].i]->weight;
			double negativeError = 1 - positiveError;
			if(negativeError < best->errors)
			{
				TestWeakClassifierFree(best);
				best = TestWeakClassifierNew(fv, fv->values[i].value - 1.0, -1, negativeError);	
			}
		}
	}

	return best;
}
