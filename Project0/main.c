#include "main.h"

int main() {
    
    while(1){
        char cmd[31];
        scanf("%s", cmd);
        
        if(cmd[0]=='b') bitmap_manage(cmd);
        
        else if(cmd[0]=='h') hash_manage(cmd);
        
        else if(cmd[0]=='l') list_manage(cmd);
        
        else if(cmd[0]=='q') break;
            
        else if(cmd[0]=='c'){
            char data[10];
            scanf("%s", data);
            
            if(data[0]=='b'){
                int size;
                char name[31];
                scanf("%s %d", name, &size);
                strcpy(bit_name[bit_idx], name);
                bit_table[bit_idx++]=bitmap_create(size);
            }
            else if(data[0]=='h'){
                char name[31];
                scanf("%s", name);
                strcpy(hash_name[hash_idx], name);
                hash_table[hash_idx]=(struct hash*)malloc(sizeof(struct hash));
                hash_init(hash_table[hash_idx++], hash_func, hash_less, NULL);
            }
            else if(data[0]=='l'){
                char name[31];
                scanf("%s", name);
                strcpy(list_name[list_idx], name);
                list_table[list_idx]=(struct list*)malloc(sizeof(struct list));
                list_init(list_table[list_idx++]);
            }
            else printf("error: invalid data type!\n");
        }
        else if (cmd[0]=='d' && cmd[1]=='e'){
            char name[31];
            int idx;
            scanf("%s", name);
            if(name[0]=='b') {
                idx=get_bit_idx(name);
                bitmap_destroy(bit_table[idx]);
                bit_table[idx]=NULL;
                strcpy(bit_name[idx], "");
            }
            else if(name[0]=='l'){
                idx=get_list_idx(name);
                while (!list_empty (list_table[idx])){
                    struct list_item *i = list_entry(list_front(list_table[idx]), struct list_item, elem);
                    list_pop_front (list_table[idx]);
                    free(i);
                }
                free(list_table[idx]);
                list_table[idx]=NULL;
                strcpy(list_name[idx], "");
            }
            else if(name[0]=='h'){
                idx=get_hash_idx(name);
                hash_destroy(hash_table[idx], hash_destructor);
                free(hash_table[idx]);
                hash_table[idx]=NULL;
                strcpy(hash_name[idx], "");
            }
        
        }
        else if (cmd[0]=='d' && cmd[1]=='u'){
            char name[31];
            int idx;
            size_t size;
            scanf("%s", name);
            
            if(name[0]=='b'){
                idx=get_bit_idx(name);
                size=bitmap_size(bit_table[idx]);
                for(size_t i=0; i<size; i++){
                    if (bitmap_test(bit_table[idx], i) == true)
                        printf("1");
                    else printf("0");
                }
                if(size>0) printf("\n");
            }
            else if(name[0]=='l'){
                idx=get_list_idx(name);
                size=list_size(list_table[idx]);
                struct list_elem *e=list_begin(list_table[idx]);
                struct list_item *item=list_entry(e, struct list_item, elem);
                for(size_t i=0; i<size; i++){
                    printf("%d ", item->data);
                    e=list_next(e);
                    item=list_entry(e, struct list_item, elem);
                }
                if(size>0) printf("\n");
            }
            else if(name[0]=='h'){
                idx=get_list_idx(name);
                size=hash_size(hash_table[idx]);
                struct hash_iterator* it = (struct hash_iterator*)malloc(sizeof(struct hash_iterator));
                hash_first(it, hash_table[idx]);
                
                for(size_t i=0; i<size; i++){
                    hash_next(it);
                    struct hash_elem *e=hash_cur(it);
                    struct hash_item *temp = hash_entry(e, struct hash_item, elem);
                    printf("%d ", temp->data);
                }
                if(size>0) printf("\n");
                free(it);
            }
        }
    
        else printf("error: invalid command! main\n");
        
    }

    return 0;
}

int get_bit_idx(char *name){
    int i;
    for(i=0; i<bit_idx; i++)
        if(!strncmp(bit_name[i], name, 30)) break;
    return i;
}
int get_hash_idx(char *name){
    int i;
    for(i=0; i<hash_idx; i++)
        if(!strncmp(hash_name[i], name, 30)) break;
    return i;
}
int get_list_idx(char *name){
    int i;
    for(i=0; i<list_idx; i++)
        if(!strncmp(list_name[i], name, 30)) break;
    return i;
}

void print_bool(bool val){
    if(val) printf("true\n");
    else printf("false\n");
}
bool get_bool(char *c){
    if(!strncmp(c, "true",4)) return true;
    else return false;
}

void bitmap_manage(char * cmd){
    int idx, size, start, cnt, index;
    
    if (!strncmp(cmd, "bitmap_mark", 11)){
        char name[31];
        scanf("%s %d", name, &size);
        idx=get_bit_idx(name);
        bitmap_mark(bit_table[idx], size);
    }
    else if (!strncmp(cmd, "bitmap_all", 10)){
        char name[31];
        scanf("%s %d %d", name, &start, &cnt);
        idx=get_bit_idx(name);
        print_bool(bitmap_all(bit_table[idx], start, cnt));
    }
    else if (!strncmp(cmd, "bitmap_any", 10)){
        char name[31];
        scanf("%s %d %d", name, &start, &cnt);
        idx=get_bit_idx(name);
        print_bool(bitmap_any(bit_table[idx], start, cnt));
    }
    else if (!strncmp(cmd, "bitmap_contains", 15)){
        char name[31], val[6];
        scanf("%s %d %d %s", name, &start, &cnt, val);
        idx=get_bit_idx(name);
        print_bool(bitmap_contains(bit_table[idx], start, cnt, get_bool(val)));
    }
    else if (!strncmp(cmd, "bitmap_count", 12)){
        char name[31], val[6];
        scanf("%s %d %d %s", name, &start, &cnt, val);
        idx=get_bit_idx(name);
        printf("%zu\n", bitmap_count(bit_table[idx], start, cnt, get_bool(val)));
    }
    else if (!strncmp(cmd, "bitmap_dump", 11)){
        char name[31];
        scanf("%s", name);
        idx=get_bit_idx(name);
        bitmap_dump(bit_table[idx]);
    }
    
    else if (!strncmp(cmd, "bitmap_expand", 13)){
        char name[31];
        scanf("%s %d", name, &size);
        idx=get_bit_idx(name);
        bit_table[idx]=bitmap_expand(bit_table[idx], size);
    }
    
    else if (!strncmp(cmd, "bitmap_set_all", 14)){
        char name[31], val[6];
        scanf("%s %s", name, val);
        idx=get_bit_idx(name);;
        bitmap_set_all(bit_table[idx], get_bool(val));
    }
    else if (!strncmp(cmd, "bitmap_flip", 11)){
        char name[31];
        scanf("%s %d", name, &index);
        idx=get_bit_idx(name);
        bitmap_flip(bit_table[idx], index);
    }
    else if (!strncmp(cmd, "bitmap_none", 11)){
        char name[31];
        scanf("%s %d %d", name, &start, &cnt);
        idx=get_bit_idx(name);
        print_bool(bitmap_none(bit_table[idx], start, cnt));
    }
    else if (!strncmp(cmd, "bitmap_reset", 12)){
        char name[31];
        scanf("%s %d", name, &index);
        idx=get_bit_idx(name);
        bitmap_reset(bit_table[idx], index);
    }
    else if (!strncmp(cmd, "bitmap_scan_and_flip", 20)){
        char name[31], val[6];
        scanf("%s %d %d %s", name, &start, &cnt, val);
        idx=get_bit_idx(name);
        printf("%zu\n", bitmap_scan_and_flip(bit_table[idx], start, cnt, get_bool(val)));
    }
    else if (!strncmp(cmd, "bitmap_scan", 11)){
        char name[31], val[6];
        scanf("%s %d %d %s", name, &start, &cnt, val);
        idx=get_bit_idx(name);
        printf("%zu\n", bitmap_scan(bit_table[idx], start, cnt, get_bool(val)));
    }
    else if (!strncmp(cmd, "bitmap_set_multiple", 19)){
        char name[31], val[6];
        scanf("%s %d %d %s", name, &start, &cnt, val);
        idx=get_bit_idx(name);
        bitmap_set_multiple(bit_table[idx], start, cnt, get_bool(val));
    }
    else if (!strncmp(cmd, "bitmap_set", 10)){
        char name[31], val[6];
        scanf("%s %d %s", name, &index, val);
        idx=get_bit_idx(name);;
        bitmap_set(bit_table[idx], index, get_bool(val));
    }
    else if (!strncmp(cmd, "bitmap_size", 11)){
        char name[31];
        scanf("%s", name);
        idx=get_bit_idx(name);
        printf("%zu\n", bitmap_size(bit_table[idx]));
    }
    else if (!strncmp(cmd, "bitmap_test", 11)){
        char name[31];
        scanf("%s %d", name, &index);
        idx=get_bit_idx(name);
        print_bool(bitmap_test(bit_table[idx], index));
    }
    else printf("error : invalid command! bitmap\n");
}

void hash_manage(char *cmd){
    
    int idx;
    
    if (!strncmp(cmd, "hash_insert", 11)){
        char name[31];
        int obj;
        scanf("%s %d", name, &obj);
        idx=get_hash_idx(name);
        struct  hash_item* i = (struct hash_item*)malloc(sizeof(struct hash_item));
        i->data = obj;
        hash_insert(hash_table[idx], &i->elem);
    }
    else if (!strncmp(cmd, "hash_apply", 10)){
        char name[31], func[10];
        scanf("%s %s", name, func);
        idx=get_hash_idx(name);
        
        if(!strncmp(func, "square", 6))
            hash_apply(hash_table[idx], hash_square);
        else if (!strncmp(func, "triple", 6))
            hash_apply(hash_table[idx], hash_triple);
    }
    else if (!strncmp(cmd, "hash_delete", 11)){
        char name[31];
        int obj;
        scanf("%s %d", name, &obj);
        idx=get_hash_idx(name);
        struct  hash_item* i = (struct hash_item*)malloc(sizeof(struct hash_item));
        i->data = obj;
        hash_delete(hash_table[idx], &i->elem);
    }
    else if (!strncmp(cmd, "hash_empty", 10)){
        char name[31];
        scanf("%s", name);
        idx=get_hash_idx(name);
        print_bool(hash_empty(hash_table[idx]));
    }
    else if (!strncmp(cmd, "hash_size", 9)){
        char name[31];
        scanf("%s", name);
        idx=get_hash_idx(name);
        size_t size=hash_size(hash_table[idx]);
        printf("%zu\n", size);
    }
    else if (!strncmp(cmd, "hash_clear", 10)){
        char name[31];
        scanf("%s", name);
        idx=get_hash_idx(name);
        hash_clear(hash_table[idx], NULL);
    }
    else if (!strncmp(cmd, "hash_find", 9)){
        char name[31];
        int obj;
        scanf("%s %d", name, &obj);
        idx=get_hash_idx(name);
        struct  hash_item* i = (struct hash_item*)malloc(sizeof(struct hash_item));
        i->data = obj;
        if (hash_find(hash_table[idx], &i->elem))
            printf("%d\n", obj);
    }
    else if (!strncmp(cmd, "hash_replace", 12)){
        char name[31];
        int obj;
        scanf("%s %d", name, &obj);
        idx=get_hash_idx(name);
        struct  hash_item* i = (struct hash_item*)malloc(sizeof(struct hash_item));
        i->data = obj;
        hash_replace(hash_table[idx], &i->elem);
    }
    else printf("error: invalid command!hash\n");
}

void list_manage(char *cmd){
    int idx, idx2;
    void *aux=NULL;
    
    if (!strncmp(cmd, "list_push_back", 14)){
        char name[31];
        int data;
        
        scanf("%s %d", name, &data);
        idx=get_list_idx(name);
        struct list_item *item=(struct list_item*)malloc(sizeof(struct list_item));
        item->data=data;
        list_push_back(list_table[idx], &(item->elem));
    }
    else if (!strncmp(cmd, "list_push_front", 15)){
        char name[31];
        int data;
        
        scanf("%s %d", name, &data);
        idx=get_list_idx(name);
        struct list_item *item=(struct list_item*)malloc(sizeof(struct list_item));
        item->data=data;
        list_push_front(list_table[idx], &(item->elem));
    }
    else if (!strncmp(cmd, "list_insert_ordered", 19)){
        char name[31];
        int d;
        
        scanf("%s %d", name, &d);
        idx=get_list_idx(name);
        struct list_item *item=(struct list_item*)malloc(sizeof(struct list_item));
        item->data=d;
        list_insert_ordered(list_table[idx], &(item->elem), list_less, NULL);
    }
    else if (!strncmp(cmd, "list_insert", 11)){
        char name[31];
        int data, before;
        
        struct list_elem *b;
        scanf("%s %d %d", name, &before, &data);
        idx=get_list_idx(name);
        struct list_item *item=(struct list_item*)malloc(sizeof(struct list_item));
        item->data=data;
        b=get_elem(list_table[idx], before);
        list_insert(b, &(item->elem));
    }
    else if (!strncmp(cmd, "list_front", 10)){
        char name[31];
        struct list_elem *e;
        struct list_item *i;
        
        scanf("%s", name);
        idx=get_list_idx(name);
        e=list_front(list_table[idx]);
        i=list_entry(e, struct list_item, elem);
        printf("%d\n", i->data);
    }
    else if (!strncmp(cmd, "list_back", 9)){
        char name[31];
        struct list_elem *e;
        struct list_item *i;
        
        scanf("%s", name);
        idx=get_list_idx(name);
        e=list_back(list_table[idx]);
        i=list_entry(e, struct list_item, elem);
        printf("%d\n", i->data);
    }
    else if (!strncmp(cmd, "list_pop_front", 14)){
        char name[31];
        
        scanf("%s", name);
        idx=get_list_idx(name);
        list_pop_front(list_table[idx]);
    }
    else if (!strncmp(cmd, "list_pop_back", 13)){
        char name[31];
        
        scanf("%s", name);
        idx=get_list_idx(name);
        list_pop_back(list_table[idx]);
    }
    
    else if (!strncmp(cmd, "list_empty", 10)){
        char name[31];
        
        scanf("%s", name);
        idx=get_list_idx(name);
        print_bool(list_empty(list_table[idx]));
    }
    else if (!strncmp(cmd, "list_size", 9)){
        char name[31];
        
        scanf("%s", name);
        idx=get_list_idx(name);
        size_t size=list_size(list_table[idx]);
        printf("%zu\n", size);
    }
    else if (!strncmp(cmd, "list_max", 8)){
        char name[31];
        struct list_elem *e;
        struct list_item *i;
        
        scanf("%s", name);
        idx=get_list_idx(name);
        e=list_max(list_table[idx], list_less, aux);
        i=list_entry(e, struct list_item, elem);
        printf("%d\n", i->data);
    }
    else if (!strncmp(cmd, "list_min", 8)){
        char name[31];
        struct list_elem *e;
        struct list_item *i;
        
        scanf("%s", name);
        idx=get_list_idx(name);
        e=list_min(list_table[idx], list_less, aux);
        i=list_entry(e, struct list_item, elem);
        printf("%d\n", i->data);
    }
    else if (!strncmp(cmd, "list_remove", 11)){
        char name[31];
        int before;
        struct list_elem *b;
        
        scanf("%s %d", name, &before);
        idx=get_list_idx(name);
        b=get_elem(list_table[idx], before);
        list_remove(b);
    }
    else if (!strncmp(cmd, "list_reverse", 12)){
        char name[31];
        
        scanf("%s", name);
        idx=get_list_idx(name);
        list_reverse(list_table[idx]);
    }
    else if (!strncmp(cmd, "list_shuffle", 12)){
        char name[31];
        
        scanf("%s", name);
        idx=get_list_idx(name);
        list_shuffle(list_table[idx]);
    }
    else if (!strncmp(cmd, "list_sort", 9)){
        char name[31];
        
        scanf("%s", name);
        idx=get_list_idx(name);
        list_sort(list_table[idx], list_less, aux);
    }
    else if (!strncmp(cmd, "list_splice", 11)){
        char name[31], name2[31];
        int before, first, last;
        struct list_elem *b, *f, *l;
        
        scanf("%s %d %s %d %d", name, &before, name2, &first, &last);
        idx=get_list_idx(name); idx2=get_list_idx(name2);
        b=get_elem(list_table[idx], before);
        f=get_elem(list_table[idx2], first);
        l=get_elem(list_table[idx2], last);
        list_splice(b, f, l);
    }
    else if (!strcmp(cmd, "list_swap")){
        char name[31];
        int a, b;
        scanf("%s %d %d", name, &a, &b);
        idx=get_list_idx(name);
        list_swap(get_elem(list_table[idx], a), get_elem(list_table[idx], b));
    }
    else if (!strncmp(cmd, "list_unique", 11)){
        char names[62];
        int arg_num=0;
        char name[2][31], *token;

        gets(names);
        token=strtok(names, " ");
        while(token!=NULL){
            strcpy(name[arg_num++], token);
            token = strtok(NULL, " ");
        }
        
        if(arg_num==1){
            idx=get_list_idx(name[0]);
            list_unique(list_table[idx], NULL, list_less, aux);
        }
        else{
            idx=get_list_idx(name[0]); idx2=get_list_idx(name[1]);
            list_unique(list_table[idx], list_table[idx2], list_less, aux);
        }
    }
    else printf("error: invalid command! list\n");
}


bool list_less(const struct list_elem *a, const struct list_elem *b, void *aux){

    int a_data, b_data;
    struct list_item *temp;
    
    temp=list_entry(a,struct list_item, elem);
    a_data=temp->data;
    temp=list_entry(b,struct list_item, elem);
    b_data=temp->data;
    
    if(a_data<b_data) return true;
    else return false;
}

unsigned hash_func(const struct hash_elem* e, void* aux) {
    return hash_int(hash_entry(e, struct hash_item, elem)->data);
}

bool hash_less(const struct hash_elem* a, const struct hash_elem* b, void* aux) {
    int a_data, b_data;
    struct hash_item *temp;
    
    temp=hash_entry(a,struct hash_item, elem);
    a_data=temp->data;
    temp=hash_entry(b,struct hash_item, elem);
    b_data=temp->data;
    
    if(a_data<b_data) return true;
    else return false;
}


void hash_square(struct hash_elem *e, void *aux){
    int d = hash_entry(e, struct hash_item, elem)->data;
    
    hash_entry(e, struct hash_item, elem)->data = d * d;
}

void hash_triple(struct hash_elem* e, void* aux) {
    int d = hash_entry(e, struct hash_item, elem)->data;
    
    hash_entry(e, struct hash_item, elem)->data = d * d * d;
}

void hash_destructor(struct hash_elem* e, void* aux) {
	struct hash_item *temp=hash_entry(e, struct hash_item, elem);

	free(temp);
}

struct list_elem *get_elem (struct list* list, int num){
    struct list_elem *b= list_begin(list);
    for(int i=0; i<num; i++)
        b=list_next(b);
    return b;
}




