/*
 * Do zawod�w przyst�puje pewna liczba dru�yn, ka�da z nich jest opisywana przez nazw�, miasto oraz adres strony domowej i email kontaktowy
 * Dru�yna musi mie� jednego lub wi�cej zawodnik�w, z kt�rych jeden jest kapitanem (ka�dy zawodnik ma imi�, nazwisko oraz wiek)
 * Po wprowadzeniu danych i uruchomieniu zawod�w, tworzona jest (losowo) kolejno�� rozgrywek w trybie pucharowym
 * Je�li liczba dru�yn nie jest pot�g� dw�jki, wolne miejsca s� uzupe�niane pustymi danymi (niekt�rzy uczestnicy od razu przejd� do kolejnego etapu)
 * Mo�emy wprowadza� wyniki kolejnych mecz�w i wy�ania� zwyci�zc�w kolejnych etap�w, a� do fina�u
 * Ka�da dru�yna mo�e r�wnie� zosta� na dowolnym etapie zdyskwalifikowana
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
