//Header for Gyakuten Tic-Tac-Toe
//Project rebuild is required if any change was made to this file

#ifndef TICTACTOE_H_INCLUDED
#define TICTACTOE_H_INCLUDED

//Allegro and standard in/out header
#include <allegro.h>
#include <stdio.h>

//Common colours in Allegro defined to simplify coding
#define BLACK makecol(0,0,0)
#define GREY makecol(128,128,128)
#define LGREY makecol(192,192,192)
#define DGREY makecol(64,64,64)
#define RED makecol(255,0,0)
#define GREEN makecol(0,255,0)
#define DGREEN makecol(0,150,0)
#define BLUE makecol(0,0,255)
#define YELLOW makecol(255,255,0)
#define DYELLOW makecol(150,150,0)
#define WHITE makecol(255,255,255)
#define ORANGE makecol(255,128,0)
#define BROWN makecol(150,100,50)
#define DBROWN makecol(100,70,40)
#define TRANS makecol(255,0,255)

//Game status
#define MENU 0
#define PLAY 1
#define MODE 2
#define HELP 3

//Grid size
#define GRID_X 200
#define GRID_Y 150

//Initialization, deinitialization, and close button handler functions are written inside the header
void init_all();
void deinit_all();
void Handler();

//Window dimentions and close button handling variables
const int SCREEN_X = 600, SCREEN_Y = 650;
volatile bool close_button_pressed = false;

//Game elements declaration
BITMAP *xSprite, *oSprite;
BITMAP *edge, *wright;
BITMAP *title;
BITMAP *buffer, *title_buffer;
SAMPLE *objection, *take, *victory;

//Initialization funcion, allegro stuff
void init_all() {
	int depth, res; //Colour depth and graphics mode

    //Error message if Allegro is unable to initiliza
	if (allegro_init() == 0) {
        allegro_message("Unable to initialize Allegro!");
        exit(-1);
    }

    //Colour depth set to screen colour depth, if screen colour depth unobtainable then set to 32
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);

    //Graphics mode initialization, if failed error message occurs
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_X, SCREEN_Y, 0, 0);
	if (res != 0) {
		allegro_message("Graphics mode error!");
		exit(-1);
	}

	//Keyboard and sound initilization
	install_keyboard();
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
}

void deinit_all() {
    //Keyboard buffer cleared
	clear_keybuf();

    //System items removal (memory released)
	remove_keyboard();
	remove_sound();

	//Game elements removal (memory released)
	destroy_bitmap(xSprite);
	destroy_bitmap(oSprite);
	destroy_bitmap(edge);
	destroy_bitmap(wright);
	destroy_bitmap(title);
	destroy_bitmap(buffer);
	destroy_bitmap(title_buffer);
	destroy_sample(objection);
	destroy_sample(take);
	destroy_sample(victory);
}

//Close button handler
void Handler(void){
     close_button_pressed = true;
}
END_OF_FUNCTION(Handler);

#endif
