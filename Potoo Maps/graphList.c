#include "graph.h"

#ifndef _GRAPH_MAT

typedef struct sGraph Graph;
typedef struct sGraphNode GraphNode;
typedef struct sArcList ArcList;

struct sGraph
{
    /// @brief Tableau contenant les noeuds du graphe.
    GraphNode* nodes;
    /// @brief Nombre de noeuds du graphe.
    int size;
};

/// @brief Structure représentant un noeud d'un graphe.
struct sGraphNode
{
    /// @brief Identifiant du noeud.
    /// Compris entre 0 et la taille du graphe moins 1.
    int id;
    /// @brief Degré entrant du noeud.
    int negativeValency;
    /// @brief Degré sortant du noeud.
    int positiveValency;
    /// @brief Liste des arcs sortants du noeud.
    ArcList* arcList;
    /// @brief Point associé au noeud.
    Point coordinates;
};

/// @brief Structure représentant une liste simplement chaînée des arcs sortants d'un noeud.
struct sArcList
{
    /// @brief Pointeur vers l'élément suivant de la liste.
    /// Vaut NULL s'il s'agit du dernier élément.
    ArcList* next;
    /// @brief Arc associé au noeud de liste.
    Arc arc;
};

Graph* Graph_create(int size)
{
    Graph* graph = (Graph*)calloc(1, sizeof(Graph));
    graph->size = size;
    graph->nodes = (GraphNode*)calloc(size, sizeof(GraphNode));
    for (int i = 0; i < size; i++)
    {
        graph->nodes[i].arcList = (ArcList*)calloc(1, sizeof(ArcList));
    }

    return graph;
}

void Graph_destroy(Graph* graph)
{

}

int Graph_size(Graph* graph)
{
    assert(graph);
    return graph->size;
}

void Graph_print(Graph* graph)
{
    printf("Nombre de noeuds : %d\n", graph->size);
    for (int i = 0; i < graph->size; i++)
    {
        printf("\nNombre de routes partant de l'intersection %lf - %lf : %d\n", graph->nodes[i].coordinates.latitude, graph->nodes[i].coordinates.longitude, graph->nodes[i].positiveValency);
        ArcList* current = graph->nodes[i].arcList;
        while (current->next)
        {
            current = current->next;
            printf("    -Route sortante de longueur %f, ciblant l'intersection de coordonnees %lf - %lf\n", current->arc.weight, graph->nodes[current->arc.target].coordinates.latitude, graph->nodes[current->arc.target].coordinates.longitude);
        }
    }
}

void Graph_setCoordinates(Graph* graph, int u, Point coordinates)
{
    if (u < graph->size && u >= 0)
        graph->nodes[u].coordinates = coordinates;
}

void Graph_set(Graph* graph, int u, int v, double weight)
{
    bool exists = false;
    ArcList* current = graph->nodes[u].arcList;
    //Recherche de l'emplacement où insérer
    while (current->next && !exists)
    {
        current = current->next;
        //Modification d'un noeud existant
        if (current->arc.target == v)
        {
            current->arc.weight = weight;
            exists = true;
        }
    }

    //Supression d'un noeud existant
    if (exists && weight == -1.0f)
    {
        graph->nodes[u].positiveValency--;
        graph->nodes[v].negativeValency--;
        ArcList* previous = graph->nodes[u].arcList;
        while (previous->next != current)
            previous = previous->next;
        previous->next = current->next;
    }

    //Ajout d'un nouveau noeud
    if (!exists && weight != -1.0f)
    {
        graph->nodes[u].positiveValency++;
        graph->nodes[v].negativeValency++;
        ArcList* new = (ArcList*)calloc(1, sizeof(ArcList));
        new->arc.source = u;
        new->arc.target = v;
        new->arc.weight = weight;

        current->next = new;
    }
}

Point Graph_getCoordinates(Graph* graph, int u)
{
    if (u < graph->size && u >= 0 && graph) return graph->nodes[u].coordinates;
}

float Graph_get(Graph* graph, int u, int v)
{
    if (u > graph->size || v > graph->size)
        return -1.0f;

    if (graph->nodes[u].arcList->next)
    {
        ArcList* current = graph->nodes[u].arcList;
        while (current->next)
        {
            current = current->next;
            if (current->arc.target == v)
                return current->arc.weight;
        }
    }
    return -1.0f;
}

int Graph_getPositiveValency(Graph* graph, int u)
{
    return graph->nodes[u].positiveValency;
}

int Graph_getNegativeValency(Graph* graph, int u)
{
    return graph->nodes[u].negativeValency;
}

Arc* Graph_getPredecessors(Graph* graph, int u, int* size)
{
    int count = 0;
    Arc* predecessors = (Arc*)calloc(graph->size, sizeof(Arc));
    for (int i = 0; i < graph->size; i++)
    {
        if (graph->nodes[i].positiveValency)
        {
            ArcList* current = graph->nodes[i].arcList;
            while (current->next)
            {
                current = current->next;
                if (current->arc.target == u)
                {
                    predecessors[count].source = current->arc.source;
                    predecessors[count].target = current->arc.target;
                    predecessors[count].weight = current->arc.weight;
                    count++;
                }
            }
        }
    }
    *size = count;
    if (count == 0)
        return NULL;
    return predecessors;
}

Arc* Graph_getSuccessors(Graph* graph, int u, int* size)
{
    int count = 0;
    Arc* successors = (Arc*)calloc(graph->size, sizeof(Arc));
    ArcList* current = graph->nodes[u].arcList;
    while (current->next)
    {
        current = current->next;
        successors[count].source = current->arc.source;
        successors[count].target = current->arc.target;
        successors[count].weight = current->arc.weight;
        count++;
    }
    *size = count;
    if (count == 0)
        return NULL;
    return successors;
}

#endif