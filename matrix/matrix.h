#ifndef __MATRIX_
#define __MATRIX_

#include <iostream>
#include <concepts>
#include <cmath>

template<typename T>
concept Numeric = std::is_integral_v<T> || std::is_floating_point_v<T>;

namespace IMD {
	//Класс, реализующий "матрицу".
	template<typename T>
	class matrix {
	public:
		//Размерный тип.
		using size_type = size_t;
		//Значимый тип.
		using value_type = T;
	private:
		value_type** _data;
		size_type _rows;
		size_type _cols;
	public:
		//Простой конструктор.
		explicit matrix(size_type rows = 0, size_type cols = 0, const value_type& value = value_type()) : _rows(rows), _cols(cols) {
			_data = new value_type * [_rows];
			for (size_type i{ 0 }; i < _rows; i++) {
				_data[i] = new value_type[_cols];
			}
			this->fill(value);
		}
		//Конструктор копии.
		explicit matrix(const matrix<value_type>& other) : _rows(other.rows()), _cols(other.cols()) {
			_data = new value_type * [rows()];
			for (size_type i{ 0 }; i < rows(); i++) {
				_data[i] = new value_type[cols()];
				for (size_type j{ 0 }; j < cols(); ++j) {
					_data[i][j] = other._data[i][j];
				}
			}
		}
		//Move-конструктор копии.
		matrix(matrix<value_type>&& other) {
			this->swap(other);
		}
		//Оператор копирующего присваивания.
		matrix<value_type>& operator=(const matrix<value_type>& other) {
			if (this != &other) {
				clear(); //В теле clear() вызывается метод free()
				_rows = (other.rows());
				_cols = (other.cols());
				_data = new value_type * [rows()];
				for (size_type i{ 0 }; i < rows(); i++) {
					_data[i] = new value_type[cols()];
					for (size_type j{ 0 }; j < cols(); ++j) {
						_data[i][j] = other._data[i][j];
					}
				}
			}
		}
		//Move-конструктор копии.
		matrix<value_type>& operator=(matrix<value_type>&& other) {
			this->swap(other);
			return *this;
		}
		//Деструктор.
		~matrix() {
			free();
		}
		//Возвращает элемент матрицы с заданными индексами.
		value_type& operator()(size_type row, size_type col) {
			if (row > rows()) throw std::invalid_argument("The row is greater than count of matrix rows");
			if (col > cols()) throw std::invalid_argument("The col is greater than count of matrix cols");
			return _data[row][col];
		}
		//Возвращает элемент матрицы с заданными индексами.
		const value_type& operator()(size_type row, size_type col) const {
			if (row > rows()) throw std::invalid_argument("The row is greater than count of matrix rows");
			if (col > cols()) throw std::invalid_argument("The col is greater than count of matrix cols");
			return _data[row][col];
		}
		//Возвращает число строк.
		size_type rows() const { return _rows; }
		//Возвращает число столбцов.
		size_type cols() const { return _cols; }
		//Очищение матрицы.
		void clear() {
			_rows = (0);
			_cols = (0);
			free();
		}
		//Проверяет, пуста ли матрица.
		bool empty() const { return _data == nullptr; }
		//Заполнение матрицы заданным значением.
		void fill(const value_type& value) {
			for (size_type i{ 0 }; i < rows(); ++i)
				for (size_type j{ 0 }; j < cols(); ++j)
					this->operator()(i, j) = value;
		}
		//Меняет местами данные матриц.
		void swap(matrix<value_type>& other) {
			std::swap(this->_rows, other._rows);
			std::swap(this->_cols, other._cols);
			std::swap(this->_data, other._data);
		}
		//Транспонирование.
		void tr() {
			value_type** new_data = new value_type * [cols()];
			for (size_type i{ 0 }; i < cols(); i++) {
				new_data[i] = new value_type[rows()];
				for (size_type j{ 0 }; j < rows(); ++j)
					new_data[i][j] = _data[j][i];
			}
			free();
			std::swap(_data, new_data);
			std::swap(_rows, _cols);
		}
	private:
		//Освобождение памяти, выделенной под _data.
		//Поля _rows и _cols не изменяются.
		void free() {
			for (size_type i{ 0 }; i < rows(); i++) {
				delete[] _data[i];
				_data[i] = nullptr;
			}
			delete[] _data;
			_data = nullptr;
		}
	};
	//Печать в консоль.
	template<typename T>
	void print(const IMD::matrix<T>& mrx, const char* separator = " ") {
		for (typename IMD::matrix<T>::size_type i{ 0 }; i < mrx.rows(); ++i) {
			for (typename IMD::matrix<T>::size_type j{ 0 }; j < mrx.cols(); ++j)
				std::cout << mrx(i, j) << separator;
			std::cout << std::endl;
		}
	}
	//Печать в консоль с переходом на следующую строку.
	template<typename T>
	void println(const IMD::matrix<T>& mrx, const char* separator = " ") {
		print(mrx, separator);
		std::cout << std::endl;
	}
	//template<Numeric T>
	//bool inverse_matrix(const matrix<T>& input, matrix<T>& output) {
	//	if (input.rows() != input.cols()) return false;

	//	matrix<T> gauss_matrix{ input.rows() * 2, input.cols() * 2 }; //Создание матрицы (input | E)
	//	for (typename IMD::matrix<T>::size_type i{ 0 }; i < input.rows(); ++i) {
	//		for (typename IMD::matrix<T>::size_type j{ 0 }; j < input.cols(); ++j) {
	//			gauss_matrix[i][j] = input[i][j];
	//			if (i == j) gauss_matrix[i * 2][j * 2] = 1;
	//			else gauss_matrix[i * 2][j * 2] = 0;
	//		}
	//	}
	//}
	template<typename T, typename P>
	matrix<T> operator*(const matrix<T>& mrx, const P& value) {
		matrix<T> result{ mrx };
		for (typename IMD::matrix<T>::size_type i{ 0 }; i < result.rows(); ++i) {
			for (typename IMD::matrix<T>::size_type j{ 0 }; j < result.cols(); ++j)
				result(i, j) *= value;
		}
		return result;
	}
	template<typename T, typename P>
	matrix<T> operator*(const P& value, const matrix<T>& mrx) {
		return mrx * value;
	}
	template<typename T, typename P>
	matrix<T>& operator/(matrix<T>& mrx, const P& value) {
		return mrx * 1 / value;
	}
	template<typename T, typename P>
	matrix<T>& operator/(const P& value, matrix<T>& mrx) {
		return mrx * 1 / value;
	}
	template<typename T, typename P>
	matrix<T>& operator*=(matrix<T>& mrx, const P& value) {
		for (typename IMD::matrix<T>::size_type i{ 0 }; i < mrx.rows(); ++i) {
			for (typename IMD::matrix<T>::size_type j{ 0 }; j < mrx.cols(); ++j)
				mrx(i, j) *= value;
		}
		return mrx;
	}
	template<typename T, typename P>
	matrix<T>& operator/=(matrix<T>& mrx, const P& value) {
		mrx *= 1 / value;
		return mrx;
	}
}




#endif
