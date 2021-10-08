#include <stdio.h>
#include "brick.h"
#include <unistd.h>

#define Sleep( msec ) usleep(( msec ) * 1000 ) // Definerar sleep, Sleep(1000)= 1 sekund

#define MOTOR_RIGHT    	OUTA
#define MOTOR_LEFT    	OUTB
#define MOTOR_C    		OUTC
#define MOTOR_D    		OUTD
#define SENSOR_TOUCH	IN1
#define SENSOR_2	IN2
#define SENSOR_3	IN3
#define SENSOR_4	IN4

#define MOTOR_BOTH     	( MOTOR_LEFT | MOTOR_RIGHT ) // Bitvis ELLER ger att båda motorerna styrs samtidigt

POOL_T sensor_us;

int max_hastighet; //Variabel för maxhastigheten för roboten



//Snurra
void turn(){



}

int main(){

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


    int wheelDeg = 771.4467455868585; //Antal grader för att Agda ska snurra ett helt varv
    sensor_us = sensor_search(LEGO_EV3_US);
    us_set_mode_us_dist_cm(sensor_us);
    printf("HEj\n");

    tacho_set_speed_sp(MOTOR_RIGHT, max_hastighet *(0.05));
    tacho_set_speed_sp(MOTOR_LEFT, max_hastighet *(-0.05));
    tacho_run_forever(MOTOR_BOTH);
    
    for(int i = 0; i < 360, i++;){

      int distance = sensor_get_value(0, sensor_us, 0);
      printf("%d\n", distance);

      if(distance > sensor_get_value(0, sensor_us,0)){
        distance = sensor_get_value(0, sensor_us, 0);
      }

      

      tacho_set_position_sp(MOTOR_RIGHT, (wheelDeg/360));
      tacho_set_position_sp(MOTOR_LEFT, -(wheelDeg/360));
      tacho_run_to_rel_pos(MOTOR_BOTH);
    }
    Sleep(8000);
    tacho_stop(MOTOR_BOTH);

    tacho_set_speed_sp(MOTOR_BOTH, max_hastighet*(0.5));
    tacho_run_forever(MOTOR_BOTH);
    Sleep(1000);
    Sleep(10000);
    return 0;
}
