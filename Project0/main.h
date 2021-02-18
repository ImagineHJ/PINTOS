#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"
#include "hash.h"
#include "list.h"

struct bitmap *bit_table[10];
struct hash *hash_table[10];
struct list *list_table[10];

/* */
int bit_idx=0, hash_idx=0, list_idx=0;
char bit_name[10][31];
char hash_name[10][31];
char list_name[10][31];


int get_bit_idx(char *);
int get_list_idx(char *);
int get_hash_idx(char *);

void print_bool(bool );
bool get_bool(char *);

void bitmap_manage(char *);
void hash_manage(char *);
void list_manage(char *);

bool list_less(const struct list_elem *a, const struct list_elem *b, void *aux);
struct list_elem *get_elem (struct list*, int num);

unsigned hash_func(const struct hash_elem* e, void* aux);
bool hash_less(const struct hash_elem* a, const struct hash_elem* b, void* aux);
void hash_square(struct hash_elem *e, void *aux);
void hash_triple(struct hash_elem *e, void *aux);
void hash_destructor(struct hash_elem *e, void *aux);




