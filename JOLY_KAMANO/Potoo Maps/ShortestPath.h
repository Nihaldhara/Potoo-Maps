#pragma once

#include "settings.h"
#include "graph.h"
#include "path.h"

/// @brief Renvoie un plus court chemin entre deux sommets d'un graphe.
/// Si aucun chemin n'existe, renvoie NULL.
/// Cette fonction suit l'algorithme de Dijkstra.
/// Elle a une complexit� en O(n^2) o� n d�signe le nombre de noeuds du graphe.
/// 
/// @param graph le graphe.
/// @param start l'identifiant du sommet de d�part.
/// @param end l'identifiant du sommet d'arriv�e.
/// @return Un plus court chemin en cas d'existance, NULL sinon.
Path* Graph_shortestPath(Graph* graph, int start, int end);

/// @brief Effectue l'algorithme de Dijkstra.
/// Si end >= 0, cette fonction calcule un plus court chemin entre les noeuds
/// start et end.
/// Le chemin doit cependant �tre reconstruit � partir des tableaux
/// predecessors et distances en utilisant la fonction Graph_dijkstraGetPath().
/// Si end < 0, l'algorithme calcule un plus court chemin pour chaque noeud
/// accessible depuis start.
/// 
/// @param graph le graphe.
/// @param start l'identifiant du sommet de d�part.
/// @param end l'identifiant du sommet d'arriv�e ou un entier < 0 pour calculer
///     tous les chemins.
/// @param predecessors tableau p�alablement allou� dont la taille est �gale au
///     nombre de noeuds du graphe. Apr�s l'appel � la fonction, il contient le
///     pr�d�cesseur de chaque noeud.
/// @param distances tableau p�alablement allou� dont la taille est �gale au
///     nombre de noeuds du graphe. Apr�s l'appel � la fonction, il contient
///     pour chaque noeud sa distance avec le noeud start.
void Graph_dijkstra(
    Graph* graph, int start, int end, int* predecessors, float* distances
);

/// @brief Reconstruit un chemin � partir de la sortie de l'algorithme de
/// Dijkstra.
/// 
/// @param predecessors tableau des pr�decesseurs.
/// @param distances tableau des distances.
/// @param end noeud d'arriv�e.
/// @return Le plus court chemin arrivant au noeud end s'il existe, NULL sinon.
Path* Graph_dijkstraGetPath(int* predecessors, float* distances, int end);

/// @brief Renvoie un plus court chemin entre deux sommets d'un graphe.
/// Si aucun chemin n'existe, renvoie NULL.
/// Cette fonction effectue une �num�ration compl�te de tous les chemins
/// possibles. Elle a une complexit� en O(n!).
/// 
/// @param graph le graphe.
/// @param start l'identifiant du sommet de d�part.
/// @param end l'identifiant du sommet d'arriv�e.
/// @return Un plus court chemin en cas d'existance, NULL sinon.
Path* Graph_enumPath(Graph* graph, int start, int end);
