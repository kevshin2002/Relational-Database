#include <expected>
#include <functional>
#include <unordered_map>

#ifndef AbstractFactory_hpp
#define AbstractFactory_hpp

namespace Creational {
    template <typename FactoryKeyType, typename FactoryValue>
    class AbstractFactory {
    public:
        using Constructor = std::function<FactoryValue* (FactoryValueState)>;
        template <FactoryKeyType Key>
        AbstractFactory& add(Constructor aConstructor) {
            factory[Key] = aConstructor;
            return *this;
        }

        template <FactoryKeyType Key>
        FactoryValue* create() {
            auto it = factory.find(Key);
            if (it != factory.end()) {
                return it->second();
            }
            return nullptr;
        }
    protected:

    private:
        std::unordered_map<FactoryKeyType, Constructor> factory;
    };

}

#endif // AbstractFactory_hpp
