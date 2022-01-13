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
#include <time.h>

#define MAX 30
#define MAX_LONG 50
#define SLEEP_SHORT 2
#define SLEEP 4
#define SLEEP_LONG 8

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
int count_file_teams(char[]);
int how_many_teams(void);
int how_many_teams_in_tournament(struct team*);
int how_many_games(int);
void print_teams(struct team*, int);
void print_all_teams_to_file(struct team*, int);
void renumber_teams(struct team*);
struct team* enter_teams(struct team*, int, int);
struct team* enter_teams_from_file(struct team*, int, int);
struct team* delete_teams(struct team*, int);
struct team* delete_all_teams(struct team*);
void games_handler(struct team*, int*, int*, int);
void tournament_starter(struct team*, int); 
int tournament_handler(struct team**, struct team*, int);

int main()
{
    int re_start;
    int participating_teams;
    struct team* first_team = NULL;
    struct team** first_team_address = &first_team;
    
    restart:
        participating_teams = how_many_teams();
        re_start = tournament_handler(first_team_address, first_team, participating_teams);
    if(re_start == 1)
    {
        goto restart;
    }

    return 0;
}

void clear(void)
{
	#if _WIN32
	    system("cls");
	#elif defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
        system("clear");
	#else
	    printf("\n");
	#endif
}

int count_file_teams(char file_name[MAX])
{
    int count = 0;
    int teams = 0;

    FILE* f = NULL;
    f = fopen(file_name,"r");
    if(f == NULL)
    {
        clear();
        printf("There is not such a file!!!\n file %s not found\n", file_name);
        sleep(SLEEP);
        return -1;
    }
    else
    {
        clear();
        char data[MAX_LONG];
        int num = 0;

        while(fscanf(f, " %[^\n]", data) != EOF)
        {
            count++;
        }

        rewind(f);
        
        for (int line = 0; line < count;)
        {
            for(int i = 0; i < 4; i++)
            {
                fscanf(f, " %[^\n]", data);
                line++;
            }
        
            fscanf(f, "%d[^\n]", &num);
            line++;
            fscanf(f, " %[^\n]", data);
            line++;
            
            for (int i = 0; i < (num * 3); i++)
            {
                fscanf(f, " %[^\n]", data);
                line++; 
            }
            teams++;
            
        }
        fclose(f);
    }
    clear();
    //printf("teams in file: %d\n", teams);
    return teams;
}

int how_many_teams(void)
{
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

int how_many_teams_in_tournament(struct team* team)
{
    int still_playing = 0;
    while (team!=NULL)
    {
        if(team->in_tournament)
        {
            still_playing++;
        }
        team = team->next_team;
    }
    return still_playing;
}

int how_many_games(int number_of_teams)
{
    int result = 0;
    if (number_of_teams < 0)
    {
        return -1;
    }
    for (int i = 1; number_of_teams >= 2; i++)
    {
        int rest = number_of_teams % 2;
        int groups = number_of_teams / 2;
        result += groups;
        printf("Stage [%d]\n number of teams: %d number of matches: %d\n\n", i, number_of_teams, groups);
        number_of_teams = groups + rest;
    }
    printf("Number of matches played in the entire tournament: %d\n", result);
    int time = SLEEP_LONG * number_of_teams/2;
    sleep(time);
    return result;
}

void print_teams(struct team *team, int number_of_teams)
{
    int x;
    do
    {
        clear();
        struct team *first_team = team;
        while(team != NULL)
        {
            printf("team[%d]: %s\n", team->team_number, team->team_name);
            team = team->next_team;
        }
        printf("\n");
        team = first_team;
        printf(" To printa all teams type '0'\n To print one team type their \"team number\"\n\noption: ");
        scanf(" %d", &x);
        if(x<0||x>number_of_teams)
        {
            clear();
            printf("Input is incorrect!!!\n the number must be in the range [0,%d]\n",number_of_teams);
            sleep(SLEEP_SHORT);
        }
    }while(x<0||x>number_of_teams);

    while (team!=NULL)
    {
        if(team->team_number == x || x==0)
        {
            clear();
            printf("team name: %s\n\nteam number: %d\n",team->team_name,team->team_number);
            printf("city: %s\nwebsite: %s\ne-mail: %s\n",team->team_city,team->team_websie,team->team_email);
            printf("team size: %d players\nis team still in tournament? ",team->team_size);
            if(team->in_tournament)
            {
                printf("yes");
            }
            else
            {
                printf("no");
            }
            printf("\n");
            printf("\nteam captain name: %s\nsurname: %s\nage: %d\n\n",team->captain.player_name,team->captain.player_surname,team->captain.player_age);
            for(int i = 0; i<(team->team_size)-1; i++)
            {
            printf("player[%d] name: %s    surname: %s    age: %d\n",i+1,team->players[i].player_name,team->players[i].player_surname,team->players[i].player_age);
            }
            printf("\n");
            printf("address: %p\nnext team address: %p \n",team,team->next_team);
            sleep(SLEEP_LONG);
        }
        team = team->next_team;
    }
}

void print_all_teams_to_file(struct team *team, int number_of_teams)
{
    char file_name[MAX];
    bool ok = false;
    do
    {
        char option[2];
        clear();
        printf("Input file name where participating teams will be saved\nfile name (without .txt): ");
        scanf(" %[^\n]",file_name);
        if(strlen(file_name)>(MAX-5))
        {
            clear();
            printf("Maximum file name is %d characters!!!\n", MAX-5);
            sleep(SLEEP_SHORT);
        }
        else
        {
            clear();
            strcat(file_name,".txt");
            printf("File to save teams: %s\n",file_name);
            printf(" to correct file name input 'r'\n to accept file name input any other letter\noption: ");
            scanf(" %[^\n]", option);
            if(strlen(option)>1)
            {
               clear();
                printf("Incorrect input!!!\n pleas enter one letter\n");
                sleep(SLEEP_SHORT); 
            }
            else{
                if(option[0] != 'r')
                {
                    ok = true;
                }
            }
        }
    }while(!ok);
    
    FILE *f = NULL;
    f = fopen(file_name, "w");

    for(int k = 0; k < number_of_teams; k++)
    {
        fprintf(f, "%s\n", team->team_name);
        fprintf(f, "%s\n%s\n%s\n", team->team_city,team->team_websie,team->team_email);
        fprintf(f, "%d\n%d\n", team->team_size,team->in_tournament);
        fprintf(f, "%s\n%s\n%d\n", team->captain.player_name,team->captain.player_surname,team->captain.player_age);
        for(int i = 0; i<(team->team_size)-1; i++){
            fprintf(f, "%s\n%s\n%d\n",team->players[i].player_name,team->players[i].player_surname,team->players[i].player_age);
        }
        team = team->next_team;
    }
    fclose(f);
}

void renumber_teams(struct team* team)
{
    int i = 1;
    while(team!=NULL)
    {
        team->team_number = i;
        i++;
        team = team->next_team;
    }
}

struct team* enter_teams(struct team* team, int number_of_teams, int number_of_added_teams)
{
    clear();
    struct team* first_team = team;
    
    int remaining_teams = number_of_teams;
    while (team != NULL)
    {
        remaining_teams--;
        team = team->next_team;
    }
    team = first_team;
    if(remaining_teams == 0){
        printf("All slots are taken!!!\n");
        sleep(SLEEP);
        return first_team;
    }
    else if(number_of_added_teams>remaining_teams){
        printf(" %d slots reamin!!!\n", remaining_teams);
        sleep(SLEEP);
        number_of_added_teams = remaining_teams;
    }
    clear();
    for (int i = 1; i <= number_of_added_teams; i++)
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
                if(strlen(name)>(MAX-1))
                {
                    clear();
                    printf("Maximum team name is %d characters!!!\n", MAX-1);
                    sleep(SLEEP_SHORT);
                }
                else
                {
                    clear();
                    printf("Team[%d] name is \"%s\"\n to correct team name input 'r'\n to accept team name input any other letter\noption: ",i,name);
                    scanf(" %c",&option);
                    if(option != 'r')
                    {
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
                if(strlen(city)>(MAX-1))
                {
                    clear();
                    printf("Maximum city name is %d characters!!!\n", MAX-1);
                    sleep(SLEEP_SHORT);
                }
                else
                {
                    clear();
                    printf("Team[%d] city is \"%s\"\n to correct city name input 'r'\n to accept city name input any other letter\noption: ",i,city);
                    scanf(" %c",&option);
                    if(option != 'r')
                    {
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
                if(strlen(website)>(MAX_LONG-1))
                {
                    clear();
                    printf("Maximum team website name is %d characters!!!\n", MAX_LONG-1);
                    sleep(SLEEP_SHORT);
                }
                else
                {
                    clear();
                    printf("Team[%d] website is \"%s\"\n to correct team website name input 'r'\n to accept team website name input any other letter\noption: ",i,website);
                    scanf(" %c",&option);
                    if(option != 'r')
                    {
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
                if(strlen(email)>(MAX_LONG-1))
                {
                    clear();
                    printf("Maximum team email size is %d characters!!!\n", MAX_LONG-1);
                    sleep(SLEEP_SHORT);
                }
                else
                {
                    clear();
                    printf("Team[%d] email is \"%s\"\n to correct team email input 'r'\n to accept team email input any other letter\noption: ",i,email);
                    scanf(" %c",&option);
                    if(option != 'r')
                    {
                        ok = true;
                    }
                }
            }while(!ok);
            strcpy(new_team->team_email, email);
        }


        int team_members;
        do
        {
            char option;
            clear();
            printf("Enter how many players team[%d] has: ",i);
            scanf("%d", &team_members);
            if (team_members < 1)
            {
                clear();
                printf("A team must have one or more players!!!\n");
                sleep(SLEEP_SHORT);
            }
            else
            {
                clear();
                printf("Team[%d] size is \"%d\"\n to correct team size input 'r'\n to accept team size input any other letter\noption: ",i, team_members);
                scanf(" %c",&option);
                if(option == 'r')
                {
                    team_members = 0;
                }
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
                if(strlen(name)>(MAX-1))
                {
                    clear();
                    printf("Maximum team captain name is %d characters!!!\n", MAX-1);
                    sleep(SLEEP_SHORT);
                }
                else
                {
                    clear();
                    printf("Team[%d] captain name is \"%s\"\n to correct team captain name input 'r'\n to accept team captain name input any other letter\noption: ",i,name);
                    scanf(" %c",&option);
                    if(option != 'r')
                    {
                        ok = true;
                    }
                }
            }while(!ok);
            strcpy(new_team->captain.player_name, name);
        }

        {
            char surname[MAX_LONG];
            bool ok = false;
            do
            {
                char option;
                clear();
                printf("Enter team[%d] captain surname: ",i);
                scanf(" %[^\n]",surname);
                if(strlen(surname)>(MAX_LONG-1))
                {
                    printf("Maximum team captain surname is %d characters!!!\n", MAX_LONG-1);
                    sleep(SLEEP_SHORT);
                }
                else
                {
                    clear();
                    printf("Team[%d] captain surname is \"%s\"\n to correct team captain surname input 'r'\n to accept team captain surname input any other letter\noption: ",i,surname);
                    scanf(" %c",&option);
                    if(option != 'r')
                    {
                        ok = true;
                    }
                }
            }while(!ok);
            strcpy(new_team->captain.player_surname, surname);
        }

        {
            int age;
            do
            {
                char option;
                clear();
                printf("Enter team[%d] captain age: ",i);
                scanf("%d", &age);
                if (age < 1)
                {
                    clear();
                    printf("Incorrect input, age must be positive integer!!!\n");
                    sleep(SLEEP_SHORT);
                }
                else
                {
                    clear();
                    printf("Captain age is \"%d\"\n to correct captain age input 'r'\n to accept captain age input any other letter\noption: ", age);
                    scanf(" %c",&option);
                    if(option == 'r')
                    {
                        age = 0;
                    }
                }
            } while (age<1);

            new_team->captain.player_age = age;
        }

        //enter rest player data
        team_members--;
        struct player* members = malloc(team_members * sizeof(struct player));
        for(int j = 0; j<team_members; j++)
        {
            {
            char name[MAX];
            bool ok = false;
            do
            {
                char option;
                clear();
                printf("Enter team[%d] player[%d] name: ",i,j+1);
                scanf(" %[^\n]",name);
                if(strlen(name)>(MAX-1))
                {
                    printf("Maximum player name is %d characters!!!\n", MAX-1);
                    sleep(SLEEP_SHORT);
                }
                else
                {
                    clear();
                    printf("Team[%d] player[%d] name is \"%s\"\n to correct player name input 'r'\n to accept player name input any other letter\noption: ",i,j+1,name);
                    scanf(" %c",&option);
                    if(option != 'r')
                    {
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
                    if(strlen(sname)>(MAX_LONG-1))
                    {
                        printf("Maximum player surname is %d characters!!!\n", MAX_LONG-1);
                        sleep(SLEEP_SHORT);
                    }
                    else
                    {
                        clear();
                            printf("Team[%d] player[%d] surname is \"%s\"\n to correct player surname input 'r'\n to accept player surname input any other letter\noption: ",i,j+1,sname);                        scanf(" %c",&option);
                        if(option != 'r')
                        {
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
                    char option;
                    clear();
                    printf("Enter team[%d] player[%d] age: ", i, j+1);
                    scanf("%d", &age);
                    if (age < 1)
                    {
                        clear();
                        printf("Incorrect input, age must be positive integer!!!\n");
                        sleep(SLEEP_SHORT);
                    }
                    else
                    {
                        clear();
                        printf("Team[%d] player[%d] age is \"%d\"\n to correct player age input 'r'\n to accept player age input any other letter\noption: ", i, j+1, age);
                        scanf(" %c",&option);
                        if(option == 'r')
                        {
                            age = 0;
                        }
                    }
                } while (age<1);

                members[j].player_age = age;
            }
        }
        new_team->players = members;


        if(first_team == NULL)
        {
           first_team = new_team;
        }
        else
        {
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

struct team* enter_teams_from_file(struct team* team, int number_of_teams, int number_of_added_teams)
{
    char file_name[MAX];
    bool ok = false;
    do
    {
        char option;
        clear();
        printf("Input the name of the file with informations about the teams\nfile name (without .txt): ");
        scanf(" %[^\n]",file_name);
        if(strlen(file_name)>(MAX-5))
        {
            clear();
            printf("Maximum file name is %d characters!!!\n", MAX-5);
            sleep(SLEEP);
        }
        else
        {
            clear();
            strcat(file_name,".txt");
            printf("File to read informations about teams: %s\n",file_name);
            printf(" to correct file name input 'r'\n to accept file name input any other letter\noption: ");
            scanf(" %c",&option);
            if(option != 'r')
            {
                ok = true;
            }
        }
    }while(!ok); 

    int file_teams = count_file_teams(file_name);

    if(file_teams == -1)
    {     
       return team;
    }

    if(file_teams < number_of_teams)
    {
        clear();
        printf("File %s:\n teams taking part in the competition is %d\n", file_name, number_of_teams);
        printf(" file contains %d teams!!!\n", file_teams);
        number_of_teams = file_teams;
        sleep(SLEEP_LONG);
    }

    FILE* f = NULL;
    f = fopen(file_name,"r");

    struct team* first_team = team;
    
    int remaining_teams = number_of_teams;
    while (team != NULL)
    {
        remaining_teams--;
        team = team->next_team;
    }
    team = first_team;
    if(remaining_teams == 0){
        printf("All slots are taken!!!\n");
        sleep(SLEEP);
        return first_team;
    }
    else if(number_of_added_teams>remaining_teams){
        printf(" %d slots reamin!!!\n", remaining_teams);
        sleep(SLEEP);
        number_of_added_teams = remaining_teams;
    }

    clear();
    for (int i = 1; i <= number_of_added_teams; i++)
    {
        printf("team[%d] loading...\n", i);
        struct team* new_team = malloc(sizeof(struct team));

        int players = 0;
        fscanf(f, " %[^\n]", new_team->team_name);
        new_team->team_number = i;
        fscanf(f, " %[^\n]", new_team->team_city);
        fscanf(f, " %[^\n]", new_team->team_websie);
        fscanf(f, " %[^\n]", new_team->team_email);
        fscanf(f, "%d" , &new_team->team_size);
        int take_part = 1;
        fscanf(f, "%d" , &take_part);
        new_team->in_tournament = (bool)take_part;
        players = new_team->team_size;
        fscanf(f, " %[^\n]", new_team->captain.player_name);
        fscanf(f, " %[^\n]", new_team->captain.player_surname);
        fscanf(f, "%d" , &new_team->captain.player_age);
        players--;
        struct player* members = malloc(players * sizeof(struct player));
        for (int k = 0; k < players; k++)
        {
            fscanf(f, " %[^\n]", members[k].player_name);
            fscanf(f, " %[^\n]", members[k].player_surname);
            fscanf(f, "%d" , &members[k].player_age);
        }
        new_team->players = members;
        new_team->next_team = NULL;

        if(first_team == NULL)
        {
            first_team = new_team;
        }
        else
        {
            team = first_team;
            while ( team->next_team != NULL)
            {
                /*
                    printf("team name: %s\n\nteam number: %d\n",new_team->team_name,new_team->team_number);
                    printf("city: %s\nwebsite: %s\ne-mail: %s\n",new_team->team_city,new_team->team_websie,new_team->team_email);
                    printf("team size: %d players\nis team still in tournament? ",new_team->team_size);
                    if(new_team->in_tournament){
                        printf("yes");
                    }
                    else{
                        printf("no");
                    }
                    printf("\n");
                    printf("\nteam captain name: %s\nsurname: %s\nage: %d\n\n",new_team->captain.player_name,new_team->captain.player_surname,new_team->captain.player_age);
                    for (int k = 0; k < players; k++)
                    {
                        printf("player[%d] name: %s    surname: %s    age: %d\n",k+1,new_team->players[i].player_name,new_team->players[i].player_surname,new_team->players[i].player_age);
                    }
                */
                team = team->next_team;
            }
            team->next_team = new_team;
        }
        
        printf("team[%d] loaded\n", i);
    }
    fclose(f);
    return first_team;        
}

struct team* delete_teams(struct team* team, int number_of_teams)
{
    int x;
    do
    {
        clear();
        struct team *first_team = team;
        while(team != NULL)
        {
            printf("team[%d]: %s\n", team->team_number, team->team_name);
            team = team->next_team;
        }
        printf("\n");
        team = first_team;
        printf("To delete all teams type '0'\nTo delete one team type their \"team number\"\noption: ");
        scanf(" %d", &x);
        if(x<0||x>number_of_teams)
        {
            clear();
            printf("Input is incorrect!!!\n the number must be in the range [0,%d]\n",number_of_teams);
            sleep(SLEEP_SHORT);
        }
    }while(x<0 || x>number_of_teams);

    if(x == 0)
    {
        struct team *tmp = team;
        while(tmp)
        {
            struct team *delete_team = tmp;
            tmp = tmp->next_team;
            free(delete_team);
        }
        return tmp;
    }
    else
    {
        if(!team) return team;
        if(team->team_number == x)
        {
            struct team *delete_team = team;
            team = team->next_team;
            free(delete_team);
            return team;
        }
        
        struct team *tmp = team, *tmp_next = team->next_team;
        while(tmp_next!=NULL)
        {
            if(tmp_next->team_number == x)
            {   
                struct team *delete_team = tmp_next;
                tmp->next_team = tmp_next->next_team;
                free(delete_team);
                break;
            }
            tmp = tmp->next_team;
            tmp_next = tmp->next_team;
        }
        return team;
    }
}

struct team* delete_all_teams(struct team* team)
{
    struct team *tmp = team;
    while(tmp)
    {
        struct team *delete_team = tmp;
        tmp = tmp->next_team;
        free(delete_team);
    }
    return tmp;
}

void games_handler(struct team* team, int* games, int* queue, int number_of_teams)
{
    //clear();
    int rest = number_of_teams % 2;
    int groups = number_of_teams / 2;
    int matches = rest + groups;
    //printf("%d\n", matches);
    struct team* first_team = team;
    
    char first_team_name[MAX];
    char second_team_name[MAX];
   

    for(int i = 0; i < matches; i++)
    {
        while(team != NULL)
        {
            if(team->team_number == queue[i])
            {
                strcpy(first_team_name,team->team_name);
            }
            if(team->team_number == queue[games[i]])
            {
                strcpy(second_team_name,team->team_name);
            }
            team=team->next_team;
        }
        team = first_team;
        printf(" team[%s] vs ", first_team_name);
        if(games[i] != -1)
        {
            printf("team[%s] \n", second_team_name);
        }
        else
        {
            printf("none \n");
        }
        printf("\n");
    }
    
}

void tournament_starter(struct team* team, int number_of_teams)
{
    srand(time(NULL));
    
    struct team* first_team = team;

    int number_of_participating_teams = how_many_teams_in_tournament(first_team);
    for (int i = 1; number_of_participating_teams >= 2; i++)
    {
        clear();
        int* queue = calloc(number_of_participating_teams,sizeof(int));
        
        int number = 0;
        while(team != NULL)
        {
            if(team->in_tournament)
            {
                queue[number] = team->team_number;
                //printf("team[%d]: %s\n", number+1, team->team_name);
                number++;
            }
            if(number == number_of_participating_teams)
            {
                break;
            }
            team = team->next_team;
        }
        printf("\n");

        int rest = number_of_participating_teams % 2;
        int groups = number_of_participating_teams / 2;
        int* games = calloc((rest+groups),sizeof(int));

        if(rest)
        {
            games[rand()%(rest+groups)] = -1; 
        }

        for (int k = (rest+groups); k < number_of_participating_teams; k++)
        {
            int fate = rand()%(rest+groups);
            while(games[fate] != 0)
            {
                fate = rand()%(rest+groups);
            }
            games[fate] = k;
        }
            
        printf("Stage [%d]: \n\n", i);

        games_handler(first_team, games, queue,number_of_participating_teams);

        number_of_participating_teams = groups + rest;
        team = first_team;

        free(games);
        free(queue);
        sleep(SLEEP_LONG);

    }
}

int tournament_handler(struct team** first_team, struct team* team, int number_of_teams)
{
    bool ok = false;
    char input[2];
    do
    {
        int entered_teams = how_many_teams_in_tournament(team);
        clear();
        printf("%d teams take part in the competition \n", number_of_teams);
        printf("%d teams entered the competition \n\n", entered_teams);
        
        printf(" Input 'n' to display number of games during the contets\n");
        printf(" Input 'a' to add one team to competition\n");
        printf(" Input 'e' to enter participating teams from keyboard\n");
        printf(" Input 'l' to load participating teams from file\n");
        printf(" Input 'r' to restart program\n");
        printf(" Input 'x' to exit program\n");
        
        if(team != NULL)
        {
            printf(" Input 'p' to display participating team\n");
            printf(" Input 'd' to delete participating team\n");
            printf(" Input 't' to start the tournament\n");
            printf(" Input 's' to save participating teams to file\n");
        }
        printf("\noption: ");
        
        scanf(" %[^\n]",input);
        if(strlen(input)>1)
        {
            clear();
            printf("Incorrect input!!!\n pleas enter one letter\n");
            sleep(SLEEP_SHORT);
        }
        else
        {
            clear();
            //printf("input: %c\n", input[0]);
            switch (input[0])
            {
                case 'n':
                    how_many_games(number_of_teams);
                break;

                case 'a':
                    {
                        bool ok = false;
                        do
                        {
                            char option;
                            clear();
                            printf("Add one team:\n input 'k' to enter from keyboard \n input 'f' to enter from keyboard \n\noption: ");
                            scanf(" %c",&option);
                            if(option == 'k')
                            {
                                team = enter_teams(team, number_of_teams, 1);
                                ok = true;
                            }
                            else if(option == 'f')
                            {
                                team = enter_teams_from_file(team, number_of_teams, 1);
                                ok = true;
                            }
                        }while(!ok);
                        (*first_team) = team; 
                        renumber_teams(team);
                    }
                break;

                case 'e':
                    team = enter_teams(team, number_of_teams, number_of_teams);
                    (*first_team) = team; 
                    renumber_teams(team);
                break;

                case 'l':
                    team = enter_teams_from_file(team, number_of_teams, number_of_teams);
                    (*first_team) = team; 
                    renumber_teams(team);
                break;

                case 'r':
                    return 1;
                break;

                case 'x':
                    team = delete_all_teams(team);
                    (*first_team) = team;
                    ok = true;
                break;

                default:
                    if (team == NULL)
                    {
                        printf("Incorect input!!!\n there is not [%c] option\n", input[0]);
                        sleep(SLEEP_SHORT);
                    }
                    else
                    {
                        switch (input[0])
                        {
                            case 'p':
                                print_teams(team, entered_teams);
                            break;

                            case 'd':
                                team = delete_teams(team, number_of_teams);
                                (*first_team) = team;
                                renumber_teams(team);
                            break;

                            case 't':
                                tournament_starter(team, entered_teams);
                            break;

                            case 's':
                                print_all_teams_to_file(team, entered_teams);
                            break;

                            default:
                                printf("Incorect input!!!\n there is not [%c] option\n", input[0]);
                                sleep(SLEEP_SHORT);
                            break;
                        } 
                    }
                break;
            }      
        }
    }
    while(!ok);

    return 0;
}