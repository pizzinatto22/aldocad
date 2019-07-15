#ifndef MATRIX_H_
#define MATRIX_H_

#include <cstring>
#include <iostream>
#include <stdexcept>

namespace utils {
  template <unsigned h, unsigned w>
    class Matrix {
      public:
        constexpr Matrix() :
          m_{}
        {}

        template <typename ...T>
          constexpr Matrix(double first, T... next):
            m_{first, next...}
        {
        }

        constexpr double operator()(unsigned x, unsigned y) const
        {
          return (x < h && y < w) ? m_[(x*w)+y] : throw std::range_error("Tried to access out of range in Matrix");
        }

        double &operator()(unsigned x, unsigned y)
        {
          if (x < h && y < w) return m_[(x*w)+y];
          else throw std::range_error("Tried to access out of range in Matrix");
        }

        const static Matrix id;

        static const Matrix &identity()
        {
          static_assert(h == w,
                        "Identities only make sense for square matrices.");
          static auto id = make_identity();

          return id;
        }


        static Matrix translation(Matrix<1,w> m)
        {
            Matrix t;

            for (auto i = 0u; i < h; ++i) {
                for (auto j = 0u; j < w; ++j) {
                	if (i == j)
                		t(i,j) = 1;
                	else if (i == h - 1)
                		t(i,j) = m(0,j);
                	else
                		t(i,j) = 0;
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
                		t(i, j) = m(0,j);
                	else
                		t(i, j) = 0;
                }
            }

            return t;

        }

      private:
        static Matrix make_identity()
        {
          Matrix id;

          for (auto i = 0; i < h; ++i) {
            for (auto j = 0; j < w; ++j) {
              id.m_[(i*w)+j] = (i == j) ? 1.0 : 0.0;
            }
          }

          return id;
        }

        double m_[h*w];
    };

  inline const Matrix<4,4> &bSpline()
  {
	static auto spline = Matrix<4,4>{
		-1.0/6.0,  1.0/2.0, -1.0/2.0, 1.0/6.0,
		 1.0/2.0, -1.0    ,  1.0/2.0, 0.0,
		-1.0/2.0,  0.0    ,  1.0/2.0, 0.0,
		 1.0/6.0,  2.0/3.0,  1.0/6.0, 0.0
	};
	return spline;
  }

  inline const Matrix<4,4> &bSplineDelta(double delta)
  {
	double delta2 = delta *delta;
	double delta3 = delta2*delta;

  	static auto splineDelta = Matrix<4,4>{
  		   0.0,    0.0,     0.0, 1.0,
  		delta3,   delta2, delta, 0.0,
  	  6*delta3, 2*delta2,   0.0, 0.0,
  	  6*delta3,      0.0,   0.0, 0.0
  	};
  	return splineDelta;
  }

  template <unsigned h, unsigned w>
    inline const Matrix<h,w> &identity()
    {
      static auto id = Matrix<h,w>::identity();
      return id;
    }

  template <unsigned h, unsigned d, unsigned e, unsigned w>
    Matrix<h,w> operator+(const Matrix<h,d>& m1, const Matrix<e,w>& m2)
    {
      static_assert(h == e && d == w,
                    "Matrices in sum must have matching sizes.");
      Matrix<h,w> result;

      for (auto i = 0; i < h; i++) {
        for (auto j = 0; j < w; j++) {
          result(i,j) = m1(i,j) + m2(i,j);
        }
      }

      return result;
    }

  template <unsigned h, unsigned d, unsigned e, unsigned w>
    Matrix<h,w> operator*(const Matrix<h,d>& m1, const Matrix<e,w>& m2)
    {
      static_assert(d == e,
                    "Matrices in multiplication must have matching height and width.");
      Matrix<h,w> result;

      for (auto i = 0; i < h; i++) {
        for (auto j = 0; j < w; j++) {
          for (auto k = 0; k < d; k++) {
            result(i,j) += m1(i,k) * m2(k,j);
          }
        }
      }

      return result;
    }

  template <unsigned h, unsigned w>
    Matrix<w,h> transpose(const Matrix<h,w>& m)
    {
      Matrix<w,h> result;

      for (auto i = 0; i < h; i++) {
        for (auto j = 0; j < w; j++) {
          result(j,i) = m(i,j);
        }
      }

      return result;
    }

  template <unsigned h, unsigned w>
    std::ostream& operator<<(std::ostream& out, const Matrix<h,w>& m)
    {
      out << "Matrix<" << h << ", " << w << ">:" << std::endl;
      for (auto i = 0; i < h; i++) {
        for (auto j = 0; j < w; j++) {
          out << m(i,j);
          if (j < w-1) out << " ";
        }
        if (i < h-1) out << std::endl;
      }

      return out;
    }

  template <unsigned h, unsigned d, unsigned e, unsigned w>
    bool operator==(const Matrix<h,d>& m1, const Matrix<e,w>& m2)
    {
      static_assert(h == e && d == w,
                    "Matrices in equality comparison must have matching sizes.");

      for (auto i = 0; i < h; i++) {
        for (auto j = 0; j < w; j++) {
          if (m1(i,j) != m2(i,j)) return false;
        }
      }

      return true;
    }
}

#endif
