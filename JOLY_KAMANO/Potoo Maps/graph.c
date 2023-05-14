#include "graph.h"

Graph* Graph_load(char* filename)
{
    FILE* fichier = fopen(filename, "r");
    if (!fichier)
    {
        printf("Impossible d'ouvrir le fichier, arret du programme\n");
        return NULL;
    }

    else
    {
        int n = 0, m = 0;
        int u = 0, v = 0;
        float p = 0.0f;
        fscanf(fichier, "%d %d", &n, &m);
        Graph* graph = Graph_create(n);
        for (int i = 0; i < m; i++)
        {
            fscanf(fichier, "%d %d %f", &u, &v, &p);
            Graph_set(graph, u, v, p);
        }
        fclose(fichier);
        return graph;
    }
}

/// @brief Sous-fonction de Graph_dfsPrint() pour les appels récursifs.
/// @param graph le graphe.
/// @param currID l'identifiant du noeud courant.
/// @param explored tableau de booléens indiquant pour chaque identifiant de
/// noeud s'il a été marqué comme atteint.
void Graph_dfsPrintRec(Graph* graph, int currID, bool* explored)
{
    if (!explored[currID])
    {
        int size = 0;
        Arc* successors = (Arc*)calloc(Graph_size(graph), sizeof(Arc));
        successors = Graph_getSuccessors(graph, currID, &size);
        printf("%d-", currID);
        explored[currID] = true;
        if (size > 0)
        {
            for (int i = 0; i < size; i++)
            {
                if (!explored[successors[i].target])
                    Graph_dfsPrintRec(graph, successors[i].target, explored);
            }
        }
    }
}

void Graph_dfsPrint(Graph* graph, int start)
{
    bool* explored = (bool*)calloc(Graph_size(graph), sizeof(bool));
    if (graph)
        Graph_dfsPrintRec(graph, start, explored);
}

void Graph_bfsPrint(Graph* graph, int start)
{
    bool* explored = (bool*)calloc(Graph_size(graph), sizeof(bool));
    IntList* list = IntList_create();
    IntList_enqueue(list, start);
    explored[start] = true;
    while (list->nodeCount > 0)
    {
        int current = IntList_dequeue(list);
        printf("%d- ", current);
        int size = 0;
        Arc* successors = Graph_getSuccessors(graph, current, &size);
        for (int i = 0; i < size; i++)
        {
            if (!explored[successors[i].target])
            {
                IntList_enqueue(list, successors[i].target);
                explored[successors[i].target] = true;
            }
        }
    }
}

IntTree* Graph_spanningTreeRec(Graph* graph, int current, bool* explored)
{
    explored[current] = true;
    IntTree* tree = IntTree_create(current);
    int size;
    Arc* successors = Graph_getSuccessors(graph, current, &size);
    for (int i = 0; i < size; i++)
    {
        if (!explored[successors[i].target])
            IntTree_addChild(tree, Graph_spanningTreeRec(graph, successors[i].target, explored));
    }
    return tree;
}

IntTree* Graph_spanningTree(Graph* graph, int start)
{
    bool* explored = (bool*)calloc(Graph_size(graph), sizeof(bool));
    IntTree* tree = IntTree_create(start);
    if (graph)
        tree = Graph_spanningTreeRec(graph, start, explored);
    return tree;
}
