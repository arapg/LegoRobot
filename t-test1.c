#include <stdio.h>
#include "brick.h"
#include <unistd.h>

#define MOTOR_RIGHT	OUTA
#define MOTOR_LEFT	OUTB
#define MOTOR_LYFT	OUTC 		/* Motorn som lyfter/håller/släpper boken */
#define MOTOR_D			OUTD 	/*används inte i nuläget*/

#define SENSOR_TOUCH	IN1
#define SENSOR_US	IN2
#define SENSOR_GYRO	IN3
#define SENSOR_COLOUR		IN4 	/*Denna sensor kommer inte användas*/

	#define MOTOR_BOTH      ( MOTOR_LEFT | MOTOR_RIGHT )
	#define MOTOR_ALLT      ( MOTOR_BOTH | MOTOR_LYFT )

#define Sleep( msec ) usleep(( msec ) * 1000 ) /* Definerar sleep, Sleep(1000)= 1 sekund */

int max_hastighet;         /* variabel för max hastighet på motorn */

/*Vi måste definiera varv/längd på hjulen när robot åker*/

POOL_T touchSensor;
	int TouchReturnValue = 0;

POOL_T sensor_us;
	int  usValue = 0;
	int min_distance=4000;

POOL_T gyroSensor;
	int gyroValue0 = 0;
	int gyroValue1 = 0;

int main( void )
{

/*------Snurra och registrera närmaste vägg-----*/	
	
tacho_set_speed_sp(MOTOR_LEFT, max_hastighet *0.3);
	
	sensor_us=sensor_search(LEGO_EV3_US);
        us_set_mode_us_dist_cm(sensor_us);
        int distance;
        while(1){
        distance=sensor_get_value(0, sensor_us, 0);
        printf("%d\n", distance);
	}
	    tacho_run_forever(MOTOR_BOTH);
                Sleep(10000);
	
/*-----Kör mot närmaste registrerade avstånd-----*/
         tacho_set_speed_sp( MOTOR_BOTH, max_hastighet*0.5);

                tacho_run_forever(  MOTOR_BOTH );
                Sleep(10000);
	
	touchSensor = sensor_search( LEGO_EV3_TOUCH ); 	// Registrerar en touch sensor på touchSensor-variabeln
        touch_set_mode_touch(touchSensor); 		// anger vilken "mode" sensorn skall ha
	
	
/*-----(Backar lite) Vänder 90* åt vänster-----*/
	    tacho_set_speed_sp( MOTOR_BOTH, max_hastighet*0.3);

                tacho_run_forever(  MOTOR_BOTH );
                Sleep(2000);
	
        gyroSensor = sensor_search(LEGO_EV3_GYRO);
        sensor_set_mode(gyroSensor, LEGO_EV3_GYRO_GYRO_G_AND_A);
        sensor_set_mode(gyroSensor, LEGO_EV3_GYRO_GYRO_G_AND_A);

        while(!TouchReturnValue){
                TouchReturnValue = sensor_get_value(0, touchSensor, 0);
                gyroValue0 = sensor_get_value(0, gyroSensor, 0);
                gyroValue1 = sensor_get_value(1, gyroSensor, 0);
        printf("Gyro0: %d, Gyro1: %d \n", gyroValue0, gyroValue1); /*Behövs nog inte printas på skärmen*/
		
		tacho_set_speed_sp( MOTOR_RIGHT, max_hastighet*0.3);
                tacho_run_forever(  MOTOR_RIGHT );
                Sleep(2000); /*Måste definiera varv/längd på hjulen*/
		
			if(gyroValue0 == -90)
				tacho_stop; /*Måste förbättras och testas*/

/*-------------Har printar vi saker pa skarmen----------------------------------*/

        if ( !brick_init()) return ( 1 ); /* Initialiserar EV3-klossen */
        printf( "******");
        Sleep( 2000 );


/*---------------Ifall nagon kabel inte ar ikopplad sa sager den till har-------*/

        if ( tacho_is_plugged( MOTOR_BOTH, TACHO_TYPE__NONE_ )) {  /* TACHO_TYPE__NONE_ = Alla typer av motorer */
        max_hastighet = tacho_get_max_speed( MOTOR_LEFT, 0 );   /* Kollar maxhastigheten som motorn kan ha */
        tacho_reset( MOTOR_BOTH );
    } else {
        printf( "Anslut v  nster motor i port A,\n"
        "Anslut h  ger motor i port B.\n"
        );
        brick_uninit();
                  return ( 0 );  /* St  nger av sig om motorer ej   r inkopplade */
    }

/*---------------------Har borjar vi definiera sensor touch----------------------*/

        touchSensor = sensor_search( LEGO_EV3_TOUCH ); /* Registrerar en touch sensor p   touchSensor-variablen*/
        touch_set_mode_touch(touchSensor); /* anger vilken "mode" sensorn skall ha*/


/*------------------Har borjar vi definiera hur snabbt vi vill att roboten ska aka------------*/

        tacho_set_speed_sp( MOTOR_BOTH, max_hastighet*0.5);

                tacho_run_forever(  MOTOR_BOTH );
                Sleep(3000);

        tacho_set_speed_sp(MOTOR_RIGHT, max_hastighet *( -1));
        tacho_set_speed_sp(MOTOR_LEFT, max_hastighet * 1);

                tacho_run_forever( MOTOR_BOTH);
                Sleep(3000);
          tacho_set_speed_sp(MOTOR_BOTH, max_hastighet * 1);

                tacho_run_forever(MOTOR_BOTH);
                Sleep(3000);

        tacho_set_speed_sp(MOTOR_RIGHT, max_hastighet *0.5);
        tacho_set_speed_sp(MOTOR_LEFT, max_hastighet *0.3);

                tacho_run_forever(MOTOR_BOTH);
                Sleep(2000);

        tacho_set_speed_sp(MOTOR_BOTH, max_hastighet*0.5);
        tacho_set_speed_sp(MOTOR_LYFT, max_hastighet * 0.1);

                tacho_run_forever(MOTOR_ALLT);
                Sleep(4000);


        brick_uninit();
        printf( "dying...\n" );
      return ( 0 );

}
