#include "fraction.h"

Fraction createFraction(int numerator, int denominator) {
    Fraction fraction;
    fraction.numerator = numerator;
    fraction.denominator = denominator;
    return fraction;
}

Fraction addFractions(const Fraction& fraction1, const Fraction& fraction2) {
    Fraction result;
    result.numerator = fraction1.numerator * fraction2.denominator + fraction2.numerator * fraction1.denominator;
    result.denominator = fraction1.denominator * fraction2.denominator;
    simplifyFraction(result);
    return result;
}

Fraction subtractFractions(const Fraction& fraction1, const Fraction& fraction2) {
    Fraction result;
    result.numerator = fraction1.numerator * fraction2.denominator - fraction2.numerator * fraction1.denominator;
    result.denominator = fraction1.denominator * fraction2.denominator;
    simplifyFraction(result);
    return result;
}

Fraction multiplyFractions(const Fraction& fraction1, const Fraction& fraction2) {
    Fraction result;
    result.numerator = fraction1.numerator * fraction2.numerator;
    result.denominator = fraction1.denominator * fraction2.denominator;
    simplifyFraction(result);
    return result;
}

Fraction divideFractions(const Fraction& fraction1, const Fraction& fraction2) {
    Fraction result;
    result.numerator = fraction1.numerator * fraction2.denominator;
    result.denominator = fraction1.denominator * fraction2.numerator;
    simplifyFraction(result);
    return result;
}

void simplifyFraction(Fraction& fraction) {
    int gcd = 1;
    for (int i = 1; i <= fraction.numerator && i <= fraction.denominator; ++i) {
        if (fraction.numerator % i == 0 && fraction.denominator % i == 0) {
            gcd = i;
        }
    }
    fraction.numerator /= gcd;
    fraction.denominator /= gcd;
}

void printFraction(const Fraction& fraction) {
    printf("%d/%d\n", fraction.numerator, fraction.denominator);
}

