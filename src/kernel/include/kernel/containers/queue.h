/*
 * queue.h
 *
 *  Created on: Apr 15, 2017
 *      Author: geri
 */

#pragma once

#define NULL 0

#include <kernel/mem/kmalloc.h>

/*
 * https://en.wikipedia.org/wiki/Heap_(data_structure)
 *
 * Thus the children of the node at position n would be at positions 2n + 1 and 2n + 2 in a zero-based array.
 * This allows moving up or down the tree by doing simple index computations.
 * Balancing a heap is done by sift-up or sift-down operations (swapping elements which are out of order).
 *
 * Different types of heaps implement the operations in different ways, but notably,
 * insertion is often done by adding the new element at the end of the heap in the first available free space.
 * This will generally violate the heap property, and so the elements are then sifted up until the heap property has been reestablished.
 * Similarly, deleting the root is done by removing the root and then putting the last element in the root and sifting down to rebalance.
 * Thus replacing is done by deleting the root and putting the new element in the root and sifting down,
 * avoiding a sifting up step compared to pop (sift down of last element) followed by push (sift up of new element).
 */

namespace bodos
{

template<class K, class V>
struct key_val
{
	key_val() {}
	key_val(const K& key_, const V& val_) : key(key_), val(val_) {}
	K key;
	V val;
};

template<class K, class V>
class priority_queue
{
	using keyval = key_val<K, V>;
public:
	priority_queue() : _size(5), _count(0)
	{
		_mem = new keyval[_size];
	}
	priority_queue(int start_size) : _size(start_size), _count(0)
	{
		_mem = new keyval[_size];
	}
	~priority_queue()
	{
		kfree(_mem);
		//klog(INFO, "Freed mem in ~priority_queue\n");

		_mem = NULL;
	}


	bool insert(const K& key, const V& val)
	{
		insert(keyval(key, val));
	}

	bool insert(const key_val<K, V>& elem)
	{
		//klog(INFO, "Inserting elem into prio queue\n");
		if(_count == _size)
		{
			increase_mem(_size * 2);
		}
		int currpos = _count;
		_mem[_count++] = elem;
		// now we might have violated the heap property
		swim(currpos);
		//klog(INFO, "Insert done. _count: %d, size: %d, mem: %d\n", _count, _size, _mem);

	}

	/*
	 * delete all key-val pair wich has the given value
	 */
	int erase(const V& val)
	{
		priority_queue<K, V>* pq = new priority_queue<K, V>();
		int c = _count;
		int nc = 0;
		for(int i = 0;i<c;i++)
		{
			if(_mem[i].val!=val)
			{
				pq->insert(_mem[i]);
				nc++;
			}
		}

		kfree(_mem);
		_mem = pq->_mem;
		_size = pq->_size;
		_count = pq->_count;
		kfree(pq);
		return c - nc;
	}
	key_val<K, V> dequeue()
	{
		--_count;
		keyval kv = _mem[0];
		int currpos = 0;
		_mem[currpos] = _mem[_count];
		sink(currpos);
		return kv;
	}
	void print() const
	{
		kprintf("Priority Queue content: \n");
		for(int i=0;i<_count;i++)
		{
			kprintf("k: %d v: %d [%d] ", _mem[i].key, _mem[i].val, &_mem[i]);
		}
		kprintf("\n");
	}
	const key_val<K, V> top()
	{
		return _mem[0];
	}

	int size() const
	{
		return _count;
	}

	// debug
	bool is_correct()
	{
		int limit = 0;
		limit = ((_count - 1) - 1) / 2;

		for(int i=0;i<=limit;i++)
		{
			if(_mem[i].key < _mem[i*2+1].key || (i*2+2 < _count && _mem[i].key < _mem[i*2+2].key))
			{
				kprintf("%d [%d] is smaller than either: %d [%d] or %d [%d]\n", _mem[i].key, i, _mem[i*2+1].key, i*2+1, _mem[i*2+2].key, i*2+2);
				while(1);
			}
		}

	}

private:
	void increase_mem(int size)
	{
		keyval* t = new keyval[size];
		//klog(INFO, "increased mem in prioirty_queue. original mem: %d and new mem: %d", _mem, t);
		for(int i=0;i<_count;i++)
		{
			t[i] = _mem[i];
		}
		kfree(_mem);
		_mem = t;
		_size  = size;
	}
	inline bool odd(int c)
	{
		if(c%2) return true;
		else return false;
	}

	void swim(int pos)
	{
		const keyval kv = _mem[pos];
		K key = kv.key;
		int parpos = parent(pos);
		const keyval& par = _mem[parpos];
		if(pos == 0 || par.key >= key)
			return;

		_mem[pos] = par;
		_mem[parpos] = kv;
		swim(parpos);
	}

	int parent(int pos)
	{
		int parpos;
		if(odd(pos))
			parpos = (pos - 1) / 2;
		else
			parpos = (pos - 2) / 2;

		return parpos;
	}

	void children(int pos, int& left, int& right)
	{
		left = 2 * pos + 1;
		right = 2 * pos + 2;
		if(left >= _count)
		{
			left = -1;
			right = -1;
		}
		else if(right >= _count)
		{
			right = -1;
		}
	}

	void sink(int pos)
	{
		keyval kv = _mem[pos];
		K key = kv.key;
		int l, r;
		children(pos, l, r);
		if(l == -1 && r == -1)
			return;
		keyval* child;
		int newpos;
		if(r == -1)
		{
			child = &_mem[l];
			newpos = l;
		}
		else
		{
			if(_mem[l].key >= _mem[r].key)
			{
				child = &_mem[l];
				newpos = l;
			}
			else
			{
				child = &_mem[r];
				newpos = r;
			}
		}

		if(key >= _mem[newpos].key)
			return;

		_mem[pos] = *child;
		*child = kv;
		sink(newpos);
	}

private:
	key_val<K, V>*	_mem;
	int 			_size;
	int				_count;
};


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
			kfree(node);
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
				kfree(curr);
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
			kfree(_head);
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

	const T top()
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

}