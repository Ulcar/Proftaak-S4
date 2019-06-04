#include "CarData.h"
#include <FastPID.h>
class CarDataSimulator
{
    public:
    
    CarDataSimulator(int hz);
    CarData GetData();
    void UpdateValues();
    void SetTargetSpeed(int targetSpeed);
    private:
    void CalculateDistance();
    int hz;
    float period;
    int targetSpeed;
    CarData data;
    FastPID* pid;

};