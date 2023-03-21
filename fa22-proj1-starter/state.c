#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t* state, unsigned int snum);
static char next_square(game_state_t* state, unsigned int snum);
static void update_tail(game_state_t* state, unsigned int snum);
static void update_head(game_state_t* state, unsigned int snum);
game_state_t * initialize_game_state();
void initialize_snakes_and_fruits(game_state_t *);

/* Task 1 */
game_state_t* create_default_state() {
  game_state_t *default_game = initialize_game_state();
  initialize_snakes_and_fruits(default_game);
  return default_game;
}

game_state_t * initialize_game_state() {
  game_state_t *default_state = calloc(1, sizeof(game_state_t));

  //initialize board
  default_state->num_rows = 18;
  default_state->num_snakes = 1;
  default_state->board = calloc(default_state->num_rows, sizeof(char*));
  default_state->snakes = calloc(default_state->num_snakes, sizeof(snake_t));
  unsigned int num_cols = 20;
  for (int i = 0; i < default_state->num_rows; i++) {
    default_state->board[i] = calloc(num_cols + 1, sizeof(char));
    char *row = default_state->board[i];
    for (int j = 1; j < num_cols - 1; j++) {
       if (i == 0 || i == default_state->num_rows - 1) {
        row[j] = '#';
       } else {
        row[j] = ' ';
       }
    }
    row[0] = '#';
    row[num_cols - 1] = '#';
    row[num_cols] = '\0';
  }
  return default_state;
}

void initialize_snakes_and_fruits(game_state_t *default_state) {
  //initialize snake and fruit
  snake_t *snake = default_state->snakes;
  snake->tail_row = 2;
  snake->tail_col = 2;
  snake->head_row = 2;
  snake->head_col = 4;
  snake->live = true;
  default_state->board[2][2] = 'd';
  default_state->board[2][3] = '>';
  default_state->board[2][4] = 'D';
  default_state->board[2][9] = '*';
}

/* Task 2 */
void free_state(game_state_t* state) {
  // free snakes
  free(state->snakes);
  // free board rows
  for (int i = 0; i < state->num_rows; i++) {
    free(state->board[i]);
  }
  // free board
  free(state->board);
  // free state
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  if (state == NULL) {
    return;
  }
  for (int i = 0; i < state->num_rows; i++) {
    fprintf(fp, "%s\n", state->board[i]);
  }
  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t* state, unsigned int row, unsigned int col) {
  return state->board[row][col];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch) {
  state->board[row][col] = ch;
}

static bool contains_ch(char* chs, size_t len, char c) {
  for (int i = 0; i < len; i++) {
    if (c == chs[i])
      return true;
  }
  return false;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  char tails[4] = {'a', 's', 'w', 'd'};
  return contains_ch(tails, 4, c);
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  char heads[5] = {'A', 'S', 'W', 'D', 'x'};
  return contains_ch(heads, 5, c);
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  char chs[13] = {'a', 's', 'w', 'd', '^', '<', 'v', '>','A', 'S', 'W', 'D', 'x'};
  return contains_ch(chs, 13, c);
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  char bodies[4] = {'<', 'v', '^', '>'};
  char tails[4] = {'a', 's', 'w', 'd'};
  for (int i = 0; i < 4; i++) {
    if (c == bodies[i])
      return tails[i];
  }
  return '\0';
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  char heads[4] = {'A', 'S', 'W', 'D'};
  char bodies[4] = {'<', 'v', '^', '>'};
  for (int i = 0; i < 4; i++) {
    if (c == heads[i])
      return bodies[i];
  }
  return '\0';
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  if (c == 'v' || c == 's' || c =='S') {
    return cur_row + 1;
  }
  if (c == '^' || c == 'w' || c =='W') {
    return cur_row - 1;
  }
  return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  if (c == '>' || c == 'd' || c =='D') {
    return cur_col + 1;
  }
  if (c == '<' || c == 'a' || c =='A') {
    return cur_col - 1;
  }
  return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t* state, unsigned int snum) {
  unsigned int col = state->snakes[snum].head_col;
  unsigned int row = state->snakes[snum].head_row;
  char cur_ch = get_board_at(state, row, col);
  unsigned int next_col = get_next_col(col, cur_ch);
  unsigned int next_row = get_next_row(row, cur_ch);
  return get_board_at(state, next_row, next_col);
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t* state, unsigned int snum) {
  unsigned int col = state->snakes[snum].head_col;
  unsigned int row = state->snakes[snum].head_row;
  char cur_ch = get_board_at(state, row, col);
  unsigned int next_col = get_next_col(col, cur_ch);
  unsigned int next_row = get_next_row(row, cur_ch);
  set_board_at(state, next_row, next_col, cur_ch);
  set_board_at(state, row, col, head_to_body(cur_ch));
  state->snakes[snum].head_col = next_col;
  state->snakes[snum].head_row = next_row;
  return;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t* state, unsigned int snum) {
  unsigned int col = state->snakes[snum].tail_col;
  unsigned int row = state->snakes[snum].tail_row;
  char cur_ch = get_board_at(state, row, col);
  unsigned int next_col = get_next_col(col, cur_ch);
  unsigned int next_row = get_next_row(row, cur_ch);
  set_board_at(state, next_row, next_col, 
               body_to_tail(get_board_at(state, next_row, next_col)));
  set_board_at(state, row, col, ' ');
  state->snakes[snum].tail_col = next_col;
  state->snakes[snum].tail_row = next_row;
  return;
}


/* Task 4.5 */
/*
Each snake moves one step in the direction of its head.
If the head crashes into the body of a snake or a wall, the snake dies and stops moving. 
When a snake dies, the head is replaced with an x.
If the head moves into a fruit, the snake eats the fruit and grows by 1 unit in length. 
(You can implement growing by 1 unit by updating the head without updating the tail.) 
Each time fruit is consumed, a new fruit is generated on the board.
*/

/* move snake one step further, return if it consumes a fruit*/
bool move_snake(game_state_t* state, unsigned int snum) {
  snake_t *cur_snake = &state->snakes[snum];
  char next_char = next_square(state, snum);
  bool consume_fruit = false;
  if (next_char == '#') {
    // wall
    set_board_at(state, cur_snake->head_row, cur_snake->head_col, 'x');
    cur_snake->live = false;
  } else if (next_char == '*') {
    update_head(state, snum);
    consume_fruit = true;
  } else if (next_char == ' ') {
    update_head(state, snum);
    update_tail(state, snum);
  } else {
    // crossing a snake
    set_board_at(state, cur_snake->head_row, cur_snake->head_col, 'x');
    cur_snake->live = false;
  }
  return consume_fruit;
}

void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  for (unsigned int i = 0; i < state->num_snakes; i++) {
    if (move_snake(state, i)) {
      add_food(state);
    }
  }
  return;
}


/* Task 5 */
game_state_t* load_board(char* filename) {
  unsigned long len = 0;
  game_state_t *state = malloc(sizeof(game_state_t));

  // get length of the content
  FILE *file;
  file = fopen(filename, "r");
  if (file == NULL) {
      printf("file can't be opened \n");
      return NULL;
  }
  while (!feof(file)) {
    fgetc(file);
    len++;
  }
  fclose(file);

  // copy file content to string, get num of rows
  file = fopen(filename, "r");
  char* content = calloc(len, sizeof(char));
  unsigned int num_rows = 0;
  for (int i = 0; i < len - 1; i++) {
    content[i] = (char) fgetc(file);
    if (content[i] == '\n') {
      content[i] = '\0';
      num_rows++;
      if (i == len - 2) {
        num_rows--;
      }
    }
  }
  num_rows++;
  
  // set the board
  state->num_rows = num_rows;
  state->board = malloc(num_rows * sizeof(char*));
  char *cur_ptr = content;
  unsigned int i = 0;
  while (cur_ptr < content + len - 1) {
    unsigned long str_len = strlen(cur_ptr);
    state->board[i] = calloc(str_len + 1, sizeof(char));
    strcpy(state->board[i], cur_ptr);
    cur_ptr += str_len + 1;
    i++;
  }
  return state;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t* state, unsigned int snum) {
  unsigned int col = state->snakes[snum].tail_col;
  unsigned int row = state->snakes[snum].tail_row;
  unsigned int next_col = col;
  unsigned int next_row = row;
  char cur_ch = get_board_at(state, next_row, next_col);
  while (!is_head(cur_ch)) {
    next_col = get_next_col(next_col, cur_ch);
    next_row = get_next_row(next_row, cur_ch);
    cur_ch = get_board_at(state, next_row, next_col);
  }
  state->snakes[snum].head_row = next_row;
  state->snakes[snum].head_col = next_col;
  return;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {

  // get num_of_snakes
  unsigned int num_snakes = 0;
  for (int i = 0; i < state->num_rows; i++) {
    for (int j = 0; j < strlen(state->board[i]); j++) {
      if (is_tail(state->board[i][j])) {
        num_snakes++;
      }
    }
  }
  state->num_snakes = num_snakes;

  // set up the snakes array
  state->snakes = malloc(num_snakes * sizeof(snake_t));
  unsigned int cur_snake = 0;
  for (unsigned int i = 0; i < state->num_rows; i++) {
    for (unsigned int j = 0; j < strlen(state->board[i]); j++) {
      if (is_tail(state->board[i][j])) {
        state->snakes[cur_snake].live = true;
        state->snakes[cur_snake].tail_row = i;
        state->snakes[cur_snake].tail_col = j;
        find_head(state, cur_snake);
        cur_snake++;
      }
    }
  }
  return state;
}
