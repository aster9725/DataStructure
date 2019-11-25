#include "avl.h"

static unsigned long get_height(struct avlnode* probe)
{
	int lft, rgt;
	if (!probe)
		return 0;
	lft = get_height(probe->lft) + 1;
	rgt = get_height(probe->rgt) + 1;

	return lft > rgt ? lft : rgt;
}

static void right_rotate(struct avltree* pTree, struct avlnode* target)
{
	struct avlnode* lft = target->lft;
	struct avlnode* parent = avl_get_parent(target);

	target->lft = lft->rgt;
	if(lft->rgt)
		avl_set_parent(lft->rgt, target);

	avl_set_parent(lft, avl_get_parent(target));
	if (!parent)
		pTree->pRoot = lft;
	else if (parent->lft == target)
		parent->lft = lft;
	else
		parent->rgt = lft;

	lft->rgt = target;
	avl_set_parent(target, lft);

	if (!avl_get_parent(lft))
		pTree->pRoot = lft;

	avl_set_bf(target, get_height(target->lft) - get_height(target->rgt) + 1);
}

static void left_rotate(struct avltree* pTree, struct avlnode* target)
{
	struct avlnode* rgt = target->rgt;
	struct avlnode* parent = avl_get_parent(target);

	target->rgt = rgt->lft;
	if(rgt->lft)
		avl_set_parent(rgt->lft, target);

	avl_set_parent(rgt, avl_get_parent(target));
	if (!parent)
		pTree->pRoot = rgt;
	else if (parent->lft == target)
		parent->lft = rgt;
	else
		parent->rgt = rgt;

	rgt->lft = target;
	avl_set_parent(target, rgt);

	if (!avl_get_parent(rgt))
		pTree->pRoot = rgt;

	avl_set_bf(target, get_height(target->lft) - get_height(target->rgt) + 1);
}

void rebalance(struct avltree* pTree, struct avlnode* target)
{
	unsigned long bf;

	if (!target)	// root 노드 추가시
		return;

	bf = get_height(target->lft) - get_height(target->rgt) + 1;

	avl_set_bf(target, bf);

	if (avl_get_bf(target) == 3)
	{
		if (bf == 3)
		{
			if (avl_get_bf(target->lft) < 1)
				left_rotate(pTree, target->lft);
			right_rotate(pTree, target);
		}
		else if (bf == ~0)
		{
			if (avl_get_bf(target->rgt) > 1)
				right_rotate(pTree, target->rgt);
			left_rotate(pTree, target);
		}
	}
	
	rebalance(pTree, avl_get_parent(target));
}

static struct avlnode* find_replacement(struct avlnode* target)
{
	if (!target->lft)
		return (struct avlnode*)0;

	for (target = target->lft; target->rgt != (struct avlnode*)0; target = target->rgt);
	
	return target;
}

static void transplant(struct avltree* pTree, struct avlnode* dst, struct avlnode* src)
{
	if (!avl_get_parent(dst))
		pTree->pRoot = src;
	else if (dst == avl_get_parent(dst)->lft)
		avl_get_parent(dst)->lft = src;
	else
		avl_get_parent(dst)->rgt = src;

	if(src)
		avl_set_parent(src, avl_get_parent(dst));
}

struct avlnode* avl_delete(struct avltree* pTree, struct avlnode* target, void (*copy)(void* target, void* replacement))
{
	struct avlnode* replacement;

	if (!target)
		return target;

	replacement = find_replacement(target);

	if (!replacement)
	{	// 대체 노드가 없다면 삭제 노드의 오른쪽 서브트리를 연결
		if (target->rgt)
			avl_set_parent(target->rgt, avl_get_parent(target));

		if (target == pTree->pRoot)
		{
			pTree->pRoot = target->rgt;
			if (target->rgt)
				avl_set_parent(target->rgt, 0);
		}
		else if (target == avl_get_parent(target)->lft)
			avl_get_parent(target)->lft = target->rgt;
		else
			avl_get_parent(target)->rgt = target->rgt;
	}
	else
	{	// 대체 노드가 있다면 부모와 왼쪽 서브트리를 연결
		if (replacement->lft)
			avl_set_parent(replacement->lft, avl_get_parent(replacement));

		if (replacement == avl_get_parent(replacement)->lft)
			avl_get_parent(replacement)->lft = replacement->lft;
		else
			avl_get_parent(replacement)->rgt = replacement->lft;

		// 대체 노드의 데이터와 삭제 노드의 데이터 교환
#ifdef _USE_LOOSE_COUPLING_
		target->data = replacement->data;
#else
		copy(target, replacement);
#endif
		target = replacement;
	}

	if (pTree->pRoot)
		rebalance(pTree, avl_get_parent(target));

	return target;
}

#ifdef _USE_LOOSE_COUPLING_
void avl_insert(struct avltree* pTree, struct avlnode* newNode)
{
	struct avlnode** probe;
	probe = &pTree->pRoot;

	while (*probe)
	{
		avl_set_parent(newNode, *probe);
		if (newNode->data < (*probe)->data) {
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
