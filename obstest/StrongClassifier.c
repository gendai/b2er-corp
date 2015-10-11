#include "StrongClassifier.h"
#include <stdlib.h>
#include <string.h>
#include "string.h"
#include "file.h"

StrongClassifier *StrongClassifierNew(int size, WeakClassifier **wcs)
{
	StrongClassifier *sc = malloc(sizeof(StrongClassifier));
	sc->size = size;
	sc->wcs = wcs;
	sc->alphaTotal = 0;
	for(int i = 0; i < size; ++i)
		sc->alphaTotal += wcs[i]->alpha;
	return sc;
}

static int parseFeatureType(char *type)
{
	if(!strcmp(type, "ViolaJones.TwoVerticalRectanglesFeature"))
		return FEATURE_TWO_VERTICAL;
	else if(!strcmp(type, "ViolaJones.TwoHorizontalRectanglesFeature"))
		return FEATURE_TWO_HORIZONTAL;
	else if(!strcmp(type, "ViolaJones.FourRectanglesFeature"))
		return FEATURE_FOUR;
	else if(!strcmp(type, "ViolaJones.ThreeHorizontalRectanglesFeature"))
		return FEATURE_THREE_HORIZONTAL;
	else if(!strcmp(type, "ViolaJones.ThreeVerticalRectanglesFeature"))
		return FEATURE_THREE_VERTICAL;
	return -1;
}

StrongClassifier *StrongClassifierNewFromFile(char *path)
{
	char *str = fileReadAllText(path);
	if(!str)
		return 0;

	int nbLines;
	char **lines = stringSplit(str, "\n", &nbLines);
	nbLines--;

	WeakClassifier **wcs = malloc(sizeof(WeakClassifier *) * nbLines);
	
	for(int i = 0; i < nbLines; ++i)
	{
		
		char *weakStr = lines[i];
		int nbVals;
		char **vals = stringSplit(weakStr, ";", &nbVals);

		double alpha = atof(vals[0]);
		int threshold = atoi(vals[1]);
		int parity = atoi(vals[2]);
		
		int featureType = parseFeatureType(vals[3]);
		int featureX = atoi(vals[4]);
		int featureY = atoi(vals[5]);
		int featureW = atoi(vals[6]);
		int featureH = atoi(vals[7]);
		Feature *feature = FeatureNew(featureType, featureX, featureY, featureW, featureH);

		wcs[i] = WeakClassifierNew(alpha, threshold, parity, feature);
		
		for(int j = 0; j < nbVals; ++j)
			free(vals[j]);
		free(vals);
		free(weakStr);
	}

	return StrongClassifierNew(nbLines, wcs);
}

void StrongClassifierFree(StrongClassifier *sc)
{
	for(int i = 0; i < sc->size; ++i)
		WeakClassifierFree(sc->wcs[i]);
	free(sc);
}

int StrongClassifierCheck(StrongClassifier *cs, IntegralImage *image, int x, int y, double scale, int deviation)
{
	double val = 0;
	for(int i = 0; i < cs->size; ++i)
	{
		if(WeakClassifierCheck(cs->wcs[i], image, x, y, scale, deviation))
			val += cs->wcs[i]->alpha;
	}
	return val >= cs->alphaTotal / 2;
}
