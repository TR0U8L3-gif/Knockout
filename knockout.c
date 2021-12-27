#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;
  
    // Storing start time
    clock_t start_time = clock();
  
    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds);
}

void start(){
    char z = '|';
    while (1)
    {
        for(int i = 0; i < 8; i++){
            switch (i%8)
            {
            case 0:
                z = '|';
                break;
            case 1:
                z = '/';
                break;
            case 2:
                z = '-';
                break;
            case 3:
                z = '\\';
                break;
            case 4:
                z = '|';
                break;
            case 5:
                z = '/';
                break;
            case 6:
                z = '-';
                break;
            case 7:
                z = '\\';
                break;
            }
            printf("%c\n", z);
            delay(150);
            system("clear");
        }
        
    }
}
int ile_meczy(int x){
    int ile = 0;
    if(x<0){
        return -1;
    }
    for(int i = 1; x>=2; i++)
    {
        int r = x%2;
        int g = x/2;
        ile += g;
        x = g+r;
        printf("Etap %d liczba druzyn: %d liczba meczy: %d\n", i, x, g);
    }
    printf("Liczba rozegranych meczy w turnieju: %d\n", ile);
    return ile;
}
int main(){
    //start();
    int ile_druzyn = 0;
    printf("Podaj liczbe druzyn bioracych udzial w zawodach: ");
    scanf("%d", &ile_druzyn);
    ile_meczy(ile_druzyn);
    return 0;
}