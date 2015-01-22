#include <allegro.h>
//#include "Colours.h"

int board[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; 
                                           
int currentSquare = 0; 
                                             
int turn = 1; 

int x = 0; 
int y = 0;

int tempX = 0;
int tempY = 0;

BITMAP *xSprite;
BITMAP *oSprite;

void setupBoard();
void moveBox();
void drawXO();
void updateBoard();
void checkForWinnner();
void announceWinner();

int main(void)
{
    if (allegro_init() != 0){
                       allegro_message("failed to initialize allegro");
                       exit (-1);
    }
    install_keyboard();
    set_color_depth(16);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 600, 450, 0, 0);
    
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
    
    MIDI *background = load_midi ("dungeon.mid");
    play_midi (background, true);
    
    xSprite = load_bitmap("x.bmp", NULL);
    oSprite = load_bitmap("o.bmp", NULL);

    setupBoard();
    
    while(!key[KEY_ESC]){
        moveBox();    
    }    
    
    destroy_midi(background);
    destroy_bitmap(xSprite);
    destroy_bitmap(oSprite);
    
    return 0;
}   
END_OF_MAIN();

void setupBoard()
{
    acquire_screen();

    line(screen, 200, 0, 200, 480, makecol(255, 255, 255));
    line(screen, 400, 0, 400, 480, makecol(255, 255, 255));
    line(screen, 0, 150, 680, 150, makecol(255, 255, 255));
    line(screen, 0, 300, 680, 300, makecol(255, 255, 255));

    rect(screen, x+1, y+1, x + 199, y + 149, makecol(255, 255, 0));
        
    release_screen();
}    

void moveBox()
{ 
    clear_keybuf();
    tempX = x;
    tempY = y;
    
    if( key[KEY_UP] && y != 0){
        y -= 150;
        currentSquare -=3;
        updateBoard();
    } else if( key[KEY_DOWN] && y != 300){
        y += 150;
        currentSquare +=3;
        updateBoard();
    }  else if( key[KEY_RIGHT] && x != 400){
        x += 200;
        ++currentSquare;
        updateBoard();
    } else if( key[KEY_LEFT] && x != 0){
        x -= 200;
        --currentSquare;
        updateBoard();
    } else if( key[KEY_ENTER] && board[currentSquare] == 0){
        drawXO();
    }          
}    

void drawXO()
{ 
   acquire_screen();
   
   if(turn == 1){
           draw_sprite(screen, xSprite, x, y);
           board[currentSquare] = 1;
           checkForWinnner();
           ++turn;   
  } else if(turn == 2){
         draw_sprite(screen, oSprite, x, y);
         board[currentSquare] = 2;
         checkForWinnner();
         --turn;
  }
  
  release_screen();
  rest(100);
}     

void updateBoard()
{
    rect(screen, tempX+1, tempY+1, tempX + 199, tempY + 149, makecol(0, 0, 0));  
    rect(screen, x+1, y+1, x + 199, y + 149, makecol(255, 255, 0));  
    rest(100);
}  

void checkForWinnner()
{
    if(board[0] == turn && board[1] == turn &&  board[2] == turn){
        announceWinner();
    } else if(board[0] == turn &&  board[3] == turn  && board[6] == turn){
        announceWinner();
    } else if(board[0] == turn &&  board[4] == turn  && board[8] == turn){
        announceWinner();
    } else if(board[1] == turn &&  board[4] == turn  && board[7] == turn){
        announceWinner();
    } else if(board[2] == turn &&  board[4] == turn  && board[6] == turn){
        announceWinner();
    } else if(board[2] == turn &&  board[5] == turn  && board[8] == turn){
        announceWinner();
    } else if(board[3] == turn &&  board[4] == turn  && board[5] == turn){
        announceWinner();
    } else if(board[6] == turn &&  board[7] == turn  && board[8] == turn){
        announceWinner();
    }
}    

void announceWinner()
{        
         if( turn == 1){
                   textout_ex( screen, font, "X Wins!!!!",  300, 240, makecol( 255, 0, 0), makecol(0, 0, 0));  
         } else {
                  textout_ex( screen, font, "O Wins!!!!",  300, 240, makecol( 255, 0, 0), makecol(0, 0, 0));  
         }  
}
