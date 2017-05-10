#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Hello

#include<string>
#include <boost/test/unit_test.hpp>

#include "Genome.h"



BOOST_AUTO_TEST_CASE(testgenomeget)
{
<<<<<<< HEAD

=======
  std::vector<std::string> axiom;
  axiom.push_back("C");
//  Genome gen(axiom);
>>>>>>> 217c7f3... Added Alphabet singleton as part of restructuring.

  BOOST_CHECK(3 == 3);
}