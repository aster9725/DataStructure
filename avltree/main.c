#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "avl.h"

void _inorderPrint_avl(struct avlnode* parent, int* depth);
void display_avl(struct avltree* pTree);

#ifndef _USE_LOOSE_COUPLING_
typedef struct _data {
	int key;
	struct avlnode avlt;
}DATA;

void avl_copyData(void* target, void* replacement);

int main()
{
	DATA* newData;
	struct avltree avlTree = { 0 };
	int i = 0, data;

	srand(time(NULL));

	for (i = 0; i < 2000; i++)
	{
		data = rand() % 64;
		//data = i % 32;
		if (newData = avl_search(&avlTree, data))
		{
			newData = avl_entry(avl_delete(&avlTree, &newData->avlt, avl_copyData), DATA, avlt);
			free(newData);
			//continue;
		}
		else
		{
			newData = (DATA*)malloc(sizeof(DATA));
			if (!newData)
				continue;

			newData->key = data;
			newData->avlt.lft = newData->avlt.rgt = NULL;
			newData->avlt.pParentBF = 0;

			avl_insert(&avlTree, &newData->avlt);
		}
#ifdef _ENABLE_DISPLAY_
		printf("\n------------------\nSTEP %d\n", i);
		display_avl(&avlTree);
		usleep(250000);
#endif
	}
	return 0;
}

void _inorderPrint_avl(struct avlnode* parent, int* depth)
{
	if (parent->lft) {
		(*depth)++;
		_inorderPrint_avl(parent->lft, depth);
	}

	printf("%*d\n", (*depth) * 4, avl_entry(parent, DATA, avlt)->key);

	if (parent->rgt) {
		(*depth)++;
		_inorderPrint_avl(parent->rgt, depth);
	}
	(*depth)--;
}

void display_avl(struct avltree* pTree)
{
	int depth = 0;
	if (pTree->pRoot)
		_inorderPrint_avl(pTree->pRoot, &depth);
}

void avl_copyData(void* target, void* replacement)
{
	DATA* pTarget, * pReplacement;
	pTarget = avl_entry(target, DATA, avlt);
	pReplacement = avl_entry(replacement, DATA, avlt);
	pTarget->key = pReplacement->key;
}

void avl_insert(struct avltree* pTree, struct avlnode* newNode)
{
	struct avlnode** probe;
	DATA* pData = avl_entry(newNode, DATA, avlt);

	probe = &pTree->pRoot;

	while (*probe)
	{
		avl_set_parent(newNode, *probe);
		if (pData->key < avl_entry((*probe), DATA, avlt)->key) {
			probe = &(*probe)->lft;
		}
		else {
			probe = &(*probe)->rgt;
		}
	}

	*probe = newNode;
	rebalance(pTree, avl_get_parent(newNode));
}

void* avl_search(struct avltree* pTree, int data)
{
	struct avlnode* probe = pTree->pRoot;
	DATA* pData = NULL;

	while (probe)
	{
		pData = avl_entry(probe, DATA, avlt);
		if (pData->key == data)
			break;
		else if (pData->key < data)
			probe = probe->rgt;
		else
			probe = probe->lft;
	}
	if (!probe)
		return probe;

	return pData;
}
#endif
#ifdef _USE_LOOSE_COUPLING_
int main()
{
	struct avltree avlTree = { 0 };
	struct avlnode* newNode;
	int i = 0, data;

	srand(time(NULL));

	for (i = 0; i < 2000; i++)
	{	
		data = rand() % 64;
		
		if (newNode = avl_search(&avlTree, data))
		{
			newNode = avl_delete(&avlTree, AVLCAST(newNode), NULL);
			free(newNode);
		}
		else
		{
			newNode = AVLCAST(malloc(sizeof(struct avlnode)));
			if (!newNode)
				continue;

			newNode->data = data;
			newNode->lft = AVLCAST(newNode)->rgt = NULL;
			newNode->pParentBF = 1;

			avl_insert(&avlTree, newNode);
		}
#ifdef _ENABLE_DISPLAY_
		printf("\n------------------\nSTEP %d\n", i);
		display_avl(&avlTree);
		usleep(250000);
#endif
	}
	return 0;
}

void _inorderPrint_avl(struct avlnode* parent, int* depth)
{
	if (parent->lft) {
		(*depth)++;
		_inorderPrint_avl(parent->lft, depth);
	}

	printf("%*d\n", (*depth) * 4, parent->data);

	if (parent->rgt) {
		(*depth)++;
		_inorderPrint_avl(parent->rgt, depth);
	}
	(*depth)--;
}

void display_avl(struct avltree* pTree)
{
	int depth = 0;
	if(pTree->pRoot)
		_inorderPrint_avl(pTree->pRoot, &depth);
}
#endif

