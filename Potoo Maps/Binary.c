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
	if (i <= 1)
		return;
	if (i == 2)
	{
		if (heap->tab[0].value > heap->tab[1].value)
		{
			tmp = heap->tab[0];
			heap->tab[0] = heap->tab[1];
			heap->tab[1] = tmp;
		}
		return;
	}
	i--;
	while ((i > 0) && (parent.value > node.value))
	{
		tmp = heap->tab[(i - 1) / 2];
		heap->tab[(i - 1) / 2] = heap->tab[i];
		heap->tab[i] = tmp;

		i = (i - 1) / 2;

		node = heap->tab[i];
		parent = heap->tab[(i - 1) / 2];
	}
}

BinHeap* BinNode_insert(BinHeap* heap, int id, double value)
{
	if(!heap)
		return NULL;
	if((id < 0) && (id >= heap->capacity))
		return NULL;

	BinNode node = BinNode_create(heap, id, value);
	heap->tab[heap->size] = node;
	heap->size++;
	BinHeap_percolateup(heap);
	
	return heap;
}

void BinHeap_percolatedown(BinHeap* heap)
{
	if (heap->size <= 2)
		return;
	int i = 0;
	BinNode fils = heap->tab[(2 * i) + 1], node = heap->tab[i], tmp = {0};

	if (heap->tab[(2 * i) + 1].value > heap->tab[(2 * i) + 2].value)
		fils = heap->tab[(2 * i) + 2];
	else
		fils = heap->tab[(2 * i) + 1];

	while ((i < heap->size) && (fils.value < node.value))
	{
		node = heap->tab[i];
		if (heap->tab[(2 * i) + 1].value > heap->tab[(2 * i) + 2].value)
		{
			tmp = heap->tab[i];
			heap->tab[i] = heap->tab[(2 * i) + 2];
			heap->tab[(2 * i) + 2] = tmp;
			i = (2 * i) + 2;
		}
		else
		{
			tmp = heap->tab[i];
			heap->tab[i] = heap->tab[(2 * i) + 1];
			heap->tab[(2 * i) + 1] = tmp;
			i = (2 * i) + 1;
		}

		if (heap->tab[(2 * i) + 1].value > heap->tab[(2 * i) + 2].value)
		{
			fils = heap->tab[(2 * i) + 2];
			return;
		}
		else
		{
			fils = heap->tab[(2 * i) + 1];
			return;
		}
	}
}

BinNode BinNode_remove(BinHeap* heap)
{
	BinNode res = heap->tab[0];
	heap->size--;
	heap->tab[0] = heap->tab[heap->size];

	BinHeap_percolatedown(heap);
	
	return res;
}


void BinHeap_destroy(BinHeap* heap)
{
	free(heap->tab);
	free(heap);
}

void BinHeap_print(BinHeap* heap)
{
	printf("Heap Tab : %d size\n", heap->size);
	for (int i = 0; i < heap->size; i++)
	{
		printf("%d : %f\n", heap->tab[i].id, heap->tab[i].value);
	}
}