#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t));	// 경계 노드 생성
  nil_node->color = RBTREE_BLACK;							// 경계 노드 색(검정)
  p->nil = nil_node;										// 트리의 경계 노드는 생성한 경계 노드 
  p->root = nil_node;										// 트리의 루트 노드는 생성한 경계 노드
  
  return p;
}

void left_rotate(rbtree *t, node_t *x) {
  node_t *y = x->right;	// y는 x의 오른쪽 자식 노드
  x->right = y->left;		// y의 왼쪽 자식을 x의 오른쪽 자식으로 만듦
  if(y->left != t->nil) {
    y->left->parent = x;	// y의 왼쪽 자식의 부모를 x로 설정
  }
  y->parent = x->parent;	// y의 부모를 x의 부모로 설정
  if(x->parent == t->nil) {
    t->root = y;		// x가 루트 노드였다면 y를 루트 노드로 설정
  } else if(x == x->parent->left) {
    x->parent->left = y;	// x가 부모의 왼쪽 자식이었다면 y를 부모의 왼쪽 자식으로 설정
  } else {
    x->parent->right = y;	// x가 부모의 오른쪽 자식이었다면 y를 부모의 오른쪽 자식으로 설정
  }
  y->left = x;			// x를 y의 왼쪽 자식으로 설정
  x->parent = y;			// x의 부모를 y로 설정
}

void right_rotate(rbtree *t, node_t *x){
  node_t *y = x->left;	// y는 x의 왼쪽 자식 노드
  x->left = y->right;	// y의 오른쪽 자식을 x의 왼쪽 자식으로 만듦
  if(y->right != t->nil) {
    y->right->parent = x;	// y의 오른쪽 자식의 부모를 x로 설정
  }
  y->parent = x->parent;	// y의 부모를 x의 부모로 설정
  if(x->parent == t->nil) {
    t->root = y;		// x가 루트 노드였다면 y를 루트 노드로 설정
  } else if(x == x->parent->right) {
    x->parent->right = y;	// x가 부모의 오른쪽 자식이었다면 y를 부모의 오른쪽 자식으로 설정
  } else {
    x->parent->left = y;	// x가 부모의 왼쪽 자식이었다면 y를 부모의 왼쪽 자식으로 설정
  }
  y->right = x;			// x를 y의 오른쪽 자식으로 설정
  x->parent = y;			// x의 부모를 y로 설정
}

void rb_insert_fixup(rbtree *t, node_t *z) { // z는 삽입된 노드
  while(z->parent->color == RBTREE_RED) {
    if(z->parent == z->parent->parent->left) {
      node_t *y = z->parent->parent->right;// y는 z의 삼촌 노드
      if(y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else {
        if(z == z->parent->right) { //z가 부모의 오른쪽 자식이라면
          z = z->parent;
          left_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    } else {
      node_t *y = z->parent->parent->left;
      if(y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else {
        if(z == z->parent->left) {
          z = z->parent;
          right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *z = (node_t *)calloc(1, sizeof(node_t));	// 삽입할 노드 setting
  z->key = key;
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;							// 삽입할 노드의 색(빨강)

  node_t *y = t->nil;                // y는 x의 부모 노드
  node_t *x = t->root;               // x는 트리의 루트 노드

  while (x != t->nil) {
    y = x;
    if (z->key < x->key) {
      x = x->left;
    } else {
      x = x->right;
    }
  }

  z->parent = y;
  if(y == t->nil) {
    t->root = z;
  } else if(z->key < y->key) {
    y->left = z;
  } else {
    y->right = z;
  }
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  rb_insert_fixup(t, z);

  return z;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
