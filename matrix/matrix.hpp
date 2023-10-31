#pragma once

#include <algorithm>
#include <vector>

template <size_t N, size_t M, typename T = int64_t>
class Matrix {
 private:
  std::vector<std::vector<T>> my_matr_;

 public:
  Matrix() {
    my_matr_.resize(N);
    for (size_t i = 0; i < N; ++i) {
      my_matr_[i].resize(M);
    }
  }

  Matrix(const std::vector<std::vector<T>>& vector) : Matrix() {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        my_matr_[i][j] = vector[i][j];
      }
    }
  }

  Matrix(const T& elem) : Matrix() {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        my_matr_[i][j] = elem;
      }
    }
  }

  ~Matrix() = default;

  Matrix& operator+=(const Matrix& other) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        my_matr_[i][j] += other.my_matr_[i][j];
      }
    }
    return *this;
  }

  friend Matrix operator+(const Matrix& first, const Matrix& second) {
    Matrix result = first;
    result += second;
    return result;
  }

  Matrix& operator-=(const Matrix& other) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        my_matr_[i][j] -= other.my_matr_[i][j];
      }
    }
    return *this;
  }
  friend Matrix operator-(const Matrix& first, const Matrix& second) {
    Matrix result = first;
    result -= second;
    return result;
  }

  Matrix& operator*=(const T& elem) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        my_matr_[i][j] *= elem;
      }
    }
    return *this;
  }

  friend Matrix operator*(const Matrix& matr, const T& elem) {
    Matrix result = matr;
    result *= elem;
    return result;
  }

  template <size_t L>
  Matrix<N, L, T>& operator*=(const Matrix<M, L, T>& other) {
    Matrix<N, L, T> res = *this * other;
    my_matr_.resize(N);
    for (size_t i = 0; i < N; ++i) {
      my_matr_[i].resize(L);
      for (size_t j = 0; j < L; ++j) {
        my_matr_[i][j] = res(i, j);
      }
    }
    return *this;
  }

  template <size_t L>
  friend Matrix<N, L, T> operator*(const Matrix<N, M, T>& first,
                                   const Matrix<M, L, T>& second) {
    Matrix<N, L, T> res;
    Matrix<L, M, T> transp = second.Transposed();
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < L; ++j) {
        for (size_t k = 0; k < M; ++k) {
          res(i, j) += first.my_matr_[i][k] * transp(j, k);
        }
      }
    }
    return res;
  }

  Matrix<M, N, T> Transposed() const {
    Matrix<M, N, T> transp;
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        transp(j, i) = my_matr_[i][j];
      }
    }
    return transp;
  }

  T& operator()(size_t ind_i, size_t ind_j) { return my_matr_[ind_i][ind_j]; }
  const T& operator()(size_t ind_i, size_t ind_j) const {
    return my_matr_[ind_i][ind_j];
  }
  friend bool operator==(const Matrix<N, M, T>& first,
                         const Matrix<N, M, T>& second) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        if (first.my_matr_[i][j] != second.my_matr_[i][j]) {
          return false;
        }
      }
    }
    return true;
  }
};

template <size_t N, typename T>
class Matrix<N, N, T> {
 private:
  std::vector<std::vector<T>> my_matr_;

 public:
  Matrix() {
    my_matr_.resize(N);
    for (size_t i = 0; i < N; ++i) {
      my_matr_[i].resize(N);
    }
  }
  Matrix(const std::vector<std::vector<T>>& vector) : Matrix() {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < N; ++j) {
        my_matr_[i][j] = vector[i][j];
      }
    }
  }
  Matrix(const T& elem) : Matrix() {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < N; ++j) {
        my_matr_[i][j] = elem;
      }
    }
  }
  ~Matrix() = default;

  Matrix& operator+=(const Matrix& other) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < N; ++j) {
        my_matr_[i][j] += other.my_matr_[i][j];
      }
    }
    return *this;
  }
  friend Matrix operator+(const Matrix<N, N, T>& first,
                          const Matrix<N, N, T>& second) {
    Matrix<N, N, T> res = first;
    res += second;
    return res;
  }

  Matrix& operator-=(const Matrix& other) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < N; ++j) {
        my_matr_[i][j] -= other.my_matr_[i][j];
      }
    }
    return *this;
  }
  friend Matrix operator-(const Matrix<N, N, T>& first,
                          const Matrix<N, N, T>& second) {
    Matrix<N, N, T> res = first;
    res -= second;
    return res;
  }

  Matrix& operator*=(const T& elem) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < N; ++j) {
        my_matr_[i][j] *= elem;
      }
    }
    return *this;
  }

  friend Matrix operator*(const Matrix<N, N, T>& matr, const T& elem) {
    Matrix<N, N, T> res = matr;
    res *= elem;
    return res;
  }

  template <size_t L>
  Matrix& operator*=(const Matrix<N, L, T>& other) {
    Matrix<N, L, T> res = *this * other;
    my_matr_.resize(N);
    for (size_t i = 0; i < N; ++i) {
      my_matr_[i].resize(L);
      for (size_t j = 0; j < L; ++j) {
        my_matr_[i][j] = res(i, j);
      }
    }
    return *this;
  }

  template <size_t L>
  friend Matrix<N, L, T> operator*(const Matrix& first,
                                   const Matrix<N, L, T>& second) {
    Matrix<N, L, T> res;
    Matrix<L, N, T> transp = second.Transposed();
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < L; ++j) {
        for (size_t k = 0; k < N; ++k) {
          res(i, j) += first.my_matr_[i][k] * transp(j, k);
        }
      }
    }
    return res;
  }

  Matrix<N, N, T> Transposed() const {
    Matrix<N, N, T> transp;
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < N; ++j) {
        transp(j, i) = my_matr_[i][j];
      }
    }
    return transp;
  }
  T Trace() const {
    T res = 0;
    for (size_t i = 0; i < N; ++i) {
      res += my_matr_[i][i];
    }
    return res;
  }

  T& operator()(size_t ind_i, size_t ind_j) { return my_matr_[ind_i][ind_j]; }

  const T& operator()(size_t ind_i, size_t ind_j) const {
    return my_matr_[ind_i][ind_j];
  }

  friend bool operator==(const Matrix<N, N, T>& first,
                         const Matrix<N, N, T>& second) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < N; ++j) {
        if (first.my_matr_[i][j] != second.my_matr_[i][j]) {
          return false;
        }
      }
    }
    return true;
  }
};