/*
 * Do zawodow przystepuje pewna liczba druzyn, kazda z nich jest opisywana przez nazwe, miasto oraz adres strony domowej i email kontaktowy
 * Druzyna musi miec jednego lub wiecej zawodnikow, z ktorych jeden jest kapitanem (kazdy zawodnik ma imie, nazwisko oraz wiek)
 * Po wprowadzeniu danych i uruchomieniu zawodow, tworzona jest (losowo) kolejnosc rozgrywek w trybie pucharowym
 * Jesli liczba druzyn nie jest potega dwojki, wolne miejsca se uzupelniane pustymi danymi (niektorzy uczestnicy od razu przejda do kolejnego etapu)
 * Mozemy wprowadzac wyniki kolejnych meczow i wylaniac zwyciezcow kolejnych etapow, az do finalu
 * Kazda druzyna moze rowniez zostac na dowolnym etapie zdyskwalifikowana
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#define MAX 30
#define MAX_LONG 50
#define SLEEP 3
#define SLEEP_LONG 5

struct player
{
    char player_name[MAX];
    char player_surname[MAX_LONG];
    int player_age;
};

struct team
{
    bool in_tournament;
    int team_number;
    int team_size;
    char team_name[MAX];
    char team_city[MAX];
    char team_websie[MAX_LONG];
    char team_email[MAX_LONG];
    struct player captain;
    struct player* players;
    struct team* next_team;
};


void clear(void);
int how_many_teams(void);
int how_many_games(int);
void print_teams(struct team*, int);
struct team* enter_teams(struct team*, int);
int tournament_handler(struct team*, int);

int main(){

    int participating_teams = how_many_teams();
    //how_many_games(participating_teams);
    struct team* first_team = NULL;
    first_team = enter_teams(first_team, participating_teams);
    //print_teams(first_team, participating_teams);
    tournament_handler(first_team, participating_teams);

    return 0;
}

void clear(void){
	#if _WIN32
	    system("cls");
	#elif defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
        system("clear");
	#else
	    printf("\n");
	#endif
}

int how_many_teams(void){
    int teams;
    do
    {
        clear();
        printf("Enter the number of teams participating in the tournament: ");
        scanf("%d", &teams);
        if (teams <=1)
        {
            clear();
            printf("The number of participating teams must be greater than one!!!\n");
            sleep(SLEEP);
        }
    } while (teams <= 1);
    return teams;
}

int how_many_games(int number_of_teams){
    int result = 0;
    if (number_of_teams < 0) {
        return -1;
    }
    for (int i = 1; number_of_teams >= 2; i++)
    {
        int rest = number_of_teams % 2;
        int groups = number_of_teams / 2;
        result += groups;
        printf("Stage %d number of teams: %d number of matches: %d\n", i, number_of_teams, groups);
        number_of_teams = groups + rest;
    }
    printf("Number of matches played in the entire tournament: %d\n", result);
    sleep(3);
    return result;
}

void print_teams(struct team *team, int number_of_teams){
    clear();
    int x;
    do{
        printf("To printa all teams type '0'\nTo print one team type their team_muber\noption: ");
        scanf("%d", &x);
        if(x<0||x>number_of_teams){
            clear();
            printf("Input is incorrect!!!\nthe number must be in the range [0,%d]",x);
            sleep(SLEEP);
        }
    }while(x<0);

    while (team!=NULL)
    {
        if(team->team_number == x || x==0){
            clear();
            printf("team name: %s\n\nteam number: %d\n",team->team_name,team->team_number);
            printf("city: %s\nwebsite: %s\ne-mail: %s\n",team->team_city,team->team_websie,team->team_email);
            printf("team size: %d players\nis team still in tournament? ",team->team_size);
            if(team->in_tournament){
                printf("yes");
            }
            else{
                printf("no");
            }
            printf("\n");
            printf("\nteam captain name: %s\nsurname: %s\nage: %d\n\n",team->captain.player_name,team->captain.player_surname,team->captain.player_age);
            for(int i = 0; i<(team->team_size)-1; i++){
            printf("player[%d] name: %s    surname: %s    age: %d\n",i+1,team->players[i].player_name,team->players[i].player_surname,team->players[i].player_age);
            }
            printf("\n");
            printf("address: %p\nnext team address: %p \n",team,team->next_team);

            sleep(SLEEP_LONG);
            team = team->next_team;
        }
    }
}

struct team* enter_teams(struct team* team, int number_of_teams){
    clear();
    struct team* first_team = NULL;
    for (int i = 1; i <= number_of_teams; i++)
    {
        struct team* new_team = malloc(sizeof(struct team));
        new_team->in_tournament = true;
        new_team->team_number = i;
        new_team->next_team = NULL;

        //input data about team
        {
            char name[MAX];
            bool ok = false;
            do
            {
                char option;
                clear();
                printf("Enter team[%d] name: ",i);
                scanf(" %[^\n]",name);
                if(strlen(name)>(MAX-1)){
                    printf("Maximum team name is %d characters!!!", MAX-1);
                    sleep(SLEEP);
                }
                else{
                    clear();
                    printf("Team[%d] name is \"%s\"\nto correct team name input 'r'\nto accept team name input any other letter\noption: ",i,name);
                    scanf(" %c",&option);
                    if(option != 'r'){
                        ok = true;
                    }
                }
            }while(!ok);
            strcpy(new_team->team_name, name);
        }

        {
            char city[MAX];
            bool ok = false;
            do
            {
                char option;
                clear();
                printf("Enter team[%d] city: ",i);
                scanf(" %[^\n]",city);
                if(strlen(city)>(MAX-1)){
                    printf("Maximum city name is %d characters!!!", MAX-1);
                    sleep(SLEEP);
                }
                else{
                    clear();
                    printf("Team[%d] city is \"%s\"\nto correct city name input 'r'\nto accept city name input any other letter\noption: ",i,city);
                    scanf(" %c",&option);
                    if(option != 'r'){
                        ok = true;
                    }
                }
            }while(!ok);
            strcpy(new_team->team_city, city);
        }

        {
            char website[MAX_LONG];
            bool ok = false;
            do
            {
                char option;
                clear();
                printf("Enter team[%d] website: ",i);
                scanf(" %[^\n]",website);
                if(strlen(website)>(MAX_LONG-1)){
                    printf("Maximum team website name is %d characters!!!", MAX_LONG-1);
                    sleep(SLEEP);
                }
                else{
                    clear();
                    printf("Team[%d] website is \"%s\"\nto correct team website name input 'r'\nto accept team website name input any other letter\noption: ",i,website);
                    scanf(" %c",&option);
                    if(option != 'r'){
                        ok = true;
                    }
                }
            }while(!ok);
            strcpy(new_team->team_websie, website);
        }

        {
            char email[MAX_LONG];
            bool ok = false;
            do
            {
                char option;
                clear();
                printf("Enter team[%d] email: ",i);
                scanf(" %[^\n]",email);
                if(strlen(email)>(MAX_LONG-1)){
                    printf("Maximum team email size is %d characters!!!", MAX_LONG-1);
                    sleep(SLEEP);
                }
                else{
                    clear();
                    printf("Team[%d] email is \"%s\"\nto correct team email input 'r'\nto accept team email input any other letter\noption: ",i,email);
                    scanf(" %c",&option);
                    if(option != 'r'){
                        ok = true;
                    }
                }
            }while(!ok);
            strcpy(new_team->team_email, email);
        }


        int team_members;
        do
        {
            clear();
            printf("Enter how many players team[%d] has: ",i);
            scanf("%d", &team_members);
            if (team_members < 1)
            {
                clear();
                printf("A team must have one or more players!!!\n");
                sleep(SLEEP);
            }
        } while (team_members<1);

        new_team->team_size = team_members;

        // input data about team captain
        {
            char name[MAX];
            bool ok = false;
            do
            {
                char option;
                clear();
                printf("Enter team[%d] captain name: ",i);
                scanf(" %[^\n]",name);
                if(strlen(name)>(MAX-1)){
                    printf("Maximum team captain name is %d characters!!!", MAX-1);
                    sleep(SLEEP);
                }
                else{
                    clear();
                    printf("Team[%d] captain name is \"%s\"\nto correct team captain name input 'r'\nto accept team captain name input any other letter\noption: ",i,name);
                    scanf(" %c",&option);
                    if(option != 'r'){
                        ok = true;
                    }
                }
            }while(!ok);
            strcpy(new_team->captain.player_name, name);
        }

        {
            char sname[MAX_LONG];
            bool ok = false;
            do
            {
                char option;
                clear();
                printf("Enter team[%d] captain surname: ",i);
                scanf(" %[^\n]",sname);
                if(strlen(sname)>(MAX_LONG-1)){
                    printf("Maximum team captain surname is %d characters!!!", MAX_LONG-1);
                    sleep(SLEEP);
                }
                else{
                    clear();
                    printf("Team[%d] captain surname is \"%s\"\nto correct team captain surname input 'r'\nto accept team captain surname input any other letter\noption: ",i,sname);
                    scanf(" %c",&option);
                    if(option != 'r'){
                        ok = true;
                    }
                }
            }while(!ok);
            strcpy(new_team->captain.player_surname, sname);
        }

        {
            int age;
            do
            {
                clear();
                printf("Enter team[%d] captain age: ",i);
                scanf("%d", &age);
                if (age < 1)
                {
                    clear();
                    printf("Incorrect input, age must be positive integer!!!\n");
                    sleep(SLEEP);
                }
            } while (age<1);

            new_team->captain.player_age = age;
        }

        //enter rest player data
        team_members--;
        struct player* members = malloc(team_members * sizeof(struct player));
        for(int j = 0; j<team_members; j++){
            {
            char name[MAX];
            bool ok = false;
            do
            {
                char option;
                clear();
                printf("Enter team[%d] player[%d] name: ",i,j+1);
                scanf(" %[^\n]",name);
                if(strlen(name)>(MAX-1)){
                    printf("Maximum player name is %d characters!!!", MAX-1);
                    sleep(SLEEP);
                }
                else{
                    clear();
                    printf("Team[%d] player[%d] name is \"%s\"\nto correct player name input 'r'\nto accept player name input any other letter\noption: ",i,j+1,name);
                    scanf(" %c",&option);
                    if(option != 'r'){
                        ok = true;
                    }
                }
            }while(!ok);
            strcpy(members[j].player_name, name);
            }

            {
                char sname[MAX_LONG];
                bool ok = false;
                do
                {
                    char option;
                    clear();
                    printf("Enter team[%d] player[%d] surname: ",i,j+1);
                    scanf(" %[^\n]",sname);
                    if(strlen(sname)>(MAX_LONG-1)){
                        printf("Maximum player surname is %d characters!!!", MAX_LONG-1);
                        sleep(SLEEP);
                    }
                    else{
                        clear();
                            printf("Team[%d] player[%d] surname is \"%s\"\nto correct player surname input 'r'\nto accept player surname input any other letter\noption: ",i,j+1,sname);                        scanf(" %c",&option);
                        if(option != 'r'){
                            ok = true;
                        }
                    }
                }while(!ok);
                strcpy(members[j].player_surname, sname);
            }

            {
                int age;
                do
                {
                    clear();
                    printf("Enter team[%d] player[%d] age: ",i,j+1);
                    scanf("%d", &age);
                    if (age < 1)
                    {
                        clear();
                        printf("Incorrect input, age must be positive integer!!!\n");
                        sleep(SLEEP);
                    }
                } while (age<1);

                members[j].player_age = age;
            }
        }
        new_team->players = members;


        if(first_team == NULL){
           first_team = new_team;
        }
        else{
            team = first_team;
            while ( team->next_team != NULL)
            {
                team = team->next_team;
            }
            team->next_team = new_team;
        }
    }
    return first_team;
}

int tournament_handler(struct team* team, int number_of_teams){
    bool ok = false;
    char input[2];
    do{
        clear();
        printf("%d teams take part in the competition \n\n", number_of_teams);
        printf(" Input 'd' to display participating team\n");
        printf(" Input 'i' to display number of games during the contets\n");
        printf(" Input 'e' to re-enter participating team\n");
        printf("\noption: ");
        scanf(" %[^\n]",input);
        if(strlen(input)>1){
            clear();
            printf("Incorrect input!!!\npleas enter one letter");
            sleep(SLEEP);
        }
        else{
            printf("%c", input[0]);
            ok = true;
        }
    }
    while(!ok);
}