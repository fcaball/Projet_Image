#ifndef MOSAIQUEMAKER_H
#define MOSAIQUEMAKER_H

#include <stdio.h>
#include "../code/image_ppm.h"
#include <vector>
#include <iostream>
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include "mainwindow.h"
class mosaiquemaker
{
public:
    mosaiquemaker();
    int makeMosaique(int choix,char* cNomImgLue, char* cNomImgEcrite,MainWindow *main,int tailleImagette=-1);
    void gif(QStringList args);


private:
    std::vector<OCTET *> base; // base de donnees
    std::vector<double> m;     // moyenne
    std::vector<double> v;     // variance

    std::vector<double> mR; // moyenne
    std::vector<double> vR; // variance

    std::vector<double> mV; // moyenne
    std::vector<double> vV; // variance

    std::vector<double> mB; // moyenne
    std::vector<double> vB; // variance

    std::vector<OCTET *> baseQuart;
    std::vector<OCTET *> baseNdG;
    std::vector<OCTET *> baseGradient;
    std::vector<OCTET *> baseGif;
    std::vector<OCTET*> baseGifResult ;


    double moy(OCTET *Image, int taille);
    double var(OCTET *Image, int taille, double moyenne);
    OCTET *modifImage(OCTET *Img, int dimDep, int dimObjectif);
    void constructionBase(int tailleBase, int h, int w);
    void constructionBaseCouleur(int tailleBase, int h, int w);
    int choixImagette(double moyenne, double variance);
    void remplacerBloc(OCTET *ImgDepart, OCTET *ImgSortie, int tailleDep, int tailleImagette);
    void UneImageEnQuatre(OCTET *ImgIn, OCTET *ImgHautGauche, OCTET *ImgHautDroite, OCTET *ImgBasGauche, OCTET *ImgBasDroite, int nW, int nH);
    OCTET *QuatreImagesEnUne(OCTET *ImgHautGauche, OCTET *ImgHautDroite, OCTET *ImgBasGauche, OCTET *ImgBasDroite, int WFinale, int HFinale);
    int choixImagetteCouleur(double &moyenneR, double &varianceR, double &moyenneV, double &varianceV, double &moyenneB, double &varianceB);
    void remplacerBlocCouleur(OCTET *ImgDepart, OCTET *ImgSortie, int tailleDep, int tailleImagette);
    void remplacerBlocGif(OCTET *ImgDepart, OCTET *ImgSortie, int tailleDepW, int tailleDepH, int tailleImagette);
    OCTET *conversionRGBtoNDG(OCTET *ImgCouleur, OCTET *ImgNdg, int taille);
    OCTET *seuilAuto(OCTET *ImgGradient, OCTET *sortie, int taille);
    OCTET *conversionGradient(OCTET *ImgNDG, OCTET *gradient, int w, int h);
    int indice(OCTET *imageGradient, int taille);
    OCTET* visage(char cNomImgLue[250], char cNomImgEcrite[250]);
    OCTET *mosaiqueClassique(char cNomImgLue[250], char cNomImgEcrite[250], int taille_des_imagettes);







};

#endif // MOSAIQUEMAKER_H
