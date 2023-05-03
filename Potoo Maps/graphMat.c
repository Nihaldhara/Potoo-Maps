#include "graph.h"

#ifdef _GRAPH_MAT

typedef struct sGraph
{
    /// @brief Matrice des poids.
    float** arcs;
    /// @brief Tableau contenant les degrés entrants.
    int* negativeValencies;
    /// @brief Tableau contenant les degrés sortants.
    int* positiveValencies;
    /// @brief Nombre de noeuds du graphe.
    int size;
}Graph;

Graph* Graph_create(int size)
{
    Graph* graph = (Graph*)calloc(1, sizeof(Graph));

    graph->arcs = (float**)calloc(size, sizeof(float*));
    for (int i = 0; i < size; i++)
    {
        graph->arcs[i] = (float*)calloc(size, sizeof(float));
        for (int j = 0; j < size; j++)
        {
            graph->arcs[i][j] = -1.0f;
        }
    }
    graph->negativeValencies = (int*)calloc(size, sizeof(int));
    graph->positiveValencies = (int*)calloc(size, sizeof(int));
    graph->size = size;

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
        printf("Nombre d'arcs sortant du noeud %d : %d\n", i, graph->positiveValencies[i]);
        for (int j = 0; j < graph->size; j++)
        {
            if (graph->arcs[i][j] != -1.0f)
                printf("    -Arc sortant de poids %f, ciblant le noeud d'indice %d\n", graph->arcs[i][j], j);
        }
    }
}

void Graph_set(Graph* graph, int u, int v, float weight)
{
    if ((u < graph->size) && (v < graph->size))
    {
        //Arc existant
        if (graph->arcs[u][v] != -1.0f)
        {
            //Supression de l'arc
            if (weight == -1.0f)
            {
                graph->arcs[u][v] = -1.0f;
                graph->positiveValencies[u]--;
                graph->negativeValencies[v]--;
            }

            //Modification du poids de l'arc
            else
                graph->arcs[u][v] = weight;
        }

        //Création d'un nouvel arc
        else if(weight != -1.0f)
        {
            graph->arcs[u][v] = weight;
            graph->positiveValencies[u]++;
            graph->negativeValencies[v]++;
        }
    }
}

float Graph_get(Graph* graph, int u, int v)
{
    if (graph->arcs[u][v] != -1.0f)
        return graph->arcs[u][v];
    return -1.0f;
}

int Graph_getPositiveValency(Graph* graph, int u)
{
    if (u < graph->size)
        return graph->positiveValencies[u];
    return -1;
}

int Graph_getNegativeValency(Graph* graph, int u)
{
    if (u < graph->size)
        return graph->negativeValencies[u];
    return -1;
}

Arc* Graph_getPredecessors(Graph* graph, int u, int* size)
{
    int count = 0;
    Arc* predecessors = (Arc*)calloc(graph->size, sizeof(Arc));
    for (int i = 0; i < graph->size; i++)
    {
        if (graph->arcs[i][u] != -1)
        {
            predecessors[count].source = i;
            predecessors[count].target = u;
            predecessors[count].weight = graph->arcs[i][u];
            count++;
        }
    }
    *size = graph->negativeValencies[u];
    if (*size == 0)
        return NULL;
    return predecessors;
}

Arc* Graph_getSuccessors(Graph* graph, int u, int* size)
{
    int count = 0;
    Arc* successors = (Arc*)calloc(graph->size, sizeof(Arc));
    for (int i = 0; i < graph->size; i++)
    {
        if (graph->arcs[u][i] != -1)
        {
            successors[count].source = u;
            successors[count].target = i;
            successors[count].weight = graph->arcs[u][i];
            count++;
        }
    }
    *size = graph->positiveValencies[u];
    if (*size == 0)
        return NULL;
    return successors;
}

#endif