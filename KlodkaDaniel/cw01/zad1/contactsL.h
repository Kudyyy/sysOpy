#include "person.h"
#ifndef CONTACTSL_H
#define CONTACTSL_H

typedef struct ContactsL{
	struct Person* somebody;
	struct ContactsL* next;
	struct ContactsL* prev;
} ContactsL;

void deleteContactsL(struct ContactsL* book);
struct ContactsL* addPersonL(struct Person* somebody,struct ContactsL* book);
struct ContactsL* deletePersonL(struct Person* somebody,struct ContactsL* book);
struct Person* getPersonL(const char* surname,const char* birth,const char* email,const char* phone,struct ContactsL* book);
struct ContactsL* sortContactsByL(const char* value,struct ContactsL* book);
void swapPersonsL(struct ContactsL* first,struct ContactsL* second);
void printContactsL(struct ContactsL* book);
void printContactL(struct ContactsL* contact);
struct ContactsL* createBookL(void);

#endif