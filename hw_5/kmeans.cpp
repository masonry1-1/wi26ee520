#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <limits>
#include <fstream>

using namespace std;

struct Point {
    double x, y;
    int cluster;
    Point(double xVal=0, double yVal=0) : x(xVal), y(yVal), cluster(-1) {}
};

class KMeans {
private:
    vector<Point> points;
    vector<Point> centers;
    int k;
    double tolerance;
    int maxIterations;
    int iterations;

public:
    KMeans(int kVal, double tol, int maxIter)
        : k(kVal), tolerance(tol), maxIterations(maxIter), iterations(0) {}

    void addPoint(double x, double y) {
        points.push_back(Point(x, y));
    }

    void generateSyntheticData(int numPoints) {
        random_device rd;
        mt19937 gen(rd());
        normal_distribution<> dis(0, 1);

        vector<pair<double,double>> clusterCenters = {
            {2,2}, {8,8}, {8,2}
        };

        int pointsPerCluster = numPoints / k;

        for(int i=0;i<k;i++){
            for(int j=0;j<pointsPerCluster;j++){
                double x = clusterCenters[i].first + dis(gen)*0.5;
                double y = clusterCenters[i].second + dis(gen)*0.5;
                addPoint(x,y);
            }
        }
    }

    double distance(const Point& p1, const Point& p2) {
        return sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
    }

    void initializeCenters() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, points.size()-1);

        centers.clear();
        for(int i=0;i<k;i++){
            centers.push_back(points[dis(gen)]);
        }
    }

    void assignPoints() {
        for(auto &p : points){
            double minDist = numeric_limits<double>::max();
            int bestCluster = -1;

            for(int i=0;i<k;i++){
                double d = distance(p, centers[i]);
                if(d < minDist){
                    minDist = d;
                    bestCluster = i;
                }
            }
            p.cluster = bestCluster;
        }
    }

    bool updateCenters() {
        vector<Point> newCenters(k);
        vector<int> counts(k,0);

        for(auto &p : points){
            newCenters[p.cluster].x += p.x;
            newCenters[p.cluster].y += p.y;
            counts[p.cluster]++;
        }

        bool converged = true;

        for(int i=0;i<k;i++){
            if(counts[i] > 0){
                newCenters[i].x /= counts[i];
                newCenters[i].y /= counts[i];
            }

            if(distance(centers[i], newCenters[i]) > tolerance){
                converged = false;
            }
        }

        centers = newCenters;
        return converged;
    }

    double calculateInertia() {
        double inertia = 0;
        for(auto &p : points){
            inertia += pow(distance(p, centers[p.cluster]),2);
        }
        return inertia;
    }

    void fit() {
        initializeCenters();

        for(iterations=0; iterations<maxIterations; iterations++){
            assignPoints();
            bool converged = updateCenters();
            cout << "Iteration " << iterations+1
                 << " | Inertia: " << calculateInertia() << endl;

            if(converged){
                cout << "Converged after " << iterations+1 << " iterations.\n";
                return;
            }
        }

        cout << "Max iterations reached.\n";
    }

    void saveAsImage(const string& filename) {
        const int width = 500;
        const int height = 500;
        vector<vector<vector<int>>> image(height,
            vector<vector<int>>(width, vector<int>(3,255)));

        vector<vector<int>> colors = {
            {255,0,0},{0,255,0},{0,0,255},{255,255,0},{255,0,255}
        };

        for(auto &p : points){
            int px = (int)(p.x/10.0*width);
            int py = (int)(p.y/10.0*height);
            if(px>=0 && px<width && py>=0 && py<height){
                image[py][px] = colors[p.cluster % colors.size()];
            }
        }

        ofstream out(filename);
        out << "P3\n" << width << " " << height << "\n255\n";
        for(int y=0;y<height;y++){
            for(int x=0;x<width;x++){
                out << image[y][x][0] << " "
                    << image[y][x][1] << " "
                    << image[y][x][2] << " ";
            }
            out << "\n";
        }
        out.close();
        cout << "Saved image: " << filename << endl;
    }
};

int main() {
    KMeans kmeans(3, 1e-4, 100);
    kmeans.generateSyntheticData(300);
    kmeans.fit();
    kmeans.saveAsImage("kmeans_output.ppm");
    return 0;
}
