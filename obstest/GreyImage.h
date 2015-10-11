#ifndef DEF_GREYIMAGE
#define DEF_GREYIMAGE

#include <gtk/gtk.h>

struct GreyImage
{
	int width;
	int height;
	int *pixels;
};
typedef struct GreyImage GreyImage;


GreyImage *GreyImageNew(int width, int height);
GreyImage *GreyImageNewFromImage(GtkImage *image);

void GreyImageFree(GreyImage *image);
GtkImage *GreyImageToImage(GreyImage *image);
GreyImage *GreyImageResize(GreyImage *image, int outW, int outH);
GreyImage *GreyImageScale(GreyImage *image, float scale);


#endif
