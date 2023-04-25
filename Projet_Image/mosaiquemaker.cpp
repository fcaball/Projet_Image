#include "mosaiquemaker.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QObject>
#include <QProcess>

mosaiquemaker::mosaiquemaker()
{

}


double mosaiquemaker::moy(OCTET *Image, int taille)
{
    double moyenne = 0;
    for (int i = 0; i < taille; i++)
    {
        moyenne += Image[i];
    }
    moyenne = (double)moyenne / taille;
    return moyenne;
}

double mosaiquemaker::var(OCTET *Image, int taille, double moyenne)
{
    double variance = 0;
    for (int i = 0; i < taille; i++)
    {
        variance += (Image[i] - moyenne) * (Image[i] - moyenne);
    }
    variance = (double)variance / taille;
    return variance;
}

OCTET *mosaiquemaker::modifImage(OCTET *Img, int dimDep, int dimObjectif)
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

void mosaiquemaker::constructionBase(int tailleBase, int h, int w)
{
    int taille, width, height;
    double moyenne, variance;
    char *ndg = "ndg.pgm";
    char *num;
    OCTET *Img;
    for (size_t i = 1; i <= tailleBase; i++)
    {
        asprintf(&num, "%d", i);

        char strIn[80];
        strcpy(strIn, "../code/imagesNdg/base");
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

void mosaiquemaker::constructionBaseCouleur(int tailleBase, int h, int w)
{
    int taille, width, height;
    double moyenneR, varianceR, moyenneV, varianceV, moyenneB, varianceB;
    char *ndg = "Couleur.ppm";
    char *num;
    OCTET *Img, *ImgOut;
    OCTET *ImgR, *ImgV, *ImgB;
    for (size_t i = 1; i <= tailleBase; i++)
    {
        asprintf(&num, "%d", i);

        char strIn[80];
        strcpy(strIn, "../code/bddPPM/base");
        strcat(strIn, num);
        strcat(strIn, ndg);

        char strOut[80];
        strcpy(strOut, "../code/bddPPM/sortie");
        strcat(strOut, num);
        strcat(strOut, ndg);

        lire_nb_lignes_colonnes_image_ppm((char *)strIn, &height, &width);

        taille = width * height;
        allocation_tableau(Img, OCTET, taille * 3);
        allocation_tableau(ImgOut, OCTET, taille * 3);
        allocation_tableau(ImgR, OCTET, taille);
        allocation_tableau(ImgV, OCTET, taille);
        allocation_tableau(ImgB, OCTET, taille);
        lire_image_ppm((char *)strIn, Img, taille);

        // RECUPERATION DES COMPOSANTES
        for (int j = 0; j < taille; j++)
        {
            ImgR[j] = Img[3 * j];
            ImgV[j] = Img[3 * j + 1];
            ImgB[j] = Img[3 * j + 2];
        }

        // MODIFICATION DES COMPOSANTES
        ImgR = modifImage(ImgR, width, w);
        ImgV = modifImage(ImgV, width, w);
        ImgB = modifImage(ImgB, width, w);

        // RECONSTRUCTION DE L'IMAGE
        for (int j = 0; j < w * w; j++)
        {
            ImgOut[3 * j] = ImgR[j];
            ImgOut[3 * j + 1] = ImgV[j];
            ImgOut[3 * j + 2] = ImgB[j];
        }
        base.push_back(ImgOut);
        // base[i - 1] = modifImage(base[i - 1], width, w);
        moyenneR = moy(ImgR, w * w);
        mR.push_back(moyenneR);
        varianceR = var(ImgR, w * w, moyenneR);
        vR.push_back(varianceR);
        moyenneV = moy(ImgV, w * w);
        mV.push_back(moyenneV);
        varianceV = var(ImgV, w * w, moyenneV);
        vV.push_back(varianceV);
        moyenneB = moy(ImgB, w * w);
        mB.push_back(moyenneB);
        varianceB = var(ImgB, w * w, moyenneB);
        vB.push_back(varianceB);
        ecrire_image_ppm((char *)strOut, base[i - 1], h, w);
        free(Img);
        free(ImgR);
        free(ImgB);
        free(ImgV);
    }

}

int mosaiquemaker::choixImagette(double moyenne, double variance)
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
    }
    return min;
}

void mosaiquemaker::remplacerBloc(OCTET *ImgDepart, OCTET *ImgSortie, int tailleDep, int tailleImagette)
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


void mosaiquemaker::UneImageEnQuatre(OCTET *ImgIn, OCTET *ImgHautGauche, OCTET *ImgHautDroite, OCTET *ImgBasGauche, OCTET *ImgBasDroite, int nW, int nH)
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

OCTET *mosaiquemaker::QuatreImagesEnUne(OCTET *ImgHautGauche, OCTET *ImgHautDroite, OCTET *ImgBasGauche, OCTET *ImgBasDroite, int WFinale, int HFinale)
{

    OCTET *ImgOut;
    allocation_tableau(ImgOut, OCTET, WFinale * HFinale * 3);

    for (size_t i = 0; i < WFinale / 2; i++)
    {
        for (size_t j = 0; j < HFinale / 2; j++)
        {
            ImgOut[3 * (i * WFinale + j)] = ImgHautGauche[3 * (i * (WFinale / 2) + j)];
            ImgOut[3 * (i * WFinale + j) + 1] = ImgHautGauche[3 * (i * (WFinale / 2) + j) + 1];
            ImgOut[3 * (i * WFinale + j) + 2] = ImgHautGauche[3 * (i * (WFinale / 2) + j) + 2];
        }
    }
    for (size_t i = 0; i < WFinale / 2; i++)
    {
        for (size_t j = 0; j < HFinale / 2; j++)
        {
            ImgOut[3 * (i * WFinale + j + WFinale / 2)] = ImgHautDroite[3 * (i * (WFinale / 2) + j)];
            ImgOut[3 * (i * WFinale + j + WFinale / 2) + 1] = ImgHautDroite[3 * (i * (WFinale / 2) + j) + 1];
            ImgOut[3 * (i * WFinale + j + WFinale / 2) + 2] = ImgHautDroite[3 * (i * (WFinale / 2) + j) + 2];
        }
    }

    for (size_t i = 0; i < WFinale / 2; i++)
    {
        for (size_t j = 0; j < HFinale / 2; j++)
        {
            ImgOut[3 * ((i + WFinale / 2) * WFinale + j)] = ImgBasGauche[3 * (i * (WFinale / 2) + j)];
            ImgOut[3 * ((i + WFinale / 2) * WFinale + j) + 1] = ImgBasGauche[3 * (i * (WFinale / 2) + j) + 1];
            ImgOut[3 * ((i + WFinale / 2) * WFinale + j) + 2] = ImgBasGauche[3 * (i * (WFinale / 2) + j) + 2];
        }
    }
    for (size_t i = 0; i < WFinale / 2; i++)
    {
        for (size_t j = 0; j < HFinale / 2; j++)
        {
            ImgOut[3 * ((i + WFinale / 2) * WFinale + j + WFinale / 2)] = ImgBasDroite[3 * (i * (WFinale / 2) + j)];
            ImgOut[3 * ((i + WFinale / 2) * WFinale + j + WFinale / 2) + 1] = ImgBasDroite[3 * (i * (WFinale / 2) + j) + 1];
            ImgOut[3 * ((i + WFinale / 2) * WFinale + j + WFinale / 2) + 2] = ImgBasDroite[3 * (i * (WFinale / 2) + j) + 2];
        }
    }
    return ImgOut;
}

int mosaiquemaker::choixImagetteCouleur(double &moyenneR, double &varianceR, double &moyenneV, double &varianceV, double &moyenneB, double &varianceB)
{
    int taille = base.size();
    double ecartMoyenne, ecartVariance;
    std::vector<double> resultR(taille);
    std::vector<double> resultV(taille);
    std::vector<double> resultB(taille);
    std::vector<double> result(taille);
    double maxDif, interDif, minDif;
    for (int i = 0; i < taille; i++)
    {
        resultR[i] = abs(mR[i] - moyenneR) * abs(mR[i] - moyenneR); //+ abs(v[i]-variance)*abs(v[i]-variance);
        resultV[i] = abs(mV[i] - moyenneV) * abs(mV[i] - moyenneV);
        resultB[i] = abs(mB[i] - moyenneB) * abs(mB[i] - moyenneB);
        maxDif = std::max(resultR[i], resultV[i]);
        maxDif = std::max(maxDif, resultB[i]);
        minDif = std::min(resultR[i], resultV[i]);
        minDif = std::min(minDif, resultV[i]);
        interDif = resultR[i] + resultV[i] + resultB[i] - maxDif - minDif;
        result[i] = 3 * maxDif + 2 * interDif + minDif;
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

void mosaiquemaker::remplacerBlocCouleur(OCTET *ImgDepart, OCTET *ImgSortie, int tailleDep, int tailleImagette)
{
    int nbBlocX = tailleDep / tailleImagette;
    int nbBlocY = tailleDep / tailleImagette;
    int xDebut, xFin, yDebut, yFin;
    int indice;
    double moyenneR, varianceR, moyenneV, varianceV, moyenneB, varianceB;
    OCTET *imagette;
    int indBase;
    int index = 0;
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
                    moyenneR += ImgDepart[(k * tailleDep + l) * 3];
                    moyenneV += ImgDepart[(k * tailleDep + l) * 3 + 1];
                    moyenneB += ImgDepart[(k * tailleDep + l) * 3 + 2];
                }
            }
            moyenneR = (double)moyenneR / (tailleImagette * tailleImagette);
            moyenneV = (double)moyenneV / (tailleImagette * tailleImagette);
            moyenneB = (double)moyenneB / (tailleImagette * tailleImagette);
            for (int k = yDebut; k < yFin; k++)
            {
                for (int l = xDebut; l < xFin; l++)
                {
                    varianceR += (ImgDepart[(k * tailleDep + l) * 3] - moyenneR) * (ImgDepart[(k * tailleDep + l) * 3] - moyenneR);
                    varianceV += (ImgDepart[(k * tailleDep + l) * 3 + 1] - moyenneV) * (ImgDepart[(k * tailleDep + l) * 3 + 1] - moyenneV);
                    varianceB += (ImgDepart[(k * tailleDep + l) * 3 + 2] - moyenneB) * (ImgDepart[(k * tailleDep + l) * 3 + 2] - moyenneB);
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
                    ImgSortie[3 * (k * tailleDep + l)] = imagette[3 * indice];
                    ImgSortie[3 * (k * tailleDep + l) + 1] = imagette[3 * indice + 1];
                    ImgSortie[3 * (k * tailleDep + l) + 2] = imagette[3 * indice + 2];
                }
            }
        }
    }
}

void mosaiquemaker::remplacerBlocGif(OCTET *ImgDepart, OCTET *ImgSortie, int tailleDepW, int tailleDepH, int tailleImagette)
{
    int nbBlocX = tailleDepW / tailleImagette;
    int nbBlocY = tailleDepH / tailleImagette;
    int xDebut, xFin, yDebut, yFin;
    int indice;
    double moyenneR, varianceR, moyenneV, varianceV, moyenneB, varianceB;
    OCTET *imagette;
    int indBase;
    int index = 0;

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
                    moyenneR += ImgDepart[(k * tailleDepW + l) * 3];
                    moyenneV += ImgDepart[(k * tailleDepW + l) * 3 + 1];
                    moyenneB += ImgDepart[(k * tailleDepW + l) * 3 + 2];
                }
            }
            moyenneR = (double)moyenneR / (tailleImagette * tailleImagette);
            moyenneV = (double)moyenneV / (tailleImagette * tailleImagette);
            moyenneB = (double)moyenneB / (tailleImagette * tailleImagette);
            for (int k = yDebut; k < yFin; k++)
            {
                for (int l = xDebut; l < xFin; l++)
                {
                    varianceR += (ImgDepart[(k * tailleDepW + l) * 3] - moyenneR) * (ImgDepart[(k * tailleDepW + l) * 3] - moyenneR);
                    varianceV += (ImgDepart[(k * tailleDepW + l) * 3 + 1] - moyenneV) * (ImgDepart[(k * tailleDepW + l) * 3 + 1] - moyenneV);
                    varianceB += (ImgDepart[(k * tailleDepW + l) * 3 + 2] - moyenneB) * (ImgDepart[(k * tailleDepW + l) * 3 + 2] - moyenneB);
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
                    ImgSortie[3 * (k * tailleDepW + l)] = imagette[3 * indice];
                    ImgSortie[3 * (k * tailleDepW + l) + 1] = imagette[3 * indice + 1];
                    ImgSortie[3 * (k * tailleDepW + l) + 2] = imagette[3 * indice + 2];
                }
            }
        }

    }
}

OCTET *mosaiquemaker::conversionRGBtoNDG(OCTET *ImgCouleur, OCTET *ImgNdg, int taille)
{
    for (int i = 0; i < taille; i++)
    {
        ImgNdg[i] = (0.299 * ImgCouleur[i * 3]) + (0.587 * ImgCouleur[i * 3 + 1]) + (0.114 * ImgCouleur[i * 3 + 2]);
    }
    return ImgNdg;
}

OCTET *mosaiquemaker::seuilAuto(OCTET *ImgGradient, OCTET *sortie, int taille)
{
    int max = 0;
    for (int i = 0; i < taille; i++)
    {
        if (ImgGradient[i] > max)
        {
            max = ImgGradient[i];
        }
    }
    int seuil = max / 2;
    for (int i = 0; i < taille; i++)
    {
        if (ImgGradient[i] > seuil)
        {
            sortie[i] = 255;
        }
        else
        {
            sortie[i] = 0;
        }
    }
    return sortie;
}

OCTET *mosaiquemaker::conversionGradient(OCTET *ImgNDG, OCTET *gradient, int w, int h)
{
    int Ii, Ij;
    for (int i = 0; i < h - 1; i++)
    {
        for (int j = 0; j < w - 1; j++)
        {
            Ij = ImgNDG[i * w + j + 1] - ImgNDG[i * w + j];
            Ii = ImgNDG[(i + 1) * w + j] - ImgNDG[i * w + j];
            gradient[i * w + j] = sqrt(Ii * Ii + Ij * Ij);
        }
    }
    int *OCCGRIS;
    allocation_tableau(OCCGRIS, int, 256);
    // FILE* histo=fopen("./code/histo.dat","w");
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
        {
            OCCGRIS[gradient[i * w + j]] += 1;
        }
    /* for(int i=0;i<256;i++){
      fprintf(histo,"%d %d\n",i, OCCGRIS[i]);
  } */
    int min = 0;
    int max = 255;
    bool trouve = false;
    while ((trouve == false) && (min < 256))
    {
        if (OCCGRIS[min] != 0)
        {
            trouve = true;
        }
        else
        {
            min++;
        }
    }
    trouve = false;
    while ((trouve == false) && (max > 0))
    {
        if (OCCGRIS[max] != 0)
        {
            trouve = true;
        }
        else
        {
            max--;
        }
    }
    double alpha, beta;
    alpha = (-255 * min) / (max - min);
    beta = 255 / (max - min);
    for (int i = 0; i < w * h; i++)
    {
        gradient[i] = ((-255) / (max - min)) * (min - gradient[i]);
    }
    /* for(int i = 0 ; i<w*h ; i++){
    if(gradient[i]>150){
      gradient[i] = 255 ;
    }
    else{
      gradient[i] = 0 ;
    }
  }*/
    // gradient = seuilAuto(gradient, gradient, w*h);
    return gradient;
}

int mosaiquemaker::indice(OCTET *imageGradient, int taille)
{
    int tailleBase = base.size();
    std::vector<int> vect(tailleBase);
    int diff;
    int minDif = 255 * taille;
    int indice = 0;
    for (int i = 0; i < tailleBase; i++)
    {
        diff = 0;
        for (int j = 0; j < taille; j++)
        {
            diff += abs(imageGradient[j] - baseGradient[i][j]);
        }
        vect[i] = diff;
        if (diff < minDif)
        {
            minDif = diff;
            indice = i;
        }
    }
    return indice;
}

OCTET* mosaiquemaker::visage(char cNomImgLue[250], char cNomImgEcrite[250])
{
    int taille_des_imagettes = 128 ;
    constructionBaseCouleur(1000, taille_des_imagettes, taille_des_imagettes);
    int length = base.size();
    int nH, nW, nTaille, nTaille3;
    OCTET *Image;
    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    allocation_tableau(Image, OCTET, nH * nW * 3);
    lire_image_ppm(cNomImgLue, Image, nH * nW);

    // Division en 4 des imagettes
    OCTET *hg, *hd, *bg, *bd;
    taille_des_imagettes = nW * nH / 4;
    for (int i = 0; i < 1000; i++)
    {
        allocation_tableau(hg, OCTET, nW * nH * 3 / 4);
        allocation_tableau(hd, OCTET, nW * nH * 3 / 4);
        allocation_tableau(bg, OCTET, nW * nH * 3 / 4);
        allocation_tableau(bd, OCTET, nW * nH * 3 / 4);
        UneImageEnQuatre(base[i], hg, hd, bg, bd, nH, nW);
        baseQuart.push_back(hg);
        baseQuart.push_back(hd);
        baseQuart.push_back(bg);
        baseQuart.push_back(bd);
    }

    base = baseQuart;

    int taille = base.size();
    baseNdG.resize(taille);
    baseGradient.resize(taille);
    // NIVEAUX DE GRIS
    OCTET *modif;

    for (int i = 0; i < taille; i++)
    {
        allocation_tableau(modif, OCTET, 64 * 64);
        baseNdG[i] = conversionRGBtoNDG(base[i], modif, nH * nW / 4);
    }

    // GRADIENT
    OCTET *modifGradient;
    for (int i = 0; i < taille; i++)
    {
        allocation_tableau(modifGradient, OCTET, nH * nW);
        baseGradient[i] = conversionGradient(baseNdG[i], modifGradient, nH, nW);
    }

    //DIVISION DE L'IMAGE DE DEPART
    OCTET *testHG, *testHD, *testBG, *testBD ;
    allocation_tableau(testHG, OCTET, 64*64*3);
    allocation_tableau(testHD, OCTET, 64*64*3);
    allocation_tableau(testBG, OCTET, 64*64*3);
    allocation_tableau(testBD, OCTET, 64*64*3);
    UneImageEnQuatre(Image, testHG, testHD, testBG, testBD,  nH, nW);

    //NIVEAUX DE GRIS
    OCTET *testHGndg, *testHDndg, *testBGndg, *testBDndg ;
    //OCTET *testHGndgB, *testHDndgB, *testBGndgB, *testBDndgB ;
    allocation_tableau(testHGndg, OCTET, nH*nW/4);
    allocation_tableau(testHDndg, OCTET, nH*nW/4);
    allocation_tableau(testBGndg, OCTET, nH*nW/4);
    allocation_tableau(testBDndg, OCTET, nH*nW/4);
    testHGndg = conversionRGBtoNDG(testHG, testHGndg, nH*nW/4);
    testHDndg = conversionRGBtoNDG(testHD, testHDndg, nH*nW/4);
    testBGndg = conversionRGBtoNDG(testBG, testBGndg, nH*nW/4);
    testBDndg = conversionRGBtoNDG(testBD, testBDndg, nH*nW/4);

    //GRADIENT
    OCTET *testHGgradient, *testHDgradient, *testBGgradient, *testBDgradient ;
    allocation_tableau(testHGgradient, OCTET, nH*nW/4);
    allocation_tableau(testHDgradient, OCTET, nH*nW/4);
    allocation_tableau(testBGgradient, OCTET, nH*nW/4);
    allocation_tableau(testBDgradient, OCTET, nH*nW/4);
    testHGgradient = conversionGradient(testHGndg, testHGgradient, nH/2, nW/2);
    testHDgradient = conversionGradient(testHDndg, testHDgradient, nH/2, nW/2);
    testBGgradient = conversionGradient(testBGndg, testBGgradient, nH/2, nW/2);
    testBDgradient = conversionGradient(testBDndg, testBDgradient, nH/2, nW/2);

    //RECUPERATION DES BONNES IMAGETTES
    int ind = indice(testHGgradient, nH*nW/4);
    int ind1 = indice(testHDgradient, nH*nW/4);
    int ind2 = indice(testBGgradient, nH*nW/4);
    int ind3 = indice(testBDgradient, nH*nW/4);
    OCTET *ImageOut;
    allocation_tableau(ImageOut, OCTET, nH*nW*3);
    ImageOut = QuatreImagesEnUne(base[ind], base[ind1], base[ind2], base[ind3], nH, nW);
    ecrire_image_ppm(cNomImgEcrite, ImageOut, nH, nW);
}

OCTET *mosaiquemaker::mosaiqueClassique(char cNomImgLue[250], char cNomImgEcrite[250], int taille_des_imagettes){
    constructionBaseCouleur(1000, taille_des_imagettes, taille_des_imagettes);

    int length = base.size();
    int nH, nW, nTaille, nTaille3;
    OCTET *Image;
    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    OCTET *ImageOut;
    allocation_tableau(Image, OCTET, nH * nW * 3);
    allocation_tableau(ImageOut, OCTET, nH * nW * 3);
    lire_image_ppm(cNomImgLue, Image, nH * nW);
    remplacerBlocGif(Image, ImageOut, nW,nH, taille_des_imagettes);

    ecrire_image_ppm(cNomImgEcrite, ImageOut, nH, nW);
}

void mosaiquemaker::gif(QStringList args)
{
    QProcess myProcess;

    // Connect the finished signal to a slot
    //connect(myProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onProcessFinished(int, QProcess::ExitStatus)));

    // Start the process
    myProcess.start("../gif", args);

    myProcess.waitForFinished(-1);
}

int mosaiquemaker::makeMosaique(int choix,char* cNomImgLue, char* cNomImgEcrite,MainWindow *main, int tailleImagette)
{
    int res;
    int tailleImg;
    if(choix==1){
        if(tailleImagette==-1){
            //Demande la taille des mosaiques à l'utilisateur
            QDialog dialog;
            dialog.setStyleSheet("background-color: #424340;color:#FFFFFF;");

            dialog.setWindowTitle("Taille Imagettes");
            QVBoxLayout layout(&dialog) ;



            QLabel text;
            text.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

            text.setText("Sélectionnez la taille des imagettes utilisées pour votre mosaïque : ");
            layout.addWidget(&text);

            QComboBox comboBox;
            comboBox.setFixedWidth(100);

            comboBox.addItem("2x2");
            comboBox.addItem("4x4");
            comboBox.addItem("8x8");
            comboBox.addItem("16x16");
            comboBox.addItem("32x32");
            comboBox.addItem("64x64");
            comboBox.addItem("128x128");
            comboBox.addItem("256x256");

            layout.addWidget(&comboBox);

            QPushButton ok;
            ok.setText("OK");

            ok.setFixedWidth(50);
            layout.addWidget(&ok);

            layout.addWidget(&text, 0, Qt::AlignHCenter);
            layout.addWidget(&comboBox, 0, Qt::AlignHCenter);
            layout.addWidget(&ok, 0, Qt::AlignHCenter);
            dialog.connect(&ok, &QPushButton::clicked, &dialog, &QDialog::accept);

            // Affichage du QDialog
            res=dialog.exec();
            tailleImg=comboBox.currentIndex();
        }else{
            res=QDialog::Accepted;
            tailleImg=tailleImagette;
        }
        if (res == QDialog::Accepted) {

            switch (tailleImg) {
            case 0:
                mosaiqueClassique(cNomImgLue, cNomImgEcrite, 2);
                break;
            case 1:
                mosaiqueClassique(cNomImgLue, cNomImgEcrite, 4);

                break;
            case 2:
                mosaiqueClassique(cNomImgLue, cNomImgEcrite, 8);

                break;
            case 3:
                mosaiqueClassique(cNomImgLue, cNomImgEcrite, 16);

                break;
            case 4:
                mosaiqueClassique(cNomImgLue, cNomImgEcrite, 32);

                break;
            case 5:
                mosaiqueClassique(cNomImgLue, cNomImgEcrite, 64);

                break;
            case 6:
                mosaiqueClassique(cNomImgLue, cNomImgEcrite, 128);

                break;
            case 7:
                mosaiqueClassique(cNomImgLue, cNomImgEcrite, 256);

                break;

            default:
                break;
            }

            return 1;
        }else if(res==QDialog::Rejected){
            return -1;
        }

    }else if(choix==2){
        visage(cNomImgLue, cNomImgEcrite);
        return 1;
    }
}
