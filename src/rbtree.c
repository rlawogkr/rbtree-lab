#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->root = p->nil;
  
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
  return;
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
  return;
}

void rb_insert_fixup(rbtree *t, node_t *z) { // z는 삽입된 노드
   while (z->parent->color == RBTREE_RED) {      // z의 부모가 RED일때
    if (z->parent == z->parent->parent->left) { // z의 부모가 z의 조상의 왼쪽 자식이라면
      node_t *y = z->parent->parent->right;     // y는 z의 조상의 오른쪽 자식(삼촌)
      if (y->color == RBTREE_RED) {             // 경우 1. z의 삼촌의 색이 빨강일때
        z->parent->color = RBTREE_BLACK;          // z의 부모 색을 BLACK으로 변경
        y->color = RBTREE_BLACK;                  // z의 삼촌의 색을 BLACK으로 변경 
        z->parent->parent->color = RBTREE_RED;    // z의 조상의 색을 RED로 변경 
        z = z->parent->parent;                    // z는 조상으로 변경
      } else {                                  // 경우 2. z의 삼촌의 색이 검정일때
          if (z == z->parent->right) {            // z가 z의 부모의 오른쪽 자식일때
            z = z->parent;                        // z는 z의 부모로 변경
            left_rotate(t, z);                  // z를 기준으로 좌회전
        }                                       // 경우 3. z의 삼촌의 색이 검정이고 z가 z의 부모의 왼쪽 자식일 때
        z->parent->color = RBTREE_BLACK;          // z의 부모 색을 검정으로 변경
        z->parent->parent->color = RBTREE_RED;    // z의 조상의 색을 레드로 변경
        right_rotate(t, z->parent->parent);     // z의 조상 기준으로 우회전
      }
    } else {                                    // z의 부모가 z의 조상의 오른쪽이라면 왼쪽과 반대로 함
      node_t *y = z->parent->parent->left;
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else {
          if (z == z->parent->left) {
            z = z->parent;
            right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;                // 특성 2을 유지하기 위해 root는 항상 BLACK으로 설정
  return;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t* y = t->nil;     // y는 트리의 nil노드
  node_t* x = t->root;    // x는 트리의 root노드
  while (x != t->nil) {   // x가 트리의 nil노드가 아닐 때 반복(즉, x의 자식들이 leaf노드에 도달할 때 까지)
    y = x;                // y는 x
    if (key < x->key) {   // 만약 x의 key값보다 삽입할 key값이 작으면
      x = x->left;        // x는 x의 왼쪽으로 변경
    } else {              // 만약 x의 key값보다 삽입할 key값이 크거나 같으면
      x = x->right;       // x는 x의 오른쪽으로 변경
    }
  }
  node_t* z = (node_t *)calloc(1, sizeof(node_t));  // z(노드) 생성
  z->parent = y;              // z의 부모는 y
  if (y == t->nil) {          // y가 트리의 nil일 때(즉, 첫 노드 삽입)
    t->root = z;              // 트리의 root는 z
  } else if (key < y->key) {  // y의 key값이 삽입할 키 값보다 작을 때
    y->left = z;              // y의 왼쪽 자식은 z
  } else {                    // y의 key값이 삽입할 키 값보다 크거나 같을 때
    y->right = z;             // y의 오른쪽 자식은 z
  }
  z->key = key;               // z의 key값은 현재 key
  z->color = RBTREE_RED;      // z의 color값은 RED, 삽입할 때는 무조건 RED
  z->left = t->nil;           // z의 왼쪽 자식은 트리의 nill, 삽입할 때는 무조건 자식은 nill
  z->right = t->nil;          // z의 왼쪽 자식은 트리의 nill, 삽입할 때는 무조건 자식은 nill
  rb_insert_fixup(t, z);  // fixup 호출
  return t->root;             // 트리의 root값 반환
}

void delete_rbtree_node(rbtree *t, node_t *x) {
  if(x->left != t->nil) { // x의 왼쪽 자식이 경계 노드가 아니라면
    delete_rbtree_node(t, x->left);
  }
  if(x->right != t->nil) { // x의 오른쪽 자식이 경계 노드가 아니라면
    delete_rbtree_node(t, x->right);
  }
  free(x); // x의 메모리 해제
  return;
}

//using 재귀
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if(t->root != t->nil) { // root가 nil이 아닐 때
    delete_rbtree_node(t, t->root);
  }
  free(t->nil);
  free(t);
  return;
}

// find should return the node with the key or NULL if no such node exists
node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *x = t->root;
  while(x != t->nil && key != x->key) {
    if(key < x->key) {
      x = x->left;
    } else {
      x = x->right;
    }
  }
  if (x == t->nil) {
    return NULL;
  }
  return x;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *x = t->root;//root부터 탐색
  while(x->left != t->nil) { // x의 왼쪽 자식이 경계 노드가 아니라면
    x = x->left;
  }
  return x;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *x = t->root;
  while(x->right != t->nil) { // x의 오른쪽 자식이 경계 노드가 아니라면
    x = x->right;
  }
  return x;
}

void rb_transplant(rbtree *t, node_t *u, node_t *v) {
  if(u->parent == t->nil) {
    t->root = v;
  } else if(u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }
  v->parent = u->parent;
  return;
}

void rb_delete_fixup(rbtree *t, node_t *x) {
  node_t *w;                                                                         
    while ((x != t->root) && (x->color == RBTREE_BLACK)) {                            // 삭제한 노드의 자식이 루트가 아니고 색깔이 BLACK일 때, 반복
        if (x == x->parent->left) {                                                   // 경우 1. 삭제한 노드의 자식이 삭제한 노드의 부모의 왼쪽 자식일 때
            w = x->parent->right;                                                       // w는 x의 오른쪽 형제
            if (w->color == RBTREE_RED) {                                               // w의 색깔이 RED일 때
                w->color = RBTREE_BLACK;                                                // w의 색깔이 BLACK으로 변경
                x->parent->color = RBTREE_RED;                                          // x의 부모의 색을 RED로 변경
                left_rotate(t, x->parent);                                            // x의 부모를 기준으로 좌회전
                w = x->parent->right;                                                   // w는 형제
            }                                                                           // 경우 1을 해결할 경우 경우 2, 3, 4중에 발생
            if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {  // 경우 2. w의 왼쪽 자식과 오른쪽 자식이 BLACK일 때
                w->color = RBTREE_RED;                                                  // w의 색깔을 RED로 변경 
                x = x->parent;                                                          // x는 x의 부모로 변경
            }  else {
                if (w->right->color == RBTREE_BLACK) {                                // 경우 3. 경우 2가 아니고, w의 오른쪽 색이 BLACK일 때
                    w->left->color = RBTREE_BLACK;                                      // w의 왼쪽 색이 BLACK으로 변경
                    w->color = RBTREE_RED;                                              // w의 색은 RED로 변경
                    right_rotate(t, w);                                               // w를 기준으로 우회전
                    w = x->parent->right;                                               // w는 x의 부모의 오른쪽 자식
                }
                w->color = x->parent->color;                                          // 경우 4. 경우 2가 아닐 때, w의 오른쪽 자식이 RED일 때
                x->parent->color = RBTREE_BLACK;                                        // x의 색은 x의 부모의 색을 BLACK으로 변경
                w->right->color = RBTREE_BLACK;                                         // w의 오른쪽 자식 색을 BLACK으로 변경
                left_rotate(t, x->parent);                                            // x의 부모를 기준으로 좌회전
                x = t->root;                                                            // x는 트리의 루트
            }
        } else {                                                                      // 삭제한 노드의 자식이 삭제한 노드의 부모의 오른쪽 자식일 때, 왼쪽일 때의 반대
            w = x->parent->left;
            if (w->color == RBTREE_RED) {
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                right_rotate(t, x->parent);
                w = x ->parent->left;
            }
            if (w->right ->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
                w->color = RBTREE_RED;
                x = x->parent;
            } else {
                if (w->left->color == RBTREE_BLACK) {
                    w->right->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    left_rotate(t, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->left -> color = RBTREE_BLACK;
                right_rotate(t, x->parent);
                x = t->root;
            }
        }
    }
    x->color = RBTREE_BLACK;                                                        // 트리의 루트를 BLACK으로 변경
    return;
}
//후임자 찾기
node_t* rb_successor(rbtree* t, node_t *x){
  node_t* y = x;
  while(y->left != t->nil){
    y = y->left;
  }
  return y;
}

int rbtree_erase(rbtree *t, node_t *z) {

  node_t *x;                              // 노드 x
  node_t *y = z;                          // y는 삭제할 노드
  color_t y_color = y->color;             // y_color는 y의 색
  if (z->left == t->nil) {                // 삭제할 노드의 왼쪽이 nil일때
    x = z->right;                         // x는 삭제할 노드의 오른쪽
    rb_transplant(t, z, z->right);    // 삭제할 노드의 부모와 삭제할 노드의 오른쪽을 연결 
  } else if (z->right == t->nil) {        // 삭제할 노드의 오른쪽이 nill일때
    x = z->left;                          // x는 삭제할 노드의 왼쪽
    rb_transplant(t, z, z->left);     // 삭제할 노드의 부모와 삭제할 노드의 왼쪽을 연결
  } else {                                // 삭제할 노드의 왼쪽, 오른쪽 자식이 둘다 있을 때
    y = rb_successor(t, z->right);    // y는 직후 원소
    y_color = y->color;                   // y_color는 직후 원소의 색
    x = y->right;                         // x는 y의 오른쪽 자식
    if (y->parent == z) {                 // y의 부모가 삭제할 노드일 때
      x->parent = y;                      // x의 부모는 y
    } else {                              // y의 부모가 삭제할 노드가 아닐 때
      rb_transplant(t, y, y->right);  // y의 부모와 y의 오른쪽 자식을 연결
      y->right = z->right;                // y의 오른쪽 자식은 삭제할 노드의 오른쪽 자식
      y->right->parent = y;               // y의 오른쪽 자식의 부모는 y
    }
    rb_transplant(t, z, y);           // 삭제할 노드 부모와 y를 연결
    y->left = z->left;                    // y의 왼쪽 자식은 삭제할 노드의 왼쪽 자식
    y->left->parent = y;                  // y의 왼쪽 자식의 부모는 y
    y->color = z->color;                  // y의 색은 삭제할 노드의 색
  }
                               // 삭제한 노드가 가리키는 공간 삭제
                               // 할당 해제 후 삭제한 노드값을 NULL로 초기화
  if (y_color == RBTREE_BLACK) {          // y_color가 BLACK 일 때, 즉 삭제한 노드의 색이 BLACK일 때(특성 5 위반)
    rb_delete_fixup(t, x);                // 노드의 색을 바꿈
  }
  free(z);   
  z = NULL;  
  return 0;
}


//rb트리 중위순회
int rb_inorder(node_t *nil, node_t *x, key_t *arr, int i){
  if(x != nil){
    i = rb_inorder(nil, x->left, arr, i);
    arr[i++] = x->key;
    i = rb_inorder(nil, x->right, arr, i);
  }
  return i;
}
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {//오름차순 출력
  // TODO: implement to_array
  if(t->root == t->nil) {
    return 0;
  } 
  rb_inorder(t->nil, t->root, arr, 0);
  return 0;
}