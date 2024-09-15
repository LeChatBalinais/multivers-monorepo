#include "sandbox-describe/nested-struct-refl.hh"
#include "sandbox-describe/nested-struct-serialization.hh"
#include "sandbox-describe/nested-struct.hh"
#include "sandbox-describe/test.hh"
#include <cmath>
#include <fstream>
#include <iostream>
#include <variant>
#include <vector>

int main() {

  using namespace schema;

  Level3ElementVariant1 Level3ElementVariant1{
      .description = Level3ElementVariant1::Description{
          {DescriptionVariablePartVariant1{
              .path = std::vector<std::string>{"fd", "re"}}},
          std::nan(""),
      }};

  Level3ElementVariant2 Level3ElementVariant2{
      .description = Level3ElementVariant2::Description{
          {DescriptionVariablePartVariant2{
              {}, std::vector<std::string>{"fd", "re"}, 94}},
          9}};

  Level2Element Level2Element{
      .description = Level2Element::Description{.widthMM = 44.},
      .structure =
          Level2Element::Structure{{Level3Element(Level3ElementVariant1),
                                    Level3Element(Level3ElementVariant2)}}};

  {
    std::ofstream myfile;
    myfile.open("app-output/example.json");
    marshall(myfile, Level2Element);
    myfile.close();
  }
  MaybeElement el;
  {
    std::ifstream myfile;
    myfile.open("app-output/example.json");
    el = parse(myfile);
    myfile.close();
  }

  if (!schm::holds_monostate(el)) {
    std::ofstream myfile;
    myfile.open("app-output/example1.json");
    marshall(myfile, schm::fromMaybeVariant(el));
    myfile.close();
  }

  std::cout
      << std::get<schema::Level2Element>(el).structure->Level3Elements.size()
      << std::endl;
}
