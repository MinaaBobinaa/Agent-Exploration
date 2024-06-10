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
  Tableau<Coordonnees> visite(100000); // Taille initiale du tableau dynamique
  int indexVisite = 0;
  return dfs(depart, arrivee, visite, indexVisite);
}

int Univers::dfs(const Coordonnees& courant, const Coordonnees& arrivee,
                 Tableau<Coordonnees>& visite, int& indexVisite) const {
    if (courant == arrivee) {
        return 1; // Chemin trouvé
    }

    // Vérifie si courant est dans visite
    for (int i = 0; i < indexVisite; ++i) {
        if (visite[i] == courant) {
            return 0; // Déjà visité
        }
    }

    // Ajoute courant à visite
    visite[indexVisite++] = courant;

    int chemins = 0;
    const Salle& salle = getSalle(courant.getSalle());

    // Directions adjacentes (up, down, left, right)
    Coordonnees directions[4] = {
        Coordonnees(courant.getSalle(), courant.getLigne() - 1, courant.getColonne()),
        Coordonnees(courant.getSalle(), courant.getLigne() + 1, courant.getColonne()),
        Coordonnees(courant.getSalle(), courant.getLigne(), courant.getColonne() - 1),
        Coordonnees(courant.getSalle(), courant.getLigne(), courant.getColonne() + 1)
    };

    // Explore les mouvements adjacents
    for (int i = 0; i < 4; ++i) {
        if (salle.estPositionValide(directions[i])) {
            chemins += dfs(directions[i], arrivee, visite, indexVisite);
        }
    }

    // Explore les portails
    const Coordonnees& portail = salle.getPortail(courant);
    if (portail.getSalle() != -1) {
        chemins += dfs(portail, arrivee, visite, indexVisite);
    }

    // Retire courant de visite
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

    // À compléter
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

    // À compléter
    // std::cout << "Ajout du portail : " << c1 << " <--> " << c2 << "\n";

    univers.salles[c1.getSalle()].placerPortailCellule(c1, c2);
    univers.salles[c2.getSalle()].placerPortailCellule(c2, c1);

    // // Affichage du portail de c1 après l'ajout
    //     const Coordonnees& portailC1 = univers.salles[c1.getSalle()].getPortail(c1);
    //     std::cout << "Portail de " << c1 << " mène à " << portailC1 << "\n";
    //     const Coordonnees& portailC2 = univers.salles[c2.getSalle()].getPortail(c2);
    //     std::cout << "Portail de " << c2 << " mène à " << portailC2 << "\n";

  }

  return is;
}
