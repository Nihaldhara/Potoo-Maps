#pragma once
#include "settings.h"

typedef struct sBinNode
{
	//La distance du noeud
	double value;
	//Le cle
	int id; //void* id;
}BinNode;


typedef struct sBinHeap
{
	//Le nombre de noeud dans le tableau
	int size;
	//Tableau des noeuds
	BinNode* tab;
	//Le nombre maximal de noeud dans le tableau
	int capacity;
}BinHeap;

BinHeap* BinHeap_create(int size);

BinHeap* BinNode_insert(BinHeap* heap, int id, double value);

BinNode BinNode_remove(BinHeap* heap);

void BinHeap_destroy(BinHeap* heap);

void BinHeap_print(BinHeap* heap);