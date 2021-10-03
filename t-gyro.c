#include <stdio.h>
#include "brick.h"
#include <unistd.h>

#define Sleep( msec ) usleep(( msec ) * 1000 ) /*Definerar sleep d  r Sleep(1000)= 1 sekund*/

#define MOTOR_A                 OUTA
#define MOTOR_B                 OUTB
#define MOTOR_C                 OUTC
#define MOTOR_D                 OUTD
#define SENSOR_TOUCH    IN1
#define SENSOR_US       IN2
#define SENSOR_GYRO     IN3
#define SENSOR_4                IN4


POOL_T touchSensor;
int TouchReturnValue = 0;

POOL_T gyroSensor;
int gyroValue0 = 0;
int gyroValue1 = 0;

POOL_T usSensor;
int  usValue = 0;

int main( void )
{

        if ( !brick_init()) return ( 1 ); /*Instansierar klossen och till  ter remote-connection*/
        printf( "*** ( EV3 ) Hello! ***\n" );
        Sleep(2000);

        if(!sensor_is_plugged((SENSOR_TOUCH|SENSOR_GYRO), SENSOR_TYPE__NONE_)) {
                printf("Stoppa in sensorer i port 1 och 2\n");
                brick_uninit();
                return(0);
        }
        /*Registrerar en sensor p   sensor variabeln*/
        touchSensor = sensor_search( LEGO_EV3_TOUCH ); //
        gyroSensor = sensor_search(LEGO_EV3_GYRO);
  
        /*Man kan antingen anv  nda "Brick" bibliotekets f  rbyggda modes*/
        touch_set_mode_touch(touchSensor);

        /*eller kolla i sensor.h filen p   vilka modes som finns f  r alla olika sensorer*/
        sensor_set_mode(gyroSensor, LEGO_EV3_GYRO_GYRO_G_AND_A);



        while(!TouchReturnValue){
                TouchReturnValue = sensor_get_value(0, touchSensor, 0);

                /*Vissa modes kan ha fler utdata, notera f  rsta parametern i sensor_get_value */
                gyroValue0 = sensor_get_value(0, gyroSensor, 0);
                gyroValue1 = sensor_get_value(1, gyroSensor, 0);

                printf("Gyro0: %d , Gyro1: %d \n", gyroValue0, gyroValue1 );

        }//S   l  nge touch sensorn inte   r intryckt kommer while loopen k  ras
        /*Man kan antingen anv  nda "Brick" bibliotekets f  rbyggda modes*/
        touch_set_mode_touch(touchSensor);

        /*eller kolla i sensor.h filen p   vilka modes som finns f  r alla olika sensorer*/
        sensor_set_mode(gyroSensor, LEGO_EV3_GYRO_GYRO_G_AND_A);



        while(!TouchReturnValue){
                TouchReturnValue = sensor_get_value(0, touchSensor, 0);

                /*Vissa modes kan ha fler utdata, notera f  rsta parametern i sensor_get_value */
                gyroValue0 = sensor_get_value(0, gyroSensor, 0);
                gyroValue1 = sensor_get_value(1, gyroSensor, 0);

                printf("Gyro0: %d , Gyro1: %d \n", gyroValue0, gyroValue1 );

        }//S   l  nge touch sensorn inte   r intryckt kommer while loopen k  ras


        brick_uninit();
        printf( "dying...\n" );
    return ( 0 );

}
