#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Alphabet

#include <map>
#include <string>

#include <boost/test/unit_test.hpp>

#include "Alphabet.h"

BOOST_AUTO_TEST_CASE(testAlphabetUniqueness)
{
  lsystem::Alphabet &alphabet1 = lsystem::Alphabet::Instance();
  lsystem::Alphabet &alphabet2 = lsystem::Alphabet::Instance();

  BOOST_CHECK_EQUAL(&alphabet1, &alphabet2);
}

BOOST_AUTO_TEST_CASE(testAlphabetLetter)
{
  lsystem::Alphabet &alphabet = lsystem::Alphabet::Instance();

  std::string letter = alphabet.Letter(0);

  BOOST_CHECK_EQUAL(letter, "ActiveJoint");
}

BOOST_AUTO_TEST_CASE(testAlphabetCommand)
{
  lsystem::Alphabet &alphabet = lsystem::Alphabet::Instance();

  std::string command = alphabet.Command(0);

  BOOST_CHECK_EQUAL(command, "front");
}

BOOST_AUTO_TEST_CASE(testAlphabetRandomLetter)
{
// TODO: Test descrete uniform distribution
//  lsystem::Alphabet &alphabet = lsystem::Alphabet::Instance();
//
//  std::map<std::string, size_t> counts;
//  for (size_t i = 0; i < 1000; ++i) {
//    std::string random_letter = alphabet.RandomLetter();
//    auto element = counts.find(random_letter);
//    if (element != counts.end()) {
//      element->second = element->second + 1;
//      counts.emplace(element->first, element->second);
//    } else {
//      counts.emplace({random_letter, 0});
//    }
//  }
//
//  size_t a = counts.begin()->second;
//  size_t b = counts.rbegin()->second;
//
//  float frequency = 1/(b - a + 1);
//
//  BOOST_CHECK_CLOSE(frequency, a, 0.1);
}
