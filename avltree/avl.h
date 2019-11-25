#pragma once

#define _USE_LOOSE_COUPLING_
#define _ENABLE_DISPLAY_
struct avlnode {
#ifdef _USE_LOOSE_COUPLING_
	int data;
#endif
	unsigned long pParentBF;
	struct avlnode* lft;
	struct avlnode* rgt;
}__attribute__((aligned(sizeof(long))));

struct avltree {
	struct avlnode* pRoot;
};

#define AVLCAST(name)	((struct avlnode*)name)

#define avl_entry(ptr, type, member)	((type*)((char*)ptr - (unsigned long) & (((type*)0)->member)))

#define avl_get_parent(node)			((struct avlnode*)((node)->pParentBF & ~3))
#define avl_set_parent(node, parent)	((node)->pParentBF = ((node)->pParentBF & 3) | (unsigned long)(parent))
#define avl_get_bf(node)				((node)->pParentBF & 3)
#define avl_set_bf(node, bf)			((node)->pParentBF = ((node)->pParentBF & ~3) | (unsigned long)((bf) & 3))

// Because avlnode was packed, lowest 2 bit will be 0
// I'll use these two bits for stroing BF(Balance Factor)
//
//          balanced
//       |------------|
// 11    10    01    00    11
//    +1    +1    -1    -1
// 
// If BF set 3(0b11), tree is unbalanced

#define avl_add_bf(node)				((node)->pParentBF = (unsigned long)avl_get_parent(node) | (((unsigned long)avl_get_bf(node) + 1) & 3))
#define avl_sub_bf(node)				((node)->pParentBF = (unsigned long)avl_get_parent(node) | (((unsigned long)avl_get_bf(node) - 1) & 3))

void rebalance(struct avltree* pTree, struct avlnode* target);
void avl_insert(struct avltree* pTree, struct avlnode* newNode);
struct avlnode* avl_delete(struct avltree* pTree, struct avlnode* target, void (*copy)(void* target, void* replacement));
void* avl_search(struct avltree* pTree, int data);

// static functino list
//
//static unsigned long get_height(struct avlnode* probe);
//static void right_rotate(struct avltree* pTree, struct avlnode* target);
//static void left_rotate(struct avltree* pTree, struct avlnode* target);

//static struct avlnode* find_replacement(struct avlnode* target);
//static void transplant(struct avltree* pTree, struct avlnode* dst, struct avlnode* src);
