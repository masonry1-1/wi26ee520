#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>

using namespace std;

struct DataPoint {
    double x, y;
    double predicted;
    DataPoint(double xv=0,double yv=0) : x(xv), y(yv), predicted(0){}
};

class LinearRegression {
private:
    vector<DataPoint> data;
    double slope;
    double intercept;
    double rSquared;

public:
    void addPoint(double x,double y){
        data.push_back(DataPoint(x,y));
    }

    void generateSyntheticData(int numPoints=100,
        double trueSlope=2.5,double trueIntercept=1.0,double noise=0.5){

        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> xDis(0,10);
        normal_distribution<> noiseDis(0,noise);

        for(int i=0;i<numPoints;i++){
            double x = xDis(gen);
            double y = trueSlope*x + trueIntercept + noiseDis(gen);
            addPoint(x,y);
        }

        cout << "Generated " << numPoints << " points\n";
    }

    void fit(){
        double sumX=0,sumY=0,sumXY=0,sumXX=0;
        int n=data.size();

        for(auto &p:data){
            sumX+=p.x;
            sumY+=p.y;
            sumXY+=p.x*p.y;
            sumXX+=p.x*p.x;
        }

        slope = (n*sumXY - sumX*sumY) / (n*sumXX - sumX*sumX);
        intercept = (sumY - slope*sumX)/n;

        calculateMetrics();

        cout<<"Model: y = "<<slope<<"x + "<<intercept<<"\n";
        cout<<"R^2: "<<rSquared<<"\n";
        cout<<"MSE: "<<calculateMSE()<<"\n";
        cout<<"MAE: "<<calculateMAE()<<"\n";
    }

    void calculateMetrics(){
        double ssTotal=0,ssResidual=0;
        double meanY=0;
        int n=data.size();

        for(auto &p:data) meanY+=p.y;
        meanY/=n;

        for(auto &p:data){
            p.predicted = slope*p.x + intercept;
            ssTotal += pow(p.y-meanY,2);
            ssResidual += pow(p.y-p.predicted,2);
        }

        rSquared = 1 - (ssResidual/ssTotal);
    }

    double calculateMSE(){
        double mse=0;
        for(auto &p:data){
            mse += pow(p.y-p.predicted,2);
        }
        return mse/data.size();
    }

    double calculateMAE(){
        double mae=0;
        for(auto &p:data){
            mae += abs(p.y-p.predicted);
        }
        return mae/data.size();
    }

    double predict(double x){
        return slope*x + intercept;
    }

    void saveAsImage(const string& filename){
        const int width=500,height=500;
        vector<vector<vector<int>>> image(height,
            vector<vector<int>>(width, vector<int>(3,255)));

        for(auto &p:data){
            int px=(int)(p.x/10.0*width);
            int py=(int)(p.y/30.0*height);
            if(px>=0&&px<width&&py>=0&&py<height){
                image[py][px]={0,0,255};
            }
        }

        for(int x=0;x<width;x++){
            double realX = x/ (double)width * 10.0;
            double y = slope*realX + intercept;
            int py=(int)(y/30.0*height);
            if(py>=0&&py<height){
                image[py][x]={255,0,0};
            }
        }

        ofstream out(filename);
        out<<"P3\n"<<width<<" "<<height<<"\n255\n";
        for(int y=0;y<height;y++){
            for(int x=0;x<width;x++){
                out<<image[y][x][0]<<" "
                   <<image[y][x][1]<<" "
                   <<image[y][x][2]<<" ";
            }
            out<<"\n";
        }
        out.close();
        cout<<"Saved image: "<<filename<<endl;
    }
};

int main(){
    LinearRegression lr;
    lr.generateSyntheticData();
    lr.fit();
    lr.saveAsImage("regression_output.ppm");
    return 0;
}
