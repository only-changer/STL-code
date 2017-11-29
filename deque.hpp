#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>
#include<iostream>
//using namespace std;
namespace sjtu {

	template<class T>
	class deque {
	public:
		struct node
		{
			T * data[2048];
			node *pre;
			node *next;
			int num;
			int beg = 1024;
			int n0 = 1024;
			node() : pre(NULL), next(NULL), num(0)
			{
			}
			node(int n) :pre(NULL), next(NULL), num(0), n0(n)
			{
			}
			node(const node *p) :pre(p->pre), next(p->next), num(p->num), n0(p->n0), beg(p->beg)
			{
				for (int i = beg; i < beg + num; ++i) data[i] = new T(*p->data[i]);
			}
			~node()
			{
				for (int i = beg; i < beg + num; ++i) if (data[i]) delete data[i];
			}
		};
		int max(int x, int y)
		{
			if (x > y) return x;
			else return y;
		}
		void copy(const deque &other)
		{
			node *p = other.head->next;
			node *q = head;
			while (p != other.tail)
			{
				node *t = new node(p);
				q->next = t;
				t->pre = q;
				q = t;
				p = p->next;
			}
			q->next = tail;
			tail->pre = q;
		}
		const T* const_find(const size_t &i)const
		{
			node *p = head->next;
			int k = 0;
			int j = i;
			while (p != tail)
			{
				if (j >= p->num) j -= p->num;
				else return (p->data[j + p->beg]);
				p = p->next;
			}
		}
		T* find(const size_t &i)
		{
			node *p = head->next;
			int k = 0;
			int j = i;
			while (p != tail)
			{
				if (j >= p->num) j -= p->num;
				else return (p->data[j + p->beg]);
				p = p->next;
			}
		}
		node * divide(node *p)
		{
			int k = 500;

			node *r = new node((p->n0) / 2 + 1);


			r->pre = p;
			r->next = p->next;
			p->next->pre = r;
			r->num = p->num - k;
			p->next = r;
			r->n0 = (p->n0) / 2 + 1;


			for (int i = k; i < p->num; ++i) r->data[i - k+r->beg] = p->data[i + p->beg];
			if (p->beg > 1024) for (int i = 0; i < k; ++i) p->data[i + 1024] = p->data[i + p->beg];
			else if (p->beg < 1024) for (int i = k - 1; i >= 0; --i) p->data[i + 1024] = p->data[i + p->beg];
			p->beg = 1024;
			p->num = k;
			return r;
		}

	public:
		node *head;
		node *tail;
		int length;
		int lc = 1;
		class const_iterator;
		class iterator {
		public:
			size_t i;
			deque *d;

			iterator() :i(0), d(NULL) {}
			iterator(deque *p) :d(p) {}
			iterator operator+(const int &n)
			{
				i += n;
				return *this;
			}
			iterator operator-(const int &n)
			{
				i -= n;
				return *this;
			}
			int operator-(const iterator &rhs) const
			{
				if (rhs.d != d) throw invalid_iterator();
				return (i - rhs.i);
			}
			iterator operator+=(const int &n)
			{
				i += n;
				return *this;
			}
			iterator operator-=(const int &n)
			{
				i -= n;
				return *this;
			}
			iterator operator++(int)
			{
				iterator k(d);
				k.i = i;
				++i;
				return k;
			}
			iterator& operator++()
			{
				++i;
				return *this;
			}
			iterator operator--(int)
			{
				iterator k(d);
				k.i = i;
				--i;
				return k;
			}
			iterator& operator--()
			{
				--i;
				return *this;
			}
			T& operator*() const
			{
				if (i >= 0 && i < d->length) return *(d->find(i));
				else throw invalid_iterator();
			}
			T* operator->() const noexcept
			{
				if (i >= 0 && i < d->length) return (d->find(i));
				else throw invalid_iterator();
			}
			bool operator==(const iterator &rhs) const
			{
				return (i == rhs.i);
			}
			bool operator==(const const_iterator &rhs) const
			{
				return (i == rhs.i);
			}
			bool operator!=(const iterator &rhs) const
			{
				return (i != rhs.i);
			}
			bool operator!=(const const_iterator &rhs) const
			{
				return (i != rhs.i);
			}
		};
		class const_iterator {
		public:
			size_t i;
			const deque *d;
			// it should has similar member method as iterator.
			//  and it should be able to construct from an iterator.
		private:
			// data members.
		public:
			const_iterator() :i(0), d(NULL) {}
			const_iterator(const deque *m) :d(m) {}
			const_iterator(const const_iterator &other) :d(other.d), i(other.i) {}
			const_iterator(const iterator &other) :d(other.d), i(other) {}
			const_iterator operator+(const int &n)
			{
				i += n;
				return *this;
			}
			const_iterator operator-(const int &n)
			{
				i -= n;
				return *this;
			}
			int operator-(const const_iterator &rhs) const
			{
				if (rhs.d != d) throw invalid_iterator();
				return (i - rhs.i);
			}
			const_iterator operator+=(const int &n)
			{
				i += n;
				return *this;
			}
			const_iterator operator-=(const int &n)
			{
				i -= n;
				return *this;
			}
			const_iterator operator++(int)
			{
				const_iterator k(d);
				k.i = i;
				++i;
				return k;
			}
			const_iterator& operator++()
			{
				++i;
				return *this;
			}
			const_iterator operator--(int)
			{
				const_iterator k(d);
				k.i = i;
				--i;
				return k;
			}
			const_iterator& operator--()
			{
				--i;
				return *this;
			}
			const T& operator*() const
			{
				if (i >= 0 && i < d->length) return *(d->const_find(i));
				else throw invalid_iterator();
			}
			const T* operator->() const noexcept
			{
				if (i >= 0 && i < d->length) return (d->const_find(i));
				else throw invalid_iterator();
			}
			bool operator==(const iterator &rhs) const
			{
				return (i == rhs.i);
			}
			bool operator==(const const_iterator &rhs) const
			{
				return (i == rhs.i);
			}
			bool operator!=(const iterator &rhs) const
			{
				return (i != rhs.i);
			}
			bool operator!=(const const_iterator &rhs) const
			{
				return (i != rhs.i);
			}
		};
		/**
		* TODO Constructors
		*/
		deque()
		{
			head = new node;
			tail = new node;
			head->next = tail;
			tail->pre = head;
			length = 0;
		}
		deque(const deque &other)
		{

			head = new node;
			tail = new node;
			head->next = tail;
			tail->pre = head;
			length = other.length;
			copy(other);
		}
		/**
		* TODO Deconstructor
		*/
		~deque()
		{
			//cout<<"!"<<endl;
			clear();
			//cout<<"!"<<endl;
			if (head) delete head;
			head = NULL;
			if (tail) delete tail;
			tail = NULL;
			//cout<<"!"<<endl;
		}
		/**
		* TODO assignment operator
		*/
		int check()
		{
			if (head->next->data == NULL) return 1; else return 0;
		}
		deque &operator=(const deque &other)
		{
			if (this == &other) return *this;
			clear();
			delete head; head = NULL; delete tail; tail = NULL;
			head = new node; tail = new node; head->next = tail; tail->pre = head;
			length = other.length;
			copy(other);
			return *this;
		}
		/**
		* access specified element with bounds checking
		* throw index_out_of_bound if out of bound.
		*/
		T & at(const size_t &pos)
		{
			if (pos >= length || pos<0) throw index_out_of_bound();
			return *find(pos);
		}
		const T & at(const size_t &pos) const
		{
			if (pos >= length || pos<0) throw index_out_of_bound();
			return *const_find(pos);
		}
		T & operator[](const size_t &pos)
		{
			if (pos >= length || pos<0) throw index_out_of_bound();
			return *find(pos);
		}
		const T & operator[](const size_t &pos) const
		{
			if (pos >= length || pos<0) throw index_out_of_bound();
			return *const_find(pos);
		}
		/**
		* access the first element
		* throw container_is_empty when the container is empty.
		*/
		const T & front() const
		{
			if (length == 0) throw index_out_of_bound();
			const T *t = head->next->data[head->next->beg];
			return (*t);
		}
		/**
		* access the last element
		* throw container_is_empty when the container is empty.
		*/
		const T & back() const
		{
			if (length == 0) throw index_out_of_bound();
			const T *t = (tail->pre->data[(tail->pre->num) - 1 + tail->pre->beg]);
			return (*t);
		}
		/**
		* returns an iterator to the beginning.
		*/
		iterator begin()
		{
			iterator k(this);
			k.i = 0;
			return k;
		}
		const_iterator cbegin() const
		{
			const_iterator k(this);
			k.i = 0;
			return k;
		}
		/**
		* returns an iterator to the end.
		*/
		iterator end()
		{
			iterator k(this);
			k.i = length;
			return k;
		}
		const_iterator cend() const
		{
			const_iterator k(this);
			k.i = length;
			return k;
		}
		/**
		* checks whether the container is empty.
		*/
		bool empty() const { return (length == 0); }
		/**
		* returns the number of elements
		*/
		size_t size() const { return (length); }
		/**
		* clears the contents
		*/
		void clear()
		{
			node *p = head->next;
			while (p != tail)
			{
				node *q = p;
				p = p->next;
				if (q) delete q;
			}
			head->next = tail;
			tail->pre = head;
			length = 0;
		}
		/**
		* inserts elements at the specified locat on in the container.
		* inserts value before pos
		* returns an iterator pointing to the inserted value
		*     throw if the iterator is invalid or it point to a wrong place.
		*/
		iterator insert(iterator pos, const T &value)
		{

			if (pos.i > length || pos.i < 0 || pos.d != this)  throw invalid_iterator();
			iterator k(pos.d);
			k.i = pos.i;
			node *p = head->next;

			++length;

			if (p == tail)
			{
				p = new node;
				head->next = p;
				p->next = tail;
				tail->pre = p;
				p->pre = head;
				p->num = 1;
				p->data[1024] = new T(value);
				iterator x(this);
				x.i = 0;
				length = 1;
				return x;
			}
			int i = pos.i;
			if (pos.i == length - 1) {p = tail->pre;i = p->num;}
			while (p != tail)
			{
				if (i >= p->num && p->next != tail) i -= p->num;
				else
				{

					++p->num;
					if (i == 0) --p->beg; else
						for (int j = p->num - 1; j >= i + 1; --j)
							p->data[j + p->beg] = p->data[j - 1 + p->beg];
					p->data[i + p->beg] = new T(value);
					if ((p->num) > 1000)  divide(p);
					/*	if (lc < 1000) { ++lc; if ((p->num)  >  1000) p = divide(p); }
					else
					{
					p = head->next;
					while (p != tail)
					{
					if ((p->num) > 1000) p = divide(p);
					else
					if ((p->num) == 0)
					{
					node *q = p->next;
					p->pre->next = p->next;
					p->next->pre = p->pre;

					if (p) delete p;
					p = NULL;
					p = q->pre;
					}
					p = p->next;
					}
					lc = 0;
					}*/
					break;
				}
				p = p->next;
			}


			return k;
		}
		/**
		* removes specified element at pos.
		* removes the element at pos.
		* returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
		* throw if the container is empty, the iterator is invalid or it points to a wrong place.
		*/
		iterator erase(iterator pos)
		{
			if (pos.i >= length || pos.i<0 || this != pos.d)  throw invalid_iterator();
			iterator k(pos.d);
			k.i = pos.i;
			--length;
			node *p = head->next;
			int i = pos.i;
			if (pos.i == length ) {p = tail->pre;i = p->num-1;}
			while (p != tail)
			{
				if (i >= p->num) i -= p->num;
				else
				{
					--p->num;
					delete p->data[i + p->beg];
					p->data[i + p->beg] = NULL;
					if (i == 0) ++p->beg;
					else {
						for (int j = i; j <= p->num - 1; ++j)
							p->data[j + p->beg] = p->data[j + 1 + p->beg];
					}
					if ((p->num) == 0)
					{
						node *q = p->next;
						p->pre->next = p->next;
						p->next->pre = p->pre;

						if (p) delete p;
						p = NULL;
						p = q->pre;
					}
					/*	if (lc < 1000)
					{

					if ((p->num) == 0)
					{
					node *q = p->next;
					p->pre->next = p->next;
					p->next->pre = p->pre;

					if (p) delete p;
					p = NULL;
					p = q->pre;
					}
					++lc;
					}
					else
					{
					p = head->next;
					while (p != tail)
					{
					if ((p->num) > 1000) p = divide(p);
					else
					if ((p->num) == 0)
					{
					node *q = p->next;
					p->pre->next = p->next;
					p->next->pre = p->pre;

					if (p) delete p;
					p = NULL;
					p = q->pre;
					}
					p = p->next;
					}
					lc = 0;
					}*/
					break;
				}
				p = p->next;
			}


			return k;
		}
		/**
		* adds an element to the end
		*/
		void push_back(const T &value)
		{
			iterator k(this);
			k.i = length;
			insert(k, value);
		}
		/**
		* removes the last element
		*     throw when the container is empty.
		*/
		void pop_back()
		{
			iterator k(this);
			k.i = length - 1;
			erase(k);
		}
		/**
		* inserts an element to the beginning.
		*/
		void push_front(const T &value)
		{
			iterator k(this);
			k.i = 0;
			insert(k, value);
		}
		/**
		* removes the first element.
		*     throw when the container is empty.
		*/
		void pop_front()
		{
			iterator k(this);
			k.i = 0;
			erase(k);
		}
	};

}

#endif
