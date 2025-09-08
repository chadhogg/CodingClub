/// \file FastestestFunction.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/fastestestfunction

#include <iostream>
#include <iomanip>
#include <cassert>

// We know that:
// (00) timeTotalInit = timeFooInit + timeOther
// (01) timeTotalFinal = timeFooFinal + timeOther
// (02) percFooInit = timeFooInit / timeTotalInit
// (03) percFooFinal = timeFooFinal / timeTotalFinal
// We can conclude that:
// (04) percFooInit = timeFooInit / (timeFooInit + timeOther) [rewriting (02) with substitution from (00)]
// (05) percFooFinal = timeFooFinal / (timeFooFinal + timeOther) [rewriting (03) with substitution from (01)]
// (06) timeFooInit = (timeOther * percFooInit) / (1 - percFooInit) [rewriting (04)]
// (07) timeFooFinal = (timeOther * percFooFinal) / (1 - percFooFinal) [rewriting (05)]
// (08) timeFooInit / timeFooFinal = ((timeOther * percFooInit) * (1 - percFooFinal))
//                                 / ((timeOther * percFooFinal) * (1 - percFooInit))
//                                 = percFooInit / percFooFinal * (1 - percFooFinal) / (1 - percFooInit)

int
main()
{
    int x, y;
    std::cin >> x >> y;
    double percX = x * 0.01;
    double percY = y * 0.01;
    std::cout << std::setprecision(10) << (percX / percY) * ((1 - percY) / (1 - percX)) << "\n";

    return 0;
}