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
    data.m += (data.speed * period * 0.277777778);
    if(data.m > 1000)
    {
        data.m -= 1000;
        data.km += 1;
    }
    // km/h to cm/s = 60 / 100.000
    // 
    data.rpm = (data.speed * (transmissionGearRatio * rearEndRatio) / 0.003768 ) / radius;
    data.totalRounds += data.rpm / 60;
    if(data.totalRounds > 10000){
    data.fuel -= (data.totalRounds / 1000);
    data.totalRounds = data.totalRounds - 1000;
    }
    
}


void CarDataSimulator::SetTargetSpeed(int targetSpeed)
{
    this->targetSpeed = targetSpeed;
}


CarData CarDataSimulator::GetData()
{
    return this->data;
}

CarDataSimulator::CarDataSimulator(int hz, float radius, float R1, float R2)
{
    this->hz = hz;
    this->period = (float)1 / (float)hz;
    this->radius = radius;
    this->transmissionGearRatio = R1;
    this->rearEndRatio = R2;
    pid = new FastPID(0.1f, 0, 0, 1, 8, true);
    data.fuel = 100;
}