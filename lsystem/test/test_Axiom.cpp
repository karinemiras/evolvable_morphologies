#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Axiom

#include<string>

#include <boost/test/unit_test.hpp>

#include "Axiom.h"

BOOST_AUTO_TEST_CASE(testAxiomUniqueness)
{
  lsystem::Axiom &axiom1 = lsystem::Axiom::Instance();
  lsystem::Axiom &axiom2 = lsystem::Axiom::Instance();

  BOOST_CHECK_EQUAL(&axiom1, &axiom2);
}

BOOST_AUTO_TEST_CASE(testAxiomValue)
{
  lsystem::Axiom &axiom = lsystem::Axiom::Instance();

  BOOST_CHECK_EQUAL(axiom.GetAxiom(), "C");
}

BOOST_AUTO_TEST_CASE(testAxiomDifferentValue)
{
  lsystem::Axiom &axiom = lsystem::Axiom::Instance();

  BOOST_CHECK(axiom.GetAxiom() != "B");
}
