#include "Cooldown.hpp"

Cooldown::Cooldown() {}

Cooldown::Cooldown(int max, bool auto_reset)
{
    this->max = max;
    this->remaining = max;
    this->auto_reset = auto_reset;
}

Cooldown::~Cooldown()
{
}

bool Cooldown::check() {
    if (--remaining <= 0)
    {
        remaining = auto_reset ? max : 0;
        return true;
    }
    return false;
}

void Cooldown::reset()
{
    remaining = max;
}

void Cooldown::set_remaining(int remaining)
{
    this->remaining = remaining;
}