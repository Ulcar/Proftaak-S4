#include "CarDataSimulator.h"
//Update Values should be updated each second
void CarDataSimulator::UpdateValues()
{
  /*  if(data.speed > targetSpeed)
    {
        data.speed += (data.speed - targetSpeed) % data.maxAcceleration;
    }

    else if(data.speed < targetSpeed)
    {
        data.speed -= (targetSpeed - data.speed) % data.maxDeAcceleration;
    } */



    data.currentAcceleration = pid->step(targetSpeed, data.speed);
    data.speed += data.currentAcceleration;
    data.m += (data.speed * period);
    data.rpm = data.speed * 3.6f;
    data.totalRounds = data.rpm / 60;
    data.fuel += data.totalRounds / 1000;
    
}


void CarDataSimulator::SetTargetSpeed(int targetSpeed)
{
    this->targetSpeed = targetSpeed;
}


CarData CarDataSimulator::GetData()
{
    return this->data;
}

CarDataSimulator::CarDataSimulator(int hz)
{
    this->hz = hz;
    this->period = (float)1 / (float)hz;
    pid = new FastPID(0.1f, 0 , 0, 1, 8, true);

}