#include "StrongClassifier.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <dirent.h>
#include <gtk/gtk.h>
#include <math.h>
#include <assert.h>
#include "Vector.h"
#include "dimensions.h"
#include "TrainingImage.h"
#include "FeatureValues.h"
#include "TestWeakClassifier.h"

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

StrongClassifier *StrongClassifierNewFromFile(char *path)
{
	
	FILE *file = fopen(path, "rb");
	if(!file)
		return 0;

	int size;
	fread(&size, sizeof(size), 1, file);
	WeakClassifier **wcs = malloc(sizeof(WeakClassifier *) * size);
	
	for(int i = 0; i < size; ++i)
	{
		WeakClassifier *wc = malloc(sizeof(WeakClassifier));
		wcs[i] = wc;

		fread(&(wc->alpha), sizeof(wc->alpha), 1, file);
		fread(&(wc->threshold), sizeof(wc->threshold), 1, file);
		fread(&(wc->parity), sizeof(wc->parity), 1, file);

		wc->feature = malloc(sizeof(Feature));
		fread(wc->feature, sizeof(Feature), 1, file);
	}

	fclose(file);
	return StrongClassifierNew(size, wcs);
}

void StrongClassifierFree(StrongClassifier *sc)
{
	for(int i = 0; i < sc->size; ++i)
		WeakClassifierFree(sc->wcs[i]);
	free(sc);
}

int StrongClassifierSave(StrongClassifier *sc, char *path)
{
	FILE *file = fopen(path, "wb");
	if(!file)
		return 0;

	fwrite(&(sc->size), sizeof(sc->size), 1, file);

	for(int i = 0; i < sc->size; ++i)
	{
		WeakClassifier *wc = sc->wcs[i];
		fwrite(&(wc->alpha), sizeof(wc->alpha), 1, file);
		fwrite(&(wc->threshold), sizeof(wc->threshold), 1, file);
		fwrite(&(wc->parity), sizeof(wc->parity), 1, file);
		fwrite(wc->feature, sizeof(Feature), 1, file);
	}

	fclose(file);
	return 1;
}

void StrongClassifierPrint(StrongClassifier *sc)
{
	for(int i = 0; i < sc->size; ++i)
	{
		WeakClassifier *wc = sc->wcs[i];
		Feature *feature = wc->feature;
		printf("%d: A=%g, T=%d, P=%d, %d:(%d, %d, %d, %d)\n", i+1, wc->alpha, wc->threshold, wc->parity, feature->type, feature->x, feature->y, feature->w, feature->h);
	}
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

//  Training

static char **readDir(char *dirPath, size_t *size)
{
	Vector *v = VectorNew();
	DIR *d;
	struct dirent *dir;
	d = opendir(dirPath);
	if(!d)
		return 0;

	while ((dir = readdir(d)) != 0)
	{
		if(dir->d_name[0] != '.')
		{
			char *str = malloc(strlen(dir->d_name)+1);
			strcpy(str, dir->d_name);
			VectorInsertBack(v, str);
		}
	}

	char **data = (char **) VectorData(v);
	*size = VectorSize(v);
	free(v);
	closedir(d);
	return data;
}

static GreyImage **LoadImages(char *dir, size_t *size)
{
	size_t dirLen = strlen(dir);
	char **files = readDir(dir, size);
	GreyImage **images = malloc(*size * sizeof(GreyImage *));

	for(size_t i = 0; i < *size; ++i)
	{
		char *file = malloc(dirLen + strlen(files[i])+1);
		strcpy(file, dir);
		strcat(file, files[i]);
		GtkImage *image = (GtkImage *) gtk_image_new_from_file(file);

		assert(gtk_image_get_pixbuf(image));
		
		GreyImage *grey = GreyImageNewFromImage(image);
		images[i] = GreyImageResize(grey, WIN_WIDTH, WIN_HEIGHT);
		GreyImageFree(grey);
	}
	

	printf("%s => %zu\n", dir, *size);
	
	return images;
}


static void LoadTraining(char *trainDir, TrainingImage ***train, size_t *trainSize, FeatureValues ***fvals, size_t *fSize)
{
	size_t size = strlen(trainDir);
	char *dirValid = malloc((size + 10) * sizeof(char));
	char *dirUnvalid = malloc((size+10) * sizeof(char));
	strcpy(dirValid, trainDir);
	strcpy(dirUnvalid, trainDir);
	strcat(dirValid, "good/");
	strcat(dirUnvalid, "bad/");
	

	size_t validSize;
	size_t unvalidSize;
	GreyImage **validImages = LoadImages(dirValid, &validSize);
	GreyImage **unvalidImages = LoadImages(dirUnvalid, &unvalidSize);

	double validWeight = 1 / (2 * (double) validSize);
	double unvalidWeight = 1 / (2 * (double) unvalidSize);

	size_t trainingSize = validSize + unvalidSize;
	TrainingImage **ti = malloc(trainingSize * sizeof(TrainingImage *));

	for(size_t i = 0; i < validSize; ++i)
		ti[i] = TrainingImageNew(validImages[i], 1, validWeight);
	for(size_t i = 0; i < unvalidSize; ++i)
		ti[validSize+i] = TrainingImageNew(unvalidImages[i], 0, unvalidWeight);

	*train = ti;
	*trainSize = trainingSize;


	size_t featuresSize;
	Feature **features = FeatureListAll(&featuresSize);

	FeatureValues **featuresValues = malloc(sizeof(FeatureValues *) * featuresSize);
	for(size_t i = 0; i < featuresSize; ++i)
		featuresValues[i] = FeatureValuesNew(features[i], ti, trainingSize);

	*fvals = featuresValues;
	*fSize = featuresSize;
}

StrongClassifier *StrongClassifierTrain(char *trainDir)
{
	TrainingImage **images;
	size_t imagesSize;
	FeatureValues **featuresValues;
	size_t featuresSize;
	LoadTraining(trainDir, &images, &imagesSize, &featuresValues, &featuresSize);


	WeakClassifier **classfifiers = malloc(sizeof(WeakClassifier *) * CLASSIFIERS_SIZE);
	size_t iClas = 1;

	while(iClas <= CLASSIFIERS_SIZE)
	{

		printf("Pass %zu/%d\n", iClas, CLASSIFIERS_SIZE);

		double weightSum = 0;
		for(size_t i = 0; i < imagesSize; ++i)
			weightSum += images[i]->weight;
		double validWeight = 0;
		for(size_t i = 0; i < imagesSize; ++i)
		{
			images[i]->weight /= weightSum;
			if(images[i]->valid)
				validWeight += images[i]->weight;
		}

		TestWeakClassifier *best = TestWeakClassifierNew(featuresValues[0], 0, 1, DBL_MAX);
		for(size_t i = 0; i < featuresSize; ++i)
		{
			TestWeakClassifier *newClassifier = TestWeakClassifierTrain(images, validWeight, featuresValues[i]);
			if(best->errors > newClassifier->errors)
			{
				TestWeakClassifierFree(best);
				best = newClassifier;
			}
		}

		double beta = best->errors / (1-best->errors);
		if(beta < 0.00000001)
			beta = 0.00000001;

		for(size_t i = 0; i < best->fv->size; ++i)
		{
			FeatureValue fv = best->fv->values[i];
			if(TestWeakClassifierCheck(best, fv.value) == images[fv.i]->valid)
				images[fv.i]->weight *= beta;
		}

		double alpha = log(1 / beta);
		classfifiers[iClas-1] = TestWeakClassifierGet(best, alpha);
		++iClas;
	}
	
	return StrongClassifierNew(CLASSIFIERS_SIZE, classfifiers);
}
