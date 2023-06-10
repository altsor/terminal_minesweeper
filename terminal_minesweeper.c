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
#define nbrOfMines 9
#define boardSize 9

// Variables
char board[boardSize][boardSize];
char minePos[boardSize][boardSize];
int squaresRemaining;
int nbrFlagsPlaced;

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
    nbrFlagsPlaced = 0;
    setupBoard();
    printBoard();

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
    printf("\nThanks for playing\n\n");
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

        int input1;
        int input2;
        char inChar;
        
        printf("** Enter F to place a flag, enter Q to exit the game **\n\n");   
        printf(" Enter row number to make a move:  "); 

        if (scanf("%d", &input1)) {   
        } else {  
            //If the user enters a character instead of an integer, the scanf call inside the if condition will fail 
            //to read an integer, and it will return 0 (indicating that 0 items were successfully scanned).
            //In the case where a character is entered, it remains in the input buffer after the failed scanf call.
            //credit ChatGTP
            scanf(" %c", &inChar);  // Added space before %c to skip whitespace
        } //
       
        if (inChar == 'Q' || inChar  == 'q') {  //Exits the game if Q is entered
            printf(" Game exited. Thanks for playing!\n");
            exit(0);

        } else if(inChar  == 'F' || inChar  == 'f'){  //Enters flag placing mode if F is entered
            placeFlag();
            printBoard();
            move = playerMove();  //Continue the normal player move mode after flagmode is exited
            break;

        } else {  // If an integer is entered
            move.x = input1-1;

            while ((getchar()) != '\n');  //Clear stdin buffer

            printf(" Enter column number:  ");
            scanf("%d", &input2); 
            while ((getchar()) != '\n');  //Clear stdin buffer
            move.y = input2-1;

            if(isValidMove(move)){
                break;
            }
            else{     
                printf("\n Invalid move! \n");                  
            }
        }     
    } 
    return move;
}
//***************************************************************
// Places a flag on user defined square
void placeFlag(){

    int input1;
    int input2;
    char inChar;
    COORD flagMove;

    printf("\n******* FLAG PLACEMENT MODE ******* \n");
    printf("Use negative numbers to remove flags\n\n");
    printBoard();

    while(true){

        while ((getchar()) != '\n'); //Clear stdin buffer
        printf("[Flag mode] Enter row number (F to exit flag mode):  "); 

        if (scanf("%d", &input1)) {  
        } else {
            scanf(" %c", &inChar);  // Added space before %c to skip whitespace
        }            

        if(inChar == 'F' || inChar == 'f'){  //Exits flag placing mode if F is entered
            return;

        } else {  // If integer is entered
            flagMove.x = abs(input1)-1;

            while ((getchar()) != '\n'); //Clear stdin buffer
            printf("[Flag mode] Enter column number:  ");
            scanf("%d", &input2);

            flagMove.y = abs(input2)-1;
            
            if(isValidMove(flagMove)){
                if(input1<0 || input2<0){  //If user entered negative integers flags are removed
                    if(board[flagMove.x][flagMove.y] == 'P'){
                        board[flagMove.x][flagMove.y] = UNCLEARED;
                        nbrFlagsPlaced--;
                    }
                }else if (board[flagMove.x][flagMove.y] = UNCLEARED){  // Flag is placed
                    board[flagMove.x][flagMove.y] = 'P';
                    nbrFlagsPlaced++;
                }
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

    int nbrOfAdjacentMines = countAdjacentMines(move); //Counts number of adjacent mines
 
    if(nbrOfAdjacentMines>0){
        board[move.x][move.y] = nbrOfAdjacentMines+'0';  //Square is set to number of adjacent mines
        squaresRemaining--;
       
    }else{
        board[move.x][move.y] = CLEARED; // If no mines, square is set to empty space
        squaresRemaining--;

        //Update adjacent squares by going over the 8 neighboring squares and calling this function for each
        for(int i =move.x-1; i<=move.x+1; i++){
            for(int j =move.y-1; j<=move.y+1; j++){
                
                COORD neighbor = {i,j};     
                //Checks that the neighbor square is a valid uncleared square that is not the current one.    
                if(isValidMove(neighbor) &&(i!=move.x || j!=move.y)){ 
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
                if(abs(i-move.x)<2 && abs(j-move.y)<2){  //if within range of current square
                    nbrOfAdjacentBombs++;
                } 
            }
        }
    }
    return nbrOfAdjacentBombs;
}
//***************************************************************
// Checks if a square is uncleared and within the board
bool isValidMove(COORD move){
    if(move.x >= 0 && move.x < boardSize && move.y >= 0 && move.y < boardSize && (board[move.x][move.y] == UNCLEARED || board[move.x][move.y] == 'P')){
        return true;
    }
    return false;
}
//***************************************************************
// Prints the current state of the board
void printBoard(){
    printf("\n  ");
    if(boardSize>9){printf(" ");}

    for(int i = 0; i<boardSize; i++){  //print column numbers on first row
        if(i<9 && boardSize > 9){
            
            printf("%d  ",i+1);
        } else{
            printf("%d ",i+1);
        }
    }
    printf("\n");
    for(int i = 0; i<boardSize; i++){  // For each row
        if(i<9 && boardSize > 9){
            printf(" %d ",i+1);
        }else{
            printf("%d ",i+1);
        }
        for(int j = 0; j<boardSize; j++){  //For each column
            if(boardSize > 9){
                printf("%c  ", board[i][j]);
            }
            else{
                printf("%c ", board[i][j]);
            }           
        }
        if(i==2){
            printf("        Mines remaining: %d", nbrOfMines);
        }
        if(i==4){
            printf("        Flags placed: %d", nbrFlagsPlaced);
        }
        if(i==6){
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