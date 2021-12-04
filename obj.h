typedef struct {
    int collision,
        positionCollisionX,
        positionCollisionY;
} element;

element putcircle(int id, float radius, float posX, float posY){
    float centerX = posX,
          centerY = posY;
        
    int maxX = (centerX + radius) * 2,
        maxY = (centerY + radius) * 2;

    int minX = (centerX - radius) / 2,
        minY = (centerY - radius) / 2;

    element elm = {-1, 0, 0};

    for(int i = minY; i < maxY; i++){
        for(int j = minX; j < maxX; j++){
            float x = (float)j - centerX;
            float y = (float)i - centerY;
            x *= CHARCORR;
            if(pow(x, 2) + pow(y, 2) <= pow(radius, 2)){
                if(i <= HEIGHT && i >= 0 && j <= WIDTH && j >= 0){
                    if(field[i][j].id != -1){
                        elm.collision = field[i][j].id;
                        if(j < centerX - radius + 1){
                            elm.positionCollisionX = -1; // TOP
                        }else if(j > centerX + radius - 1){
                            elm.positionCollisionX = 1; // BOTTOM
                        }
                        if(i < centerY - radius + 1){
                            elm.positionCollisionY = -1;
                        }else if(i > centerY + radius - 1){
                            elm.positionCollisionY = 1;
                        }
                    }else{
                        field[i][j].id = id;
                        field[i][j].ch = BASICCHAR;
                    }
                }
            }
        }
    }

    return elm;
}

void putsquare(float size, float posX, float posY, float alpha){
    float centerX = posX,
          centerY = posY;
        
    float maxX = centerX + size,
          maxY = centerY + size;

    float minX = centerX - size,
          minY = centerY - size;

    float angle = alpha * PI / 180; // радианы

    float maxXCorr = maxX,
          minXCorr = minX;
    
    float maxYCorr = maxY,
          minYCorr = minY;

    if(cos(angle) < 0.8 && cos(angle) > -0.25){
        maxXCorr *= CHARCORR;
        minXCorr *= CHARCORR;
        centerX *= CHARCORR;
    }else{
        maxYCorr *= CHARCORR;
        minYCorr *= CHARCORR;
        centerY *= CHARCORR;
    }

    for(int i = minY / 2; i < maxY * 2; i++){
        for(int j = minX / 2; j < maxX * 2; j++){
            if(j <= maxXCorr && j >= minXCorr && i <= maxYCorr && i >= minYCorr){
                int x = round(((float)j - centerX) * cos(angle) - ((float)i - centerY) * sin(angle) + posX);
                int y = round(((float)j - centerX) * sin(angle) + ((float)i - centerY) * cos(angle) + posY);
                if(y <= HEIGHT && y >= 0 && x <= WIDTH && x >= 0){
                    field[y][x].ch = BASICCHAR;
                }
            }
        }
    }
}