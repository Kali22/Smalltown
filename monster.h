//Batosz Burny, Maciej Gontar
#ifndef MONSTER_H
#define MONSTER_H

#include <string>
#include <typeinfo>
#include <algorithm>
#include <type_traits>
#include <cassert>
#include "citizen.h"

//enum for monster types
enum MonstType {
    mummy, vampire, zombie
};

//template for all kinds of monsters
template<typename T, enum MonstType,
        typename = std::enable_if_t<std::is_arithmetic<T>::value, T>>
class Monster {

private:
    
    T health, attackPower;

public:
    
    //constructor
    Monster(T h, T ap) : health(h), attackPower(ap) {
        assert(h >= 0);
        assert(ap >= 0);
    }
    
    //getters
    T getHealth() const { return health; }
    
    T getAttackPower() const { return attackPower; }
    
    void takeDamage(T damage) {
		assert(damage >= 0);
        health -= std::min(health, damage);
    }
    
};

//aliases for templates of every type of monster
template<typename T>
using Mummy = Monster<T, mummy>;

template<typename T>
using Vampire = Monster<T, vampire>;

template<typename T>
using Zombie = Monster<T, zombie>;

//overloaded version of attack function, with attacking a sheriff as a separate case
template<typename M, typename V, typename =
std::enable_if_t<!std::is_same<V, Sheriff<typename V::valueType>>::value>>
void attack(const M &monster, V &victim) {
    victim.takeDamage(monster.getAttackPower());
}

template<typename M, typename V, typename =
std::enable_if_t<std::is_same<V, Sheriff<typename V::valueType>>::value>>
void attack(M &monster, V &victim) {
    monster.takeDamage(victim.getAttackPower());
    victim.takeDamage(monster.getAttackPower());
}

#endif //MONSTER_H