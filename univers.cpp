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
  
  if (!getSalle(depart.getSalle()).estPositionValide(depart)) {
    return -1; // Cellule de départ invalide
  }

  if (!getSalle(arrivee.getSalle()).estPositionValide(arrivee)) {
    return -1;
  }

  Tableau<Coordonnees> visite(10000);
  int indexVisite = 0;
  return rechercherChemin(depart, arrivee, visite, indexVisite);
}


int Univers::rechercherChemin(const Coordonnees& courant, const Coordonnees& arrivee,
                 Tableau<Coordonnees>& visite, int& indexVisite) const {

  if (courant == arrivee) {
    return 1;
  }

  for (int i = 0; i < indexVisite; ++i) {
    if (visite[i] == courant) {
      return 0; // Déjà visité
    }
  }

  // Ajoute courant à visite
  visite[indexVisite++] = courant;

  int chemins = 0;
  const Salle& salle = getSalle(courant.getSalle());

  // Directions adjacentes
  Coordonnees directions[4] = {
    Coordonnees(courant.getSalle(), courant.getLigne() - 1, courant.getColonne()),
    Coordonnees(courant.getSalle(), courant.getLigne() + 1, courant.getColonne()),
    Coordonnees(courant.getSalle(), courant.getLigne(), courant.getColonne() - 1),
    Coordonnees(courant.getSalle(), courant.getLigne(), courant.getColonne() + 1)
  };

  // Explore les mouvements adjacents
  for (int i = 0; i < 4; ++i) {
    if (salle.estPositionValide(directions[i])) {
      chemins += rechercherChemin(directions[i], arrivee, visite, indexVisite);
    }
  }

  // Explore les portails
  const Coordonnees& portail = salle.getPortail(courant);
  if (portail.getSalle() != -1) {
    chemins += rechercherChemin(portail, arrivee, visite, indexVisite);
  }

  --indexVisite;

  return chemins;
}


istream& operator>>(istream& is, Univers& univers) {

  univers.salles.vider();
  
  while (true) {
    string type;
    is >> type;
    if (type == "-----") break;  // fin de la lecture des types de salle

    int dim;
    is >> dim;

    Salle salle(type, dim);
    univers.salles.ajouter(salle);
  }

  // Lecture des portails
  while (!(is >> ws).eof()) {
    Coordonnees c1, c2;

    is >> c1;

    string fleche;
    is >> fleche;
    assert(fleche == "<-->");

    is >> c2;

    univers.salles[c1.getSalle()].placerPortailCellule(c1, c2);
    univers.salles[c2.getSalle()].placerPortailCellule(c2, c1);

  }

  return is;
}
