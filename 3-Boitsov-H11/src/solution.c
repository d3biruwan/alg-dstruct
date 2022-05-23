#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>


typedef struct node_t {
	int key;
	int height;
	int data;
	struct node_t* left;
	struct node_t* right;
}node_t;


node_t* create_node(int key, int data) {
	node_t* p = (node_t*)malloc(sizeof(node_t));
	if (!p)
		return NULL;
	p->data = data;
	p->left = NULL;
	p->right = NULL;
	p->height = 1;
	p->key = key;
	return p;
}


int height(node_t* p) {
	return p ? p->height : 0;
}


int balance_factor(node_t* p) {
	return height(p->right) - height(p->left);
}


void fix_height(node_t* p) {
	int hl = height(p->left);
	int hr = height(p->right);
	p->height = (hl > hr ? hl : hr) + 1;
}


node_t* rotate_right(node_t* p) {
	node_t* q = p->left;
	p->left = q->right;
	q->right = p;
	fix_height(p);
	fix_height(q);
	return q;
}


node_t* rotate_left(node_t* q) {
	node_t* p = q->right;
	q->right = p->left;
	p->left = q;
	fix_height(q);
	fix_height(p);
	return p;
}


node_t* balance(node_t* p) {
	fix_height(p);
	if (balance_factor(p) == 2) {
		if (balance_factor(p->right) < 0)
			p->right = rotate_right(p->right);
		return rotate_left(p);
	}
	if (balance_factor(p) == -2) {
		if (balance_factor(p->left) > 0)
			p->left = rotate_left(p->left);
		return rotate_right(p);
	}
	return p;
}


node_t* insert(node_t* p, int key, int data) {
	if (!p)
		return create_node(key,data);
	if (p->key == key) {
		p->data = data;
		return p;
	}
	if (key < p->key)
		p->left = insert(p->left, key, data);
	if (key > p->key)
		p->right = insert(p->right, key, data);
	return balance(p);
}


node_t* find_max(node_t* p) {
	return p->right ? find_max(p->right) : p;
}


node_t* remove_max(node_t* p) {
	if (!p->right)
		return p->left;
	p->right = remove_max(p->right);
	return balance(p);
}


node_t* find_min(node_t* p) {
	return p->left ? find_min(p->left) : p;
}


node_t* remove_min(node_t* p) {
	if (!p->left)
		return p->right;
	p->left = remove_min(p->left);
	return balance(p);
}


node_t* delete_key(node_t* p, int key) {
	if (!p)
		return NULL;
	if (key < p->key)
		p->left = delete_key(p->left, key);
	else if (key > p->key)
		p->right = delete_key(p->right, key);
	else {
		node_t* q = p->left;
		node_t* r = p->right;
		free(p);
		if (!r) 
			return q;
		node_t* min = find_min(r);
		min->right = remove_min(r);
		min->left = q;
		return balance(min);
	}
	return balance(p);
}


node_t* delete_data(node_t* p, int data) {
	if (!p) 
		return NULL;
	if (data == p->data) {
		node_t* q = p->left;
		node_t* r = p->right;
		free(p);
		if (!r) 
			return q;
		node_t* min = find_min(r);
		min->right = remove_min(r);
		min->left = q;
		return balance(min);
	}
	node_t* q = delete_data(p->left, data);
	node_t* r = delete_data(p->right, data);
	return balance(p);
}


node_t* search_data(node_t* p, int data) {
	if (!p)
		return NULL;
	if (data == p->data)
		return p;
	node_t* q = search_data(p->left, data);
	if ((q)&&(q->data == data))
		return q;
	node_t* r = search_data(p->right, data);
	if ((r)&&(r->data == data)) 
		return r;
	return NULL;
}


node_t* search_key(node_t* p, int k) {
	if (!p) {
		fprintf(stdout, "no\n");
		return NULL;
	}
	if (k == p->key) {
		fprintf(stdout, "yes\n");
		return p;
	}
	if (k < p->key)
		return search_key(p->left, k);
	else
		return search_key(p->right, k);
}


void destroy_tree(node_t* p) {
	if (p) {
		destroy_tree(p->left);
		destroy_tree(p->right);
		free(p);
	}
}

void set_keys(node_t* p, int val) {
	if (p) {
		p->key += val+1;
		set_keys(p->left, val);
		set_keys(p->right, val);
	}	
}

node_t* merge_balancing(node_t* prev_branch, node_t* left_tree, node_t* cur_branch, node_t* rotate_point) {
	if (height(cur_branch) - height(left_tree) > 1) 
		cur_branch=merge_balancing(cur_branch, left_tree, cur_branch->left, rotate_point);
	else {
		node_t* temp_node = cur_branch;
		rotate_point->height = height(cur_branch) + 1;
		rotate_point->left = left_tree;
		rotate_point->right = temp_node;
		if(prev_branch)
			prev_branch->left = rotate_point;
	}
	if (prev_branch)
		return balance(prev_branch);
	else
		return cur_branch;
}


node_t* merge(node_t* p, node_t* q) {
	if (!p)
		return q;
	if (!q)
		return p;
	node_t* right_tree = height(p) > height(q) ? p : q;
	node_t* left_tree = (right_tree == p) ? q : p;
	node_t* rotate_point = find_max(left_tree);
	remove_max(left_tree);
	return merge_balancing(NULL, left_tree, right_tree, rotate_point);
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
			temp = search_key(p, key);
			break;
		case 'r':
			p = delete_key(p, key);
			break;
		case 'a':
			p = insert(p, key, 1);
			break;
		default: 
			destroy_tree(p);
			return 0;
		}
	}
	destroy_tree(p);
	return 0;
}


int main() {
	node_t* p=NULL;
	test(p);
	return 0;
}
