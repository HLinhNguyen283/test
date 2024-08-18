#include "fraction.h"

Fraction addThreeFractions(const Fraction& fraction1, const Fraction& fraction2, const Fraction& fraction3) {
    Fraction result, result1;
    result1 = addFractions(fraction1, fraction2);
    result = addFractions(result1, fraction3);
    return result;
}