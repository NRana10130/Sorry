#include "game.h"
#include "LED.h"

pawn_t red =    {44,  0, 42, 48, 51};
pawn_t yellow = {45, 11,  9, 52, 55};
pawn_t green =  {46, 22, 20, 56, 59};
pawn_t blue =   {47, 33, 31, 60, 63};

int led_color[4] = {LED_RED, LED_YELLOW, LED_GREEN, LED_BLUE};

pawn_t pawns_init[4];
int led_num = 0;
int pawns[4][4] = {{44, 45, 46, 47},
                     {44, 45, 46, 47},
                     {44, 45, 46, 47},
                     {44, 45, 46, 47}};
int led[64];
extern int array[64];
bool done[4] = {false, false, false, false};
bool endgame = false;

int board_sensor[64] = {1,2,3,4,5,6,7,15,14,13,12,17,18,19,20,21,22,23,31,30,29,28,33,34,35,36,37,38,39,47,46,45,44,49,50,51,52,53,54,55,63,62,61,60,0,16,32,48,59,58,57,56,11,10,9,8,27,26,25,24,43,42,41,40};

void increment_card(int card_num, int loc, int turn) {
    int curr_loc;
    for (int j = 0; j < 4; j++) {
        curr_loc = pawns[j][turn];
        if (curr_loc == pawns_init[turn].start || curr_loc == pawns_init[turn].home)
            continue;
        else if ((curr_loc >= (pawns_init[turn].before_stretch-(card_num-1))) && (curr_loc <= (pawns_init[turn].before_stretch))) {
            if (!pos_occ(curr_loc+pawns_init[turn].start_stretch-pawns_init[turn].before_stretch+(card_num-1), turn))
                led[led_num++] = curr_loc+pawns_init[turn].start_stretch-pawns_init[turn].before_stretch+(card_num-1);
        } else if ((curr_loc >= (44-card_num) && curr_loc <= 43)) {
            if (!pos_occ(curr_loc-(44-card_num), turn))
                led[led_num++] = curr_loc-(44-card_num);
        } else if (!pos_occ(curr_loc+card_num, turn)) {
            led[led_num++] = curr_loc+card_num;
        }
    }
}

int increment_ret(int card_num, int loc, int turn) {
	/*
    int curr_loc;
    //for (int j = 0; j < 4; j++) {
        curr_loc = loc;
        if (curr_loc == pawns_init[turn].start || curr_loc == pawns_init[turn].home)
            continue;
        else if ((curr_loc >= (pawns_init[turn].before_stretch-(card_num-1))) && (curr_loc <= (pawns_init[turn].before_stretch))) {
            if (!pos_occ(curr_loc+pawns_init[turn].start_stretch-pawns_init[turn].before_stretch+(card_num-1), turn))
                led[led_num++] = curr_loc+pawns_init[turn].start_stretch-pawns_init[turn].before_stretch+(card_num-1);
        } else if ((curr_loc >= (44-card_num) && curr_loc <= 43)) {
            if (!pos_occ(curr_loc-(44-card_num), turn))
                led[led_num++] = curr_loc-(44-card_num);
        } else if (!pos_occ(curr_loc+card_num, turn)) {
            led[led_num++] = curr_loc+card_num;
        }
        */
        return led_num;
    //}
}


void decrement_card(int card_num, int loc, int turn) {
    int curr_loc;
    for (int j = 0; j < 4; j++) {
        curr_loc = pawns[j][turn];
        if (curr_loc == pawns_init[turn].start || curr_loc == pawns_init[turn].home)
            continue;
        else if ((curr_loc >= 0 && curr_loc < card_num)) {
            if (!pos_occ(curr_loc+(44-card_num), turn))
                led[led_num++] = curr_loc+(44-card_num);
        } else if ((card_num == 4) && (curr_loc >= (pawns_init[turn].start_stretch)) && (curr_loc < (pawns_init[turn].home))) {
            if (!pos_occ(curr_loc+pawns_init[turn].before_stretch-pawns_init[turn].start_stretch-(card_num-1), turn))
                led[led_num++] = curr_loc+pawns_init[turn].before_stretch-pawns_init[turn].start_stretch-(card_num-1);
        } else if ((card_num == 1) && (curr_loc == pawns_init[turn].start_stretch)) {
            if ((!pos_occ(pawns_init[turn].before_stretch, turn)))
                led[led_num++] = pawns_init[turn].before_stretch;
        } else if (!pos_occ(curr_loc-card_num, turn)) {
            led[led_num++] = curr_loc-card_num;
        }
    }
}

bool valid_lift(int check) {
    bool in_arr = 0;
    for (int i = 0; i < 4; i++) {
        if (check == led[i]) {
            in_arr = true;
            return in_arr;
        }
    }
    return in_arr;
}

bool valid_move(int check) {
    bool in_arr = 0;
    for (int i = 4; i < led_num; i++) {
        if (check == led[i]) {
            in_arr = true;
            return in_arr;
        }
    }
    return in_arr;
}

void zero_led(int start) {
    for (int i = start; i < 64; i++) {
        led[i] = 0;
    }
}

void print_player_pos() {
    int j = 0;
    for (int i = 0; i < 4; i++) {
        printf("Player %d: %d %d %d %d\n", j, pawns[0][i], pawns[1][i], pawns[2][i], pawns[3][i]);
        j++;
    }
}

void incorrect_move() {
    printf("Audio: Incorrect Move\n");
}

void home_move() {
    printf("Audio: Home\n");
}

void win_move() {
    printf("Audio: You Win!\n");
}

int sensor() {
    int pos;
    printf("Enter Sensor: ");
    scanf("%d", &pos);
    printf("Display Sensor: %d \n", pos);
    return pos;
}

bool pos_occ(int pos, int turn) {
    if (pos > pawns_init[turn].home)
        return true;
    for(int i = 0; i < 4; i++) {
        if (pawns[i][turn] == pos && pos < pawns_init[turn].home)
            return true;
    }
    return false;
}

int display_card() {
    int card;
    printf("Enter Card: ");
    scanf("%d", &card);
    printf("Display Card: %d \n", card);
    printf("Audio: Play Card\n");
    return card;
}

void initialize_board() {
    pawns_init[0] = red;
    pawns_init[1] = yellow;
    pawns_init[2] = green;
    pawns_init[3] = blue;
}

//Current pawn locations
void populate_led_array() {
    led_num = 0;
    LED_Init();
     for (int i = 0; i < 4; i++) {
         for (int j = 0; j < 4; j++) {
             bool found = false;
             for (int k = 0; k < led_num; k++) {
            	 //Check if led sequence already added to array
                 if(pawns[i][j] == led[k]) {
                     found = true;
                     break;
                 }
             }
             if (!found) {
            	 Set_LED_Color(pawns[i][j], led_color[j], 10);
            	 array[board_sensor[pawns[i][j]]] = 0;
                 led[led_num] = pawns[i][j];
                 led_num++;
             }
         }
     }
}

int update_turn(int turn) {
    if (turn == 3)
      turn = 0;
    else
      turn++;

    for (int i = 0; i < 4; i++) {
        if (!done[turn])
            return turn;
        else if (turn == 3)
            turn = 0;
        else
            turn++;
    }
    endgame = 1;
    return turn;
}

void print_led(int num, int turn) {
	LED_Init();
    for (int i = 0; i < num; i++) {
    	Set_LED_Color(led[i], led_color[turn], 10);
        if (i < 4)
        	Set_LED_Off(led[i], led_color[turn], 10);
        if (i >= 4 && led[i] == pawns_init[turn].home)
        	Set_LED_Off(led[i], LED_OFF, 10);
    }
    led_status = 2;
}
