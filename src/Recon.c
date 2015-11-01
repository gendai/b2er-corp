#include "GreyImage.h"
#include "Recon.h"
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_matrix_double.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_vector_double.h>
#include <math.h>

void InitializeRecon(char *path)
{
	int len = SetVector(path);
	CalColumnMatrix(len);
	Mean();
	CalVariance();
	DoubleToColor();
	Covariance();
	SvdMatrix();
	EigenValues();
	EigenFaces();
}



int SetVector(char *path)
{	
	 //DIR *dp;
	 //struct dirent *ep;
    	 //int i = 0;	 
	 //dp = opendir (path);
	 /*if (dp != NULL)
	 {
	     while (ep = readdir (dp))
	     {
		GtkWidget *image = gtk_image_new_from_file(ep->d_name);
		*(ListImg + i) = *GreyImageNewFromImage((GtkImage *)image);
		i++;
	     }
	 }*/
	GtkWidget *image1 = gtk_image_new_from_file("../el11.jpg");
	GtkWidget *image2 = gtk_image_new_from_file("../el12.jpg");
	GtkWidget *image3 = gtk_image_new_from_file("../el13.jpg");
	ListImg[0] = *GreyImageNewFromImage((GtkImage *)image1);	
	ListImg[1] = *GreyImageNewFromImage((GtkImage *)image2);
	ListImg[2] = *GreyImageNewFromImage((GtkImage *)image3);
	return 3;			         
}

void CalColumnMatrix(int len)
{
	columnmatrixw = len;
	columnmatrixh = ListImg->width * ListImg->height;		
	ColumnMatrix = malloc(columnmatrixw * columnmatrixh * sizeof(double));
	for(int i = 0; i < columnmatrixw; ++i)
	{
		GreyImage curr = ListImg[i];
		for(int j = 0; j < columnmatrixh; ++j)
		{
			*(ColumnMatrix + (j * columnmatrixw + i)) = (double)(curr.pixels[j]);
		}
	}
	
}

void Mean()
{
	MeanMatrix = malloc(columnmatrixh * sizeof(double));
	for(int i = 0; i < columnmatrixh; ++i)
	{
		double sig = 0;
		for(int j = 0; j < columnmatrixw; ++j)
		{
			double curr = *(ColumnMatrix + (i * columnmatrixw + j));
			sig += curr;
		}
		double avg = 0;
		avg = (double)((double)sig / (double)columnmatrixw);
		*(MeanMatrix + i) = avg;	
	}
	
}

void CalVariance()
{
	for(int j = 0; j < columnmatrixw; ++j)
	{
		for(int i = 0; i < columnmatrixh; ++i)
		{
			double sub = *(ColumnMatrix +(i * columnmatrixw + j)) - *(MeanMatrix + i);
			*(ColumnMatrix + (i * columnmatrixw + j)) = sub;
		}
	}
	
}

void DoubleToColor()
{
	DoubleColumn = (double **)malloc(columnmatrixh * sizeof(double*));
	double *temptab = (double *)malloc(sizeof(double)*columnmatrixw*columnmatrixh);
	for(int k = 0; k < columnmatrixh; ++k)
	{
		DoubleColumn[k] = &temptab[k*columnmatrixw];
	}
	for(int i = 0; i < columnmatrixw; ++i)
	{
		for(int j = 0; j < columnmatrixh; ++j)
		{
			DoubleColumn[j][i] = (*(ColumnMatrix + (j * columnmatrixw + i)));
		}
	}
	
}

void Covariance()
{
	ma = gsl_matrix_calloc(columnmatrixh, columnmatrixw);
	for(int i = 0; i < columnmatrixw; ++i)
	{
		for(int j = 0; j < columnmatrixh; ++j)
		{
			gsl_matrix_set(ma, j, i, DoubleColumn[j][i]);
		}
	}
	cov = gsl_matrix_calloc(columnmatrixw,columnmatrixw);
	trans = gsl_matrix_calloc(columnmatrixw, columnmatrixh);
	gsl_matrix_transpose_memcpy(trans, ma);
	gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, trans, ma, 0.0, cov);
}

void SvdMatrix()
{
	_v = gsl_matrix_calloc(columnmatrixw, columnmatrixw);
	sing = gsl_vector_calloc(columnmatrixw);
	gsl_vector *work = gsl_vector_calloc(columnmatrixw);
	gsl_linalg_SV_decomp(cov, _v, sing, work);
	gsl_vector_free(work);
}

void Print(gsl_matrix *ma, gsl_vector *col)
{
	gsl_vector *tmp = gsl_vector_calloc(columnmatrixh);
	printf("Matrix\n");
	for(int i = 0; i < columnmatrixw; ++i)
	{
		gsl_matrix_get_col(tmp, ma, i);
		for(int j = 0; j < 10; ++j)
		{
			printf("%g ", gsl_vector_get(tmp, j));
		}
		printf("\n");
	}
	printf("Vector\n");
	for(int i = 0; i < 3; ++i)
	{
		printf("%g ", gsl_vector_get(col, i));
	}
	printf("\n");
}

void EigenValues()
{
	int i,j = 0;
	gsl_vector *eixp = gsl_vector_calloc(columnmatrixw);
	eigv = gsl_vector_calloc(columnmatrixw);
	eigu = gsl_matrix_calloc(columnmatrixh,columnmatrixw);
	gsl_vector *coltmp = gsl_vector_calloc(columnmatrixw);
	gsl_vector *coltmpm = gsl_vector_calloc(columnmatrixh);
	double sumsq = 0.0;
	double mean = 0.0;
	for(i = 0; i < columnmatrixw; ++i)
	{
		gsl_vector_set(eixp, i, pow(gsl_vector_get(sing, i),(double)(-0.5)));		
	}
	for(i = 0; i < columnmatrixw; ++i)
	{
		gsl_matrix_get_col(coltmp, _v, i);
		gsl_blas_dgemv(CblasNoTrans, (double)1.0, ma, coltmp, (double)0.0, coltmpm);
		Print(ma, coltmp);
		gsl_blas_dscal(gsl_vector_get(eixp, i),coltmpm); 
		gsl_matrix_set_col(eigu, i, coltmpm);
	}
	for(i = 0; i < columnmatrixw; ++i)
	{
		gsl_matrix_get_col(coltmpm, eigu, i);
		for(j = 0; j < columnmatrixh; ++j)
		{
			sumsq += pow(gsl_vector_get(coltmpm, j), (double)2.0);

		}
		mean = sqrt(sumsq);
		gsl_blas_dscal((double)(1.0/mean), coltmpm);
		gsl_matrix_set_col(eigu, i, coltmpm);
	}
	//return Check(eigu, eixp, _v);
	gsl_vector *res = gsl_vector_calloc(columnmatrixw);
	eigw = gsl_matrix_calloc(columnmatrixw, columnmatrixw);
	gsl_vector *norm = gsl_vector_calloc(columnmatrixh);
	double result = 0.0;
	for(i = 0; i < columnmatrixw; ++i)
	{
		for(int k = 0; k < columnmatrixh; ++k)
		{
			gsl_vector_set(norm, k, *(ColumnMatrix + (k * columnmatrixw + i)) - *(MeanMatrix + k));
		}
		for(j = 0; j < columnmatrixw; ++j)
		{			
			gsl_matrix_get_col(coltmpm, eigu, j);
		       	gsl_blas_ddot(norm, coltmpm, &result);
			gsl_vector_set(res, j, result);
		}
		gsl_matrix_set_col(eigw, i, res);				
	}
	gsl_matrix_get_col(coltmp, eigw, 1);
	gsl_vector_free(coltmp);
	gsl_vector_free(coltmpm);
	gsl_vector_free(eixp);
	gsl_vector_free(res);
	gsl_vector_free(norm);
}


GtkImage *Check(gsl_matrix *u, gsl_vector *ex, gsl_matrix *v)
{
	gsl_matrix *utmp = gsl_matrix_calloc(columnmatrixh, columnmatrixw);
	gsl_vector *coltmp = gsl_vector_calloc(columnmatrixh);
	gsl_matrix *vtrans = gsl_matrix_calloc(columnmatrixw, columnmatrixw);
	gsl_matrix *res = gsl_matrix_calloc(columnmatrixh, columnmatrixw);
	for(int i = 0; i < columnmatrixw; ++i)
	{
		gsl_matrix_get_col(coltmp, u, i);
		gsl_blas_dscal((1.0/gsl_vector_get(ex, i)), coltmp);
		gsl_matrix_set_col(utmp, i, coltmp);
	}
	gsl_matrix_transpose_memcpy(vtrans, v);
	gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, utmp, vtrans, 0.0, res);
	gsl_matrix_get_col(coltmp, res, 1);
	for(int i = 0; i < columnmatrixh; ++i)
	{
		gsl_vector_set(coltmp, i, (gsl_vector_get(coltmp, i) + *(MeanMatrix + i)));
	}
	return VectToGrey(coltmp);
}

GtkImage *EigenFaces()
{
	gsl_vector *coltmp = gsl_vector_calloc(columnmatrixh);
	eigfaces = malloc(columnmatrixw * sizeof(GreyImage *));
	for(int i = 0; i < columnmatrixw; ++i)
	{
		double min,max,curr = 0.0;
		gsl_matrix_get_col(coltmp, eigu, i);
		for(int j = 0; j < columnmatrixh; ++j)
		{
			curr = gsl_vector_get(coltmp, j);
			if(max < curr)
			{
				max = curr;
			}
			if(min > curr)
			{
				min = curr;
			}
		}
		GreyImage *ftmp = GreyImageNew(93, 122);
		for(int k = 0; k < columnmatrixh; ++k)
		{
			ftmp->pixels[k] =  (255.0 * ((gsl_vector_get(coltmp, k) - min) / (max - min)));
		}
		*(eigfaces + i) = *ftmp;
	}
	return GreyImageToImage(eigfaces + 2);
	
}

GtkImage *VectToGrey(gsl_vector *col)
{
	GdkPixbuf *pixbuf = gdk_pixbuf_new(
			GDK_COLORSPACE_RGB,
			0,
			8,
			93,
			122
			);
	int rows = gdk_pixbuf_get_rowstride(pixbuf);
	int nchan = gdk_pixbuf_get_n_channels(pixbuf);
	guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);
	int pos = 0;
	for(int i = 0; i < 122; ++i)
	{
		for(int j = 0; j < 93; ++j)
		{
			guchar *pixel = pixels + i * rows + j * nchan;
			pixel[0] = pixel[1] = pixel[2] = gsl_vector_get(col, pos);
			pos++;
		}
	}
	return (GtkImage *) gtk_image_new_from_pixbuf(pixbuf);
}

GtkImage *GetFace(GreyImage *img)
{
	gsl_vector *res = gsl_vector_calloc(columnmatrixw);
	gsl_vector *norm = gsl_vector_calloc(columnmatrixh);
	double result = 0.0;
	gsl_vector *coltmpm = gsl_vector_calloc(columnmatrixh);
	gsl_vector *coltmp = gsl_vector_calloc(columnmatrixw);
	gsl_matrix *pic = gsl_matrix_calloc(columnmatrixh, columnmatrixw);
	gsl_vector *wei = gsl_vector_calloc(columnmatrixw);
	gsl_vector *imgi = gsl_vector_calloc(columnmatrixh);
	int lenwei,lenpic = 0;
	gsl_vector *rep = gsl_vector_calloc(columnmatrixh);

	for(int k = 0; k < columnmatrixh; ++k)
	{
		gsl_vector_set(norm, k, (double)(img->pixels[k]) - *(MeanMatrix + k));
	}
	for(int i = 0; i < columnmatrixw; ++i)
	{
		gsl_matrix_get_col(coltmpm, eigu, i);
		gsl_blas_ddot(norm, coltmpm, &result);
		gsl_vector_set(res, i, result);
	}
	for(int j = 0; j < columnmatrixw; ++j)
	{
		gsl_matrix_get_col(coltmp, eigw, j);
		double dist = CalDistance(res, coltmp);
		printf("Dist : %g\n", dist);
		if(dist <= 0.05)
		{
			printf("PASS: %d\n",j);			
			for(int l = 0; l < columnmatrixh; ++l)
			{
				gsl_vector_set(imgi, l, *(ColumnMatrix + (l * columnmatrixw + j)) + *(MeanMatrix + l));
			}
			
			if(lenwei == 0)
			{
				gsl_vector_set(wei, lenwei, dist);
				lenwei++;
				gsl_matrix_set_col(pic, lenpic, imgi);
			}
			else
			{
				for(int q = 0; q < lenwei; ++q)
				{
					if(dist < gsl_vector_get(wei, q))
					{
						gsl_vector_set(wei, j, dist);
						gsl_matrix_set_col(pic, j, imgi);
						q = lenwei;
						break;
					}
				}
			}
		}
	}
	gsl_matrix_get_col(coltmpm, pic, 0);
	return VectToGrey(coltmpm);	
}

double CalDistance(gsl_vector *wimg, gsl_vector *eigw)
{
	double res = 0.0;
	for(int i = 0; i < columnmatrixw; ++i)
	{
		res += pow((gsl_vector_get(wimg, i) - gsl_vector_get(eigw, i)) / (double)columnmatrixh , 2.0);
	}
	res = sqrt(res);
	return (res / sqrt((double)columnmatrixw));
}


