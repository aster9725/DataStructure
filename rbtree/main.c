#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "RBTree.h"

void _inorderPrint_rb(struct rbnode* parent, int* depth);
void display_rb(struct rbtree* pTree);


#ifndef _USE_LOOSE_COUPLING_
typedef struct _data {
	int key;
	struct rbnode rbt;
}DATA;

void rb_copyData(void* target, void* replacement);

int main()
{
	DATA* newData;
	const struct rbnode* rbNIL = rb_get_nil();
	struct rbtree rbTree = { rbNIL, };
	int i = 0, data;

	srand(time(NULL));

	for (i = 0; i < 2000; i++)
	{
		data = rand() % 64;
		if (newData = rb_search(&rbTree, data))
		{
			newData = rb_entry(rb_delete(&rbTree, &newData->rbt, rb_copyData), DATA, rbt);
			free(newData);
		}
		else
		{
			newData = (DATA*)malloc(sizeof(DATA));
			if (!newData)
				continue;

			newData->key = data;
			newData->rbt.lft = newData->rbt.rgt = rbNIL;
			newData->rbt.pParentColor = (unsigned long)rbNIL;

			rb_insert(&rbTree, &newData->rbt);
		}
#ifdef _ENABLE_DISPLAY_
		printf("\n------------------\nSTEP %d\n", i);
		display_rb(&rbTree);
		usleep(250*1000);
#endif
	}
	return 0;
}

void _inorderPrint_rb(struct rbnode* parent, int* depth)
{
	if (parent->lft != rb_get_nil()) {
		(*depth)++;
		_inorderPrint_rb(parent->lft, depth);
	}

	if (rb_get_color(parent))
		printf("%*d^\n", (*depth) * 4, rb_entry(parent, DATA, rbt)->key);
	else
		printf("%*d\n", (*depth) * 4, rb_entry(parent, DATA, rbt)->key);

	if (parent->rgt != rb_get_nil()) {
		(*depth)++;
		_inorderPrint_rb(parent->rgt, depth);
	}
	(*depth)--;
}

void display_rb(struct rbtree* pTree)
{
	int depth = 0;
	if (pTree->pRoot != rb_get_nil())
		_inorderPrint_rb(pTree->pRoot, &depth);
}

void rb_copyData(void* target, void* replacement)
{
	DATA* pTarget, * pReplacement;
	pTarget = rb_entry(target, DATA, rbt);
	pReplacement = rb_entry(replacement, DATA, rbt);
	pTarget->key = pReplacement->key;
}

void rb_insert(struct rbtree* pTree, struct rbnode* pNewNode)
{
	struct rbnode** probe;
	DATA* pData = rb_entry(pNewNode, DATA, rbt);

	probe = &pTree->pRoot;

	while (*probe != rb_get_nil())
	{
		rb_set_parent(pNewNode, *probe);
		if (pData->key < rb_entry((*probe), DATA, rbt)->key)
			probe = &(*probe)->lft;
		else
			probe = &(*probe)->rgt;
	}
	*probe = pNewNode;
	pTree->cnt++;
	insert_fixup(pTree, pNewNode);
}

void* rb_search(struct rbtree* pTree, int data)
{
	struct rbnode* probe = pTree->pRoot;
	DATA* pData = NULL;

	while (probe != rb_get_nil())
	{
		pData = rb_entry(probe, DATA, rbt);
		if (pData->key == data)
			break;
		else if (pData->key < data)
			probe = probe->rgt;
		else
			probe = probe->lft;
	}


	if (probe == rb_get_nil())
		return NULL;
	else
		return pData;
}
#endif

#ifdef _USE_LOOSE_COUPLING_
int main()
{
	const struct rbnode* rbNIL = rb_get_nil();
	struct rbtree rbTree = { rbNIL, };
	struct rbnode* newNode;
	int i = 0, data;

	srand(time(NULL));

	for (i=0; i < 2000; i++)
	{
		data = rand() % 64;

		if (newNode = rb_search(&rbTree, data))
		{
			newNode = rb_delete(&rbTree, newNode, NULL);
			free(newNode);
		}
		else
		{
			newNode = RBCAST(malloc(sizeof(struct rbnode)));
			if (!newNode)
				continue;

			newNode->data = data;
			newNode->lft = newNode->rgt = rbNIL;
			newNode->pParentColor = (unsigned long)rbNIL;

			rb_insert(&rbTree, newNode);
		}

#ifdef _ENABLE_DISPLAY_
		printf("\n------------------\nSTEP %d\n", i);
		display_rb(&rbTree);
		usleep(250*1000);
#endif
		
	}

	return 0;
}

void _inorderPrint_rb(struct rbnode* parent, int* depth)
{
	if (parent->lft != rb_get_nil()) {
		(*depth)++;
		_inorderPrint_rb(parent->lft, depth);
	}

	if(rb_get_color(parent))
		printf("%*d^\n", (*depth) * 4, parent->data);
	else
		printf("%*d\n", (*depth) * 4, parent->data);

	if (parent->rgt != rb_get_nil()) {
		(*depth)++;
		_inorderPrint_rb(parent->rgt, depth);
	}
	(*depth)--;
}

void display_rb(struct rbtree* pTree)
{
	int depth = 0;
	if (pTree->pRoot)
		_inorderPrint_rb(pTree->pRoot, &depth);
}
#endif

