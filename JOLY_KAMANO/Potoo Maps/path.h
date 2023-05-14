#pragma once

#include "settings.h"
#include "intList.h"

/// @brief Structure repr�sentant un chemin dans un graphe.
typedef struct Path_s
{
    /// @brief Liste des sommets du chemin.
    /// Le premier �l�ment de la liste est le sommet et d�part
    /// et le dernier �l�ment le sommet d'arriv�e.
    IntList* list;

    /// @brief Longueur du chemin.
    /// Autrement dit la somme des poids des arcs qui composent le chemin.
    float distance;
} Path;

/// @brief Cr�e un nouveau chemin.
/// @param start le sommet de d�part du chemin.
/// @return Le chemin cr��.
Path* Path_create(int start);

/// @brief D�truit un chemin cr�� avec Path_create().
/// @param path le chemin � d�truire.
void Path_destroy(Path* path);

/// @brief Copie un chemin.
/// Il s'agit d'une copie profonde, tous les noeuds de la liste sont r�allou�s.
/// @param path le chemin.
/// @return Une copie profonde du chemin.
Path* Path_copy(Path* path);

/// @brief Insert un sommet au d�but d'un chemin.
/// @param path le chemin.
/// @param nodeID l'identifiant du noeud � ajouter.
/// @param weight le poids le l'arc ajout�.
void Path_insertFirst(Path* path, int nodeID, float weight);

/// @brief Insert un sommet � la fin d'un chemin.
/// @param path le chemin.
/// @param nodeID l'identifiant du noeud � ajouter.
/// @param weight le poids le l'arc ajout�.
void Path_insertLast(Path* path, int nodeID, float weight);

/// @brief Affiche un chemin.
/// @param path le chemin.
void Path_print(Path* path);
