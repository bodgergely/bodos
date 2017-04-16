/*
 * queue.h
 *
 *  Created on: Apr 15, 2017
 *      Author: geri
 */

#pragma once

#define NULL 0

#include <kernel/mem/kmalloc.h>

template<class T>
class queue
{
struct Node
{
	Node() : next(NULL) {}
	Node(const T& val) : value(val), next(NULL) {}
	T     value;
	Node* next;
};
public:
	queue() : _head(NULL){}
	~queue()
	{
		Node* node = _head;
		while(node)
		{
			Node* next = node->next;
			delete node;
			node = next;
		}

	}
	bool insert(const T& elem)
	{
		Node** curr = &_head;
		while(*curr)
			*curr = (*curr)->next;
		*curr = new Node(elem);
		if(*curr)
			return true;
		else
			return false;

	}
	T dequeue()
	{
		if(_head)
		{
			T val = _head->value;
			Node* next = _head->next;
			delete _head;
			_head = next;
			return val;
		}

		return T();

	}

	void print()
	{
		Node* node = _head;
		kprintf("queue content:\n");
		while(node)
		{
			kprintf("Value at node: %d is: %d ",node, node->value);
			node = node->next;
		}
		kprintf("\n");
	}

	const T& top()
	{
		if(_head)
		{
			return _head->value;
		}
		return T();
	}

private:
	Node* _head;
};

