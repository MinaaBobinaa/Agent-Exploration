/*  INF3105 - Structures de données et algorithmes
    UQAM | Faculté des sciences | Département d'informatique
    TP2 -- http://cria2.uqam.ca/INF3105/tp2
    © Jaël Champagne Gareau.
*/

#include "univers.h"

#include <cassert>

using namespace std;

int Univers::calculerNombreChemins(const Coordonnees& depart,
                                   const Coordonnees& arrivee) const {
  // À compléter
  return 0;
}

istream& operator>>(istream& is, Univers& univers) {
  while (true) {
    string type;
    is >> type;
    if (type == "-----") break;  // fin de la lecture des types de salle

    int dim;
    is >> dim;

    // À compléter
  }

  // Lecture des portails
  while (!(is >> ws).eof()) {
    Coordonnees c1, c2;

    is >> c1;

    string fleche;
    is >> fleche;
    assert(fleche == "<-->");

    is >> c2;

    // À compléter
  }

  return is;
}
