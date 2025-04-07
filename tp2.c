#include "tp2.h"
#include <stdlib.h>
#include <stdbool.h>

struct node;
typedef struct node node_t;

struct node {
    void* value;
    node_t* next;
    node_t* prev;
};

struct list {
    node_t* head;
    node_t* tail;
    size_t size;
};

struct list_iter {
    list_t* list;
    node_t* curr;
};

list_t *list_new(void) {
    list_t *lista = malloc(sizeof(list_t));
    if (!lista) return NULL;
    
    lista->head = NULL;
    lista->tail = NULL;
    lista->size = 0;
    
    return lista;
}

size_t list_length(const list_t *list) {
    return list->size;
}

bool list_is_empty(const list_t *list) {
    if (list->size == 0) {
        return true;
    }
    return false;
}

bool list_insert_head(list_t *list, void *value){
    
    node_t *new_head = malloc(sizeof(node_t));
    if (new_head == NULL) {
        return false;
    }
    new_head->value = value;
    new_head->next = list->head;
    new_head->prev = NULL;
    if (list->head != NULL) {
        list->head->prev = new_head;
    } else {
        list->tail = new_head;
    }
    list->head = new_head;
    list->size++;

    return true;
}

bool list_insert_tail(list_t *list, void *value){
    node_t *new_tail = malloc(sizeof(node_t));
    if (new_tail == NULL) {
        return false;
    }
    new_tail->value = value;
    new_tail->next = NULL;
    new_tail->prev = list->tail;
    if (list->tail != NULL) {
        list->tail->next = new_tail;
    } else {
        list->head = new_tail;
    }
    list->tail = new_tail;
    list->size++;

    return true;
    
}

void *list_peek_head(const list_t *list){
    if(list->size == 0){
        return NULL;
    }
    return list->head->value;
    
}

void *list_peek_tail(const list_t *list){
    if(list->size == 0){
        return NULL;
    }
    return list->tail->value;
    
    
}

void *list_pop_head(list_t *list){
    if(list->size == 0){
        return NULL;
    }
    void *head_value = list->head->value;
    node_t *second_node = list->head->next;
    free(list->head);
    list->head = second_node;
    if (list->head != NULL){
        list->head->prev = NULL;
    }else{
        list->tail = NULL;
    }
    list->size--;
    return head_value;
}

void *list_pop_tail(list_t *list){
    if(list->size == 0){
        return NULL;
    }
    void *tail_value = list->tail->value;
    node_t *new_tail = list->tail->prev;
    free(list->tail);
    list->tail = new_tail;
    if (list->tail != NULL){
        list->tail->next = NULL;
    }else{
        list->tail = NULL;
    }
    list->size--;
    return tail_value;
}

void list_destroy(list_t *list, void destroy_value(void *)){
    while (list->size > 0) {
        void *value = list_pop_head(list);
        if (destroy_value != NULL) {
            destroy_value(value);
        }
    }
}

list_iter_t *list_iter_create_head(list_t *list){
    if (list->size == 0 && list == NULL){
        return NULL;
    }
    list_iter_t *iter = malloc(sizeof(list_iter_t));
    iter->curr = list->head;
    return iter;
}

list_iter_t *list_iter_create_tail(list_t *list){
    if (list->size == 0 && list == NULL){
        return NULL;
    }
    list_iter_t *iter = malloc(sizeof(list_iter_t));
    iter->curr = list->tail;
    return iter;
}

bool list_iter_forward(list_iter_t *iter){
    if (iter == NULL || iter->curr == NULL || iter->curr->next == NULL) {
        return false;
    }
    iter->curr = iter->curr->next; 
    return true;
}

bool list_iter_backward(list_iter_t *iter){
    if (iter == NULL || iter->curr == NULL || iter->curr->prev == NULL) {
        return false;
    }
    iter->curr = iter->curr->prev; 
    return true;
}

void *list_iter_peek_current(const list_iter_t *iter){
    if(iter->list->size == 0){
        return NULL;
    }
    return iter->curr->value;
}

bool list_iter_at_last(const list_iter_t *iter){
    if(iter->list->size == 0){
        return true;
    }
    if (iter->curr == iter->list->tail){
        return true;
    }
    return false;
}

bool list_iter_at_first(const list_iter_t *iter){
    if(iter->list->size == 0){
        return true;
    }
    if (iter->curr == iter->list->head){
        return true;
    }
    return false;
}

void list_iter_destroy(list_iter_t *iter){
    free(iter);
    return;
}

bool list_iter_insert_after(list_iter_t *iter, void *value){
    if (iter == NULL || iter->curr == NULL) {
        return false;
    }
    node_t *new_node = malloc(sizeof(node_t));

    new_node->value = value;
    new_node->next = iter->curr->next;
    new_node->prev = iter->curr;

    iter->curr->next = new_node;

    if (new_node->next != NULL) {
        new_node->next->prev = new_node;
    } else {
        iter->list->tail = new_node;
    }
    iter->list->size++;
    return true;
}

bool list_iter_insert_before(list_iter_t *iter, void *value){
    if (iter == NULL || iter->curr == NULL) {
        return false;
    }
    node_t *new_node = malloc(sizeof(node_t));

    new_node->value = value;
    new_node->next = iter->curr;
    new_node->prev = iter->curr->prev;

    iter->curr->prev = new_node;

    if (new_node->prev != NULL) {
        new_node->prev->next = new_node;
    } else {
        iter->list->head = new_node;
    }
    iter->list->size++;
    return true;
}

void *list_iter_delete(list_iter_t *iter){
    if(iter->list->size == 0){
        return NULL;
    }
    void *curr_value = iter->curr->value;
    node_t *iter_next = iter->curr->next;
    node_t *iter_prev = iter->curr->prev;
    if(iter->curr->next != NULL){
        free(iter->curr);
        iter->curr = iter_next;
    }else{
        free(iter->curr);
        iter->curr = iter_prev;
    }
    iter->list->size--;
    return curr_value;
}