/**
* implement a container like std::map
*/
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP
#include<iostream>
//using namespace std;
// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

	template<
		class Key,
		class T,
		class Compare = std::less<Key>
	> class map {
	public:
		typedef pair<const Key, T> value_type;
		Compare cmp;
	private:

		struct node
		{
			value_type *data;
			node *pre;
			node *next;
			node & operator=(const node &other)
			{
				data = other.data;
				pre = other.pre;
				next = other.next;
			}
			node(const Key &k) :data(new value_type(k)), pre(NULL), next(NULL) {}
			node(const value_type &v) :data(new value_type(v)), pre(NULL), next(NULL) {}
			node() :data(NULL), pre(NULL), next(NULL) {}

			node(const node *b) :
				pre(b->pre), next(b->next)
			{
				if (b->data == NULL) data = NULL;
				else
				{
					data = new value_type(*(b->data));
				}
			}
			//	~node() {if (data) delete data; data = NULL;}
		};
		struct avl
		{
			node *lc;
			avl *left;
			avl *right;
			int height = 0;
			avl() :lc(NULL), left(NULL), right(NULL), height(0) {}
			avl(node *a) :lc(a)
			{
				left = NULL;
				right = NULL;
				height = 0;
			}
			avl(const avl*a)
			{
				lc = new node(*(a->lc));
				left = a->left;
				right = a->right;
				height = a->height;
			}
		};



		int height(avl *t)const {
			if (t == NULL)return -1; else return t->height;
		}


		void make(avl *&A)
		{

			if (A == NULL) { return; }
			make(A->left);
			make(A->right);
			delete A;
			A = NULL;
		}
		int max(int x, int y)
		{
			if (x > y) return x;
			else return y;
		}
		void LL(avl *&t)
		{
			avl *t1 = t->left;
			t->left = t1->right;
			t1->right = t;
			t->height = max(height(t->left), height(t->right)) + 1;
			t1->height = max(height(t1->left), height(t)) + 1;
			t = t1;
		}
		void RR(avl *&t)
		{
			avl *t1 = t->right;
			t->right = t1->left;
			t1->left = t;

			t->height = max(height(t->left), height(t->right)) + 1;
			t1->height = max(height(t1->right), height(t)) + 1;
			t = t1;

		}
		void LR(avl *&t)
		{
			RR(t->left);
			LL(t);
		}
		void RL(avl *&t)
		{
			LL(t->right);
			RR(t);
		}
		void inserts(const Key &x, node *y, avl *&t)
		{
			if (t == NULL) { t = new avl(y); }
			else if (cmp(x, t->lc->data->first))
			{
				inserts(x, y, t->left);
				if (height(t->left) - height(t->right) == 2)
					if (cmp(x, t->left->lc->data->first)) LL(t); else LR(t);
			}
			else if (cmp(t->lc->data->first, x))
			{
				inserts(x, y, t->right);
				if (height(t->right) - height(t->left) == 2)
					if (cmp(t->right->lc->data->first, x)) RR(t); else RL(t);
			}

			t->height = max(height(t->left), height(t->right)) + 1;

		}
		bool remove(const Key &x, avl *&t)
		{
			//cout<<1<<endl;
			bool stop = false;
			int sub;
			//	cout<<2<<endl;
			if (t == NULL) return true;
			//	cout<<3<<endl;
			if (cmp(x, t->lc->data->first)) { stop = remove(x, t->left); if (t) t->height = max(height(t->left), height(t->right)) + 1; sub = 1; }
			else if (cmp(t->lc->data->first, x)) { stop = remove(x, t->right); if (t) t->height = max(height(t->left), height(t->right)) + 1; sub = 2; }
			else if (t->left != NULL && t->right != NULL)
			{
				//	cout<<4<<endl;
				avl * tmp = t->right;
				while (tmp->left != NULL) tmp = tmp->left;
				//if (t->lc->data) delete (t->lc->data);
				t->lc = tmp->lc;
				stop = remove(t->lc->data->first, t->right);
				if (t) t->height = max(height(t->left), height(t->right)) + 1;
				sub = 2;

			}
			else
			{
				//	cout<<5<<endl;
				avl * oldnode = t;
				t = (t->left != NULL) ? t->left : t->right;
				//cout<<"MNWDNIWOFNOWIFI?"<<oldnode->lc->data->first.val<<endl;
				//if (oldnode->lc->data) delete oldnode->lc->data;
				delete oldnode;
				oldnode = NULL;
				if (t) t->height = max(height(t->left), height(t->right)) + 1;
				return false;
			}
			if (stop) return true;
			int bf;
			//	cout<<6<<sub<<endl;
			switch (sub)
			{

			case 1:
				//cout<<"!"<<endl;
				//  if (t == NULL) cout<<"?"<<endl;
				bf = height(t->left) - height(t->right) + 1;
				//cout<<1<<endl;
				if (bf == 0) return true;

				if (bf == 1) return false;

				if (bf == -1)
				{
					int bfr = height(t->right->left) - height(t->right->right);
					switch (bfr)
					{

					case 0:	RR(t); return true;
					case -1:RR(t); return false;
					default: RL(t); return false;
					}
				}
				break;
			case 2:
				//    cout<<"?"<<endl;
				bf = height(t->left) - height(t->right) - 1;
				//   cout<<1<<endl;
				if (bf == 0) return true;
				if (bf == -1) return false;
				// cout<<2<<endl;
				if (bf == 1)
				{
					int bfl = height(t->left->left) - height(t->left->right);
					switch (bfl)
					{
					case 0:LL(t); return true;
					case 1:LL(t); return false;
					default:LR(t); return false;
					}
				}
				//  cout<<3<<endl;
			}
			//cout<<8<<endl;
		}
		bool findd(const Key &x) const
		{
			//	cout<<">><<<"<<endl;
			//	cout<<"!!"<<endl;
			avl * t = root;
			//cout<<1<<endl;

			if (t == NULL) return false;
			while (t != NULL && (cmp(x, t->lc->data->first) || cmp(t->lc->data->first, x)))
			{
				//	cout<<2<<endl;
				if (cmp(x, t->lc->data->first)) t = t->left; else t = t->right;
				// cout<<"????"<<endl;
				if (t == NULL) return false;
			}
			// cout<<1<<endl;
			// cout<<"@"<<endl;
			if (t == NULL) return false; else return true;
		}
		node* finder(const Key &x) const
		{
			avl * t = root;
			avl * tt = root;
			while (t != NULL && (cmp(x, t->lc->data->first) || cmp(t->lc->data->first, x)))
			{
				tt = t;
				if (cmp(x, t->lc->data->first)) t = t->left; else t = t->right;
			}
			//cout<<"        "<<t->lc->data->second<<endl;
			if (t != NULL) return t->lc;
		}
		avl* finderi(const Key &x) const
		{
			avl * t = root;
			avl * tt = root;
			while (t != NULL && (cmp(x, t->lc->data->first) || cmp(t->lc->data->first, x)))
			{
				tt = t;
				if (cmp(x, t->lc->data->first)) t = t->left; else t = t->right;
			}
			//cout<<"        "<<t->lc->data->second<<endl;
			if (t == NULL) return tt;
		}

	public:

		avl *root;
		node *head;
		node *tail;
		int n;
		int abs(int x) { if (x < 0) return -x; else return x; }
		void check(avl *t)
		{
			if (t == NULL) return;
			//	if (abs((height(t->left) - height(t->right))) >= 2)
			//	cout << "lc" << endl;
			check(t->left);
			check(t->right);
		}
		void Check() { check(root); }
		class const_iterator;
		class iterator {
		private:
			/**
			* TODO add data->first->first members
			*   just add whatever you want.
			*/

		public:
			node *i;
			const map *m;
			iterator(const map *t) :m(t) {}
			iterator() {

				// TODO
			}
			iterator(node *k) :i(k) { }
			iterator(const iterator &other) :i(other.i) {
				m = other.m;
				// TODO
			}
			/**
			* return a new iterator which pointer n-next elements
			*   even if there are not enough elements, just return the answer.
			* as well as operator-
			*/
			/**
			* TODO iter++
			*/
			iterator operator++(int)
			{
				if (i->next == NULL)throw invalid_iterator();
				node *j = i;
				i = i->next;
				iterator k(m);
				k.i = j;
				return k;
			}
			/**
			* TODO ++iter
			*/
			iterator & operator++()
			{
				if (i->next == NULL) throw invalid_iterator();
				i = i->next;
				return *this;
			}
			/**
			* TODO iter--
			*/
			iterator operator--(int)
			{
				if (i->pre->pre == NULL) throw invalid_iterator();
				node *j = i;
				i = i->pre;
				iterator k(m);
				k.i = j;
				return k;
			}
			/**
			* TODO --iter
			*/
			iterator & operator--()
			{
				if (i->pre->pre == NULL) throw invalid_iterator();
				i = i->pre;
				return *this;
			}
			iterator & operator=(const iterator &other)
			{
				i = other.i;
				m = other.m;
				return *this;
			}
			iterator & operator=(const const_iterator &other)
			{
				i = other.i;
				m = other.m;
				return *this;
			}
			/**
			* a operator to check whether two iterators are same (pointing to the same memory).
			*/
			value_type* operator->() const noexcept
			{
				return i->data;
			}
			value_type & operator*() const
			{
				value_type *v = ((*i).data);
				return *v;
			}

			bool operator==(const iterator &rhs) const
			{
				return (rhs.i == i);
			}
			bool operator==(const const_iterator &rhs) const
			{
				return (rhs.i == i);
			}
			/**
			* some other operator for iterator.
			*/
			bool operator!=(const iterator &rhs) const
			{
				return (rhs.i != i);
			}
			bool operator!=(const const_iterator &rhs) const
			{
				return (rhs.i != i);
			}
			/**
			* for the support of it->first.
			* See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
			*/

		};
		class const_iterator {

		public:
			const map *m;
			const node *i;
			const_iterator() {

			}
			const_iterator(const map *t) :m(t) {}
			const_iterator(const node *k) :i(k) { }
			const_iterator(const const_iterator &other) :i(other.i) {
				m = other.m;
			}
			const_iterator(const iterator &other) :i(other.i) {
				m = other.m;
			}
			const_iterator operator++(int)
			{
				if (i->next == NULL) throw invalid_iterator();
				const node *j = i;
				i = i->next;
				const_iterator k(m);
				k.i = j;
				return k;
			}
			/**
			* TODO ++iter
			*/
			const_iterator & operator++()
			{
				if (i->next == NULL) throw invalid_iterator();
				i = i->next;
				return *this;
			}
			/**
			* TODO iter--
			*/
			const_iterator operator--(int)
			{
				if (i->pre->pre == NULL) throw invalid_iterator();
				const node *j = i;
				i = i->pre;
				const_iterator k(m);
				k.i = j;
				return k;
			}
			/**
			* TODO --iter
			*/
			const_iterator & operator=(const const_iterator &other)
			{
				i = other.i;
				m = other.m;
				return *this;
			}
			const_iterator & operator=(const iterator &other)
			{
				i = other.i;
				m = other.m;
				return *this;
			}
			const_iterator & operator--()
			{
				if (i->pre->pre == NULL) throw invalid_iterator();
				i = i->pre;
				return *this;
			}
			value_type* operator->() const noexcept
			{
				return i->data;
			}
			value_type & operator*() const
			{
				value_type *v = ((*i).data);
				return *v;
			}
			bool operator==(const iterator &rhs) const
			{
				return (rhs.i == i);
			}
			bool operator==(const const_iterator &rhs) const
			{
				return (rhs.i == i);
			}

			bool operator!=(const iterator &rhs) const
			{
				return (rhs.i != i);
			}
			bool operator!=(const const_iterator &rhs) const
			{
				return (rhs.i != i);
			}
		};

		map()
		{
			root = NULL; head = new node; tail = new node; head->next = tail; tail->pre = head; n = 0;
		}
		void copy(avl *B)
		{

			if (!B)return;
			value_type *v =(B->lc->data);

			insert(*v);
			if (B->left) copy(B->left);
			if (B->right)   copy(B->right);

		}
		map(const map &other)
		{
			root = NULL; head = new node; tail = new node; head->next = tail; tail->pre = head; n = 0;
			copy(other.root);
		}
		map & operator=(const map &other)
		{
			if (this == &other) return *this;
			clear();
			delete head; head = NULL; delete tail; tail = NULL;
			root = NULL; head = new node; tail = new node; head->next = tail; tail->pre = head; n = 0;
			copy(other.root);
			return *this;
		}

		~map()
		{
			clear();
			delete head;
			head = NULL;
			delete tail;
			tail = NULL;
		}
		T & at(const Key &key)
		{
			if (findd(key)) return finder(key)->data->second;
			else throw index_out_of_bound();
		}
		const T & at(const Key &key) const
		{
			if (findd(key)) return finder(key)->data->second;
			else throw index_out_of_bound();
		}
		T & operator[](const Key &key)
		{


			if (findd(key)) { return finder(key)->data->second; }
			else
			{
				iterator x(this);
				node *k = new node(key);
					avl *t = finderi(k->data->first);
					if (n == 0)
					{
						head->next = k;
						k->next = tail;
						tail->pre = k;
						k->pre = head;
					}
					else
					{
						if (cmp(k->data->first, t->lc->data->first))
						{
							node *y = t->lc;
							k->pre = y->pre;
							k->next = y;
							y->pre->next = k;
							y->pre = k;
						}
						else
						{
							node *y = t->lc;
							k->next = y->next;
							k->pre = y;
							y->next->pre = k;
							y->next = k;
						}
					}
					inserts(key, k, root);
					++n;
				x.i = finder(key);
				return finder(key)->data->second;
			}
		}
		const T & operator[](const Key &key) const
		{
			if (findd(key)) return finder(key)->data->second;
			else throw index_out_of_bound();
		}
		iterator begin()
		{
			iterator k(this);
			k.i = head->next;
			return k;
		}
		const_iterator cbegin()  const
		{
			const_iterator k(this);
			k.i = head->next;
			return k;
		}
		iterator end()
		{
			iterator k(this);
			k.i = tail;
			return k;
		}
		const_iterator cend() const
		{
			const_iterator k(this);
			k.i = tail;
			return k;
		}
		bool empty() const
		{
			if (root == NULL) return true; else return false;
		}
		size_t size() const
		{
			return n;
		}
		void clear()
		{
			make(root);
			node *p = head->next, *q;
			head->next = tail;
			tail->pre = head;
			while (p != tail)
			{
				//	cout<<"!!!"<<endl;
				q = p->next;
				if (p->data) { delete p->data; p->data = NULL; }
				delete p;
				p = q;
			}
			n = 0;
		}
		pair<iterator, bool> insert(const value_type &value)
		{
			iterator x(this);
			pair<iterator, bool> par(x, true);
			if (findd(value.first)) par.second = false;
			else
			{

				node *k = new node(value);

				par.second = true;
				par.first = x;
				avl *t = finderi(k->data->first);
				if (n == 0)
				{
					head->next = k;
					k->next = tail;
					tail->pre = k;
					k->pre = head;
				}
				else
				{
					if (cmp(k->data->first, t->lc->data->first))
					{
						node *y = t->lc;
						k->pre = y->pre;
						k->next = y;
						y->pre->next = k;
						y->pre = k;
					}
					else
					{
						node *y = t->lc;
						k->next = y->next;
						k->pre = y;
						y->next->pre = k;
						y->next = k;
					}
				}
				inserts(value.first, k, root);
				++n;
			}
			x.i = finder(value.first);
			par.first = x;
			return par;
		}
		void erase(iterator pos)
		{
			if (pos.m != this) throw invalid_iterator();
			if (pos.i == tail || pos.i == NULL) throw index_out_of_bound();
			Key k = pos.i->data->first;
			if (!findd(k)) throw index_out_of_bound();
			else
			{
				--n;
				remove(k, root);
				pos.i->next->pre = pos.i->pre;
				pos.i->pre->next = pos.i->next;
				if (pos.i->data) { delete pos.i->data; pos.i->data = NULL; }
				delete pos.i;
				pos.i = NULL;
			}

		}
		size_t count(const Key &key) const
		{
			//cout<<0<<endl;
			if (findd(key)) return 1; else return 0;
		}
		iterator find(const Key &key)
		{

			if (!findd(key)) {
				iterator k(this); k.i = tail; return k;

			}
			else
			{
				iterator k(this); k.i = finder(key);
				return k;
			}
		}
		const_iterator find(const Key &key) const
		{
			if (!findd(key))
			{
				const_iterator k(this); k.i = tail; return k;

			}
			else
			{
				const_iterator k(this); k.i = finder(key);
				return k;
			}
		}

	};


};

#endif
