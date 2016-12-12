#ifndef THLIST_H
#define THLIST_H

#include "THObject.h"
#include "THDefine.h"
#include <algorithm>

namespace THEngine
{
	template <class T> class Iterator;
	template <class T> class List;
	template <class T> class LinkedListIterator;
	template <class T> class LinkedList;
	template <class T> class ArrayList;
	template <class T> class ArrayListIterator;
	
	template <class T>
	class Iterator
	{
	public:
		virtual bool HasNext() = 0;
		virtual T Next() = 0;
		virtual void Remove() = 0;
		virtual void Set(T obj) = 0;
		virtual void AddBefore(T obj) = 0;
		virtual void AddAfter(T obj) = 0;
	};

	template <class T>
	class List
	{
	public:
		virtual ~List() {}
		virtual void Add(T obj) = 0;
		virtual void AddAt(T obj, int index) = 0;
		virtual int Size() = 0;
		virtual void Set(T obj, int index) = 0;
		virtual T Get(int index) = 0;
		virtual void Remove(T obj) = 0;
		virtual void RemoveAt(int index) = 0;
		virtual void Clear() = 0;
		virtual Iterator<T>* GetIterator() = 0;
	};

	template <class T>
	struct ListNode
	{
		T obj;
		ListNode<T>* next;
		ListNode<T>* prior;

		ListNode()
		{
			obj = nullptr;
		}

		~ListNode()
		{
			if (obj == nullptr)
			{
				return;
			}
			obj->Release();
		}
	};


	template <class T>
	class LinkedList : public List<T>
	{
	private:
		ListNode<T>* head;
		ListNode<T>* rear;
		int size;
		LinkedListIterator<T>* iter;

	public:
		LinkedList()
		{
			head = new ListNode<T>();
			head->next = NULL;
			head->prior = NULL;
			rear = head;
			size = 0;
			iter = NULL;
		}

		LinkedList(const LinkedList<T>& list) 
		{
			head = new ListNode<T>();
			head->prior = NULL;
			head->next = nullptr;

			ListNode<T>* p = head;
			ListNode<T>* q = list.head;
			while (q->next)
			{
				q = q->next;
				p->next = new ListNode<T>();
				p->next->prior = p;
				p = p->next;
				p->obj = q->obj;
				p->obj->Retain();
				p->next = nullptr;
			}

			rear = p;
			size = list.size;
			iter = nullptr;
		}

		virtual ~LinkedList()
		{
			auto iter = GetIterator();
			while (iter->HasNext())
			{
				iter->Next();
				iter->Remove();
			}

			TH_SAFE_DELETE(iter);

			delete head;
		}

		virtual void Add(T obj) override
		{
			ListNode<T>* node = new ListNode<T>();
			node->prior = rear;
			rear->next = node;
			rear = node;
			rear->obj = obj;
			rear->next = NULL;
			size++;
			obj->Retain();
		}

		virtual void AddAt(T obj, int index) override
		{
			ASSERT(index < size);
			auto iter = (LinkedListIterator<T>*)GetIterator();
			int i = 0;
			while (iter->HasNext() && i < index)
			{
				iter->Next();
				i++;
			}
			iter->AddAfter(obj);
			obj->Retain();
		}

		virtual int Size() override
		{
			return size;
		}

		virtual void Set(T obj, int index) override
		{
			ASSERT(index < size);
			auto iter = (LinkedListIterator<T>*)GetIterator();
			int i = 0;
			while (iter->HasNext() && i < index)
			{
				iter->Next();
				i++;
			}
			iter->Next();
			iter->Set(obj);
		}

		virtual T Get(int index) override
		{
			ASSERT(index < size);
			auto iter = (LinkedListIterator<T>*)GetIterator();
			int i = 0;
			while (iter->HasNext() && i < index)
			{
				iter->Next();
				i++;
			}
			return iter->Next();
		}

		virtual void Remove(T obj) override
		{
			auto p = head;

			while (p->next)
			{
				p = p->next;
				if (p->obj == obj)
				{
					if (p->next)
					{
						p->prior->next = p->next;
						p->next->prior = p->prior;
						delete p;
					}
					else
					{
						p->prior->next = NULL;
						rear = p->prior;
						delete p;
					}
					size--;
					return;
				}
			}
		}

		virtual void RemoveAt(int index) override
		{
			ASSERT(index < size);
			auto p = head;
			for (int i = 0; i < index; i++)
			{
				if (p->next)
				{
					p = p->next;
				}
			}
			if (p->next)
			{
				p->prior->next = p->next;
				p->next->prior = p->prior;;
				delete p;
			}
			else
			{
				p->prior->next = NULL;
				rear = p->prior;
				delete p;
			}
			size--;
		}

		virtual void Clear() override
		{
			auto iter = GetIterator();
			while (iter->HasNext())
			{
				iter->Next();
				iter->Remove();
			}
		}

		virtual Iterator<T>* GetIterator() override
		{
			if (iter)
			{
				delete iter;
			}
			iter = new LinkedListIterator<T>(this);
			return iter;
		}

		friend class LinkedListIterator<T>;
	};

	template <class T>
	class LinkedListIterator : public Iterator<T>
	{
	private:
		LinkedList<T>* list;
		ListNode<T>* pointer;

	public:
		LinkedListIterator(LinkedList<T>* list)
		{
			this->list = list;
			this->pointer = list->head;
		}

		virtual bool HasNext() override
		{
			return pointer->next != NULL;
		}

		virtual T Next() override
		{
			ASSERT(pointer->next);
			pointer = pointer->next;
			return pointer->obj;
		}

		virtual void Remove() override
		{
			ASSERT(pointer->prior);
			pointer->prior->next = pointer->next;
			if (pointer->next)
			{
				pointer->next->prior = pointer->prior;
			}
			else
			{
				list->rear = pointer->prior;
			}
			auto temp = pointer->prior;
			delete pointer;
			pointer = temp;
			list->size--;
			
		}

		virtual void Set(T obj) override
		{
			ASSERT(pointer != list->head);
			pointer->obj = obj;
		}

		virtual void AddBefore(T obj) override
		{
			ASSERT(pointer->prior);
			auto node = new ListNode<T>();
			node->prior = pointer->prior;
			node->next = pointer;
			node->obj = obj;
			pointer->prior->next = node;
			pointer->prior = node;
			list->size++;
			obj->Retain();
		}

		virtual void AddAfter(T obj) override
		{
			auto node = new ListNode<T>();
			node->prior = pointer;
			node->next = pointer->next;
			node->obj = obj;
			if (pointer->next)
			{
				pointer->next->prior = node;
			}
			pointer->next = node;
			list->size++;
			obj->Retain();
		}

		friend class LinkedListIterator<T>;
	};
	

	template <class T>
	class ArrayList : public List<T>
	{
	private:
		T* elements = nullptr;
		int size = 0;
		int capacity = 0;
		int baseSize = 100;
		ArrayListIterator<T>* iter = nullptr;

		typedef bool CompareFunc(T a, T b);

	private:
		void ExpandCapacity()   //扩容
		{
			T* newElements = new T[capacity + baseSize];
			capacity += baseSize;
			if (elements)
			{
				//拷贝旧数据到新的存储区
				memcpy(newElements, elements, size * sizeof(T*));
				delete[] elements;
			}
			elements = newElements;
		}

	public:
		ArrayList()
		{

		}

		ArrayList(const ArrayList<T>& list)
		{
			size = list.size;
			capacity = list.capacity;
			baseSize = list.baseSize;
			iter = nullptr;
			elements = new T[list.capacity];

			for (int i = 0; i < size; i++)
			{
				elements[i] = list.elements[i];
				elements[i]->Retain();
			}
		}

		ArrayList(int baseSize)
		{
			this->baseSize = baseSize;
		}

		virtual ~ArrayList()
		{
			for (int i = 0; i < size; i++)
			{
				TH_SAFE_RELEASE(elements[i]);
			}
			if (elements)
			{
				delete[] elements;
			}
			if (iter)
			{
				delete iter;
			}
		}

		virtual void Add(T obj) override
		{
			if (size >= capacity)
			{
				//容量不够，需要扩容
				ExpandCapacity();
			}
			elements[size] = obj;
			size++;
			obj->Retain();
		}

		virtual void AddAt(T obj, int index) override
		{
			ASSERT(index <= size);
			if (size >= capacity)
			{
				//容量不够，需要扩容
				ExpandCapacity();
			}
			for (int i = size; i > index; i--)
			{
				elements[i] = elements[i - 1];
			}
			elements[index] = obj;
			size++;
			obj->Retain();
		}

		virtual int Size() override
		{
			return size;
		}

		virtual void Set(T obj, int index) override
		{
			ASSERT(index < size);
			elements[index] = obj;
		}

		virtual T Get(int index) override
		{
			ASSERT(index < size);
			return elements[index];
		}

		virtual void Remove(T obj) override
		{
			for (int i = 0; i < size; i++)
			{
				if (elements[i] == obj)
				{
					for (int j = i; j < size - 1; j++)
					{
						elements[j] = elements[j + 1];
					}
					size--;
					obj->Release();
					return;
				}
			}
		}

		virtual void RemoveAt(int index) override
		{
			ASSERT(index < size);
			elements[index]->Release();
			for (int i = index; i < size - 1; i++)
			{
				elements[i] = elements[i + 1];
			}
			size--;
		}

		virtual void Clear() override
		{		
			auto iter = GetIterator();
			while (iter->HasNext())
			{
				iter->Next()->Release();
			}
			size = 0;
		}


		inline void Sort(int begin, int end, CompareFunc compare)
		{
			std::stable_sort(elements + begin, elements + end, compare);
		}

		virtual Iterator<T>* GetIterator() override
		{
			if (iter)
			{
				delete iter;
			}
			iter = new ArrayListIterator<T>(this);
			return iter;
		}

		inline void SetBaseSize(int baseSize)
		{
			this->baseSize = baseSize;
		}

		friend class ArrayListIterator<T>;
	};

	template <class T>
	class ArrayListIterator : public Iterator<T>
	{
	private:
		int pointer;
		ArrayList<T>* list;

	public:
		ArrayListIterator(ArrayList<T>* list)
		{
			this->list = list;
			pointer = -1;
		}

		virtual ~ArrayListIterator()
		{

		}

		virtual bool HasNext() override
		{
			return pointer < list->size - 1;
		}

		virtual T Next() override
		{
			pointer++;
			ASSERT(pointer < list->size);
			return list->elements[pointer];
		}

		virtual void Remove() override
		{
			ASSERT(pointer >= 0 && pointer < list->size);
			list->elements[pointer]->Release();
			for (int i = pointer; i < list->size - 1; i++)
			{
				list->elements[i] = list->elements[i + 1];
			}
			list->size--;
		}

		virtual void Set(T obj) override
		{
			ASSERT(pointer >= 0 && pointer <= list->size);
			list->elements[pointer] = obj;
		}

		virtual void AddBefore(T obj) override
		{
			ASSERT(pointer > 0);
			list->AddAt(obj, pointer);
		}

		virtual void AddAfter(T obj) override
		{
			list->AddAt(obj, pointer + 1);
		}
	};
}

#endif