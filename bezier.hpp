#ifndef BEZIER
#define BEZIER

#include <raylib.h>
#include <vector>
#include <raymath.h>
#include <iostream>

const int POINT_RADIUS = 5;
const int s = 200;
const int noOfControlPoints = 4;
const int noOfCurves = 2;
const int r = 3;
const int v = 100;

struct Circle{
    Vector2 center;
    float radius = POINT_RADIUS;
    Color color;
};

struct Point{
    Vector2 actualPoint;
    bool hasTangent = false;
    Vector2 tangent;
    Vector2 normal;
};

Circle circles[noOfControlPoints];
std::vector<Point> actualPoints;
std::vector<Vector2> actualPointsDerivative;
std::vector<Vector2> derivatives;

Circle circles2[noOfControlPoints];
std::vector<Point> actualPoints2;
std::vector<Vector2> actualPointsDerivative2;
std::vector<Vector2> derivatives2;

int factorial(int n){
    if(n==0){
        return 1;
    }
    if(n==1){
        return n;
    }
    return n*factorial(n-1);
}

float combination(int n, int r){
    float ans = factorial(n)/(factorial(n-r)*factorial(r));
    return ans;
}

std::vector<int> pascalsTriangleRow(int order){
    std::vector<int> ret;
    for(int i = 0; i <= order; i++){
        int raw = (int) combination(order, i);
        ret.push_back(raw);
    }
    return ret;
}

void addPointParametric(std::vector<Circle> points, float t, std::vector<int> ptRow){
    Point gab;
    gab.actualPoint = {0.0f, 0.0f};
    for(int j = 0; j < ptRow.size(); j++){
        gab.actualPoint.x += ptRow[j]*(points[j].center.x)*pow(1-t, ptRow.size()-1-j)*pow(t, j);
        gab.actualPoint.y += ptRow[j]*(points[j].center.y)*pow(1-t, ptRow.size()-1-j)*pow(t, j);
    }
    // std::cout << gab.actualPoint.x << std::endl;
    actualPoints.push_back(gab);
}

void addPointParametric(std::vector<Circle> points, float t, std::vector<int> ptRow, int order){
    Vector2 gab = {0.0f, 0.0f};
    for(int j = 0; j < ptRow.size(); j++){
        gab.x += order*(ptRow[j]*(points[j].center.x)*pow(1-t, ptRow.size()-1-j)*pow(t, j));
        gab.y += order*(ptRow[j]*(points[j].center.y)*pow(1-t, ptRow.size()-1-j)*pow(t, j));
    }
    actualPointsDerivative.push_back(gab);
}

void addPointParametric2(std::vector<Circle> points, float t, std::vector<int> ptRow){
    Point gab;
    gab.actualPoint = {0.0f, 0.0f};
    for(int j = 0; j < ptRow.size(); j++){
        gab.actualPoint.x += ptRow[j]*(points[j].center.x)*pow(1-t, ptRow.size()-1-j)*pow(t, j);
        gab.actualPoint.y += ptRow[j]*(points[j].center.y)*pow(1-t, ptRow.size()-1-j)*pow(t, j);
    }
    // std::cout << gab.actualPoint.x << std::endl;
    actualPoints2.push_back(gab);
}

void addPointParametric2(std::vector<Circle> points, float t, std::vector<int> ptRow, int order){
    Vector2 gab = {0.0f, 0.0f};
    for(int j = 0; j < ptRow.size(); j++){
        gab.x += order*(ptRow[j]*(points[j].center.x)*pow(1-t, ptRow.size()-1-j)*pow(t, j));
        gab.y += order*(ptRow[j]*(points[j].center.y)*pow(1-t, ptRow.size()-1-j)*pow(t, j));
    }
    actualPointsDerivative2.push_back(gab);
}

void normalizeAll(){
    for(auto i = actualPointsDerivative.begin(); i != actualPointsDerivative.end(); i++){
        *i = Vector2Scale(Vector2Normalize(*i), 50.0f);
    }
}

void normalizeAll2(){
    for(auto i = actualPointsDerivative2.begin(); i != actualPointsDerivative2.end(); i++){
        *i = Vector2Scale(Vector2Normalize(*i), 50.0f);
    }
}

void curveDerivative(Circle points[], int newOrder){
    std::vector<Circle> derivativePoints;

    std::vector<int> newCoeff = pascalsTriangleRow(newOrder);
    for(int i = 1; i < noOfControlPoints; i++){
        Circle circle;
        circle.center = {points[i-1].center.x - points[i].center.x, points[i-1].center.y - points[i].center.y};
        circle.radius = POINT_RADIUS;
        if(i % newOrder == 0) circle.color = GREEN;
        else circle.color = RED;
        derivativePoints.push_back(circle);
    }
    // for(auto x : derivativePoints){
    //     std::cout << x.center.x << "|" << x.center.y << std::endl;
    // }

    for(auto i = derivativePoints.begin(); i != (derivativePoints.end()-1); i = i + newOrder){
        // addPoints(*i, *(i+1), *(i+2));
        bool tamana = false;
        for(int j = 0; j <= newOrder; j++){
            if((i+j) == (derivativePoints.end())){
                tamana = true;
            }
        }
        if(tamana) break;

        std::vector<Circle> samplePoints;
        for(int j = 0; j <= r; j++){
            samplePoints.push_back(*(i+j));
        }
        for(int j = 0; j < v; j++){
            addPointParametric(samplePoints, (float) j/v, newCoeff, newOrder);
        }
    }

    normalizeAll();
}

void curveDerivative2(Circle points[], int newOrder){
    std::vector<Circle> derivativePoints;

    std::vector<int> newCoeff = pascalsTriangleRow(newOrder);
    for(int i = 1; i < noOfControlPoints; i++){
        Circle circle;
        circle.center = {points[i-1].center.x - points[i].center.x, points[i-1].center.y - points[i].center.y};
        circle.radius = POINT_RADIUS;
        if(i % newOrder == 0) circle.color = GREEN;
        else circle.color = RED;
        derivativePoints.push_back(circle);
    }
    // for(auto x : derivativePoints){
    //     std::cout << x.center.x << "|" << x.center.y << std::endl;
    // }

    for(auto i = derivativePoints.begin(); i != (derivativePoints.end()-1); i = i + newOrder){
        // addPoints(*i, *(i+1), *(i+2));
        bool tamana = false;
        for(int j = 0; j <= newOrder; j++){
            if((i+j) == (derivativePoints.end())){
                tamana = true;
            }
        }
        if(tamana) break;

        std::vector<Circle> samplePoints;
        for(int j = 0; j <= r; j++){
            samplePoints.push_back(*(i+j));
        }
        for(int j = 0; j < v; j++){
            addPointParametric2(samplePoints, (float) j/v, newCoeff, newOrder);
        }
    }

    normalizeAll2();
}


#endif