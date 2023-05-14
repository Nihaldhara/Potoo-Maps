#pragma once
#include "settings.h"
#include "map.h"

typedef struct KVPair_s {
    Point key;
    int value;
} KVPair;

typedef struct DictNode_s
{
    struct DictNode_s *parent;
    struct DictNode_s *leftChild;
    struct DictNode_s *rightChild;
    int height;

    KVPair pair;
} DictNode;

typedef struct Dict_s
{
    DictNode *root;
    int size;
} Dict;

Dict *Dict_New();
void Dict_Delete(Dict *dict);
int Dict_GetSize(Dict *dict);

// Recherche une cl� dans le dictionnaire.
// Renvoie la valeur associ�e � la cl� ou NULL si la cl� n'est pas pr�sente
int Dict_Get(Dict *dict, Point key);

// Ins�re une paire cl�/valeur dans le dictionnaire.
// Renvoie la valeur pr�c�dente si la cl� est d�j� dans le dictionnaire, sinon revoie 0.
int Dict_Insert(Dict *dict, Point key, int value);

// Supprime une paire cl�/valeur dans le dictionnaire.
// Renvoie la valeur associ�e � la cl� ou NULL si la cl� n'est pas pr�sente.
int Dict_Remove(Dict *dict, Point key);

// Structure d'it�rateur pour parcourir un dictionnaire.
typedef struct DictIter_s
{
    Dict *dict;
    DictNode *curr;
    bool first;
} DictIter;

// Initialise un it�rateur sur le dictionnaire.
void Dict_GetIterator(Dict *dict, DictIter *iter);

// Indique s'il reste des �l�ments � parcourir.
bool DictIter_HasNext(DictIter *iter);

// Renvoie l'�l�ment actuel dans le dictionnaire puis fait avancer l'it�rateur au suivant.
KVPair *DictIter_Next(DictIter *iter);

// Pour le debug
void Dict_Print(Dict *dict);
void Dict_PrintTree(Dict *dict);



