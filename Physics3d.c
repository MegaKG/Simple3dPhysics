//gcc Physics3d.c -lm
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "StringInputs.h"

//This Struct is the base object in the physics engine
//It Contains all values that can be set for a body in the simulation
struct body {
    //These can be configured
    double mass;
    double radius;

    double xpos;
    double ypos;
    double zpos;

    double xvel;
    double yvel;
    double zvel;

    char grav; //Gravity Enable Flag

    //These Flags indicate whether a body becomes an infinite surface (EG, Simulates Gravity Plane or Box for Gas Sim)
    char allx; //All X Flag
    char ally; //All Y Flag
    char allz; //All Z Flag


    //The Charge, non zero = Attraction or Repulsion
    double charge;

    //TODO
    struct body* BoundTo; //Rigid Body Physics [TODO]

    //Here are values only for output [DO NOT EDIT]
    double energy;
    double lorentz;
    double rel_mass;
    uint64_t ID;

    //Hidden Aspects, These are used only for calculations [DO NOT EDIT]
    double _newxvel;
    double _newyvel;
    double _newzvel;
};

//Constants
double pi = acos(-1);
double g = 9.8;
double G = 6.67 * pow(10,-11);
double q = 1.6 * pow(10,-19);
double c = 3.0 * pow(10,8);
double k = 9.0 * pow(10,9);



//This contains the initialised bodies
struct body* Bodies = NULL;
uint64_t bodycount = 0;

//This contains global config
struct config {
    double timeframe;
    uint64_t iterations;
} MainConfig;

//Converters
union dconv {
    char BData[8];
    double D;
};
union lconv {
    char BData[8];
    uint64_t L;
};

//Loading Code, this is where the above variable are initialised
void loadBodies(char** args){
    //First get main commandline settings
    MainConfig.timeframe = stod(args[3]);
    MainConfig.iterations = stoui64(args[2]);

    //The Converters
    union dconv DoubleConverter;
    union lconv LongConverter;

    //Open file
    FILE* InputFile = fopen(args[1],"rb");

    //Now Load Bodies
    fread(&LongConverter.BData,1,8,InputFile);
    uint64_t amm = LongConverter.L; 

    //Assign Main Body Count and Memory
    bodycount = amm;
    struct body* NewBodies = (struct body*)malloc(amm * sizeof(struct body));
    Bodies = NewBodies;
    
    
    printf("Loading %li Bodies from %s, iterating %li timeframe %lf\n",amm,args[1],MainConfig.iterations,MainConfig.timeframe);
    for (uint64_t i = 0; i < amm; i++){
        fread(&DoubleConverter.BData,1,8,InputFile); Bodies[i].mass = DoubleConverter.D;
        fread(&DoubleConverter.BData,1,8,InputFile); Bodies[i].xpos = DoubleConverter.D;
        fread(&DoubleConverter.BData,1,8,InputFile); Bodies[i].ypos = DoubleConverter.D;
        fread(&DoubleConverter.BData,1,8,InputFile); Bodies[i].zpos = DoubleConverter.D;
        fread(&DoubleConverter.BData,1,8,InputFile); Bodies[i].xvel = DoubleConverter.D;
        fread(&DoubleConverter.BData,1,8,InputFile); Bodies[i].yvel = DoubleConverter.D;
        fread(&DoubleConverter.BData,1,8,InputFile); Bodies[i].zvel = DoubleConverter.D;
        fread(&DoubleConverter.BData,1,8,InputFile); Bodies[i].radius = DoubleConverter.D;
        Bodies[i].grav = fgetc(InputFile);
        fread(&DoubleConverter.BData,1,8,InputFile); Bodies[i].charge = DoubleConverter.D;
        Bodies[i].allx = fgetc(InputFile);
        Bodies[i].ally = fgetc(InputFile);
        Bodies[i].allz = fgetc(InputFile);
        Bodies[i].ID = i;
        
    }

    fclose(InputFile);
    
    

}


//Macro for the Distance Formula
double distance(double x1, double y1, double z1, double x2, double y2, double z2){
    return pow(pow(x2-x1,2) + pow(y2-y1,2) + pow(z2-z1,2),0.5);
}



void dumpLong(uint64_t Inp){
    union lconv LongConvert;
    LongConvert.L = Inp;
    for (short i = 0; i < 8; i++){
        putc(LongConvert.BData[i],stdout);
    }
}

void dumpDouble(double Inp){
    union dconv DoubleConvert;
    DoubleConvert.D = Inp;
    for (short i = 0; i < 8; i++){
        putc(DoubleConvert.BData[i],stdout);
    }
}

void outputData(){
    //Enables human readable output
    const char debug = 0;

    //Ensure static so retains value
    static uint64_t FrameCount = 0; //The Zero is only set on startup, then incremented during runtime

    
    if (debug){
        //Human Readable Debug
        printf("Frame: %li\n",FrameCount);
        printf("Bodies: %li\n",bodycount);

        for (uint64_t j = 0; j < bodycount; j++){
            //ID
            printf("ID: %li\n",Bodies[j].ID);

            //Mass
            printf("Mass: %lf\n",Bodies[j].rel_mass);
            
            //Radius
            printf("Radius: %lf\n",Bodies[j].radius);

            //Xpos
            printf("XPOS: %lf\n",Bodies[j].xpos);

            //Ypos
            printf("YPOS: %lf\n",Bodies[j].ypos);

            //Zpos
            printf("ZPOS: %lf\n",Bodies[j].zpos);
            
            //Xvelocity
            printf("XVEL: %lf\n",Bodies[j].xvel);

            //Yvelocity
            printf("YVEL: %lf\n",Bodies[j].yvel);

            //Zvelocity
            printf("ZVEL: %lf\n",Bodies[j].zvel);

            //Kinetic Energy
            printf("KE: %lf\n",Bodies[j].energy);

            //lorentz factor
            printf("LORENTZ: %lf\n",Bodies[j].lorentz);

            printf("\n");
        }
        printf("\n\n");

    }
    else {   
        //Output
        //The Frame Number
        dumpLong(FrameCount);
        
        //The Count of Bodies
        dumpLong(bodycount);
        
        for (uint64_t j = 0; j < bodycount; j++){
            //ID
            dumpLong(Bodies[j].ID);

            //Mass
            dumpDouble(Bodies[j].rel_mass);
            
            //Radius
            dumpDouble(Bodies[j].radius);

            //Xpos
            dumpDouble(Bodies[j].xpos);

            //Ypos
            dumpDouble(Bodies[j].ypos);

            //Zpos
            dumpDouble(Bodies[j].zpos);
            
            //Xvelocity
            dumpDouble(Bodies[j].xvel);

            //Yvelocity
            dumpDouble(Bodies[j].yvel);

            //Zvelocity
            dumpDouble(Bodies[j].zvel);

            //Kinetic Energy
            dumpDouble(Bodies[j].energy);

            //lorentz factor
            dumpDouble(Bodies[j].lorentz);
        }
        fflush(stdout);
        
        }
    FrameCount += 1;



}

//This does all the maths for each timeframe
void timeTick(double time){
    //Acceleration 
    double xaccel = 0;
    double yaccel = 0;
    double zaccel = 0;

    //Velocity Calc
    double netveloc = 0; 

    //Distance between points
    double netdist = 0;

    //Iterators, i = index of body we are working on, j = the one we are comparing to
    uint64_t i = 0;
    uint64_t j = 0;

    //Lorentz Factor
    double gamma;

    //Forces Applied
    double netforce; //Broken into the three following
    double netxforce;
    double netyforce;
    double netzforce;

    //The Unit Vector
    double UnitX;
    double UnitY;
    double UnitZ;

    
    for (i = 0; i < bodycount; i++){
        //Initialise Values
        Bodies[i].energy = 0;

        //Calculate Lorentz Factor
        netveloc = pow(pow(Bodies[i].xvel,2) + pow(Bodies[i].yvel,2) + pow(Bodies[i].zvel,2),0.5);
        gamma = 1.0 / pow(1 - (pow(netveloc,2)/pow(c,2)),0.5);
        Bodies[i].lorentz = gamma;
        Bodies[i].rel_mass = Bodies[i].mass * gamma;

        //Clear the Net force
        netxforce = 0;
        netyforce = 0;
        netzforce = 0;

        //Update the Internal Velocity Values to be applied in the next cycle
        Bodies[i]._newxvel = Bodies[i].xvel;
        Bodies[i]._newyvel = Bodies[i].yvel;
        Bodies[i]._newzvel = Bodies[i].zvel;

        for (j = 0; j < bodycount; j++){
            if (i != j){
                //Get Distance between Body i and Body j
                netdist = distance(Bodies[i].xpos,Bodies[i].ypos,Bodies[i].zpos,Bodies[j].xpos,Bodies[j].ypos,Bodies[j].zpos);
                
                //Find the Unit Vector
                UnitX = (Bodies[i].xpos - Bodies[j].xpos)/netdist;
                UnitY = (Bodies[i].ypos - Bodies[j].ypos)/netdist;
                UnitZ = (Bodies[i].zpos - Bodies[j].zpos)/netdist;
               

                //Do Collision Physics
                //printf("Sep By %lf\n",netdist);
                if (netdist < fabs(Bodies[i].radius + Bodies[j].radius)){
                    //printf("Collision!\n");
                    //Compute X
                    Bodies[i]._newxvel = (((Bodies[i].mass - Bodies[j].mass)*Bodies[i].xvel) + (2 * Bodies[j].mass * Bodies[j].xvel)) / (Bodies[i].mass + Bodies[j].mass);

                    //Compute Y
                    Bodies[i]._newyvel = (((Bodies[i].mass - Bodies[j].mass)*Bodies[i].yvel) + (2 * Bodies[j].mass * Bodies[j].yvel)) / (Bodies[i].mass + Bodies[j].mass);

                    //Compute Z
                    Bodies[i]._newzvel = (((Bodies[i].mass - Bodies[j].mass)*Bodies[i].zvel) + (2 * Bodies[j].mass * Bodies[j].zvel)) / (Bodies[i].mass + Bodies[j].mass);

                }


                //Calculate Forces
                //Apply Forces f = ma

                //If Gravity
                if (Bodies[j].grav){
                    //printf("Gravity!\n");
                    //f = GmM / r^2
                    netforce = (G * Bodies[i].mass * Bodies[j].mass) / pow(netdist,2);
                    
                    //Break up the net force into components
                    netzforce += UnitZ*netforce;
                    netyforce += UnitY*netforce;
                    netxforce += UnitX*netforce;

                }

                //If Charge
                if (Bodies[j].charge != 0){
                    //printf("Charge!\n");
                    // f = kqq / r^2
                    netforce = (k * Bodies[j].charge * Bodies[i].charge) / pow(netdist,2);

                    //Invert Force If Same Sign to repel charges
                    if ((Bodies[j].charge > 0) && (Bodies[i].charge > 0)){
                        netforce = netforce * -1;
                    }
                    if ((Bodies[j].charge < 0) && (Bodies[i].charge < 0)){
                        netforce = netforce * -1;
                    }

                    //Break up the net force into components
                    netzforce += UnitZ*netforce;
                    netyforce += UnitY*netforce;
                    netxforce += UnitX*netforce;
                }

            }
        }
        //Apply Forces to Accels
        //F=ma
        xaccel = netxforce / Bodies[i].mass;
        yaccel = netyforce / Bodies[i].mass;
        zaccel = netzforce / Bodies[i].mass; 
        //printf("Accel: %lf %lf %lf\n",xaccel,yaccel,zaccel);
        

        //Update Velocities with acceleration
        Bodies[i]._newxvel += (xaccel * time);
        Bodies[i]._newyvel += (yaccel * time);
        Bodies[i]._newzvel += (zaccel * time);

        

        //Update Energy with Reletivistic Kinetic Energy
        // Ek = (gamma -1)mc^2
        //KE By Newtonian Maths
        if (Bodies[i].lorentz == 1.0){
            Bodies[i].energy = 0.5 * Bodies[i].mass * pow(netveloc,2);
        }
        //KE Through Special Relativity
        else {
            Bodies[i].energy = (Bodies[i].lorentz - 1) * Bodies[i].mass * pow(c,2);
        }
        
       
    }


    //Move code is seperate, this avoids issues with values changing during earlier calculations
    for (i = 0; i < bodycount; i++){
        //Update Velocities
        Bodies[i].xvel = Bodies[i]._newxvel;
        Bodies[i].yvel = Bodies[i]._newyvel;
        Bodies[i].zvel = Bodies[i]._newzvel;


        //Update Locations
        Bodies[i].xpos += (Bodies[i].xvel * time);
        Bodies[i].ypos += (Bodies[i].yvel * time);
        Bodies[i].zpos += (Bodies[i].zvel * time);
    }

}

int main(int argc, char** argv){
    if (argc > 1){
        loadBodies(argv);

        //Iterate over frames specified. Test value at the moment [TEST]
        for (uint64_t i = 0; i < MainConfig.iterations; i++){
            outputData();
            timeTick(MainConfig.timeframe);
        }
    }

    else {
        printf("Usage: %s <Config File> <Iterations> <Timespan> \nEG: %s Test.phys 200 0.1\n",argv[0],argv[0]);
    }
    

}