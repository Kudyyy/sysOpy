#ifndef PERSON_H
#define PERSON_H

typedef struct Person{
	char* name;
	char* surname;
	char* birth;
	char* email;
	char* phone;
	char* address;
} Person; 

void freePerson(struct Person* somebody);
struct Person* createPerson(const char* name,const char* surname,const char* birth,const char* email,const char* phone,const char* address);

#endif