#include <stdio.h>
#include "brick.h"
#include <unistd.h>

#define Sleep( msec ) usleep(( msec ) * 1000 ) /* Definerar sleep, Sleep(1000)= 1 sekund */

#define MOTOR_RIGHT     OUTB
#define MOTOR_LEFT      OUTA
#define MOTOR_C         OUTC
#define MOTOR_D             OUTD
#define SENSOR_TOUCH	IN1
#define SENSOR_US	    IN2
#define SENSOR_GYRO	    IN3
#define MOTOR_BOTH     	( MOTOR_LEFT | MOTOR_RIGHT ) /* Bitvis ELLER ger att båda motorerna styrs samtidigt */
#define MOTOR_ALL       ( MOTOR_BOTH | MOTOR_C ) // Båda motorer och lyft 

int max_hastighet;         /* variabel för max hastighet på motorn */
POOL_T touchSensor;
POOL_T sensor_us;
int usValue = 0;
POOL_T gyroSensor;
int gyroValue0 = 0;


void resetGyro();
void turn1();
void driveToWall40();
void driveToWall30();
void turnLeft();
void drive125();
void turn2();
void dropOff();


int main()
{
	if ( !brick_init()) return ( 1 ); /* Initialiserar EV3-klossen */
	printf( "*** ( EV3 ) Hello! ***\n" );
	Sleep( 2000 );

	if ( tacho_is_plugged( MOTOR_BOTH, TACHO_TYPE__NONE_ ))
	{
        	max_hastighet = tacho_get_max_speed( MOTOR_LEFT, 0 );	/* Kollar maxhastigheten som motorn kan ha */
        	tacho_reset( MOTOR_BOTH );
    	}
	else
	{
		printf( "Anslut vänster motor i port A,\n"
		       "Anslut höger motor i port B.\n");
		brick_uninit();
	}
//--------Allt ovanför detta i main kommer från exempelMotorTouch.c---------------------------------------------------------------------------------------//


    //TOUCH-SENSOR//
    touchSensor = sensor_search( LEGO_EV3_TOUCH ); // Registrerar en touch sensor på touchSensor-variabeln
	touch_set_mode_touch(touchSensor); // Anger vilken "mode" sensorn skall ha

    //GYRO//
	gyroSensor = sensor_search(LEGO_EV3_GYRO);
    sensor_set_mode(gyroSensor, LEGO_EV3_GYRO_GYRO_G_AND_A);

    //US-SENSOR//
	sensor_us=sensor_search(LEGO_EV3_US);
	us_set_mode_us_dist_cm(sensor_us);	



    resetGyro();    //Anropar funktion för reset gyro

	turn1();    //Anropar funktion för att rotera och hitta närmsta vägg
	
    driveToWall40();    //Anropar funktion som kör mot väggen och stanna 40 cm från vägg
    
    turnLeft(); //Anropar funktion för att svänga 90 grader vänster 
    
    drive125(); //Anropar funktion som kör framåt 125cm

    turn2();    //Anropar funktion för att rotera och hitta närmsta vägg

    driveToWall40();    //Anropar funktion som kör mot väggen och stanna 40 cm från vägg

    turnLeft(); //Anropar funktion för att svänga 90 grader vänster 

    drive125(); //Anropar funktion som kör framåt 125cm

    turn2();    //Anropar funktion för att rotera och hitta närmsta vägg

    driveToWall30();    //Anropar funktion som kör mot väggen och stanna 30 cm från vägg

    dropOff();  //Anropar funktion för att släppa av bok





//-----------------------------Kod nedanför detta kommer från exempelMotorTouch.c---------------------------------------------------------//
	while(!sensor_get_value(0, touchSensor, 0)); //Så länge touch-sensorn inte är intryckt kommer while-loopen köras
	tacho_stop( MOTOR_BOTH );

	printf( "dying...\n" );
	brick_uninit();

    	return 0;
//-----------------------------Kod ovanför detta kommer från exempelMotorTouch.c---------------------------------------------------------//

}



//---------FUNKTIONER----------------------------------//

// Reset gyro
void resetGyro() {
    Sleep(1000);
    sensor_set_mode(gyroSensor,LEGO_EV3_GYRO_GYRO_RATE);
    sensor_set_mode(gyroSensor,LEGO_EV3_GYRO_GYRO_G_AND_A);
    Sleep(1000);
}

//Rotera och hitta närmsta vägg 
	void turn1()
	{
        resetGyro();

		int min_distance = sensor_get_value(0, sensor_us, 0);
		int rotation = sensor_get_value(0, gyroSensor, 0) ;  
		printf("%d \n", min_distance);
		printf("%d \n", rotation);

		tacho_set_speed_sp(MOTOR_RIGHT, max_hastighet *( -0.04)); //Roterar med 4% av maxhastighet
		tacho_set_speed_sp(MOTOR_LEFT, max_hastighet * 0.04);
			tacho_run_forever(MOTOR_BOTH);

        int rotDist;
        int sensor;
		while(sensor_get_value(0, gyroSensor, 0) < 360) // När gyro sensor värdet är mindre än 360 så fortsätter den att rotera
		{	
            rotDist = sensor_get_value(0, gyroSensor, 0);
            sensor = sensor_get_value(0, sensor_us, 0);
            printf("%d, %d \n", sensor, rotDist);

			if(min_distance > sensor) // Om akutellt avstånd är mindre än minsta avstånd så uppdateras minsta avstånd + gyrovärde 
			{
			    rotation = rotDist; 
			    min_distance = sensor;
			}
        }

        while((rotation + 358) >= sensor_get_value(0, gyroSensor,0))  //När det inlästa gyrovärdet för minsta distansen + 360 blir större eller lika med aktuellt gyrovärde stannar roboten
        { 
            rotDist = sensor_get_value(0, gyroSensor, 0);
            sensor = sensor_get_value(0, sensor_us, 0);
            printf("%d, %d \n", sensor, rotDist);
        }
		tacho_stop( MOTOR_BOTH );
        Sleep( 3000 );
	}


//Kör mot vägg och stanna 40 cm från vägg 
void driveToWall40()
{
    tacho_set_speed_sp( MOTOR_BOTH, max_hastighet * 0.2 ); //20% av maxhastiheten
    tacho_run_forever(  MOTOR_BOTH );

    while(sensor_get_value(0, sensor_us, 0) > 400) {}  // Om avståndet till vägg är större än 40 cm - kör fram mot vägg

    tacho_set_speed_sp( MOTOR_BOTH, max_hastighet * (-0.3));
    tacho_run_forever(  MOTOR_BOTH );
        
    while(sensor_get_value(0, sensor_us, 0) < 400) {} // Om avståndet till vägg är mindre än 40 cm - backa

    tacho_stop(MOTOR_BOTH);
    Sleep(3000);
}

// Sväng 90 grader vänster
void turnLeft()
    {
    int rotate = sensor_get_value(0, gyroSensor, 0) - 90;  // Atuellt gyrovärde - 90 grader
        tacho_set_speed_sp(MOTOR_RIGHT, max_hastighet * (0.1));
        tacho_set_speed_sp(MOTOR_LEFT, max_hastighet * (-0.1));
        tacho_run_forever(  MOTOR_BOTH );


     while(sensor_get_value(0, gyroSensor, 0) >= rotate) //Rotera sålänge aktuell gyrovärde är större eller lika med rotate (gyrovärde - 90)
    {

        }
    tacho_stop(MOTOR_BOTH);
    }

//Kör framåt 125 cm
void drive125(){
    tacho_set_speed_sp(MOTOR_BOTH, max_hastighet * 0.4); // 40 % av maxhastighet
    tacho_run_forever( MOTOR_BOTH );
    Sleep(6000); 
    tacho_stop(MOTOR_BOTH);
    }

//Kör mot vägg och stanna 30 cm framför
void driveToWall30(){
    tacho_set_speed_sp( MOTOR_BOTH, max_hastighet * 0.2 ); // 20% av maxhstighet
    tacho_run_forever(  MOTOR_BOTH );

    while(sensor_get_value(0, sensor_us, 0) > 300) {}  // Om avståndet till vägg är större än 30 cm - kör fram mot vägg

    tacho_set_speed_sp( MOTOR_BOTH, max_hastighet * (-0.3));
    tacho_run_forever(  MOTOR_BOTH );
        
    while(sensor_get_value(0, sensor_us, 0) < 300) {} // Om avståndet till vägg är mindre än 30 cm - backa

    tacho_stop(MOTOR_BOTH);
    Sleep(3000);
}
// Fäll ner lyft
void dropOff(){
    tacho_set_speed_sp(MOTOR_C, max_hastighet * (-0.5));  // Kör motor C (Lyften)
    tacho_run_forever( MOTOR_C );
    Sleep(1000);
    tacho_stop(MOTOR_BOTH);
    }

//Hitta närmsta vägg
    void turn2()
	{
        resetGyro();

		int min_distance = sensor_get_value(0, sensor_us, 0);
		int rotation = sensor_get_value(0, gyroSensor, 0) ;  
		printf("%d \n", min_distance);
		printf("%d \n", rotation);

		tacho_set_speed_sp(MOTOR_RIGHT, max_hastighet *( -0.04)); //Roterar med 4% av maxhastighet
		tacho_set_speed_sp(MOTOR_LEFT, max_hastighet * 0.04);
			tacho_run_forever(MOTOR_BOTH);

        int rotDist;
        int sensor;
		while((rotation+ 360) > sensor_get_value(0, gyroSensor, 0)) // När gyro sensor värdet är mindre än(rotation + 360)så fortsätter den att rotera
		{	
            rotDist = sensor_get_value(0, gyroSensor, 0);
            sensor = sensor_get_value(0, sensor_us, 0);
            printf("%d, %d \n", sensor, rotDist);

			if(min_distance > sensor) // Om akutellt avstånd är mindre än minsta avstånd så uppdateras minsta avstånd + gyrovärde 
			{
			    rotation = rotDist; 
			    min_distance = sensor;
			}
        }

        while((rotation + 358) >= sensor_get_value(0, gyroSensor,0))  //När det inlästa gyrovärdet för minsta distansen + 360 blir större eller lika med aktuellt gyrovärde stannar roboten
        { 
            rotDist = sensor_get_value(0, gyroSensor, 0);
            sensor = sensor_get_value(0, sensor_us, 0);
            printf("%d, %d \n", sensor, rotDist);
        }
		tacho_stop( MOTOR_BOTH );
        Sleep( 3000 );
	}
