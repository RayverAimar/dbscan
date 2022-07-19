#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>

using namespace std;

#define UNDEFINED 0
#define NOISE -1
#define N 2 //Dimensions

int n_points = 14;

struct Point{
    double* coords;
};

vector<int> labels(n_points, UNDEFINED);

double distance(Point one, Point two){
    double result = 0.0;
    for(int i = 0; i < N; i++){
        result += ((one.coords[i]-two.coords[i])*(one.coords[i]-two.coords[i]));
    }
    return sqrt(result);
}

vector<int> set_union(const vector<int>& one, const vector<int>& two){
    vector<int> union_set;
    set_union(one.begin(), one.end(), two.begin(), two.end(), back_inserter(union_set));
    return union_set;
}

vector<int> RangeQuery(vector<Point>& myPoints, const Point& p, double epsilon){
    vector<int> neighbors;
    for(int i = 0; i < myPoints.size(); i++){
        if(distance(p, myPoints[i]) <= epsilon){
            neighbors.push_back(i);
        }
    }
    return neighbors;
}

vector<int> dbscan(vector<Point>& myPoints, const double epsilon, const int minPoints){
    int cluster_id = 0;
    for(int i = 0; i < myPoints.size(); i++){
        if(labels[i] != UNDEFINED) continue;
        vector<int> neighbors = RangeQuery(myPoints, myPoints[i], epsilon);
        if(neighbors.size() < minPoints){
            labels[i] = NOISE;
            continue;
        }
        labels[i] = ++cluster_id;
        while(!neighbors.empty()){
            int cur = neighbors.back();
            neighbors.pop_back();
            if(labels[cur] == NOISE) labels[cur] = cluster_id;
            if(labels[cur] != UNDEFINED) continue;
            labels[cur] = cluster_id;
            vector<int> neighbors_of_neighbors = RangeQuery(myPoints, myPoints[cur], epsilon);
            if(neighbors_of_neighbors.size() >= minPoints){
                neighbors = set_union(neighbors, neighbors_of_neighbors);
            }
        }
    }
    return labels;
}

void read_from_file(vector<Point>& myPoints){
    string name = "../data/data.in";
    ifstream file(name.c_str());
    while(!file.eof()){
        Point p;
        p.coords = new double[N];
        file>>p.coords[0]>>p.coords[1];
        myPoints.push_back(p);
    }
    file.close();
}


int main(){
    vector<Point> myPoints;
    read_from_file(myPoints);
    labels = dbscan(myPoints, 4.0, 3);
    for(int i = 0; i < labels.size(); i++){
        cout<<"Cluster "<<labels[i]<<":  {"<<myPoints[i].coords[0]<<", "<<myPoints[i].coords[1]<<"}\n";
    }
}