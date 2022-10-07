#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef char board_t[3][3];

struct move{
  int row;
  int col;
};

struct minimaxReturn{
  int score;
  int numTurns;
};

//return X if x wins, O if o wins, T for tie, and _ for no winner yet
char check_wins(board_t board){

//check row
    int xCount = 0;
    int oCount = 0;
    
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(board[i][j] == 'O'){
                oCount ++;
            }else if(board[i][j] == 'X'){
                xCount ++;
            }            
        }
        if(xCount == 3){
            return 'X';
        }else if(oCount == 3){
            return 'O';
        }
        xCount = 0;
        oCount = 0;
    }

//check col
    int xCount2 = 0;
    int oCount2 = 0;
    
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(board[j][i] == 'O'){
                oCount2 ++;
            }else if(board[j][i] == 'X'){
                xCount2 ++;
            }            
        }
        if(xCount2 == 3){
            return 'X';
        }else if(oCount2 == 3){
            return 'O';
        }

        xCount2 = 0;
        oCount2 = 0;
    }

//check diagonals 
    int xCount3 = 0;
    int oCount3 = 0;

    for(int i = 0; i < 3; i++){
        if(board[i][i] == 'O'){
            oCount3 ++;
        }else if(board[i][i] == 'X'){
            xCount3 ++;
        }
    }
    if(xCount3 == 3){
        return 'X';
    }else if(oCount3 == 3){
        return 'O';
    }

    xCount3 = 0;
    oCount3 = 0;

    int i = 0;
    int j = 2;

    while(i < 3 && j >= 0){
        if(board[i][j] == 'O'){
            oCount3 ++;
        }else if(board[i][j] == 'X'){
            xCount3 ++;
        }
        j--;
        i++;
    }

    if(xCount3 == 3){
        return 'X';
    }else if(oCount3 == 3){
        return 'O';
    }

//if there are any empty spaces, no winner yet
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(board[i][j] == '_'){
                return '_';
            }
        }
    }

//if all spaces full and no winner, return T
    return 'T';
}


void player_move(board_t board){
  int row;
  int col;
  bool notValid = true;
  
  while (notValid){
    printf("Enter your move: \n");
    scanf("%d %d", &row, &col);
  //if row and col between 0 and 2 and place is empty 
    if(row < 3 && row >= 0 && col < 3 && col >= 0 && board[row][col] == '_'){
      board[row][col] = 'X';
      notValid = false; 
    }else{
      printf("Invalid move\n");
    }
  }
}

//if o wins score=1, x wins score=-1, tie score=0
struct minimaxReturn * minimax(board_t board, bool isO, int turns){
  struct minimaxReturn * toReturn = malloc(sizeof(struct minimaxReturn));
    if(!toReturn){
        printf("failed to malloc(struct inventory)\n");
        exit(-1);
    }
  char done = check_wins(board);

//base case: someone won or the board is full
  if(done != '_'){
//if o won, return 1, else 0
    if(done == 'O'){
      toReturn -> score = 1;
      toReturn -> numTurns = turns;
      return toReturn;
    }else if (done == 'X'){
      toReturn -> score = -1;
      toReturn -> numTurns = turns;
      return toReturn;
    }else{
      toReturn -> score = 0;
      toReturn -> numTurns = turns;
      return toReturn;
    }
  }
  
//if is O's turn
  if(isO){
    int largestScore = -100;
    int theTurns;
    int score;
    for(int i = 0; i < 3; i++){
      for(int j = 0; j < 3; j++){
  //for every choice, place O there, call minimax
        if(board[i][j] == '_'){
          board[i][j] = 'O';
          struct minimaxReturn * theScore = minimax(board, false, turns + 1);
          board[i][j] = '_';
          if(theScore -> score > largestScore){
            largestScore = theScore -> score;
            theTurns = theScore -> numTurns;
          }
          free(theScore);
        }
      }
    }
  //return the best score for the bot
      toReturn -> score = largestScore;
      toReturn -> numTurns = theTurns;
      return toReturn;
  }
    
//if is X's turn 
  else{
    //human players best score is -1/the smallest
    int smallestScore = 100;
    int theTurns;
    int score;
    for(int i = 0; i < 3; i++){
      for(int j = 0; j < 3; j++){
  //for every choice, place X there, call minimax
        if(board[i][j] == '_'){
          board[i][j] = 'X';
          struct minimaxReturn * theScore = minimax(board, true, turns + 1);
          board[i][j] = '_';
          if(theScore -> score < smallestScore){
            smallestScore = theScore -> score;
            theTurns = theScore -> numTurns;
          }
          free(theScore);
        }
      }
    }
  //return the best score for the human player
      toReturn -> score = smallestScore;
      toReturn -> numTurns = theTurns;
      return toReturn;
  }
}

struct move * bot_move(board_t board){
  
  struct move *play = malloc(sizeof(struct move));
    if(!play){
        printf("failed to malloc(struct inventory)\n");
        exit(-1);
    }

//if never been set, largest score = -100
  int largestScore = -100;
  int minTurns;
  int score;
  int row;
  int col;
  
//for every open space
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
//place O there, call minimax
      if(board[i][j] == '_'){
        board[i][j] = 'O';
        struct minimaxReturn * theScore = minimax(board, false, 0);
//reset board
        board[i][j] = '_';
//if best play so far, store values
        if(theScore -> score > largestScore || largestScore == -100){
          largestScore = theScore -> score;
          row = i;
          col = j;
          minTurns = theScore -> numTurns;
        }else if(theScore -> score == largestScore){
          if(theScore -> numTurns < minTurns){
            row = i;
            col = j;
            minTurns = theScore -> numTurns;
          }
        }
        free(theScore);
      }
    }
  }
  
//return the best play 
  play -> row = row;
  play -> col = col;
  return play; 
}



void print(board_t board){

    for(int i = 0; i < 2; i ++){
        printf(" %c |", board[0][i]);
    }
    printf(" %c \n", board[0][2]);
    printf("---+---+---\n");
    
    for(int i = 0; i < 2; i ++){
        printf(" %c |", board[1][i]);
    }    
    printf(" %c \n", board[1][2]);
    printf("---+---+---\n");
    
    for(int i = 0; i < 2; i ++){
        printf(" %c |", board[2][i]);
    }
    printf(" %c \n", board[2][2]);
    
}


int main(int argc, char *argv[]) {

    board_t board;

  if(argc == 1){
    
    int turn = 0;
  //x if even, o if odd
    char player;
      
  //make board blank 
      for(int i = 0; i < 3; i ++){
        for(int j = 0; j < 3; j++){
          board[i][j] = '_';
        }
      }
      print(board);
      
  //while game ongoing
      while(check_wins(board) == '_'){
        if(turn % 2 == 0){
          player = 'X';
        }else{
          player = 'O';
        }
    
        if(player == 'X'){
          player_move(board);
          print(board);
        }else{
          printf("Computer move-\n");
          struct move *play = bot_move(board);
          board[play -> row][play -> col] = 'O';
          free(play);
          print(board);
        }
        turn++;
      }
      char winner = check_wins(board);
      if(winner == 'T'){
            printf("draw\n");
        }else if(winner == 'X'){
            printf("X wins\n");
        }else if(winner == 'O'){
            printf("O wins\n");
      }
  }

//if argc > 2
    //find where o should go
  else{
  //fill board
      int i = 1;
      for(int row = 0; row < 3; row++){
          for(int col = 0; col < 3;  col ++){
              board[row][col] = argv[i][0];
              i++;
          }
      }
      print(board);
//find the best move
    struct move * play = bot_move(board);
    printf("O: %d %d\n", play -> row, play -> col);
    free(play); 
  }
}
