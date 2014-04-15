#ifndef __OPT_H__
#define __OPT_H__

/**
 * Fichier de sortie
 *
 * Si on ne spécifie pas de fichier avec l'option o 
 * a.out.s sera choisi
 */
extern FILE* file_out;

/**
 * Traite les options du compilateur
 *
 * @param argc Nombre d'argument y compris le nom de l'exécutable
 * @param argv Tableau des arguments
 */
void do_options(int argc, char **argv);

/**
 * Fermeture des fichiers et annulation de redirection de l'entrée standard
 */
void close_files();

extern FILE *stdin_terminal;

#endif//__OPT_H__
