#include <stdio.h>
#include <string.h>

#define BOARDSIZE 64

long long shift = 1;
long long solutions[1024];
int solution = 0;

void print_board(long long *board){
  for(int i = 0; i < BOARDSIZE; i++){
    printf("%d ", (*board & (shift << i)) ? 1 : 0);
    if (i%8 == 7) printf("\n");
  }
  printf("\n");
}

int check_playable(long long *board, int place){
  //check row
  for (int pos = (place - (place % 8)); pos < place + (7 - (place % 8)); pos++)
    if (*board & (shift << pos)) return 0;

  //check column
  for (int pos = place % 8; pos < BOARDSIZE; pos += 8)
    if (*board & (shift << pos)) return 0;

  //move up-left and check down-right diagonal
  for (int pos = (place - (place%8)) - (8 * (place%8)); pos < 63; pos += 9)
    if (*board & (shift << pos)) return 0;

  //move up-right and move down-left diagonal
  for (int pos = (place + (7 - (place%8)) - (8 * (7 - (place%8)))); pos < 63; pos += 7)
    if (*board & (shift << pos)) return 0;

  return 1;
}

int check_prev(long long *board, int place){
  if (place <= 15) return 1;
  for (int pos = (place - (place % 8) - 8); pos < place + (7 - (place % 8)); pos++)
    if (*board & (shift << pos)) return 1;
  return 0;
}

int generate_permutation(long long *board, int queens, int place){
  if (queens == 8){
    solutions[solution++] = *board ^ (shift << place);
    return 1;
  }
  else if (!check_prev(board, place)) return 0; //if previous line not played

  long long *temp;
  bcopy(&board, &temp, 8);
  *temp ^= (shift << place); //place queen

  //itterate through board spaces to test if locations are playable
  for (int position = (place - (place%8)) + 8; position < BOARDSIZE; position++){
    if (!check_prev(temp, position)) return 0; //if previous line not played
    if (check_playable(temp, position))
      if (!generate_permutation(temp, queens + 1, position))
        *temp ^= (shift << position); //unset placed queen
  }
  return 0;
}

int main() {
  long long board = 0;
  for (int start_pos = 0; start_pos < 8; start_pos++){
    generate_permutation(&board, 1, start_pos); //permute and set queens to 1
    board ^= (shift << start_pos); //unset initial queen
  }

  printf("Solutions found: %d\n", solution);
  for (int i = 0; i < solution; i++){
    printf("Solution %d: %lld\n", i + 1, solutions[i]);
    print_board(&solutions[i]);
  }
  return 0;
}
