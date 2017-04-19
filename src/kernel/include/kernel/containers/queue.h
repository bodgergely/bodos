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
	queue() : _head(NULL), _size(0){}
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
		Node* curr = _head;
		Node* prev = NULL;
		while(curr)
		{
			prev = curr;
			curr = curr->next;
		}
		curr = new Node(elem);
		if(prev)
			prev->next = curr;
		else
			_head = curr;
		if(curr)
		{
			_size++;
			return true;
		}
		else
			return false;
		/*
		Node** curr = &_head;
		while(*curr)
			*curr = (*curr)->next;
		*curr = new Node(elem);
		if(*curr)
			return true;
		else
			return false;
			*/

	}
	/*
	 * erase all nodes with the value equal to elem
	 */
	int erase(const T& elem)
	{
		Node* curr = _head;
		Node* prev = NULL;
		int count = 0;
		while(curr)
		{
			if(curr->value == elem)
			{
				if(prev)
				{
					prev->next = curr->next;
				}
				else
					_head = curr->next;

				Node* next = curr->next;
				delete curr;
				curr = next;
				_size--;
				count++;
			}
			else
			{
				prev = curr;
				curr = curr->next;
			}
		}
		return count;
	}

	T dequeue()
	{
		if(_head)
		{
			T val = _head->value;
			Node* next = _head->next;
			delete _head;
			_head = next;
			_size--;
			return val;
		}

		return T();

	}

	void print() const
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

	int size() const
	{
		return _size;
	}

private:
	Node* _head;
	int   _size;
};

