/*stb_autrun.cc
Auto Shut the Box
Nick Powers
23 Nov 2013

***Auto Only, Verbose Printing***

This will be a simulation of the game, Shut the Box.
StB is a literal box with 2 dice and a set of tabs
numbered 1-9 on a rack within the box.

For a general description with many variants, see:
https://en.wikipedia.org/wiki/Shut_the_Box

For this version:
The object of the game is to flip all tabs down 
and shut the box. A player starts out with all tabs up
[1] [2] [3] [4] [5] [6] [7] [8] [9] 
and must roll the dice. The player may then flip down tabs
based on the dice roll. A player must flip down a tab or 
tabs whose (summed) value equals that of the (summed)
dice roll.

ex: dice rolled [4] & [3] == 7. So if the board is new,
[1] [2] [3] [4] [5] [6] [7] [8] [9]
The player must choose among the following options:
flip down [7],
flip down [6] & [1],
flip down [5] & [2], or
flip down [4] & [3], 

If the player chooses [7], the board would 
look like:

[1] [2] [3] [4] [5] [6] [X] [8] [9]
and the player would need to roll again.

Repeat until:
You cannot make a valid move,
or
the sum of the remaining numbers <= 7
(when this happens, use one die).

When you cannot make a valid move, your score
is the number made of the digits of the remaining
tabs up from left to right. ex:
tabs up: [X] [X] [3] [X] [X] [6] [X] [X] [X] 3,6
dice roll: [1], [1]
score: 36
In a group, the next person is up. Golf scoring:
lower score is better, since 0 (all tabs are down)
shuts the box.

notes: 
-for now use global variables
*/

#include <stdio.h>
#include <iostream>
#include <time.h>

using namespace std;

void init(void);
bool auto_go(void);
bool solve(int);
void temp_adder(void);
void temp_clear_under(int);
void board_adder(void);
void print_b(void);
void print_temp(void);
int dice_roll(void);


int board[9];
int tempb[9];
bool win = false;
int score = -1;
int testvar = 9999;
int play = -1;
int die1 = 0;
int die2 = 0;
int turn_ct = 1;
int temp_sum = 0;
int board_sum;
bool done = true;


int main (void)
{
  cout << "\nenter: main\n";
  cout << "\n \n Welcome to Shut the Box! \n \n" 
       << "2. auto-play? [2]: ";

  cin >> play;
  cout << "\n";

  while(play != 2){
    cout << "2 for auto-play \n";
    cin >> play;
  }//if bad selection

  init();
  win = auto_go();

  if (win)
    cout << "You won in " << turn_ct << " turns\n";

  else {
    cout << "Your score was: ";
    for(int i = 0; i < 9; i++)
      if (board[i] != 0)
	cout << board[i];

    cout << " in " << turn_ct << " turns... :( \n \n";
  }//else
  cout << "exit: main\n";
  return 0;
}//main

//void init
//
void init()
{
  cout << "enter: init\n";
  for(int i = 0; i < 9; i++)//set up board
    board[i] = i + 1;
  
  for(int i = 0; i < 9; i++)//set up temp decision board to check against real
    tempb[i] = 0;
  cout << "exit: init\n";
}//init()

//bool autogo
//
bool auto_go()
{
  cout << "enter: auto_go\n";
  while(done == true){
    
    print_b();
    die1 = dice_roll();
    die2 = dice_roll();

    board_adder();

    if(board_sum <= 6){
      cout << "board_sum ==" << board_sum << endl;
      die2 = 0;
    }//if use one die
    cout << "current dice: " << die1 << ", " << die2 << endl;
    
    temp_clear_under(8);//clear whole tempboard
    done = solve(8);

    turn_ct++;
    cout << "turn_ct == " << turn_ct << endl;
    if(done){
      for(int i = 0; i < 9; i++)
	if(tempb[i] != 0)
	  board[i] = 0;
     
    }//if successful move

    board_adder();
    if(board_sum == 0){
      cout << "exit: auto_go\n";
      return true;
    }//if won

    if(turn_ct == 50)
      return false;
  }//while
  cout << "exit: auto_go\n";
  return done;
}//auto_go

//solve
//pre: board[9], tempb[9] all elements 0,
//post: goes through all possible options to look for a valid move
bool solve(int num)
{
  temp_adder();
  print_temp();
  cout << "enter: solve, i == " << num + 1 << endl;
  for(int i = num; i > -1; i--){
    
    if((i + 1) > (die1 + die2)){
      cout << "solve: skip " << i + 1 << endl;
      tempb[i] = 0;
    }//skip this value, ex: dice roll == 6, i == 8

    else if(board[i] != 0){

      if((board[i] + temp_sum) > (die1 + die2)){	
	cout << "solve: " << i + 1 << " + tempb > " << die1 + die2 << endl; 
	print_temp();
	done = solve(i - 1);

	if(done == true)
	  return done;
      }//if done
      
      else if((board[i] + temp_sum) < (die1 + die2)){
	tempb[i] = i + 1;
        temp_adder();
	cout << "solve: " << i + 1 << " < " << die1 + die2 << endl; 
	done = solve(i - 1);

	if(done == true){
	  print_temp();
	  return done;
	}//if true

	else {
	  temp_clear_under(i);
	  temp_adder();
	}//else clear the tree beneath
      }//else if add this and another # maybe
      
      else if((board[i] + temp_sum) == (die1 + die2)){
	tempb[i] = i + 1;
	for(int j = 0; j < 9; j++){
	  if(tempb[j] != 0){
	    cout << "[" << tempb[j] << "] ";
	  }//if
	}//for j
	cout << "\n^exit: solve chain, @ i == " << i + 1  << endl;
	print_temp();
	return done = true;
      }//else if don't add this # but add another maybe
    }//else if unflipped tab
    cout << "solve: failed @ " << i + 1 << endl;
  }//for i 
  cout << "exit: solve, i == " << num << endl;
  return false;
}//solve
 
 
//temp_adder
//pre:
//post: temp_sum = sum of possible turn selection tabs
void temp_adder(void)
{
  temp_sum = 0;
  for(int i = 0; i < 9; i++)
    temp_sum += tempb[i];
}//temp_add

void temp_clear_under(int num)
{
  for(int i = 0; i <= num; i++)
    tempb[i] = 0;
}//temp_clear

//board_adder
//pre: 
//post: gives sum of the board
void board_adder(void)
{
  board_sum = 0;
  for(int i = 0; i < 9; i++)
    board_sum += board[i];
}//board_adder

void print_b(void)
{
  cout << "Current Board: ";
  for(int i = 0; i < 9; i++){
    if(board[i] == 0)
      cout << "[/] ";
    else 
      cout << "[" << board[i] << "] ";
  }//for i
  cout << "\n";
}//print the board

void print_temp(void)
{
  cout << "tempb: ";;

  for(int i = 0; i < 9; i++){
    if(tempb[i] != 0)
      cout << "[" << tempb[i] << "] ";
    else
      cout << "[x] "; 
  }//for
  cout << endl;
}//print_temp


//dice_roll
//pre: none
//post: returns a random number 1-6 for the dice roll
int dice_roll()
{
  clock_t t = clock() % 7;
  while(t == 0)
    t = clock() % 7;
  return t;
}//dice_roll
