#include "dict.h"

bool Dict_Find(Dict *dict, Point key, DictNode **res);
void Dict_Balance(Dict *dict, DictNode *node);

int coordinatescmp(Point a, Point b)
{
    if (a.latitude < b.latitude) return -1;
    else if (a.latitude > b.latitude) return 1;
    else
    {
        if (a.longitude < b.longitude) return -1;
        else if (a.longitude > b.longitude) return 1;
        else return 0;
    }
}

DictNode *DictNode_New(Point key, int value)
{
    DictNode *node = (DictNode *)calloc(1, sizeof(DictNode));
    AssertNew(node);

    node->pair.key = key;
    node->pair.value = value;

    return node;
}

void DictNode_Delete(DictNode *node)
{
    if (!node) return;
    free(node);
}

void DictNode_DeleteRec(DictNode *node)
{
    if (!node) return;
    DictNode_DeleteRec(node->leftChild);
    DictNode_DeleteRec(node->rightChild);
    DictNode_Delete(node);
}

void DictNode_SetLeft(DictNode *node, DictNode *left)
{
    node->leftChild = left;
    if (left != NULL)
        left->parent = node;
}

void DictNode_SetRight(DictNode *node, DictNode *right)
{
    node->rightChild = right;
    if (right != NULL)
        right->parent = node;
}

bool DictNode_Update(DictNode *node)
{
    if (node == NULL) return false;

    int hr = (node->rightChild) ? node->rightChild->height : -1;
    int hl = (node->leftChild) ? node->leftChild->height : -1;
    int h = (hr > hl) ? hr + 1 : hl + 1;

    if (node->height != h)
    {
        node->height = h;
        return true;
    }
    return false;
}

int DictNode_GetBalance(DictNode *node)
{
    if (node == NULL) return 0;

    int hr = (node->rightChild) ? node->rightChild->height : -1;
    int hl = (node->leftChild) ? node->leftChild->height : -1;
    return hr - hl;
}

DictNode *DictNode_Maximum(DictNode *node)
{
    assert(node != NULL);

    while (node->rightChild != NULL)
    {
        node = node->rightChild;
    }

    return node;
}

Dict *Dict_New()
{
    Dict *dict = (Dict *)calloc(1, sizeof(Dict));
    AssertNew(dict);

    return dict;
}

void Dict_Delete(Dict *dict)
{
    if (!dict) return;

    DictNode_DeleteRec(dict->root);
    free(dict);
}

int Dict_GetSize(Dict *dict)
{
    assert(dict);
    return dict->size;
}

void DictNode_Print(DictNode *node)
{
    if (!node) return;
    DictNode_Print(node->leftChild);
    printf("  \"%lf %lf\": %p,\n", node->pair.key.latitude, node->pair.key.longitude, node->pair.value);
    DictNode_Print(node->rightChild);
}

void Dict_Print(Dict *dict)
{
    printf("size = %d\n", dict->size);
    DictNode_Print(dict->root);
}

void DictNode_PrintTree(DictNode *node, int depth, int isLeft)
{
    if (!node) return;

    DictNode_PrintTree(node->rightChild, depth + 1, false);

    for (int i = 0; i < depth - 1; i++) printf("    ");
    if (depth > 0)
    {
        if (isLeft) printf("  \\-");
        else printf("  /-");
    }
    printf("[%lf %lf]\n", node->pair.key.latitude, node->pair.key.longitude);

    DictNode_PrintTree(node->leftChild, depth + 1, true);
}

void Dict_PrintTree(Dict *dict)
{
    DictNode_PrintTree(dict->root, 0, false);
}

int Dict_Insert(Dict *dict, Point key, int value)
{
    if (dict->root == NULL)
    {
        dict->root = DictNode_New(key, value);
        dict->size = 1;

        return 0;
    }

    DictNode *node = NULL;
    if (Dict_Find(dict, key, &node))
    {
        // La donnée est déjà présente dans l'arbre
        int prevValue = node->pair.value;
        node->pair.value++;

        return prevValue;
    }

    // Crée le nouveau noeud
    DictNode *newNode = DictNode_New(key, value);
    if (coordinatescmp(key, node->pair.key) < 0)
    {
        DictNode_SetLeft(node, newNode);
    }
    else
    {
        DictNode_SetRight(node, newNode);
    }
    dict->size++;

    // Equilibre l'arbre à partir du parent du nouveau noeud
    Dict_Balance(dict, node);

    return 0;
}

bool Dict_Find(Dict *dict, Point key, DictNode **res)
{
    if (dict->root == NULL)
    {
        *res = NULL;
        return false;
    }

    DictNode *node = dict->root;
    while (true)
    {
        int cmp = coordinatescmp(key, node->pair.key);
        if (cmp == 0)
        {
            *res = node;
            return true;
        }
        else if (cmp < 0)
        {
            if (node->leftChild == NULL)
            {
                *res = node;
                return false;
            }
            node = node->leftChild;
        }
        else
        {
            if (node->rightChild == NULL)
            {
                *res = node;
                return false;
            }
            node = node->rightChild;
        }
    }
}

int Dict_Get(Dict *dict, Point key)
{
    DictNode *res = NULL;
    if (Dict_Find(dict, key, &res))
    {
        return res->pair.value;
    }
    return 0;
}

void Dict_ReplaceChild(
    Dict *dict, DictNode *parent,
    DictNode *curChild, DictNode *newChild)
{
    if (parent == NULL)
    {
        dict->root = newChild;
    }
    else
    {
        if (parent->leftChild == curChild)
            parent->leftChild = newChild;
        else if (parent->rightChild == curChild)
            parent->rightChild = newChild;
        else
            assert(false);
    }

    if (newChild)
        newChild->parent = parent;
}

void Dict_RotateLeft(Dict *dict, DictNode *node)
{
    if (node == NULL || node->rightChild == NULL) return;

    DictNode *parent = node->parent;
    DictNode *rightChild = node->rightChild;

    DictNode_SetRight(node, rightChild->leftChild);
    DictNode_SetLeft(rightChild, node);
    Dict_ReplaceChild(dict, parent, node, rightChild);

    // Met à jour les hauteurs
    DictNode_Update(node);
    DictNode_Update(rightChild);
}

void Dict_RotateRight(Dict *dict, DictNode *node)
{
    if (node == NULL || node->leftChild == NULL) return;

    DictNode *parent = node->parent;
    DictNode *leftChild = node->leftChild;

    DictNode_SetLeft(node, leftChild->rightChild);
    DictNode_SetRight(leftChild, node);
    Dict_ReplaceChild(dict, parent, node, leftChild);

    // Met à jour les hauteurs
    DictNode_Update(node);
    DictNode_Update(leftChild);
}

void Dict_Balance(Dict *dict, DictNode *node)
{
    // Remonte éventuellement jusqu'à la racine pour rééquilibrer l'arbre
    while (node != NULL)
    {
        bool updated = DictNode_Update(node);
        int balance = DictNode_GetBalance(node);
        /*if (!updated && (abs(balance) < 2))
        {
            // Le noeud est à jour
            return;
        }*/
        // Il suffit de tester tous les noeuds 
        DictNode *parent = node->parent;

        if (balance == 2)
        {
            int rightBalance = DictNode_GetBalance(node->rightChild);
            if (rightBalance == -1)
            {
                Dict_RotateRight(dict, node->rightChild);
            }
            Dict_RotateLeft(dict, node);
        }
        else if (balance == -2)
        {
            int leftBalance = DictNode_GetBalance(node->leftChild);
            if (leftBalance == 1)
            {
                Dict_RotateLeft(dict, node->leftChild);
            }
            Dict_RotateRight(dict, node);
        }

        node = parent;
    }
}

int Dict_Remove(Dict *dict, Point key)
{
    if (dict->root == NULL)
    {
        return 0;
    }

    DictNode *node = NULL;
    if (Dict_Find(dict, key, &node) == false)
    {
        return 0;
    }
    assert(node != NULL);
    void *value = node->pair.value;

    DictNode *start = NULL;
    if (node->leftChild == NULL)
    {
        // Remplacement par le fils droit
        Dict_ReplaceChild(dict, node->parent, node, node->rightChild);
        start = node->parent;
        DictNode_Delete(node);
    }
    else if (node->rightChild == NULL)
    {
        // Remplacement par le fils gauche
        Dict_ReplaceChild(dict, node->parent, node, node->leftChild);
        start = node->parent;
        DictNode_Delete(node);
    }
    else
    {
        // Le noeud a deux fils
        // On l'échange avec sa valeur immédiatement inférieure (qui n'a plus de fils droit)
        DictNode *maxLeft = DictNode_Maximum(node->leftChild);
        KVPair pair = node->pair;
        node->pair = maxLeft->pair;
        maxLeft->pair = pair;

        // Puis on le supprime comme précédemment
        Dict_ReplaceChild(dict, maxLeft->parent, maxLeft, maxLeft->leftChild);
        start = maxLeft->parent;
        DictNode_Delete(maxLeft);
    }
    dict->size--;

    // Equilibre l'arbre à partir du parent du noeud supprimé
    Dict_Balance(dict, start);

    return value;
}

void Dict_GetIterator(Dict *dict, DictIter *iter)
{
    DictNode *curr = NULL;
    if (dict->root)
    {
        curr = dict->root;
        while (curr->leftChild)
        {
            curr = curr->leftChild;
        }
    }

    iter->curr = curr;
    iter->dict = dict;
}

KVPair *DictIter_Next(DictIter *iter)
{
    if (iter->curr == NULL)
    {
        return NULL;
    }

    DictNode *curr = iter->curr;
    DictNode *next = NULL;

    KVPair *pair = &(curr->pair);

    if (curr->rightChild != NULL)
    {
        // On cherche la valeur suivante en descendant dans l'arbre
        next = curr->rightChild;
        while (next->leftChild)
            next = next->leftChild;
    }
    else
    {
        // On cherche la valeur suivante en remontant dans l'arbre
        next = curr->parent;
        while (next)
        {
            if (curr == next->leftChild)
                break;

            curr = next;
            next = next->parent;
        }
    }

    iter->curr = next;

    return pair;
}

bool DictIter_HasNext(DictIter *iter)
{
    return iter->curr != NULL;
}


/*Dict* createAVLDictionary(char** words, int wordcount)
{
    int i, sertarien;
    void* value;    
    Dict* dict = Dict_New();

    //On met toutes les mots dans le dictionnaire précédement crée
    //La clé étant le mot et la valeur le nombre de fois que la clé apparait dans la liste de mots 
    for (i = 0; i < wordcount; i++)
    {
        value = Dict_Get(dict, words[i]);
        if (!Dict_Get(dict, words[i]))
        {
            Dict_Insert(dict, words[i], 1);
        }
        else if (Dict_Get(dict, words[i]))
            Dict_Insert(dict, words[i], ++value);
    }
    return dict;
}*/