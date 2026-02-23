HW5 – Supervised and Unsupervised Machine Learning in C++

This project implements:

1) K-Means Clustering (Unsupervised Learning)
2) Linear Regression (Supervised Learning)

Both programs generate synthetic 2D datasets and produce PPM image visualizations.

Run K-Means:
./kmeans

Output:
- Iteration number and inertia printed in terminal
- Image file generated: kmeans_output.ppm

Run Linear Regression:
./regression

Output:
- Fitted equation printed
- R² score printed
- MSE printed
- MAE printed
- Image file generated: regression_output.ppm

K-Means Clustering:
1. Generates synthetic clustered 2D data
2. Randomly initializes K cluster centers
3. Assigns each point to the nearest center
4. Updates cluster centers as the mean of assigned points
5. Repeats until convergence or max iterations reached
6. Saves a colored cluster visualization

Linear Regression:
1. Generates synthetic linear data with noise
2. Computes slope and intercept using Least Squares method
3. Calculates R², MSE, and MAE
4. Saves regression visualization with fitted line
