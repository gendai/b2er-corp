#ifndef DEF_STRONGCLASSIFIER
#define DEF_STRONGCLASSIFIER

#include "WeakClassifier.h"
#include "IntegralImage.h"

struct StrongClassifier
{
	int size;
	WeakClassifier **wcs;
	double alphaTotal;
};
typedef struct StrongClassifier StrongClassifier;

int getDeviation(IntegralImage *image, IntegralImage *squaredImage, double scale ,int x, int y, int w, int h);

StrongClassifier *StrongClassifierNew(int size, WeakClassifier **wcs);
StrongClassifier *StrongClassifierNewFromFile(char *path);
void StrongClassifierFree(StrongClassifier *sc);

int StrongClassifierSave(StrongClassifier *sc, char *path);
int StrongClassifierCheck(StrongClassifier *cs, IntegralImage *image, int x, int y, double scale, int deviation);
void StrongClassifierPrint(StrongClassifier *sc);

StrongClassifier *StrongClassifierTrain(char *trainDir);

#endif
