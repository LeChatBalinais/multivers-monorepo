#pragma once

#include "type-tagging.hh"
#include <optional>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

namespace schema {

template <typename T> using Maybe = std::optional<T>;

template <typename... T> using Variant = std::variant<T...>;

using ID_t = std::string;

struct LocallyIdentified {
  Maybe<ID_t> localID;
};

struct Identified {
  Maybe<ID_t> ID;
};

struct ElementBase : public LocallyIdentified {
  enum class Type {
    Level1Element,
    Level2Element,
    Level3ElementVariant1,
    Level3ElementVariant2,
    Level3ElementVariant3
  };
  ElementBase() {}
  ElementBase(ElementBase::Type type) : elementType{type} {}

  Maybe<Type> elementType;
  Type et;
};

struct Level1Element;
struct Level2Element;
struct Level3ElementVariant1;
struct Level3ElementVariant2;
struct Level3ElementVariant3;

using elem_tagging =
    schm::tagging<ElementBase, ElementBase::Type, &ElementBase::elementType>;
using elem_tags = elem_tagging::tag_mapping<
    elem_tagging::tag<ElementBase::Type::Level1Element, Level1Element>,
    elem_tagging::tag<ElementBase::Type::Level2Element, Level2Element>,
    elem_tagging::tag<ElementBase::Type::Level3ElementVariant1,
                      Level3ElementVariant1>,
    elem_tagging::tag<ElementBase::Type::Level3ElementVariant2,
                      Level3ElementVariant2>,
    elem_tagging::tag<ElementBase::Type::Level3ElementVariant3,
                      Level3ElementVariant3>>;

using MaybeElement = elem_tags::MaybeVariant;
using Element = elem_tags::Variant;

template <typename T> struct TypedElement : public ElementBase {
  TypedElement() : ElementBase(elem_tags::enumValue<T>()) {}
};

struct DescriptionVariablePartBase {
  enum class Type {
    DescriptionVariablePartVariant1,
    DescriptionVariablePartVariant2
  };
  DescriptionVariablePartBase() {}
  DescriptionVariablePartBase(
      DescriptionVariablePartBase::Type descriptionVariablePartType)
      : descriptionVariablePartType{descriptionVariablePartType} {}
  Maybe<Type> descriptionVariablePartType;
};

struct DescriptionVariablePartVariant1;
struct DescriptionVariablePartVariant2;

using description_variable_part_tagging =
    schm::tagging<DescriptionVariablePartBase,
                  DescriptionVariablePartBase::Type,
                  &DescriptionVariablePartBase::descriptionVariablePartType>;
using description_variable_part_tags =
    description_variable_part_tagging::tag_mapping<
        description_variable_part_tagging::tag<
            DescriptionVariablePartBase::Type::DescriptionVariablePartVariant1,
            DescriptionVariablePartVariant1>,
        description_variable_part_tagging::tag<
            DescriptionVariablePartBase::Type::DescriptionVariablePartVariant2,
            DescriptionVariablePartVariant2>>;

using DescriptionVariablePart = description_variable_part_tags::Variant;

template <typename T>
struct TypedDescriptionVariablePart : public DescriptionVariablePartBase {
  TypedDescriptionVariablePart()
      : DescriptionVariablePartBase(
            description_variable_part_tags::enumValue<T>()) {}
};

struct DescriptionVariablePartVariant1
    : TypedDescriptionVariablePart<DescriptionVariablePartVariant1> {
  std::vector<std::string> path;
};

struct DescriptionVariablePartVariant2
    : TypedDescriptionVariablePart<DescriptionVariablePartVariant2> {
  std::vector<std::string> path;
  int channel;
};

struct ContainsDescriptionVariablePart {
  DescriptionVariablePart part;
};

struct Level3ElementVariant1 : public TypedElement<Level3ElementVariant1>,
                               public Identified {
  struct Description : public ContainsDescriptionVariablePart {
    Maybe<double> lineWidth;
  };

  Maybe<Description> description;
};

struct Level3ElementVariant2 : public TypedElement<Level3ElementVariant2>,
                               public Identified {
  struct Description : public ContainsDescriptionVariablePart {
    Maybe<int> channel;
  };

  Maybe<Description> description;
};

struct Level3ElementVariant3 : public TypedElement<Level3ElementVariant3>,
                               public Identified {

  struct Description : public ContainsDescriptionVariablePart {
    Maybe<std::string> palette;
  };

  Maybe<Description> description;
};

using Level3Element = Variant<Level3ElementVariant1, Level3ElementVariant2,
                              Level3ElementVariant3>;

struct Level2Element : public TypedElement<Level2Element> {
  struct Description {
    Maybe<double> widthMM;
  };

  struct Structure {
    std::vector<Level3Element> Level3Elements;
  };
  Maybe<Description> description;
  Maybe<Structure> structure;
};

struct Level1Element : public TypedElement<Level1Element> {
  struct Description {
    Maybe<std::string> caption;
  };

  struct Structure {
    std::vector<Level2Element> level2Elements;
  };
  Maybe<Description> description;
  Maybe<Structure> structure;
};

} // namespace schema
