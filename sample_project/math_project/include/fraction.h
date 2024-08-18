#ifndef FRACTION_H
#define FRACTION_H

struct Fraction {
    int numerator;
    int denominator;
};

Fraction createFraction(int numerator, int denominator);
Fraction addFractions(const Fraction& fraction1, const Fraction& fraction2);
Fraction subtractFractions(const Fraction& fraction1, const Fraction& fraction2);
Fraction multiplyFractions(const Fraction& fraction1, const Fraction& fraction2);
Fraction divideFractions(const Fraction& fraction1, const Fraction& fraction2);
void simplifyFraction(Fraction& fraction);
void printFraction(const Fraction& fraction);

#endif // FRACTION_H