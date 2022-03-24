#ifndef COOLDOWN_H
#define COOLDOWN_H 1

class Cooldown
{
private:
    int max;
    int remaining;
    bool auto_reset;
public:
    Cooldown(int max, bool auto_reset);
    Cooldown();
    ~Cooldown();
    bool check();
    void reset();
    void set_remaining(int remaining);
    void set_max(int max);
};

#endif