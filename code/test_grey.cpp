// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"
#include <vector>
#include <iostream>

std::vector<OCTET *> base; // base de donnees
std::vector<double> m;     // moyenne
std::vector<double> v;     // variance

std::vector<double> mR;     // moyenne
std::vector<double> vR;     // variance

std::vector<double> mV;     // moyenne
std::vector<double> vV;     // variance

std::vector<double> mB;     // moyenne
std::vector<double> vB;     // variance

double moy(OCTET *Image, int taille)
{
  double moyenne = 0;
  for (int i = 0; i < taille; i++)
  {
    moyenne += Image[i];
  }
  moyenne = (double)moyenne / taille;
  return moyenne;
}

double var(OCTET *Image, int taille, double moyenne)
{
  double variance = 0;
  for (int i = 0; i < taille; i++)
  {
    variance += (Image[i] - moyenne) * (Image[i] - moyenne);
  }
  variance = (double)variance / taille;
  return variance;
}

OCTET *modifImage(OCTET *Img, int dimDep, int dimObjectif)
{
  OCTET *ImgSortie;
  double taille = dimObjectif * dimObjectif;
  allocation_tableau(ImgSortie, OCTET, taille);
  double facteur = (double)dimDep / dimObjectif;
  int indicei, indicej;
  double somme;
  for (int i = 0; i < dimObjectif; i++)
  {
    for (int j = 0; j < dimObjectif; j++)
    {
      indicei = (int)i * facteur;
      indicej = (int)j * facteur;
      somme = 0;
      for (int k = 0; k < facteur; k++)
      {
        for (int l = 0; l < facteur; l++)
        {
          somme += Img[(indicei + k) * dimDep + (indicej + l)];
        }
      }
      ImgSortie[i * dimObjectif + j] = somme / (facteur * facteur);
    }
  }
  return ImgSortie;
}

void constructionBase(int tailleBase, int h, int w)
{
  int taille, width, height; 
  double moyenne, variance ;
  char *ndg = "ndg.pgm";
  char* num ;
  OCTET *Img ;
  for (size_t i = 1; i <= tailleBase; i++)
  {
    asprintf(&num, "%d", i);

    char strIn[80];
    strcpy(strIn, "./imagesNdg/base");
    strcat(strIn, num);
    strcat(strIn, ndg);

    char strOut[80];
    strcpy(strOut, "sortie");
    strcat(strOut, num);
    strcat(strOut, ndg);


    lire_nb_lignes_colonnes_image_pgm((char *)strIn, &height, &width);
    taille = width * height;
    allocation_tableau(Img, OCTET, taille);
    lire_image_pgm((char *)strIn, Img, taille);
    base.push_back(Img);
    base[i - 1] = modifImage(base[i - 1], width, w);
    moyenne = moy(Img, taille);
    m.push_back(moyenne);
    variance = var(Img, taille, moyenne);
    v.push_back(variance);
    ecrire_image_pgm((char *)strOut, base[i - 1], h, w);
    free(Img);
  } 

}

void constructionBaseCouleur(int tailleBase, int h, int w)
{
  int taille, width, height; 
  double moyenneR, varianceR, moyenneV, varianceV, moyenneB, varianceB ;
  char *ndg = "Couleur.ppm";
  char* num ;
  OCTET *Img, *ImgOut ;
  OCTET *ImgR, *ImgV, *ImgB ;
  for (size_t i = 1; i <= tailleBase; i++)
  {
    asprintf(&num, "%d", i);

    char strIn[80];
    strcpy(strIn, "base");
    strcat(strIn, num);
    strcat(strIn, ndg);

    char strOut[80];
    strcpy(strOut, "sortie");
    strcat(strOut, num);
    strcat(strOut, ndg);


    lire_nb_lignes_colonnes_image_ppm((char *)strIn, &height, &width);
    taille = width * height;
    allocation_tableau(Img, OCTET, taille*3);
    allocation_tableau(ImgOut, OCTET, taille*3);
    allocation_tableau(ImgR, OCTET, taille) ;
    allocation_tableau(ImgV, OCTET, taille) ;
    allocation_tableau(ImgB, OCTET, taille) ;
    lire_image_ppm((char *)strIn, Img, taille);

    //RECUPERATION DES COMPOSANTES
    for(int j = 0 ; j<taille ; j++){
      ImgR[j] = Img[3*j] ;
      ImgV[j] = Img[3*j + 1] ;
      ImgB[j] = Img[3*j + 2] ;
    }

    //MODIFICATION DES COMPOSANTES
    ImgR = modifImage(ImgR, width, w);
    ImgV = modifImage(ImgV, width, w);
    ImgB = modifImage(ImgB, width, w);

    //RECONSTRUCTION DE L'IMAGE
    for(int j = 0 ; j<w*w ; j++){
      ImgOut[3*j] = ImgR[j] ;
      ImgOut[3*j+1] = ImgV[j] ;
      ImgOut[3*j+2] = ImgB[j] ;
    }
    base.push_back(ImgOut);
    //base[i - 1] = modifImage(base[i - 1], width, w);
    moyenneR = moy(ImgR, w*w);
    mR.push_back(moyenneR);
    varianceR = var(ImgR, w*w, moyenneR);
    vR.push_back(varianceR);
    moyenneV = moy(ImgV, w*w);
    mV.push_back(moyenneV);
    varianceV = var(ImgV, w*w, moyenneV);
    vV.push_back(varianceV);
    moyenneB = moy(ImgB, w*w);
    mB.push_back(moyenneB);
    varianceB = var(ImgB, w*w, moyenneB);
    vB.push_back(varianceB);
    ecrire_image_ppm((char *)strOut, base[i - 1], h, w);
    free(Img);
    free(ImgR);
    free(ImgB);
    free(ImgV);
  }
}

int choixImagette(double moyenne, double variance)
{
  int taille = base.size();
  // std::cout<<"taille de la base : "<<taille ;
  double ecartMoyenne, ecartVariance;
  std::vector<double> result(taille);
  for (int i = 0; i < taille; i++)
  {
    result[i] = abs(m[i] - moyenne) * abs(m[i] - moyenne); //+ abs(v[i]-variance)*abs(v[i]-variance);
  }
  int indice = 1;
  int min = 0;
  while (indice < taille)
  {
    if (result[indice] < result[min])
    {
      min = indice;
    }
    indice++;
    // std::cout<<"indice : "<<indice ;
  }
  // std::cout<<"min = "<<min ;
  return min;
}

void remplacerBloc(OCTET *ImgDepart, OCTET *ImgSortie, int tailleDep, int tailleImagette)
{
  int nbBlocX = tailleDep / tailleImagette;
  int nbBlocY = tailleDep / tailleImagette;
  int xDebut, xFin, yDebut, yFin;
  int indice;
  double moyenne, variance;
  OCTET *imagette;
  int indBase;
  for (int i = 0; i < nbBlocY; i++)
  {
    for (int j = 0; j < nbBlocX; j++)
    {
      xDebut = j * tailleImagette;
      xFin = (j + 1) * tailleImagette;
      yDebut = i * tailleImagette;
      yFin = (i + 1) * tailleImagette;
      moyenne = 0;
      variance = 0;
      for (int k = yDebut; k < yFin; k++)
      {
        for (int l = xDebut; l < xFin; l++)
        {
          moyenne += ImgDepart[k * tailleDep + l];
        }
      }
      moyenne = (double)moyenne / (tailleImagette * tailleImagette);
      for (int k = yDebut; k < yFin; k++)
      {
        for (int l = xDebut; l < xFin; l++)
        {
          variance += (ImgDepart[k * tailleDep + l] - moyenne) * (ImgDepart[k * tailleDep + l] - moyenne);
        }
      }
      variance = (double)variance / (tailleImagette * tailleImagette);
      indBase = choixImagette(moyenne, variance);
      // std::cout << "indice = " << indBase << std::endl;
      imagette = base[indBase];
      for (int k = yDebut; k < yFin; k++)
      {
        for (int l = xDebut; l < xFin; l++)
        {
          indice = (k - yDebut) * tailleImagette + (l - xDebut);
          ImgSortie[k * tailleDep + l] = imagette[indice];
        }
      }
    }
  }
} 

int choixImagetteCouleur(double &moyenneR, double &varianceR, double &moyenneV, double &varianceV, double &moyenneB, double &varianceB)
{
  int taille = base.size();
  // std::cout<<"taille de la base : "<<taille ;
  double ecartMoyenne, ecartVariance;
  std::vector<double> resultR(taille);
  std::vector<double> resultV(taille);
  std::vector<double> resultB(taille);
  std::vector<double> result(taille);
  double maxDif, interDif, minDif ;
  for (int i = 0; i < taille; i++)
  {
    resultR[i] = abs(mR[i] - moyenneR) * abs(mR[i] - moyenneR); //+ abs(v[i]-variance)*abs(v[i]-variance);
    resultV[i] = abs(mV[i] - moyenneV) * abs(mV[i] - moyenneV);
    resultB[i] = abs(mB[i] - moyenneB) * abs(mB[i] - moyenneB);
    maxDif = std::max(resultR[i], resultV[i]);
    maxDif = std::max(maxDif, resultB[i]) ;
    minDif = std::min(resultR[i], resultV[i]);
    minDif = std::min(minDif, resultV[i]);
    interDif = resultR[i] + resultV[i] + resultB[i] - maxDif - minDif ;
    result[i] = 3*maxDif + 2*interDif + minDif ;
  }

  int indice = 1;
  int min = 0;
  while (indice < taille)
  {
    if (result[indice] < result[min])
    {
      min = indice;
    }
    indice++;
  }
  return min;
}

void remplacerBlocCouleur(OCTET *ImgDepart, OCTET *ImgSortie, int tailleDep, int tailleImagette)
{
  int nbBlocX = tailleDep / tailleImagette;
  int nbBlocY = tailleDep / tailleImagette;
  int xDebut, xFin, yDebut, yFin;
  int indice;
  double moyenneR, varianceR, moyenneV, varianceV, moyenneB, varianceB;
  OCTET *imagette;
  int indBase;
  int index = 0 ;
  for (int i = 0; i < nbBlocY; i++)
  {
    for (int j = 0; j < nbBlocX; j++)
    {
      xDebut = j * tailleImagette;
      xFin = (j + 1) * tailleImagette;
      yDebut = i * tailleImagette;
      yFin = (i + 1) * tailleImagette;
      moyenneR = 0;
      varianceR = 0;
      moyenneV = 0;
      varianceV = 0;
      moyenneB = 0;
      varianceB = 0;
      for (int k = yDebut; k < yFin; k++)
      {
        for (int l = xDebut; l < xFin; l++)
        {
          moyenneR += ImgDepart[(k * tailleDep + l)*3];
          moyenneV += ImgDepart[(k * tailleDep + l)*3 + 1];
          moyenneB += ImgDepart[(k * tailleDep + l)*3 + 2];
        }
      }
      moyenneR = (double)moyenneR / (tailleImagette * tailleImagette);
      moyenneV = (double)moyenneV / (tailleImagette * tailleImagette);
      moyenneB = (double)moyenneB / (tailleImagette * tailleImagette);
      for (int k = yDebut; k < yFin; k++)
      {
        for (int l = xDebut; l < xFin; l++)
        {
          varianceR += (ImgDepart[(k * tailleDep + l)*3] - moyenneR) * (ImgDepart[(k * tailleDep + l)*3] - moyenneR);
          varianceV += (ImgDepart[(k * tailleDep + l)*3 + 1] - moyenneV) * (ImgDepart[(k * tailleDep + l)*3 + 1] - moyenneV);
          varianceB += (ImgDepart[(k * tailleDep + l)*3 + 2] - moyenneB) * (ImgDepart[(k * tailleDep + l)*3 + 2] - moyenneB);
        }
      }
      varianceR = (double)varianceR / (tailleImagette * tailleImagette);
      varianceV = (double)varianceV / (tailleImagette * tailleImagette);
      varianceB = (double)varianceB / (tailleImagette * tailleImagette);
      indBase = choixImagetteCouleur(moyenneR, varianceR, moyenneV, varianceV, moyenneB, varianceB);
      imagette = base[indBase];
     
      for (int k = yDebut; k < yFin; k++)
      {
        for (int l = xDebut; l < xFin; l++)
        {
          indice = (k - yDebut) * tailleImagette + (l - xDebut);
          ImgSortie[3*(k * tailleDep + l)] = imagette[3*indice];
          ImgSortie[3*(k * tailleDep + l)+1] = imagette[3*indice+1];
          ImgSortie[3*(k * tailleDep + l)+2] = imagette[3*indice+2];
        }
      } 
    }
  }
}


int main(int argc, char *argv[])
{
  int taille_des_imagettes=16;
  //constructionBase(24, taille_des_imagettes, taille_des_imagettes);
  constructionBaseCouleur(24, taille_des_imagettes, taille_des_imagettes);
  int length = base.size();

  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille, nTaille3;

  if (argc != 3)
  {
    printf("Usage: ImageIn.pgm ImageOut.pgm \n");
    exit(1);
  }

  sscanf(argv[1], "%s", cNomImgLue);
  sscanf(argv[2], "%s", cNomImgEcrite);

  OCTET *ImgIn, *ImgOut;

  lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
  //lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;
  nTaille3 = 3*nTaille ;

  allocation_tableau(ImgIn, OCTET, nTaille3);
  //allocation_tableau(ImgIn, OCTET, nTaille);

  //lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
  lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(ImgOut, OCTET, nTaille3);
  
  remplacerBlocCouleur(ImgIn, ImgOut, 512, taille_des_imagettes);
  //remplacerBloc(ImgIn, ImgOut, 512, taille_des_imagettes);

  ecrire_image_ppm(cNomImgEcrite, ImgOut, nH, nW);
  //ecrire_image_ppm((char*)"TESTindice.ppm", base[1], taille_des_imagettes, taille_des_imagettes);
  //ecrire_image_ppm((char*)"TESTindice.ppm", base[0], taille_des_imagettes, taille_des_imagettes);

  return 1;
}
