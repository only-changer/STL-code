#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP
#include<iostream>
#include <cstddef>
#include <functional>
#include "exceptions.hpp"
//#include<iostream>
using namespace std; 
namespace sjtu {

/**
 * a container like std::priority_queue which is a heap internal.
 * it should be based on the vector written by yourself.
 */
template<typename T, class Compare = std::less<T>>

class priority_queue {
private:
	Compare cmp;
    struct mergeheap
    {
	    mergeheap * left;
	    mergeheap * right;
	    T data;
	    int dist;
	    mergeheap(const mergeheap *&A):data(A->data),dist(A->dist),left(A->left),right(A->right)
	    {
	    }
	    mergeheap(T data, int dist):left(NULL),right(NULL),dist(dist), data(data)
	    {
	    }
	    ~mergeheap()
	    {
	    }
	
    };
	mergeheap *A;
	int n;
	template<class T2>
	void swap(T2 &A,T2 &B)
    {
	    T2 C = A;
	    A = B;
	    B = C;
    }
    mergeheap *me(mergeheap *A,mergeheap *B) 
	{
		if (A == NULL) return B;
	    if (B == NULL) return A;
	
	    if (cmp(A->data , B->data)) swap(A,B);
	
	    A->right = me(A->right,B);

        if (A->right != NULL && A->left != NULL)
        { 
	        if (A->left->dist<A->right->dist) swap(A->left,A->right);
	    }
	    else if (A->right != NULL && A->left == NULL) swap(A->left,A->right);
    
	    if (A->right == NULL) A->dist = 0; else A->dist = A->right->dist + 1;
	    return A;
	}
	void lc(mergeheap * A)
	{
		
		if (A == NULL) {delete A;return;}
		lc(A->left);
		lc(A->right);
		delete A;
		A = NULL;
	}
	/*void copy(mergeheap *&A,const mergeheap *B)
	{
			
		A = new mergeheap(B->data,B->dist);
		A->data = B->data;
	    A->dist = B->dist;
		A->left = B->left;
		A->right = B->right;
		if (A->left != NULL) copy(A->left,B->left);
		if (A->right != NULL) copy(A->right,B->right);
	}*/
	mergeheap* copy(const mergeheap *B)
	{
		if(!B)return NULL;
		mergeheap* A = new mergeheap(B->data,B->dist);
		A->data = B->data;
	    A->dist = B->dist;
		A->left = B->left;
		A->right = B->right;
		A->left=copy(B->left);
		A->right=copy(B->right);
		return A; 
	}	
public:
	/**
	 * TODO constructors
	 */
	
	priority_queue() :A(NULL),n(0) 
	{
	}
	priority_queue(const priority_queue &other) 
	{ 
	    A = NULL;
		n = other.n;
		A=copy(other.A);
	}

	/**
	 * TODO deconstructor
	 */
	void pop() 
	{
		if (n > 0)
		{
            mergeheap *B(A);
            A = me(B->left,B->right);
            --n;
	    	delete B; 
	    	B = NULL;
	    }
	    else throw container_is_empty();
	}
	~priority_queue() 
	{
		lc(A); 
	}
	/**
	 * TODO Assignment operator
	 */
	priority_queue &operator=(const priority_queue &other) 
	{
		if (this == &other) return *this;
		lc(A);
		A = NULL;
		n = other.n;
			A=copy(other.A);	
		return *this;
	}
	/**
	 * get the top of the queue.
	 * @return a reference of the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	const T & top() const 
	{
		if (n > 0)
         return A->data;
         else throw container_is_empty();
	}
	/**
	 * TODO
	 * push new element to the priority queue.
	 */
	void push(const T &e) 
	{
		mergeheap * B = new mergeheap(e, 0);
		B->left = NULL;
		B->right = NULL;
		B->data = e;
		B->dist = 0;
        A = me(A,B);
        ++n;
	}
	/**
	 * TODO
	 * delete the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	
	/**
	 * return the number of the elements.
	 */
	size_t size() const 
	{
         return n;
	}
	/**
	 * check if the container has at least an element.
	 * @return true if it is empty, false if it has at least an element.
	 */
	bool empty()  
	{
	//	cout<<1<<endl;
        if (A == NULL) return 1;
        else return 0;
	}
	void merge(priority_queue &other) 
	{
		n += other.n;
		A = me(A,other.A);
		using namespace std;
		other.n = 0;
		other.A = NULL;
	}

};

}

#endif
