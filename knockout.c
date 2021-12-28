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

void delay(int);
void clear(void);
int how_many_games(int);

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
            delay(3000);
        }
    } while (how_many_teams <= 0);
    how_many_games(how_many_teams);
    return 0;
}

void delay(int number_of_milli_seconds){
    clock_t start_time = clock();
    while (clock() < start_time + number_of_milli_seconds);
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
    return result;
}
