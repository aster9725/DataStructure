#pragma once

//#define _USE_LOOSE_COUPLING_
#define _ENABLE_DISPLAY_

struct node {
	struct node* parent;
	struct node* lft;
	struct node* rgt;
#ifdef _USE_LOOSE_COUPLING_
	int data;
#endif
}__attribute((aligned(sizeof(long))));

struct tree {
	struct node* pRoot;
	int cnt;
};

#define tree_entry(ptr, type, member)	((type*)((char*)(ptr) - (unsigned long) & (((type*)0)->member)))

void tree_insert(struct tree* pTree, struct node* pNewNode);
struct node* tree_delete(struct tree* pTree, struct node* target, void (*copy)(void* target, void* replacement));
void* tree_search(struct tree* pTree, int data);

//
// static function list
//
//static struct node* find_replacement(struct node* pTarget);
//static void transplant(struct tree* pTree, struct node* dst, struct node* src);
