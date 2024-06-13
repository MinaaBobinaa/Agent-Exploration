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

int Coordonnees::getLigne() const {
  return ligne;
}

int Coordonnees::getColonne() const {
  return colonne;
}

bool Coordonnees::operator==(const Coordonnees& c) const {
  return salle == c.salle && ligne == c.ligne && colonne == c.colonne;
}

istream& operator>>(istream& is, Coordonnees& c) {
  char ch1, ch2, ch3;
  is >> c.salle >> ch1 >> c.ligne >> ch2 >> c.colonne >> ch3;
  return is;
}

ostream& operator<<(std::ostream& os, const Coordonnees& coord) {
    os << coord.salle << " (" << coord.ligne << "," << coord.colonne << ")";
    return os;
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

int Salle::getDimension() const {
  return dimension;
}

TypeSalle Salle::getType() const {
 return type; 
}


const Coordonnees& Salle::getPortail(const Coordonnees& p) const {
    int indice = obtenirIndice(p.ligne, p.colonne);
    if (indice != -1 && indice < cellules.taille()) {
        const Coordonnees& portail = cellules[indice].portail;
        if (portail.getSalle() != -1) {
            return portail;
        }
    }
    // Retourne des coordonnées invalides si aucun portail n'est trouvé
    static Coordonnees coordInvalides(-1, -1, -1);
    return coordInvalides;
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
    int half = dimension / 2;
    return 2 * half * (half + 1) + 1;
  }

  else if (type == Triangle) {
    int total = 0;
        int row_length = dimension;
        for (int i = 0; i < dimension; ++i) {
            total += row_length;
            row_length -= 2;
            if (row_length < 1) break;
        }
        return total; 
  }

  return -1;  // Erreur
}

int Salle::obtenirIndice(int ligne, int colonne) const {
  if (type == Carre) {
    return obtenirIndiceCarre(ligne, colonne);
  } else if (type == Diamant) {
    return obtenirIndiceDiamant(ligne, colonne);
  } else if (type == Triangle) {
    return obtenirIndiceTriangle(ligne, colonne);
  }

  return -1;  // type invalide
}

int Salle::obtenirIndiceCarre(int ligne, int colonne) const {
  if (ligne < 0 || ligne >= dimension || colonne < 0 || colonne >= dimension)
    return -1;
  return (ligne * dimension) + colonne;
}

int Salle::obtenirIndiceDiamant(int ligne, int colonne) const {
  int n = dimension;
  int half = n / 2;
  int index = 0;

  if (ligne < 0 || ligne >= n || colonne < 0 || colonne >= n)
    return -1;

  if (ligne <= half) {
    if (colonne < half - ligne || colonne > half + ligne)
      return -1;

    for (int i = 0; i < ligne; ++i) {
      index += 2 * i + 1;
    }

    index += colonne - (half - ligne);
  } else {
    if (colonne < ligne - half || colonne > 3 * half - ligne)
      return -1;

    for (int i = 0; i <= half; ++i) {
      index += 2 * i + 1;
    }
    for (int i = half + 1; i < ligne; ++i) {
      index += 2 * (n - i - 1) + 1;
    }

    index += colonne - (ligne - half);
  }

  return index;
}

int Salle::obtenirIndiceTriangle(int ligne, int colonne) const {
  int n = dimension;
  int mid = n / 2;
  if (ligne < 0 || ligne >= n - mid || colonne < ligne || colonne >= n - ligne)
    return -1;

  int index = 0;
  for (int i = 0; i < ligne; ++i) {
    index += n - 2 * i;
  }

  index += colonne - ligne;

  return index;
}
