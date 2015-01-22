#include "tictactoe.h"

//Game structure function prototypes
void setupBoard();
int moveBox();
bool drawXO();
void updateBoard();
bool checkRoundFinishedAndWinnner();
void announceWinner();
int menu();
int help();
int option();
int mode_selection();
void reInitialize();

//AI function prototypes
void computerFirstMove();
void computerRandomMove();
void computerSmartMove();
void computerSmartMoveHelper();

//Despite globals been bad programming practice, most of these globals are required in multiple functions
//as passing references/values is not used at all
int game_status = MENU;
int board[9] = {0};
int currentSquare = 0;
int turn = 1, turn_counter = 0;
int x = 0, y = 0;
int tempX = 0, tempY = 0;
bool menu_first = TRUE, play_first = TRUE;
bool isGameOver = false;
bool AI = false;

int main()
{
    init_all();

    //Lock function needed to use handler in Allegro
	LOCK_FUNCTION(Handler);
	set_close_button_callback(Handler);

    //Sound effects
    objection = load_sample("objection.wav");
    take = load_sample("take.wav");
    victory = load_sample("victory.wav");

    //Images
    xSprite = load_bitmap("take.bmp", NULL);
    oSprite = load_bitmap("objection.bmp", NULL);
    title = load_bitmap("title.bmp", NULL);
    edge = load_bitmap("edge.bmp", NULL);
    wright = load_bitmap("wright.bmp", NULL);

    //Buffers for images
    buffer = create_bitmap(200, 150);
    title_buffer = create_bitmap(363, 53);

    //Main loop controlling the entire program will always return to menu
    while (!key[KEY_ESC] && !close_button_pressed && game_status == MENU)
    {
        game_status = menu();
        //Help loop only occurs when the status is help
        while (!key[KEY_ESC] && !close_button_pressed && game_status == HELP) game_status = help();
        //Play loop is the main game
        while (!key[KEY_ESC] && !close_button_pressed && game_status == PLAY)
        {
            //First run will set up new grid
            if (play_first) setupBoard();

            game_status = moveBox(); //Main game starts

            rest(1); //Used to prevent keyboard over-sensitive
        }
    }

    deinit_all();

    return 0;
}
END_OF_MAIN();

//Setting up the grid and character sprites
void setupBoard()
{
    acquire_screen(); //Used to make the process faster, not needed

    //Lines making up the 3*3 grid
    vline(screen, 200, 0, 450, WHITE);
    vline(screen, 400, 0, 450, WHITE);
    hline(screen, 0, 150, SCREEN_X, WHITE);
    hline(screen, 0, 300, SCREEN_X, WHITE);
    hline(screen, 0, 450, SCREEN_X, WHITE);

    //Default selection rectangle positioned at top left corner
    rect(screen, x, y, x + GRID_X, y + GRID_Y, YELLOW);

    //Set up the character images & explanatory texts
    draw_sprite(screen, edge, 430, 500);
    draw_sprite(screen, wright, 0, 500);
    textout_ex(screen, font, "Defence", 50, 475, WHITE, -1);
    textout_ex(screen, font, "Procecution", 465, 475, WHITE, -1);

    release_screen(); //Used to make the process faster, not needed
}

//Function governing the selection box's movement and the placement of moves
int moveBox()
{
    clear_keybuf();
    tempX = x;
    tempY = y;

    //4 if and else if statements will manage the movement of the selection box
    if (key[KEY_UP] && y != 0 && !isGameOver)
    {
        y -= GRID_Y;
        currentSquare -=3;
        updateBoard();
    }
    else if (key[KEY_DOWN] && y != 2 * GRID_Y && !isGameOver)
    {
        y += GRID_Y;
        currentSquare +=3;
        updateBoard();
    }
    else if (key[KEY_RIGHT] && x != 2 * GRID_X && !isGameOver)
    {
        x += GRID_X;
        ++currentSquare;
        updateBoard();
    }
    else if (key[KEY_LEFT] && x != 0 && !isGameOver)
    {
        x -= GRID_X;
        --currentSquare;
        updateBoard();
    }

    //This statement manages the placement of step
    else if (key[KEY_SPACE] && board[currentSquare] == 0)
    {
        drawXO(); //Player's turn
        //If the round is not over (won or draw) then AI will play
        if (!isGameOver && AI)
        {
            computerSmartMove();
            drawXO();
        }
        return PLAY;
    }
    else if (key[KEY_M]) //Pressing M returns to menu
    {
        reInitialize();

        return MENU;
    }
    else if (key[KEY_R]) //Presssing R resets the board and starts a new round
    {
        reInitialize();

        return PLAY;
    }

    return PLAY;
}

//Function governing the drawing of steps' sprites
bool drawXO()
{
    bool end = false;

    acquire_screen();
    turn_counter++; //counter to check if the board is filled

    if (!end)
    {
        //Plays different sound effect and places different sprite depending on current player
        if(turn == 1)
        {
            play_sample(take, 255, 127, 1000, 0);
            draw_sprite(buffer, xSprite, 0, 0);
            blit(buffer, screen, 0, 0, x, y, buffer->w, buffer->h);
            board[currentSquare] = 1;
            end = checkRoundFinishedAndWinnner();
            ++turn;
        }
        else if(turn == 2)
        {
            play_sample(objection, 255, 127, 1000, 0);
            draw_sprite(buffer, oSprite, 0, 0);
            blit(buffer, screen, 0, 0, x, y, buffer->w, buffer->h);
            board[currentSquare] = 2;
            end = checkRoundFinishedAndWinnner();
            --turn;
        }
    }

    clear_bitmap(buffer);
    release_screen();
    rest(100); //Rest to prevent keyboard over sensitive

    return end;
}

//Required to keep the board "alive", or else the board will frezze after first step
void updateBoard()
{
    rect(screen, tempX + 1, tempY + 1, tempX + 199, tempY + 149, BLACK);
    rect(screen, x + 1, y + 1, x + 199, y + 149, YELLOW);
    rest(150);
}

//Function that checks for winner or draw (hard coded)
bool checkRoundFinishedAndWinnner()
{
    //If any of the possible 8 ways to win is achieved then returns TRUE into the calling function
    if (board[0] == turn && board[1] == turn &&  board[2] == turn)
    {
        isGameOver = true;
        announceWinner();
        return 1;
    }
    else if (board[0] == turn &&  board[3] == turn  && board[6] == turn)
    {
        isGameOver = true;
        announceWinner();
        return 1;
    }
    else if (board[0] == turn &&  board[4] == turn  && board[8] == turn)
    {
        isGameOver = true;
        announceWinner();
        return 1;
    }
    else if (board[1] == turn &&  board[4] == turn  && board[7] == turn)
    {
        isGameOver = true;
        announceWinner();
        return 1;
    }
    else if (board[2] == turn &&  board[4] == turn  && board[6] == turn)
    {
        isGameOver = true;
        announceWinner();
        return 1;
    }
    else if (board[2] == turn &&  board[5] == turn  && board[8] == turn)
    {
        isGameOver = true;
        announceWinner();
        return 1;
    }
    else if (board[3] == turn &&  board[4] == turn  && board[5] == turn)
    {
        isGameOver = true;
        announceWinner();
        return 1;
    }
    else if (board[6] == turn &&  board[7] == turn  && board[8] == turn)
    {
        isGameOver = true;
        announceWinner();
        return 1;
    }
    //If the board is filled and no victory is achieved then still returns TURE but prints out different message
    else if (turn_counter >= 9)
    {
        isGameOver = true;
        textout_ex(screen, font, "TRIAL EXTENDED!", 250, 470, RED, -1);
        textout_ex(screen, font, "Press M to return to main menu, press R to play again", 100, 490, WHITE, -1);
        return 1;
    }

    return 0; //No winner or draw, game keeps going
}

//Prints out winning message depending on current round
void announceWinner()
{
    play_sample(victory, 255, 127, 1000, 0); //Plays Victory Fanfare no matter who won

    textout_ex(screen, font, "Press M to return to main menu, press R to play again", 100, 490, WHITE, -1);

    if (turn == 1) textout_ex(screen, font, "GUILTY!", 265, 470, RED, -1);
    else if (turn == 2) textout_ex(screen, font, "NOT GUILTY!", 250, 470, RED, -1);
}

//The entire menu is controlled by this function
int menu()
{
    clear_keybuf();

    //Prints title and credits
    draw_sprite(title_buffer, title, 0, 0);
    blit(title_buffer, screen, 0, 0, 115, 75, title_buffer->w, title_buffer->h);

    //If first time in menu then default selection to "play"
    if (menu_first)
    {
        textout_ex(screen, font, "Press P to play PvP game (Player vs Player)", 130, 270, WHITE, -1);
        textout_ex(screen, font, "Press A to play PvE game (Player vs AI)", 140, 320, WHITE, -1);
        textout_ex(screen, font, "Press H for the Help menu", 195, 370, WHITE, -1);
        menu_first = FALSE;
    }

    if (key[KEY_P])
    {
        AI = false;
        play_first = true;
        clear_bitmap(screen);
        return PLAY;
    }

    if (key[KEY_A])
    {
        AI = true;
        play_first = true;
        clear_bitmap(screen);
        return PLAY;
    }

    if (key[KEY_H])
    {
        clear_bitmap(screen);
        return HELP;
    }

    clear_bitmap(title_buffer);

    return MENU;
}

//Help menu (no selections, just text)
int help()
{
    clear_keybuf();

    //Prints title and credit
    draw_sprite(title_buffer, title, 0, 0);
    blit(title_buffer, screen, 0, 0, 115, 75, title_buffer->w, title_buffer->h);

    //Prints instructions
    textout_ex(screen, font, "Use arrow keys to move the selection box", 130, 150, WHITE, -1);
    textout_ex(screen, font, "Use space key to place your step", 160, 170, WHITE, -1);
    textout_ex(screen, font, "Use ESC key or the Close button to quit the game", 100, 190, WHITE, -1);

    textout_ex(screen, font, "Press M at any time in game to return to main menu", 100, 330, WHITE, -1);
    textout_ex(screen, font, "Press R at any time in game to start a new round", 102, 350, WHITE, -1);

    textout_ex(screen, font, "Press Space to Return to Main Menu", 160, 500, RED, -1);

    //If space is pressed then return to menu
    if (key[KEY_SPACE])
    {
        clear_to_color(screen, BLACK);
        menu_first = TRUE;
        return MENU;
    }

    clear_bitmap(title_buffer);

    return HELP;
}

//At the end of the round, no matter returning to menu or starting new round, this is called to reset everything
void reInitialize()
{
    clear_bitmap(screen);

    //Game not over
    isGameOver = false;

    //All squares are empty
    for(int i = 0; i < 9; i++) board[i] = 0;

    //Game controlling variables reset, Victory Fanfare stopped
    currentSquare = 0;
    turn = 1;
    turn_counter = 0;
    x = 0;
    y = 0;
    tempX = 0;
    tempY = 0;
    menu_first = TRUE;
    play_first = TRUE;
    stop_sample(victory);
}

//If computer has to make the first move (not in standard program) then randomly generate a step
void computerFirstMove()
{

    clear_keybuf();
    tempX = x;
    tempY = y;

    currentSquare = rand() % 9;

    if( currentSquare == 0 )
    {
        x = 0;
        y = 0;
        updateBoard();
    }
    else if( currentSquare == 3 )
    {
        x = 0;
        y = 150;
        updateBoard();
    }
    else if( currentSquare == 4 )
    {
        x = 200;
        y = 150;
        updateBoard();
    }
    else if( currentSquare == 5 )
    {
        x = 400;
        y = 150;
        updateBoard();
    }
    else if( currentSquare == 6 )
    {
        x = 0;
        y = 300;
        updateBoard();
    }
    else if( currentSquare == 7 )
    {
        x = 200;
        y = 300;
        updateBoard();
    }
    else if( currentSquare == 8 )
    {
        x = 400;
        y = 300;
        updateBoard();
    }
}

//If computer has no move prioritized (block or win) then it will randomly play a move
void computerRandomMove()
{
    int keyAction = -1;

    clear_keybuf();
    tempX = x;
    tempY = y;

    do
    {
        keyAction = rand() % 4;

        if( keyAction == 0 && y != 0)
        {
            y -= 150;
            currentSquare -=3;
            updateBoard();
        }
        else if( keyAction == 1 && y != 300)
        {
            y += 150;
            currentSquare +=3;
            updateBoard();
        }
        else if( keyAction == 2 && x != 400)
        {
            x += 200;
            ++currentSquare;
            updateBoard();
        }
        else if( keyAction == 3 && x != 0)
        {
            x -= 200;
            --currentSquare;
            updateBoard();
        }
    }
    while(board[currentSquare] != 0);
}

//Attempt to win or block
//Computer will attempt to finish a row if squareValue is 1, and attempt to bolck opponent if squareValue is 2
int computerSmartMoveHelper(int squareValue)
{

    int success = 0;

    clear_keybuf();
    tempX = x;
    tempY = y;

    if( ( board[0] == 0 ) && ( board[1] == squareValue ) && ( board[2] == squareValue ) )
    {
        x = 0;
        y = 0;
        currentSquare = 0;
        updateBoard();

        success = 1;
    }
    else if( ( board[0] == squareValue ) && ( board[1] == 0 ) && ( board[2] == squareValue ) )
    {
        x = 200;
        y = 0;
        currentSquare = 1;
        updateBoard();

        success = 1;
    }
    else if( ( board[0] == squareValue ) && ( board[1] == squareValue ) && ( board[2] == 0 ) )
    {
        x = 400;
        y = 0;
        currentSquare = 2;
        updateBoard();

        success = 1;
    }
    //--------------------------------//
    else if( ( board[3] == 0 ) && ( board[4] == squareValue) && ( board[5] == squareValue ) )
    {
        x = 0;
        y = 150;
        currentSquare = 3;
        updateBoard();

        success = 1;
    }
    else if( ( board[3] == squareValue ) && ( board[4] == 0 ) && ( board[5] == squareValue ) )
    {
        x = 200;
        y = 150;
        currentSquare = 4;
        updateBoard();

        success = 1;
    }
    else if( ( board[3] == squareValue ) && ( board[4] == squareValue ) && ( board[5] == 0 ) )
    {
        x = 400;
        y = 150;
        currentSquare = 5;
        updateBoard();

        success = 1;
    }
    //--------------------------------//
    else if( ( board[6] == 0 ) && ( board[7] == squareValue ) && ( board[8] == squareValue ) )
    {
        x = 0;
        y = 300;
        currentSquare = 6;
        updateBoard();

        success = 1;
    }
    else if( ( board[6] == squareValue ) && ( board[7] == 0 ) && ( board[8] == squareValue ) )
    {
        x = 200;
        y = 300;
        currentSquare = 7;
        updateBoard();

        success = 1;
    }
    else if( ( board[6] == squareValue ) && ( board[7] == squareValue ) && ( board[8] == 0 ) )
    {
        x = 400;
        y = 300;
        currentSquare = 8;
        updateBoard();

        success = 1;
    }
    //--------------------------------//
    else if( ( board[0] == 0 ) && ( board[3] == squareValue ) && ( board[6] == squareValue ) )
    {
        x = 0;
        y = 0;
        currentSquare = 0;
        updateBoard();

        success = 1;
    }
    else if( ( board[0] == squareValue ) && ( board[3] == 0 ) && ( board[6] == squareValue ) )
    {
        x = 0;
        y = 150;
        currentSquare = 3;
        updateBoard();

        success = 1;
    }
    else if( ( board[0] == squareValue ) && ( board[3] == squareValue ) && ( board[6] == 0 ) )
    {
        x = 0;
        y = 300;
        currentSquare = 6;
        updateBoard();

        success = 1;
    }
    //--------------------------------//
    else if( ( board[1] == 0 ) && ( board[4] == squareValue ) && ( board[7] == squareValue ) )
    {
        x = 200;
        y = 0;
        currentSquare = 1;
        updateBoard();

        success = 1;
    }
    else if( ( board[1] == squareValue ) && ( board[4] == 0 ) && ( board[7] == squareValue ) )
    {
        x = 200;
        y = 150;
        currentSquare = 4;
        updateBoard();

        success = 1;
    }
    else if( ( board[1] == squareValue ) && ( board[4] == squareValue ) && ( board[7] == 0 ) )
    {
        x = 200;
        y = 300;
        currentSquare = 7;
        updateBoard();

        success = 1;
    }
    //--------------------------------//
    else if( ( board[2] == 0 ) && ( board[5] == squareValue ) && ( board[8] == squareValue ) )
    {
        x = 400;
        y = 0;
        currentSquare = 2;
        updateBoard();

        success = 1;
    }
    else if( ( board[2] == squareValue ) && ( board[5] == 0 ) && ( board[8] == squareValue ) )
    {
        x = 400;
        y = 150;
        currentSquare = 5;
        updateBoard();

        success = 1;
    }
    else if( ( board[2] == squareValue ) && ( board[5] == squareValue ) && ( board[8] == 0 ) )
    {
        x = 400;
        y = 300;
        currentSquare = 8;
        updateBoard();

        success = 1;
    }
    //--------------------------------//
    else if( ( board[0] == 0 ) && ( board[4] == squareValue ) && ( board[8] == squareValue ) )
    {
        x = 0;
        y = 0;
        currentSquare = 0;
        updateBoard();

        success = 1;
    }
    else if( ( board[0] == squareValue ) && ( board[4] == 0 ) && ( board[8] == squareValue ) )
    {
        x = 200;
        y = 150;
        currentSquare = 4;
        updateBoard();

        success = 1;
    }
    else if( ( board[0] == squareValue ) && ( board[4] == squareValue ) && ( board[8] == 0 ) )
    {
        x = 400;
        y = 300;
        currentSquare = 8;
        updateBoard();

        success = 1;
    }
    //--------------------------------//
    else if( ( board[2] == 0 ) && ( board[4] == squareValue ) && ( board[6] == squareValue ) )
    {
        x = 400;
        y = 0;
        currentSquare = 2;
        updateBoard();

        success = 1;
    }
    else if( ( board[2] == squareValue ) && ( board[4] == 0 ) && ( board[6] == squareValue ) )
    {
        x = 200;
        y = 150;
        currentSquare = 4;
        updateBoard();

        success = 1;
    }
    else if( ( board[2] == squareValue ) && ( board[4] == squareValue ) && ( board[6] == 0 ) )
    {
        x = 0;
        y = 300;
        currentSquare = 6;
        updateBoard();

        success = 1;
    }

    return success;
}

//Through computerSmartMoveHelper, the move is generated then paseed to random move
void computerSmartMove()
{

    clear_keybuf();
    tempX = x;
    tempY = y;

    //computerSmartMoveHelper(1): try to finish a row to win
    //computerSmartMoveHelper(2): try to block opponent
    if(computerSmartMoveHelper(1) == 0)
    {
        if(computerSmartMoveHelper(2) == 0) computerRandomMove();
    }
}
