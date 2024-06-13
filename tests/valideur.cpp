/*  INF3105 - Structures de données et algorithmes
    UQAM | Faculté des sciences | Département d'informatique
    TP2 -- http://cria2.uqam.ca/INF3105/tp2
    © Jaël Champagne Gareau.
*/

#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
  if (argc < 3) {
    cout << "Syntaxe : ./valideur votre-reqN+.txt solution-reqN+.txt " << endl;
    return 1;
  }

  ifstream fsVotre(argv[1]);
  if (fsVotre.fail()) {
    cout << "Échec d'ouverture: " << argv[1] << endl;
    return 2;
  }

  ifstream fsSolution(argv[2]);
  if (fsSolution.fail()) {
    cout << "Échec d'ouverture: " << argv[2] << endl;
    return 3;
  }

  int nbReqs = 0;
  int nbExistenceOK = 0;
  int nbCheminsOK = 0;
  while (!(fsSolution >> ws).eof() && !(fsVotre >> ws).eof()) {
    ++nbReqs;

    int repVotre;
    int repSol;
    fsVotre >> repVotre;
    fsSolution >> repSol;

    if ((repSol == -1 && repVotre == -1) || (repSol == 0 && repVotre == 0)
        || (repSol > 0 && repVotre > 0))
      ++nbExistenceOK;  // critère C1/C2

    if (repSol == repVotre) ++nbCheminsOK;  // critère C3/c4
  }

  fsSolution.close();
  fsVotre.close();

  cout << "Nombre de requêtes dans les fichiers: " << nbReqs << '\n';
  cout << "nbReqs\tnbExistenceOK\tnbCheminsOK\n";
  cout << nbReqs << '\t' << nbExistenceOK << '\t' << nbCheminsOK << endl;

  return 0;
}
