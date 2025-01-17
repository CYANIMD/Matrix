#ifndef __ENTITY_
#define __ENTITY_

#include <iostream>

namespace IMD {
	class entity {
	public:
		//Обычный конструктор.
		entity() {
			std::cout << "Entity is created" << std::endl;
		}
		//Конструктор копии.
		entity(const entity& other) {
			std::cout << "Entity is copied" << std::endl;
		}
		//Move-конструктор копии.
		entity(entity&& other) noexcept {
			std::cout << "Entity is move copied" << std::endl;
		}
		//Оператор копирующего присваивания.
		entity& operator=(const entity& other) {
			if (this != &other) {
				std::cout << "Entity is assigned" << std::endl;
			}
			return *this;
		}
		//Move-оператор копирующего присваивания.
		entity& operator=(entity&& other) noexcept {
			std::cout << "Entity is move assigned" << std::endl;
			return *this;
		}
		//Деструктор.
		~entity() {
			std::cout << "Entity is destroyed" << std::endl;
		}
	};
}

std::ostream& operator<<(std::ostream& os, const IMD::entity& other) {
	os << "Entity is consoled";
	return os;
}


#endif // !__ENTITY_
