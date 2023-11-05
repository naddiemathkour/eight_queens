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
  //move up-left and check down-right diagonal
  for (int pos = (place - (place%8)) - (8 * (place%8)); pos < 63; pos += 9)
    if (*board & (shift << pos)) return 0;

  //move up-right and move down-left diagonal
  for (int pos = (place + (7 - (place%8)) - (8 * (7 - (place%8)))); pos < 63; pos += 7)
    if (*board & (shift << pos)) return 0;

  return 1;
}

void build_board(int *positions){
  long long board = 0;
  int position;
  for (int i = 0; i < 8; i++){
    position = (8 * i) + positions[i];
    if (check_playable(&board, position))
      board ^= (shift << position);
    else return;
  }
  solutions[solution++] = board;
}

void generate(int k, int *positions){ //heap's algorithm: https://en.wikipedia.org/wiki/Heap%27s_algorithm
  if (k == 1){
    //test permutation
    build_board(positions);
  }
  else {
    generate(k - 1, positions);

    for (int i = 0; i < k - 1; i++){
      int temp;
      if (k%2){
        //swap pos[0] with pos[k-1] if k is odd
        temp = positions[0];
        positions[0] = positions[k-1];
        positions[k-1] = temp;
      } else {
        //swap pos[i] with pos[k-1] if k is even
        temp = positions[i];
        positions[i] = positions[k-1];
        positions[k-1] = temp;
      }
      generate(k - 1, positions);
    }
  }
}

int main() {
  long long board = 0;
  int queen_positions[8] = {0, 1, 2, 3, 4, 5, 6, 7};

  generate(8, queen_positions);

  printf("Solutions found: %d\n", solution);

  for (int i = 0; i < solution; i++){
    printf("Solution %d: %lld\n", i + 1, solutions[i]);
    print_board(&solutions[i]);
  }

  return 0;
}
