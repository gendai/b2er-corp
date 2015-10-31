#ifndef DEF_TESTWEAKCLASSIFIER
#define DEF_TESTWEAKCLASSIFIER

#include "FeatureValues.h"
#include "WeakClassifier.h"
#include "TrainingImage.h"

struct TestWeakClassifier
{
	FeatureValues *fv;
	int threshold;
	int parity;
	double errors;
};
typedef struct TestWeakClassifier TestWeakClassifier;

TestWeakClassifier *TestWeakClassifierNew(FeatureValues *fv, int threshold, int parity, double errors);
void TestWeakClassifierFree(TestWeakClassifier *twc);
int TestWeakClassifierCheck(TestWeakClassifier *twc, int val);
WeakClassifier *TestWeakClassifierGet(TestWeakClassifier *twc, double alpha);
TestWeakClassifier *TestWeakClassifierTrain(TrainingImage **images, double validWeight, FeatureValues *fv);


#endif
