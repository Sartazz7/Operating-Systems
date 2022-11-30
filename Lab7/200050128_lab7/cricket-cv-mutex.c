#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define MAX_SCORE 100

int titans = 0;
int capitals = 0;
int titansScore = 0;
int capitalsScore = 0;
int highestScore = 0;

int match = 1;
int winCapitals = 0;
int winTitans = 0;
int highestTeamScore = 0;
int highestIndividualScore = 0;

int numPlayers;
pthread_mutex_t lock1;

int randomNumber(){
    return rand()%MAX_SCORE;
}

void displayResult(){
    printf("------------------------MATCH: ( %d ) Summary------------------------\n\n",match);
    printf("SCORE: Capitals : %d :: Titans : %d\n",capitalsScore,titansScore);
    printf("Highest Individual Score : %d\n",highestScore);
    if(highestScore > highestIndividualScore) highestIndividualScore = highestScore;
    if(titansScore > capitalsScore){
        printf("Result : Titans won by %d runs\n\n",titansScore-capitalsScore);
        winTitans++;
        if(titansScore > highestTeamScore)highestTeamScore = titansScore;
    }
    else if(capitalsScore > titansScore){
        printf("Result : Capitals won by %d runs\n\n",capitalsScore-titansScore);
        winCapitals++;
        if(capitalsScore > highestTeamScore)highestTeamScore = capitalsScore;
    }
    else{
        printf("Result : Draw\n\n");
    }

    titans = 0;
    capitals = 0;
    titansScore = 0;
    capitalsScore = 0;
    highestScore = 0;
    match++;
}

void play(int isTitans){
    int score = randomNumber();
    if(isTitans == 0)capitalsScore += score;
    else titansScore += score;
    if(highestScore < score)highestScore = score;
    if(titans == 11)displayResult();
}

void *enterMatch(){
    pthread_mutex_lock(&lock1);
    if(capitals < 11){
        capitals++;
        play(0);
    }
    else if(titans < 11){
        titans++;
        play(1);
    }
    pthread_mutex_unlock(&lock1);
}

int main(int argc, char* argv[]){
    srand(0);
    int inpt = atoi(argv[1]);
    numPlayers = inpt-inpt%22;
    pthread_t th[numPlayers];
    
    for(int i=0; i<numPlayers; i++){
        pthread_create(th+i,NULL,&enterMatch,NULL);
    }
    for(int i=0; i<numPlayers; i++){
        pthread_join(th[i],NULL);
    }
    if(inpt%22!=0){
        printf("-----------------Match : ( %d ) Cannot take place-------------------\n\n",match);
    }
    match--;
    printf("-------------------------SUMMARY OF THE DAY-------------------------\n");
    printf("Matches Played : %d\n",match);
    printf("Titans     :: Won : %d || Lost : %d || Tied : %d\n",winTitans,winCapitals,match-winCapitals-winTitans);
    printf("Capitals   :: Won : %d || Lost : %d || Tied : %d\n",winCapitals,winTitans,match-winCapitals-winTitans);
    printf("Highest Team Score          : %d\n",highestTeamScore);
    printf("Highest Individual Score    : %d\n",highestIndividualScore);
    printf("--------------------------------------------------------------------\n");
}