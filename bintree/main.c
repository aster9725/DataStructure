#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "binary_tree.h"

#ifdef _USE_LOOSE_COUPLING_
void _inorderPrint_tree(struct node* parent, int* dept);
void display_tree(struct tree* pTree);

int main()
{
	int i, data;
	struct node* newNode;
	struct tree binTree = { 0 };

	srand(time(NULL));

	for (i = 0; i < 200; i++)
	{
		data = rand() % 16;
		
		if (newNode = tree_search(&binTree, data))
		{
			newNode = tree_delete(&binTree, newNode, NULL);
			free(newNode);
			//continue;
		}
		else
		{
			newNode = (struct node*)malloc(sizeof(struct node));
			if (newNode)
			{
				newNode->data = data;
				newNode->parent = newNode->lft = newNode->rgt = NULL;
				tree_insert(&binTree, newNode);
			}
		}
#ifdef _ENABLE_DISPLAY_
		printf("\n------------------\nSTEP %d\n", i);
		display_tree(&binTree);
		usleep(250*1000);
		//getchar();
#endif
	}

	return 0;
}
#else

typedef struct _data {
	int data;
	struct node bint;
}DATA;

void _inorderPrint_tree(struct node* parent, int* dept);
void display_tree(struct tree* pTree);
void copyData(void* target, void* replacement);

int main()
{
	int i, data;
	DATA* newNode;
	struct tree binTree = { 0 };

	srand(time(NULL));

	for (i = 0; i < 200; i++)
	{
		data = rand() % 16;

		if (newNode = tree_search(&binTree, data))
		{
			newNode = tree_entry(tree_delete(&binTree, (struct node*)newNode, copyData), DATA, bint);
			free(newNode);
			//continue;
		}
		else
		{
			newNode = (DATA*)malloc(sizeof(DATA));
			if (newNode)
			{
				newNode->data = data;
				newNode->bint.parent = newNode->bint.lft = newNode->bint.rgt = NULL;
				tree_insert(&binTree, &newNode->bint);
			}
		}
#ifdef _ENABLE_DISPLAY_
		printf("\n------------------\nSTEP %d\n", i);
		display_tree(&binTree);
		usleep(250*1000);
		//getchar();
#endif
	}

	return 0;
}

#endif

void tree_insert(struct tree* pTree, struct node* pNewNode)
{
	struct node** probe = &pTree->pRoot;
	DATA* pData = tree_entry(pNewNode, DATA, bint);

	while (*probe)
	{
		pNewNode->parent = *probe;
		if (pData->data < tree_entry(*probe, DATA, bint)->data)
			probe = &(*probe)->lft;
		else
			probe = &(*probe)->rgt;
	}
	*probe = pNewNode;
	pTree->cnt++;
}

void* tree_search(struct tree* pTree, int data)
{
	struct node* probe = pTree->pRoot;
	DATA* pData;

	while (probe)
	{
		pData = tree_entry(probe, DATA, bint);
		if (pData->data == data)
			break;
		else if (pData->data < data)
			probe = probe->rgt;
		else
			probe = probe->lft;
	}

	return probe;
}

void copyData(void* target, void* replacement)
{
	DATA* pTarget, *pReplacement;
	pTarget = tree_entry(target, DATA, bint);
	pReplacement = tree_entry(replacement, DATA, bint);

	pReplacement->data = pTarget->data;
}

void _inorderPrint_tree(struct node* parent, int* depth)
{
	if (parent->lft)
	{
		(*depth)++;
		_inorderPrint_tree(parent->lft, depth);
	}

	printf("%*d\n", (*depth) * 4, tree_entry(parent, DATA, bint)->data);

	if (parent->rgt)
	{
		(*depth)++;
		_inorderPrint_tree(parent->rgt, depth);
	}
	(*depth)--;
}

void display_tree(struct tree* pTree)
{
	int depth = 0;
	if (pTree->pRoot)
		_inorderPrint_tree(pTree->pRoot, &depth);
}
