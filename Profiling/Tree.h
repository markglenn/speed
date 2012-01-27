/* 

$Id: Tree.h,v 1.1 2005/08/12 18:34:07 e11669 Exp $

STL-like templated tree class.
Copyright (C) 2001  Kasper Peeters <k.peeters@damtp.cam.ac.uk>

Microsoft VC version by Jason Avinger, see 

http://www.damtp.cam.ac.uk/user/kp229/tree/

for the original.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#ifndef _TREE_H_
#define _TREE_H_

#include <cassert>
#include <memory>
#include <stdexcept>
#include <iterator>
#include <set>

namespace System
{
	namespace Profiling
	{
#ifdef _MSC_VER // MSVC does not have HP style construct/destroy 
		template <class T1, class T2>
		inline void constructor(T1* p, T2& val) 
		{
			new ((void *) p) T1(val);
		}

		template <class T1>
		inline void constructor(T1* p) 
		{
			new ((void *) p) T1;
		}

		template <class T1>
		inline void destructor(T1* p)
		{
			p->~T1();
		}
		#else
		#define constructor std::construct
		#define destructor  std::destroy
		#endif


		template<class T>
		struct tree_node_ {
			tree_node_<T> *parent;
			tree_node_<T> *first_child, *last_child;
			tree_node_<T> *prev_sibling, *next_sibling;
			T data;
		};

		template <class T, class tree_node_allocator = std::allocator<tree_node_<T> > >
		class tree {
		protected:
			typedef tree_node_<T> tree_node;
		public:
			typedef T value_type;

			class iterator;
			class sibling_iterator;

			tree()
			{
				head_initialise_();
			}
			~tree()
			{
				clear();
				alloc_.deallocate(head,1);
			}
			tree(const tree<T, tree_node_allocator>& other)
			{
				head_initialise_();
				copy_(other);
			}
			void operator=(const tree<T, tree_node_allocator>& other)
			{
				copy_(other);
			}

			class sibling_iterator {
				friend class tree<T, tree_node_allocator>;
				//friend class tree<T, tree_node_allocator>::iterator;
			public:
				typedef T                          value_type;
				typedef T*                         pointer;
				typedef T&                         reference;
				typedef size_t                     size_type;
				typedef ptrdiff_t                  difference_type;
				typedef std::bidirectional_iterator_tag iterator_category;

				sibling_iterator()
					: node(0), parent_(0)
				{
				}

				sibling_iterator(tree_node *tn)
					: node(tn)
				{
					set_parent_();
				}

				sibling_iterator(const sibling_iterator& other)
					: node(other.node), parent_(other.parent_)
				{
				}

				sibling_iterator(const iterator& other)
					: node(other.node)
				{
					set_parent_();
				}

				sibling_iterator&  operator++(void)
				{
					if(node)
						node=node->next_sibling;
					return *this;
				}

				sibling_iterator&  operator--(void)
				{
					if(node) node=node->prev_sibling;
					else {
						assert(parent_);
						node=parent_->last_child;
					}
					return *this;
				}

				sibling_iterator&  operator+=(unsigned int num)
				{
					while(num>0) {
						++(*this);
						--num;
					}
					return (*this);
				}

				sibling_iterator&  operator-=(unsigned int num)
				{
					while(num>0) {
						--(*this);
						--num;
					}
					return (*this);
				}

				T&                 operator*(void) const
				{
					return node->data;
				}

				T*                 operator->(void) const
				{
					return &(node->data);
				}

				bool               operator==(const sibling_iterator& other) const
				{
					if(other.node==node) return true;
					else return false;
				}

				bool               operator!=(const sibling_iterator& other) const
				{
					if(other.node!=node) return true;
					else return false;
				}

				sibling_iterator   operator+(int num) const
				{
					sibling_iterator ret(*this);
					while(num>0) {
						++ret;
						--num;
					}
					return ret;
				}


				bool       is_valid() const
				{
					if(node==0) return false;
					else return true;
				}

				tree_node *range_first() const
				{
					tree_node *tmp=parent_->first_child;
					return tmp;
				}

				tree_node *range_last() const
				{
					return parent_->last_child;
				}

				tree_node *node;


			private:
				void set_parent_()
				{
					parent_=0;
					if(node==0) return;
					if(node->parent!=0)
						parent_=node->parent;
				}

				tree_node *parent_;
			};



			class iterator { 
			public:
				typedef T                          value_type;
				typedef T*                         pointer;
				typedef T&                         reference;
				typedef size_t                     size_type;
				typedef ptrdiff_t                  difference_type;
				typedef std::bidirectional_iterator_tag iterator_category;

				iterator()
					: node(0), skip_current_children_(false)
				{
				}

				iterator(tree_node *tn)
					: node(tn), skip_current_children_(false)
				{
				}

				iterator(const sibling_iterator& other)
					: node(other.node), skip_current_children_(false)
				{
					if(node==0) {
						node=other.range_last();
						skip_children();
						increment_();
					}
				}

				iterator&  operator++(void)
				{
					if(!increment_()) {
						node=0;
					}
					return *this;
				}

				iterator&  operator--(void)
				{
					if(!decrement_()) {
						node=0;
					}
					return *this;
				}

				iterator&  operator+=(unsigned int num)
				{
					while(num>0) {
						++(*this);
						--num;
					}
					return (*this);
				}

				iterator&  operator-=(unsigned int num)
				{
					while(num>0) {
						--(*this);
						--num;
					}
					return (*this);
				}

				T&         operator*(void) const
				{
					return node->data;
				}

				T*         operator->(void) const
				{
					return &(node->data);
				}

				bool       operator==(const iterator& other) const
				{
					if(other.node==node) return true;
					else return false;
				}

				bool       operator!=(const iterator& other) const
				{
					if(other.node!=node) return true;
					else return false;
				}

				iterator   operator+(int num) const
				{
					iterator ret(*this);
					while(num>0) {
						++ret;
						--num;
					}
					return ret;
				}

				sibling_iterator begin() const
				{
					sibling_iterator ret(node->first_child);
					ret.parent_=node;
					return ret;
				}

				sibling_iterator end() const
				{
					sibling_iterator ret(0);
					ret.parent_=node;
					return ret;
				}

				// do not iterate over children of this node
				void skip_children() 
				{
					skip_current_children_=true;
				}

				bool is_valid() const
				{
					if(node==0) return false;
					else return true;
				}

				unsigned int number_of_children() const
				{
					tree_node *pos=node->first_child;
					if(pos==0) return 0;

					unsigned int ret=1;
					while(pos!=node->last_child) {
						++ret;
						pos=pos->next_sibling;
					}
					return ret;
				}

				tree_node *node;
			private:
				bool increment_()
				{
					assert(node!=0);
					if(!skip_current_children_ && node->first_child != 0) {
						node=node->first_child;
						return true;
					}
					else {
						skip_current_children_=false;
						while(node->next_sibling==0) {
							node=node->parent;
							if(node==0)
								return false;
						}
						node=node->next_sibling;
						return true;
					}
				}

				bool decrement_()
				{
					assert(node!=0);
					if(node->parent==0) {
						if(node->last_child==0)
							node=node->prev_sibling;
						while(node->last_child)
							node=node->last_child;
						if(!node) return false;
					}
					else {
						if(node->prev_sibling) {
							if(node->prev_sibling->last_child) {
								node=node->prev_sibling->last_child;
							}
							else {
								node=node->prev_sibling;
							}
						}
						else {
							node=node->parent;
							if(node==0)
								return false;
						}
					}
					return true;
				}

				bool skip_current_children_;
			};

			// begin/end of tree
			iterator begin() const
			{
				return iterator(head->next_sibling);
			}
			iterator end() const
			{
				return iterator(head);
			}
			// begin/end of children of node
			sibling_iterator begin(iterator pos) const
			{
				if(pos.node->first_child==0) {
					return end(pos);
				}
				return pos.node->first_child;
			}

			sibling_iterator end(iterator pos) const
			{
				sibling_iterator ret(0);
				ret.parent_= pos.node;
				return ret;
			}

			iterator parent(iterator position) const
			{
				assert(position.node!=0);
				return iterator(position.node->parent);
			}

			iterator previous_sibling(iterator position) const
			{
				assert(position.node!=0);
				return iterator(position.node->prev_sibling);
			}

			iterator next_sibling(iterator position) const
			{
				assert(position.node!=0);
				return iterator(position.node->next_sibling);
			}

			void clear()
			{
				if(head)
					while(head->next_sibling!=head)
						erase(head->next_sibling);
			}

			// erase element at position pointed to by iterator, increment iterator
			iterator erase(iterator it)
			{
				tree_node *cur=it.node;
				assert(cur!=head);
				iterator ret=it;
				ret.skip_children();
				++ret;
				erase_children(it);
				if(cur->prev_sibling==0) {
					cur->parent->first_child=cur->next_sibling;
				}
				else {
					cur->prev_sibling->next_sibling=cur->next_sibling;
				}
				if(cur->next_sibling==0) {
					cur->parent->last_child=cur->prev_sibling;
				}
				else {
					cur->next_sibling->prev_sibling=cur->prev_sibling;
				}

				destructor(&cur->data);
				alloc_.deallocate(cur,1);
				return ret;
			}
			// erase all children of the node pointed to by iterator
			void     erase_children(iterator it)
			{
				tree_node *cur=it.node->first_child;
				tree_node *prev=0;

				while(cur!=0) {
					prev=cur;
					cur=cur->next_sibling;
					erase_children(prev);
					destructor(&prev->data);
					alloc_.deallocate(prev,1);
				}
				it.node->first_child=0;
				it.node->last_child=0;
			}

			// insert node as last child of node pointed to by position
			iterator append_child(iterator position)
			{
				assert(position.node!=head);

				tree_node* tmp = alloc_.allocate(1,0);
				constructor(&tmp->data);
				tmp->first_child=0;
				tmp->last_child=0;

				tmp->parent=position.node;
				if(position.node->last_child!=0) {
					position.node->last_child->next_sibling=tmp;
				}
				else {
					position.node->first_child=tmp;
				}
				tmp->prev_sibling=position.node->last_child;
				position.node->last_child=tmp;
				tmp->next_sibling=0;
				return tmp;
			} 

			iterator append_child(iterator position, const T& x)
			{
				// If your program fails here you probably used 'append_child' to add the top
				// node to an empty tree. From version 1.45 the top element should be added
				// using 'insert'. See the documentation for further information, and sorry about
				// the API change.
				assert(position.node!=head);

				tree_node* tmp = alloc_.allocate(1,0);
				constructor(&tmp->data, x);
				tmp->first_child=0;
				tmp->last_child=0;

				tmp->parent=position.node;
				if(position.node->last_child!=0) {
					position.node->last_child->next_sibling=tmp;
				}
				else {
					position.node->first_child=tmp;
				}
				tmp->prev_sibling=position.node->last_child;
				position.node->last_child=tmp;
				tmp->next_sibling=0;
				return tmp;
			}

			iterator append_child(iterator position, iterator other_position)
			{
				assert(position.node!=head);

				sibling_iterator aargh=append_child(position, value_type());
				return replace(aargh, aargh+1, other, sibling_iterator(other)+1);
			}

			// short-hand to insert topmost node in otherwise empty tree
			iterator set_head(const T& x)
			{
				assert(begin()==end());
				return insert(begin(), x);
			}

			// insert node as previous sibling of node pointed to by position
			iterator insert(iterator position, const T& x)
			{
				tree_node* tmp = alloc_.allocate(1,0);
				constructor(&tmp->data, x);
				tmp->first_child=0;
				tmp->last_child=0;

				tmp->parent=position.node->parent;
				tmp->next_sibling=position.node;
				tmp->prev_sibling=position.node->prev_sibling;
				position.node->prev_sibling=tmp;

				if(tmp->prev_sibling==0)
					tmp->parent->first_child=tmp;
				else
					tmp->prev_sibling->next_sibling=tmp;
				return tmp;
			}

			// insert node as previous sibling of node pointed to by position
			iterator insert(sibling_iterator position, const T& x)
			{
				tree_node* tmp = alloc_.allocate(1,0);
				constructor(&tmp->data, x);
				tmp->first_child=0;
				tmp->last_child=0;

				tmp->next_sibling=position.node;
				if(position.node==0) { // iterator points to end of a subtree
					tmp->parent=position.parent_;
					tmp->prev_sibling=position.range_last();
				}
				else {
					tmp->parent=position.node->parent;
					tmp->prev_sibling=position.node->prev_sibling;
					position.node->prev_sibling=tmp;
				}

				if(tmp->prev_sibling==0)
					tmp->parent->first_child=tmp;
				else
					tmp->prev_sibling->next_sibling=tmp;
				return tmp;
			}

			// insert node (with children) pointed to by subtree as previous sibling of node pointed to by position
			iterator insert(iterator position, iterator subtree)
			{
				// insert dummy
				iterator it=insert(position, value_type());
				// replace dummy with subtree
				return replace(it, subtree);
			}

			// insert node (with children) pointed to by subtree as previous sibling of node pointed to by position
			iterator insert(sibling_iterator position, iterator subtree)
			{
				// insert dummy
				iterator it=insert(position, value_type());
				// replace dummy with subtree
				return replace(it, subtree);
			}

			// insert node as next sibling of node pointed to by position
			iterator insert_after(iterator position, const T& x)
			{
				tree_node* tmp = alloc_.allocate(1,0);
				constructor(&tmp->data, x);
				tmp->first_child=0;
				tmp->last_child=0;

				tmp->parent=position.node->parent;
				tmp->prev_sibling=position.node;
				tmp->next_sibling=position.node->next_sibling;
				position.node->next_sibling=tmp;

				if(tmp->next_sibling==0) {
					tmp->parent->last_child=tmp;
				}
				return tmp;
			}


			// replace node at 'position' with other node (keeping same children)
			iterator replace(iterator position, const T& x)
			{
				destructor(&position.node->data);
				constructor(&position.node->data, x);
				return position;
			}

			// replace node at 'position' with subtree starting at 'from' (do not erase subtree at 'from')
			iterator replace(iterator position, iterator from)
			{
				assert(position.node!=head);

				tree_node *current_from=from.node;
				tree_node *start_from=from.node;
				tree_node *last=from.node->next_sibling;
				tree_node *current_to  =position.node;

				// replace the node at position with head of the replacement tree at from
				erase_children(position);	
				tree_node* tmp = alloc_.allocate(1,0);
				constructor(&tmp->data, (*from));
				tmp->first_child=0;
				tmp->last_child=0;
				if(current_to->prev_sibling==0) {
					current_to->parent->first_child=tmp;
				}
				else {
					current_to->prev_sibling->next_sibling=tmp;
				}
				tmp->prev_sibling=current_to->prev_sibling;
				if(current_to->next_sibling==0) {
					current_to->parent->last_child=tmp;
				}
				else {
					current_to->next_sibling->prev_sibling=tmp;
				}
				tmp->next_sibling=current_to->next_sibling;
				tmp->parent=current_to->parent;
				destructor(&current_to->data);
				alloc_.deallocate(current_to,1);
				current_to=tmp;

				iterator toit=tmp;

				// copy all children
				do {
					assert(current_from!=0);
					if(current_from->first_child != 0) {
						current_from=current_from->first_child;
						toit=append_child(toit, current_from->data);
					}
					else {
						while(current_from->next_sibling==0 && current_from!=start_from) {
							current_from=current_from->parent;
							toit=parent(toit);
							assert(current_from!=0);
						}
						current_from=current_from->next_sibling;
						if(current_from!=last) {
							toit=append_child(parent(toit), current_from->data);
						}
					}
				} while(current_from!=last);

				return current_to;
			}

			// replace string of siblings (plus their children) with copy of a new string (with children)
			iterator replace(sibling_iterator orig_begin, sibling_iterator orig_end, 
				sibling_iterator new_begin,  sibling_iterator new_end)
			{
				tree_node *orig_first=orig_begin.node;
				tree_node *new_first=new_begin.node;
				tree_node *orig_last=orig_first;
				while(++orig_begin!=orig_end)
					orig_last=orig_last->next_sibling;
				tree_node *new_last=new_first;
				while(++new_begin!=new_end)
					new_last=new_last->next_sibling;

				// insert all siblings in new_first..new_last before orig_first
				bool first=true;
				iterator ret;
				while(1==1) {
					iterator tt=insert(iterator(orig_first), new_first);
					if(first) {
						ret=tt;
						first=false;
					}
					if(new_first==new_last)
						break;
					new_first=new_first->next_sibling;
				}

				// erase old range of siblings
				bool last=false;
				tree_node *next=orig_first;
				while(1==1) {
					if(next==orig_last) 
						last=true;
					next=next->next_sibling;
					erase(orig_first);
					if(last) 
						break;
					orig_first=next;
				}
				return ret;
			}


			// move all children of node at 'position' to be siblings
			iterator flatten(iterator position)
			{
				if(position.node->first_child==0)
					return position;

				tree_node *tmp=position.node->first_child;
				while(tmp) {
					tmp->parent=position.node->parent;
					tmp=tmp->next_sibling;
				} 
				if(position.node->next_sibling) {
					position.node->last_child->next_sibling=position.node->next_sibling;
					position.node->next_sibling->prev_sibling=position.node->last_child;
				}
				else {
					position.node->parent->last_child=position.node->last_child;
				}
				position.node->next_sibling=position.node->first_child;
				position.node->next_sibling->prev_sibling=position.node;
				position.node->first_child=0;
				position.node->last_child=0;

				return position;
			}

			// move nodes in range to be children of 'position'
			iterator reparent(iterator position, sibling_iterator begin, sibling_iterator end)
			{
				tree_node *first=begin.node;
				tree_node *last=first;
				while(++begin!=end) {
					last=last->next_sibling;
				}
				// move subtree
				if(first->prev_sibling==0) {
					first->parent->first_child=last->next_sibling;
				}
				else {
					first->prev_sibling->next_sibling=last->next_sibling;
				}
				if(last->next_sibling==0) {
					last->parent->last_child=first->prev_sibling;
				}
				else {
					last->next_sibling->prev_sibling=first->prev_sibling;
				}
				if(position.node->first_child==0) {
					position.node->first_child=first;
					position.node->last_child=last;
					first->prev_sibling=0;
				}
				else {
					position.node->last_child->next_sibling=first;
					first->prev_sibling=position.node->last_child;
					position.node->last_child=last;
				}
				last->next_sibling=0;

				tree_node *pos=first;
				while(1==1) {
					pos->parent=position.node;
					if(pos==last) break;
					pos=pos->next_sibling;
				}

				return first;
			}

			// ditto, the range being all children of the 'from' node
			iterator reparent(iterator position, iterator from)
			{
				if(from.node->first_child==0) return position;
				return reparent(position, from.node->first_child, from.node->last_child);
			}

			// merge with other tree, creating new branches and leaves only if they are not already present
			void     merge(iterator position, iterator other, bool duplicate_leaves=false)
			{
				sibling_iterator fnd;
				sibling_iterator oit=other;
				while(oit.is_valid()) {
					if((fnd=find(position.begin(), position.end(), (*other)))!=position.end()) {
						if(duplicate_leaves && other.begin()==other.end()) { // it's a leave
							append_child(position, (*other));
						}
						else {
							if(other.begin()!=other.end())
								merge(fnd, other.begin(), duplicate_leaves);
						}
					}
					else {
						insert(position.end(), oit);
					}
					++oit;
				}
			}

			// sort (std::sort only moves values of nodes, this one moves children as well)
			void     sort(sibling_iterator from, sibling_iterator to, bool deep=false)
			{
				std::less<T> comp;
				sort(from, to, comp, deep);
			}

			template<class StrictWeakOrdering>
				void     sort(sibling_iterator from, sibling_iterator to, StrictWeakOrdering comp, bool deep=false)
			{
				if(from==to) return;
				// make list of sorted nodes
				// CHECK: if multiset stores equivalent nodes in the order in which they
				// are inserted, then this routine should be called 'stable_sort'.
				std::multiset<tree_node *, compare_nodes<StrictWeakOrdering> > nodes;
				sibling_iterator it=from, it2=to;
				while(it != to) {
					nodes.insert(it.node);
					++it;
				}
				// reassemble
				--it2;
				tree_node *prev=from.node->prev_sibling;
				tree_node *next=it2.node->next_sibling;
				typename std::multiset<tree_node *, compare_nodes<StrictWeakOrdering> >::iterator nit=nodes.begin(), eit=nodes.end();
				if(prev==0) {
					(*nit)->parent->first_child=(*nit);
				}
				--eit;
				while(nit!=eit) {
					(*nit)->prev_sibling=prev;
					if(prev)
						prev->next_sibling=(*nit);
					prev=(*nit);
					++nit;
				}
				if(prev)
					prev->next_sibling=(*eit);
				(*eit)->next_sibling=next;
				if(next==0) {
					(*eit)->parent->last_child=next;
				}

				if(deep) {	// sort the children of each node too
					sibling_iterator bcs(*nodes.begin());
					sibling_iterator ecs(*eit);
					++ecs;
					while(bcs!=ecs) {
						sort(begin(bcs), end(bcs), comp, deep);
						++bcs;
					}
				}
			}

			// compare subtrees starting at the two iterators (compares nodes as well as tree structure)
			template<class BinaryPredicate>
				bool     equal(iterator one, iterator two, iterator three, BinaryPredicate fun) const
			{
				while(one!=two && three.is_valid()) {
					if(one.number_of_children()!=three.number_of_children()) 
						return false;
					if(!fun(*one,*three))
						return false;
					++one;
					++three;
				}
				return true;
			}

			// extract a new tree formed by the range of siblings plus all their children
			tree     subtree(sibling_iterator from, sibling_iterator to) const
			{
				tree tmp;
				tmp.set_head(value_type());
				tmp.replace(tmp.begin(), tmp.end(), from, to);
				return tmp;
			}

			void     subtree(tree&, sibling_iterator from, sibling_iterator to) const
			{
				tmp.set_head(value_type());
				tmp.replace(tmp.begin(), tmp.end(), from, to);
			}

			// count the total number of nodes
			int      size() const
			{
				int i=0;
				iterator it=begin(), eit=end();
				while(it!=eit) {
					++i;
					++it;
				}
				return i;
			}

			// compute the depth to the root
			int      depth(iterator it) const
			{
				tree_node* pos=it.node;
				assert(pos!=0);
				int ret=0;
				while(pos->parent!=0) {
					pos=pos->parent;
					++ret;
				}
				return ret;
			}

			// count the number of children of node at position
			unsigned int number_of_children(iterator it) const
			{
				tree_node *pos=it.node->first_child;
				if(pos==0) return 0;

				unsigned int ret=1;
				while(pos!=it.node->last_child) {
					++ret;
					pos=pos->next_sibling;
				}
				return ret;
			}

			// count the number of 'next' siblings of node at iterator
			unsigned int number_of_siblings(iterator it) const
			{
				tree_node *pos=it.node;
				unsigned int ret=1;
				while(pos->next_sibling && pos->next_sibling!=head) {
					++ret;
					pos=pos->next_sibling;
				}
				return ret;
			}

			// determine whether node at position is in the subtrees with root in the range
			bool     is_in_subtree(iterator position, iterator begin, iterator end) const
			{
				// FIXME: this should be optimised.
				iterator tmp=begin;
				while(tmp!=end) {
					if(tmp==it) return true;
					++tmp;
				}
				return false;
			}

			// return the n-th child of the node at position
			T&       child(iterator position, unsigned int) const
			{
				tree_node *tmp=it.node->first_child;
				while(num--) {
					assert(tmp!=0);
					tmp=tmp->next_sibling;
				}
				return tmp->data;
			}

		private:
			tree_node_allocator alloc_;
			tree_node *head;    // head is always a dummy; if an iterator points to head it is invalid
			void head_initialise_()
			{ 
				head = alloc_.allocate(1,0); // MSVC does not have default second argument 

				head->parent=0;
				head->first_child=0;
				head->last_child=0;
				head->prev_sibling=head;
				head->next_sibling=head;
			}

			void copy_(const tree<T, tree_node_allocator>& other)
			{
				clear();
				iterator it=other.begin(), to=begin();
				while(it!=other.end()) {
					to=insert(to, (*it));
					it.skip_children();
					++it;
				}
				to=begin();
				it=other.begin();
				while(it!=other.end()) {
					to=replace(to, it);
					to.skip_children();
					it.skip_children();
					++to;
					++it;
				}
			}

			template<class StrictWeakOrdering>
			class compare_nodes {
			public:
				bool operator()(const tree_node* a, const tree_node* b)
				{
					static StrictWeakOrdering comp;
					return comp(a->data, b->data);
				}
			};
		};
	}
}
// Local variables:
// default-tab-width: 3
// End:

#endif
