#include <stdio.h>

typedef struct {
    float tank_capacity;
    int tank_psi;
    const char *suit_masterial;
} equipment;

typedef struct scuba {
    const char *name;
    equipment kit;
} diver;

void badge(diver d){
    printf("Name: %s Tank: %2.2f(%d) Suit: %s\n",
        d.name, d.kit.tank_capacity, d.kit.tank_psi, d.kit.suit_masterial);
}

int main()
{
    diver randy = {"Randy", {5.5, 3500, "Neoprene"}};
    badge(randy);

    return 0;
}
