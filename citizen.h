//Batosz Burny, Maciej Gontar
#ifndef CITIZEN_H
#define CITIZEN_H

#include <type_traits>
#include <cassert>
#include <algorithm>

//template for citizens, with boundaries of allowed ages and ability to attack a monster
template<typename T, int minAge, int maxAge, bool canAttack,
        typename = std::enable_if_t<std::is_arithmetic<T>::value, T>>
class Citizen {

private:
    
    T health, age, attackPower;
    
    void checkAsserts(T h, T a) const {
        assert(h >= 0);
        assert(minAge <= a && a <= maxAge);
    }

public:
    
    //overloaded constructor for adult & teen with a second version for the sheriff
    template<typename U = T,
            typename = std::enable_if_t<!canAttack, U>>
    Citizen(T h, T a) : health(h), age(a) {
        checkAsserts(h, a);
    }
    
    template<typename U = T,
            typename = std::enable_if_t<canAttack, U>>
    Citizen(T h, T a, T ap) : health(h), age(a), attackPower(ap) {
        checkAsserts(h, a);
    }
    
    //getters
    T getHealth() const { return health; }
    
    T getAge() const { return age; }
    
    template<typename U = T,
            typename = std::enable_if_t<canAttack, U>>
    T getAttackPower() const { return attackPower; }
    
    void takeDamage(T damage) {
        health -= std::min(health, damage);
    }
    
    using valueType = T;
    
};

//aliases for templates of every type of citizen
template<typename T>
using Teenager = Citizen<T, 11, 17, false>;

template<typename T>
using Adult = Citizen<T, 18, 100, false>;

template<typename T>
using Sheriff = Citizen<T, 18, 100, true>;

#endif //CITIZEN_H