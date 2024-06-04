# INF3105 - Structures de données et algorithmes
# UQAM / Département d'informatique
#
# Fichier Makefile pour le TP2.
# Pour compiler en mode release, il suffit de taper la commande make.
# Pour compiler en mode debug, il suffit de taper la commande make DEBUG=1.

OPTIONS = -Wall -Wextra -std=c++17 $(if $(DEBUG),-ggdb -O0,-O3 -DNDEBUG)

all: tp2

tp2: tp2.o univers.o salle.o
	g++ ${OPTIONS} -o tp2 tp2.o univers.o salle.o

tp2.o: tp2.cpp univers.h
	g++ ${OPTIONS} -c tp2.cpp

univers.o: univers.cpp univers.h salle.h
	g++ ${OPTIONS} -c univers.cpp

salle.o: salle.cpp salle.h
	g++ ${OPTIONS} -c salle.cpp

clean:
	rm -rf tp2 *~ *.o
