#pragma once

class LifeBar
{
  public:
    void Add(float amount)
    {
        if (value == 0)
        {
            // Player is dead.
            return;
        }

        value += amount;
        if (value > maxValue)
        {
            value = maxValue;
        }
        if (value < 0)
        {
            value = 0;
        }
    }

    float GetRatio()
    {
        return value / maxValue;
    }

  private:
    float maxValue = 100;
    float value = maxValue;
};