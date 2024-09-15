#pragma once
#include "nested-struct.hh"
#include <boost/describe.hpp>
#include <boost/mp11.hpp>

namespace schema {

BOOST_DESCRIBE_ENUM(ElementBase::Type, Level1Element, Level2Element,
                    Level3ElementVariant1, Level3ElementVariant2,
                    Level3ElementVariant3)
BOOST_DESCRIBE_STRUCT(LocallyIdentified, (), (localID))
BOOST_DESCRIBE_STRUCT(Identified, (), (ID))
BOOST_DESCRIBE_STRUCT(ElementBase, (LocallyIdentified), (elementType, localID))

BOOST_DESCRIBE_ENUM(DescriptionVariablePartBase::Type,
                    DescriptionVariablePartVariant1,
                    DescriptionVariablePartVariant2)
BOOST_DESCRIBE_STRUCT(DescriptionVariablePartBase, (),
                      (descriptionVariablePartType))

BOOST_DESCRIBE_STRUCT(
    TypedDescriptionVariablePart<DescriptionVariablePartVariant1>,
    (DescriptionVariablePartBase), ())
BOOST_DESCRIBE_STRUCT(
    DescriptionVariablePartVariant1,
    (TypedDescriptionVariablePart<DescriptionVariablePartVariant1>), (path))

BOOST_DESCRIBE_STRUCT(
    TypedDescriptionVariablePart<DescriptionVariablePartVariant2>,
    (DescriptionVariablePartBase), ())
BOOST_DESCRIBE_STRUCT(
    DescriptionVariablePartVariant2,
    (TypedDescriptionVariablePart<DescriptionVariablePartVariant2>),
    (path, channel))

BOOST_DESCRIBE_STRUCT(ContainsDescriptionVariablePart, (), (part))

BOOST_DESCRIBE_STRUCT(Level3ElementVariant1::Description,
                      (ContainsDescriptionVariablePart), (lineWidth))
BOOST_DESCRIBE_STRUCT(TypedElement<Level3ElementVariant1>, (ElementBase), ())
BOOST_DESCRIBE_STRUCT(Level3ElementVariant1,
                      (TypedElement<Level3ElementVariant1>, Identified),
                      (description))

BOOST_DESCRIBE_STRUCT(Level3ElementVariant2::Description,
                      (ContainsDescriptionVariablePart), (channel))
BOOST_DESCRIBE_STRUCT(TypedElement<Level3ElementVariant2>, (ElementBase), ())
BOOST_DESCRIBE_STRUCT(Level3ElementVariant2,
                      (TypedElement<Level3ElementVariant2>, Identified),
                      (description))

BOOST_DESCRIBE_STRUCT(Level3ElementVariant3::Description, (), (palette))
BOOST_DESCRIBE_STRUCT(TypedElement<Level3ElementVariant3>, (ElementBase), ())
BOOST_DESCRIBE_STRUCT(Level3ElementVariant3,
                      (TypedElement<Level3ElementVariant3>, Identified),
                      (description))

BOOST_DESCRIBE_STRUCT(Level2Element::Description, (), (widthMM))
BOOST_DESCRIBE_STRUCT(Level2Element::Structure, (), (Level3Elements))
BOOST_DESCRIBE_STRUCT(TypedElement<Level2Element>, (ElementBase), ())
BOOST_DESCRIBE_STRUCT(Level2Element, (TypedElement<Level2Element>),
                      (description, structure))

BOOST_DESCRIBE_STRUCT(Level1Element::Description, (), (caption))
BOOST_DESCRIBE_STRUCT(Level1Element::Structure, (), (level2Elements))
BOOST_DESCRIBE_STRUCT(TypedElement<Level1Element>, (ElementBase), ())
BOOST_DESCRIBE_STRUCT(Level1Element, (TypedElement<Level1Element>),
                      (description, structure))
} // namespace schema