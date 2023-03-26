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

std::vector<OCTET *> baseQuart ;
std::vector<OCTET *> baseNdG ;
std::vector<OCTET *> baseGradient ;

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
    strcpy(strIn, "./bddPPM/base");
    strcat(strIn, num);
    strcat(strIn, ndg);

    char strOut[80];
    strcpy(strOut, "./bddPPM/sortie");
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
  double ecartMoyenne, ecartVariance;
  std::vector<double> result(taille);
  for (int i = 0; i < taille; i++)
  {
    result[i] = abs(m[i] - moyenne) * abs(m[i] - moyenne); 
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

void UneImageEnQuatre(OCTET *ImgIn, OCTET *ImgHautGauche, OCTET *ImgHautDroite, OCTET *ImgBasGauche, OCTET *ImgBasDroite, int nW, int nH)
{

  for (size_t i = 0; i < nW / 2; i++)
  {
    for (size_t j = 0; j < nH / 2; j++)
    {
      ImgHautGauche[3 * (i * (nW / 2) + j)] = ImgIn[3 * (i * nW + j)];
      ImgHautGauche[3 * (i * (nW / 2) + j) + 1] = ImgIn[3 * (i * nW + j) + 1];
      ImgHautGauche[3 * (i * (nW / 2) + j) + 2] = ImgIn[3 * (i * nW + j) + 2];
    }
  }

  for (size_t i = 0; i < nW / 2; i++)
  {
    for (size_t j = 0; j < nH / 2; j++)
    {
      ImgHautDroite[3 * (i * (nW / 2) + j)] = ImgIn[3 * ((i)*nW + j + nW / 2)];
      ImgHautDroite[3 * (i * (nW / 2) + j) + 1] = ImgIn[3 * ((i)*nW + j + nW / 2) + 1];
      ImgHautDroite[3 * (i * (nW / 2) + j) + 2] = ImgIn[3 * ((i)*nW + j + nW / 2) + 2];
    }
  }

  for (size_t i = 0; i < nW / 2; i++)
  {
    for (size_t j = 0; j < nH / 2; j++)
    {
      ImgBasGauche[3 * (i * (nW / 2) + j)] = ImgIn[3 * ((i + nW / 2) * nW + j)];
      ImgBasGauche[3 * (i * (nW / 2) + j) + 1] = ImgIn[3 * ((i + nW / 2) * nW + j) + 1];
      ImgBasGauche[3 * (i * (nW / 2) + j) + 2] = ImgIn[3 * ((i + nW / 2) * nW + j) + 2];
    }
  }

  for (size_t i = 0; i < nW / 2; i++)
  {
    for (size_t j = 0; j < nH / 2; j++)
    {
      ImgBasDroite[3 * ((i * (nW / 2)) + j)] = ImgIn[3 * ((i + nW / 2) * nW + j + nW / 2)];
      ImgBasDroite[3 * ((i * (nW / 2)) + j) + 1] = ImgIn[3 * ((i + nW / 2) * nW + j + nW / 2) + 1];
      ImgBasDroite[3 * ((i * (nW / 2)) + j) + 2] = ImgIn[3 * ((i + nW / 2) * nW + j + nW / 2) + 2];
    }
  }
}

OCTET *QuatreImagesEnUne(OCTET *ImgHautGauche, OCTET *ImgHautDroite, OCTET *ImgBasGauche, OCTET *ImgBasDroite, int WFinale, int HFinale)
{

  OCTET *ImgOut;
  allocation_tableau(ImgOut, OCTET, WFinale * HFinale * 3);

  for (size_t i = 0; i < WFinale / 2; i++)
  {
    for (size_t j = 0; j < HFinale / 2; j++)
    {
      ImgOut[3 * (i * WFinale + j)] = ImgHautGauche[3 * (i * (WFinale / 2) + j)];
      ImgOut[3 * (i * WFinale + j)+1] = ImgHautGauche[3 * (i * (WFinale / 2) + j)+1];
      ImgOut[3 * (i * WFinale + j)+2] = ImgHautGauche[3 * (i * (WFinale / 2) + j)+2];
    }
  }
  for (size_t i = 0; i < WFinale / 2; i++)
  {
    for (size_t j = 0; j < HFinale / 2; j++)
    {
      ImgOut[3*(i*WFinale + j + WFinale / 2)] = ImgHautDroite[3*(i * (WFinale / 2) + j)];
      ImgOut[3*(i*WFinale + j + WFinale / 2)+1] = ImgHautDroite[3*(i * (WFinale / 2) + j)+1];
      ImgOut[3*(i*WFinale + j + WFinale / 2)+2] = ImgHautDroite[3*(i * (WFinale / 2) + j)+2];
    }
  }

  for (size_t i = 0; i < WFinale / 2; i++)
  {
    for (size_t j = 0; j < HFinale / 2; j++)
    {
      ImgOut[3*((i + WFinale / 2) * WFinale + j)] = ImgBasGauche[3*(i * (WFinale / 2) + j)];
      ImgOut[3*((i + WFinale / 2) * WFinale + j) +1] = ImgBasGauche[3*(i * (WFinale / 2) + j) +1];
      ImgOut[3*((i + WFinale / 2) * WFinale + j) +2] = ImgBasGauche[3*(i * (WFinale / 2) + j) +2];
    }
  }
  for (size_t i = 0; i < WFinale / 2; i++)
  {
    for (size_t j = 0; j < HFinale / 2; j++)
    {
      ImgOut[3*((i + WFinale / 2) * WFinale + j + WFinale / 2)] = ImgBasDroite[3*(i * (WFinale / 2) + j)];
      ImgOut[3*((i + WFinale / 2) * WFinale + j + WFinale / 2) +1] = ImgBasDroite[3*(i * (WFinale / 2) + j) +1];
      ImgOut[3*((i + WFinale / 2) * WFinale + j + WFinale / 2) +2] = ImgBasDroite[3*(i * (WFinale / 2) + j) +2];
    }
  }
  return ImgOut;
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

OCTET* conversionRGBtoNDG(OCTET* ImgCouleur, OCTET* ImgNdg, int taille){
  for(int i = 0 ; i<taille ; i++){
    ImgNdg[i] = (0.299 * ImgCouleur[i * 3]) + (0.587 * ImgCouleur[i * 3 + 1]) + (0.114 * ImgCouleur[i * 3 + 2]);
  }
  return ImgNdg ;
}


OCTET* conversionGradient(OCTET* ImgNDG, OCTET* gradient, int w, int h){
  int Ii, Ij ;
  for(int i = 0 ; i<h-1 ; i++){
    for(int j = 0 ; j<w-1 ; j++){
        Ij = ImgNDG[i*w + j+1] - ImgNDG[i*w + j];
        Ii = ImgNDG[(i+1)*w + j] - ImgNDG[i*w + j];
        gradient[i*w + j] = sqrt(Ii*Ii + Ij*Ij);
    }
   }
   return gradient ;
}

OCTET* seuilAuto(OCTET* ImgGradient, OCTET* sortie, int taille){
  int max = 0 ;
  for(int i = 0 ; i<taille ; i++){
    if(ImgGradient[i]>max){
      max = ImgGradient[i];
    }
  }
  int seuil = max/3 ;
  for(int i = 0 ; i<taille ; i++){
    if(ImgGradient[i]>seuil){
      sortie[i] = 255 ;
    }
    else{
      sortie[i] = 0 ;
    }
  }
  return sortie ;
}

int indice(OCTET* imageGradient, int taille){
  int tailleBase = base.size() ;
  std::vector<int> vect(tailleBase);
  int diff ;
  int minDif = 255*taille ;
  int indice = 0 ;
  for(int i = 0 ; i<tailleBase ; i++){
    diff = 0 ;
    for(int j = 0 ; j<taille ; j++){
      diff += abs(imageGradient[j] - baseGradient[i][j]);
    }
    vect[i] = diff ;
    if(diff<minDif){
      minDif = diff ;
      indice = i ;
    }
  }
  return indice ;
}

int main(int argc, char *argv[])
{
  int taille_des_imagettes=128;
  //constructionBase(24, taille_des_imagettes, taille_des_imagettes);
  constructionBaseCouleur(1000, taille_des_imagettes, taille_des_imagettes);
  int length = base.size();

  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille, nTaille3;

  if (argc != 3)
  {
    printf("Usage: ImageIn.ppm ImageOut.ppm \n");
    exit(1);
  }

  sscanf(argv[1], "%s", cNomImgLue);
  sscanf(argv[2], "%s", cNomImgEcrite);

  OCTET *ImgIn, *ImgOut;


  //DIVISION EN IMAGETTES (QUARTS)
  OCTET *hg, *hd, *bg, *bd ;
  
  for(int i = 0 ; i<1000 ; i++){
    allocation_tableau(hg, OCTET, 64*64*3);
    allocation_tableau(hd, OCTET, 64*64*3);
    allocation_tableau(bg, OCTET, 64*64*3);
    allocation_tableau(bd, OCTET, 64*64*3);
    UneImageEnQuatre(base[i], hg, hd, bg, bd, 128, 128);
    baseQuart.push_back(hg);
    baseQuart.push_back(hd);
    baseQuart.push_back(bg);
    baseQuart.push_back(bd);
  }

  base = baseQuart ;

  int taille = base.size() ;
  baseNdG.resize(taille);
  baseGradient.resize(taille);

  //NIVEAUX DE GRIS
  OCTET* modif ;
  for(int i = 0 ; i<taille ; i++){
    allocation_tableau(modif, OCTET, 64*64);
    baseNdG[i] = conversionRGBtoNDG(base[i], modif, 64*64);
  }

  //GRADIENT
  OCTET* modifGradient ;
  for(int i = 0 ; i<taille ; i++){
    allocation_tableau(modifGradient, OCTET, 64*64);
    baseGradient[i] = conversionGradient(baseNdG[i], modifGradient, 64, 64);
  }

  lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);

  allocation_tableau(ImgIn, OCTET, 128*128*3);

  lire_image_ppm(cNomImgLue, ImgIn, 128*128);


  //DIVISION DE L'IMAGE DE DEPART
  OCTET *testHG, *testHD, *testBG, *testBD ;
  allocation_tableau(testHG, OCTET, 64*64*3);
  allocation_tableau(testHD, OCTET, 64*64*3);
  allocation_tableau(testBG, OCTET, 64*64*3);
  allocation_tableau(testBD, OCTET, 64*64*3);
  UneImageEnQuatre(ImgIn, testHG, testHD, testBG, testBD, 128, 128);


  //NIVEAUX DE GRIS
  OCTET *testHGndg, *testHDndg, *testBGndg, *testBDndg ;
  //OCTET *testHGndgB, *testHDndgB, *testBGndgB, *testBDndgB ;
  allocation_tableau(testHGndg, OCTET, 64*64);
  allocation_tableau(testHDndg, OCTET, 64*64);
  allocation_tableau(testBGndg, OCTET, 64*64);
  allocation_tableau(testBDndg, OCTET, 64*64);
  testHGndg = conversionRGBtoNDG(testHG, testHGndg, 64*64);
  testHDndg = conversionRGBtoNDG(testHD, testHDndg, 64*64);
  testBGndg = conversionRGBtoNDG(testBG, testBGndg, 64*64);
  testBDndg = conversionRGBtoNDG(testBD, testBDndg, 64*64);

  //GRADIENT
  OCTET *testHGgradient, *testHDgradient, *testBGgradient, *testBDgradient ;
  allocation_tableau(testHGgradient, OCTET, 64*64);
  allocation_tableau(testHDgradient, OCTET, 64*64);
  allocation_tableau(testBGgradient, OCTET, 64*64);
  allocation_tableau(testBDgradient, OCTET, 64*64);
  testHGgradient = conversionGradient(testHGndg, testHGgradient, 64, 64);
  testHDgradient = conversionGradient(testHDndg, testHDgradient, 64, 64);
  testBGgradient = conversionGradient(testBGndg, testBGgradient, 64, 64);
  testBDgradient = conversionGradient(testBDndg, testBDgradient, 64, 64);

  //RECUPERATION DES BONNES IMAGETTES
  int ind = indice(testHGgradient, 64*64);
  std::cout<<"valeur de l'indice : "<<ind ;
  int ind1 = indice(testHDgradient, 64*64);
  std::cout<<"valeur de l'indice : "<<ind1 ;
  int ind2 = indice(testBGgradient, 64*64);
  std::cout<<"valeur de l'indice : "<<ind2 ;
  int ind3 = indice(testBDgradient, 64*64);
  std::cout<<"valeur de l'indice : "<<ind3 ;
  allocation_tableau(ImgOut, OCTET, 128*128*3);
  ImgOut = QuatreImagesEnUne(base[ind], base[ind1], base[ind2], base[ind3], 128, 128);
  ecrire_image_ppm(cNomImgEcrite, ImgOut, 128, 128); 
  /* OCTET* modifGradientSeuil ;
  for(int i = 0 ; i<taille ; i++){
    allocation_tableau(modifGradientSeuil, OCTET, 64*64);
    baseGradient[i] = seuilAuto(baseGradient[i], modifGradientSeuil, 64*64);
  }

  ecrire_image_pgm((char*) "caMarcheGradientSeuil.pgm", baseGradient[100], 64, 64); */



  /* lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
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

  ecrire_image_ppm(cNomImgEcrite, ImgIn, nH, nW);
  //ecrire_image_ppm((char*)"TESTindice.ppm", base[1], taille_des_imagettes, taille_des_imagettes);
  //ecrire_image_ppm((char*)"TESTindice.ppm", base[0], taille_des_imagettes, taille_des_imagettes); */

  return 1;
}