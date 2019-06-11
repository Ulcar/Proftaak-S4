#include "CarData.h"
#include <FastPID.h>
class CarDataSimulator
{
    public:
    
    CarDataSimulator(int hz, float radius, float R1, float R2);
    CarData GetData();
    void UpdateValues();
    void SetTargetSpeed(int targetSpeed);
    private:
    void CalculateDistance();
    int hz;
    float period;
    int targetSpeed;
    float radius;
    //transmission in real life would depend on what gear you're driving in
    float transmissionGearRatio;
    // 
    float rearEndRatio;
    CarData data;
    FastPID* pid;

};