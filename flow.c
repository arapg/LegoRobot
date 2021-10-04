#include <stdio.h>
#include "brick.h"
#include <unistd.h>

#define Sleep( msec ) usleep(( msec ) * 1000 ) /*Definerar sleep d  r Sleep(1000)= 1 sekund*/

#define MOTOR_RIGHT       OUTA
#define MOTOR_LEFT        OUTB
#define MOTOR_C           OUTC
#define MOTOR_D           OUTD
#define SENSOR_TOUCH      IN1
#define SENSOR_GYRO       IN2
#define SENSOR_US         IN3
#define SENSOR_4          IN4

#define MOTOR_BOTH        ( MOTOR_LEFT | MOTOR_RIGHT ) /* Bitvis ELLER ger att b  da motorerna styrs samtidigt */

POOL_T touch_sensor;
POOL_T gyro_sensor;
POOL_T sonic_sensor;

int max_hastighet;         /* variabel f  r max hastighet p   motorn */

void initialize_max();
int find_wall();
void go(int);
void turn_to_angle(int);
void go_until_distance(int);
void drop_off();
int initialize_max();

int main( void )
{
    int nearest_wall;

    //starta upp grejerna
    initialize_max();

    //hitta n  rmaste v  ggen
    nearest_wall = find_wall();
    //starta upp grejerna
    initialize_max();

    //hitta n  rmaste v  ggen
    nearest_wall = find_wall();

    //snurra till 90 grader fr  n n  rmaste v  ggen
    turn_to_angle(nearest_wall + 90); //  ndra till -90 f  r uppgift 2 & 4

    //  k 250cm
    go(250);

    //hitta n  rmaste v  ggen
    nearest_wall = find_wall();
    turn_to_angle(nearest_wall); //f  r uppgift 3 och 4, nearest_wall + 180
    go_until_distance(30);

    //l  mna av paketet
    drop_off();

    brick_uninit(); //funktion fr  n biblioteket
    return(0);

}

//en funktion som startar upp och registrerar sensorer och l  gen p   sensorer, allt som beh  ver g  ras innan programmet b  rjar. Detta kommer i huvudsak fr  n filerna ex$int initialize_max(){
    if ( !brick_init()) return ( 1 ); /* Initialiserar EV3-klossen */
    printf( "*** ( EV3 ) Hello! ***\n" );
    Sleep( 2000 );

    if ( tacho_is_plugged( MOTOR_BOTH, TACHO_TYPE__NONE_ )) {  /* TACHO_TYPE__NONE_ = Alla typer av motorer */
        max_hastighet = tacho_get_max_speed( MOTOR_LEFT, 0 );    /* Kollar maxhastigheten som motorn kan ha */
        tacho_reset( MOTOR_BOTH );
    } else {
        printf( "Anslut v  nster motor i port A,\n"
        "Anslut h  ger motor i port B.\n"
        );
        Sleep( 2000 );
          brick_uninit();
        return ( 0 );  /* St  nger av sig om motorer ej   r inkopplade */
    }

    //kolla att touch och gyro   r inkopplade!!!
    if(!sensor_is_plugged((SENSOR_TOUCH|SENSOR_GYRO), SENSOR_TYPE__NONE_)) {
        printf("Stoppa in sensorer i port 1 och 2\n");
        brick_uninit();
      
          }

    // TOUCH SENSOR
    touch_sensor = sensor_search( LEGO_EV3_TOUCH ); // Registrerar en touch sensor p   touch_sensor-variabeln
    touch_set_mode_touch(touch_sensor); // anger vilken "mode" sensorn skall ha

    //SONIC SENSOR
    sonic_sensor = sensor_search(LEGO_EV3_US);
    us_set_mode_us_dist_cm(sonic_sensor);

    //GYRO SENSOR
    sensor_set_mode(gyroSensor, LEGO_EV3_GYRO_GYRO_G_AND_A);

    return 0;
}

//hittar vinkeln till n  rmaste v  ggen och returnerar det v  rdet
int find_wall(){
    //get start_angle
    //start turning
      //while |current_angle - start_angle| < 360 ?????
        //if distance forward < smallest distance forward, update smallest distance forward and angle for it
    //stop turning
    return 0; //return the angle that had the smallest distance forward
}

//  ker det givna avst  ndet rakt fram  t (modifierad version av koden VILLE & ELIN skrev)
void go(int distance){
    float speedPercentage = 0.3; //den ska   ka 30% av maxhastigheten
    int seconds = (distance/0.15) * 1000; //r  knar ut antal millisekunder den ska   ka
    tacho_set_speed_sp( MOTOR_BOTH, max_hastighet * speedPercentage ); //s  tter hastigheten
    tacho_run_forever(  MOTOR_BOTH ); //startar motorerna
    Sleep( seconds * 1000 ); //v  ntar i s   m  nga sekunder som r  knats ut
    tacho_stop( MOTOR_BOTH ); //stoppar motorerna
}

//vrider sig till den givna vinkeln
void turn_to_angle(int goal_angle){
    //start turning
    //while goal_angle != current_angle
        //update current_angle
