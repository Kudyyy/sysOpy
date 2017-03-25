#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	 
	if (argc > 1){
	 	printf("\n\nWho called me with those args ? ");
	 	for (int i = 0; i < argc; ++i) printf("%s ",argv[i]);
	}
	else printf("\n\nPoor call with no args\n");
	char* s = getenv("Twaciescia");
	printf("\nBtw Twaciescia to %s\n\n", (s!=NULL) ? s : "iles tam, nie wiem bo nigdzie nie pisze");
}