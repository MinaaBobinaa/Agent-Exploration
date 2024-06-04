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
  // À compléter
  return false;
}

istream& operator>>(istream& is, Coordonnees& c) {
  // À compléter
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
    return 0;  // À compléter
  }

  else if (type == Triangle) {
    return 0;  // À compléter
  }

  return -1;  // Erreur
}

int Salle::obtenirIndice(int ligne, int colonne) const {
  if (type == Carre) {
    if (ligne < 0 || ligne >= dimension || colonne < 0 || colonne >= dimension)
      return -1;
    return (ligne * dimension) + colonne;
  } else if (type == Diamant) {
    // À compléter
  } else if (type == Triangle) {
    // À compléter
  }

  return -1;  // type invalide
}
