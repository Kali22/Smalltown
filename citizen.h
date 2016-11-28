#ifndef CITIZEN_H
#define CITIZEN_H

#include <type_traits>
#include <cassert>

template <typename T, int minAge, int maxAge, bool canAttack,
	typename = typename std::enable_if_t<std::is_arithmetic<T>::value, T>>
class Citizen {
	
private:
	
	T health, age, attackPower;

	void checkAssert(T h, T a) {
		assert (h > 0);
		assert (minAge <= a && a <= maxAge);
	}

public:
	
	template <typename U = T, 
				typename = typename std::enable_if_t<!canAttack, U>>
	Citizen(T h, T a) : health(h), age(a) {
		checkAssert(h, a);
	}
	
	template <typename U = T, 
				typename = typename std::enable_if_t<canAttack, U>>
	Citizen(T h, T a, T ap) : health(h), age(a), attackPower(ap) {
		checkAssert(h, a);
	}
	
	T getHealth() const { return health; }
	T getAge() const { return age; }
	
	void takeDamage(T damage) {
		health = max(health - damage, 0);
	}
	
	template <typename U = T, 
				typename = typename std::enable_if_t<canAttack, U>>
	T getAttackPower() const { return attackPower; }
	
	using valueType = T;
	
};

template <typename T>
using Teenager = Citizen<T, 11, 17, false>;

template <typename T>
using Adult = Citizen<T, 18, 100, false>;

template <typename T>
using Sheriff = Citizen<T, 18, 100, true>;

#endif //CITIZEN_H