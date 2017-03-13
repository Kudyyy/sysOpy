#include "../1/contactsT.h"
#include "../1/contactsL.h"
#include <stdio.h>
#include <time.h> //dla clock i CLOCK_PER_SEC
#include <sys/times.h> //dla times
#include <string.h>
#include <unistd.h>

void checkTimes(struct tms *prevTimes, clock_t *prevReal, struct tms *firstTimes, clock_t *firstReal);

int main(void){
	 char *names[] = {"Anna", "Barney", "Cecille", "Duncan", "Elize", "Frodo", "Gertrude",
            "Hipolito", "Inez", "Johnny", "Kay", "Luke", "Monique", "Nico", "Oprah", "Peter",
            "Raquel", "Stan", "Trinity", "Uriel", "Vivien", "Warwick", "Zenoby"};
     char *surnames[] = {"Aaron", "Abandoned", "Beforeable", "Bundy", "Charizard", "Chomsky", "Dreyfuse",
            "Ernnie", "Foldable", "Growth", "Huntery", "Jopp", "Kustard", "Leopard", "Lohn", "Mummond",
            "Osterrich", "Pueblo", "Robbinsky", "Sequel", "Straw", "Tintin", "Wicked"};
     char *births[] = {"1997-02-24", "1994-01-15", "1984-01-14", "1999-10-15", "1954-11-10", "2000-02-14", "1991-05-25",
            "1977-08-01", "1993-01-01", "1948-12-13", "1992-04-21", "1988-01-15", "1971-09-21", "1983-01-17", "1991-01-15", "2010-04-09",
            "1998-11-19", "1990-10-10", "2009-07-14", "2002-02-12", "1964-04-24", "1967-12-01", "1994-12-12","1985-05-01"};
     char *emails[] = {"@onet.pl", "@gmail.pl", "@vp.pl", "@interia.pl", "@yahoo.com", "@wp.pl", "@dem.com"};
     char *numbers[] = {"423534523","1543535234","242354234","23423423","65765434","5236346543","235663456","7525524",
     "843523234","2753465239","4235387623","563535234","241554234","237633423","6577432434","5236734543"};
     char *addresses[] = {"High Street", "Station Road", "Main Street", "Park Road", "Church Road", "Church Street", "London Road",
            "Victoria Road", "Green Lane", "Manor Road", "Church Lane", "Park Avenue", "The Avenue", "The Crescent", "Queens Road", "New Road",
            "Grange Road", "Kings Road", "Kingsway", "Windsor Road", "Highfield Road", "Mill Lane", "Alexander Road","York Road"};
     int NUMBER_OF_NAMES = 23;
     int NUMBER_OF_SURNAMES = 23;
     int NUMBER_OF_BIRTHS = 24;
     int NUMBER_OF_EMAILS = 7;
     int NUMBER_OF_NUMBERS = 16;
     int NUMBER_OF_ADDRESSES = 24;
     int LIST_SIZE = 10000;

     struct tms prevTimes;
     clock_t prevReal;
     struct tms firstTimes;
     clock_t firstReal;



     prevTimes.tms_stime = -1;

     checkTimes(&prevTimes, &prevReal, &firstTimes, &firstReal);
     printf("Creating the list of %d elements.\n", LIST_SIZE-2);
     Person* sb1;
     Person* sb2;
     ContactsL* bookL = createBookL();
     srand(time(NULL));
     char* name;
     char* surname;
     char* birth;
     char* email;
     char* phone;
     char* address;
    for (int i = 0; i < LIST_SIZE-2; i++) {
        name = names[rand() % NUMBER_OF_NAMES];
        surname = surnames[rand() % NUMBER_OF_SURNAMES];
        birth = births[rand() % NUMBER_OF_BIRTHS];
        email = emails[rand() % NUMBER_OF_EMAILS];
        phone = numbers[rand() % NUMBER_OF_NUMBERS];
        address = addresses[rand() % NUMBER_OF_ADDRESSES];
        sb1 = createPerson(name,surname,birth,email,phone,address);
        bookL = addPersonL(sb1,bookL);
    }
    checkTimes(&prevTimes, &prevReal, &firstTimes, &firstReal);
    printf("Adding 2 contacts to list.\n");
    sb1 = createPerson("Daniel","Klodka","1111-01-17","dankl@gmail.com","669432253","Pomodagfa");
    bookL = addPersonL(sb1,bookL);
    sb1 = createPerson("Danoo","Klogaga","3333-01-17","datcal@gmail.com","669436753","Pomgagfa");
    bookL = addPersonL(sb1,bookL);

    checkTimes(&prevTimes, &prevReal, &firstTimes, &firstReal);

    printf("Sorting the list by birth.\n");
    bookL = sortContactsByL("birth",bookL);
    checkTimes(&prevTimes, &prevReal, &firstTimes, &firstReal);

    printf("Finding youngest contact in the list. \n");
    sb1 = getPersonL("Klodka","1111-01-17","dankl@gmail.com","669432253",bookL);
	checkTimes(&prevTimes, &prevReal, &firstTimes, &firstReal);

	printf("Finding oldest contact in the list. \n");
    sb2 = getPersonL("Klogaga","3333-01-17","datcal@gmail.com","669436753",bookL);
	checkTimes(&prevTimes, &prevReal, &firstTimes, &firstReal);

	printf("Deleting youngest contact from the list.\n");
	deletePersonL(sb1,bookL);
	checkTimes(&prevTimes, &prevReal, &firstTimes, &firstReal);

	printf("Deleting oldest contact from the list.\n");
	bookL = deletePersonL(sb2,bookL);
	checkTimes(&prevTimes, &prevReal, &firstTimes, &firstReal);

	printf("Deleting the list.\n");
	deleteContactsL(bookL);
	checkTimes(&prevTimes, &prevReal, &firstTimes, &firstReal);

	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	prevTimes.tms_stime = -1;
	checkTimes(&prevTimes, &prevReal, &firstTimes, &firstReal);

    printf("Creating the tree of %d elements.\n", LIST_SIZE-2);
	ContactsT* bookT = createBookSortedByT("surname");
    for (int i = 0; i < LIST_SIZE-1; i++) {
        name = names[rand() % NUMBER_OF_NAMES];
        surname = surnames[rand() % NUMBER_OF_SURNAMES];
        birth = births[rand() % NUMBER_OF_BIRTHS];
        email = emails[rand() % NUMBER_OF_EMAILS];
        phone = numbers[rand() % NUMBER_OF_NUMBERS];
        address = addresses[rand() % NUMBER_OF_ADDRESSES];
        sb1 = createPerson(name,surname,birth,email,phone,address);
        addPersonT(sb1,bookT);
    }

    checkTimes(&prevTimes, &prevReal, &firstTimes, &firstReal);
    printf("Adding 2 contacts to tree.\n");
    sb1 = createPerson("Daniel","ZZZZZZZ","1111-01-17","dankl@gmail.com","669432253","Pomodagfa");
	addPersonT(sb1,bookT);
	sb1 = createPerson("Daniel","lalal","3333-01-17","dankl@gmail.com","669432253","Pomodagfa");
	addPersonT(sb1,bookT);
	checkTimes(&prevTimes, &prevReal, &firstTimes, &firstReal);

	printf("Sorting the tree by birth.\n");
    bookT = sortContactsByT("birth",bookT);
    checkTimes(&prevTimes, &prevReal, &firstTimes, &firstReal);

    printf("Finding root contact in the tree. \n");
    sb1 = getPersonT("ZZZZZZZ","1111-01-17","dankl@gmail.com","669432253",bookT);
	checkTimes(&prevTimes, &prevReal, &firstTimes, &firstReal);

	printf("Finding leaf contact in the tree. \n");
    sb2 = getPersonT("lalal","3333-01-17","dankl@gmail.com","669432253",bookT);
	checkTimes(&prevTimes, &prevReal, &firstTimes, &firstReal);

	printf("Deleting leaf contact from the tree.\n");
	bookT = deletePersonT(sb2,bookT);
	checkTimes(&prevTimes, &prevReal, &firstTimes, &firstReal);

	printf("Deleting root contact from the tree.\n");
	bookT = deletePersonT(sb1,bookT);
	checkTimes(&prevTimes, &prevReal, &firstTimes, &firstReal);

	printf("Deleting the tree.\n");
	deleteContactsT(bookT);
	checkTimes(&prevTimes, &prevReal, &firstTimes, &firstReal);
}

#define CLK sysconf(_SC_CLK_TCK)

void checkTimes(struct tms *prevTimes, clock_t *prevReal,
        struct tms *firstTimes, clock_t *firstReal) {

    struct tms now;
    times(&now);
    clock_t nowReal = clock();
    if (prevTimes->tms_stime == -1) {
        *firstTimes = now;
        *firstReal = nowReal;
    } else {
        printf("\n\tFrom the first check:\t\tR %.6f\tS %.6f\tU %.6f",
                ((double) (nowReal - *(firstReal))) / CLOCKS_PER_SEC,
                ((double) (now.tms_stime - firstTimes->tms_stime)) / CLK,
                ((double) (now.tms_utime - firstTimes->tms_utime)) / CLK);

        printf("\n\tFrom the previous check:\tR %.6f\tS %.6f\tU %.6f",
                ((double) (nowReal - *(prevReal))) / CLOCKS_PER_SEC,
                ((double) (now.tms_stime - prevTimes->tms_stime)) / CLK,
                ((double) (now.tms_utime - prevTimes->tms_utime)) / CLK);
    }
    printf("\n\tTime:\t\t\t\tR %.6f\tS %.6f\tU %.6f\n\n",
            ((double) nowReal) / CLOCKS_PER_SEC,
            ((double) now.tms_stime) / CLK,
            ((double) now.tms_utime) / CLK);
    *prevReal = nowReal;
    *prevTimes = now;

}