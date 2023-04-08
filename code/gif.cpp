#include <gif_lib.h>
#include "image_ppm.h"

int main(int argc, char **argv) {
  // Chargez les images PPM
  OCTET *images[54];
  int w, h ;
  for (size_t i = 1; i <= 54; i++)
  {
    w = 0 ;
    h = 0 ;
    char* num;
    asprintf(&num, "%d", i);

    char strIn[80];
    strcpy(strIn, "./imagesVieillissementPGM/image");
    strcat(strIn, num);
    strcat(strIn, "Vieillissement.ppm");

    lire_nb_lignes_colonnes_image_ppm(strIn,&w,&h);
    lire_image_ppm(strIn,images[i],w*h);
  }
  

  // Initialisez un objet d'animation GIF
  /* GifWriter gifWriter;
  GifBegin(&gifWriter, "animation.gif", w, h, 100); */

  // Ajoutez les images à l'animation GIF
  for (int i = 0; i < 2; i++) {
    // Convertissez l'image PPM en un tableau de pixels RGB
    unsigned char *pixels = new unsigned char[w * h * 3];
    for (int j = 0; j < w *h; j++) {
      pixels[j * 3] = images[i][j*3];
      pixels[j * 3 + 1] = images[i][j*3+1];
      pixels[j * 3 + 2] = images[i][j*3+2];
    }

    // Ajoutez l'image à l'animation GIF
    GifWriteFrame(&gifWriter, pixels, w,h, 100);
    delete[] pixels;
  }

  // Finalisez l'animation GIF
  GifEnd(&gifWriter);

  // Libérez la mémoire
  ppm_freearray(images, 2);

  return 0;
}