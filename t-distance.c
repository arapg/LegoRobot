#include <stdio.h>
#include "brick.h"
#include <unistd.h>

#define Sleep( msec ) usleep(( msec ) * 1000 ) /* Definerar sleep, Sleep(1000)= 1 sekund */

#define MOTOR_LEFT      OUTA
#define MOTOR_RIGHT     OUTB
#define MOTOR_C         OUTC
#define MOTOR_D                 OUTD
#define SENSOR_TOUCH    IN1
#define SENSOR_US       IN2
#define SENSOR_GYRO     IN3
#define SENSOR_COLOUR   IN4

#define MOTOR_BOTH      ( MOTOR_LEFT | MOTOR_RIGHT ) /* Bitvis ELLER ger att b  da motorerna styrs samtidigt */

int max_hastighet;         /* variabel f  r max hastighet p   motorn */
POOL_T touchSensor, sensor_us;
POOL_T gyroSensor;
int gyroValue0 = 0;
int gyroValue1 = 0;

int min_distance=4000;


int main( void )
{

        if ( !brick_init()) return ( 1 ); /* Initialiserar EV3-klossen */
        printf( "*** ( EV3 ) Hello! ***\n" );
        Sleep( 2000 );

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


        touchSensor = sensor_search( LEGO_EV3_TOUCH ); // Registrerar en touch sensor p   touchSensor-variabeln
        touch_set_mode_touch(touchSensor); // anger vilken "mode" sensorn skall ha

        tacho_set_speed_sp( MOTOR_BOTH, max_hastighet * 0.5 );  // S  tter hastigheten p   b  da motorerna till 50% av maxhastigheten
        /* Om man vill k  ra bak  t anger man negativ hastighet, till exempel max_hastighet * (-0.5) */


        gyroSensor = sensor_search(LEGO_EV3_GYRO);

        sensor_set_mode(gyroSensor, LEGO_EV3_GYRO_GYRO_G_AND_A);

        sensor_set_mode(gyroSensor, LEGO_EV3_GYRO_GYRO_G_AND_A);

        while(!TouchReturnValue){
                TouchReturnValue = sensor_get_value(0, touchSensor, 0);
                gyroValue0 = sensor_get_value(0, gyroSensor, 0);
                gyroValue1 = sensor_get_value(1, gyroSensor, 0);
        printf("Gyro0: %d, Gyro1: %d \n", gyroValue0, gyroValue1);

        /*sensor_us=sensor_search(LEGO_EV3_US);
        us_set_mode_us_dist_cm(sensor_us);
        int distance;
        while(1){
        distance=sensor_get_value(0, sensor_us, 0);
        printf("%d\n", distance);
        }*/

        /*tacho_run_forever(  MOTOR_BOTH );
        Sleep( 1000 );
        tacho_stop( MOTOR_LEFT );
        Sleep( 2000 );
        tacho_stop( MOTOR_RIGHT );
        Sleep( 2000 );*/

        tacho_set_speed_sp(MOTOR_RIGHT, max_hastighet * 0.5);
        tacho_run_forever(MOTOR_RIGHT);
                tacho_set_speed_sp(MOTOR_RIGHT, max_hastighet * 0.5);
        tacho_run_forever(MOTOR_RIGHT);


        sensor_us=sensor_search(LEGO_EV3_US);
        us_set_mode_us_dist_cm(sensor_us);
        tacho_run_forever(MOTOR_BOTH);
        int distance;
        while(1){
        distance=sensor_get_value(0, sensor_us, 0);
        printf("%d\n", distance);
        }


        tacho_run_forever( MOTOR_BOTH );
        while(!sensor_get_value(0, touchSensor, 0)); //S   l  nge touch-sensorn inte   r intryckt kommer while-loopen k  ras
        tacho_stop( MOTOR_BOTH );

        brick_uninit();
        printf( "dying...\n" );
    return ( 0 );

}
