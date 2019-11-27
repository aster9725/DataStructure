#include "binary_tree.h"

static struct node* find_replacement(struct node* pTarget)
{
	if (!pTarget->lft)
		return (struct node*)0;

	for (pTarget = pTarget->lft; pTarget->rgt; pTarget = pTarget->rgt);

	return pTarget;
}

static void transplant(struct tree* pTree, struct node* dst, struct node* src)
{
	if (!dst->parent)
		pTree->pRoot = src;
	else if (dst == dst->parent->lft)
		dst->lft = src;
	else
		dst->rgt = src;

	if (src)
		src->parent = dst->parent;
}

#ifdef _USE_LOOSE_COUPLING_
void tree_insert(struct tree* pTree, struct node* pNewNode)
{
	struct node** probe;
	probe = &pTree->pRoot;

	while (*probe)
	{
		pNewNode->parent = *probe;
		if (pNewNode->data < (*probe)->data)
			probe = &(*probe)->lft;
		else
			probe = &(*probe)->rgt;
	}
	*probe = pNewNode;
	pTree->cnt++;
}
#endif

struct node* tree_delete(struct tree* pTree, struct node* target, void (*copy)(void* target, void* replacement))
{
	struct node *replacement, *subtree;

	if (!target)
		return target;

	if (target->lft)
		replacement = find_replacement(target);
	else
		replacement = target;

	if (replacement->lft)
		subtree = replacement->lft;
	else
		subtree = replacement->rgt;

	if(subtree)
		subtree->parent = replacement->parent;

	if (replacement->parent)
	{
		if (replacement == replacement->parent->lft)
			replacement->parent->lft = subtree;
		else
			replacement->parent->rgt = subtree;
	}
	else
		pTree->pRoot = subtree;

#ifdef _USE_LOOSE_COUPLING_
	if (replacement != target) {
		target->data ^= replacement->data;
		replacement->data ^= target->data;
		target->data ^= replacement->data;
	}
#else
	if (replacement != target)
		copy(target, replacement);
#endif

	return replacement;
}

#ifdef _USE_LOOSE_COUPLING_
void* tree_search(struct tree* pTree, int data)
{
	struct node* probe = pTree->pRoot;

	while (probe)
	{
		if (probe->data == data)
			break;
		else if (probe->data < data)
			probe = probe->rgt;
		else
			probe = probe->lft;
	}
	return probe;
}
#endif
