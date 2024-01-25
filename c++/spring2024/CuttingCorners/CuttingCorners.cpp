/// \file CuttingCorners.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/cuttingcorners

#include <iostream>
#include <vector>
#include <cmath>

using Shape = std::vector<std::pair<int, int>>;

std::vector<Shape>
readInput()
{
    std::vector<Shape> shapes;
    int numCorners, x, y;
    std::cin >> numCorners;
    while (numCorners > 0) {
        Shape shape;
        for (int i = 0; i < numCorners; ++i) {
            std::cin >> x >> y;
            shape.push_back ({x, y});
        }
        std::cin >> numCorners;
        shapes.push_back (shape);
    }
    return shapes;
}

bool
isTriangle(const Shape& shape)
{
    return shape.size () == 3;
}

double
computeAngle(const Shape& shape, unsigned index)
{
    std::pair<int, int> center(shape.at(index));
    std::pair<int, int> before(shape.at((index + shape.size() - 1) % shape.size()));
    std::pair<int, int> after(shape.at((index + 1) % shape.size()));
    std::pair<int, int> beforeVec({before.first - center.first, before.second - center.second});
    std::pair<int, int> afterVec({after.first - center.first, after.second - center.second});
    double dotProduct = beforeVec.first * afterVec.first + beforeVec.second * afterVec.second;
    double beforeMagnitude = sqrt(beforeVec.first * beforeVec.first + beforeVec.second * beforeVec.second);
    double afterMagnitude = sqrt(afterVec.first * afterVec.first + afterVec.second * afterVec.second);
    return acos(dotProduct / (beforeMagnitude * afterMagnitude));
}

std::pair<unsigned, double>
findSharpestAngle(const Shape& shape)
{
    std::pair<unsigned, double> sharpest({0, 2 * M_PI});
    for (unsigned index = 0; index < shape.size (); ++index) {
        double angle = computeAngle(shape, index);
        if (angle < sharpest.second) {
            sharpest = {index, angle};
        }
    }
    return sharpest;
}

Shape
cutAsMuchAsPossible(const Shape& shape) {
    Shape previous = shape;
    std::pair<unsigned, double> prevSharpest = findSharpestAngle(previous);
    Shape current = shape;
    while (true) {
        std::pair<unsigned, double> currSharpest = findSharpestAngle(current);
        //std::cout << "Previous was " << prevSharpest.first << " " << prevSharpest.second << "\n";
        //std::cout << "Current is " << currSharpest.first << " " << currSharpest.second << "\n";
        if (currSharpest.second < prevSharpest.second) {
            //std::cout << "Early exit because " << currSharpest.first << " " << currSharpest.second << " is sharper than " << prevSharpest.first << " " << prevSharpest.second << "\n";
            return previous;
        }
        else if (isTriangle(current)) {
            return current;
        }
        previous = current;
        current.erase(current.begin() + currSharpest.first);
        prevSharpest = currSharpest;
    }
    //std::cout << "Reached a triangle.\n";
    return current;
}

void
printShape(const Shape& shape)
{
    std::cout << shape.size();
    for (unsigned i = 0; i < shape.size(); ++i) {
        std::cout << " " << shape.at(i).first << " " << shape.at(i).second;
    }
    std::cout << "\n";
}

int 
main()
{
    std::vector<Shape> shapes = readInput ();
    for (const Shape& shape : shapes) {
        Shape cutted = cutAsMuchAsPossible (shape);
        printShape(cutted);
    }
    return 0;
}