#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "functions.h"

#define NMAX 1000

// ---ELIBERARE MEMORIE---

// Eliberare memorie pentru P2/P5
image free_mem(image *img)
{
	if (img->loaded == 1) {
		for (int i = 0; i < img->height; ++i)
			free(img->mat[i]);
		free(img->mat);
	}

	return *img;
}

// Eliberare memorie pentru P3/P6 
image free_mem_px(image *img)
{
	if (img->loaded_px == 1) {
		for (int i = 0; i < img->height; ++i)
			free(img->px_mat[i]);
		free(img->px_mat);
	}

	return *img;
}

// ---FUNCTII CITIRE COMENZI---

// Verifica daca un string poate fi convertit in integer
int valid_int(char *str)
{
   if (*str == '-')
      ++str;

   while (*str)
   {
      if (!isdigit(*str))
         return 0;	// Nu poate fi convertit in integer
      else
         ++str;
   }

   return 1;	// Poate fi convertit in integer
}

// Este verificata corectitudinea comenzilor
int choose_command(char op_line[])
{
	char *oper, sep[3]=" \n", *argum;
	int ok;

	oper = strtok(op_line, sep);

	if (strcmp(oper, "LOAD") == 0) {
		argum = strtok(NULL, sep);
		if (argum != NULL)
			return 1;	// Comanda LOAD este valida
	}

	if (strcmp(oper, "SELECT") == 0) {
		argum = strtok(NULL, sep);
		if (argum == NULL)
			return -1;	// Comanda invalida
		else if (strcmp(argum, "ALL") == 0)
			return 2;	// Comanda SELECT ALL este valida
		else {
			ok = 1;
			for (int i = 1; i <= 4; ++i) {
				if (valid_int(argum) == 0) {
					ok = 0;
					i = 5;
				}
				argum = strtok(NULL, sep);
			}
			if (ok == 1)
				return 3;	// Comanda SELECT este valida
			else
				return -1;
		}
	}

	if (strcmp(oper, "ROTATE") == 0) {
		argum = strtok(NULL, sep);
		if (argum != NULL && valid_int(argum) == 1)
			return 4;	// Comanda ROTATE este valida
	}

	if (strcmp(oper, "CROP") == 0)
		return 5;	// Comanda CROP este valida

	if (strcmp(oper, "GRAYSCALE") == 0)
		return 6;	// Comanda GRAYSCALE este valida

	if (strcmp(oper, "SEPIA") == 0)
		return 7;	// Comanda SEPIA este valida
	
	if (strcmp(oper, "SAVE") == 0) {
		argum = strtok(NULL, sep);
		if (argum != NULL)
			return 8;	// Comanda SAVE este valida
	}

	return -1;	// Comanda invalida
}

// ---FUNCTII PENTRU LOAD---

// Trece peste comentarii in citire
void pass_comm(char *comment, int comm_len, FILE *in)
{
	char c;
	comment[0] = '\0'; // Presupun ca nu exista comentariu
	for ( ; ; ) {	// Ciclu infinit
		c = getc(in);
		if (isspace(c)) {
			continue;
		} else {
			ungetc(c, in);
			if (c == '#')	// Am gasit comentariu
				fgets(comment, comm_len, in);
			return;	// Iese din ciclul infinit
		}
	}
}

image load_p2(image *img, FILE *in)
{
	img->mat = (int **)malloc(img->height * sizeof(int *));
	for (int i = 0; i < img->height; ++i)
		img->mat[i] = (int *)malloc((img->width) * sizeof(int));

	for (int i = 0; i < img->height; ++i)
		for (int j = 0; j < img->width; ++j)
			fscanf(in, " %d", &img->mat[i][j]);
	img->loaded = 1;

	return *img;
}

image load_p3(image *img, FILE *in)
{
	img->px_mat = (pixel **)malloc(img->height * sizeof(pixel *));
	for (int i = 0; i < img->height; ++i)
		img->px_mat[i] = (pixel *)malloc(img->width * sizeof(pixel));
	for (int i = 0; i < img->height; ++i)
		for (int j = 0; j < img->width; ++j) {
			fscanf(in, " %d", &img->px_mat[i][j].red);
			fscanf(in, " %d", &img->px_mat[i][j].green);
			fscanf(in, " %d", &img->px_mat[i][j].blue);
		}
	img->loaded_px = 1;

	return *img;
}

image load_p5(image *img, FILE *in)
{
	img->mat = (int **)malloc(img->height * sizeof(int *));
	for (int i = 0; i < img->height; ++i)
		img->mat[i] = (int *)malloc(img->width * sizeof(int));

	for (int i = 0; i < img->height; ++i)
		for (int j = 0; j < img->width; ++j)
			img->mat[i][j] = fgetc(in);

	img->loaded = 1;

	return *img;
}

image load_p6(image *img, FILE *in)
{
	img->px_mat = (pixel **)malloc(img->height * sizeof(pixel *));
	for (int i = 0; i < img->height; ++i)
		img->px_mat[i] = (pixel *)malloc(img->width * sizeof(pixel));
	for (int i = 0; i < img->height; ++i)
		for (int j = 0; j < img->width; ++j) {
			img->px_mat[i][j].red = fgetc(in);
			img->px_mat[i][j].green = fgetc(in);
			img->px_mat[i][j].blue = fgetc(in);
		}
	img->loaded_px = 1;

	return *img;
}

image load(FILE *in, image *img)
{
	char comment[NMAX] = "";

	fscanf(in, " %2s", img->magic_word); // Specificator de format
	do
		pass_comm(comment, NMAX, in);
	while (comment[0] != '\0');

	fscanf(in, " %d", &img->width);	// Latime imagine
	do
		pass_comm(comment, NMAX, in);
	while (comment[0] != '\0');

	fscanf(in, " %d", &img->height);	// Inaltime imagine
	do
		pass_comm(comment, NMAX, in);
	while (comment[0] != '\0');

	fscanf(in, " %d", &img->maxval);	// Valoare maxima imagine
	do
		pass_comm(comment, NMAX, in);
	while (comment[0] != '\0');

	switch(img->magic_word[1]) {
		case '2':	// Format P2
			*img = load_p2(img, in);
			break;

		case '3':	// Format P3
			*img = load_p3(img, in);
			break;

		case '5':	// Format P5
			*img = load_p5(img, in);
			break;

		case '6':	// Format P6
			*img = load_p6(img, in);
			break;
	}

	img->xmin = 0; img->ymin = 0;	// Este selectata intreaga suprafata
	img->xmax = img->width; img->ymax = img->height;

	return *img;
}

// ---FUNCTIE PENTRU SELECT ALL---

image select_all(image *img)
{
	img->xmin = 0;
	img->ymin = 0;
	img->xmax = img->width;
	img->ymax = img->height;

	printf("Selected ALL\n");
	return *img;
}

// ---FUNCTIE PENTRU SELECT---

image select_area(image *img, char aux[NMAX])
{
	int x1, y1, x2, y2;
	char *argum, sep[3] = " \n";

	argum = strtok(aux, sep);

	argum = strtok(NULL, sep);
	x1 = atoi(argum);

	argum = strtok(NULL, sep);
	y1 = atoi(argum);

	argum = strtok(NULL, sep);
	x2 = atoi(argum);

	argum = strtok(NULL, sep);
	y2 = atoi(argum);

	if (x1 < 0 || x2 < 0 ||
		y1 < 0 || y2 < 0 ||
		x1 > img->width || x2 > img->width ||
		y1 > img->height || y2 > img->height)
		printf("Invalid set of coordinates\n");
	else {
		if (x1 < x2) {
			img->xmin = x1;
			img->xmax = x2;
		}
		else {
			img->xmin = x2;
			img->xmax = x1;
		}

		if (y1 < y2) {
			img->ymin = y1;
			img->ymax = y2;
		}
		else {
			img->ymin = y2;
			img->ymax = y1;
		}
		printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
	}

	return *img;
}

// ---FUNCTII PENTRU ROTATE---

image rotate_2_5_full(image *img)
{
	int **aux, sw = 0;

	aux = (int **)malloc(img->width * sizeof(int *));
	for(int i = 0; i < img->width; ++i)
		aux[i] = (int *)malloc(img->height * sizeof(int));

	for (int i = 0; i < img->width; ++i)
		for(int j = 0; j < img->height; ++j)
			aux[i][j] = img->mat[img->height - j - 1][i];

	*img = free_mem(img);

	img->mat = (int **)malloc(img->width * sizeof(int *));
	for(int i = 0; i < img->width; ++i)
		img->mat[i] = (int *)malloc(img->height * sizeof(int));

	for (int i = 0; i < img->width; ++i)
		for(int j = 0; j < img->height; ++j)
			img->mat[i][j] = aux[i][j];

	for(int i = 0; i < img->width; i++)
		free(aux[i]);
	free(aux);

	sw = img->width; img->width = img->height; img->height = sw;
	img->xmin = 0; img->ymin = 0;
	img->xmax = img->width; img->ymax = img->height;

	return *img;
}

image rotate_3_6_full(image *img)
{
	pixel **aux;
	int sw = 0;

	aux = (pixel **)malloc(img->width * sizeof(pixel *));
	for(int i = 0; i < img->width; ++i)
		aux[i] = (pixel *)malloc(img->height * sizeof(pixel));

	for (int i = 0; i < img->width; ++i)
		for(int j = 0; j < img->height; ++j)
			aux[i][j] = img->px_mat[img->height - j - 1][i];

	*img = free_mem_px(img);

	img->px_mat = (pixel **)malloc(img->width * sizeof(pixel *));
	for(int i = 0; i < img->width; ++i)
		img->px_mat[i] = (pixel*)malloc(img->height * sizeof(pixel));

	for (int i = 0; i < img->width; ++i)
		for(int j = 0; j < img->height; ++j)
			img->px_mat[i][j] = aux[i][j];

	for(int i = 0; i < img->width; i++)
		free(aux[i]);
	free(aux);

	sw = img->width; img->width = img->height; img->height = sw;
	img->xmin = 0; img->ymin = 0;
	img->xmax = img->width; img->ymax = img->height;

	return *img;
}

image rotate_2_5_square(image *img)
{
	int **aux1, **aux2;

	aux1 = (int **)malloc((img->ymax - img->ymin) * sizeof(int *));
	for(int i = 0; i < (img->ymax - img->ymin); ++i)
		aux1[i] = (int *)malloc((img->xmax - img->xmin) * sizeof(int));

	for (int i = 0; i < (img->ymax - img->ymin); ++i)
		for (int j = 0; j < (img->xmax - img->xmin); ++j)
			aux1[i][j] = img->mat[i + img->ymin][j + img->xmin];

	aux2 = (int **)malloc((img->ymax - img->ymin) * sizeof(int *));
	for(int i = 0; i < (img->ymax - img->ymin); ++i)
		aux2[i] = (int *)malloc((img->xmax - img->xmin) * sizeof(int));

	for (int i = 0 ; i < (img->ymax - img->ymin); ++i)
		for (int j = 0 ; j < (img->xmax - img->xmin); ++j)
			aux2[i][j] = aux1[img->ymax - img->ymin - j - 1][i];

	for (int i = img->ymin; i < img->ymax; ++i)
		for (int j = img->xmin; j < img->xmax; ++j)
			img->mat[i][j] = aux2[i - img->ymin][j - img->xmin];

	for(int i = 0; i < (img->ymax - img->ymin); i++)
		free(aux1[i]);
	free(aux1);

	for(int i = 0; i < (img->ymax - img->ymin); i++)
		free(aux2[i]);
	free(aux2);

	return *img;
}

image rotate_3_6_square(image *img)
{
	pixel **aux1, **aux2;

	aux1 = (pixel **)malloc((img->ymax - img->ymin) * sizeof(pixel *));
	for(int i = 0; i < (img->ymax - img->ymin); ++i)
		aux1[i] = (pixel *)malloc((img->xmax - img->xmin) * sizeof(pixel));

	for (int i = 0; i < (img->ymax - img->ymin); ++i)
		for (int j = 0; j < (img->xmax - img->xmin); ++j)
			aux1[i][j] = img->px_mat[i + img->ymin][j + img->xmin];

	aux2 = (pixel **)malloc((img->ymax - img->ymin) * sizeof(pixel *));
	for(int i = 0; i < (img->ymax - img->ymin); ++i)
		aux2[i] = (pixel *)malloc((img->xmax - img->xmin) * sizeof(pixel));

	for (int i = 0 ; i < (img->ymax - img->ymin); ++i)
		for (int j = 0 ; j < (img->xmax - img->xmin); ++j)
			aux2[i][j] = aux1[img->ymax - img->ymin - j - 1][i];

	for (int i = img->ymin; i < img->ymax; ++i)
		for (int j = img->xmin; j < img->xmax; ++j)
			img->px_mat[i][j] = aux2[i - img->ymin][j - img->xmin];

	for(int i = 0; i < (img->ymax - img->ymin); i++)
		free(aux1[i]);
	free(aux1);

	for(int i = 0; i < (img->ymax - img->ymin); i++)
		free(aux2[i]);
	free(aux2);

	return *img;
}

// Rotire 90 de grade spre stanga
image rotate_90(image *img)
{
	if (img->magic_word[1] == '2' || img->magic_word[1] == '5') {
		if (img->xmin == 0 && img->xmax == img->width &&
		img->ymin == 0 && img->ymax == img->height)
			*img = rotate_2_5_full(img);
		else
			*img = rotate_2_5_square(img);
		}
	else {
		if (img->xmin == 0 && img->xmax == img->width &&
		img->ymin == 0 && img->ymax == img->height)
			*img = rotate_3_6_full(img);
		else
			*img = rotate_3_6_square(img);
	}
	return *img;
}

image rotate(image *img, char aux[NMAX])
{
	char *argum = NULL, sep[3] = " \n";
	int angle = 0;

	if ((img->xmin != 0 || img->xmax != img->width ||
		img->ymin != 0 || img->ymax != img->height) &&
		(img->xmax - img->xmin != img->ymax - img->ymin)) {
		printf("The selection must be square\n");
		return *img;
	}

	argum = strtok(aux, sep);
	argum = strtok(NULL, sep);
	angle = atoi(argum);

	if(angle % 90 != 0 || angle > 270 || angle < -270) {
		printf("Unsupported rotation angle\n");
		return *img;
	}

	if (angle > 0)
		for (int i = 1; i <= angle / 90; ++i)
			*img = rotate_90(img);
	else
		for (int i = 1; i <= 4 - abs(angle / 90); ++i)
			*img = rotate_90(img);

	printf("Rotated %s\n", argum);
	return *img;
}

// ---FUNCTIE PENTRU CROP---

image crop_2_5(image *img)
{
	int **aux;

	aux = (int **)malloc((img->ymax - img->ymin) * sizeof(int *));
	for(int i = 0; i < (img->ymax - img->ymin); ++i)
		aux[i] = (int *)malloc((img->xmax - img->xmin) * sizeof(int));

	for (int i = 0; i < (img->ymax - img->ymin); ++i)
		for (int j = 0; j < (img->xmax - img->xmin); ++j)
			aux[i][j] = img->mat[i + img->ymin][j + img->xmin];

	*img = free_mem(img);

	img->mat = (int **)malloc((img->ymax - img->ymin) * sizeof(int *));
	for(int i = 0; i < (img->ymax - img->ymin); ++i)
		img->mat[i] = (int *)malloc((img->xmax - img->xmin) * sizeof(int));

	for (int i = 0; i < (img->ymax - img->ymin); ++i)
		for (int j = 0; j < (img->xmax - img->xmin); ++j)
			img->mat[i][j] = aux[i][j];

	for (int i = 0; i < (img->ymax - img->ymin); ++i)
		free(aux[i]);
	free(aux);

	img->width = img->xmax - img->xmin;
	img->height = img->ymax - img->ymin;
	img->xmin = 0; img->ymin = 0;
	img->xmax = img->width; img->ymax = img->height;

	return *img;
}

image crop_3_6(image *img)
{
	pixel **aux;

	aux = (pixel **)malloc((img->ymax - img->ymin) * sizeof(pixel *));
	for(int i = 0; i < (img->ymax - img->ymin); ++i)
		aux[i] = (pixel *)malloc((img->xmax - img->xmin) * sizeof(pixel));

	for (int i = 0; i < (img->ymax - img->ymin); ++i)
		for (int j = 0; j < (img->xmax - img->xmin); ++j)
			aux[i][j] = img->px_mat[i + img->ymin][j + img->xmin];

	*img = free_mem_px(img);

	img->px_mat = (pixel**)malloc((img->ymax - img->ymin) * sizeof(pixel *));
	for(int i = 0; i < (img->ymax - img->ymin); ++i)
		img->px_mat[i] = (pixel *)malloc((img->xmax - img->xmin) * sizeof(pixel));

	for (int i = 0; i < (img->ymax - img->ymin); ++i)
		for (int j = 0; j < (img->xmax - img->xmin); ++j)
			img->px_mat[i][j] = aux[i][j];

	for (int i = 0; i < (img->ymax - img->ymin); ++i)
		free(aux[i]);
	free(aux);

	img->width = img->xmax - img->xmin;
	img->height = img->ymax - img->ymin;
	img->xmin = 0; img->ymin = 0;
	img->xmax = img->width; img->ymax = img->height;

	return *img;
}

image crop(image *img)
{
	if (img->magic_word[1] == '2' || img->magic_word[1] == '5')
		*img = crop_2_5(img);
	else
		*img = crop_3_6(img);

	printf("Image cropped\n");
	return *img;
}

// ---FUNCTIE GREYSCALE---

image grayscale(image *img)
{
	if (img->magic_word[1] == '3' || img->magic_word[1] == '6') {

		for (int i = img->ymin; i < img->ymax; ++i)
			for (int j = img->xmin; j < img->xmax; ++j) {
				double new_val = (img->px_mat[i][j].red +
							   img->px_mat[i][j].green +
							   img->px_mat[i][j].blue) / 3;

				img->px_mat[i][j].red = (int)new_val;
				img->px_mat[i][j].green = (int)new_val;
				img->px_mat[i][j].blue = (int)new_val;
			}

		printf("Grayscale filter applied\n");
		return *img;
	}
	printf("Grayscale filter not available\n");
	return *img;
}

// ---FUNCTII SEPIA---

int minimum(int a, int b)
{
	if (a < b)
		return a;
	return b;
}

image sepia(image *img)
{
	int ok = 0;
	if (img->magic_word[1] == '3' || img->magic_word[1] == '6')
		for (int i = img->ymin; i < img->ymax; ++i)
			for (int j = img->xmin; j < img->xmax; ++j)
				if(img->px_mat[i][j].red != img->px_mat[i][j].green ||
				   img->px_mat[i][j].green != img->px_mat[i][j].blue ||
				   img->px_mat[i][j].blue != img->px_mat[i][j].red)
					ok = 1;

	if (ok == 1) {
		for (int i = img->ymin; i < img->ymax; ++i)
			for (int j = img->xmin; j < img->xmax; ++j) {
				double new_red, new_green, new_blue;
				new_red = img->px_mat[i][j].red * 0.393 +
						  img->px_mat[i][j].green * 0.769 +
						  img->px_mat[i][j].blue * 0.189;

				new_green = img->px_mat[i][j].red * 0.349 +
							img->px_mat[i][j].green * 0.686 +
							img->px_mat[i][j].blue * 0.168;

				new_blue = img->px_mat[i][j].red * 0.272 +
						   img->px_mat[i][j].green * 0.534 +
						   img->px_mat[i][j].blue * 0.131;

				img->px_mat[i][j].red = (int)minimum(new_red, img->maxval);
				img->px_mat[i][j].green = (int)minimum(new_green, img->maxval);
				img->px_mat[i][j].blue = (int)minimum(new_blue, img->maxval);
			}
		printf("Sepia filter applied\n");
		return *img;
	}
	printf("Sepia filter not available\n");
	return *img;
}

// ---FUNCTIE PENTRU SAVE---

void save_img(image *img, char aux[NMAX])
{
	FILE *out;
	int ascii = 0;
	char *argum = NULL, sep[3] = " \n", output_filename[25] = "";

	argum = strtok(aux, sep);

	argum = strtok(NULL, sep);
	strcpy(output_filename, argum);

	argum = strtok(NULL, sep);
	if (argum != NULL)
		ascii = 1;

	switch(ascii) {
		case 0:	// Salvare in format binar
			out = fopen(output_filename, "wb");
			if (img->magic_word[1] == '2' || img->magic_word[1] == '5')
				fprintf(out, "P5\n");
			else
				fprintf(out, "P6\n");
		    fprintf(out, "%d %d\n", img->width, img->height);
		    fprintf(out, "%d\n", img->maxval);

		    if (img->magic_word[1] == '2' || img->magic_word[1] == '5')
		    	for (int i = 0; i < img->height; i++)
			        for (int j = 0; j < img->width; j++)
			        	fputc(img->mat[i][j], out);
			else
				for (int i = 0; i < img->height; i++)
			        for (int j = 0; j < img->width; j++) {
			            fputc(img->px_mat[i][j].red, out);
			            fputc(img->px_mat[i][j].green, out);
			            fputc(img->px_mat[i][j].blue, out);
			        }
			break;

		case 1:	// Salvare in format ASCII
			out = fopen(output_filename, "wt");
			if (img->magic_word[1] == '2' || img->magic_word[1] == '5')
				fprintf(out, "P2\n");
			else
				fprintf(out, "P3\n");
		    fprintf(out, "%d %d\n", img->width, img->height);
		    fprintf(out, "%d\n", img->maxval);

		    if (img->magic_word[1] == '2' || img->magic_word[1] == '5')
		    	for (int i = 0; i < img->height; i++) { 
			        for (int j = 0; j < img->width; j++)
			            fprintf(out, " %d", img->mat[i][j]);
			        fprintf(out, "\n"); 
			    }
			else
				for (int i = 0; i < img->height; i++) { 
			        for (int j = 0; j < img->width; j++) {
			            fprintf(out, " %d", img->px_mat[i][j].red);
			            fprintf(out, " %d", img->px_mat[i][j].green);
			            fprintf(out, " %d", img->px_mat[i][j].blue);
			        }
			        fprintf(out, "\n"); 
			    }
			break;
	}
	printf("Saved %s\n", output_filename);
	fclose(out);
}
