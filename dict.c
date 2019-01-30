
// AUTHOR: Thuy Trang Nguyen
// DATE: December 17, 2018

/*
	BINARY TREE DICITONARY IMPLEMENTATION
	
	The dictionary consists of key-value pairs with key of string type and val of int type.
	It holds unique keys.
	Value -1 is reserved for returning value of failure to find a key in the dictionary.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>


// Struct for a pair holding a key and a value
typedef struct Pair{
	char* key;
	int value;
}Pair;

// Struct for a tree dictionary with a pointer to a pair as item and pointers to a left and right node
typedef struct BT_Dictionary{
	Pair* item;
	struct BT_Dictionary* left;
	struct BT_Dictionary* right;
}BT_Dictionary;

// "Constructor" for a key-value pair - returns a pointer to a pair.
Pair* newPair(char* k, int val) { 
    Pair* p =  (Pair*)malloc(sizeof(Pair)); 
    p->key = (char*)malloc(sizeof(k));
    strncat(p->key, k, strlen(k));
    p->value = val; 
    return p; 
} 

// "Constructor" for tree dictionary - returns a pointer to a dictionary.
BT_Dictionary* dict(){
	BT_Dictionary* d = (BT_Dictionary*)malloc(sizeof(BT_Dictionary)); 
	d->item = NULL;
	d->left = NULL;
	d->right = NULL;
	return d;
}


// Method for checking if dictionary is empty. On true it returns 1 and on false 0.
int dict_isEmpty(BT_Dictionary* d){
	if (d->item == NULL){
		return 1;	
	}else{
		return 0;	
	}
}

// Returns dictionary size
int dict_size(BT_Dictionary* d){

	if (d == NULL || d->item == NULL){
		return 0;
	}else{
		return 1 + dict_size(d->left) + dict_size(d->right);
	}
}

// If specified key is in dictionary, it returns the key's value, ow it returns -1
int dict_get(BT_Dictionary* d, char* key){
	if (d->item == NULL && d != NULL){
		return -1;
	}
	char* k = (d->item)->key;
	int v = (d->item)->value;
	int cmp = strcmp(key, k);
	if (cmp == 0){
		return v;
	}
	else if (cmp > 0){
		if (d->right != NULL){
			return dict_get(d->right, key);
		}else{
			return -1;
		}
	}
	else{
		if (d->left != NULL){
			return dict_get(d->left, key);	
		}else{
			return -1;
		}
	}
}

// Method for adding new pairs into the dictionary
void dict_put(BT_Dictionary* d, char* key, int val){

	// case when dictionary is empty
	if (d->item == NULL && d !=NULL){
		Pair* pair = newPair(key, val);	
		d->item = pair;
	}
	else{		
		char* k = (d->item)->key;
		int v = (d->item)->value;
		int cmp = strcmp(key, k);

		if (cmp==0){
			(d->item)->value = val;
		}
		else if (cmp > 0){
			if (d->right != NULL){
				dict_put(d->right, key, val);
			}else{
				d->right = dict();
				Pair* pair = newPair(key, val);	
				(d->right)->item = pair;
			}			
		}
		else{
			if (d->left != NULL){
				dict_put(d->left, key, val);
			}else{
				d->left = dict();
				Pair* pair = newPair(key, val);	
				(d->left)->item = pair;
			}	
		}	
	}
}

// print dictionary tree in order
void print_node(BT_Dictionary* dict){
	if  (dict != NULL){
		Pair* x = dict->item;
		printf("%s=%d, ", x->key, x->value);
		print_node(dict->left);
		print_node(dict->right);
	}
}
void dict_print(BT_Dictionary* dict){
	if (dict_isEmpty(dict)){
		printf("{}\n");
	}else{
		printf("{");
		print_node(dict);
		printf("}\n");
	}
}

// helper function for dict_free()
void free_helper(BT_Dictionary* parent, int s, BT_Dictionary* d){

	if (d->left == NULL && d->right == NULL){
		free((d->item)->key);
		free(d->item);
		free(d);
		if (s == 0){
			parent->right = NULL;
		}else{
			parent->left = NULL;
		}
	}else{
		if (d->left != NULL){
			free_helper(d, 1, d->left);
		}
		if (d->right != NULL){
			free_helper(d, 0, d->right);
		}
	}
}
// Method for freeing all allocated memory
void dict_free(BT_Dictionary* d){

	while (dict_isEmpty(d) != 1){
		// freeing the head (first item)
		if (d->left == NULL && d->right == NULL){
			Pair* to_free = d->item;
			d->item = NULL;
			free(to_free->key);
			free(to_free);
			free(d);
		}
		if (d->left != NULL){
			free_helper(d, 1, d->left);
		}
		if (d->right != NULL){
			free_helper(d, 0, d->right);
		}
	}

}

int main(int argc, char *argv[]) {

	BT_Dictionary* d = dict();
	
	FILE *fd = fopen("macbeth-processed.txt","r");
	if(fd == NULL){
		fclose(fd);
		exit(EXIT_FAILURE);
	}
	char *line = NULL;
	ssize_t read;
	size_t len = 0;
	long long total_elasped = 0;
	
	struct timeval start, end;

	while ((read=getline(&line, &len, fd)) != -1){
		char* token = strtok(line, " ");
		while(token != NULL){
			
			gettimeofday(&start, NULL);
			long long start_msec = start.tv_sec*1000LL + start.tv_usec/1000; // calculate milliseconds
			int val = dict_get(d, token);
			if (val == -1){
				dict_put(d, token, 1);
			}else{
				dict_put(d, token, val+1);
			}
			gettimeofday(&end, NULL);
			long long end_msec = end.tv_sec*1000LL + end.tv_usec/1000; // calculate milliseconds
			
			total_elasped += end_msec-start_msec;
			token = strtok(NULL, " ");
		}
	}
	fclose(fd);
	free(line);
	//dict_print(d);
	//printf("\n");
	printf("Time elasped of put in milliseconds: %lld\n", total_elasped);
	printf("Average time of put per argument in milliseconds %lld\n", (total_elasped)/dict_size(d));
	gettimeofday(&start, NULL);
	printf("dict_get(macbeth): %d\n", dict_get(d, "macbeth"));
	printf("dict_get(duncan): %d\n", dict_get(d, "duncan"));
	printf("dict_get(malcolm): %d\n", dict_get(d, "malcolm"));
	printf("dict_get(banquo): %d\n", dict_get(d, "banquo"));	
	printf("dict_get(war): %d\n", dict_get(d, "war"));
	printf("dict_get(treasonous): %d\n", dict_get(d, "treasonous"));
	gettimeofday(&end, NULL);		
	long long start_msec = start.tv_sec*1000LL + start.tv_usec/1000; // calculate milliseconds
	long long end_msec = end.tv_sec*1000LL + end.tv_usec/1000; // calculate milliseconds
	total_elasped = end_msec-start_msec;
	printf("Time elasped of get in milliseconds: %lld\n", total_elasped);
	printf("Average time of get per argument in milliseconds %lld\n", (total_elasped)/6);
	printf("Size of c dictionary (BST): %d\n", dict_size(d));
	dict_free(d);
	return 0;
}







