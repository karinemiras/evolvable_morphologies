#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Hello

#include<string>
#include <boost/test/unit_test.hpp>

#include "Genome.h"



BOOST_AUTO_TEST_CASE(testgenomeget)
{
  std::vector<std::string> axiom;
  axiom.push_back("C");
  Genome gen(axiom);

  BOOST_CHECK(gen.getTo() == 3);
}