#include <iostream>
#include <complex>
#include <vector>
#include <type_traits>

template<class Type> class Matrix;

template<class Type> struct Extract { using value_type = Type; };
template<class Type> struct Extract<Matrix<Type>> { using value_type = Type; };
template<class Type> constexpr bool IsMatrix = false;
template<class Type> constexpr bool IsMatrix<Matrix<Type>> = true;

// base class 
// class SFINAE = std::enable_if_t<IsMatrix<Derived>, void>
template<class Derived>
class MatrixBase
{
	public:
	using value_type = typename Extract<Derived>::value_type;
	MatrixBase(std::size_t row, std::size_t col, value_type init = 0) : row_{row}, col_{col}, elements_(row * col, init) 
	{	// only deriving classes can instantiate this class because of protected dtor
		// static_assert(IsMatrix<Derived>, "can only be instantiated with Matrix classes");
	}
	// operations defined same for both real and complex matrices
	Derived t() const;
	void print() const;
	// access elements
	value_type& operator()(std::size_t i) { return elements_[i]; }
	const value_type& operator()(std::size_t i) const { return elements_[i]; }
	value_type& operator()(std::size_t i, std::size_t j) { return elements_[i * col_ + j]; }
	const value_type& operator()(std::size_t i, std::size_t j) const { return elements_[i * col_ + j]; }
	std::size_t size() const { return row_ * col_; }
	std::size_t row() const { return row_; }
	std::size_t col() const { return col_; }
	protected:
	// base class dtor should be either public virtual or protected nonvirtual
	~MatrixBase() = default;
	private:
	std::size_t row_, col_;
	std::vector<value_type> elements_;
};


template<class Derived>
Derived MatrixBase<Derived>::t() const
{
	Derived transposed(col_, row_);
	for(std::size_t i = 0; i < row_; ++i)
	{
		for(std::size_t j = 0; j < col_; ++j)
		{
			transposed(j, i) = this->operator()(i, j);
		}
	}
	return transposed;
}

template<class Derived>
void MatrixBase<Derived>::print() const
{	// should be compared with double loop implementation
	for(std::size_t i = 0, total = row_ * col_; i < total; ++i)
	{
		std::cout << elements_[i] << " ";
		if((i + 1) % col_ == 0) { std::cout << "\n"; }
	}
}

// real matrix
template<class Type>
class Matrix : public MatrixBase<Matrix<Type>>
{
	public:
	using self_type = Matrix<Type>;
	Matrix(std::size_t row, std::size_t col, Type init = 0) : MatrixBase<Matrix<Type>>{row, col, init} {}
	// operations not defined for complex matrices
	// ...
	private:
};

// complex matrix
template<class Type>
class Matrix<std::complex<Type>> : public MatrixBase<Matrix<std::complex<Type>>>
{
	public:
	using self_type = Matrix<std::complex<Type>>;
	Matrix(std::size_t row, std::size_t col, std::complex<Type> init = 0) : MatrixBase<Matrix<std::complex<Type>>>{row, col, init} {}
	// operations not defined for real matrices
	self_type conj() const;
	private:
};

template<class Type>
Matrix<std::complex<Type>> Matrix<std::complex<Type>>::conj() const
{
	self_type conjugated(this->row(), this->col());
	for(std::size_t i = 0; i < this->size(); ++i)
	{
		conjugated(i) = std::conj(conjugated(i));
	}
	return conjugated;
}

int main()
{
	Matrix<double> a(2, 3);
	a(1, 0) = 3;
	a.print();
	a.t().print();

	Matrix<std::complex<double>> b(3, 3);
	b.conj().print();

	return 0;
}
