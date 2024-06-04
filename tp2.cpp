/*  INF3105 - Structures de données et algorithmes
    UQAM | Faculté des sciences | Département d'informatique
    TP2 -- http://cria2.uqam.ca/INF3105/tp2
    © Jaël Champagne Gareau.
*/

#include <cassert>
#include <fstream>
#include <iostream>

#include "univers.h"

using namespace std;

void traiterRequetes(const Univers& univers, istream& is) {
  while (!(is >> ws).eof()) {
    Coordonnees c1, c2;
    string fleche;

    is >> c1 >> fleche >> c2;
    assert(fleche == "-->");

    cout << univers.calculerNombreChemins(c1, c2) << endl;
  }
}

int main(int argc, char* argv[]) {
  if (argc < 2 || argc > 3) {
    cerr << "Usage: ./tp2 univers.txt [requetes.txt]" << endl;
    return EXIT_FAILURE;
  }

  // Lecture du fichier univers
  ifstream funivers(argv[1]);
  if (!funivers.is_open()) {
    cerr << "Erreur d'ouverture du fichier: " << argv[1] << endl;
    return EXIT_FAILURE;
  }
  Univers univers;
  funivers >> univers;

  // Lecture des requêtes
  if (argc == 3) {
    ifstream frequetes(argv[2]);
    if (!frequetes.is_open()) {
      cerr << "Erreur d'ouverture du fichier: " << argv[2] << endl;
      return EXIT_FAILURE;
    }
    traiterRequetes(univers, frequetes);
  } else
    traiterRequetes(univers, cin);

  return EXIT_SUCCESS;
}
