// intensity.c
// Assignment 2, COMP1511 18s1: Intensity
//
// This program by Aven Au (z5208734) on 23/04/2018
//
// Version 1.0.1: Minor changes to wording of comments.
// Version 1.0.0: Assignment released.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

// SOME USEFUL #defines - YOU WILL NEED MORE

#define ACTION_PLAYER_NAME    0
#define ACTION_DISCARD        1
#define ACTION_PLAY_CARD      2
#define ACTION_UNIT_TESTS     3

#define N_CARDS              40
#define N_CARDS_INITIAL_HAND 10
#define N_PLAYERS             4
#define N_CARDS_DISCARDED     3
#define N_CARDS_PER_TURN      4
#define LARGEST_SECOND_DIGIT  9

#define CARD_MIN             10
#define CARD_MAX             49


// ADD EXTRA #defines HERE

void print_player_name(void);
void choose_discards(void);
void choose_card_to_play(void);
void run_unit_tests(void);


// ADD PROTOTYPES FOR YOUR FUNCTIONS HERE
void deck(int deck_array[], int cards_left);
void print(int value);
int max(int array[], int number_of_cards);
void debugger(int cards_played, int position, int value);
int max_array(int array[], int number_of_cards);
int firstroundcardprint(int cards_on_hand[], int cards_left);
void stage1_discard (int card_48, int card_49, int cards_picked, int card_print[]);
int max_second_digit (int board_now[], int largest_number);
int min_array(int array[], int number_of_cards);
// You should not need to change this main function

int main(void) {

    int which_action = 0;
    scanf("%d", &which_action);
    if (which_action == ACTION_PLAYER_NAME) {
        print_player_name();
    } else if (which_action == ACTION_DISCARD) {
        choose_discards();
    } else if (which_action == ACTION_PLAY_CARD) {
        choose_card_to_play();
    } else {
        run_unit_tests();
    }

    return 0;
}

void print_player_name(void) {
    // CHANGE THIS PRINTF TO YOUR DESIRED PLAYER NAME
    printf("Tony did a shoey");

}

void choose_discards() {
	 //Array for cards
	int card[N_CARDS_INITIAL_HAND] = {0};
	 //Counter
	int x = 0;
	 //Basically a buffalo checker, when 1 there is a buffalo
	int buffalo = 0;
	 //When card_48 or 49 is equal to 1, 48 or 49 is in your hand
	int card_48 = 0;
	int card_49 = 0;
	 // Count how many cards were picked
	int cards_picked = 0;

	 //Adding one so the card print later in the code is more readable
	int card_print[N_CARDS_DISCARDED + 1] = {0};
	int i = 0;
	 //Checks whether the cards is already picked yet
	int flag[N_CARDS_INITIAL_HAND] = {0};
	
	//This is a card scanner, collecting all the values into an array	
	for (x = 0; x < N_CARDS_INITIAL_HAND; x++) {
		scanf("%d", &card[x]);
		if (card[x] == 47) {
			buffalo = 1; //Show that there is buffalo in deck
		} else if (card[x] == 48) {
			card_48 = 1; //Shows that 48 is in deck
		} else if (card[x] == 49) {
			card_49 = 1; //Shows that 49 is in deck	
		}
		i++;
	}	

	int four_length = i;
	i = 0;

	/*If the player have buffalo it is only worth keeping it if the player dont 
	have both 48 and 49 as they are the only cards to stop your gaining points */
	if (buffalo == 1) {
		if (four_length == 1) {
			cards_picked++;
			card_print[cards_picked] = 47;
		} else if (card_48 != 0 && card_49 != 0) {
			cards_picked++;
			card_print[cards_picked] = 47;
			//Since we are discarding 47, 48 and 49 must be discard to eliminate
			// any most chances from winning buffalo round
			cards_picked++;
			card_print[cards_picked] = 48;
			cards_picked++;
			card_print[cards_picked] = 49;
		} else if (card_48 == 1 || card_49 == 1) {
			cards_picked++;
			card_print[cards_picked] = 47;
			if (card_48 == 1) {
				cards_picked++;
				card_print[cards_picked] = 48;
			} else if (card_49 == 1) {
				cards_picked++;
				card_print[cards_picked] = 49;
			} 
		}
		
	//If you dont have buffalo 48 and 49 must be eliminated so you dont win 
	// buffalo rounds
	} else if (buffalo == 0) { 
		if (card_48 == 1 && card_49 == 1) {
			cards_picked++;
			card_print[cards_picked] = 48;
			cards_picked++;
			card_print[cards_picked] = 49;
		} else if (card_48 == 1) {
			cards_picked++;
			card_print[cards_picked] = 48;
		} else if (card_49 == 1) {
			cards_picked++;
			card_print[cards_picked] = 49;
		}
	}
	int second_digit = LARGEST_SECOND_DIGIT;
	//This is basically a loop that discards cards with high second digits and
	// only executed when less than 3 cards are picked
	while (second_digit > 0 && cards_picked < 3) {	
		x = 0;
		while (x < 10 && cards_picked < 3) {
			if ((card[x] != 47 && card[x] != 48 && card[x] != 49) && card[x] 
					% 10 > second_digit && flag[x] != 1) {						
				cards_picked++;
				card_print[cards_picked] = card[x];
				flag[x] = 1; //Marks the card so it cannot be repicked
			}
			x++;		
		}
		second_digit = second_digit - 1;
	} 
	//This is a backup card discarder just in case the rest of conditions dont
	// match which guarentees legal play
	while (x < 10 && cards_picked < 3) {
		if (card[x] != 47 || card[x] != 48 || card[x] != 49) {
			cards_picked++;
			card_print[cards_picked] = card[x];
		}
		x++;	
	}	
	//Print out all the cards that have been selected
	printf("%d %d %d", card_print[1], card_print[2], card_print[3]);

}

void choose_card_to_play(void) {
	//Card Counter
	int x = 0;
	//Same First Digit array counter
	int i = 0;
	// Number of Cards Left in hand
	int cards_left = 0;
	//What table position the player is in
	int table_position = 0;
	//How many cards have been played already this round
	int cards_played = 0;
	//Array for cards that are currently in player's hand
	int cards_on_hand[N_CARDS_INITIAL_HAND] = {0};

	//Array of cards with same first digit
	int same_first_digit[N_CARDS_INITIAL_HAND] = {0};
	//When card_47 is 0 there is no 47, if it is 1, the card is in the deck
	int card_47 = 0;
	//Check the max_value of the board
	int max_value = 0;
		
	//Basically Scans any information that is needed
	scanf("%d", &cards_left);
	scanf("%d", &cards_played);
	scanf("%d", &table_position);

	//Array for all cards on current board
	int board_now[N_CARDS_INITIAL_HAND] = {0};
	//Array for all cards on last board
	int board_last[N_CARDS_PER_TURN] = {0};
	//Array for cards that is in the 40s
	int four_first_digit[N_CARDS_INITIAL_HAND] = {0};
	int three_first_digit[N_CARDS_INITIAL_HAND] = {0};
	
	// Created a deck() function which automatically scans the values and put them
	// into arrays while also check if buffalo is in hand
	for (int x = 0; x < cards_left; x++){
		scanf("%d", &cards_on_hand[x]);
		if (cards_on_hand[x] == 47) {
			card_47 = 1;
		}
	}
	//Put all cards that is on the board into arrays
	deck(board_now, cards_played);
	//Put all cards that is played last game into arrays
	deck(board_last, N_CARDS_PER_TURN);


	/*The general strategy is to play very defensive and play the biggest cards
	possible without losing any points. Only time, the bot is agressive is when
	they dont have cards that have the same first digit as the first guy on the
	board. */
	

	//Check and put all 40s cards into arrays
	for (x = 0; x < cards_left; x++) {
		if (cards_on_hand[x] / 10 == 4) {
			four_first_digit[i] = cards_on_hand[x];
			i++; //Counter for how many cards in the 40s there is
		}
	}

	//When cards_played == 0 it means the player is first up	
	if (cards_played == 0) {
		/*Not much strategy in choosing first card. Playing first position is the 
		worst position to be in so lowest value cards are being played*/

		//Choosing legal first hand
		int chosen_card = firstroundcardprint(cards_on_hand, cards_left);
		print(chosen_card);
	//When you are not first up
	} else if (cards_played > 0) {	
		i = 0;
		//Put all legal cards into array
		for (x = 0; x < cards_left; x++) {
			if (cards_on_hand[x] / 10 == board_now[0] / 10) {
				same_first_digit[i] = cards_on_hand[x];
				i++; 
			}
		}
		 //This is the number of cards with same first digit
		int length = i;
		
		//Find max value of second digit
		max_value = max_second_digit (board_now, N_CARDS_PER_TURN);

		// If i > 0 it means that there must be more cards with same first digit
		if (i > 0) {
			//Placing 47 only when there is a number bigger than 47
			if (board_now[0] / 10 == 4 && card_47 == 1 && cards_played == 3 &&
 					(board_now[0] > 47 || board_now[1] > 47 || board_now[2] > 47)){
				printf("47");
			} else {
				//Picking the largest number that is bigger than the max value
				//On the board
				while (i > 0 && same_first_digit[i - 1] > max_value) {
					i = i - 1;
				}
				// If i > 0 it means there is a number smaller than max_value
				if (i > 0){	
					print(same_first_digit[i - 1]);
				// There is no card that is bigger than max_value
				} else {
					//Last play of the round
					if (cards_played == 3  && same_first_digit[length - 1] < 47) {
						//Print the largest card possible because the player 
						//is losing 100%
						print(same_first_digit[length - 1]);
					//Not play 47s by accident
					} else if (length > 1 && same_first_digit[0] == 47) {
						print(same_first_digit[1]);
					} else {
						print(same_first_digit[0]);
					}
				}
			}
		/* This is when the player does not have any cards with the same first
		digit. This usually means players can be aggressive as there are no
		consequences. */
		} else {
			//Resetting i value
			i = 0; 
			//Counting calves
			for (x = 0; x < cards_left; x++) {
				if (cards_on_hand[x] / 10 == 3) {
					three_first_digit[i] = cards_on_hand[x];
					i++; //Counter for how many cards in the 30s there is
				}
			}
			//Playing buffalo is first priority
			if (card_47 == 1) {
				printf("47");
			//If there is a buffalo on the board
			} else if (board_now[0] == 47 || board_now[1] == 47 || board_now[2]
							 == 47 || board_now[3] == 47) {
				//Starting for the end of the deck (as decks are already sorted
				// from smallest the largest
				x = cards_left - 1;
				//Make sure 48 and 49s are not played when buffalo is on board
				while ((cards_on_hand[x] == 48 || cards_on_hand[x] == 49) && x > 0) {
					x =  x - 1;
				}
				print(cards_on_hand[x]);
			//There are calve cards
			} else if (i > 0) {
				//Playing the largest calf possible
				print(three_first_digit[i - 1]);
			} else {
				print(cards_on_hand[cards_left - 1]);
			}
		}
	}

}

// ADD A COMMENT HERE EXPLAINING YOUR OVERALL TESTING STRATEGY

/* My overall strategy is to test as many possibilites as possible by using a
set generator to create many scenarios decks. The sets are also randomised to
test if the overall logic of the code works without relying on the positioning
of the sorted order of the cards/ */

void run_unit_tests(void) {
    // PUT YOUR UNIT TESTS HERE
	int array[10] = {1, 5, 6, 8, 31, 100, 54, 0, 19, 6};
	int thirty_deck[5] = {31, 32, 35, 38, 39};
	int thirtyforty_deck[6] = {31, 32, 35, 38, 39, 45};
	int array1[10] = {18, 24, 25, 27, 31, 32, 35, 37, 46, 49};
	int array2[10] = {44, 48, 12, 42, 27, 33, 13, 10, 14, 32};
	int array3[10] = {37, 21, 14, 38, 32, 46, 42, 18, 15, 41};
	int array4[10] = {25, 16, 19, 23, 21, 18, 26, 17, 36, 41};
	int array5[10] = {40, 27, 46, 48, 28, 25, 19, 23, 16, 47};
	int array6[10] = {10, 17, 38, 46, 11, 12, 19, 37, 24, 41};
	int array7[10] = {16, 42, 21, 33, 35, 10, 15, 13, 48, 34};
	int array8[10] = {22, 49, 17, 31, 46, 34, 32, 41, 38, 24};
	int array9[10] = {34, 21, 29, 49, 41, 11, 38, 30, 27, 47};
	int array10[10] = {15, 12, 49, 41, 28, 25, 14, 40, 19, 34};
	int array11[10] = {20, 46, 21, 36, 11, 41, 15, 12, 48, 22};
	int array12[10] = {14, 16, 20, 42, 23, 27, 24, 48, 13, 35};
	int array13[10] = {31, 38, 30, 17, 28, 41, 21, 46, 25, 44};

	assert(max(array, 10) == 100);
	printf("Max value function test1 passed!\n");

	assert(max(thirty_deck, 5) == 39);
	printf("Max value function test2 passed!\n");

	assert(max(array1, 10) == 49);
	printf("Max value function test3 passed!\n");

	assert(max(array2, 10) == 48);
	printf("Max value function test4 passed!\n");

	assert(max(array3, 10) == 46);
	printf("Max value function test5 passed!\n");

	assert(max(array4, 10) == 41);
	printf("Max value function test6 passed!\n");

	assert(max(array5, 10) == 48);
	printf("Max value function test7 passed!\n");

	assert(max(array6, 10) == 46);
	printf("Max value function test8 passed!\n");

	assert(max(array7, 10) == 48);
	printf("Max value function test9 passed!\n");

	assert(max(array8, 10) == 49);
	printf("Max value function test10 passed!\n");

	assert(max(array9, 10) == 49);
	printf("Max value function test11 passed!\n");

	assert(max(array10, 10) == 49);
	printf("Max value function test12 passed!\n");

	assert(max(array11, 10) == 48);
	printf("Max value function test13 passed!\n");

	assert(max(array12, 10) == 48);
	printf("Max value function test14 passed!\n");

	assert(max(array13, 10) == 46);
	printf("Max value function test15 passed!\n");

	assert(max_array(array, 10) == 5);
	printf("Max array function test1 passed!\n");

	assert(max_array(thirty_deck, 5) == 4);
	printf("Max array function test2 passed!\n");

	assert(max_array(array1, 10) == 9);
	printf("Max array function test3 passed!\n");

	assert(max_array(array2, 10) == 1);
	printf("Max array function test4 passed!\n");

	assert(max_array(array3, 10) == 5);
	printf("Max array function test5 passed!\n");

	assert(max_array(array4, 10) == 9);
	printf("Max array function test6 passed!\n");

	assert(max_array(array5, 10) == 3);
	printf("Max array function test7 passed!\n");

	assert(max_array(array6, 10) == 3);
	printf("Max array function test8 passed!\n");

	assert(max_array(array7, 10) == 8);
	printf("Max array function test9 passed!\n");

	assert(max_array(array8, 10) == 1);
	printf("Max array function test10 passed!\n");

	assert(max_array(array9, 10) == 3);
	printf("Max array function test11 passed!\n");

	assert(max_array(array10, 10) == 2);
	printf("Max array function test12 passed!\n");

	assert(max_array(array11, 10) == 8);
	printf("Max array function test13 passed!\n");

	assert(max_array(array12, 10) == 7);
	printf("Max array function test14 passed!\n");

	assert(max_array(array13, 10) == 7);
	printf("Max array function test15 passed!\n");

	assert(firstroundcardprint(thirty_deck, 5) == 31);
	printf("First Round Card Print function test1 passed!\n");

	assert(firstroundcardprint(thirtyforty_deck, 6) == 45);
	printf("First Round Card Print function test2 passed!\n");

	assert(max_second_digit(array1, 10) == 18);	
	printf("Max Second Digit function test1 passed!\n");

	assert(max_second_digit(array2, 10) == 48);
	printf("Max Second Digit function test2 passed!\n");

	assert(max_second_digit(array3, 10) == 38);
	printf("Max Second Digit function test3 passed!\n");

	assert(max_second_digit(array4, 10) == 26);
	printf("Max Second Digit function test4 passed!\n");

	assert(max_second_digit(array5, 10) == 48);
	printf("Max Second Digit function test5 passed!\n");

	assert(max_second_digit(array6, 10) == 19);
	printf("Max Second Digit function test6 passed!\n");

	assert(max_second_digit(array7, 10) == 16);
	printf("Max Second Digit function test7 passed!\n");

	assert(max_second_digit(array8, 10) == 24);
	printf("Max Second Digit function test8 passed!\n");

	assert(max_second_digit(array9, 10) == 38);
	printf("Max Second Digit function test9 passed!\n");

	assert(max_second_digit(array10, 10) == 19);
	printf("Max Second Digit function test10 passed!\n");

	assert(max_second_digit(array11, 10) == 22);
	printf("Max Second Digit function test11 passed!\n");

	assert(max_second_digit(array12, 10) == 16);
	printf("Max Second Digit function test12 passed!\n");

	assert(max_second_digit(array13, 10) == 38);
	printf("Max Second Digit function test13 passed!\n");

	printf("All test passed :D !\n");
}


// ADD YOUR FUNCTIONS HERE
void print(int value) {
	printf("%d\n", value);
}

void deck(int deck_array[], int cards_left) {
	for (int x = 0; x < cards_left; x++){
		scanf("%d", &deck_array[x]);
	}
}

int max(int array[], int number_of_cards) {
	int max_value = array[0];
	for (int x = 0; x < number_of_cards; x++) {
		for (int i = x + 1; i < number_of_cards; i++) {
			if (array[x] > array[i] && array[x] > max_value) {
				max_value = array[x];
			} else if (array[i] > array[x] && array[i] > max_value) {
				max_value = array[i];
			}
		}
	}
	return max_value;
}

int max_array(int array[], int number_of_cards) {
	int max = 0;
	int max_value = array[0];
	for (int x = 0; x < number_of_cards; x++) {
		for (int i = x + 1; i < number_of_cards; i++) {
			if (array[x] > array[i] && array[x] > max_value) {
				max_value = array[x];
				max = x;
			} else if (array[i] > array[x] && array[i] > max_value) {
				max_value = array[i];
				max = i;
			}
		}
	}
	return max;
}

int firstroundcardprint(int cards_on_hand[], int cards_left) {
	int x = 0;
	int result = 0;
	//Check if the player have a full hand of 30s
	if (cards_on_hand[0] / 10 == 3 && cards_on_hand[cards_left - 1] / 10 == 3) {
		result = cards_on_hand[0];
	//Check if the player have 30s and 40s, if so not play 47
	} else if (cards_on_hand[0] / 10 == 3 && cards_on_hand[cards_left - 1] / 10 != 3 
				&& cards_on_hand[cards_left - 1] < 47 ) {
		result = cards_on_hand[cards_left - 1];
	} else if (cards_on_hand[0] / 10 == 3 && cards_on_hand[cards_left - 1] / 10 != 3) {
		while (cards_on_hand[x] / 10 == 3) {
		x++;
		}
		result = cards_on_hand[x];
	} else {
		result = cards_on_hand[0];
	}
	return result;
}

int max_second_digit (int board_now[], int largest_number){ //Need to check
	int max_value = board_now[0];
	for (int x = 0; x < largest_number; x++) {
		for (int j = x + 1; j < largest_number; j++) {
			if (board_now[x] > board_now[j] && board_now[x] > max_value && 
					board_now[x]/10 == board_now[0]/10 && board_now[j]/10 == 
						board_now[0]/10) {
				max_value = board_now[x];
			} else if (board_now[j] > board_now[x] && board_now[j] > max_value && 
							board_now[x]/10 == board_now[0]/10 
								&& board_now[j]/10 == board_now[0]/10) {
				max_value = board_now[j];
			}
		}
	}
	return max_value;
}

int min_array(int array[], int number_of_cards) {
	int min = 0;
	int min_value = array[0];
	for (int x = 0; x < number_of_cards; x++) {
		for (int i = x + 1; i < number_of_cards; i++) {
			if (array[x] % 10 < array[i] % 10 && array[x] % 10 < min_value % 10 
					&& array[x] / 10 != 3 && array[i] / 10 != 3 && array[x] != 47 
						&& array[i] != 47) {
				min_value = array[x];
				min = x;
			} else if (array[i] % 10 < array[x] % 10 && array[i] % 10 < min_value 
							% 10 && array[x] / 10 != 3 && array[i] / 10 != 3 && 
								array[x] != 47 && array[i] != 47) {
				min_value = array[i];
				min = i;
			}
		}
	}
	return min_value;
}


