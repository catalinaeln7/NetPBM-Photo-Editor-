#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_
#include <stdio.h>

#define NMAX 1000

typedef struct pixel {
	int red, green, blue;
} pixel;

typedef struct image {
	char magic_word[3];
	int width, height, maxval;
	int **mat;
	pixel **px_mat;
	int loaded, loaded_px;
	int xmin, xmax, ymin, ymax;	//coordonate pentru selectii
} image;

image free_mem(image *img);

image free_mem_px(image *img);

int choose_command(char op_line[]);

image load(FILE *in, image *img);

image select_all(image *img);

image select_area(image *img, char aux[NMAX]);

image rotate(image *img, char aux[NMAX]);

image crop(image *img);

image grayscale(image *img);

image sepia(image *img);

void save_img(image *img, char aux[NMAX]);

#endif // _FUNCTIONS_H_