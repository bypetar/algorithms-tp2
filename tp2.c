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
    if (!lista){
        return NULL;
    }

    lista->head = NULL;
    lista->tail = NULL;
    lista->size = 0;

    return lista;
}

size_t list_length(const list_t *list) {
    if (!list){
        return 0;
    }
    return list->size;
}

bool list_is_empty(const list_t *list) {
    if (!list){
        return true;
    }
    return list->size == 0;
}

bool list_insert_head(list_t *list, void *value) {
    if (!list){
        return false;
    }

    node_t *nuevo = malloc(sizeof(node_t));
    if (!nuevo){
        return false;
    }

    nuevo->value = value;
    nuevo->next = list->head;
    nuevo->prev = NULL;
    
    if (list->head) {
        list->head->prev = nuevo;
    }

    if (list->size == 0) {
        list->tail = nuevo;  
    }

    list->head = nuevo;
    list->size++;
    return true;
}

bool list_insert_tail(list_t *list, void *value) {
    if (!list){
        return false;
    }

    node_t *new_tail = malloc(sizeof(node_t));

    if (!new_tail){
        return false;
    }

    new_tail->value = value;
    new_tail->next = NULL;
    new_tail->prev = list->tail;

    if (list->tail) {
        list->tail->next = new_tail;
    } else {
        list->head = new_tail;
    }

    list->tail = new_tail;
    list->size++;
    return true;
}

void *list_peek_head(const list_t *list) {
    if (!list || !list->head){
        return NULL;
    }
    return list->head->value;
}

void *list_peek_tail(const list_t *list) {
    if (!list || !list->tail){
        return NULL;
    }
    return list->tail->value;
}

void *list_pop_head(list_t *list) {
    if (!list || !list->head){
        return NULL;
    }

    void *head_value = list->head->value;
    node_t *old_head = list->head;

    list->head = old_head->next;
    if (list->head) {
        list->head->prev = NULL;
    } else {
        list->tail = NULL;
    }

    free(old_head);
    list->size--;
    return head_value;
}

void *list_pop_tail(list_t *list) {
    if (!list || list->size == 0){
        return NULL;
    }

    void *tail_value = list->tail->value;

    if (list->size == 1) {
        free(list->tail);
        list->head = NULL;
        list->tail = NULL;
    } else {
        node_t *new_tail = list->tail->prev;
        free(list->tail);
        list->tail = new_tail;
        new_tail->next = NULL;
    }

    list->size--;
    return tail_value;
}

void list_destroy(list_t *list, void destroy_value(void *)) {
    if (!list){
        return;
    }

    node_t *actual = list->head;
    while (actual) {
        node_t *siguiente = actual->next;
        if (destroy_value) {
            destroy_value(actual->value);
        }
        free(actual);
        actual = siguiente;
    }

    free(list);

}

list_iter_t *list_iter_create_head(list_t *list) {
    if (!list){
        return NULL;
    }
    
    list_iter_t *iter = malloc(sizeof(list_iter_t));
    if (!iter){
        return NULL;
    }

    iter->list = list;
    iter->curr = list->head;

    return iter;
}

list_iter_t *list_iter_create_tail(list_t *list) {
    if (!list){
        return NULL;
    }
    list_iter_t *iter = malloc(sizeof(list_iter_t));
    if (!iter){
        return NULL;
    }

    iter->list = list;
    iter->curr = list->tail;  

    return iter;
}

bool list_iter_forward(list_iter_t *iter) {
    if (!iter || !iter->list || !iter->curr){
        return false;
    }

    if (iter->curr->next) {
        iter->curr = iter->curr->next;
        return true;
    }

    return false;
}

bool list_iter_backward(list_iter_t *iter) {
    if (!iter || !iter->list || !iter->curr){
        return false;
    }

    if (iter->curr->prev) {
        iter->curr = iter->curr->prev;
        return true;
    }

    return false;
}

void *list_iter_peek_current(const list_iter_t *iter) {
    if (!iter || !iter->curr){
        return NULL;
    }
    return iter->curr->value;
}

bool list_iter_at_last(const list_iter_t *iter) {
    if (!iter || !iter->list){
        return true;
    }
    if (!iter->curr){
        return false;
    }

    return iter->curr == iter->list->tail;
}

bool list_iter_at_first(const list_iter_t *iter) {
    if (!iter || !iter->list){
        return true;
    }
    if (list_is_empty(iter->list)){
        return true;
    }
    return iter->curr == iter->list->head;
}

void list_iter_destroy(list_iter_t *iter){
    free(iter);
}

bool list_iter_insert_after(list_iter_t *iter, void *value) {
    if (!iter || !iter->list){
        return false;
    }

    node_t *new_node = malloc(sizeof(node_t));
    if (!new_node){
        return false;
    }

    new_node->value = value;
    new_node->next = NULL;
    new_node->prev = NULL;

    if (iter->list->size == 0) {
        iter->list->head = new_node;
        iter->list->tail = new_node;
        iter->curr = new_node;
    }
    else if (iter->curr != NULL) {
        new_node->next = iter->curr->next;
        new_node->prev = iter->curr;

        if (iter->curr->next) {
            iter->curr->next->prev = new_node;
        } else {
            iter->list->tail = new_node;
        }

        iter->curr->next = new_node;
    }
    else {
        free(new_node);
        return false;
    }

    iter->list->size++;
    return true;
}

bool list_iter_insert_before(list_iter_t *iter, void *value) {
    if (!iter || !iter->list){
        return false;
    } 

    node_t *new_node = malloc(sizeof(node_t));
    if (!new_node){
        return false;
    } 

    new_node->value = value;

    if (iter->list->size == 0) {
        new_node->next = NULL;
        new_node->prev = NULL;
        iter->list->head = new_node;
        iter->list->tail = new_node;
        iter->curr = new_node;
    }
    else if (iter->curr) {
        new_node->next = iter->curr;
        new_node->prev = iter->curr->prev;
        iter->curr->prev = new_node;
        
        if (new_node->prev) {
            new_node->prev->next = new_node;
        } else {
            iter->list->head = new_node;
        }
    }
    else {
        free(new_node);
        return false;
    }

    iter->list->size++;
    return true;
}

void *list_iter_delete(list_iter_t *iter) {
    if (!iter || !iter->list || !iter->curr){
        return NULL;
    } 

    void *curr_value = iter->curr->value;
    node_t *to_delete = iter->curr;

    if (to_delete->prev) {
        to_delete->prev->next = to_delete->next;
    } else {
        iter->list->head = to_delete->next;
    }

    if (to_delete->next) {
        to_delete->next->prev = to_delete->prev;
        iter->curr = to_delete->next;
    } else {
        iter->list->tail = to_delete->prev;
        iter->curr = to_delete->prev;
    }

    free(to_delete);
    iter->list->size--;

    return curr_value;
}