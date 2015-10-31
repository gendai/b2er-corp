#ifndef _RECON_H_
#define _RECON_H_
#include <gtk/gtk.h>
#include "GreyImage.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_matrix_double.h>

GreyImage ListImg[6];
double *ColumnMatrix;
double *MeanMatrix;
double *covar;
int columnmatrixw;
int columnmatrixh;
double **DoubleColumn;
gsl_matrix *cov;
gsl_matrix *svd;
gsl_matrix *_v;
gsl_matrix *eigu;
gsl_vector *eigv;
gsl_vector *sing;
gsl_matrix *ma;
gsl_matrix *eigw;
gsl_matrix *trans;
GreyImage *eigfaces;

void InitializeRecon(char *path);
int SetVector(char *path);
GtkImage *VectToGrey(gsl_vector *img);
void CalColumnMatrix(int len);
void Mean();
void CalVariance();
void DoubleToColor();
void Covariance();
void SvdMatrix();
GtkImage *Check(gsl_matrix *u, gsl_vector *ex, gsl_matrix *v);
void EigenValues();
GtkImage *EigenFaces();
GtkImage *GetFace(GreyImage *img);
double CalDistance(gsl_vector *wimg, gsl_vector *eigw);


#endif
