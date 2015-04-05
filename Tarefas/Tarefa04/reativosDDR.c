#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#define ARROW_WIDTH 64//296
#define ARROW_HEIGHT 64//214
#define ARROW_PADD 16
#define ARROW_NUMB 9
#define BUTTON_WIDTH 75//296
#define BUTTON_HEIGHT 75//214
#define BUTTON_OFFSET 200
#define BUTTON_PADD 20
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 640

typedef enum { false, true } bool;
typedef enum a_type {
    right,
    down,
    left,
    up
} DDR_type;

typedef struct arrow {
	double y;
	DDR_type type;
	bool activated;
} DDR_arrow;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;
SDL_Texture* button_tex = NULL;
SDL_Event e;

DDR_arrow v_arrow[ARROW_NUMB];
double gameSpeed=0.1;
Uint32 t_now, t_old;

bool init();
bool loadMedia();
void close ();
void iniArrow();
void updateArrow();
void renderArrow ( DDR_arrow *arw);
void createArrow(DDR_arrow *arrow);
void format(DDR_type type,Uint8 *r,Uint8 *g,Uint8 *b,SDL_Rect *renderQuad,double *angle, int obj_width);
void renderButtons();
bool checkColision(DDR_type type);

int main (int argv, char* args[]) {
	char quit = false;
	int i;

	if(!init()) {
		printf("Failed to initialize!\n");
		close();
		return 1;
	}
	if(!loadMedia()){
        printf("Failed to load media!\n");
        return 1;
	}
	t_old=SDL_GetTicks();
    iniArrow();
	while(!quit) { // main loop
		while( SDL_PollEvent(&e)!=0) { //input loop
			if(e.type == SDL_QUIT) {
				quit=true;
            }
			else if(e.type == SDL_KEYDOWN) {
                switch(e.key.keysym.sym){
                    case SDLK_UP:
                        checkColision(up);
                        break;
                    case SDLK_DOWN:
                        checkColision(down);
                        break;
                    case SDLK_LEFT:
                        checkColision(left);
                        break;
                    case SDLK_RIGHT:
                        checkColision(right);
                        break;
                    default:
                        printf("Invalid input!\n");
                        break;
                }
            }
        }
        t_now=SDL_GetTicks();
        updateArrow();

		SDL_SetRenderDrawColor(renderer,255,255,255,255);
		SDL_RenderClear(renderer);
        renderButtons();
        for(i=0;i<ARROW_NUMB;i++){
            renderArrow(&v_arrow[i]);
        }
		SDL_RenderPresent(renderer);
        t_old=t_now;
	}

	close();
	return 0;
}
bool init() {
	if(SDL_Init(SDL_INIT_EVERYTHING)== -1) {
		printf("SDL could not initialize! SDL_Error: %s\n",SDL_GetError());
		return false;
	}
	window = SDL_CreateWindow("reativosDDR",
	                          SDL_WINDOWPOS_CENTERED,
	                          SDL_WINDOWPOS_CENTERED,
	                          SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	if(window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n",SDL_GetError());
		return false;
	}
	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL){
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
	}
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init( imgFlags ) & imgFlags)){
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }
	return true;
}
bool loadMedia (){
    SDL_Surface* loadedSurface = IMG_Load ("arrow.png");
    if(loadedSurface==NULL){
        printf("Unable to load arrow image! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }
    texture=SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if(texture==NULL){
        printf("Unable to create arrow texture! SDL Error: %s\n",SDL_GetError());
        return false;
    }
    loadedSurface = IMG_Load ("arrowbut.png");
    if(loadedSurface==NULL){
        printf("Unable to load button image! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }
    button_tex=SDL_CreateTextureFromSurface(renderer,loadedSurface);
    if(button_tex==NULL){
        printf("Unable to create button texture! SDL Error: %s\n",SDL_GetError());
        return false;
    }
    SDL_FreeSurface(loadedSurface);
    return true;
}
void close () {
    SDL_DestroyTexture(texture);
    texture=NULL;
    SDL_DestroyTexture(button_tex);
    button_tex=NULL;
	SDL_DestroyRenderer(renderer);
	renderer=NULL;
	SDL_DestroyWindow(window);
	window=NULL;
	IMG_Quit();
	SDL_Quit();
}
void iniArrow(){
    int i;
    for(i=0;i<ARROW_NUMB;i++){
        v_arrow[i].y=-((ARROW_HEIGHT+ARROW_PADD)*i)-200;
        createArrow(&v_arrow[i]);
    }
}
void updateArrow() {
    int i;
    for(i=0;i<ARROW_NUMB;i++){
        v_arrow[i].y+=((t_now-t_old)*gameSpeed);
        if(v_arrow[i].y>SCREEN_HEIGHT) {
            if(v_arrow[i].activated==true){
                iniArrow();
                gameSpeed=0.1;
            }
            else {
                v_arrow[i].y=0-(ARROW_HEIGHT+ARROW_PADD);
                createArrow(&v_arrow[i]);
            }
        }
    }
}
void format(DDR_type type,Uint8 *r,Uint8 *g,Uint8 *b,SDL_Rect *renderQuad,double *angle, int obj_width){
    *r=255;*g=255;*b=255;
    switch (type){
        case up: //red
            *angle=270;
            renderQuad->x=(SCREEN_WIDTH/5)*3-obj_width/2;
            *g=0;
            *b=0;
            break;
        case down: //green
            *angle=90;
            renderQuad->x=(SCREEN_WIDTH/5)*2-obj_width/2;
            *r=0;
            *b=0;
            break;
        case right: //blue
            *angle=0;
            renderQuad->x=(SCREEN_WIDTH/5)*4-obj_width/2;
            *r=0;
            *g=0;
            break;
        case left: //yellow
            *angle=180;
            renderQuad->x=(SCREEN_WIDTH/5)*1-obj_width/2;
            *b=0;
            break;
    }
}
void renderArrow ( DDR_arrow *arw ){
    SDL_Rect renderQuad= {0,arw->y,ARROW_WIDTH,ARROW_HEIGHT};
    double angle;
    Uint8 r,g,b;
    if(arw->activated==true){
        format(arw->type,&r,&g,&b,&renderQuad,&angle,ARROW_WIDTH);
        SDL_SetTextureColorMod(texture,r,g,b);
        SDL_RenderCopyEx(renderer,texture,NULL,&renderQuad,angle,NULL,SDL_FLIP_NONE);
    }
}
void renderButtons(void){
    SDL_Rect renderQuad= {0,SCREEN_HEIGHT-BUTTON_OFFSET,BUTTON_WIDTH,BUTTON_HEIGHT};
    double angle;
    int i;
    Uint8 r,g,b;
    for (i=0;i<4;i++){
        format(i,&r,&g,&b,&renderQuad,&angle,BUTTON_WIDTH);
        SDL_SetTextureColorMod(button_tex,r,g,b);
        SDL_RenderCopyEx(renderer,button_tex,NULL,&renderQuad,angle,NULL,SDL_FLIP_NONE);
    }
}
void createArrow(DDR_arrow *arrow){
    arrow->activated=true;
    arrow->type=rand()%4;
}
bool checkColision(DDR_type type){
    int i;
    for(i=0;i<ARROW_NUMB;i++){
        if(type==v_arrow[i].type && v_arrow[i].activated==true){
            if(v_arrow[i].y>SCREEN_HEIGHT-BUTTON_OFFSET-BUTTON_PADD
            && v_arrow[i].y+ARROW_HEIGHT<SCREEN_HEIGHT-BUTTON_OFFSET+BUTTON_HEIGHT+BUTTON_PADD){
                v_arrow[i].activated=false;
                gameSpeed+=0.003;
                printf("sim!\n");
                return true;
            }
        }
    }
    printf("nao!!!!\n");
    return false;
}
