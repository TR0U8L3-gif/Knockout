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
#include <time.h>
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
    char player_surname[MAX];
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
    struct player capitan;
    //struct player* players;
    struct team* next_team;
};


void clear(void);
int how_many_games(int);
void print_all_teams(struct team*);
struct team* enter_teams(int, struct team*);

int main(){
    int how_many_teams = 0;
    do
    {
        clear();
        printf("Enter the number of teams participating in the tournament: ");
        scanf("%d", &how_many_teams);
        if (how_many_teams <= 0)
        {
            clear();
            printf("The number of participating teams must be greater than one!!!\n");
            sleep(SLEEP);
        }
    } while (how_many_teams <= 0);
   // how_many_games(how_many_teams);
    struct team* first_team = NULL;
    first_team = enter_teams(how_many_teams, first_team);
    print_all_teams(first_team);

    return 0;
}

void clear(){
	#if _WIN32
	    system("cls");
	#elif defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
        system("clear");
	#else
	    printf("\n");
	#endif
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

void print_all_teams(struct team *team){
    while (team!=NULL)
    {
        clear();
        printf("team name: %s\nteam number: %d\n",team->team_name,team->team_number);
        printf("city: %s\nwebsite: %s\ne-mail: %s\n",team->team_city,team->team_websie,team->team_email);
        printf("team size: %d players\nis team still in tournament? ",team->team_size);
        if(team->in_tournament){
            printf("yes");
        }
        else{
            printf("no");
        }
        printf("\n");
        printf("address: %p\nnext team address: %p \n",team,team->next_team);

        sleep(SLEEP_LONG);
        team = team->next_team;
    }
}

struct team* enter_teams(int number_of_teams, struct team* team){
    clear();
    struct team* first_team = NULL;
    for (int i = 1; i <= number_of_teams; i++)
    {
        struct team* new_team = malloc(sizeof(struct team));
        new_team->in_tournament = true;
        new_team->team_number = i;
        new_team->next_team = NULL;


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
                printf("Team[%d] name is \"%s\"\nto correct team name press 'r'\nto accept team name press any other letter\noption: ",i,name);
                scanf(" %c",&option);
                if(option != 'r'){
                    ok = true;
                }
            }
        }while(!ok);
        strcpy(new_team->team_name, name);

        char city[MAX];
        ok = false;
        do
        {
            char option;
            clear();
            printf("Enter team[%d] city: ",i);
            scanf(" %[^\n]",city);
            if(strlen(name)>(MAX-1)){
                printf("Maximum city name is %d characters!!!", MAX-1);
                sleep(SLEEP);
            }
            else{
                clear();
                printf("Team[%d] city is \"%s\"\nto correct city name press 'r'\nto accept city name press any other letter\noption: ",i,city);
                scanf(" %c",&option);
                if(option != 'r'){
                    ok = true;
                }
            }
        }while(!ok);
        strcpy(new_team->team_city, city);

        char website[MAX_LONG];
        ok = false;
        do
        {
            char option;
            clear();
            printf("Enter team[%d] website: ",i);
            scanf(" %[^\n]",website);
            if(strlen(name)>(MAX_LONG-1)){
                printf("Maximum team website name is %d characters!!!", MAX_LONG-1);
                sleep(SLEEP);
            }
            else{
                clear();
                printf("Team[%d] website is \"%s\"\nto correct team website name press 'r'\nto accept team website name press any other letter\noption: ",i,website);
                scanf(" %c",&option);
                if(option != 'r'){
                    ok = true;
                }
            }
        }while(!ok);
        strcpy(new_team->team_websie, website);

        char email[MAX_LONG];
        ok = false;
        do
        {
            char option;
            clear();
            printf("Enter team[%d] email: ",i);
            scanf(" %[^\n]",email);
            if(strlen(name)>(MAX_LONG-1)){
                printf("Maximum team email size is %d characters!!!", MAX_LONG-1);
                sleep(SLEEP);
            }
            else{
                clear();
                printf("Team[%d] email is \"%s\"\nto correct team email press 'r'\nto accept team email press any other letter\noption: ",i,email);
                scanf(" %c",&option);
                if(option != 'r'){
                    ok = true;
                }
            }
        }while(!ok);
        strcpy(new_team->team_email, email);

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
