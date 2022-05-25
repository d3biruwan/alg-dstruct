#pragma warning(disable:4996)


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct node_t {
	struct node_t* parent;
	struct node_t* son[3];
	int height, full;
	int key[2];
} node_t;


node_t* init_node(node_t* brother, int key, node_t* pointer);
node_t* search_node(node_t* root, int key);
node_t* split_parent(node_t* son, int key);
node_t* insert_node(node_t* root, int key, node_t* pointer);
int find_node(node_t* tree, int key);
void destroy_tree(node_t* root);
node_t* remove_node(node_t* root, int key);
void print_tree(node_t* t, int offset);
node_t* split_tree(node_t* tree, int key, node_t** pLeft, node_t** pRight);
node_t* merge_tree(node_t* left, node_t* right);


node_t* init_node(node_t* brother, int key, node_t* pointer)
{
	node_t* res = (node_t*)malloc(sizeof(node_t));
	if (res)
	{
		res->full = 0;
		res->key[0] = key;
		res->son[0] = pointer;
		if (brother)
		{
			res->height = brother->height;
			if (res->height)
			{
				res->son[0]->parent = res;
			}
			res->parent = brother->parent;
			res->son[2] = brother->son[2];
			brother->son[2] = res;
		}
		else
		{
			res->height = 0;
			res->parent = NULL;
			res->son[2] = NULL;
		}
	}
	return res;
}


node_t* search_node(node_t* tree, int key)
{
	if (!tree)
	{
		return NULL;
	}
	node_t* current_node = tree;
	while (current_node->height)
	{
		if (key <= current_node->key[0])
		{
			current_node = current_node->son[0];
		}
		else if (current_node->full && key > current_node->key[1])
		{
			current_node = current_node->son[2];
		}
		else
		{
			current_node = current_node->son[1];
		}
	}
	return current_node;
}


int find_node(node_t* tree, int key)
{
	node_t* leaf = search_node(tree, key);
	if (!leaf)
		return NULL;
	if (key == leaf->key[0] || (leaf->full && key == leaf->key[1]))
	{
		return 1;
	}
	return 0;
}


node_t* find_root(node_t* node)
{
	node_t* cur = node;
	while (cur->parent)
	{
		cur = cur->parent;
	}
	return cur;
}


void leaf_insert(node_t* node, int key, node_t* pointer)
{
	if (key > node->key[0])
	{
		node->key[1] = key;
		node->son[1] = pointer;
	}
	else
	{
		node->key[1] = node->key[0];
		node->son[1] = node->son[0];
		node->key[0] = key;
		node->son[0] = pointer;
	}
}


node_t* insert_node(node_t* tree, int key, node_t* pointer)
{
	if (!tree)
	{
		return init_node(NULL, key, pointer);
	}
	node_t* insert_node = search_node(tree, key);
	if (key == insert_node->key[0] || (insert_node->full && key == insert_node->key[1]))
	{
		return tree;
	}
	if (!insert_node->full)
	{
		leaf_insert(insert_node, key, pointer);
		insert_node->full = 1;
		return tree;
	}
	else
	{
		if (key < insert_node->key[0])
		{
			node_t* next = init_node(insert_node, insert_node->key[1], insert_node->son[1]);
			if (!next)
			{
				destroy_tree(tree);
				return NULL;
			}
			leaf_insert(insert_node, key, pointer);
			return split_parent(insert_node, insert_node->key[1]);
		}
		else if (key < insert_node->key[1])
		{
			node_t* next = init_node(insert_node, insert_node->key[1], insert_node->son[1]);
			if (!next)
			{
				destroy_tree(tree);
				return NULL;
			}
			leaf_insert(insert_node, key, pointer);
			return split_parent(insert_node, key);
		}
		else
		{
			node_t* next = init_node(insert_node, key, pointer);
			if (!next)
			{
				destroy_tree(tree);
				return NULL;
			}
			return split_parent(insert_node, insert_node->key[1]);
		}
	}
}


node_t* split_parent(node_t* son, int key)
{
	node_t* node = son->parent;
	if (!node)
	{
		node = init_node(NULL, key, son);
		if (!node)
		{
			destroy_tree(son->son[2]);
			destroy_tree(son);
			return NULL;
		}
		node->height = son->height + 1;
		node->son[1] = son->son[2];
		son->son[2] = NULL;
		son->parent = node;
		node->son[1]->parent = node;
		return node;
	}
	if (!node->full)
	{
		if (key > node->key[0])
		{
			node->key[1] = key;
			node->son[2] = son->son[2];
			node->son[2]->parent = node;
			son->son[2] = NULL;
		}
		else
		{
			node->key[1] = node->key[0];
			node->son[2] = node->son[1];
			node->key[0] = key;
			node->son[1] = son->son[2];
			node->son[1]->parent = node;
			son->son[2] = NULL;
		}
		node->full = 1;
	}
	else
	{
		if (key < node->key[0])
		{
			node_t* next = init_node(node, node->key[1], node->son[1]);
			if (!next)
			{
				destroy_tree(son->son[2]);
				destroy_tree(find_root(node));
				return NULL;
			}
			node->full = 0;
			node->key[1] = 0;
			next->son[1] = next->son[2];
			next->son[2] = NULL;
			next->son[1]->parent = next;
			int old_key = node->key[0];
			node->key[0] = key;
			node->son[1] = son->son[2];
			son->son[2] = NULL;
			return split_parent(node, old_key);
		}
		else if (key < node->key[1])
		{
			node_t* next = init_node(node, node->key[1], son->son[2]);
			if (!next)
			{
				destroy_tree(son->son[2]);
				destroy_tree(find_root(node));
				return NULL;
			}
			node->full = 0;
			node->key[1] = 0;
			next->son[1] = next->son[2];
			next->son[1]->parent = next;
			next->son[2] = NULL;
			son->son[2] = NULL;
			return split_parent(node, key);
		}
		else
		{
			node_t* next = init_node(node, key, node->son[2]);
			if (!next)
			{
				destroy_tree(son->son[2]);
				destroy_tree(find_root(node));
				return NULL;
			}
			node->full = 0;
			int old_key = node->key[1];
			node->key[1] = 0;
			next->son[1] = son->son[2];
			next->son[2] = NULL;
			next->son[1]->parent = next;
			son->son[2] = NULL;
			return split_parent(node, old_key);
		}
	}
	return find_root(node);
}


void destroy_tree(node_t* root)
{
	if (root)
	{
		if (root->height)
		{
			destroy_tree(root->son[0]);
			destroy_tree(root->son[1]);
			if (root->full)
			{
				destroy_tree(root->son[2]);
			}
		}
		free(root);
	}
}


void print_tree(node_t* tree, int offset)
{
	if (!tree)
	{
		return;
	}
	if (!tree->height)
	{
		if (offset > 0)
		{
			for (int i = 0; i < offset; i++)
			{
				printf(" ");
			}
		}
		printf("[%2d", tree->key[0]);
		if (tree->full)
		{
			printf(", %2d", tree->key[1]);
		}
		printf("]\n");
	}
	else
	{
		print_tree(tree->son[0], offset + 4);
		if (!tree->full)
		{
			if (offset > 0)
			{
				for (int i = 0; i < offset; i++)
				{
					printf(" ");
				}
			}
			printf("%2d\n", tree->key[0]);
			print_tree(tree->son[1], offset + 4);
		}
		else
		{
			for (int i = 0; i < offset; i++)
			{
				printf(" ");
			}
			printf("%2d\n", tree->key[0]);
			print_tree(tree->son[1], offset + 4);
			for (int i = 0; i < offset; i++)
			{
				printf(" ");
			}
			printf("%2d\n", tree->key[1]);
			print_tree(tree->son[2], offset + 4);
		}
	}
}


node_t* merge_tree(node_t* left, node_t* right)
{
	if (!left)
	{
		return right;
	}
	if (!right)
	{
		return left;
	}
	if (left->height == right->height)
	{
		node_t* leftParent = left->parent;
		node_t* rightParent = right->parent;
		int lmax;
		node_t* cur = left;
		while (cur->height)
		{
			cur = (!cur->full) ? cur->son[1] : cur->son[2];
		}
		lmax = (!cur->full) ? cur->key[0] : cur->key[1];
		if (leftParent)
		{
			right->parent = leftParent;
			if (!leftParent->full)
			{
				leftParent->key[1] = lmax;
				leftParent->son[2] = right;
				leftParent->full = 1;
				return find_root(leftParent);
			}
			else
			{
				node_t* next = init_node(leftParent, lmax, left);
				if (!next)
				{
					return NULL;
				}
				next->son[1] = right;
				right->parent = next;
				next->son[2] = NULL;
				leftParent->full = 0;
				return split_parent(leftParent, leftParent->key[1]);
			}
		}
		else if (rightParent)
		{
			left->parent = rightParent;
			if (!rightParent->full)
			{
				rightParent->key[1] = rightParent->key[0];
				rightParent->son[2] = rightParent->son[1];
				rightParent->son[1] = rightParent->son[0];

				rightParent->key[0] = lmax;
				rightParent->son[0] = left;
				rightParent->full = 1;
				return find_root(rightParent);
			}
			else
			{
				node_t* next = init_node(rightParent, rightParent->key[1], rightParent->son[1]);
				if (!next)
				{
					return NULL;
				}
				rightParent->full = 0;
				next->son[1] = next->son[2];
				next->son[1]->parent = next;
				next->son[2] = NULL;
				int old_key = rightParent->key[0];
				rightParent->son[1] = rightParent->son[0];
				rightParent->son[0] = left;
				rightParent->key[0] = lmax;
				return split_parent(rightParent, old_key);
			}
		}
		else
		{
			node_t* tree = init_node(NULL, lmax, left);
			if (!tree)
			{
				return NULL;
			}
			tree->height = left->height + 1;
			tree->son[1] = right;
			left->parent = tree;
			right->parent = tree;
			return tree;
		}
	}
	if (left->height < right->height)
	{
		return merge_tree(left, right->son[0]);
	}
	else
	{
		return merge_tree((!left->full) ? left->son[1] : left->son[2], right);
	}
}


node_t* extract_node(node_t** ptr)
{
	node_t* res = *ptr;
	*ptr = NULL;
	res->parent = NULL;
	return res;
}


node_t* split_tree(node_t* tree, int key, node_t** pLeft, node_t** pRight)
{
	if (tree)
	{
		if (tree->height)
		{
			node_t* left = extract_node(&tree->son[0]);
			node_t* mid = extract_node(&tree->son[1]);
			node_t* right = NULL;
			if (tree->full)
			{
				right = extract_node(&tree->son[2]);
			}
			if (key <= tree->key[0])
			{
				if (right)
				{
					*pRight = merge_tree(right, *pRight);
				}
				*pRight = merge_tree(mid, *pRight);
				free(tree);
				return split_tree(left, key, pLeft, pRight);
			}
			*pLeft = merge_tree(*pLeft, left);
			if (tree->full && key > tree->key[1])
			{
				*pLeft = merge_tree(*pLeft, mid);
				free(tree);
				return split_tree(right, key, pLeft, pRight);
			}
			if (right)
			{
				*pRight = merge_tree(right, *pRight);
			}
			free(tree);
			return split_tree(mid, key, pLeft, pRight);
		}
		if (key < tree->key[0])
		{
			*pRight = insert_node(*pRight, tree->key[0], tree->son[0]);
			if (tree->full)
			{
				*pRight = insert_node(*pRight, tree->key[1], tree->son[1]);
			}
			free(tree);
			return NULL;
		}
		if (key == tree->key[0])
		{
			if (tree->full)
			{
				*pRight = insert_node(*pRight, tree->key[1], tree->son[1]);
			}
			tree->parent = NULL;
			tree->key[1] = 0;
			tree->son[1] = NULL;
			return tree;
		}
		*pLeft = insert_node(*pLeft, tree->key[0], tree->son[0]);
		if (tree->full && key < tree->key[1])
		{
			*pRight = insert_node(*pRight, tree->key[1], tree->son[1]);
			free(tree);
			return NULL;
		}
		if (tree->full && key == tree->key[1])
		{
			tree->parent = NULL;
			tree->key[0] = key;
			tree->son[0] = tree->son[1];
			tree->key[1] = 0;
			tree->son[1] = NULL;
			tree->full = 0;
			return tree;
		}
		if (tree->full)
		{
			*pLeft = insert_node(*pLeft, tree->key[1], tree->son[1]);
		}
		free(tree);
		return NULL;
	}
	return NULL;
}


node_t* remove_node(node_t* tree, int key)
{
	node_t* left = NULL;
	node_t* right = NULL;
	if (!find_node(tree, key))
	{
		return tree;
	}
	node_t* m = split_tree(tree, key, &left, &right);
	free(m);
	return merge_tree(left, right);
}


int test(node_t* p) {
	node_t* temp;
	char buffer[16];
	char a;
	int key;
	while (fgets(buffer, 16, stdin)) {
		sscanf(buffer, "%c %d", &a, &key);
		switch (a) {
		case 'f':
			if (find_node(p, key))
				printf("yes\n");
			else
				printf("no\n");
			break;
		case 'r':
			p = remove_node(p, key);
			break;
		case 'a':
			p = insert_node(p, key, NULL);
			break;
		case 'p':
			print_tree(p, key);
		default:
			destroy_tree(p);
			return 0;
		}
	}
	destroy_tree(p);
	return 0;
}


int main() {
	node_t* p = NULL;
	test(p);
	return 0;
}