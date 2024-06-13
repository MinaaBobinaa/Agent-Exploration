/*  INF3105 - Structures de données et algorithmes
    UQAM | Faculté des sciences | Département d'informatique
    TP2 -- http://cria2.uqam.ca/INF3105/tp2
    © Jaël Champagne Gareau.
*/

#ifndef UNIVERS_H
#define UNIVERS_H

#include <iostream>

#include "salle.h"
#include "tableau.h"

class Univers {
 public:
  int calculerNombreChemins(const Coordonnees&, const Coordonnees&) const;

  friend std::istream& operator>>(std::istream&, Univers&);
  const Salle& getSalle(int index) const { return salles[index]; }

 private:
  int rechercherChemin(const Coordonnees& courant, const Coordonnees& arrivee,
            Tableau<Coordonnees>& visite, int& indexVisite) const;
  Tableau<Salle> salles;
};

#endif
