/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

//Texture wrapper class
class LTexture {
public:
    //Initializes variables
    LTexture();

    //Deallocates memory
    ~LTexture();

    //Loads image at specified path
    bool loadFromFile(std::string path);

#ifdef _SDL_TTF_H
    //Creates image from font string
    bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
#endif

    //Deallocates texture
    void free();

    //Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    //Set blending
    void setBlendMode(SDL_BlendMode blending);

    //Set alpha modulation
    void setAlpha(Uint8 alpha);

    //Renders texture at given point
    void render(int x, int y, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL,
                SDL_RendererFlip flip = SDL_FLIP_NONE);

    //Gets image dimensions
    int getWidth();

    int getHeight();

private:
    //The actual hardware texture
    SDL_Texture *mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

void setSpritesRunForward();

void setSpritesRunBack();

void setSpritesWait();

void setSpritesJump();

void setSpritesSquat();

void setSpritesRings();

//The window we'll be rendering to
SDL_Window *gWindow = NULL;

//The window renderer
SDL_Renderer *gRenderer = NULL;

//Scene textures
LTexture gPressTexture;
const int WATING_ANIMATION_FRAMES = 1;
const int RUNING_FORWARD_ANIMATION_FRAMES = 14;
const int RUNING_BACK_ANIMATION_FRAMES = 14;
const int JUMPING_ANIMATION_FRAMES = 9;
const int SQUAT_ANIMATION_FRAMES = 9;
const int RINGS_ANIMATION_FRAMES = 16;

SDL_Rect gSpriteClipsRunForward[RUNING_FORWARD_ANIMATION_FRAMES];
SDL_Rect gSpriteClipsRunBack[RUNING_BACK_ANIMATION_FRAMES];
SDL_Rect gSpriteClipsJumping[JUMPING_ANIMATION_FRAMES];
SDL_Rect gSpriteClipsSquat[SQUAT_ANIMATION_FRAMES];
SDL_Rect gSpriteClipsWaiting[WATING_ANIMATION_FRAMES];
SDL_Rect gSpriteClipsRings[RINGS_ANIMATION_FRAMES];

LTexture::LTexture() {
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture() {
    //Deallocate
    free();
}

bool LTexture::loadFromFile(std::string path) {
    //Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture *newTexture = NULL;

    //Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    } else {
        //Color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        } else {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
    //Get rid of preexisting texture
    free();

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
    if( textSurface != NULL )
    {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
        if( mTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    else
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }


    //Return success
    return mTexture != NULL;
}
#endif

void LTexture::free() {
    //Free texture if it exists
    if (mTexture != NULL) {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
    //Modulate texture rgb
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending) {
    //Set blending function
    SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha) {
    //Modulate texture alpha
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip) {
    //Set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    //Set clip rendering dimensions
    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth() {
    return mWidth;
}

int LTexture::getHeight() {
    return mHeight;
}

bool init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    } else {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!");
        }

        //Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                   SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            //Create vsynced renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            } else {
                //Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0xFF, 0xFF);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia() {
    //Loading success flag
    bool success = true;

    //Load press texture
    if (!gPressTexture.loadFromFile("sonic_hd_sprite.png")) {
        printf("Failed to load press texture!\n");
        success = false;
    } else {
        //SONIC WAITING
        setSpritesWait();

        //SONIC RUNING FORWARD
        setSpritesRunForward();

        //SONIC RUNING BACK
        setSpritesRunBack();

        //SONIC JUMPING
        setSpritesJump();

        //SONIC SQUAT
        setSpritesSquat();

        //SONIC RINGS
        setSpritesRings();

    }


    return success;
}

void setSpritesWait() {
    gSpriteClipsWaiting[0].x = 0;
    gSpriteClipsWaiting[0].y = 0;
    gSpriteClipsWaiting[0].w = 95 - gSpriteClipsWaiting[0].x;
    gSpriteClipsWaiting[0].h = 110 - gSpriteClipsWaiting[0].y;
}

void setSpritesRunBack() {
    gSpriteClipsRunBack[0].x = 930;
    gSpriteClipsRunBack[0].y = 360;
    gSpriteClipsRunBack[0].w = 1024 - gSpriteClipsRunBack[0].x;
    gSpriteClipsRunBack[0].h = 480 - gSpriteClipsRunBack[0].y;

    gSpriteClipsRunBack[1].x = 830;
    gSpriteClipsRunBack[1].y = 360;
    gSpriteClipsRunBack[1].w = 920 - gSpriteClipsRunBack[1].x;
    gSpriteClipsRunBack[1].h = 480 - gSpriteClipsRunBack[1].y;

    gSpriteClipsRunBack[2].x = 730;
    gSpriteClipsRunBack[2].y = 360;
    gSpriteClipsRunBack[2].w = 810 - gSpriteClipsRunBack[2].x;
    gSpriteClipsRunBack[2].h = 480 - gSpriteClipsRunBack[2].y;

    gSpriteClipsRunBack[3].x = 620;
    gSpriteClipsRunBack[3].y = 360;
    gSpriteClipsRunBack[3].w = 710 - gSpriteClipsRunBack[3].x;
    gSpriteClipsRunBack[3].h = 480 - gSpriteClipsRunBack[3].y;

    gSpriteClipsRunBack[4].x = 510;
    gSpriteClipsRunBack[4].y = 360;
    gSpriteClipsRunBack[4].w = 610 - gSpriteClipsRunBack[4].x;
    gSpriteClipsRunBack[4].h = 480 - gSpriteClipsRunBack[4].y;

    gSpriteClipsRunBack[5].x = 420;
    gSpriteClipsRunBack[5].y = 360;
    gSpriteClipsRunBack[5].w = 500 - gSpriteClipsRunBack[5].x;
    gSpriteClipsRunBack[5].h = 480 - gSpriteClipsRunBack[5].y;

    gSpriteClipsRunBack[6].x = 315;
    gSpriteClipsRunBack[6].y = 360;
    gSpriteClipsRunBack[6].w = 410 - gSpriteClipsRunBack[6].x;
    gSpriteClipsRunBack[6].h = 480 - gSpriteClipsRunBack[6].y;

    gSpriteClipsRunBack[7].x = 210;
    gSpriteClipsRunBack[7].y = 360;
    gSpriteClipsRunBack[7].w = 300 - gSpriteClipsRunBack[7].x;
    gSpriteClipsRunBack[7].h = 480 - gSpriteClipsRunBack[7].y;

    gSpriteClipsRunBack[8].x = 100;
    gSpriteClipsRunBack[8].y = 360;
    gSpriteClipsRunBack[8].w = 200 - gSpriteClipsRunBack[8].x;
    gSpriteClipsRunBack[8].h = 480 - gSpriteClipsRunBack[8].y;

    gSpriteClipsRunBack[9].x = 0;
    gSpriteClipsRunBack[9].y = 360;
    gSpriteClipsRunBack[9].w = 95 - gSpriteClipsRunBack[9].x;
    gSpriteClipsRunBack[9].h = 480 - gSpriteClipsRunBack[9].y;

    gSpriteClipsRunBack[10].x = 915;
    gSpriteClipsRunBack[10].y = 480;
    gSpriteClipsRunBack[10].w = 1024 - gSpriteClipsRunBack[10].x;
    gSpriteClipsRunBack[10].h = 590 - gSpriteClipsRunBack[10].y;

    gSpriteClipsRunBack[11].x = 820;
    gSpriteClipsRunBack[11].y = 480;
    gSpriteClipsRunBack[11].w = 910 - gSpriteClipsRunBack[11].x;
    gSpriteClipsRunBack[11].h = 590 - gSpriteClipsRunBack[11].y;

    gSpriteClipsRunBack[12].x = 720;
    gSpriteClipsRunBack[12].y = 480;
    gSpriteClipsRunBack[12].w = 800 - gSpriteClipsRunBack[12].x;
    gSpriteClipsRunBack[12].h = 590 - gSpriteClipsRunBack[12].y;

    gSpriteClipsRunBack[13].x = 610;
    gSpriteClipsRunBack[13].y = 480;
    gSpriteClipsRunBack[13].w = 700 - gSpriteClipsRunBack[13].x;
    gSpriteClipsRunBack[13].h = 590 - gSpriteClipsRunBack[13].y;
}

void setSpritesRunForward() {
    gSpriteClipsRunForward[0].x = 0;
    gSpriteClipsRunForward[0].y = 0;
    gSpriteClipsRunForward[0].w = 95 - gSpriteClipsRunForward[0].x;
    gSpriteClipsRunForward[0].h = 110 - gSpriteClipsRunForward[0].y;

    gSpriteClipsRunForward[1].x = 110;
    gSpriteClipsRunForward[1].y = 0;
    gSpriteClipsRunForward[1].w = 195 - gSpriteClipsRunForward[1].x;
    gSpriteClipsRunForward[1].h = 110 - gSpriteClipsRunForward[1].y;

    gSpriteClipsRunForward[2].x = 220;
    gSpriteClipsRunForward[2].y = 0;
    gSpriteClipsRunForward[2].w = 300 - gSpriteClipsRunForward[2].x;
    gSpriteClipsRunForward[2].h = 110 - gSpriteClipsRunForward[2].y;

    gSpriteClipsRunForward[3].x = 320;
    gSpriteClipsRunForward[3].y = 0;
    gSpriteClipsRunForward[3].w = 410 - gSpriteClipsRunForward[3].x;
    gSpriteClipsRunForward[3].h = 110 - gSpriteClipsRunForward[3].y;

    gSpriteClipsRunForward[4].x = 420;
    gSpriteClipsRunForward[4].y = 0;
    gSpriteClipsRunForward[4].w = 525 - gSpriteClipsRunForward[4].x;
    gSpriteClipsRunForward[4].h = 110 - gSpriteClipsRunForward[4].y;

    gSpriteClipsRunForward[5].x = 525;
    gSpriteClipsRunForward[5].y = 0;
    gSpriteClipsRunForward[5].w = 610 - gSpriteClipsRunForward[5].x;
    gSpriteClipsRunForward[5].h = 110 - gSpriteClipsRunForward[5].y;

    gSpriteClipsRunForward[6].x = 630;
    gSpriteClipsRunForward[6].y = 0;
    gSpriteClipsRunForward[6].w = 710 - gSpriteClipsRunForward[6].x;
    gSpriteClipsRunForward[6].h = 110 - gSpriteClipsRunForward[6].y;

    gSpriteClipsRunForward[7].x = 730;
    gSpriteClipsRunForward[7].y = 0;
    gSpriteClipsRunForward[7].w = 810 - gSpriteClipsRunForward[7].x;
    gSpriteClipsRunForward[7].h = 110 - gSpriteClipsRunForward[7].y;

    gSpriteClipsRunForward[8].x = 830;
    gSpriteClipsRunForward[8].y = 0;
    gSpriteClipsRunForward[8].w = 930 - gSpriteClipsRunForward[8].x;
    gSpriteClipsRunForward[8].h = 110 - gSpriteClipsRunForward[8].y;

    gSpriteClipsRunForward[9].x = 935;
    gSpriteClipsRunForward[9].y = 0;
    gSpriteClipsRunForward[9].w = 1024 - gSpriteClipsRunForward[9].x;
    gSpriteClipsRunForward[9].h = 110 - gSpriteClipsRunForward[9].y;

    gSpriteClipsRunForward[10].x = 0;
    gSpriteClipsRunForward[10].y = 120;
    gSpriteClipsRunForward[10].w = 110 - gSpriteClipsRunForward[10].x;
    gSpriteClipsRunForward[10].h = 230 - gSpriteClipsRunForward[10].y;

    gSpriteClipsRunForward[11].x = 120;
    gSpriteClipsRunForward[11].y = 120;
    gSpriteClipsRunForward[11].w = 210 - gSpriteClipsRunForward[11].x;
    gSpriteClipsRunForward[11].h = 230 - gSpriteClipsRunForward[11].y;

    gSpriteClipsRunForward[12].x = 220;
    gSpriteClipsRunForward[12].y = 120;
    gSpriteClipsRunForward[12].w = 310 - gSpriteClipsRunForward[12].x;
    gSpriteClipsRunForward[12].h = 230 - gSpriteClipsRunForward[12].y;

    gSpriteClipsRunForward[13].x = 324;
    gSpriteClipsRunForward[13].y = 120;
    gSpriteClipsRunForward[13].w = 410 - gSpriteClipsRunForward[13].x;
    gSpriteClipsRunForward[13].h = 230 - gSpriteClipsRunForward[13].y;
}

void setSpritesJump() {
    gSpriteClipsJumping[0].x = 0;
    gSpriteClipsJumping[0].y = 240;
    gSpriteClipsJumping[0].w = 95 - gSpriteClipsJumping[0].x;
    gSpriteClipsJumping[0].h = 320 - gSpriteClipsJumping[0].y;

    gSpriteClipsJumping[1].x = 110;
    gSpriteClipsJumping[1].y = 240;
    gSpriteClipsJumping[1].w = 195 - gSpriteClipsJumping[1].x;
    gSpriteClipsJumping[1].h = 320 - gSpriteClipsJumping[1].y;

    gSpriteClipsJumping[2].x = 210;
    gSpriteClipsJumping[2].y = 240;
    gSpriteClipsJumping[2].w = 300 - gSpriteClipsJumping[2].x;
    gSpriteClipsJumping[2].h = 320 - gSpriteClipsJumping[2].y;

    gSpriteClipsJumping[3].x = 310;
    gSpriteClipsJumping[3].y = 240;
    gSpriteClipsJumping[3].w = 410 - gSpriteClipsJumping[3].x;
    gSpriteClipsJumping[3].h = 320 - gSpriteClipsJumping[3].y;

    gSpriteClipsJumping[4].x = 420;
    gSpriteClipsJumping[4].y = 240;
    gSpriteClipsJumping[4].w = 505 - gSpriteClipsJumping[4].x;
    gSpriteClipsJumping[4].h = 320 - gSpriteClipsJumping[4].y;

    gSpriteClipsJumping[5].x = 520;
    gSpriteClipsJumping[5].y = 240;
    gSpriteClipsJumping[5].w = 610 - gSpriteClipsJumping[5].x;
    gSpriteClipsJumping[5].h = 320 - gSpriteClipsJumping[5].y;

    gSpriteClipsJumping[6].x = 620;
    gSpriteClipsJumping[6].y = 240;
    gSpriteClipsJumping[6].w = 715 - gSpriteClipsJumping[6].x;
    gSpriteClipsJumping[6].h = 320 - gSpriteClipsJumping[6].y;

    gSpriteClipsJumping[7].x = 720;
    gSpriteClipsJumping[7].y = 240;
    gSpriteClipsJumping[7].w = 810 - gSpriteClipsJumping[7].x;
    gSpriteClipsJumping[7].h = 320 - gSpriteClipsJumping[7].y;

    gSpriteClipsJumping[8].x = 820;
    gSpriteClipsJumping[8].y = 240;
    gSpriteClipsJumping[8].w = 920 - gSpriteClipsJumping[8].x;
    gSpriteClipsJumping[8].h = 320 - gSpriteClipsJumping[8].y;

}

void setSpritesSquat() {
    gSpriteClipsSquat[0].x = 935;
    gSpriteClipsSquat[0].y = 600;
    gSpriteClipsSquat[0].w = 1024 - gSpriteClipsSquat[0].x;
    gSpriteClipsSquat[0].h = 685 - gSpriteClipsSquat[0].y;

    gSpriteClipsSquat[1].x = 830;
    gSpriteClipsSquat[1].y = 600;
    gSpriteClipsSquat[1].w = 195 - gSpriteClipsSquat[1].x;
    gSpriteClipsSquat[1].h = 910 - gSpriteClipsSquat[1].y;

    gSpriteClipsSquat[2].x = 725;
    gSpriteClipsSquat[2].y = 600;
    gSpriteClipsSquat[2].w = 815 - gSpriteClipsSquat[2].x;
    gSpriteClipsSquat[2].h = 685 - gSpriteClipsSquat[2].y;

    gSpriteClipsSquat[3].x = 625;
    gSpriteClipsSquat[3].y = 600;
    gSpriteClipsSquat[3].w = 715 - gSpriteClipsSquat[3].x;
    gSpriteClipsSquat[3].h = 685 - gSpriteClipsSquat[3].y;

    gSpriteClipsSquat[4].x = 515;
    gSpriteClipsSquat[4].y = 600;
    gSpriteClipsSquat[4].w = 610 - gSpriteClipsSquat[4].x;
    gSpriteClipsSquat[4].h = 685 - gSpriteClipsSquat[4].y;

    gSpriteClipsSquat[5].x = 415;
    gSpriteClipsSquat[5].y = 600;
    gSpriteClipsSquat[5].w = 510 - gSpriteClipsSquat[5].x;
    gSpriteClipsSquat[5].h = 685 - gSpriteClipsSquat[5].y;

    gSpriteClipsSquat[6].x = 315;
    gSpriteClipsSquat[6].y = 600;
    gSpriteClipsSquat[6].w = 410 - gSpriteClipsSquat[6].x;
    gSpriteClipsSquat[6].h = 685 - gSpriteClipsSquat[6].y;

    gSpriteClipsSquat[7].x = 210;
    gSpriteClipsSquat[7].y = 600;
    gSpriteClipsSquat[7].w = 310 - gSpriteClipsSquat[7].x;
    gSpriteClipsSquat[7].h = 685 - gSpriteClipsSquat[7].y;

    gSpriteClipsSquat[8].x = 110;
    gSpriteClipsSquat[8].y = 600;
    gSpriteClipsSquat[8].w = 200 - gSpriteClipsSquat[8].x;
    gSpriteClipsSquat[8].h = 685 - gSpriteClipsSquat[8].y;
}

void setSpritesRings() {
    gSpriteClipsRings[0].x = 30;
    gSpriteClipsRings[0].y = 710;
    gSpriteClipsRings[0].w = 65 - gSpriteClipsRings[0].x;
    gSpriteClipsRings[0].h = 750 - gSpriteClipsRings[0].y;

    gSpriteClipsRings[1].x = 65;
    gSpriteClipsRings[1].y = 710;
    gSpriteClipsRings[1].w = 104 - gSpriteClipsRings[1].x;
    gSpriteClipsRings[1].h = 750 - gSpriteClipsRings[1].y;

    gSpriteClipsRings[2].x = 104;
    gSpriteClipsRings[2].y = 710;
    gSpriteClipsRings[2].w = 142 - gSpriteClipsRings[2].x;
    gSpriteClipsRings[2].h = 750 - gSpriteClipsRings[2].y;

    gSpriteClipsRings[3].x = 142;
    gSpriteClipsRings[3].y = 710;
    gSpriteClipsRings[3].w = 182 - gSpriteClipsRings[3].x;
    gSpriteClipsRings[3].h = 750 - gSpriteClipsRings[3].y;

    gSpriteClipsRings[4].x = 30;
    gSpriteClipsRings[4].y = 750;
    gSpriteClipsRings[4].w = 65 - gSpriteClipsRings[4].x;
    gSpriteClipsRings[4].h = 788 - gSpriteClipsRings[4].y;

    gSpriteClipsRings[5].x = 65;
    gSpriteClipsRings[5].y = 750;
    gSpriteClipsRings[5].w = 104 - gSpriteClipsRings[5].x;
    gSpriteClipsRings[5].h = 788 - gSpriteClipsRings[5].y;

    gSpriteClipsRings[6].x = 104;
    gSpriteClipsRings[6].y = 750;
    gSpriteClipsRings[6].w = 142 - gSpriteClipsRings[6].x;
    gSpriteClipsRings[6].h = 788 - gSpriteClipsRings[6].y;

    gSpriteClipsRings[7].x = 142;
    gSpriteClipsRings[7].y = 750;
    gSpriteClipsRings[7].w = 182 - gSpriteClipsRings[7].x;
    gSpriteClipsRings[7].h = 788 - gSpriteClipsRings[7].y;

    gSpriteClipsRings[8].x = 30;
    gSpriteClipsRings[8].y = 788;
    gSpriteClipsRings[8].w = 65 - gSpriteClipsRings[8].x;
    gSpriteClipsRings[8].h = 827 - gSpriteClipsRings[8].y;

    gSpriteClipsRings[9].x = 65;
    gSpriteClipsRings[9].y = 788;
    gSpriteClipsRings[9].w = 104 - gSpriteClipsRings[9].x;
    gSpriteClipsRings[9].h = 827 - gSpriteClipsRings[9].y;

    gSpriteClipsRings[10].x = 104;
    gSpriteClipsRings[10].y = 788;
    gSpriteClipsRings[10].w = 142 - gSpriteClipsRings[10].x;
    gSpriteClipsRings[10].h = 827 - gSpriteClipsRings[10].y;

    gSpriteClipsRings[11].x = 142;
    gSpriteClipsRings[11].y = 788;
    gSpriteClipsRings[11].w = 182 - gSpriteClipsRings[11].x;
    gSpriteClipsRings[11].h = 827 - gSpriteClipsRings[11].y;

    gSpriteClipsRings[12].x = 30;
    gSpriteClipsRings[12].y = 827;
    gSpriteClipsRings[12].w = 65 - gSpriteClipsRings[12].x;
    gSpriteClipsRings[12].h = 865 - gSpriteClipsRings[12].y;

    gSpriteClipsRings[13].x = 65;
    gSpriteClipsRings[13].y = 827;
    gSpriteClipsRings[13].w = 104 - gSpriteClipsRings[13].x;
    gSpriteClipsRings[13].h = 865 - gSpriteClipsRings[13].y;

    gSpriteClipsRings[14].x = 104;
    gSpriteClipsRings[14].y = 827;
    gSpriteClipsRings[14].w = 142 - gSpriteClipsRings[14].x;
    gSpriteClipsRings[14].h = 865 - gSpriteClipsRings[14].y;

    gSpriteClipsRings[15].x = 142;
    gSpriteClipsRings[15].y = 827;
    gSpriteClipsRings[15].w = 182 - gSpriteClipsRings[15].x;
    gSpriteClipsRings[15].h = 865 - gSpriteClipsRings[15].y;
}

void close() {
    //Free loaded images
    gPressTexture.free();


    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int sprits() {
    //Start up SDL and create window
    if (!init()) {
        printf("Failed to initialize!\n");
    } else {
        //Load media
        if (!loadMedia()) {
            printf("Failed to load media!\n");
        } else {
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;
            int frameRunForward = 0;
            int frameRunBack = 0;
            int frameJumping = 0;
            int frameSquat = 0;
            int frameRings = 0;
            //Current rendered texture
            LTexture *currentTexture = NULL;

            //While application is running
            while (!quit) {
                //Handle events on queue
                while (SDL_PollEvent(&e) != 0) {
                    //User requests quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                }

                //Set texture based on current keystate
                const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);


                //Clear screen
                SDL_RenderClear(gRenderer);

                //Render current texture
                if (currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_W]) {
                    //SONIC JUMP
                    //currentTexture = &gPressTexture;
                    SDL_Rect *currentClip = &gSpriteClipsJumping[frameJumping / JUMPING_ANIMATION_FRAMES];
                    currentTexture->render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, currentClip);
//                    std::cout << "FRAME---> JUMP "<< frameJumping / JUMPING_ANIMATION_FRAMES << endl;
                    //Go to next frame
                    if (frameJumping / JUMPING_ANIMATION_FRAMES > JUMPING_ANIMATION_FRAMES) {
                        currentClip = &gSpriteClipsJumping[8];
                        currentTexture->render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, currentClip);
                        --frameJumping;
                    }else if (frameJumping / JUMPING_ANIMATION_FRAMES == 8) {
                        currentClip = &gSpriteClipsJumping[7];
                        currentTexture->render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, currentClip);
                        --frameJumping;
                    }else{
                        ++frameJumping;
                    }

                } else if (currentKeyStates[SDL_SCANCODE_DOWN] || currentKeyStates[SDL_SCANCODE_S]) {
                    //SONIC SE SQUATS
                    //currentTexture = &gPressTexture;
                    SDL_Rect *currentClip = &gSpriteClipsSquat[frameSquat / SQUAT_ANIMATION_FRAMES];
                    currentTexture->render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, currentClip);
//                    std::cout << "FRAME---> SQUAT "<< frameSquat / SQUAT_ANIMATION_FRAMES << endl;
                    //Go to next frame
                    if (frameSquat / SQUAT_ANIMATION_FRAMES > SQUAT_ANIMATION_FRAMES) {
                        currentClip = &gSpriteClipsSquat[8];
                        currentTexture->render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, currentClip);
                        --frameSquat;
                    }else if (frameSquat / SQUAT_ANIMATION_FRAMES == 8) {
                        currentClip = &gSpriteClipsSquat[7];
                        currentTexture->render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, currentClip);
                        --frameSquat;
                    }else{
                        ++frameSquat;
                    }

                } else if (currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A]) {
                    //currentTexture = &gPressTexture;
                    SDL_Rect *currentClip = &gSpriteClipsRunBack[frameRunBack / RUNING_BACK_ANIMATION_FRAMES];
                    currentTexture->render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, currentClip);
                    //std::cout << "FRAME---> LEFT "<< frameRunBack / RUNING_BACK_ANIMATION_FRAMES << endl;
                    //Go to next frame
                    if (frameRunBack / RUNING_BACK_ANIMATION_FRAMES > RUNING_BACK_ANIMATION_FRAMES) {
                        currentClip = &gSpriteClipsRunBack[13];
                        currentTexture->render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, currentClip);
                        --frameRunBack;
                    }else if (frameRunBack / RUNING_BACK_ANIMATION_FRAMES == 13) {
                        currentClip = &gSpriteClipsRunBack[12];
                        currentTexture->render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, currentClip);
                        --frameRunBack;
                    }else{
                        ++frameRunBack;
                    }

                } else if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D]) {
                    //currentTexture = &gPressTexture;
                    SDL_Rect *currentClip = &gSpriteClipsRunForward[frameRunForward / RUNING_FORWARD_ANIMATION_FRAMES];
                    currentTexture->render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, currentClip);
                    //std::cout << "FRAME---> RIGHT "<< frameRunForward / RUNING_FORWARD_ANIMATION_FRAMES << endl;
                    //Go to next frame
                    if (frameRunForward / RUNING_FORWARD_ANIMATION_FRAMES > RUNING_FORWARD_ANIMATION_FRAMES) {
                        currentClip = &gSpriteClipsRunForward[13];
                        currentTexture->render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, currentClip);
                        --frameRunForward;
                    }else if (frameRunForward / RUNING_FORWARD_ANIMATION_FRAMES == 13) {
                        currentClip = &gSpriteClipsRunForward[12];
                        currentTexture->render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, currentClip);
                        --frameRunForward;
                    }else{
                        ++frameRunForward;
                    }

                } else {

                    //SONIC QUIETO
                    currentTexture = &gPressTexture;
                    SDL_Rect *currentClip = &gSpriteClipsWaiting[0];
                    currentTexture->render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, &gSpriteClipsWaiting[0]);

                    frameRunForward = 0;
                    frameRunBack = 0;
                    frameJumping = 0;
                    frameSquat = 0;
                }
                SDL_Rect *ringClip = &gSpriteClipsRings[frameRings / RINGS_ANIMATION_FRAMES];
                currentTexture->render((SCREEN_WIDTH + 200 - ringClip->w) / 2, (SCREEN_HEIGHT - 80 ) / 2, ringClip);

                //Go to next frame
                ++frameRings;

                //Cycle animation
                if( frameRings / RINGS_ANIMATION_FRAMES >= RINGS_ANIMATION_FRAMES ){
                    frameRings = 0;
                }
                //Update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }

    //Free resources and close SDL
    close();
}