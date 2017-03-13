#include "contactsL.h"
#include <string.h>
#include <stdlib.h>  
#include <stdio.h>
#include <stdbool.h>

struct ContactsL* createBookL(void){
	ContactsL* contact = (ContactsL*)malloc(sizeof(struct ContactsL));
	contact->next = NULL;
	contact->prev = NULL;
	contact->somebody = NULL;
	return contact;
}

void deleteContactsL(struct ContactsL* book){
	while(book->prev != NULL){
		book = book->prev;
	}
	ContactsL* next;
	while(book != NULL){
		next = book->next;
		freePerson(book->somebody);
		free(book);
		book = next;
	}
}

struct ContactsL* addPersonL(struct Person* somebody,struct ContactsL* book){
	while(book->prev != NULL){
		book = book->prev;
	}
	if (book->somebody == NULL){
		book->somebody = somebody;
	}
	else{
	ContactsL* contact = (ContactsL*)malloc(sizeof(struct ContactsL));
	contact->next = book;
	book->prev = contact;
	contact->prev = NULL;
	book = contact;
	book->somebody = somebody;
	}
	return book;
}

struct ContactsL* deletePersonL(struct Person* somebody,struct ContactsL* book){
	while(book->prev != NULL){
		book = book->prev;
	}
	ContactsL* tmp = book;
	while(book->somebody != somebody && book->next != NULL) book = book->next;
	if(book->somebody == somebody){
		freePerson(book->somebody);
		if(book->prev != NULL){
			book->prev->next=book->next;
			if(book->next != NULL) book->next->prev = book->prev;
		}
		else{
			book->next->prev = NULL;
			tmp = book->next;
		}
		free(book);
		book = tmp;
	}
	return book;
}

struct Person* getPersonL(const char* surname,const char* birth,const char* email,const char* phone,struct ContactsL* book){
	while(book->prev != NULL){
		book = book->prev;
	}
	while(book != NULL){
		if (!strcmp(book->somebody->surname,surname) && !strcmp(book->somebody->birth,birth) && !strcmp(book->somebody->email,email) && !strcmp(book->somebody->phone,phone)) return book->somebody;
		book = book->next;
	}
	return NULL;
}

struct ContactsL* sortContactsByL(const char* value,struct ContactsL* book){
	while(book->prev != NULL){
		book = book->prev;
	}
	bool sorted = true;
	if(!strcmp(value,"name")){
		while(1){
			if (book->next == NULL) {
				while(book->prev != NULL){
					book = book->prev;
				}
				if (sorted) return book;
				sorted = true;
			}
			if (strcmp(book->somebody->name,book->next->somebody->name) > 0){
				sorted = false;
				swapPersonsL(book,book->next);
				book = book->prev;
			}
			book = book->next;
		}
	}
	else if(!strcmp(value,"surname")){
		while(1){
			if (book->next == NULL) {
				while(book->prev != NULL){
					book = book->prev;
				}
				if (sorted) return book;
				sorted = true;
			}
			if (strcmp(book->somebody->surname,book->next->somebody->surname) > 0){
				sorted = false;
				swapPersonsL(book,book->next);
				book = book->prev;
			}
			book = book->next;
		}
	}
	else if(!strcmp(value,"birth")){
		while(1){
			if (book->next == NULL) {
				while(book->prev != NULL){
					book = book->prev;
				}
				if (sorted) return book;
				sorted = true;
			}
			if (strcmp(book->somebody->birth,book->next->somebody->birth) > 0){
				sorted = false;
				swapPersonsL(book,book->next);
				book = book->prev;
			}
			book = book->next;
		}
	}
	else if(!strcmp(value,"email")){
		while(1){
			if (book->next == NULL) {
				while(book->prev != NULL){
					book = book->prev;
				}
				if (sorted) return book;
				sorted = true;
			}
			if (strcmp(book->somebody->email,book->next->somebody->email) > 0){
				sorted = false;
				swapPersonsL(book,book->next);
				book = book->prev;
			}
			book = book->next;
		}
	}
	else if(!strcmp(value,"phone")){
		while(1){
			if (book->next == NULL) {
				while(book->prev != NULL){
					book = book->prev;
				}
				if (sorted) return book;
				sorted = true;
			}
			if (strcmp(book->somebody->phone,book->next->somebody->phone) > 0){
				sorted = false;
				swapPersonsL(book,book->next);
				book = book->prev;
			}
			book = book->next;
		}
	}
	else if(!strcmp(value,"address")){
		while(1){
			if (book->next == NULL) {
				while(book->prev != NULL){
					book = book->prev;
				}
				if (sorted) return book;
				sorted = true;
			}
			if (strcmp(book->somebody->address,book->next->somebody->address) > 0){
				sorted = false;
				swapPersonsL(book,book->next);
				book = book->prev;
			}
			book = book->next;
		}
	}
	else return book;
}

void swapPersonsL(struct ContactsL* first,struct ContactsL* second){
	first->next = second->next;
	if (second->next!=NULL) second->next->prev = first;
	second->prev = first->prev;
	second->next = first;
	first->prev = second;
	if (second->prev!= NULL) second->prev->next = second;
}

void printContactL(struct ContactsL* contact){
	printf("****************************************\n");
	printf("Name: %s\n", contact->somebody->name);
	printf("Surname: %s\n", contact->somebody->surname);
	printf("Birth: %s\n", contact->somebody->birth);
	printf("Email: %s\n", contact->somebody->email);
	printf("Phone: %s\n", contact->somebody->phone);
	printf("Address: %s\n", contact->somebody->address);
	printf("****************************************\n");
}

void printContactsL(struct ContactsL* book){
	while(book->prev!= NULL) book=book->prev;
	while(book != NULL){
		printContactL(book);
		book = book->next;
	}
}


