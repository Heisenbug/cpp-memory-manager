#ifndef MMRBTREE_H_INCLUDE_GUARD
#define MMRBTREE_H_INCLUDE_GUARD

#include <utility>
#include <iostream>
#include <string>

namespace MM
{
	enum color_type { RED, BLACK };

	class rbtree_error {};

	template<typename K, typename V>
	struct rbtnode
	{
		rbtnode(const K& k, const V& v, const color_type& c)
			: key(k), value(v), p(0), l(0), r(0), color(c) { }

		K key;
		V value;
		rbtnode* p;
		rbtnode* l;
		rbtnode* r;
		color_type color;
	};

	template<typename K, typename V>
	class rbtree
	{
		typedef rbtnode<K, V> Node;

		Node* nil;
		Node* handle;

	public:

		typedef std::pair<K, V> pair;

		rbtree()
			: nil(new rbtnode<K, V>(0, 0, BLACK)), handle(nil)
		{
			handle->p = nil;
		}

		rbtree(const K* keys, const V* values, size_t n) 
			: nil(new rbtnode<K, V>(0, 0, BLACK)), handle(nil)
		{
			handle->p = nil;

			for (size_t i = 0; i < n; ++i)
			{
				insert(keys[i], values[i]);
			}
		}

		~rbtree() { }

		template<typename Function_Object>
		void walk(Function_Object do_)
		{
			if (handle != nil) walk_(do_, handle);
		}

		void insert(const K& k, const V& v)
		{
			RBinsert(new Node(k, v, RED));
		}

		void remove(const K& k)
		{
			if (handle != nil)
			{
				Node* p = search_(k, handle);
				if (p != nil)
				{
					Node* free_node = RBdelete(p);
					delete free_node;
				}
			} else { throw rbtree_error(); }
		}

		V minimum() const 
		{
			if (handle != nil)
			{
				Node* p = minimum_(handle);
				return p->value;
			}
			throw rbtree_error();
		}

		pair minimum_p() const
		{
			if (handle != nil)
			{
				Node* p = minimum_(handle);
				return std::make_pair(p->key, p->value);
			}
			throw rbtree_error();
		}

		V maximum() const 
		{
			if (handle != nil)
			{
				Node* p = maximum_(handle);
				return p->value;
			}
			throw rbtree_error();
		}

		pair maximum_p() const
		{
			if (handle != nil)
			{
				Node* p = maximum_(handle);
				return std::make_pair(p->key, p->value);
			}
			throw rbtree_error();
		}

		V search(const K& k) const 
		{
			if (handle != nil)
			{
				Node* p = search_(k);
				return p->value;
			}
			throw rbtree_error();
		}

		unsigned int bheight() const 
		{
			if (handle != nil) return bheight_(handle);
			else return 0;
		}

	private:

		template<typename Function_Object>
		void walk_(Function_Object do_, Node* n)
		{
			if (n->l != nil) walk_(do_, n->l);
			do_(std::make_pair(n->key, n->value));
			if (n->r != nil) walk_(do_, n->r);
		}

		Node* minimum_(Node* x) const
		{
			while(x->l != nil) { x = x->l; }
			return x;
		}

		Node* maximum_(Node* x) const
		{
			while(x->r != nil) { x = x->r; }
			return x;
		}

		Node* successor_(Node* x) const
		{
			if (x != nil)
			{
				if (x->r != nil) { return minimum_(x->r); }
				Node* y = x->p;
				while (y != nil && x == y->r)
				{
					x = y;
					y = y->p;
				}
				return y;
			}
		}

		Node* search_(const K& k, Node* x) const
		{
			while (x != nil && k != x->key)
			{
				if (k < x->key)
					x = x->l;
				else
					x = x->r;
			}

			return x;
		}
		
		unsigned int bheight_(Node* x) const
		{
			unsigned int h = 0;
			while(x->r != nil)
			{
				x = x->r;
				if (x->color == BLACK) { h++; }
			}
			return (h + 1);
		}

		void left_rotate(Node* x)
		{
			if (x->r != nil)
			{
				Node* y = x->r;
				x->r = y->l;
				if (y->l != nil)
				{
					y->l->p = x;
				}
				y->p = x->p;
				if (x->p == nil)
				{
					handle = y;
				} 
				else 
				{
					if (x->p->l == x)
					{
						x->p->l = y;
					}
					else 
					{
						x->p->r = y;
					}
				}
				y->l = x;
				x->p = y;
			}
		}

		void right_rotate(Node* y)
		{
			if (y->l != nil)
			{
				Node* x = y->l;
				y->l = x->r;
				if (x->r != nil)
				{
					x->r->p = y;
				}
				x->p = y->p;
				if (y->p == nil)
				{
					handle = x;
				} 
				else 
				{
					if (y == y->p->r)
					{
						y->p->r = x;
					}
					else 
					{
						y->p->l = x;
					}
				}
				x->r = y;
				y->p = x;
			}
		}

		void RBdelete_fixup(Node* x)
		{
			Node* w = nil;
			while ((x != handle) && (x->color == BLACK))
			{
				if (x == x->p->l)
				{
					w = x->p->r;
					if (w->color == RED)
					{
						w->color = BLACK;
						x->p->color = RED;
						left_rotate(x->p);
						w = x->p->r;
					}
					if ((w->l->color == BLACK) && (w->r->color == BLACK))
					{
						w->color = RED;
						x = x->p;
					}
					else 
					{
						if (w->r->color == BLACK)
						{
							w->l->color = BLACK;
							w->color = RED;
							right_rotate(w);
							w = x->p->r;
						}
						w->color = x->p->color;
						x->p->color = BLACK;
						w->r->color = BLACK;
						left_rotate(x->p);
						x = handle;
					}
				}
				else 
				{
					w = x->p->l;
					if (w->color == RED)
					{
						w->color = BLACK;
						x->p->color = RED;
						right_rotate(x->p);
						w = x->p->l;
					}
					if (w->r->color == BLACK && w->l->color == BLACK)
					{
						w->color = RED;
						x = x->p;
					}
					else
					{
						if (w->l->color == BLACK)
						{
							w->r->color = BLACK;
							w->color = RED;
							left_rotate(w);
							w = x->p->l;
						}
						w->color = x->p->color;
						x->p->color = BLACK;
						w->r->color = BLACK;
						right_rotate(x->p);
						x = handle;
					}
				}
			}
			x->color = BLACK;
		}

		Node* RBdelete(Node* z)
		{
			Node* x = nil;
			Node* y = nil;
			if (z->l == nil || z->r == nil)
			{
				y = z;
			}
			else 
			{
				y = successor_(z);
			}
			if (y->l != nil)
			{
				x = y->l;
			}
			else
			{
				x = y->r;
			}
			x->p = y->p;
			if (y->p == nil)
			{
				handle = x;
			}
			else 
			{
				if (y == y->p->l)
				{
					y->p->l = x;
				}
				else
				{
					y->p->r = x;
				}
			}
			if (y != z)
			{
				z->key = y->key;
				z->value = y->value;
			}
			if (y->color == BLACK) 
			{
				RBdelete_fixup(x);
			}
			return y;
		}

		void RBinsert_fixup(Node* z)
		{
			Node* y = nil;
			while (z->p->color == RED)
			{
				if (z->p == z->p->p->l)
				{
					y = z->p->p->r;
					if (y->color == RED)
					{
						z->p->color = BLACK;
						y->color = BLACK;
						z->p->p->color = RED;
						z = z->p->p;
					}
					else 
					{
						if (z == z->p->r)
						{
							z = z->p;
							left_rotate(z);
						}
						z->p->color = BLACK;
						z->p->p->color = RED;
						right_rotate(z->p->p);
					}
				}
				else
				{
					y = z->p->p->l;
					if (y->color == RED)
					{
						z->p->color = BLACK;
						y->color = BLACK;
						z->p->p->color = RED;
						z = z->p->p;
					}
					else 
					{
						if (z == z->p->l)
						{
							z = z->p;
							right_rotate(z);
						}
						z->p->color = BLACK;
						z->p->p->color = RED;
						left_rotate(z->p->p);
					}
				}
			}
			handle->color = BLACK;
		}

		void RBinsert(Node* new_node)
		{
			Node* x = handle;
			Node* y = nil;

			while (x != nil)
			{
				y = x;
				if (new_node->key < x->key)
				{
					x = x->l;
				}
				else
				{
					x = x->r;
				}
			}
			new_node->p = y;
			if (y == nil)
			{
				handle = new_node;
			}
			else 
			{
				if (new_node->key < y->key)
				{
					y->l = new_node;
				}
				else
				{
					y->r = new_node;
				}
			}
			new_node->l = nil;
			new_node->r = nil;

			RBinsert_fixup(new_node);
		}
	};

	template<typename K, typename V>
	struct Show
	{
		std::ostream& out;
		std::string sep;

		Show(std::ostream& o, const std::string& s = "")
			: out(o), sep(s) { }

		void operator()(std::pair<K, V> p) 
		{
			out << "K: " << p.first << "\t V: " << p.second << sep;
		}
	};
}

#endif // MMRBTREE_H_INCLUDE_GUARD