#include "contactsT.h"
#include <string.h>
#include <stdlib.h>  
#include <stdio.h>

struct ContactsT* createBookSortedByT(const char* value){
	ContactsT* contact = (ContactsT*)malloc(sizeof(struct ContactsT));
	contact->left = NULL;
	contact->right = NULL;
	contact->somebody = NULL;
	contact->parent = NULL;
	if(!strcmp(value,"surname")){
		contact->comparator = &compareSurname;
	}
	else if(!strcmp(value,"birth")){
		contact->comparator = &compareBirth;
	}
	else if(!strcmp(value,"email")){
		contact->comparator = &compareEmail;
	}
	else if(!strcmp(value,"phone")){
		contact->comparator = &comparePhone;
	}
	else {
		free(contact);
		return NULL;
	}
	return contact;
}

void deleteContactsT(struct ContactsT* book){
	if(book->left!=NULL)deleteContactsT(book->left);
	if(book->right!=NULL)deleteContactsT(book->right);
	freePerson(book->somebody);
	free(book);
}


void addPersonT(struct Person* somebody,struct ContactsT* book){
	if (book->somebody == NULL){
		book->somebody = somebody;
	}
	else{
		ContactsT* contact = (ContactsT*)malloc(sizeof(struct ContactsT));
		contact->somebody = somebody;
		contact->left = NULL;
		contact->right = NULL;
		contact->comparator = book->comparator;
		contact->parent = NULL;
		while(1){
			if ((*(book->comparator))(book->somebody,somebody) > 0){
				if (book->right == NULL){
					book->right = contact;
					contact->parent = book;
					return;
				}
				book = book->right;
			}
			else{
				if (book->left == NULL){
					book->left = contact;
					contact->parent = book;
					return;
				}
				book = book->left;
			}
		}
	
	}
}

struct ContactsT* deletePersonT(struct Person* somebody,struct ContactsT* book){
	ContactsT* head = book;
	while(book != NULL){
		if (!strcmp(book->somebody->surname,somebody->surname) && !strcmp(book->somebody->birth,somebody->birth) && !strcmp(book->somebody->email,somebody->email) && !strcmp(book->somebody->phone,somebody->phone)) break;
		if ((*(book->comparator))(book->somebody,somebody) > 0){
			if (book->right == NULL) return head;
				else{
					book = book->right;
				}
			}
		else{
			if (book->left == NULL)	return head;
				else{
					book = book->left;
				}
			}
	}
	freePerson(book->somebody);
	if (book->right == NULL && book->left == NULL && book->parent!=NULL){
		if(book->parent->left == book) book->parent->left = NULL;
		else book->parent->right = NULL;
		free(book);
		return head;
	}
	else if (book->right == NULL && book->left == NULL && book->parent==NULL){
		free(book);
		return NULL;
	}
	else if (book->right == NULL && book->parent!=NULL){
		if(book->parent->left == book) book->parent->left = book->left;
		else book->parent->right = book->left;
		free(book);
		return head;
	}
	else if (book->left == NULL && book->parent!=NULL){
		if(book->parent->left == book) book->parent->left = book->right;
		else book->parent->right = book->right;
		free(book);
		return head;
	}
	else if (book->right == NULL && book->parent==NULL){
		book = book->left;
		free(book->parent);
		book->parent = NULL;
		return book;
	}
	else if (book->left == NULL && book->parent==NULL){
		book = book->right;
		free(book->parent);
		book->parent = NULL;
		return book;
	}
	else {
		ContactsT* succ = findSucc(book);
		succ->left = book->left;
		book->left->parent = succ;
		
		if (succ->parent->left == succ){ 
			succ->parent->left= succ->right;
			if(succ->right != NULL) succ->right->parent = succ->parent;
		}
		else{
			succ->parent->right = succ->right;
			if(succ->right != NULL) succ->right->parent = succ->parent;
		}
		
		
		succ->right = book->right;
		succ->parent = book->parent;
		free(book);
		return succ;
	}
}

struct Person* getPersonT(const char* surname,const char* birth,const char* email,const char* phone,struct ContactsT* book){
	Person* helper = createPerson("name",surname,birth,email,phone,"address");
	while(book != NULL){
		if (!strcmp(book->somebody->surname,surname) && !strcmp(book->somebody->birth,birth) && !strcmp(book->somebody->email,email) && !strcmp(book->somebody->phone,phone)){
			freePerson(helper);
		 return book->somebody;
		}
		if ((*(book->comparator))(book->somebody,helper) > 0){
				if (book->right == NULL) return NULL;
				book = book->right;
			}
			else{
				if (book->left == NULL)	return NULL;
				book = book->left;
			}
	}
	return NULL;
}

struct ContactsT* sortContactsByT(const char* value,struct ContactsT* book){
	ContactsT* newBook = createBookSortedByT(value);
	if(!strcmp(value,"surname") || !strcmp(value,"birth") || !strcmp(value,"email") || !strcmp(value,"phone")){
		sortBookT(newBook,book);
	}
	else{
		free(newBook);
		return book;
	}
	return newBook;
}

void sortBookT(struct ContactsT* newBook,struct ContactsT* book){
	if (book->left != NULL){
		sortBookT(newBook,book->left);
	}
	if (book->right != NULL){
		sortBookT(newBook,book->right);
	}
	addPersonT(book->somebody,newBook);
	free(book);
}

void printContactT(struct ContactsT* contact){
	printf("****************************************\n");
	printf("Name: %s\n", contact->somebody->name);
	printf("Surname: %s\n", contact->somebody->surname);
	printf("Birth: %s\n", contact->somebody->birth);
	printf("Email: %s\n", contact->somebody->email);
	printf("Phone: %s\n", contact->somebody->phone);
	printf("Address: %s\n", contact->somebody->address);
	printf("****************************************\n");
}

void printContactsT(struct ContactsT* book){
	if (book->left != NULL){
		printContactsT(book->left);
	}
	printContactT(book);
	if (book->right != NULL){
		printContactsT(book->right);
	}
}

int compareSurname(struct Person* one,struct Person* two){
	return strcmp(one->surname,two->surname);
}
int compareBirth(struct Person* one,struct Person* two){
	return strcmp(one->birth,two->birth);
}
int compareEmail(struct Person* one,struct Person* two){
	return strcmp(one->email,two->email);
}
int comparePhone(struct Person* one,struct Person* two){
	return strcmp(one->phone,two->phone);
}

struct ContactsT* findSucc(struct ContactsT* node){
	return minContact(node->right);
}

struct ContactsT* minContact(struct ContactsT* node){
	if (node->left != NULL) return minContact(node->left);
	return node;
}


