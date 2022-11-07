#include "decideEspState.h"

float EspState::readTemp()
{
  tempValue = bme.readTemperature();
}

int EspState::newState()
{
  if(tempValue>0 && tempValue)
  

  return stateVariable;
}
