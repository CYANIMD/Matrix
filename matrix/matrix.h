#ifndef __MATRIX_
#define __MATRIX_

#include <iostream>
#include <concepts>
#include <cmath>
#include <vector>
#include <random>

namespace IMD {
	template<typename T>
	//Концепт целых чисел.
	concept Integral = std::is_integral_v<T>;
	template<typename T>
	//Концепт вещественных чисел.
	concept Floating = std::is_floating_point_v<T>;
	template<typename T>
	//Концепт целых и вещественных чисел.
	concept Numeric = Integral<T> || Floating<T>;
	template<typename T, typename T1>
	using ResultTypeSum = decltype(std::declval<T>() + std::declval<T1>());
	template<typename T, typename T1>
	using ResultTypeMinus = decltype(std::declval<T>() - std::declval<T1>());
	template<typename T, typename T1>
	using ResultTypeProduct = decltype(std::declval<T>() * std::declval<T1>());
	template<typename T, typename T1>
	using ResultTypeDivision = decltype(std::declval<T>() / std::declval<T1>());
	//Класс, реализующий "матрицу".
	template<typename ValueType>
	class matrix {
	private:
		ValueType** _data;
		size_t _rows;
		size_t _cols;
	public:
		//Простой конструктор.
		explicit matrix(size_t rows = 0, size_t cols = 0, const ValueType& value = ValueType()) : _rows(rows), _cols(cols) {
			_data = new ValueType * [_rows];
			for (size_t i{ 0 }; i < _rows; i++) {
				_data[i] = new ValueType[_cols];
			}
			this->fill(value);
		}
		//Конструктор копии.
		explicit matrix(const matrix<ValueType>& other) : _rows(other.rows()), _cols(other.cols()) {
			_data = new ValueType * [rows()];
			for (size_t i{ 0 }; i < rows(); i++) {
				_data[i] = new ValueType[cols()];
				for (size_t j{ 0 }; j < cols(); ++j) {
					_data[i][j] = other._data[i][j];
				}
			}
		}
		//Move-конструктор копии.
		matrix(matrix<ValueType>&& other) noexcept {
			this->swap(other);
		}
		//Оператор приведения матрицы к заданному типу.
		template<typename OtherValueType>
		operator matrix<OtherValueType>() {
			matrix<OtherValueType> result{ rows(), cols() };
			for (size_t i{ 0 }; i < rows(); ++i)
				for (size_t j{ 0 }; j < cols(); ++j)
					result(i, j) = (OtherValueType)(this->operator()(i, j));
			return result;
		}
		//Оператор копирующего присваивания.
		matrix<ValueType>& operator=(const matrix<ValueType>& other) {
			if (this != &other) {
				clear(); //В теле clear() вызывается метод free()
				_rows = (other.rows());
				_cols = (other.cols());
				_data = new ValueType * [rows()];
				for (size_t i{ 0 }; i < rows(); i++) {
					_data[i] = new ValueType[cols()];
					for (size_t j{ 0 }; j < cols(); ++j) {
						_data[i][j] = other._data[i][j];
					}
				}
			}
			return *this;
		}
		//Move-конструктор копии.
		matrix<ValueType>& operator=(matrix<ValueType>&& other) noexcept {
			this->swap(other);
			return *this;
		}
		//Деструктор.
		~matrix() {
			free();
		}
		//Возвращает элемент матрицы с заданными индексами.
		ValueType& operator()(size_t row, size_t col) {
			if (row > rows()) throw std::invalid_argument("The row is greater than count of matrix rows");
			if (col > cols()) throw std::invalid_argument("The col is greater than count of matrix cols");
			return _data[row][col];
		}
		//Возвращает элемент матрицы с заданными индексами.
		const ValueType& operator()(size_t row, size_t col) const {
			if (row > rows()) throw std::invalid_argument("The row is greater than count of matrix rows");
			if (col > cols()) throw std::invalid_argument("The col is greater than count of matrix cols");
			return _data[row][col];
		}
		//Возвращает число строк.
		constexpr size_t rows() const { return _rows; }
		//Возвращает число столбцов.
		constexpr size_t cols() const { return _cols; }
		//Возвращает количество элементов.
		constexpr size_t size() const { return cols() * rows(); };
		//Очищение матрицы.
		void clear() {
			_rows = (0);
			_cols = (0);
			free();
		}
		//Проверяет, пуста ли матрица.
		constexpr bool empty() const { return _data == nullptr; }
		//Заполнение матрицы заданным значением.
		void fill(const ValueType& value) {
			for (size_t i{ 0 }; i < rows(); ++i)
				for (size_t j{ 0 }; j < cols(); ++j)
					this->operator()(i, j) = value;
		}
		//Меняет местами данные матриц.
		void swap(matrix<ValueType>& other) {
			std::swap(this->_rows, other._rows);
			std::swap(this->_cols, other._cols);
			std::swap(this->_data, other._data);
		}
		//Проверяет, является ли матрица квадратной.
		constexpr bool is_square() const {
			return rows() == cols();
		}
		//Проверяет, является ли матрица симметричной.
		constexpr bool is_symmetric() const {
			for (size_t i{ 0 }; i < rows(); ++i)
				for (size_t j{ 0 }; j < cols(); ++j)
					if (operator()(i, j) != operator()(j, i)) return false;
			return true;
		}
	private:
		//Освобождение памяти, выделенной под _data.
		//Поля _rows и _cols не изменяются.
		void free() {
			for (size_t i{ 0 }; i < rows(); i++) {
				delete[] _data[i];
				_data[i] = nullptr;
			}
			delete[] _data;
			_data = nullptr;
		}
	public:
		//Транспонирование матрицы произвольного размера.
		friend void transpose(matrix<ValueType>& mrx) {
			ValueType** new_data = new ValueType * [mrx.cols()];
			for (size_t i{ 0 }; i < mrx.cols(); i++) {
				new_data[i] = new ValueType[mrx.rows()];
				for (size_t j{ 0 }; j < mrx.rows(); ++j)
					new_data[i][j] = mrx._data[j][i];
			}
			mrx.free();
			std::swap(mrx._data, new_data);
			std::swap(mrx._rows, mrx._cols);
		}
		//Разворачивает матрицу произвольного размера на pi/2 радиан (90 градусов).
		friend void rotate(matrix<ValueType>& mrx) {
			transpose(mrx);
			for (size_t i{ 0 }; i < mrx.rows(); ++i) {
				size_t left = 0;
				size_t right = mrx.cols() - 1;
				while (left < right) {
					std::swap(mrx._data[i][left], mrx._data[i][right]);
					++left;
					--right;
				}
			}
		}
	};
	//Печать в консоль.
	template<typename ValueType>
	void print(const IMD::matrix<ValueType>& mrx, const char* separator = " ") {
		for (size_t i{ 0 }; i < mrx.rows(); ++i) {
			for (size_t j{ 0 }; j < mrx.cols(); ++j)
				std::cout << mrx(i, j) << separator;
			std::cout << std::endl;
		}
	}
	//Печать в консоль с переходом на следующую строку.
	template<typename ValueType>
	void println(const IMD::matrix<ValueType>& mrx, const char* separator = " ") {
		print(mrx, separator);
		std::cout << std::endl;
	}
	//template<Numeric T>
	//bool inverse_matrix(const matrix<T>& input, matrix<T>& output) {
	//	if (input.rows() != input.cols()) return false;

	//	matrix<T> gauss_matrix{ input.rows() * 2, input.cols() * 2 }; //Создание матрицы (input | E)
	//	size_t current_row{ 0 };
	//	for (size_t i{ 0 }; i < input.cols(); ++i) {
	//		T current_value = input[current_row][i];
	//		for (size_t j{ 0 }; j < input.rows(); ++j) {
	//			if (j != current_row) {
	//				input[][] += 
	//			}
	//		}
	//	}
	//}
	// 
	//Возвращает определитель квадратной матрицы.
	template<Numeric T>
	T determinate(const matrix<T>& mrx) {
		if (mrx.rows() != mrx.cols()) throw std::invalid_argument("The matrix isn't square");

		T result{ 0 };
		size_t rows{ mrx.rows() };

		//Загатовки для вычисления детерминантов матриц размера 1x1 и 2x2.
		if (rows == 1) return mrx(0, 0);
		if (rows == 2) return mrx(0, 0) * mrx(1, 1) - mrx(0, 1) * mrx(1, 0);

		for (size_t i{ 0 }; i < rows; ++i) {
			matrix<T> minor(rows - 1, rows - 1); //Создание минора матрицы.
			for (size_t j{ 1 }; j < rows; ++j) {
				size_t col{ 0 };
				for (size_t k{ 0 }; k < rows; ++k) {
					if (k != i) {
						minor(j - 1, col) = mrx(j, k);
						col++;
					}
				}
			}
			result += (i % 2 == 0 ? 1 : -1) * mrx(0, i) * determinate(minor);
		}
		return result;
	}
	//Возвращает вектор элементов матрицы, расположенных в спиральном порядке.
	template<typename ValueType>
	std::vector<ValueType> spiral_values(const matrix<ValueType>& mrx) {
		long left{ 0 };
		long right = mrx.cols() - 1;
		long bottom = mrx.rows() - 1;
		long top{ 0 };
		std::vector<ValueType> result{};

		while (left <= right && top <= bottom) {
			for (int i{ left }; i <= right; ++i) {
				result.push_back(mrx(top, i));
			}
			++top;
			for (int i{ top }; i <= bottom; ++i) {
				result.push_back(mrx(i, right));
			}
			--right;
			if (top <= bottom) {
				for (int i{ right }; i >= left; --i) {
					result.push_back(mrx(bottom, i));
				}
				--bottom;
			}
			if (left <= right) {
				for (int i{ bottom }; i >= top; --i) {
					result.push_back(mrx(i, left));
				}
				++left;
			}
		}
		return result;
	}
	template<typename ValueType, typename P>
	matrix<ResultTypeProduct<ValueType, P>> operator*(const matrix<ValueType>& mrx, const P& value) {
		matrix<ValueType> result{ mrx };
		for (size_t i{ 0 }; i < result.rows(); ++i) {
			for (size_t j{ 0 }; j < result.cols(); ++j)
				result(i, j) *= value;
		}
		return result;
	}
	template<typename ValueType, typename P>
	matrix<ResultTypeProduct<ValueType, P>> operator*(const P& value, const matrix<ValueType>& mrx) {
		return mrx * value;
	}
	template<typename ValueType, typename D>
	matrix<ResultTypeDivision<ValueType, D>>& operator/(matrix<ValueType>& mrx, const D& value) {
		return mrx * 1 / value;
	}
	template<typename ValueType, typename D>
	matrix<ResultTypeDivision<ValueType, D>>& operator/(const ValueType& value, matrix<D>& mrx) {
		return mrx * 1 / value;
	}
	template<typename ValueType, typename P> requires requires(ValueType a, P b) { { a * b } -> std::same_as<ValueType>; }
	matrix<ValueType>& operator*=(matrix<ValueType>& mrx, const P& value) {
		for (size_t i{ 0 }; i < mrx.rows(); ++i) {
			for (size_t j{ 0 }; j < mrx.cols(); ++j)
				mrx(i, j) *= value;
		}
		return mrx;
	}
	template<typename ValueType, typename D> requires requires(ValueType a, D b) { { a / b } -> std::same_as<ValueType>; }
	matrix<ValueType>& operator/=(matrix<ValueType>& mrx, const D& value) {
		mrx *= 1 / value;
		return mrx;
	}
	template<typename T0, typename T1>
	matrix<ResultTypeSum<T0, T1>> operator+(const matrix<T1>& first, const matrix<T0>& second) {
		if (first.rows() != second.rows() || first.cols() != second.cols())
			throw std::invalid_argument("The matrixes have uncorrect size");
		matrix<ResultTypeSum<T0, T1>> result{ first.rows(), first.cols() };
		for (size_t i{ 0 }; i < result.rows(); ++i) {
			for (size_t j{ 0 }; j < result.cols(); ++j)
				result(i, j) = first(i, j) + second(i, j);
		}
		return result;
	}
	template<typename T0, typename T1>
	matrix<ResultTypeMinus<T0, T1>> operator-(const matrix<T0>& first, const matrix<T1>& second) {
		return first + (-1) * second;
	}
	template<typename T0, typename T1> requires requires(T0 a, T1 b) { { a + b } -> std::same_as<T1>; }
	matrix<T1>& operator+=(matrix<T1>& first, const matrix<T0>& second) {
		if (first.rows() != second.rows() || first.cols() != second.cols())
			throw std::invalid_argument("The matrixes have uncorrect size");
		first = first + second;
		return first;
	}
	template<typename T0, typename T1>requires requires(T0 a, T1 b) { { a - b } -> std::same_as<T1>; }
	matrix<T1>& operator-=(matrix<T0>& first, const matrix<T1>& second) {
		return first += (-1) * second;
	}
	//Возвращает матрица заданного размера, заполненную случайный целыми числами в диапазоне [min_value; max_value).
	template<Integral T>
	inline matrix<T> generate_random_matrix(size_t rows, size_t cols, T min_value, T max_value) {
		std::srand(std::time(nullptr));
		matrix<int> result{ rows, cols };
		for (size_t i{ 0 }; i < rows; ++i)
			for (size_t j{ 0 }; j < cols; ++j)
				result(i, j) = std::rand() % max_value + min_value;
		return result;
	}
	//Возвращает матрица заданного размера, заполненную случайный вещественными числами в диапазоне [min_value; max_value).
	template<Floating T>
	inline matrix<T> generate_random_matrix(size_t rows, size_t cols, T min_value, T max_value) {
		matrix<T> result{ rows, cols };
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution dist(min_value, max_value);
		for (size_t i{ 0 }; i < rows; ++i)
			for (size_t j{ 0 }; j < cols; ++j) {
				auto x = dist(gen);
				result(i, j) = x;
			}
		return result;
	}
}



#endif
