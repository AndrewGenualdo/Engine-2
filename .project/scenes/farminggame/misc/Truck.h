//
// Created by cobble on 11/20/2025.
//

#ifndef ENGINE_2_TRUCK_H
#define ENGINE_2_TRUCK_H
#include "cobb/misc/multiTexture2d.h"


class Truck {
public:
    static MultiTexture2d texture;
    float timeLeftInState = 0.0f;
    vec2 pos = vec2(0.0f, 0.0f);
    enum TruckState {
        GONE = 0,
        ENTERING,
        OPENING,
        HERE,
        CLOSING,
        EXITING
    };
    TruckState state = GONE;
    Truck();

    [[nodiscard]] bool available() const;

    void update(float dt);
    void draw() const;

    void enter();

private:
    void changeState(TruckState newState);

};


#endif //ENGINE_2_TRUCK_H