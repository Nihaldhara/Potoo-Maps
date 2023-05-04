#include "Binary.h"
#include "settings.h"

BinHeap* BinHeap_create(int capacity)
{
	//On cree le heap
	BinHeap* root = calloc(1, sizeof(BinHeap));
	if (!root)
		return NULL;
	root->tab = calloc(capacity, sizeof(BinNode));
	root->size = 0;
	root->capacity = capacity;
	return root;
}

BinNode BinNode_create(BinHeap* heap, int id, double value)
{
	BinNode node = { 0 };
	node.value = value;
	node.id = id;
	return node;
}

//Est ce que ca marche vraiment ?
void BinNode_swap(BinNode* nodea, BinNode* nodeb)
{
	BinNode* tmp = nodea;
	nodea = nodeb;
	nodeb = tmp;
}

void BinHeap_percolateup(BinHeap* heap)
{
	int i = heap->size;
	BinNode parent = heap->tab[(i - 1) / 2], node = heap->tab[i], tmp = { 0 };
	while ((i != 0) && (parent.value > node.value))
	{
		tmp = node;
		node = parent;
		parent = tmp;

		i = (i - 1) / 2;

		node = heap->tab[i];
		parent = heap->tab[(i - 1) / 2];
	}
}

BinHeap* BinNode_insert(BinHeap* heap, int id, double value)
{
	assert(heap);
	assert((id >= 0) && (id < heap->capacity));

	BinNode node = BinNode_create(heap, id, value);
	heap->size++;
	heap->tab[heap->size] = node;

	BinHeap_percolateup(heap);
	return heap;
}

void BinHeap_percolatedown(BinHeap* heap)
{
	int i = 0;
	BinNode fils = heap->tab[(2 * i) + 1], node = heap->tab[i], tmp = { 0 };

	if (heap->tab[(2 * i) + 1].value > heap->tab[(2 * i) + 2].value)
		fils = heap->tab[(2 * i) + 1];
	else
		fils = heap->tab[(2 * i) + 2];

	while ((i > heap->size) && (fils.value > node.value))
	{
		tmp = node;
		node = fils;
		fils = tmp;

		if (heap->tab[(2 * i) + 1].value > heap->tab[(2 * i) + 2].value)
		{
			i = (2 * i) + 1;
			fils = heap->tab[(2 * i) + 1];
		}
		else
		{
			i = (2 * i) + 2;
			fils = heap->tab[(2 * i) + 2];
		}
		node = heap->tab[i];
	}
}

BinNode BinNode_remove(BinHeap* heap)
{
	if (heap->size == 0)
	{
		printf("########################\n");
		printf("#    Gros Probleme!    #\n");
		printf("########################\n\n\n");
		assert(0);
	}
	assert(heap);
	BinNode zero = { 0 };
	BinNode res = heap->tab[0];
	heap->tab[0] = heap->tab[heap->size];
	heap->tab[heap->size] = zero;
	heap->size--;
	BinHeap_percolatedown(heap);
	return res;
}


void BinHeap_destroy(BinHeap* heap)
{
	free(heap->tab);
	free(heap);
}