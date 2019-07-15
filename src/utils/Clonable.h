#ifndef CLONABLE_H
#define CLONABLE_H

#include <memory>

namespace utils {

template <typename Base, typename Derived = Base>
class Clonable : public Base {
    public:
        using ClonableBase = Clonable;

        using Base::Base;

        friend std::unique_ptr<Derived> clone(const Clonable &c) {
            return std::unique_ptr<Derived>(static_cast<Derived*>(c.clone()));
        }
    
    private:
        virtual Base *clone() const override
        {
            return new Derived(static_cast<const Derived&>(*this));
        }
};

template <typename Base>
class Clonable<Base, Base> {
    public:
        friend std::unique_ptr<Base> clone(const Clonable &c) {
            return std::unique_ptr<Base>(static_cast<Base*>(c.clone()));
        }
    
    private:
        virtual Base *clone() const = 0;
};

}

#endif
