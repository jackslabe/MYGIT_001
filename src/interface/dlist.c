/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê12ÔÂ22ÈÕ
* Compiler   : gcc
* FILE NAME  : dlist.c
*
* DESCRIPTION:
*
*/

#include "dlist.h"



#define NULL	(void *)0

void dlist_insert_node(DlistNode *new_node, DlistNode *node){
	DlistNode *temp;

	if((new_node != NULL) && (node != NULL)){
		temp = node->next;
		node->next = new_node;
		new_node->prev = node;
		new_node->next = temp;
		temp->prev = new_node;
	}
}

void dlist_insert_tail(DlistNode *new_node, DlistNode *tail){

	if((new_node != NULL) && (tail != NULL)){
		tail->prev->next = new_node;
		new_node->prev = tail->prev;
		tail->prev = new_node;
		new_node->next = tail;
	}
}


void dlist_destroy(DlistNode *node){

	if(node != NULL){
		node->prev->next = node->next;
		node->next->prev = node->prev;
		node->next = NULL;
		node->prev = NULL;
	}
}

