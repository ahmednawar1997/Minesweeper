#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BOMB '*'


typedef struct
{
    char name[20];
    char score[20];

} playersList;
//Global Variables
playersList player[20];
int numberOfRows,numberOfColumns;
char board[30][30];
char userBoard[30][30];
char boardOfFlags[30][30];
int i,j,numberOfMoves=0,numberOfCellsWithoutMines=1,exitGame=0,numberOfFlags=0,numberOfQuestionMarks=0,m=0;
int numberOfScoresInFile=0,firstTime=1;
int arrayOfMines[40][2];
time_t start, end;
float diff, time1 = 0;
float timeFromFile = 0.0;


void initializeArray()
{
    for(i=0; i<numberOfRows; i++)
    {
        for(j=0; j<numberOfColumns; j++)
        {
            board[i][j]='X';
            userBoard[i][j]='X';
            boardOfFlags[i][j]=0
                               ;
        }
    }
}


void placeRandomMines(int firstMineRow,int firstMineColumn)
{
    int row,column;
    int dontTakeInput=0;
    int numberOfMines = 1+((numberOfRows+1)*numberOfColumns/10);
    numberOfCellsWithoutMines=numberOfRows*numberOfColumns-numberOfMines;

    srand(time(NULL));//copied

    for(i=0; i<numberOfMines;)
    {
        dontTakeInput=0;

        row =rand()%numberOfRows;
        column= rand()%numberOfColumns;

        for(m=0; m<i; m++)
        {
            if((arrayOfMines[m][0]==row && arrayOfMines[m][1]==column) || (row==firstMineRow&&column==firstMineColumn))
                dontTakeInput=1;
            break;

        }

        if(dontTakeInput==0)
        {
            board[row][column]=BOMB;

            arrayOfMines[i][0]=row;
            arrayOfMines[i][1]=column;
        }
        if(dontTakeInput==0)
            i++;

    }
    buildBoard();



}


void printUserBoard()
{
    int j,k;
    printf("\nNumber Of Moves: %d\n",numberOfMoves);
    printf("Number Of Flags: %d\n",numberOfFlags);
    printf("Number Of Marks: %d\n",numberOfQuestionMarks);
    time(&end);
    diff = difftime(end, start);
    time1 += diff;
    time(&start);
    printf("Time Passed: %.2f seconds\n\n", time1);

    printf("\n      ");

    for(k=0; k<numberOfColumns; k++)
        printf("  %2d ",k+1);

    printf("\n");
    printf("       ");
    for(i=0; i<numberOfColumns; i++)
        printf(" ___ ");
    printf("\n\n");

    for(i=0; i<numberOfRows; i++)
    {
        printf(" %2d   |",i+1);
        for(j=0; j<numberOfColumns; j++)
            printf("  %c  ",userBoard[i][j]);

        printf("|\n");
        if(i<numberOfRows-1)
            printf("\n");
    }
    printf("       ");

    for(i=0; i<numberOfColumns; i++)
        printf(" ___ ");
    printf("\n");

}


void printBoard()//Just for developing purposes
{
    int j,k;
    printf("\n   ");

    for(k=0; k<numberOfColumns; k++)
        printf("%2d ",k+1);

    printf("\n\n");

    for(i=0; i<numberOfRows; i++)
    {
        printf("%2d ",i+1);
        for(j=0; j<numberOfColumns; j++)
            printf(" %c ",board[i][j]);

        printf("|\n");
    }
}


void buildBoard()
{
    //put the numbers of mines in the board
    int numberOfMinesNearCell=0;
    char C;
    for(i=0; i<numberOfRows; i++)
        for(j=0; j<numberOfColumns; j++)
        {
            if(board[i][j]!=BOMB)
            {
                if(board[i-1][j-1] == BOMB&&i-1>=0&&j-1>=0)
                    numberOfMinesNearCell++;
                if(board[i-1][j] == BOMB&&i-1>=0)
                    numberOfMinesNearCell++;
                if(board[i-1][j+1] == BOMB&&i-1>=0&&j+1<numberOfColumns)
                    numberOfMinesNearCell++;
                if(board[i][j-1] == BOMB&&j-1>=0)
                    numberOfMinesNearCell++;
                if(board[i][j+1] == BOMB&&j+1<numberOfColumns)
                    numberOfMinesNearCell++;
                if(board[i+1][j-1] == BOMB&&i+1<numberOfRows&&j-1>=0)
                    numberOfMinesNearCell++;
                if(board[i+1][j] == BOMB&&i+1<numberOfRows)
                    numberOfMinesNearCell++;

                if(board [i+1][j+1] == BOMB&&i+1<numberOfRows&&j+1<numberOfColumns)
                    numberOfMinesNearCell++;

                C = (char)(((int)'0')+numberOfMinesNearCell);
                numberOfMinesNearCell=0;

                board[i][j] = C;
            }
        }
}


void openCell()
{
    int row,column;
    do
    {

        printf("\nEnter the row and column of the cell you want to perform your action on: ");
        scanf("%d %d",&row,&column);
        row=row-1;
        column=column-1;
    }
    while(row<0||row>numberOfRows-1||column<0||column>numberOfColumns-1);

    if(firstTime==1)
    {
        placeRandomMines(row,column);
    }



    if(board[row][column] == BOMB)
    {
        numberOfMoves++;

        int choice;
        system("cls");

        time(&end);
        diff = difftime(end, start);

        printf("You Stepped On A Mine! GAME OVER!\ntime is %.2f seconds\a", diff + timeFromFile);

        for(i=0; i<numberOfRows; i++)
        {
            for(j=0; j<numberOfColumns; j++)
            {
                userBoard[row][column]='!';
                board[row][column]='!';
                if(board[i][j]==BOMB&&userBoard[i][j]!='F')
                {

                    userBoard[i][j]=BOMB;

                }



                if(board[i][j]==BOMB&&userBoard[i][j]=='F')
                {

                    userBoard[i][j]='-';

                }
            }
        }

        printUserBoard();
        do
        {

            printf("You Lost!\n What Do You Want To Do?\n1-New Game\t2-Quit\n");
            scanf("%d",&choice);
        }
        while(choice<1||choice>2);
        switch(choice)
        {

        case 1:
            newGame();
            break;
        case 2:
            exit(0);
        }



    }

    else if(board[row][column] == '0')
    {
        numberOfMoves++;

        floodFill(row,column);
        //userBoard[row][column]=' ';                 //Lazem DFS
        printUserBoard();

    }

    else
    {
        userBoard[row][column]=board[row][column];
        numberOfMoves++;
    }

    system("cls");
    printUserBoard();

    firstTime=0;
}


void flagCell()
{

    int row,column;
    printf("\nEnter the row and column of the cell you want to Flag: ");
    scanf("%d %d",&row,&column);
    row=row-1;
    column=column-1;
    if(userBoard[row][column]!=' ')
    {

        numberOfMoves++;

        userBoard[row][column]='F';
        numberOfFlags++;
        system("cls");
        printUserBoard();
    }
    else printf("Can't Flag This Position!\n");

}


void markCell()
{
    int row,column;
    printf("\nEnter the row and column of the cell you want to Mark: ");
    scanf("%d %d",&row,&column);
    row=row-1;
    column=column-1;
    if(userBoard[row][column]=='X')
    {
        numberOfMoves++;

        userBoard[row][column]='?';
        numberOfQuestionMarks++;
        system("cls");
        printUserBoard();
    }
    else printf("Can't Mark This Position!\n");

}


void unMarkCell()
{
    int row,column;
    printf("\nEnter the row and column of the cell you want to Unmark: ");
    scanf("%d %d",&row,&column);
    row=row-1;
    column=column-1;
    if(userBoard[row][column]=='?' || userBoard[row][column] == 'F')
    {
        numberOfMoves++;

        if(userBoard[row][column] == 'F')
                numberOfFlags--;

        userBoard[row][column]='X';

        if(userBoard[row][column]=='?')
              numberOfQuestionMarks--;
        system("cls");
        printUserBoard();
    }
    else printf("There Is No Mark At This Cell!\n");

}


void loadScores()
{
    i=0;
    FILE* f1 = fopen("GameScores.txt","r");
    system("cls");
    if(f1!=NULL)
    {
        while(!feof(f1))
        {
            fscanf(f1,"%[^,],%s\n",player[i].name,player[i].score);
            i++;


        }
        numberOfScoresInFile=i;
    }
    else printf("File Not Found!\n\n");
    fclose(f1);


}

void viewScores()
{
    loadScores();
    system("cls");
    printf("Players Names and Scores: \n");

    for(i=0; i<numberOfScoresInFile; i++)
        printf("%d-%s\t%s\n",i+1,player[i].name,player[i].score);

    printf("\n");

}


void addPlayerScore(char winnerUser[20],char winnerScore[15])  //until i calculate score
{
    int found=0,index;
    loadScores();
    system("cls");
    FILE* f1 = fopen("GameScores.txt","a");


    for(i=0; i<numberOfScoresInFile; i++)
    {
        if(strcmp(winnerUser,player[i].name)==0)
        {
            found=1;
            index=i;

        }
    }
    if(found==1)
    {
        if(strcmp(winnerScore,player[index].score)>0)
        {
            if(f1!=NULL)
            {

                fprintf(f1,"\n%s,%s",winnerUser,winnerScore);
                printf("Score Updated\n");

            }
            else printf("File Not Found!\n\n");
            printf("Added Player's Score");
            fclose(f1);

        }

    }

    else if(found==0)
    {
        if(f1!=NULL)
        {
            fprintf(f1,"\n%s,%s",winnerUser,winnerScore);
            printf("Score Added!\n");
        }
        else printf("File Not Found!\n\n");

        fclose(f1);
    }
}


int findNearbyMines(int x,int y)
{
    int numberOfMinesNearCell=0;
    if(board[x-1][y-1] == BOMB&&x-1>=0&&y-1>=0)
        numberOfMinesNearCell++;
    if(board[x-1][y] == BOMB&&x-1>=0)
        numberOfMinesNearCell++;
    if(board[x-1][y+1] == BOMB&&x-1>=0&&y+1<numberOfColumns)
        numberOfMinesNearCell++;
    if(board[x][y-1] == BOMB&&y-1>=0)
        numberOfMinesNearCell++;
    if(board[x][y+1] == BOMB&&y+1<numberOfColumns)
        numberOfMinesNearCell++;
    if(board[x+1][y-1] == BOMB&&x+1<numberOfRows&&y-1>=0)
        numberOfMinesNearCell++;
    if(board[x+1][y] == BOMB&&x+1<numberOfRows)
        numberOfMinesNearCell++;
    if(board [x+1][y+1] == BOMB&&x+1<numberOfRows&&y+1<numberOfColumns)
        numberOfMinesNearCell++;

    return numberOfMinesNearCell;



}


void floodFill( int x, int y )
{
    if (board[x][y]!=BOMB&&x<numberOfRows&&x>=0&&y<numberOfColumns&&y>=0)
    {
        if(board[x][y]=='0')
        {
            userBoard[x][y]=' ';
            boardOfFlags[x][y]=1;          // 1=visited
        }
        else
        {
            userBoard[x][y]=(char)(((int)'0')+findNearbyMines(x,y));
            boardOfFlags[x][y]=1;
            return;
        }

        if(x+1<numberOfRows&&boardOfFlags[x+1][y]==0)
            floodFill( x+1, y );
        if(x-1>=0&&boardOfFlags[x-1][y]==0)
            floodFill( x-1, y );
        if(y-1>=0&&boardOfFlags[x][y-1]==0)
            floodFill( x, y-1 );
        if(y+1<numberOfColumns&&boardOfFlags[x][y+1]==0)
            floodFill( x, y+1 );
        if(x+1<numberOfRows&&y-1>=0&&boardOfFlags[x+1][y-1]==0)
            floodFill( x+1, y-1 );
        if(y+1<numberOfColumns&&x-1>=0&&boardOfFlags[x-1][y+1]==0)
            floodFill( x-1, y+1 );
        if(y+1<numberOfColumns&&x+1<numberOfRows&&boardOfFlags[x+1][y+1]==0)
            floodFill( x+1, y+1 );
        if(y-1>=0&&x-1>=0&&boardOfFlags[x-1][y-1]==0)
            floodFill( x-1, y-1 );

    }
    else
    {
        return;
    }
}



void newGame()
{
    firstTime=1;
    do
    {
        printf("Enter Number Of Rows And Number Of Columns Of the Board: ");
        scanf("%d %d",&numberOfRows,&numberOfColumns);
    }
    while(numberOfRows<2||numberOfColumns<2||numberOfRows>30||numberOfColumns>30);
    system("cls");
    time(&start);
    initializeArray();
    printUserBoard();
    startGame();

}



int checkIfWon()
{
    int count=0;

    for(i=0; i<numberOfRows; i++)
        for(j=0; j<numberOfColumns; j++)
        {
            if(userBoard[i][j]!='X'&&userBoard[i][j]!='F')
                count++;


        }
    if(count==numberOfCellsWithoutMines)
        return 1;
    else return 0;

}



void startGame()
{
    int choice;
    char winnerUser[20];
    char winnerScore[20];
    printf("\nStart the game!\n");
    while(exitGame==0)
    {

        if(checkIfWon()==1)
        {
            printf("Congrats!! You Won The Game\n\a\a");
            printf("Enter Your Name:");
            getchar();
            gets(winnerUser);
            addPlayerScore(winnerUser,winnerScore);

            do
            {

                printf("You Won!\n What Do You Want To Do?\n1-New Game\t2-Quit\n");
                scanf("%d",&choice);
            }
            while(choice<1||choice>2);
            switch(choice)
            {

            case 1:
                newGame();
                break;
            case 2:
                exit(0);
            }


        }

        //printBoard();



        printf("Next Move: \n");
        do
        {
            printf("1-Open Cell\t2-Flag Cell\t3-Mark A Cell\t4-UnMark A Cell\t 5-Exit\t 6-Save Game\t7-New Game\n");
            scanf("%d",&choice);
        }
        while(choice<1||choice>7);


        switch (choice)
        {

        case 1:
            openCell();
            break;
        case 2:
            flagCell();
            break;
        case 3:
            markCell();
            break;
        case 4:
            unMarkCell();
            break;
        case 5:
            exit(0);
        case 6 :
            saveGame();
            break;
        case 7:
            newGame();
            break;
        }

    }

}



void loadGame()
{
    char fileName[15];
    char fileNameInfo[15] = "info";

    printf("Enter file name:\n\n");

    scanf("%s",fileName);

    FILE *f, *f1;

    int i, j;

    f = fopen(strcat(fileName, ".txt"), "r");

    f1 = fopen(strcat(fileNameInfo, fileName), "r");

    if(f == NULL || f1 == NULL)
    {
        printf("File not found");
        exit(0);
    }

    fscanf(f1, "%f\n%d\n%d\n%d", &timeFromFile, &numberOfRows, &numberOfColumns, &numberOfMoves);

    time(&start);

    for(i = 0; i < numberOfRows; i++)
    {
        for(j = 0; j < numberOfColumns; j++)
        {
            *(*(board+i)+j) = fgetc(f);
            *(*(userBoard+i)+j) = fgetc(f);
        }
        fgetc(f);
    }

    fclose(f);

    system("cls");

    printf("File loaded\n\n");

    printUserBoard();

    startGame();
}



void saveGame()
{
    system("cls");

    FILE *f, *f1;

    char fileName[15];

    char fileNameInfo[15] = "info";

    printf("\nEnter file name:\n\n");

    scanf("%s",fileName);

    f = fopen(strcat(fileName,".txt"),"w");

    f1 = fopen(strcat(fileNameInfo, fileName),"w");

    fprintf(f1,"%f\n%d\n%d\n%d",time1, numberOfRows, numberOfColumns, numberOfMoves)    ;

    fclose(f1);

    int i, j;

    for(i = 0; i < numberOfRows; i++)
    {
        for(j = 0; j < numberOfColumns; j++)
        {
            fprintf(f,"%c",board[i][j]);
            fprintf(f,"%c",userBoard[i][j]);
        }
        fprintf(f,"\n");
    }

    fclose(f);

    printf("\n\nFile saved");

    exit(0);

}


int main()
{
    int choice;
    while(1)
    {
        printf("Hi, Choose What You Wish To Perform: \n");
        do
        {
            printf("1-Start A New Game\n2-Load A Saved Game\n3-View Player's Scores\n4-Quit\n");
            scanf("%d",&choice);
        }
        while(choice<1||choice>4);
        switch(choice)
        {
        case 1:
            newGame();
            break;
        case 2:
            loadGame();
            break;
        case 3:
            viewScores();
            break;
        case 4:
            exit(0);
        }
    }
    return 0;
}
