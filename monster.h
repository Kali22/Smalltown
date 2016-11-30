#ifndef MONSTER_H
#define MONSTER_H

#include <string>
#include <typeinfo>
#include <algorithm>
#include <type_traits>
#include <cassert>
#include "citizen.h"

enum MonstType { mummy, vampire, zombie };

template <typename T, enum MonstType,
	typename = typename std::enable_if_t<std::is_arithmetic<T>::value, T>>
class Monster {
	
private:
	
	T health, attackPower;
	std::string typeName;
	
public:
	
	Monster(T h, T ap) : health(h), attackPower(ap), typeName(typeid(T).name()) {
		assert(h > 0);
		assert(ap > 0);
	}
	
	T getHealth() const { return health; }
	T getAttackPower() const { return attackPower; }
	std::string getTypeName() const { return typeName; }
	
	void takeDamage(T damage) {
		health -= std::min(health, damage);
	}
	
};

template <typename T>
using Mummy = Monster<T, mummy>;

template <typename T>
using Vampire = Monster<T, vampire>;

template <typename T>
using Zombie = Monster<T, zombie>;


template<typename M, typename V, 
	typename = typename std::enable_if_t<!std::is_same<V, Sheriff<typename V::valueType>>::value>>
void attack(const M &monster, V &victim) {
	victim.takeDamage(monster.getAttackPower());
}

template<typename M, typename V, 
	typename = typename std::enable_if_t<std::is_same<V, Sheriff<typename V::valueType>>::value>>
void attack(M &monster, V &victim) {
	monster.takeDamage(victim.getAttackPower());
	victim.takeDamage(monster.getAttackPower());
}

#endif //MONSTER_H