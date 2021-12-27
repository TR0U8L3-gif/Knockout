/*
 * Do zawodów przystêpuje pewna liczba dru¿yn, ka¿da z nich jest opisywana przez nazwê, miasto oraz adres strony domowej i email kontaktowy
 * Dru¿yna musi mieæ jednego lub wiêcej zawodników, z których jeden jest kapitanem (ka¿dy zawodnik ma imiê, nazwisko oraz wiek)
 * Po wprowadzeniu danych i uruchomieniu zawodów, tworzona jest (losowo) kolejnoœæ rozgrywek w trybie pucharowym
 * Jeœli liczba dru¿yn nie jest potêg¹ dwójki, wolne miejsca s¹ uzupe³niane pustymi danymi (niektórzy uczestnicy od razu przejd¹ do kolejnego etapu)
 * Mo¿emy wprowadzaæ wyniki kolejnych meczów i wy³aniaæ zwyciêzców kolejnych etapów, a¿ do fina³u
 * Ka¿da dru¿yna mo¿e równie¿ zostaæ na dowolnym etapie zdyskwalifikowana
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void delay(int);
void clear();
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
    system("cls");
   // system("clear");
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
