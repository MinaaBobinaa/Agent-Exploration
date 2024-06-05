/*  INF3105 - Structures de données et algorithmes
    UQAM | Faculté des sciences | Département d'informatique
    TP2 -- http://cria2.uqam.ca/INF3105/tp2
    © Jaël Champagne Gareau.
*/

#include "salle.h"

#include <cassert>
#include <iostream>

using namespace std;

Coordonnees::Coordonnees(int _salle, int _ligne, int _colonne)
    : salle(_salle), ligne(_ligne), colonne(_colonne) {}

int Coordonnees::getSalle() const {
  return salle;
}

bool Coordonnees::operator==(const Coordonnees& c) const {
  return salle == c.salle && ligne == c.ligne && colonne == c.colonne;
}

istream& operator>>(istream& is, Coordonnees& c) {
  is >> c.salle >> c.ligne >> c.colonne;
  return is;
}

Salle::Salle() {}

Salle::Salle(const string& nomType, int _dimension) : dimension(_dimension) {
  if (nomType == "carre")
    type = Carre;
  else if (nomType == "diamant")
    type = Diamant;
  else if (nomType == "triangle")
    type = Triangle;
  else
    assert(false && "nomType est invalide");

  for (int i = 0; i < obtenirNbCases(); ++i)
    cellules.ajouter(Cellule());
}

bool Salle::estPositionValide(const Coordonnees& p) const {
  return obtenirIndice(p.ligne, p.colonne) != -1;
}

const Coordonnees& Salle::obtenirPortail(const Coordonnees& p) const {
  return cellules[obtenirIndice(p.ligne, p.colonne)].portail;
}

bool& Salle::marqueurVisite(const Coordonnees& p) const {
  return cellules[obtenirIndice(p.ligne, p.colonne)].visite;
}

void Salle::placerPortailCellule(const Coordonnees& de,
                                 const Coordonnees& vers) {
  cellules[obtenirIndice(de.ligne, de.colonne)].portail = vers;
}

int Salle::obtenirNbCases() const {
  if (type == Carre)
    return dimension * dimension;

  else if (type == Diamant) {
    int size = 0;
    for (int i = 0; i < dimension; i++){
      for (int j = 0; j < dimension; ++j){
        if (abs(i - dimension / 2) + abs(j - dimension / 2) <= dimension / 2){
          ++size;
        }
      }
    }
    return size;
  }

  else if (type == Triangle) {
    int size = 0;
    for (int i = 0; i <= dimension / 2; ++i) {
      size += (2 * i + 1);
    }
    return size;
  }

  return -1;  // Erreur
}

int Salle::obtenirIndice(int ligne, int colonne) const {
  if (type == Carre) {
    if (ligne < 0 || ligne >= dimension || colonne < 0 || colonne >= dimension)
      return -1;
    return (ligne * dimension) + colonne;
  } else if (type == Diamant) {
    if (ligne < 0 || ligne >= dimension || colonne < 0 || colonne >= dimension)
      return -1;
    if (abs(ligne - dimension / 2) + abs(colonne - dimension / 2) > dimension / 2)
      return -1;
    int index = 0;
    for (int i = 0; i < ligne; ++i) {
      for (int j = 0; j < dimension; ++j) {
        if (abs(i - dimension / 2) + abs(j - dimension / 2) <= dimension / 2) {
          ++index;
        }
      }
    }
    for (int j = 0; j < colonne; ++j) {
      if (abs(ligne - dimension / 2) + abs(j - dimension / 2) <= dimension / 2) {
        ++index;
      }
    }
    return index;
  } else if (type == Triangle) {
    if (ligne < 0 || ligne > dimension / 2 || colonne < 0 || colonne >= dimension)
      return -1;
    if (colonne < dimension / 2 - ligne || colonne > dimension / 2 + ligne)
      return -1;
    int index = 0;
    for (int i = 0; i < ligne; ++i) {
      index += (2 * i + 1);
    }
    index += (colonne - (dimension / 2 - ligne));
    return index;
  }

  return -1;  // type invalide
}
