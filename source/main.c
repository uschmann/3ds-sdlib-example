#include <string.h>
#include <stdio.h>
#include <3ds.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define MAX_FILENAME_SIZE 15

// screen attr
const int TOPSCREEN_WIDTH = 400;
const int BOTTOMSCREEN_WIDTH = 340;
const int SCREEN_HEIGHT = 240;
const int SCREEN_BPP = 16;

//The surfaces that will be used
SDL_Surface *message = NULL;
SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;


SDL_Surface *load_image(char *filename) {
    //Temporary storage for the image that's loaded
    SDL_Surface* loadedImage = NULL;
    
    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

    char full_path[MAX_FILENAME_SIZE+8];
    memset(full_path, '\0', sizeof(char)*(MAX_FILENAME_SIZE+8));
    strcpy(full_path, "romfs:/");
    strcat(full_path, filename);

    // let's load dat BMP
    loadedImage = SDL_LoadBMP(full_path);
    // loadedImage = IMG_Load(full_path);

    if( loadedImage != NULL ) {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat(loadedImage);
        
        //Free the old image
        SDL_FreeSurface(loadedImage);
    }

    // else
    //     printf("IMG_Load: %s\n", IMG_GetError());

    //Return the optimized image
    return optimizedImage;
}

void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination) {
    //Make a temporary rectangle to hold the offsets
    SDL_Rect offset;
    
    //Give the offsets to the rectangle
    offset.x = x;
    offset.y = y;

    //Blit the surface
    SDL_BlitSurface(source, NULL, destination, &offset);
}

int main(int argc, char **argv) {

    //Initialize all SDL subsystems
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        return 1;

    SDL_ShowCursor(0);

    //Set up the screen
    screen = SDL_SetVideoMode(TOPSCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_TOPSCR | 
            SDL_CONSOLEBOTTOM);
    if (screen == NULL)
        return 1;

    Result rs = romfsInit();
    if (rs) 
        printf("romfsInit: %08lx\n", rs);
    else {
        printf("romfs inicializado");

        //Load the images
        message = load_image("hello.bmp");
        background = load_image("background.bmp");

        //Apply the background to the screen
        apply_surface( 0, 0, background, screen);
        apply_surface( 200, 0, background, screen );
        apply_surface( 0, 120, background, screen );
        apply_surface( 200, 120, background, screen );

        //Apply the message to the screen
        apply_surface( 100, 60, message, screen );

        //Update the screen
        if(SDL_Flip(screen) < 0)
            return 1;

        //Wait 2 seconds
        SDL_Delay(2000);

        //Free the surfaces
        SDL_FreeSurface(message);
        SDL_FreeSurface(background);
    }

	// Main loop
	while (aptMainLoop()) {
        hidScanInput();

		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu
	}

    //Quit SDL
    SDL_Quit();

	return 0;
}
