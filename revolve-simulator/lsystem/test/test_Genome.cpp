#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Hello

#include<string>
#include <boost/test/unit_test.hpp>

#include "Genome.h"



BOOST_AUTO_TEST_CASE(testgenomeget)
{

// test aboiut test
  BOOST_CHECK(gen.getTo() == 3);
}