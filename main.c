#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h> 

#define WIDTH 100
#define HEIGHT 50
#define TIMEPAUSE 10
#define CHARCORR 12.0f / 24.0f
#define PI 3.14159265359

#define BASICCHAR '@'
#define SPACECHAR ' '
#define CHARSPATTERN ".,-~:;=!*#$@"

typedef struct {
    char ch; // char
    int id;  // point id
} map;

map field[HEIGHT][WIDTH];
char mapval[HEIGHT][WIDTH];

void init(){
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            field[i][j].ch = SPACECHAR;
            field[i][j].id = -1;
        }
    }
}

#include "obj.h"
#include "physics.h"

void show(){
    for(int i = 0; i < HEIGHT; i++){
        if(i + 1 < HEIGHT){
            field[i][WIDTH - 1].ch = '\n';
        }else{
            field[i][WIDTH - 1].ch = '\0';
        }
    } 
}

void withdraw(){
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            mapval[i][j] = field[i][j].ch;
        }
    }
    return mapval;
}

void setwindowsize(){
    char mode[512];
    snprintf(mode, sizeof mode, "mode %d,%d", WIDTH, HEIGHT);
    system(mode);
    SMALL_RECT WinRect = {0, 0, WIDTH, HEIGHT};
    SMALL_RECT *WinSize = &WinRect;
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, WinSize);
}

void setcur(int x, int y){
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/*
 * 
 *  Type:
 *  1 - circle
 *  2 - square
 * 
 */

typedef struct{
    float x,                // cord X
          y,                // cord Y
          speedX,           // speed x
          speedY;           // speed y
    unsigned short size;    // size
    unsigned short type;    // type obj
    unsigned int weight;    // positive object mass
} obj;


int main(int argc, char const *argv[])
{
    setwindowsize();
    float t = 0;

    obj objs[] = {
        {20, 20, 1, 0, 3, 1, 20},
        {80, 17, -2.5, 0, 3, 1, 6}
    };

    do
    {   
        setcur(0,0);
        init();

        for(int i = 0; i < sizeof objs / sizeof *objs; i++){

            element elm;

            switch(objs[i].type){
                case 1:
                    elm = putcircle(i, objs[i].size, objs[i].x, objs[i].y);
                    break;
                case 2:
                    putsquare(objs[i].size, objs[i].x, objs[i].y, 0);
                    break;
            }
            
            if(elm.collision != -1){
                vectorExchange(&objs[i].speedX, &objs[elm.collision].speedX, objs[i].weight, objs[elm.collision].weight);
                vectorExchange(&objs[i].speedY, &objs[elm.collision].speedY, objs[i].weight, objs[elm.collision].weight);
                
                ejection(&objs[i].x, elm.positionCollisionX * -1);
                ejection(&objs[elm.collision].x, elm.positionCollisionX);

                ejection(&objs[i].y, elm.positionCollisionY * -1);
                ejection(&objs[elm.collision].y, elm.positionCollisionY);
            }

            if(objs[i].y + objs[i].size + 2 <= HEIGHT){
                gravityFall(&objs[i].speedY);

                windage(objs[i].size, &objs[i].speedY);
                windage(objs[i].size, &objs[i].speedX);
            }else{
                bounce(&objs[i].speedY, -1);
                bounce(&objs[i].speedX, 0);
            }

            movingvector(objs[i].speedY, &objs[i].y);
            movingvector(objs[i].speedX, &objs[i].x);

        }

        //light(9, WIDTH / 2 + cos(t * 0.01), HEIGHT / 2 + sin(t * 0.01), 2);
        show();
        withdraw();
        printf("%s", mapval);
        Sleep(TIMEPAUSE);
        t += 1;
    } while (GetKeyState(VK_ESCAPE) >= 0);
    return 0;
}
