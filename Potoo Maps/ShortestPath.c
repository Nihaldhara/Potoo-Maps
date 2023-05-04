#include "shortestPath.h"
#include <math.h>
#include "Binary.h"

#define INFINI 10000

Path* Graph_shortestPath(Graph* graph, int start, int end)
{
    //Creation des tableaux
    int size = Graph_size(graph);
    int* predecessors = (int*)calloc(size, sizeof(int));
    float* distances = (float*)calloc(size, sizeof(float));

    if ((!predecessors) || (!distances))
        return NULL;

    //Initialisation des tableaux
    for (int i = 0; i < size; i++)
    {
        distances[i] = INFINI;
        predecessors[i] = -1;
    }
    Graph_dijkstra(graph, start, end, predecessors, distances);
    Path* path = Graph_dijkstraGetPath(predecessors, distances, end);
    free(predecessors);
    free(distances);
    return path;
}

int Graph_dijkstra_firstnode(Graph* graph, int u, bool* explored, float* distances)
{
    if (!graph)
        return 0;
    int size = Graph_size(graph);
    //Recupere le premier noeud non visite
    for (int i = 0; i < size; i++)
    {
        if (explored[i] == false)
        {
            u = i;
            break;
        }
    }
    //Cherche le premier noeud non visite avec la distance minimale
    for (int i = 0; i < size; i++)
    {
        if ((explored[i] == false) && (distances[i]) < distances[u])
            u = i;
    }
    return u;
}

void Graph_dijkstra(Graph* graph, int start, int end, int* predecessors, float* distances)
{
    if ((!graph) || (end < 0) || (start < 0) || (end > Graph_size(graph)) || (start > Graph_size(graph)))
        return;
    if (start == end)
        return;

    //Creation et Initialisation des variables
    int size = Graph_size(graph), u = start;
    bool* explored = (bool*)calloc(size, sizeof(bool));
    bool allexplored = false;


    int nbvoisins = 0;
    if (!explored)
        return;

    for (int i = 0; i < size; i++)
        explored[i] = false;
    distances[start] = 0.0f;
    predecessors[start] = -1;

    BinHeap* heap = BinHeap_create(size);
    heap = BinNode_insert(heap, start, distances[start]);

    //Si end est inf�rieur � 0 on cherche le plus court chemin pour tous les noeuds 
    if (end < 0)
    {
        while (!allexplored)
        {
            //On cherche le noeud pas explore avec la distance minimale
            BinNode node = BinNode_remove(heap);
            u = node.id;
            //u = Graph_dijkstra_firstnode(graph, u, explored, distances);

            explored[u] = true;
            if (distances[u] == INFINI)
            {
                continue;
            }

            //On cherche les plus courts chemin pour tous les voisins du noeud u
            int nbvoisins = Graph_getPositiveValency(graph, u);
            for (int j = 0; j < nbvoisins; j++)
            {
                if ((distances[u] + Graph_get(graph, u, j)) < distances[j])
                {
                    predecessors[j] = u;
                    distances[j] = distances[u] + Graph_get(graph, u, j);
                    BinNode_insert(heap, j, distances[j]);
                }
            }

            //On regarde si tous les noeuds ont ete explore
            allexplored = true;
            for (int i = 0; i < size; i++)
            {
                if (explored[i] == false)
                    allexplored = false;
            }
        }
    }

    //Si end est superieur � 0 on cherche le plus court chemin entre end et le start
    else if (end >= 0)
    {
        while (u != end)
        {
            //On cherche le noeud pas explore avec la distance minimale
            BinNode node = BinNode_remove(heap);
            u = node.id;
            //u = Graph_dijkstra_firstnode(graph, u, explored, distances);

            explored[u] = true;
            if (distances[u] == INFINI)
            {
                continue;
            }

            //On cherche les plus courts chemin pour tous les voisins du noeud u
            Arc* arc = Graph_getSuccessors(graph, u, &nbvoisins);
            for (int j = 0; j < nbvoisins; j++)
            {
                if ((distances[u] + Graph_get(graph, u, arc[j].target)) < distances[arc[j].target])
                {
                    predecessors[arc[j].target] = u;
                    distances[arc[j].target] = distances[u] + Graph_get(graph, u, arc[j].target);
                    BinNode_insert(heap, arc[j].target, distances[arc[j].target]);
                }
            }
            free(arc);
        }
    }
    BinHeap_destroy(heap);
    free(explored);
}

Path* Graph_dijkstraGetPath(int* predecessors, float* distances, int end)
{
    //S'il y a un probl�me, return instant
    if ((end < 0) || (!predecessors) || (!distances))
        return NULL;

    //On regarde quel est le noeud de d�part
    int start = 0, current = end;
    while (predecessors[start] != -1)
        start++;

    Path* path = Path_create(end);
    if (!path)
        return NULL;

    current = predecessors[current];
    //Tant que le noeud current n'est pas le noeud de start on continue a inserer dans le chemin
    while ((current) && (current != start))
    {
        Path_insertFirst(path, current, 0);
        current = predecessors[current];
    }
    Path_insertFirst(path, current, 0);
    //La distance de path est la distance entre la fin et le d�but
    path->distance = distances[end];
    return path;
}

Path* Graph_enumPathRec(Graph* graph, Path* currPath, int start, int end)
{
    if (start == end)return currPath;
    Path* best = NULL;
    int size;
    Arc* neighbours = Graph_getSuccessors(graph, start, &size);
    for (int i = 0; i < size; i++)
    {
        if (IntList_isIn(currPath->list, neighbours[i].target))
            continue;

        Path* Newpath = Path_copy(currPath);
        Path_insertLast(Newpath, neighbours[i].target, neighbours[i].weight);
        Newpath = Graph_enumPathRec(graph, Newpath, neighbours[i].target, end);
        if (Newpath == NULL)
            continue;
        if (best == NULL || Newpath->distance < best->distance)
            best = Path_copy(Newpath);
        Path_destroy(Newpath);
    }
    Path_destroy(currPath);
    free(neighbours);
    return best;
}

Path* Graph_enumPath(Graph* graph, int start, int end)
{
    if ((!graph) || (start < 0) || (start > Graph_size(graph)) || (end > Graph_size(graph)) || (end < 0))
        return NULL;
    Path* path = Path_create(start);
    path = Graph_enumPathRec(graph, path, start, end);
    return path;
}