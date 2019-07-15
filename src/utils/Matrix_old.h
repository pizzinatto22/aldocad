#ifndef CG_MATRIX_H_
#define CG_MATRIX_H_

#include <cstring>
#include <iostream>
#include <stdexcept>

namespace utils {
    template <std::size_t h, std::size_t w>
        class Matrix {
        public:
            Matrix() {
                memset(m_, 0, sizeof(double[h][w]));
            }

            Matrix(const double m[h][w]) { memcpy(m_, m, sizeof(double[h][w])); }

            double operator()(unsigned x, unsigned y) const
            {
                if (x < h && y < w) return m_[x][y];
                else throw std::range_error("Tried to access out of range in Matrix");
            }

            double &operator()(unsigned x, unsigned y)
            {
                if (x < h && y < w) return m_[x][y];
                else throw std::range_error("Tried to access out of range in Matrix");
            }

            static Matrix identity() {
                Matrix id;

                for (auto i = 0u; i < h; ++i) {
                    for (auto j = 0u; j < w; ++j) {
                        id.m_[i][j] = (i == j) ? 1.0 : 0.0;
                    }
                }

                return id;
            }

            static Matrix translation(Matrix<1,w> m)
            {
                Matrix t;

                for (auto i = 0u; i < h; ++i) {
                    for (auto j = 0u; j < w; ++j) {
                    	if (i == j)
                    		t.m_[i][j] = 1;
                    	else if (i == h - 1)
                    		t.m_[i][j] = m(0,j);
                    	else
                    		t.m_[i][j] = 0;
                    }
                }

                return t;

            }

            static Matrix diagonalize(Matrix<1,w> m)
            {
                Matrix t;

                for (auto i = 0u; i < h; ++i) {
                    for (auto j = 0u; j < w; ++j) {
                    	if (i == j)
                    		t.m_[i][j] = m(0,j);
                    	else
                    		t.m_[i][j] = 0;
                    }
                }

                return t;

            }

        protected:
            double m_[h][w];
        };

    template <std::size_t h, std::size_t d, std::size_t e, std::size_t w>
        Matrix<h,w> operator+(const Matrix<h,d>& m1, const Matrix<e,w>& m2)
        {
            static_assert(h == e && d == w,
                    "Matrices in sum must have matching sizes.");
            Matrix<h,w> result;

            for (auto i = 0u; i < h; i++) {
                for (auto j = 0u; j < w; j++) {
                    result(i,j) = m1(i,j) + m2(i,j);
                }
            }

            return result;
        }

    template <std::size_t h, std::size_t d, std::size_t e, std::size_t w>
        Matrix<h,w> operator*(const Matrix<h,d>& m1, const Matrix<e,w>& m2)
        {
            static_assert(d == e,
                    "Matrices in multiplication must have matching height and width.");
            Matrix<h,w> result;

            for (auto i = 0u; i < h; i++) {
                for (auto j = 0u; j < w; j++) {
                    for (auto k = 0u; k < d; k++) {
                        result(i,j) += m1(i,k) * m2(k,j);
                    }
                }
            }

            return result;
        }

    template <std::size_t h, std::size_t w>
        Matrix<w,h> transpose(const Matrix<h,w>& m)
        {
            Matrix<w,h> result;

            for (auto i = 0u; i < h; i++) {
                for (auto j = 0u; j < w; j++) {
                    result(j,i) = m(i,j);
                }
            }

            return result;
        }

    template <std::size_t h, std::size_t w>
        std::ostream& operator<<(std::ostream& out, const Matrix<h,w>& m)
        {
            out << "Matrix<" << h << ", " << w << ">:" << std::endl;
            for (auto i = 0u; i < h; i++) {
                for (auto j = 0u; j < w; j++) {
                    out << m(i,j);
                    if (j < w-1) out << " ";
                }
                if (i < h-1) out << std::endl;
            }

            return out;
        }

    template <std::size_t h, std::size_t d, std::size_t e, std::size_t w>
        bool operator==(const Matrix<h,d>& m1, const Matrix<e,w>& m2)
        {
            static_assert(h == e && d == w,
                    "Matrices in equality comparison must have matching sizes.");

            for (auto i = 0u; i < h; i++) {
                for (auto j = 0u; j < w; j++) {
                    if (m1(i,j) != m2(i,j)) return false;
                }
            }

            return true;
        }
}


#endif
