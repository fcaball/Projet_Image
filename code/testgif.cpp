#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gif_lib.h"
#include <iostream>
#include "image_ppm.h"

int main()
{
    char *gif_filename = "animation.gif";
    int num_frames = 1;
    char filename[100];
    int image_width, image_height;

    sprintf(filename, "./imageVieillissementPGM/image%dvieillissement.ppm", 1);
    OCTET *image_data;
    lire_nb_lignes_colonnes_image_ppm(filename, &image_width, &image_height);
    allocation_tableau(image_data, OCTET, image_width * image_height * 3);
    // Charger la première image de la séquence
    lire_image_ppm(filename, image_data, image_width * image_height);

    // Créer une nouvelle animation GIF
    int color_resolution = 8;
    int screen_width = image_width;
    int screen_height = image_height;
    ColorMapObject *color_map = NULL;
    GifColorType background_color = {0, 0, 0};
    int animation_speed = 100; // centièmes de seconde par image

    GifFileType *gif_file = EGifOpenFileName(gif_filename, false, NULL);
    if (gif_file == NULL)
    {
        printf("Erreur : impossible de créer le fichier GIF\n");
        exit(EXIT_FAILURE);
    }

    EGifSetGifVersion(gif_file, "89a");
    EGifPutScreenDesc(gif_file, screen_width, screen_height, color_resolution, 0, color_map);
    EGifPutImageDesc(gif_file, 0, 0, screen_width, screen_height, false, NULL);
    EGifPutExtension(gif_file, GRAPHICS_EXT_FUNC_CODE, 4, "\x04\x04\x00\x00\x00");
    EGifPutExtension(gif_file, APPLICATION_EXT_FUNC_CODE, 11, "NETSCAPE2.0");
    EGifPutExtension(gif_file, APPLICATION_EXT_FUNC_CODE, 3, "\x01\x00\x00");

    // Ajouter chaque image de la séquence à l'animation GIF
    for (int i = 1; i < num_frames; i++)
    {
        // Charger l'image i-ème de la séquence
        sprintf(filename, "./imageVieillissementPGM/image%dvieillissement.ppm", i);
        std::cout << i << std::endl;
        //OCTET *image_data;
        lire_nb_lignes_colonnes_image_ppm(filename, &image_width, &image_height);
        allocation_tableau(image_data, OCTET, image_width * image_height * 3);
        lire_image_ppm(filename, image_data, image_width * image_height);
        // Ajouter l'image à l'animation GIF
        GifByteType *gif_data = (GifByteType *)malloc(screen_width * screen_height * sizeof(GifByteType));
        for (int y = 0; y < screen_height; y++)
        {
            for (int x = 0; x < screen_width; x++)
            {
                int index = y * screen_width + x;
                gif_data[index] = image_data[index];
            }
        }
        EGifPutImageDesc(gif_file, 0, 0, screen_width, screen_height, false, color_map);
        free(gif_data);
        free(image_data);
    }

    // Fermer le fichier GIF
    EGifCloseFile(gif_file, NULL);

    return 0;
}
