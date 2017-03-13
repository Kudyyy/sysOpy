#ifndef CONTACTST_H
#define CONTACTST_H
#include "person.h"

typedef struct ContactsT{
	int (*comparator)(struct Person* one,struct Person* two);
	struct Person* somebody;
	struct ContactsT* left;
	struct ContactsT* right;
	struct ContactsT* parent;
} ContactsT;

void deleteContactsT(struct ContactsT* book);
void addPersonT(struct Person* somebody,struct ContactsT* book);
struct ContactsT* deletePersonT(struct Person* somebody,struct ContactsT* book);
struct Person* getPersonT(const char* surname,const char* birth,const char* email,const char* phone,struct ContactsT* book);
void printContactsT(struct ContactsT* book);
void printContactT(struct ContactsT* contact);
struct ContactsT* createBookSortedByT(const char* value);
int compareName(struct Person* one,struct Person* two);
int compareSurname(struct Person* one,struct Person* two);
int compareBirth(struct Person* one,struct Person* two);
int compareEmail(struct Person* one,struct Person* two);
int comparePhone(struct Person* one,struct Person* two);
int compareAddress(struct Person* one,struct Person* two);
struct ContactsT* findSucc(struct ContactsT* node);
struct ContactsT* minContact(struct ContactsT* node);
void sortBookT(struct ContactsT* newBook,struct ContactsT* book);
struct ContactsT* sortContactsByT(const char* value,struct ContactsT* book);
#endif