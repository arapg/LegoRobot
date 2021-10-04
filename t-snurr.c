#include <stdio.h>
#include "brick.h"
#include <unistd.h>

#define MOTOR_RIGHT	OUTA
#define MOTOR_LEFT	OUTB
#define MOTOR_LYFT	OUTC 		/* Motorn som lyfter/håller/släpper boken */
#define MOTOR_D		OUTD 	/*används inte i nuläget*/

#define SENSOR_TOUCH	IN1
#define SENSOR_US	IN2
#define SENSOR_GYRO	IN3
#define SENSOR_COLOUR	IN4 	/*Denna sensor kommer inte användas*/

	#define MOTOR_BOTH      ( MOTOR_LEFT | MOTOR_RIGHT )
	#define MOTOR_ALLT      ( MOTOR_BOTH | MOTOR_LYFT ) /*Kommer antagligen inte behövas*/

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
int turn(void){
	int minVal = 2147483647,degrees,data;
	us_set_mode_us_dist_cm(sensor_us);
	sensor_set_mode(gyroSensor, LEGO_EV3_GYRO_GYRO_G_AND_A);
	for(int i = 0; i<360;i++){
		//skriver data från sensor till lista
		data = sensor_get_value(0, sensor_us, 0);
		if(minVal > data){
			minVal = data;
			degrees = i;
		}
		//snurrar 1 grad
		while(sensor_get_value(0, gyroSensor, 0) < i){
			tacho_set_speed_sp(MOTOR_RIGHT, max_hastighet *( -1));
        		tacho_set_speed_sp(MOTOR_LEFT, max_hastighet * 1);
			tacho_run_forever(  MOTOR_BOTH );
                	Sleep(50);
			tacho_stop(MOTOR_BOTH);
		}
	}
	if(degrees < 180)
		return degrees;
	else
		return degrees - 360;
	}
int main( void )
{
/*-------Kallar på dist funk--------------------*/
	int rotate = turn();
	while(sensor_get_value(0, gyroSensor, 0) < rotate){
		tacho_set_speed_sp(MOTOR_RIGHT, max_hastighet *( -1));
		tacho_set_speed_sp(MOTOR_LEFT, max_hastighet * 1);
		tacho_run_forever(  MOTOR_BOTH );
		Sleep(50);
		tacho_stop(MOTOR_BOTH);
	}
	//snurra tillbaka
	rotate *= -1;
	while(sensor_get_value(0, gyroSensor, 0) < rotate){
		tacho_set_speed_sp(MOTOR_RIGHT, max_hastighet *( -1));
		tacho_set_speed_sp(MOTOR_LEFT, max_hastighet * 1);
		tacho_run_forever(  MOTOR_BOTH );
		Sleep(50);
		tacho_stop(MOTOR_BOTH);
	}
/*------Snurra och registrera närmaste vägg-----*/	
	
tacho_set_speed_sp(MOTOR_RIGHT, max_hastighet *0.3);
	
	sensor_us=sensor_search(LEGO_EV3_US);
        us_set_mode_us_dist_cm(sensor_us);
        int distance;
        while(1){
        distance=sensor_get_value(0, sensor_us, 0);
	int min_distance; /*På något sätt kom ihåg minsta avstånd*/
	}
	    tacho_run_forever(MOTOR_RIGHT);
                Sleep(10000);
		if(min_distance < ...){
			tacho_stop(MOTOR_RIGHT);
			Sleep(10000);
		}
