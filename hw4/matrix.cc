#include "matrix.h"

Matrix::Matrix() : data_(), rows_(0), cols_(0) {}

Matrix::Matrix(size_t rows, size_t cols)
    : data_(rows * cols, 0.0), rows_(rows), cols_(cols) {}

Matrix::Matrix(size_t rows, size_t cols, double value)
    : data_(rows * cols, value), rows_(rows), cols_(cols) {}

Matrix::Matrix(std::initializer_list<std::initializer_list<double>> list) {
    rows_ = list.size();
    cols_ = 0;
    for (auto& row : list) {
        if (cols_ == 0) cols_ = row.size();
        if (row.size() != cols_) throw std::invalid_argument("Dimension mismatch");
    }
    data_.assign(rows_ * cols_, 0.0);
    size_t r = 0;
    for (auto& row : list) {
        size_t c = 0;
        for (double v : row) {
            data_[r * cols_ + c] = v;
            c++;
        }
        r++;
    }
}

Matrix::Matrix(const Matrix& other)
    : data_(other.data_), rows_(other.rows_), cols_(other.cols_) {}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        data_ = other.data_;
        rows_ = other.rows_;
        cols_ = other.cols_;
    }
    return *this;
}

double& Matrix::operator()(size_t row, size_t col) {
    return data_[idx(row, col)];
}

const double& Matrix::operator()(size_t row, size_t col) const {
    return data_[idx(row, col)];
}

double& Matrix::at(size_t row, size_t col) {
    if (row >= rows_ || col >= cols_) throw std::out_of_range("Out of range");
    return data_[idx(row, col)];
}

const double& Matrix::at(size_t row, size_t col) const {
    if (row >= rows_ || col >= cols_) throw std::out_of_range("Out of range");
    return data_[idx(row, col)];
}

size_t Matrix::rows() const { return rows_; }
size_t Matrix::cols() const { return cols_; }
bool Matrix::isEmpty() const { return rows_ == 0 || cols_ == 0; }
bool Matrix::isSquare() const { return rows_ == cols_; }

Matrix Matrix::operator+(const Matrix& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) throw std::invalid_argument("Dimension mismatch");
    Matrix r(rows_, cols_);
    for (size_t i = 0; i < data_.size(); i++) r.data_[i] = data_[i] + other.data_[i];
    return r;
}

Matrix Matrix::operator-(const Matrix& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) throw std::invalid_argument("Dimension mismatch");
    Matrix r(rows_, cols_);
    for (size_t i = 0; i < data_.size(); i++) r.data_[i] = data_[i] - other.data_[i];
    return r;
}

Matrix Matrix::operator*(const Matrix& other) const {
    if (cols_ != other.rows_) throw std::invalid_argument("Dimension mismatch");
    Matrix r(rows_, other.cols_, 0.0);
    for (size_t i = 0; i < rows_; i++) {
        for (size_t k = 0; k < cols_; k++) {
            double aik = (*this)(i, k);
            for (size_t j = 0; j < other.cols_; j++) {
                r(i, j) += aik * other(k, j);
            }
        }
    }
    return r;
}

Matrix Matrix::operator*(double scalar) const {
    Matrix r(rows_, cols_);
    for (size_t i = 0; i < data_.size(); i++) r.data_[i] = data_[i] * scalar;
    return r;
}

Matrix operator*(double scalar, const Matrix& m) {
    return m * scalar;
}

Matrix Matrix::operator/(double scalar) const {
    if (std::fabs(scalar) <= EPSILON) throw std::invalid_argument("Division by zero");
    Matrix r(rows_, cols_);
    for (size_t i = 0; i < data_.size(); i++) r.data_[i] = data_[i] / scalar;
    return r;
}

Matrix Matrix::operator-() const {
    Matrix r(rows_, cols_);
    for (size_t i = 0; i < data_.size(); i++) r.data_[i] = -data_[i];
    return r;
}

Matrix& Matrix::operator+=(const Matrix& other) {
    if (rows_ != other.rows_ || cols_ != other.cols_) throw std::invalid_argument("Dimension mismatch");
    for (size_t i = 0; i < data_.size(); i++) data_[i] += other.data_[i];
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& other) {
    if (rows_ != other.rows_ || cols_ != other.cols_) throw std::invalid_argument("Dimension mismatch");
    for (size_t i = 0; i < data_.size(); i++) data_[i] -= other.data_[i];
    return *this;
}

Matrix& Matrix::operator*=(double scalar) {
    for (double& v : data_) v *= scalar;
    return *this;
}

Matrix& Matrix::operator/=(double scalar) {
    if (std::fabs(scalar) <= EPSILON) throw std::invalid_argument("Division by zero");
    for (double& v : data_) v /= scalar;
    return *this;
}

bool Matrix::operator==(const Matrix& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) return false;
    for (size_t i = 0; i < data_.size(); i++) {
        if (std::fabs(data_[i] - other.data_[i]) > EPSILON) return false;
    }
    return true;
}

bool Matrix::operator!=(const Matrix& other) const {
    return !(*this == other);
}

Matrix Matrix::transpose() const {
    Matrix r(cols_, rows_);
    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < cols_; j++) {
            r(j, i) = (*this)(i, j);
        }
    }
    return r;
}

double Matrix::trace() const {
    if (!isSquare()) throw std::logic_error("Trace on non-square matrix");
    double s = 0.0;
    for (size_t i = 0; i < rows_; i++) s += (*this)(i, i);
    return s;
}

Matrix Matrix::diagonal() const {
    size_t n = (rows_ < cols_) ? rows_ : cols_;
    Matrix r(n, 1);
    for (size_t i = 0; i < n; i++) r(i, 0) = (*this)(i, i);
    return r;
}

void Matrix::fill(double value) {
    for (double& v : data_) v = value;
}

double Matrix::norm() const {
    double s = 0.0;
    for (double v : data_) s += v * v;
    return std::sqrt(s);
}

Matrix Matrix::identity(size_t n) {
    Matrix r(n, n, 0.0);
    for (size_t i = 0; i < n; i++) r(i, i) = 1.0;
    return r;
}

Matrix Matrix::zeros(size_t rows, size_t cols) {
    return Matrix(rows, cols, 0.0);
}

Matrix Matrix::ones(size_t rows, size_t cols) {
    return Matrix(rows, cols, 1.0);
}

Matrix Matrix::diagonal(const std::vector<double>& diag) {
    Matrix r(diag.size(), diag.size(), 0.0);
    for (size_t i = 0; i < diag.size(); i++) r(i, i) = diag[i];
    return r;
}
