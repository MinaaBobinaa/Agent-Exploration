/**
 * Squelette de code d'une matrice générique
 * INF3105 - Structures de données et algorithmes
 * UQAM / Département d'informatique
 */
#ifndef __MATRICE_H__
#define __MATRICE_H__

#include <iostream>
#include <cassert>

#include "tableau.h"

template<class T>
class Matrice {
    public:
    // Constructeurs
    Matrice(int lignes = 1, int colonnes = 1);
    Matrice(const Matrice<T>& autre);                 
    ~Matrice();                                                             

    Matrice<T> t() const;    // Transposition de la matrice
    void ajouterLigne(int position, const T& valeur = T());

    // Opérateurs d'accès ()
    T& operator()(int i, int j);
    const T& operator()(int i, int j) const;

    // Opérateurs
    bool operator==(const Matrice<T>& autre) const;
    bool operator!=(const Matrice<T>& autre) const;
    Matrice<T> operator+(const Matrice<T>& autre) const;
    Matrice<T> operator*(const Matrice<T>& autre) const;
    Matrice<T>& operator=(const Matrice<T>& autre);                 

    Matrice<T> multiplyIJK(const Matrice<T>& autre) const;
    Matrice<T> multiplyIKJ(const Matrice<T>& autre) const;
    Matrice<T> multiplyJIK(const Matrice<T>& autre) const;
    Matrice<T> multiplyJKI(const Matrice<T>& autre) const;
    Matrice<T> multiplyKIJ(const Matrice<T>& autre) const;
    Matrice<T> multiplyKJI(const Matrice<T>& autre) const;

    friend std::istream& operator>>(std::istream& is, Matrice<T>& matrice) {
        int lig, col;
        is >> lig >> col;
        matrice = Matrice<T>(lig, col);

        for (int i = 0; i < lig; ++i) {
            for (int j = 0; j < col; ++j) {
                is >> matrice(i, j);
            }
        }

        return is;
    }

 private:
    T** elements;
    int lignes, colonnes;

    void allouerMemoire();                                 
    void libererMemoire();                                 
    void copierElements(const Matrice<T>& autre); 
    void initialiserMatrice();
    void validerMultiplicationDimensions(const Matrice<T>& autre) const;


    // Pour permettre aux tests d'accéder aux méthodes privées à comparer
    friend void evalEmpirique(); // Ne pas enlever.
};

template <class T>
Matrice<T>::Matrice(int lignes, int colonnes) : lignes(lignes), colonnes(colonnes) {
    allouerMemoire();
}

template <class T>
Matrice<T>::Matrice(const Matrice<T>& autre) : lignes (autre.lignes), colonnes(autre.colonnes) {
    allouerMemoire();
    copierElements(autre);
}

template <class T>
Matrice<T>::~Matrice() {
    libererMemoire();
}

template <class T>
void Matrice<T>::allouerMemoire() {
    elements = new T*[lignes];
    for (int i = 0; i < lignes; i++){
        elements[i] = new T [colonnes];
    }
}

template <class T>
void Matrice<T>::libererMemoire() {
    for (int i = 0; i < lignes; i++){
        delete[] elements [i];
    }
    delete[] elements;
} 

template <class T>
void Matrice<T>::copierElements(const Matrice<T>& autre) {
    for (int i = 0; i < lignes; ++i) {
        for (int j = 0; j < colonnes; ++j) {
            elements[i][j] = autre.elements[i][j];
        }
    }
}

template<class T>
void Matrice<T>::initialiserMatrice() {
    for (int i = 0; i < lignes; ++i) {
        for (int j = 0; j < colonnes; ++j) {
            elements[i][j] = T();    
        }
    }
}

template<class T>
void Matrice<T>::validerMultiplicationDimensions(const Matrice<T>& autre) const {
    assert(colonnes == autre.lignes && "Le nombre de colonnes de la première matrice doit être égal au nombre de lignes de la deuxième matrice.");
    (void)autre;
}

template<class T>
T& Matrice<T>::operator()(int i, int j) {
    return elements[i][j];
}

template<class T>
const T& Matrice<T>::operator()(int i, int j) const {
    return elements[i][j];
}

template<class T>
bool Matrice<T>::operator==(const Matrice<T>& autre) const {
    if (lignes != autre.lignes || colonnes != autre.colonnes) {
        return false;
    }
    for (int i = 0; i < lignes; ++i) {
        for (int j = 0; j < colonnes; ++j) {
            if (elements[i][j] != autre.elements[i][j]) {
                return false;
            }
        }
    }
    return true;
}

template<class T>
bool Matrice<T>::operator!=(const Matrice<T>& autre) const{
    return !(*this == autre);
}

template<class T>
Matrice<T> Matrice<T>::operator+(const Matrice<T>& autre) const{
    assert(lignes == autre.lignes && colonnes == autre.colonnes && "Les dimensions des matrices doivent être égales pour l'addition.");

    Matrice<T> resultat(lignes, colonnes);
    for (int i = 0; i < lignes; i++){
        for (int j = 0; j < colonnes; j++){
            resultat(i, j) = elements[i][j] + autre.elements[i][j];
        }
    }

    return resultat;

}

template<class T>
Matrice<T> Matrice<T>::operator*(const Matrice<T>& autre) const{
    return multiplyIKJ(autre);
}


template<class T>
Matrice<T> Matrice<T>::multiplyIJK(const Matrice<T>& autre) const {
    validerMultiplicationDimensions(autre);

    Matrice<T> resultat(lignes, autre.colonnes);
    for (int i = 0; i < lignes; ++i) {
        for (int j = 0; j < autre.colonnes; ++j) {
            resultat(i, j) = T();
            for (int k = 0; k < colonnes; ++k) {
                resultat(i, j) += elements[i][k] * autre.elements[k][j];
            }
        }
    }

    return resultat;
}


template<class T>
Matrice<T> Matrice<T>::multiplyIKJ(const Matrice<T>& autre) const {
    validerMultiplicationDimensions(autre);

    Matrice<T> resultat(lignes, autre.colonnes);
    resultat.initialiserMatrice();

    for (int i = 0; i < lignes; ++i) {
        for (int k = 0; k < colonnes; ++k) {
            for (int j = 0; j < autre.colonnes; ++j) {
                resultat(i, j) += elements[i][k] * autre.elements[k][j];
            }
        }
    }

    return resultat;
}


template<class T>
Matrice<T> Matrice<T>::multiplyJIK(const Matrice<T>& autre) const {
    validerMultiplicationDimensions(autre);

    Matrice<T> resultat(lignes, autre.colonnes);
    for (int j = 0; j < autre.colonnes; ++j) {
        for (int i = 0; i < lignes; ++i) {
            resultat(i, j) = T();
            for (int k = 0; k < colonnes; ++k) {
                resultat(i, j) += elements[i][k] * autre.elements[k][j];
            }
        }
    }

    return resultat;
}


template<class T>
Matrice<T> Matrice<T>::multiplyJKI(const Matrice<T>& autre) const {
    validerMultiplicationDimensions(autre);

    Matrice<T> resultat(lignes, autre.colonnes);
    resultat.initialiserMatrice();

    for (int j = 0; j < autre.colonnes; ++j) {
        for (int k = 0; k < colonnes; ++k) {
            for (int i = 0; i < lignes; ++i) {
                resultat(i, j) += elements[i][k] * autre.elements[k][j];
            }
        }
    }

    return resultat;
}


template<class T>
Matrice<T> Matrice<T>::multiplyKIJ(const Matrice<T>& autre) const {
    validerMultiplicationDimensions(autre);

    Matrice<T> resultat(lignes, autre.colonnes);
    for (int k = 0; k < colonnes; ++k) {
        for (int i = 0; i < lignes; ++i) {
            for (int j = 0; j < autre.colonnes; ++j) {
                if (k == 0) {
                    resultat(i, j) = T(); 
                }
                resultat(i, j) += elements[i][k] * autre.elements[k][j];
            }
        }
    }

    return resultat;
}


template<class T>
Matrice<T> Matrice<T>::multiplyKJI(const Matrice<T>& autre) const {
    validerMultiplicationDimensions(autre);

    Matrice<T> resultat(lignes, autre.colonnes);
    for (int k = 0; k < colonnes; ++k) {
        for (int j = 0; j < autre.colonnes; ++j) {
            for (int i = 0; i < lignes; ++i) {
                if (k == 0) {
                    resultat(i, j) = T();    // Initialisation à zéro
                }
                resultat(i, j) += elements[i][k] * autre.elements[k][j];
            }
        }
    }

    return resultat;
}


template<class T>
Matrice<T>& Matrice<T>::operator=(const Matrice<T>& autre) {
    if (this != &autre) {
        libererMemoire();
        lignes = autre.lignes;
        colonnes = autre.colonnes;
        allouerMemoire();
        copierElements(autre);
    }
    return *this;
}

template<class T>
Matrice<T> Matrice<T>::t() const {
    Matrice<T> transposition(colonnes, lignes);
    for (int i = 0; i < lignes; ++i) {
        for (int j = 0; j < colonnes; ++j) {
            transposition(j, i) = elements[i][j];
        }
    }

    return transposition;
}

template<class T>
void Matrice<T>::ajouterLigne(int position, const T& valeur) {
    assert(position >= 0 && position <= lignes && "Position invalide pour ajouter une ligne");

    T** nouveauxElements = new T*[lignes + 1];
    for (int i = 0; i < lignes + 1; ++i) {
        nouveauxElements[i] = new T[colonnes];
    }

    for (int i = 0, j = 0; i < lignes + 1; ++i) {
        if (i == position) {
            for (int k = 0; k < colonnes; ++k) {
                nouveauxElements[i][k] = valeur;
            }
        } else {
            for (int k = 0; k < colonnes; ++k) {
                nouveauxElements[i][k] = elements[j][k];
            }
            ++j;
        }
    }

    libererMemoire();
    elements = nouveauxElements;
    ++lignes;
}


#endif
