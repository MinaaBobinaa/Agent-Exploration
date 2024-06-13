#!/usr/bin/env python3
"""Programme permettant d'évaluer des TPs du cours INF3105.

Le mode "valider" permet de vérifier que le(s) programme(s)
compile(nt) et s'exécute(nt) sans erreur sur un test simple.

Le mode "evaluer" vérifie le(s) programme(s) sur chacun des tests.

Hiérarchie de dossiers pour étudiants:
    racine/  // evaluer.py doit être lancé à partir d'ici
        -> exécutable du projet
        -> tests/
            -> [valideur]

Hiérarchie de dossier pour correcteurs:
    racine/  // evaluer.py doit être lancé à partir d'ici
        -> etudiants/
            -> etudiant1/
            -> etudiant2/
            -> ...
        -> tests/
            -> [valideur]
"""

from datetime import datetime
from enum import Enum
from shutil import which
import os
import re
import resource
import subprocess
import sys

__author__ = "Jaël Champagne Gareau, Guillaume Gosset"


class Mode(Enum):
    """Type énumératif spécifiant les modes de lancement du programme."""
    etudiant_valider = 1
    etudiant_evaluer = 2
    correcteur_valider = 3
    correcteur_evaluer = 4


# Constantes paramétrables
MODE = Mode.etudiant_evaluer
NOM_EXECUTABLE = "tp2"
DOSSIER_ETUDIANTS = "etudiants"
DOSSIER_TESTS = "tests"
SORTIES_VALIDEUR = ["nbReqs", "nbExistenceOK", "NbCheminsOK"]
TYPE_FICHIERS_ENTREES = ["-univers", "-req"]  # 2 fichiers d'entrées
TEST_SIMPLE = "C1_1c"  # test exécuté pour le mode valider
PREFIXES_TESTS = [  # ordre aléatoire si laissé vide

]

COULEUR = True  # mettre en couleur les résultats
MAX_TEMPS_PROCESSUS = 600  # en secondes
MAX_TAILLE_PILE = resource.RLIM_INFINITY  # en octets
MAX_MEMOIRE_VIRTUELLE = 8 * 1024 * 1024 * 1024  # en octets


def obtenir_info_systeme():
    """Retourne le nom du CPU et le type d'OS de l'ordinateur actuel."""

    # Type de système d'exploitation (Linux et Mac (Darwin) sont supportés)
    type_os = subprocess.run(["uname", "-s"],
                             stdout=subprocess.PIPE,
                             check=True).stdout.decode('utf-8').strip("\n")

    # Modèle de processeur
    if "Linux" in type_os:
        cmd = "grep \"model name\" /proc/cpuinfo | sort -u | cut -d \":\" -f 2"
    elif "Darwin" in type_os:
        cmd = "sysctl -n machdep.cpu.brand_string"
    else:
        sys.exit(f"Le système \"{type_os}\" n'est pas pris en charge")
    info = subprocess.run(cmd,
                          stdout=subprocess.PIPE,
                          shell=True,
                          check=True).stdout.decode('utf-8').strip("\n")
    cpu = ''.join(char or '?' for char in info)  # remplace chaîne vide par '?'

    return type_os, cpu


def obtenir_tests():
    """Retourne une liste des tests à exécuter

    Si PREFIXES_TESTS est vide, déduit une liste de préfixes en ordre aléatoire
    en fonction des fichiers présents dans le dossier de tests.

    Chaque "test" est une liste contenant les arguments du programme.
    """
    tests = []
    chemin_tests = DOSSIER_RACINE + os.sep + DOSSIER_TESTS + os.sep

    # trouver tous les préfixes possibles si pas spécifié
    if len(PREFIXES_TESTS) == 0:

        def natural_sort(l):
            convert = lambda txt: int(txt) if txt.isdigit() else txt.lower()
            alphanum_key = lambda key: [convert(c)
                                        for c in re.split('([0-9]+)', key)]
            return sorted(l, key=alphanum_key)

        prefixes = natural_sort(list({f.split("-", 1)[0]
                                      for f in os.listdir(chemin_tests)
                                      if "-" in f}))
    else:
        prefixes = PREFIXES_TESTS

    # construire chaque test
    for prefixe in prefixes:
        test = []
        for nom_type in TYPE_FICHIERS_ENTREES:
            test.append(chemin_tests + prefixe + nom_type + ".txt")

        # Trouver les fichiers de requêtes pour le préfixe courant
        requetes = sorted([f for f in os.listdir(chemin_tests)
                           if prefixe in f and "req" in f and "+" not in f])

        for requete in requetes:
            test[-1] = chemin_tests + requete
            tests.append(test[:])

    return tests


# Vérifie que le dossier des tests est bien placé
if not os.path.exists(DOSSIER_TESTS):
    sys.exit(f"Le dossier '{DOSSIER_TESTS}' doit être à la racine")

# Constantes automatiques
DATE_HEURE = datetime.now().strftime("%Y%m%d_%H%M%S")
NOM_RAPPORT = f"rapport-{DATE_HEURE}.txt"
TYPE_OS, CPU_INFO = obtenir_info_systeme()
DOSSIER_SCRIPT = os.path.dirname(os.path.realpath(__file__))
DOSSIER_RACINE = os.getcwd()
TESTS = obtenir_tests()
VALIDEUR = (
    os.path.abspath(DOSSIER_TESTS + os.sep + "valideur")
    if "valideur" in os.listdir(DOSSIER_TESTS)
    else "diff"
)

# Codes ANSI
VERT = '\033[92m'
JAUNE = '\033[93m'
ROUGE = '\033[91m'
STOP = '\033[0m'

UP = '\033[A'
DOWN = '\033[B'
ERASE = '\033[2K'
NL = '\033[L'

# Checks if gtime command exists (OSX only)
TIME = "/usr/bin/time"  # chemin de la commande time
if "Darwin" in TYPE_OS:
    HAS_TIME = which("gtime") is not None

    if not HAS_TIME:
        print("Attention: La commande gtime n'est pas installée, "
              "les données d'utilisation CPU et de mémoire ne seront "
              "donc pas disponibles.")
        print("Pour corriger ce problème, exécuter la commande: "
              "brew install gnu-time")


def main():
    """Exécute tous les tests pour tous les étudiants."""

    if len(sys.argv) > 1:
        sys.exit("Usage: ./evaluer.sh")

    print(f"Évaluation du programme {NOM_EXECUTABLE} d'INF3105")

    if VALIDEUR == "diff":
        afficher_couleur("Aucun programme 'valideur' n'a été trouvé.\n"
                         "Utilisation de 'diff' pour comparer les résultats.",
                         JAUNE, '\n')

    if MODE == Mode.etudiant_valider:
        print("Mode etudiant_valider\n")
        valider_tp()
    elif MODE == Mode.etudiant_evaluer:
        print("Mode etudiant_evaluer")
        evaluer_tp()
    else:
        if not os.path.exists(DOSSIER_ETUDIANTS):
            sys.exit(f"Le dossier {DOSSIER_ETUDIANTS} doit être à la racine")

        if MODE == Mode.correcteur_valider:
            print("Mode correcteur_valider\n")
        else:
            print("Mode correcteur_evaluer")
            ecrire_entete_rapport()

        os.chdir(DOSSIER_ETUDIANTS)

        # Itérer sur chaque étudiant
        for etudiant in sorted(os.listdir(os.curdir)):
            os.chdir(etudiant)

            if MODE == Mode.correcteur_valider:
                valider_tp()
            else:
                # Nettoyer le dossier
                subprocess.run("rm -f *.txt",
                               capture_output=True,
                               shell=True,
                               check=False)

                afficher_couleur(f"\nÉvaluation de: {etudiant}", JAUNE, '\n\n')
                evaluer_tp()
            os.chdir("..")  # Revenir au dossier contenant les étudiants

        os.chdir("..")  # Revenir au dossier initial


def valider_tp():
    """Vérifie que le TP courant compile et fonctionne minimalement."""

    dossier_actuel = str(os.path.basename(os.getcwd()))
    afficher_couleur(dossier_actuel, JAUNE)  # Affiche l'étudiant actuel

    # Compiler le TP
    code_make = subprocess.run(["make"],
                               capture_output=True,
                               check=False).returncode
    if code_make != 0:
        afficher_couleur("compiler", ROUGE)
        afficher_couleur("executer", ROUGE)
        afficher_couleur("valider", ROUGE)
    else:
        afficher_couleur("compiler", VERT)

        chemin = DOSSIER_RACINE + os.sep + DOSSIER_TESTS + os.sep + TEST_SIMPLE
        test_simple = [chemin + f + ".txt" for f in TYPE_FICHIERS_ENTREES]
        test_simple[-1] = test_simple[-1].replace("req", "req0")
        res_sol = test_simple[-1].replace(".", "+.")
        res_tp = os.path.basename(res_sol)

        with open(res_tp, "w") as f_resultat:
            cmd = [f"./{NOM_EXECUTABLE}"] + test_simple
            code_exec = subprocess.run(cmd,
                                       stdout=f_resultat,
                                       check=False,
                                       preexec_fn=mettre_limites).returncode

        if code_exec != 0:
            afficher_couleur("executer", ROUGE)
            afficher_couleur("valider", ROUGE)
        else:
            afficher_couleur("executer", VERT)

            code_diff = subprocess.run([VALIDEUR, res_tp, res_sol],
                                       capture_output=True,
                                       check=False).returncode
            afficher_couleur("valider", VERT if code_diff == 0 else ROUGE)

    print()


def evaluer_tp():
    """Évalue le TP présent dans le dossier courant avec tous les tests."""

    # Si le TP n'est pas compilé, le compiler
    if not os.path.exists(NOM_EXECUTABLE):
        subprocess.run("make", capture_output=True, check=False)

    # Ligne écrite dans le rapport (s'il y a lieu)
    ligne_rapport = [os.path.basename(os.getcwd())]

    # Afficher noms colonnes
    if VALIDEUR == "diff":
        print("Nom_test\tCPU\tMém\tDiff")
    else:
        print("Nom_test\tCPU\tMém\t" + '\t'.join(SORTIES_VALIDEUR))

    # Itérer sur chaque test
    for test in TESTS:
        nom_requetes = os.path.basename(test[-1])
        res_sol = test[-1].replace(".", "+.")
        res_tp = nom_requetes.replace(".", "+.")
        afficher_couleur(nom_requetes.strip(".txt"), JAUNE)

        cmd = ' '.join([f"./{NOM_EXECUTABLE}"] + test) + f" 2>&1 > {res_tp}"
        cmd_time = f"{TIME} -f \"%U\t%M\" " + cmd + " | tail -n1"
        cmd_time_osx = "gtime -f \"%U\t%M\" " + cmd + " | tail -n1"
        proc = subprocess.run(cmd_time if "Linux" in TYPE_OS else (cmd_time_osx if ("Darwin" in TYPE_OS and HAS_TIME ) else cmd),
                              capture_output=True,
                              shell=True,
                              check=False,
                              preexec_fn=mettre_limites)

        if "Linux" in TYPE_OS or ("Darwin" in TYPE_OS and HAS_TIME):
            temps, mem = proc.stdout.decode("utf-8").strip("\n").split("\t")
        else:
            temps = mem = '?'

        afficher_couleur(temps)
        ligne_rapport.append(temps)
        afficher_couleur(mem)
        ligne_rapport.append(mem)

        cmd_valideur = VALIDEUR + " " + res_tp + " " + res_sol
        if VALIDEUR != "diff":
            cmd_valideur += " | tail -n1"
        proc_valide = subprocess.run(cmd_valideur,
                                     capture_output=True,
                                     shell=True,
                                     check=False)
        if VALIDEUR == "diff":
            if proc_valide.returncode == 0:
                afficher_couleur("OK", VERT)
                ligne_rapport.append("OK")
            else:
                afficher_couleur("Pas OK", ROUGE)
                ligne_rapport.append("Pas OK")
        else:
            out_valideur = proc_valide.stdout.decode("utf-8")
            for stat in out_valideur.strip("\n").split("\t"):
                afficher_couleur(stat)
                ligne_rapport.append(stat)
        print()

    if MODE == Mode.correcteur_evaluer:
        with open(f"../../{NOM_RAPPORT}", "a") as f_rapport:
            f_rapport.write('\t'.join(ligne_rapport) + '\n')


# Source: https://stackoverflow.com/a/34325723/5183410
def progress_bar(iterable, prefix='Progression', suffix='Complété',
                 length=50, fill='█'):
    """
    Call in a loop to create terminal progress bar,

    @params:
        iterable  : current iteration (Int)
        prefix    : prefix string (Str)
        suffix    : suffix string (Str)
        length    : character length of progress bar (Int)
        fill      : progress bar fill character (Str)
    """
    total = len(iterable)

    # Progress Bar Printing Function
    def print_bar(iteration):
        percent = ("{0:.1f}").format(100 * (iteration / float(total)))
        filled_length = int(length * iteration // total)
        bar_string = fill * filled_length + '-' * (length - filled_length)
        print(f'\r{prefix} |{bar_string}| {percent}% {suffix}', end='\r')

    # Initial Call
    print_bar(0)
    # Update Progress progress_bar
    for i, item in enumerate(iterable):
        yield item
        print_bar(i + 1)
    # Print New Line on Complete
    print()


def ecrire_entete_rapport(nom=NOM_RAPPORT):
    """Écrit les lignes d'entête du rapport dans le fichier 'nom'."""
    if VALIDEUR == "diff":
        colonnes_par_test = 3  # CPU, Mem, Diff
    else:
        colonnes_par_test = 2 + len(SORTIES_VALIDEUR)

    with open(nom, "w") as f_rapport:
        # Première ligne avec les noms de tests
        f_rapport.write("\t")  # Colonne avec le nom des soumissions
        for test in TESTS:
            nom_test = os.path.basename(test[-1]).strip(".txt")
            f_rapport.write(f"{nom_test}\t")
            for _ in range(colonnes_par_test - 1):
                f_rapport.write('\t')
        f_rapport.write('\n')

        # Deuxième ligne de l'entête
        colonnes = '\t'.join(["CPU", "Mem"] + SORTIES_VALIDEUR) + '\t'
        f_rapport.write("Soumissions\t")
        for _ in range(len(TESTS)):
            f_rapport.write(colonnes)
        f_rapport.write('\n')


def afficher_couleur(msg, couleur=STOP, fin='\t'):
    """Affiche un message en couleur sur la ligne courante."""
    if COULEUR:
        print(f"{couleur}{msg}{STOP}", end=fin)
    else:
        print(msg, end=fin)


def mettre_limites():
    """Met des limites aux ressources du processus courant."""
    if "Linux" in TYPE_OS:
        resource.setrlimit(resource.RLIMIT_CPU,
                           (MAX_TEMPS_PROCESSUS, MAX_TEMPS_PROCESSUS))
        resource.setrlimit(resource.RLIMIT_STACK,
                           (MAX_TAILLE_PILE, MAX_TAILLE_PILE))
        resource.setrlimit(resource.RLIMIT_AS,
                           (MAX_MEMOIRE_VIRTUELLE, MAX_MEMOIRE_VIRTUELLE))


if __name__ == '__main__':
    main()
