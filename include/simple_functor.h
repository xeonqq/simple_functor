#ifndef SIMPLE_FUNCTOR_H
#define SIMPLE_FUNCTOR_H

#include <memory>
#include <type_traits>

template <typename Signature>
class simple_functor;

template <typename ReturnType, typename... Args>
class simple_functor<ReturnType(Args...)>
{
    struct Concept
    {
        virtual ReturnType Invoke(Args... args) = 0;
    };
    template <typename T>
    struct Model : public Concept
    {
        Model(T &&model) : model_(std::forward<T>(model)) {}

        ReturnType Invoke(Args... args) override { return model_(args...); }
        T model_;
    };

  public:
    template <typename Function>
    simple_functor(Function &&function)
        : function_concept_{std::make_unique<Model<Function>>(std::forward<Function>(function))}
    {}

    ReturnType operator()(Args... args) { function_concept_->Invoke(std::forward<Args>(args)...); }

  private:
    std::unique_ptr<Concept> function_concept_;
};

#endif  // SIMPLE_FUNCTOR_H
