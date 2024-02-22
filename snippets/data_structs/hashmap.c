#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "hashmap.h"

struct head {
    struct node* first_node;
};

struct node {
    char* val;
    struct node* next;
};

struct hashmap {
    struct head** map;
    int size;
    int capacity;
};

struct hashmap* hashmap_new(int size);
void hashmap_insert(struct hashmap** hm, char* key);
int hashmap_search(struct hashmap* hm, char* key);
void hashmap_erase(struct hashmap* hm, char* key);
void hashmap_free(struct hashmap **p_hm);

struct node* new_node_hash(char* val){
    struct node* n = malloc(sizeof(struct node));
    n->val = malloc((strlen(val) + 1) * sizeof(char));
    strcpy(n->val, val);
    n->next = NULL;
    return n;
}

struct head* new_head_hash(){
    struct head* h = malloc(sizeof(struct head));
    h->first_node = NULL;
    return h;
}

void ll_insert_hm(struct head* h, char* val){
    assert(h != NULL);
    struct node* n = new_node_hash(val);
    n->next = h->first_node;
    h->first_node = n;
    return;
}

void ll_free_hm(struct head* h){
    assert(h != NULL);
    if(h->first_node == NULL) {
        free(h);
        return;
    }
    struct node* temp = h->first_node;
    struct node* prev = h->first_node;
    while(temp != NULL){
        prev = temp;
        temp = temp->next;
        free(prev->val);
        free(prev);
    }
    free(h);
}


int hash_function_hm(char* key, int size){
    return (1LL * strlen(key) * 23) % size;
}

struct hashmap* hashmap_new(int size){
    struct hashmap* hmap = malloc(sizeof(struct hashmap));
    hmap->map = malloc(sizeof(struct head*) * size);
    for(int i=0; i<size; i++){
        hmap->map[i] = new_head_hash();
    }
    hmap->size = size;
    return hmap;
}

void hashmap_insert(struct hashmap** p_hm, char* key){
    assert(p_hm != NULL);
    assert(*p_hm != NULL);

    struct hashmap *hm = *p_hm;

    if (hashmap_search(hm, key))
        return;

    int index = hash_function_hm(key, hm->size);
    ll_insert_hm(hm->map[index], key);
    hm->capacity++;
    if(hm->capacity == hm->size){
        struct hashmap* new_hm = hashmap_new(hm->size * 2);
        for(int i=0; i<hm->size; i++){
            struct node* temp = hm->map[i]->first_node;
            while(temp != NULL) {
                hashmap_insert(&new_hm, temp->val);
                temp = temp->next;
            }
        }
        hashmap_free(p_hm);
        *p_hm = new_hm;
    }
    return;
}

int hashmap_search(struct hashmap* hm, char* key){
    assert(hm != NULL);
    int index = hash_function_hm(key, hm->size);
    if(hm->map[index]->first_node == NULL) return 0;
    struct node* temp = hm->map[index]->first_node;
    while(temp != NULL){
        if(!strcmp(key, temp->val)) return 1;
        temp = temp->next;
    }
    return 0;
}

void hashmap_erase(struct hashmap* hm, char* key){
    assert(hm != NULL);
    if(!hashmap_search(hm, key)) return;
    
    int index = hash_function_hm(key, hm->size);
    if(!strcmp(hm->map[index]->first_node->val, key)){
        struct node* temp = hm->map[index]->first_node;
        hm->map[index]->first_node = temp->next;
        temp->next = NULL;
        free(temp->val);
        free(temp);
        return;
    }
    struct node* temp = hm->map[index]->first_node->next;
    struct node* prev = hm->map[index]->first_node;
    
    while(strcmp(temp->val, key) && temp != NULL){
        prev = temp;
        temp = temp->next;
    }

    if(temp == NULL) return;

    prev->next = temp->next;
    temp->next = NULL;
    free(temp->val);
    free(temp);
    return;
}

void hashmap_free(struct hashmap **p_hm){
    assert(p_hm != NULL);
    assert(*p_hm != NULL);

    struct hashmap *hm = *p_hm;

    int hashmap_size = hm->size;
    for(int i=0; i<hashmap_size; i++){
        ll_free_hm(hm->map[i]);
    }
    free(hm->map);
    free(hm);

    *p_hm = NULL;

    return;
}

const struct hashmap_lib Hashmap = {.new = hashmap_new, .insert = hashmap_insert, .search = hashmap_search, .erase = hashmap_erase, .free  = hashmap_free};