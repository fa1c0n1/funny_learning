#include <stdio.h>

typedef union {
    float lemon;
    int lime_pieces;
} lemon_lime;

typedef struct {
    float tequila;
    float cointreau;
    lemon_lime citrus;
} margarita;

int main(void)
{
    margarita m1 = {2.0, 1.0, .citrus.lemon = 2};
    margarita m2 = {2.0, 1.0, {0.5}};
    margarita m3 = {2.0, 1.0, {.lime_pieces = 1}};
    printf("(1)\n");
    printf("%2.1f measures of tequila\n%2.1f measures of cointreau\n%2.1f \
            measures of juice\n", m1.tequila, m1.cointreau, m1.citrus.lemon);

    printf("(2)\n");
    printf("%2.1f measures of tequila\n%2.1f measures of cointreau\n%2.1f \
            measures of juice\n", m2.tequila, m2.cointreau, m2.citrus.lemon);

    printf("(3)\n");
    printf("%2.1f measures of tequila\n%2.1f measures of cointreau\n%d \
        pieces of lime\n", m3.tequila, m3.cointreau, m3.citrus.lime_pieces);
    
    /* (1)
     * 2.0 measures of tequila
     * 1.0 measuers of cointreau
     * 2.0 measures of juice
     *
     * (2)
     * 2.0 measures of tequila 
     * 1.0 measures of cointreau
     * 0.5 measures of juice
     *
     * (3)
     * 2.0 measures of tequila
     * 1.0 measures of cointreau
     * 1 pieces of lime
     */

    return 0;
}
