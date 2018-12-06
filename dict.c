
// AUTHOR: Thuy Trang Nguyen
// DATE: December 17, 2018

/*
	The dictionary consists of key-value pairs with key of string type and val of int type.
	It holds unique keys.
	Value -1 is reserved for returning value of failure to find a key in the dictionary.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Struct for a pair holding a key and a value
typedef struct Pair{
	char* key;
	int value;
}Pair;

// Struct for a dictionary with a pointer to a pair as first and pointer to a dictionary as next
typedef struct Dictionary{
	Pair* first;
	struct Dictionary* next;
}Dictionary;

// "Constructor" for a key-value pair - returns a pointer to a pair.
Pair* newPair(char* k, int val) { 
    Pair* p =  (Pair*)malloc(sizeof(Pair)); 
    p->key = k;
    p->value = val; 
    return p; 
} 

// "Constructor" for dictionary - returns a pointer to a dictionary.
Dictionary* dict(){
	Dictionary* d = (Dictionary*)malloc(sizeof(Dictionary)); 
	d->first = NULL;
	d->next = NULL;
	return d;
}

// Method for checking if dictionary is empty. On true it returns 1 and on false 0.
int dict_isEmpty(Dictionary* d){
	if (d->first == NULL){
		return 1;	
	}else{
		return 0;	
	}
}

// Method returning the size of a dictionary.
int dict_size(Dictionary* d){
	int count = 0;
	Dictionary* h = d;
	while (h != NULL){
		count++;
		h = h->next;
	}
	return count;
}

// Method returning the value of the given key if that key exists in the dictionary.
// If the key is not found then -1 is returned.
int dict_get(Dictionary* d, char* key){
 	if(d->first == NULL){
 		return -1;
 	} 	
 	Dictionary* h = d;
	while (h != NULL){
		if (strcmp((h->first)->key, key) == 0){
			return (h->first)->value;
		}
		h = h->next;
	}
	return -1; 
}

// This is a helper function for the put method -> when a key already exists in the dictionary,
// if a user tries to call put with a same key and some value, then the dictionary updates the pair's old value with the current one
void updateVal(Dictionary* d, char* key, int val){
 	Dictionary* h = d;
	while (h != NULL){
		if (strcmp((h->first)->key, key) == 0){
			(h->first)->value = val;
		}
		h = h->next;
	}
}

// Method for adding new pairs into the dictionary
void dict_put(Dictionary* d, char* key, int val){
	if (val == -1){
		printf("-1 value reserved, please use another one\n");
		return;
	}
	if (key == NULL || val == NULL){
		printf("You cannot use null keyes and values\n");
		return;
	}
	int k = dict_get(d,key);
	if (k<0){
		Pair* pair = newPair(key, val);
		if (d->first == NULL){
			d->first = pair;
		}
		else if(d->next == NULL){
			Dictionary* new = dict();
			new->first = pair;
			new->next = NULL;
			d->next = new;

		}else{
			dict_put(d->next, key, val);
		}
	}else{
		updateVal(d, key, val);
	}
}

// This method removes a pair specified by the key from the dictionary
void dict_remove(Dictionary* d, char* key){

	int deleted = 0;
	Dictionary* h = d;
	Dictionary* prev = d;
	while (h != NULL){
		if (strcmp((h->first)->key, key) == 0){
			Pair* rmv = h->first;   
			prev->next = h->next;
			free(rmv);
			free(h);
			deleted = 1;
			h=NULL;
		}
		else{
			prev = h;
			h = h->next;
		}
	}
	if (deleted == 0){printf("Key not in dictionary\n");}

}

// This is a helper method for printing the contents of a dictionary.
// It prints out one pair specified by x.
void printPair(Pair* x){
	char* p_str = (char*)malloc(sizeof(x)+2);
	//printf("size size: %d\n", sizeof(x)+2);
	memset(p_str, '\0', sizeof(x)+5);
	strncat(p_str, x->key, strlen(x->key));
	strncat(p_str, ":",1);
	char b[(sizeof(int)*8+1)];
	sprintf(b, "%d", x->value);
	strcat(p_str,b);
	printf("%s",p_str);
	free(p_str); 
}

// Mehtod for printing out the contents of a dictionary. 
// Format: {"key1":val1, "key2":val2 ...}
void dict_print(Dictionary* d){
	
	Dictionary* h = d;
	if(h->first==NULL){
		printf("{}\n");
		return;
	}
	printf("{");
	while (h != NULL){
		printPair(h->first);
		if (h->next != NULL){
			printf(" ");
		}		
		h = (h->next);

	}
	printf("}");
	printf("\n");
}

// Method for freeing all malloced memory from the dictionary.
// At the end makes the dictionary empty.
void dict_free(Dictionary* d){

	Dictionary* h = d;
	Dictionary* prev = NULL;
	while (h->next != NULL){
		prev = h;
		h = h->next;
	}
	free(h->first);
	free(h);
	if(prev != NULL){
		prev->next = NULL;
		dict_free(d);
	}else{
		d->first = NULL;
		d->next = NULL;
	}
}

int main(int argc, char *argv[]) {
	Dictionary* d = dict();
	printf("Is it empty?: %d\n", dict_isEmpty(d));
	dict_put(d, "super", 1);
	dict_put(d, "tired", 2);
	dict_put(d, "of", 3);
	dict_put(d, "cs",4);
	dict_put(d, "...",70000);
	dict_print(d);
	printf("Size of dict: %d\n", dict_size(d));
	printf("get(...): %d\n", dict_get(d, "..."));
	printf("get(fqf): %d\n", dict_get(d, "fqf"));
	dict_put(d,"super",20);
	dict_print(d);
	dict_remove(d, "cs");
	dict_print(d);
	//dict_put(d, "u", NULL);
	dict_free(d);
	dict_print(d);
}








