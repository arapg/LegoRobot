#include <stdio.h>
#include "brick.h"
#include <unistd.h>

#define Sleep( msec ) usleep(( msec ) * 1000 ) /* Definerar sleep, Sleep(1000)= 1 sekund */

#define MOTOR_RIGHT     OUTA
#define MOTOR_LEFT      OUTB
#define MOTOR_LYFT      OUTC
#define MOTOR_D                 OUTD

#define SENSOR_TOUCH    IN1
#define SENSOR_2                IN2
#define SENSOR_3                IN3
#define SENSOR_4                IN4

#define MOTOR_BOTH      ( MOTOR_LEFT | MOTOR_RIGHT ) /* Bitvis ELLER ger att b  da motorerna styrs samtidigt */
#define MOTOR_ALLT      ( MOTOR_BOTH | MOTOR_LYFT )

int max_hastighet;         /* variabel f  r max hastighet p   motorn */
POOL_T touchSensor;

int main( void )
{

        if ( !brick_init()) return ( 1 ); /* Initialiserar EV3-klossen */
        printf( "*************");        
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
       
        tacho_set_speed_sp( MOTOR_BOTH, max_hastighet *(0.5) );  // S  tter hastigheten p   b  da motorerna till 50% av maxhastigheten
        /* Om man vill k  ra bak  t anger man negativ hastighet, till exempel max_hastighet * (-0.5) */

        tacho_run_forever(  MOTOR_BOTH );
        Sleep( 1000 );
        tacho_stop( MOTOR_LEFT );
        Sleep( 2000 );
        tacho_stop( MOTOR_RIGHT );
        Sleep( 2000 );

        tacho_set_speed_sp(MOTOR_BOTH, max_hastighet * -1);

        tacho_run_forever( MOTOR_BOTH);
        Sleep(3000);
        tacho_stop(MOTOR_LEFT);
        Sleep(3000);
        tacho_stop(MOTOR_RIGHT);
        Sleep(1000);

        tacho_set_speed_sp(MOTOR_BOTH, max_hastighet * 0.3);
               
        tacho_run_forever(MOTOR_BOTH);
        Sleep(1000);
        tacho_stop(MOTOR_LEFT);
        Sleep(5000);
        tacho_run_forever(MOTOR_BOTH);
        Sleep(2000);

        tacho_set_speed_sp(MOTOR_RIGHT, max_hastighet * 0.7);
        tacho_set_speed_sp(MOTOR_LEFT, max_hastighet *0.5);

        tacho_run_forever(MOTOR_BOTH);
        Sleep(10000);

        tacho_set_speed_sp(MOTOR_ALLT, max_hastighet*0.5);
        tacho_run_forever(MOTOR_ALLT);
        Sleep(1000);


        brick_uninit();
        printf( "dying...\n" );
        return ( 0 );

}
