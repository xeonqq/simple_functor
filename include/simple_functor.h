#ifndef SIMPLE_FUNCTOR_H
#define SIMPLE_FUNCTOR_H

#include <memory>
#include <type_traits>

template <typename Signature>
class simple_functor;

template <typename ReturnType, typename... Args>
class simple_functor<ReturnType(Args...)>
{
    template <typename U>
    using is_same_as_simple_functor = std::is_same<simple_functor, std::decay_t<U>>;

    struct Concept
    {
        virtual ReturnType Invoke(Args... args)        = 0;
        virtual std::unique_ptr<Concept> Clone() const = 0;
        virtual ~Concept()                             = default;
    };

    template <typename T>
    struct Model : public Concept
    {
        using function_type                                  = T;
        static constexpr bool is_callable_copy_constructable = std::is_copy_constructible<T>::value;

        template <typename U>
        Model(U &&model) : model_(std::forward<U>(model))
        {}

        ReturnType Invoke(Args... args) override { return model_(args...); }

        std::unique_ptr<Concept> Clone() const override
        {
            return Clone(std::integral_constant<bool, is_callable_copy_constructable>());
        };

        std::unique_ptr<Concept> Clone(std::false_type) const
        {
            throw std::runtime_error("Can not copy move-only function");
        }

        std::unique_ptr<Concept> Clone(std::true_type) const
        {
            return std::make_unique<Model<T>>(model_);
        }

        T model_;
    };

  public:
    template <typename Function,
              typename = std::enable_if_t<!is_same_as_simple_functor<Function>::value>>
    simple_functor(Function &&function)
        : function_concept_{
              std::make_unique<Model<std::decay_t<Function>>>(std::forward<Function>(function))}
    {}

    simple_functor(const simple_functor &other)
        : function_concept_{other.function_concept_->Clone()}
    {}

    simple_functor(simple_functor &&other) : function_concept_{std::move(other.function_concept_)}
    {}
    ReturnType operator()(Args... args) { function_concept_->Invoke(std::forward<Args>(args)...); }

  private:
    std::unique_ptr<Concept> function_concept_;
};

#endif  // SIMPLE_FUNCTOR_H
