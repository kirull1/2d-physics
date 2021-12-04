#define ACCEGRAVITY 9.81f // Acceleration of gravity
#define MAXSPEED 50.0f // Maximum speed
#define SLOWDOWNVECTOR 0.1f // Slow vector change
#define RHO 0.85f // Coefficient of restitution
#define RESISTANCEAIR 0.0001f // Some resistance when moving
#define EJECTION 1.0f // The force of pushing out of the object

void light(float radius, float posX, float posY, short diffusion){
    int len = strlen(CHARSPATTERN);

    float centerX = posX,
          centerY = posY;

    int maxX = (centerX + radius) * 2,
        maxY = (centerY + radius) * 2;

    int minX = (centerX - radius) / 2,
        minY = (centerY - radius) / 2;

    for(int i = minY; i < maxY; i++){
        for(int j = minX; j < maxX; j++){
            float x = (float)j - centerX;
            float y = (float)i - centerY;

            x *= CHARCORR;

            if(pow(x, 2) + pow(y, 2) <= pow(radius, 2) && field[i][j].ch != SPACECHAR){
                int d = sqrt(pow(j - centerX, 2) + pow(i - centerY, 2));
                field[i][j].ch = CHARSPATTERN[(d / diffusion) % len];
            }
        }
    }
}

void movingvector(float correctSpeed, float *vector){
    *vector += SLOWDOWNVECTOR * correctSpeed;
}

void gravityFall(float *correctSpeed){
    if(*correctSpeed < MAXSPEED){
        *correctSpeed += (float)TIMEPAUSE / 1000 * (float)ACCEGRAVITY;
    }
}

void bounce(float *correctSpeed, float direction){
    if(direction == 0){
        *correctSpeed = *correctSpeed * RHO;
    }else{
        *correctSpeed = fabsf(*correctSpeed * RHO) * direction;
    }
}

void vectorExchange(float *speed1, float *speed2, float weight1, float weight2){
    float speedTemp = (2.0f * weight2 * (*speed2) + (*speed1) * (weight1 - weight2)) / (weight1 + weight2);
    *speed2 = (2.0f * weight1 * (*speed1) + (*speed2) * (weight2 - weight1)) / (weight1 + weight2);
    *speed1 = speedTemp;
}

void windage(int size, float *speed){
    *speed -= (float)size * RESISTANCEAIR * (float)(*speed <= 0 ? -1 : 1);
}

void ejection(float *point, float direction){
    *point += EJECTION * direction;
}