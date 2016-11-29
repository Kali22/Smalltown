#ifndef CITIZEN_H
#define CITIZEN_H

#include <type_traits>
#include <cassert>
#include <algorithm>

template <typename T, int minAge, int maxAge, bool canAttack,
	typename = typename std::enable_if_t<std::is_arithmetic<T>::value, T>>
class Citizen {
	
private:
	
	T health, age, attackPower;

	const void checkAssert(T h, T a) {
		assert(h > 0);//, "Negative health"); //TODO check
		assert(minAge <= a && a <= maxAge);//, "Wrong age"); 
	}

public:
	
	template <typename U = T, 
				typename = typename std::enable_if_t<!canAttack, U>>
	Citizen(T h, T a) : health(h), age(a) {
		checkAssert(h, a);
		//static_assert (minAge <= a && a <= maxAge, "Wrong age"); 
	}
	
	template <typename U = T, 
				typename = typename std::enable_if_t<canAttack, U>>
	Citizen(T h, T a, T ap) : health(h), age(a), attackPower(ap) {
		checkAssert(h, a);
		//static_assert (minAge <= a && a <= maxAge, "Wrong age"); 
	}
	
	T getHealth() const { return health; }
	T getAge() const { return age; }
	
	void takeDamage(T damage) {
		health -= std::min(health, damage);
	}
	
	template <typename U = T, 
				typename = typename std::enable_if_t<canAttack, U>>
				//TODO check why not
	//template <typename = std::enable_if_t<canAttack>>
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