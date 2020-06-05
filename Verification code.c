/*
This module controls the steering system of vehicles.
The following is a list of requirements it should adhere to.

This is a vehicle steering system controlling module.
The requirements below are the module should be sticked to.

1. If there is no flow in the primary circult
   or there is a short circult,
   then the primary circult will not able to provide power steering.

2. If the signal of wheel based speed is over 3 km/h,
   then the vehicle should be consider as moving.

3. If the vehicle is consider moving
   and the primary circult is not able to provide power steering,
   then the vehicle should be consider as moving without primary power steering.

4. If the vehicle is consider as moving without primary power steering,
   then the secondary circult have to take over the power steering.

5. If the power steering is provided by the secondary circult
   and the parking brake is off,
   then activation of the electric motor is necessary.
*/

#define VEH_MOVING_LIMIT 3
#define TRUE 1
#define FALSE 0
typedef enum
{
WORKING,
NO_FLOW,
SHORT_CIRCUIT,
} SENSOR_status;

typedef struct
{
int wheel_speed;
int parking_brake;
int prim_low_flow;
int prim_high_volt;
int second_circ_handles_stee;
int elec_motor_act;
} VEHICLE_INFO;

typedef enum
{
PARKING_BRAKE_APPLIED,
PRIMARY_CIRCUIT_LOW_FLOW,
PRIMARY_CIRCUIT_HIGH_VOLTAGE,
WHEEL_BASED_SPEED,
SECONDARY_CIRCUIT_HANDLES_STEERING,
ELECTRIC_MOTOR_ACTIVATED,
NUM_SIGNALS
} SIGNAL;

//@ ghost int ghost_vehicle_is_moving = 0;
//@ ghost int ghost_vehicle_moving_without_primary_power_steering = 0;
//@ ghost int ghost_primary_circuit_providing_power_steering = 0;

int status[NUM_SIGNALS];

/*
Reads the specific signal from the status.
*/

/*@
requires i < NUM_SIGNALS && i >= 0;

ensures \result == status[i];
*/
int read(SIGNAL i)
{
if(i < NUM_SIGNALS)
{
return status[i];
}
}
/*
Writes the specific signal to the status.
*/

/*@
requires i < NUM_SIGNALS && i >=0;

ensures status[i] == value;
*/

void write(SIGNAL i, int value)
{
if(i < NUM_SIGNALS)
{
status[i] = value;
}
}
/*
Get the current status of the system.
*/

/*@
requires \valid_read(veh_info);

ensures veh_info->wheel_speed == \old(status[WHEEL_BASED_SPEED]);
ensures veh_info->parking_brake == \old(status[PARKING_BRAKE_APPLIED]);
ensures veh_info->prim_low_flow == \old(status[PRIMARY_CIRCUIT_LOW_FLOW]);
ensures veh_info->prim_high_volt == \old(status[PRIMARY_CIRCUIT_HIGH_VOLTAGE]);
ensures veh_info->second_circ_handles_stee == \old(status[SECONDARY_CIRCUIT_HANDLES_STEERING]);
ensures veh_info->elec_motor_act == \old(status[ELECTRIC_MOTOR_ACTIVATED]);

*/
void get_system_status(VEHICLE_INFO *veh_info)
{
veh_info->wheel_speed = read(WHEEL_BASED_SPEED);
veh_info->parking_brake = read(PARKING_BRAKE_APPLIED);
veh_info->prim_low_flow = read(PRIMARY_CIRCUIT_LOW_FLOW);
veh_info->prim_high_volt = read(PRIMARY_CIRCUIT_HIGH_VOLTAGE);
veh_info->second_circ_handles_stee = read(SECONDARY_CIRCUIT_HANDLES_STEERING);
veh_info->elec_motor_act = read(ELECTRIC_MOTOR_ACTIVATED);
}
/*
Check the status of the primary steering circuit sensors.
*/

/*@
requires \valid(veh_info) && \valid_read(sensor_status);

ensures veh_info->prim_high_volt == \true 
==> *sensor_status == SHORT_CIRCUIT;
ensures veh_info->prim_high_volt != \true && veh_info->prim_low_flow == \true
==> *sensor_status == NO_FLOW;
ensures veh_info->prim_high_volt == \false && veh_info->prim_low_flow == \false
==> *sensor_status == WORKING;
ensures *sensor_status == NO_FLOW || *sensor_status == SHORT_CIRCUIT || *sensor_status == WORKING;
*/
void check_prim_sensor_status(VEHICLE_INFO *veh_info, SENSOR_status *sensor_status)
{
if(veh_info->prim_high_volt == TRUE)
{
*sensor_status = SHORT_CIRCUIT;
}
else if(veh_info->prim_low_flow == TRUE)
{
*sensor_status = NO_FLOW;
}
else
{
*sensor_status = WORKING;
}
}
/*
Check if the secondary circult need to handle the steering. 
*/

/*@
requires \valid(sensor_status);
requires *sensor_status == NO_FLOW || *sensor_status == SHORT_CIRCUIT || *sensor_status == WORKING;

//Req. 1
ensures \old(status[PRIMARY_CIRCUIT_HIGH_VOLTAGE]) == \true || 
\old(status[PRIMARY_CIRCUIT_LOW_FLOW]) == \true 
==> ghost_primary_circuit_providing_power_steering == \false;
//Req. 2
ensures \old(veh_info->wheel_speed) > 3 ==> ghost_vehicle_is_moving == \true;
// Req. 3 
ensures ghost_vehicle_is_moving == \true && \old(*sensor_status) != WORKING 
==> ghost_vehicle_moving_without_primary_power_steering == \true;
// Req. 4
ensures ghost_vehicle_moving_without_primary_power_steering == \true 
==> veh_info->second_circ_handles_stee == \true;
// Req. 5
ensures veh_info->second_circ_handles_stee == \true && \old(veh_info->parking_brake) == \false 
==> veh_info->elec_motor_act == \true;

*/
void secondary_steering(VEHICLE_INFO * veh_info, SENSOR_status *
sensor_status)
{
char vehicleIsMoving;
char vehicleIsMovingWithoutPrimaryPowerSteering;
// Check whether the vehicle is moving.
if(veh_info->wheel_speed > VEH_MOVING_LIMIT)
{
vehicleIsMoving = TRUE;
}
else
{
vehicleIsMoving = FALSE;
}
// Check whether vehicle is moving without primary power steering.
if(vehicleIsMoving == TRUE &&
(*sensor_status == NO_FLOW || *sensor_status == SHORT_CIRCUIT))
{
vehicleIsMovingWithoutPrimaryPowerSteering = TRUE;
}
else
{
vehicleIsMovingWithoutPrimaryPowerSteering = FALSE;
}
// Let secondary circuit handle steering if necessary.
if(vehicleIsMovingWithoutPrimaryPowerSteering == TRUE)
{
veh_info->second_circ_handles_stee = TRUE;
}
// Activate the electric motor.
if(veh_info->second_circ_handles_stee == TRUE
&& veh_info->parking_brake == FALSE)
{
veh_info->elec_motor_act = TRUE;
}
//@ ghost ghost_vehicle_moving_without_primary_power_steering = vehicleIsMovingWithoutPrimaryPowerSteering;
//@ ghost ghost_vehicle_is_moving = vehicleIsMoving;
}

/*
Entry point of steering module.
*/
void steering()
{
VEHICLE_INFO veh_info;
SENSOR_status prim_sensor;
get_system_status(&veh_info);
check_prim_sensor_status(&veh_info, &prim_sensor);
secondary_steering(&veh_info, &prim_sensor);
write(SECONDARY_CIRCUIT_HANDLES_STEERING, veh_info.second_circ_handles_stee);
write(ELECTRIC_MOTOR_ACTIVATED, veh_info.elec_motor_act);
}
