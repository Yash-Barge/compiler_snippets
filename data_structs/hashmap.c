#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "hashmap.h"


struct head {
    struct node* first_node;
};

struct node {
    int val;
    struct node* next;
};

struct hashmap {
    struct head** map;
};


struct node* new_node_hash(int val){
    struct node* n = malloc(sizeof(struct node));
    n->val = val;
    n->next = NULL;
    return n;
}

struct head* new_head_hash(){
    struct head* h = malloc(sizeof(struct head));
    h->first_node = NULL;
    return h;
}

void ll_insert(struct head* h, int val){
    assert(h != NULL);
    struct node* n = new_node_hash(val);
    // if(h->next == NULL){
    //     h->next = n;
    //     return;
    // }
    n->next = h->first_node;
    h->first_node = n;
    return;
}

void ll_free(struct head* h){
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
        free(prev);
    }
    free(h);
}

int hash_function(int key){
    return (1ll * key * 23) % 11;
}

struct hashmap* hashmap_new(){
    struct hashmap* hmap = malloc(sizeof(struct hashmap));
    int hashmap_size = 11; // CHANGE IF HASH FUNCTION CHANGES
    for(int i=0; i<hashmap_size; i++){
        hmap->map[i] = new_head_hash();
    }
    return hmap;
}

void hashmap_insert(struct hashmap* hm, int key){
    assert(hm != NULL);
    int index = hash_function(key);
    ll_insert(hm->map[index], key);
    return;
}

int hashmap_search(struct hashmap* hm, int key){
    assert(hm != NULL);
    int index = hash_function(key);
    if(hm->map[index]->first_node == NULL) return 0;
    struct node* temp = hm->map[index]->first_node;
    while(temp != NULL){
        if(temp->val == key) return 1;
        temp = temp->next;
    }
    return 0;
}

void hashmap_erase(struct hashmap* hm, int key){
    assert(hm != NULL);
    if(!hashmap_search(hm, key)) return;
    
    int index = hash_function(key);
    if(hm->map[index]->first_node->val == key){
        struct node* temp = hm->map[index]->first_node;
        hm->map[index]->first_node = temp->next;
        temp->next = NULL;
        free(temp);
        return;
    }
    struct node* temp = hm->map[index]->first_node->next;
    struct node* prev = hm->map[index]->first_node;
    
    while(temp->val != key){
        prev = temp;
        temp = temp->next;
    }

    prev->next = temp->next;
    temp->next = NULL;
    free(temp);
    return;
}

void hashmap_free(struct hashmap* hm){
    int hashmap_size = 11; // CHANGE IF HASH FUNCTION CHANGES   
    for(int i=0; i<hashmap_size; i++){
        ll_free(hm->map[i]);
    }
    free(hm);
    return;
}

const struct hashmap_lib Hashmap = {.new = hashmap_new, .insert = hashmap_insert, .search = hashmap_search, .erase = hashmap_erase, .free  = hashmap_free};
