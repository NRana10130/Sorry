#ifndef GAME_H
#define GAME_H

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

void initialize_board();
//void print_board();
//void print_led(int num);
void populate_led_array();
int display_card();
int update_turn(int turn);
int sensor();
bool pos_occ(int pos, int turn);
void incorrect_move();
void home_move();
void win_move();
void zero_led(int start);
void print_player_pos();
bool valid_move(int check);
bool valid_lift(int check);
void increment_card(int card_num, int loc, int turn);
int increment_ret(int card_num, int loc, int turn);
void increment_lift(int card_num, int loc);
void decrement_card(int card_num, int loc, int turn);
void print_led(int num, int turn);

typedef struct {
   int start;
   int after_start;
   int before_stretch;
   int start_stretch;
   int home;
} pawn_t;

extern int pawns[4][4];
extern pawn_t pawns_init[4];
extern int turn;
extern bool endgame;
extern bool done[4];
extern int start[4];
extern int home[4];
extern int led[64];
extern int led_num;

#endif
