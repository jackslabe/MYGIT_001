/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê12ÔÂ22ÈÕ
* Compiler   : gcc
* FILE NAME  : dlist.h
*
* DESCRIPTION:
*
*/
#ifndef DLIST_H_
#define DLIST_H_

typedef struct _DlistNode{
	struct _DlistNode *prev;
	struct _DlistNode *next;
	void *data;
}DlistNode;

void dlist_insert_node(DlistNode *new_node, DlistNode *node);
void dlist_insert_tail(DlistNode *new_node, DlistNode *tail);
void dlist_destroy(DlistNode *node);

#endif /* DLIST_H_ */
