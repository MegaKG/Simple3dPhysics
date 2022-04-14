#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

union dconv {
    char BData[8];
    double D;
};
union lconv {
    char BData[8];
    uint64_t L;
};


int main(){
    char readc;

    //Read header up past the newline
    while (1){
        readc = getc(stdin);
        //printf("%i\n",readc);
        if (readc == '\n'){
            break;
        }
    }

    printf("Begin Decode\n\n");

    union dconv DoubleConvert;
    union lconv LongConvert;
    uint64_t i = 0;
    uint64_t bodies;

    double tmpx, tmpy, tmpz;

    while (1){
        //Here we use the readc char as an 8 bit int
        readc = read(STDIN_FILENO,&LongConvert.BData,8);
        if (readc < 8){
            printf("EOF Read %i, not 8\n",readc);
            break;
        }

        printf("Frame: %li\n",LongConvert.L);
        
        read(STDIN_FILENO,&LongConvert.BData,8);
        bodies = LongConvert.L;
        printf("Bodies: %li\n\n",bodies);

        for (i = 0; i < bodies; i++){
            read(STDIN_FILENO,&LongConvert.BData,8);printf("Body: %li\n",LongConvert.L);

            read(STDIN_FILENO,&DoubleConvert.BData,8);printf("Mass: %lf kg\n",DoubleConvert.D);

            read(STDIN_FILENO,&DoubleConvert.BData,8);printf("Radius: %lf m\n",DoubleConvert.D);

            read(STDIN_FILENO,&DoubleConvert.BData,8);tmpx = DoubleConvert.D;
            read(STDIN_FILENO,&DoubleConvert.BData,8);tmpy = DoubleConvert.D;
            read(STDIN_FILENO,&DoubleConvert.BData,8);tmpz = DoubleConvert.D;
            printf("Position: X: %lf Y: %lf Z: %lf m\n",tmpx,tmpy,tmpz);

            read(STDIN_FILENO,&DoubleConvert.BData,8);tmpx = DoubleConvert.D;
            read(STDIN_FILENO,&DoubleConvert.BData,8);tmpy = DoubleConvert.D;
            read(STDIN_FILENO,&DoubleConvert.BData,8);tmpz = DoubleConvert.D;
            printf("Velocity: X: %lf Y: %lf Z: %lf  m/s\n",tmpx,tmpy,tmpz);

            read(STDIN_FILENO,&DoubleConvert.BData,8);printf("Kinetic Energy: %lf J\n",DoubleConvert.D);

            read(STDIN_FILENO,&DoubleConvert.BData,8);printf("Lorentz Factor: %lf m\n",DoubleConvert.D);

            printf("\n");
        }

        printf("\n\n");
    }




}
