// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"
#include <vector>
#include <iostream>

std::vector<OCTET *> base; //base de donnees
std::vector<double> m; //moyenne
std::vector<double> v ; //variance

double moy(OCTET* Image, int taille){
  double moyenne = 0 ;
  for(int i = 0 ; i<taille ; i++){
    moyenne += Image[i];
  }
  moyenne = (double)moyenne/taille ;
  return moyenne ;
}

double var(OCTET* Image, int taille, double moyenne){
  double variance = 0 ;
  for(int i = 0 ; i<taille ; i++){
    variance += (Image[i]-moyenne) * (Image[i]-moyenne);
  }
  variance = (double)variance/taille ;
  return variance ;
}

OCTET* modifImage(OCTET* Img, int dimDep, int dimObjectif){
  OCTET* ImgSortie ;
  double taille = dimObjectif*dimObjectif ;
  allocation_tableau(ImgSortie, OCTET, taille);
  double facteur = (double)dimDep/dimObjectif ;
  int indicei, indicej ;
  double somme ;
  for(int i = 0 ; i<dimObjectif ; i++){
    for(int j = 0 ; j<dimObjectif ; j++){
      indicei = (int)i*facteur ;
      indicej = (int)j*facteur ;
      somme = 0 ;
      for(int k = 0 ; k<facteur ; k++){
        for(int l = 0 ; l<facteur ; l++){
          somme += Img[(indicei + k)*dimDep + (indicej + l)];
        }
      }
      ImgSortie[i*dimObjectif+j] = somme/(facteur*facteur);
    }
  }
  return ImgSortie ;
}

void constructionBase()
{
  OCTET *Img;
  int w, h, taille ;
  double moyenne, variance ;
  lire_nb_lignes_colonnes_image_pgm((char*)"base1ndg.pgm", &h, &w);
  taille = w*h ;
  allocation_tableau(Img, OCTET, taille);
  lire_image_pgm((char*)"base1ndg.pgm", Img, taille) ;
  base.push_back(Img);
  moyenne = moy(Img, taille);
  m.push_back(moyenne);
  variance = var(Img, taille, moyenne) ;
  v.push_back(variance);
  

  lire_nb_lignes_colonnes_image_pgm((char*)"base2ndg.pgm", &h, &w);
  allocation_tableau(Img, OCTET, taille);
  lire_image_pgm((char*)"base2ndg.pgm", Img, taille) ;
  base.push_back(Img);
  moyenne = moy(Img, taille);
  m.push_back(moyenne);
  variance = var(Img, taille, moyenne) ;
  v.push_back(variance);

  lire_nb_lignes_colonnes_image_pgm((char*)"base3ndg.pgm", &h, &w);
  allocation_tableau(Img, OCTET, taille);
  lire_image_pgm((char*)"base3ndg.pgm", Img, taille) ;
  base.push_back(Img);
  moyenne = moy(Img, taille);
  m.push_back(moyenne);
  variance = var(Img, taille, moyenne) ;
  v.push_back(variance);

  lire_nb_lignes_colonnes_image_pgm((char*)"base4ndg.pgm", &h, &w);
  allocation_tableau(Img, OCTET, taille);
  lire_image_pgm((char*)"base4ndg.pgm", Img, taille) ;
  base.push_back(Img);
  moyenne = moy(Img, taille);
  m.push_back(moyenne);
  variance = var(Img, taille, moyenne) ;
  v.push_back(variance);

  lire_nb_lignes_colonnes_image_pgm((char*)"base5ndg.pgm", &h, &w);
  allocation_tableau(Img, OCTET, taille);
  lire_image_pgm((char*)"base5ndg.pgm", Img, taille) ;
  base.push_back(Img);
  moyenne = moy(Img, taille);
  m.push_back(moyenne);
  variance = var(Img, taille, moyenne) ;
  v.push_back(variance);

  lire_nb_lignes_colonnes_image_pgm((char*)"base6ndg.pgm", &h, &w);
  allocation_tableau(Img, OCTET, taille);
  lire_image_pgm((char*)"base6ndg.pgm", Img, taille) ;
  base.push_back(Img);
  moyenne = moy(Img, taille);
  m.push_back(moyenne);
  variance = var(Img, taille, moyenne) ;
  v.push_back(variance);

  lire_nb_lignes_colonnes_image_pgm((char*)"base7ndg.pgm", &h, &w);
  allocation_tableau(Img, OCTET, taille);
  lire_image_pgm((char*)"base7ndg.pgm", Img, taille) ;
  base.push_back(Img);
  moyenne = moy(Img, taille);
  m.push_back(moyenne);
  variance = var(Img, taille, moyenne) ;
  v.push_back(variance);

  lire_nb_lignes_colonnes_image_pgm((char*)"base8ndg.pgm", &h, &w);
  allocation_tableau(Img, OCTET, taille);
  lire_image_pgm((char*)"base8ndg.pgm", Img, taille) ;
  base.push_back(Img);
  moyenne = moy(Img, taille);
  m.push_back(moyenne);
  variance = var(Img, taille, moyenne) ;
  v.push_back(variance);

  lire_nb_lignes_colonnes_image_pgm((char*)"base9ndg.pgm", &h, &w);
  allocation_tableau(Img, OCTET, taille);
  lire_image_pgm((char*)"base9ndg.pgm", Img, taille) ;
  base.push_back(Img);
  moyenne = moy(Img, taille);
  m.push_back(moyenne);
  variance = var(Img, taille, moyenne) ;
  v.push_back(variance);

  lire_nb_lignes_colonnes_image_pgm((char*)"base10ndg.pgm", &h, &w);
  allocation_tableau(Img, OCTET, taille);
  lire_image_pgm((char*)"base10ndg.pgm", Img, taille) ;
  base.push_back(Img);
  moyenne = moy(Img, taille);
  m.push_back(moyenne);
  variance = var(Img, taille, moyenne) ;
  v.push_back(variance);

  lire_nb_lignes_colonnes_image_pgm((char*)"base11ndg.pgm", &h, &w);
  allocation_tableau(Img, OCTET, taille);
  lire_image_pgm((char*)"base11ndg.pgm", Img, taille) ;
  base.push_back(Img);
  moyenne = moy(Img, taille);
  m.push_back(moyenne);
  variance = var(Img, taille, moyenne) ;
  v.push_back(variance);

  lire_nb_lignes_colonnes_image_pgm((char*)"base12ndg.pgm", &h, &w);
  allocation_tableau(Img, OCTET, taille);
  lire_image_pgm((char*)"base12ndg.pgm", Img, taille) ;
  base.push_back(Img);
  moyenne = moy(Img, taille);
  m.push_back(moyenne);
  variance = var(Img, taille, moyenne) ;
  v.push_back(variance);

  lire_nb_lignes_colonnes_image_pgm((char*)"base13ndg.pgm", &h, &w);
  allocation_tableau(Img, OCTET, taille);
  lire_image_pgm((char*)"base13ndg.pgm", Img, taille) ;
  base.push_back(Img);
  moyenne = moy(Img, taille);
  m.push_back(moyenne);
  variance = var(Img, taille, moyenne) ;
  v.push_back(variance);

  lire_nb_lignes_colonnes_image_pgm((char*)"base14ndg.pgm", &h, &w);
  allocation_tableau(Img, OCTET, taille);
  lire_image_pgm((char*)"base14ndg.pgm", Img, taille) ;
  base.push_back(Img);
  moyenne = moy(Img, taille);
  m.push_back(moyenne);
  variance = var(Img, taille, moyenne) ;
  v.push_back(variance);

  lire_nb_lignes_colonnes_image_pgm((char*)"base15ndg.pgm", &h, &w);
  allocation_tableau(Img, OCTET, taille);
  lire_image_pgm((char*)"base15ndg.pgm", Img, taille) ;
  base.push_back(Img);
  moyenne = moy(Img, taille);
  m.push_back(moyenne);
  variance = var(Img, taille, moyenne) ;
  v.push_back(variance);

  lire_nb_lignes_colonnes_image_pgm((char*)"base16ndg.pgm", &h, &w);
  allocation_tableau(Img, OCTET, taille);
  lire_image_pgm((char*)"base16ndg.pgm", Img, taille) ;
  base.push_back(Img);
  moyenne = moy(Img, taille);
  m.push_back(moyenne);
  variance = var(Img, taille, moyenne) ;
  v.push_back(variance);

  lire_nb_lignes_colonnes_image_pgm((char*)"base17ndg.pgm", &h, &w);
  allocation_tableau(Img, OCTET, taille);
  lire_image_pgm((char*)"base17ndg.pgm", Img, taille) ;
  base.push_back(Img);
  moyenne = moy(Img, taille);
  m.push_back(moyenne);
  variance = var(Img, taille, moyenne) ;
  v.push_back(variance);

  lire_nb_lignes_colonnes_image_pgm((char*)"base18ndg.pgm", &h, &w);
  allocation_tableau(Img, OCTET, taille);
  lire_image_pgm((char*)"base18ndg.pgm", Img, taille) ;
  base.push_back(Img);
  moyenne = moy(Img, taille);
  m.push_back(moyenne);
  variance = var(Img, taille, moyenne) ;
  v.push_back(variance);

  lire_nb_lignes_colonnes_image_pgm((char*)"base19ndg.pgm", &h, &w);
  allocation_tableau(Img, OCTET, taille);
  lire_image_pgm((char*)"base19ndg.pgm", Img, taille) ;
  base.push_back(Img);
  moyenne = moy(Img, taille);
  m.push_back(moyenne);
  variance = var(Img, taille, moyenne) ;
  v.push_back(variance);

  lire_nb_lignes_colonnes_image_pgm((char*)"base20ndg.pgm", &h, &w);
  allocation_tableau(Img, OCTET, taille);
  lire_image_pgm((char*)"base20ndg.pgm", Img, taille) ;
  base.push_back(Img);
  moyenne = moy(Img, taille);
  m.push_back(moyenne);
  variance = var(Img, taille, moyenne) ;
  v.push_back(variance);

  lire_nb_lignes_colonnes_image_pgm((char*)"base21ndg.pgm", &h, &w);
  allocation_tableau(Img, OCTET, taille);
  lire_image_pgm((char*)"base21ndg.pgm", Img, taille) ;
  base.push_back(Img);
  moyenne = moy(Img, taille);
  m.push_back(moyenne);
  variance = var(Img, taille, moyenne) ;
  v.push_back(variance);

  lire_nb_lignes_colonnes_image_pgm((char*)"base22ndg.pgm", &h, &w);
  allocation_tableau(Img, OCTET, taille);
  lire_image_pgm((char*)"base22ndg.pgm", Img, taille) ;
  base.push_back(Img);
  moyenne = moy(Img, taille);
  m.push_back(moyenne);
  variance = var(Img, taille, moyenne) ;
  v.push_back(variance);

  lire_nb_lignes_colonnes_image_pgm((char*)"base23ndg.pgm", &h, &w);
  allocation_tableau(Img, OCTET, taille);
  lire_image_pgm((char*)"base23ndg.pgm", Img, taille) ;
  base.push_back(Img);
  moyenne = moy(Img, taille);
  m.push_back(moyenne);
  variance = var(Img, taille, moyenne) ;
  v.push_back(variance);

  lire_nb_lignes_colonnes_image_pgm((char*)"base24ndg.pgm", &h, &w);
  allocation_tableau(Img, OCTET, taille);
  lire_image_pgm((char*)"base24ndg.pgm", Img, taille) ;
  base.push_back(Img);
  moyenne = moy(Img, taille);
  m.push_back(moyenne);
  variance = var(Img, taille, moyenne) ;
  v.push_back(variance);


  int tailleBase = base.size() ;
  for(int i = 0 ; i<tailleBase ; i++){
    base[i] = modifImage(base[i], w, 32);
  }
  for(int i = 0 ; i<tailleBase ; i++){
    m[i] = moy(base[i], 32);
    v[i] = var(base[i], 32, m[i]);
  }
  h = 32 ;
  w = 32 ;
  ecrire_image_pgm((char*)"sortie0.pgm", base[0], h, w);
  ecrire_image_pgm((char*)"sortie1.pgm", base[1], h, w);
  ecrire_image_pgm((char*)"sortie2.pgm", base[2], h, w);
  ecrire_image_pgm((char*)"sortie3.pgm", base[3], h, w);
  ecrire_image_pgm((char*)"sortie4.pgm", base[4], h, w);
  ecrire_image_pgm((char*)"sortie5.pgm", base[5], h, w);
  ecrire_image_pgm((char*)"sortie6.pgm", base[6], h, w);
  ecrire_image_pgm((char*)"sortie7.pgm", base[7], h, w);
  ecrire_image_pgm((char*)"sortie8.pgm", base[8], h, w);
  ecrire_image_pgm((char*)"sortie9.pgm", base[9], h, w);
  ecrire_image_pgm((char*)"sortie10.pgm", base[10], h, w);
  ecrire_image_pgm((char*)"sortie11.pgm", base[11], h, w);
  ecrire_image_pgm((char*)"sortie12.pgm", base[12], h, w);
  ecrire_image_pgm((char*)"sortie13.pgm", base[13], h, w);
  ecrire_image_pgm((char*)"sortie14.pgm", base[14], h, w);
  ecrire_image_pgm((char*)"sortie15.pgm", base[15], h, w);
  ecrire_image_pgm((char*)"sortie16.pgm", base[16], h, w);
  ecrire_image_pgm((char*)"sortie17.pgm", base[17], h, w);
  ecrire_image_pgm((char*)"sortie18.pgm", base[18], h, w);
  ecrire_image_pgm((char*)"sortie19.pgm", base[19], h, w);
  ecrire_image_pgm((char*)"sortie20.pgm", base[20], h, w);
  ecrire_image_pgm((char*)"sortie21.pgm", base[21], h, w);
  ecrire_image_pgm((char*)"sortie22.pgm", base[22], h, w);
  ecrire_image_pgm((char*)"sortie23.pgm", base[23], h, w);
}

int choixImagette(double moyenne, double variance){
  int taille = base.size() ;
 //std::cout<<"taille de la base : "<<taille ;
  double ecartMoyenne, ecartVariance ;
  std::vector<double> result(taille) ;
  for(int i = 0 ; i<taille ; i++){
    result[i] = abs(m[i] - moyenne)*abs(m[i] - moyenne) + abs(v[i]-variance)*abs(v[i]-variance);
  }
  int indice = 1 ;
  int min =  0;
  while(indice < taille){
    if(result[indice]<result[min]){
      min = indice ;
    }
    indice++;
    //std::cout<<"indice : "<<indice ;
  }
  //std::cout<<"min = "<<min ;
  return min;
}

void remplacerBloc(OCTET* ImgDepart, OCTET* ImgSortie, int tailleDep, int tailleImagette){
  int nbBlocX = tailleDep / tailleImagette ;
  int nbBlocY = tailleDep / tailleImagette ;
  int xDebut, xFin, yDebut, yFin ;
  int indice ;
  double moyenne, variance ;
  OCTET* imagette ;
  int indBase ;
  for(int i = 0 ; i<nbBlocY ; i++){
    for(int j = 0 ; j<nbBlocX ; j++){
      xDebut = j*tailleImagette ;
      xFin = (j+1)*tailleImagette ;
      yDebut = i*tailleImagette ;
      yFin = (i+1)*tailleImagette ;
      moyenne = 0 ;
      variance = 0 ;
      for(int k = yDebut ; k<yFin ; k++){
        for(int l = xDebut ; l<xFin ; l++){
          moyenne += ImgDepart[k*tailleDep + l];
        }
      }
      moyenne = (double)moyenne/(tailleImagette*tailleImagette);
      for(int k = yDebut ; k<yFin ; k++){
        for(int l = xDebut ; l<xFin ; l++){
          variance += (ImgDepart[k*tailleDep + l]-moyenne) * (ImgDepart[k*tailleDep + l]-moyenne);
        }
      }
      variance = (double)variance/(tailleImagette*tailleImagette);
      indBase = choixImagette(moyenne, variance);
      std::cout<<"indice = "<<indBase<<std::endl ;
      imagette = base[indBase] ;
      for(int k = yDebut ; k<yFin ; k++){
        for(int l = xDebut ; l<xFin ; l++){
          indice = (k - yDebut) * tailleImagette + (l - xDebut) ;
          ImgSortie[k * tailleDep+ l] = imagette[indice] ;
        }
      }
    }
  }
}

int main(int argc, char *argv[])
{
  constructionBase() ;
  int length = base.size() ;
  /* for(int i = 0 ; i<length ; i++){
    std::cout<<"moyenne = "<<m[i]<<std::endl ;
    std::cout<<"variance = "<<v[i]<<std::endl ;
    std::cout<<std::endl ;
  } */

  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille ;

  if (argc != 3)
  {
    printf("Usage: ImageIn.pgm ImageOut.pgm \n");
    exit(1);
  }

  sscanf(argv[1], "%s", cNomImgLue);
  sscanf(argv[2], "%s", cNomImgEcrite);

  OCTET *ImgIn, *ImgOut;

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(ImgOut, OCTET, nTaille);
  remplacerBloc(ImgIn, ImgOut, 512, 32);
  ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
  return 1;
}
