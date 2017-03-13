#include "person.h"
#include <string.h>
#include <stdlib.h> 

void freePerson(struct Person* somebody){
	free(somebody->name);
	free(somebody->surname);
	free(somebody->birth);
	free(somebody->email);
	free(somebody->phone);
	free(somebody->address);
	free(somebody);
}

struct Person* createPerson(const char* name,const char* surname,const char* birth,const char* email,const char* phone,const char* address){
	Person* somebody = (Person*)malloc(sizeof(struct Person));
	somebody->name = (char*)malloc(50);
	strcpy(somebody->name , name);
	somebody->surname = (char*)malloc(50);
	strcpy(somebody->surname , surname);
	somebody->birth = (char*)malloc(20);
	strcpy(somebody->birth , birth);
	somebody->email = (char*)malloc(70);
	strcpy(somebody->email , email);
	somebody->phone = (char*)malloc(20);
	strcpy(somebody->phone , phone);
	somebody->address = (char*)malloc(100);
	strcpy(somebody->address , address);
	return somebody;
}