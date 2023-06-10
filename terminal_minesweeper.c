//***************************************************************
//****************  Terminal Mine Sweeper  **********************
//***************************************************************
// Author: Johan Rengstedt
// email: johan.rengstedt@gmail.com
// year: 2023

// A game of minsweeper played in the terminal window


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

//Constants
#define MINE '*'
#define CLEARED ' '
#define UNCLEARED '.'
#define nbrOfMines 8
#define boardSize 9

// Variables
char board[boardSize][boardSize];
char minePos[boardSize][boardSize];
int squaresRemaining;

typedef struct coordinates
{
    int x;
    int y;
}COORD;

// Function declarations
void setupBoard();
COORD playerMove();
void placeFlag();
void placeMines(COORD);
char checkForMine(COORD);
void updateBoard(COORD);
int countAdjacentMines(COORD);
bool isValidMove(COORD);
void printBoard();
void revealBoard();
void youWon();
void youLost();



//***************************************************************
int main(){
    printf("------------------------------------------------\n");
    printf("**** Hello! Welcome to terminal minesweeper ****\n");
    printf("------------------------------------------------\n");

    squaresRemaining = sizeof(board);
    setupBoard();
    printBoard();

    printf("------------ Press enter to play ---------------\n");
    COORD move = playerMove();  //First move
    placeMines(move);           //Place mines after first move
    updateBoard(move);
    printBoard();

    
    while(true){  //Main loop

        move = playerMove();

        if(checkForMine(move)==MINE){  //Check if a mine was hit
            board[move.x][move.y]  = MINE;
            revealBoard();
            printBoard();
            youLost();
            break;
        }

        updateBoard(move);
        printBoard();
        
        if(squaresRemaining <= nbrOfMines){ //Check if all squares are cleared
            revealBoard();
            printBoard();
            youWon();
            break;
        }
        
    }   
    printf("Thanks for playing\n\n");
    return 0;
}
//***************************************************************
// Initial setup of board
void setupBoard(){
    for(int i =0; i< boardSize; i++){
        for(int j = 0; j<boardSize; j++){
            board[i][j] = UNCLEARED;
            minePos[i][j] = ' ';
        }
    }
}
//***************************************************************
// Place mines at random locations (but not adjacent to first move)
void placeMines(COORD move){
    int x;
    int y;
    srand(time(0));

    for(int i = 0; i<nbrOfMines; i++){
        do
        {
            x = rand() % boardSize;
            y = rand() % boardSize; 
        } while (minePos[x][y] == MINE || (abs(x-move.x)<2 && abs(y-move.y)< 2));

        minePos[x][y] = MINE;
    }
}
//***************************************************************
//Promts player for move
COORD playerMove(){
    COORD move;

    while(true){

        char input1;
        char input2;
        char garbage;

        scanf("%c",&garbage); //Clear the buffer
        
        printf("** Enter F to place a flag, enter Q to exit the game **\n\n ");   
        printf("Enter row number to make a move:  ");           
        scanf("%c", &input1);

        if (input1 == 'Q'+0 || input1 == 'q'+0) {
            printf("Game exited. Thanks for playing!\n");
            exit(0);
        } else if(input1 == 'F' || input1 == 'f'){
            placeFlag();
            printBoard();
            move = playerMove();
            break;

        } else {
            move.x = input1-'1';
            scanf("%c",&garbage); //Clear the buffer
            printf("Enter column number:  ");
            scanf("%c", &input2);
            move.y = input2-'1';

            if(isValidMove(move)){
                break;
            }
            else{     
                printf("Invalid move! \n");                  
            }
        }     
    } 
    return move;
}
//***************************************************************
// Places a flag on user defined square
void placeFlag(){

    char input1;
    char input2;
    char garbage;
    COORD flagMove;

    printf("\n***** FLAG PLACEMENT MODE ***** \n");

    while(true){

        scanf("%c",&garbage); //Clear the buffer
        printf("[Flag mode] Enter row number (F to exit flag mode):  ");             
        scanf("%c", &input1);

        if(input1 == 'F' || input1 == 'f'){
            return;
        } else {
            flagMove.x = input1-'1';
            scanf("%c",&garbage); //Clear the buffer
            printf("[Flag mode] Enter column number:  ");
            scanf("%c", &input2);
            flagMove.y = input2-'1';

            if(isValidMove(flagMove)){
                board[flagMove.x][flagMove.y] = 'P';
                printBoard();
            }
            else{     
                printf("Invalid flag position! \n");                  
            }
        }
    }   
}
//***************************************************************
// Checks square for mine
char checkForMine(COORD posToCheck){
    
    if(minePos[posToCheck.x][posToCheck.y]==MINE){
        return MINE;
    }else {
        return ' ';
    }
}
//***************************************************************
// Updates the board to show cleared squares and number of adjacent mines
void updateBoard(COORD move){

    int adjacentBombs = countAdjacentMines(move); //Counts number of adjacent mines
 
    if(adjacentBombs>0){
        board[move.x][move.y] = adjacentBombs+'0';  //Square is set to number of adjacent mines
        squaresRemaining--;
       
    }else{
        board[move.x][move.y] = CLEARED; // If no mines, square is set to empty space
        squaresRemaining--;

        //Update adjacent squares 
        for(int i =move.x-1; i<=move.x+1; i++){
            for(int j =move.y-1; j<=move.y+1; j++){
                
                COORD neighbor = {i,j};          
                if(isValidMove(neighbor) &&(i!=move.x || j!=move.y)){ //Check that the square is a new uncleared square
                    updateBoard(neighbor);
                } 
            }
        }
    }
}
//***************************************************************
// Counts the number of adjacent mines
int countAdjacentMines(COORD move){
    int nbrOfAdjacentBombs =0;

    for(int i =0; i<boardSize; i++){
        for(int j =0; j<boardSize; j++){           
            if(minePos[i][j] == MINE){
                if(abs(i-move.x)<2 && abs(j-move.y)<2){  //if within range of move square
                    nbrOfAdjacentBombs++;
                } 
            }
        }
    }
    return nbrOfAdjacentBombs;
}
//***************************************************************
// Checks if a square coordinate is uncleared and within the board
bool isValidMove(COORD move){
    if(move.x >= 0 && move.x < boardSize && move.y >= 0 && move.y < boardSize && board[move.x][move.y] == UNCLEARED ){
        return true;
    }
    return false;
}
//***************************************************************
// Prints the current state of the board
void printBoard(){
    printf("\n   ");
    for(int i = 0; i<boardSize; i++){
        printf("%d ",i+1);
    }
    printf("\n");
    for(int i = 0; i<boardSize; i++){
        if(i<9){
            printf(" %d ",i+1);
        }else{
            printf("%d ",i+1);
        }
        for(int j = 0; j<boardSize; j++){
            printf("%c", board[i][j]);
            printf(" ");
        }
        if(i==2){
            printf("        Mines remaining: %d", nbrOfMines);
        }
        if(i==4){
            printf("        Uncleared squares: %d", squaresRemaining);
        }
        printf("\n");
    }
    printf("\n");   

    // DEVTOOL: For printing mine positions    
    // for(int i = 0; i<10; i++){
    //     for(int j = 0; j<10; j++){
    //         printf("%c", bombPos[i][j]);
    //         printf(" ");
    //     }
    //     printf("\n");
    // }
}
//***************************************************************
// Updates the board so that all squares are cleared and all mines are displayed
void revealBoard(){
    for(int i = 0; i<boardSize; i++){
        for(int j = 0; j<boardSize; j++){
            COORD pos = {i,j};
            if(checkForMine(pos)==MINE){
                board[i][j] = minePos[i][j];
            }
            else{
                updateBoard(pos);
            }
        }
    }
    squaresRemaining = 0;
}
//***************************************************************
void youWon(){
    printf("************************\n");
    printf("YOU WON! Congratulations  \n\n");
}
//***************************************************************
void youLost(){
    printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
    printf("YOU EXPLODED AND DIED! Game over  \n\n");
}
//***************************************************************