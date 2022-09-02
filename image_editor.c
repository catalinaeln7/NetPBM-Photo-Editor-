#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "functions.h"

int main(void)
{
	int command = 0, l_all = 0;
	char op_line[NMAX] = "", sep[3]=" \n", *argum, aux[NMAX] = "";
	static image info, *img; FILE *in;
	img = &info;
	img->loaded = 0; img->loaded_px = 0;

	fgets(op_line, NMAX, stdin);
	while (strcmp(op_line, "EXIT\0") != 0) {

		strcpy(aux, op_line); command = choose_command(op_line);
		
		if (command == 1) {
			if (img->loaded == 1) *img = free_mem(img);
			if (img->loaded_px == 1) *img = free_mem_px(img);
			img->loaded = 0; img->loaded_px = 0; l_all = 0;

			argum = strtok(aux, sep); argum = strtok(NULL, sep);
			in = fopen(argum, "r+");

			if (in == NULL) {
				printf("Failed to load %s\n", argum);
			}
			else {
				*img = load(in, img); 
				if(img->loaded == 1 || img->loaded_px == 1)
					printf("Loaded %s\n", argum);
				else
					printf("Failed to load %s\n", argum);
				if(img->loaded == 1 || img->loaded_px == 1) l_all = 1;
			}
			fclose(in);
		}
		else
			switch(command) {
				case -1:
					printf("Invalid command\n");
					break;
				default:
					switch(l_all) {
						case 0:
							printf("No image loaded\n");
							break;
						case 1:
							switch(command) {
								case 2:	//SELECT ALL
									*img = select_all(img);
									break;

								case 3: //SELECT
									*img = select_area(img, aux);
									break;

								case 4: //ROTATE
									*img = rotate(img, aux);
									break;

								case 5: //CROP
									*img = crop(img);
									break;

								case 6: //GRAYSCALE
									*img = grayscale(img);
									break;

								case 7: //SEPIA
									*img = sepia(img);
									break;

								case 8: //SAVE
									save_img(img, aux);
									break;
							}
					}
			}
		fgets(op_line, NMAX, stdin);
	}
	*img = free_mem(img);
	*img = free_mem_px(img);

	return 0;
}