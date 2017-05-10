#include <random>

#include "Alphabet.h"

namespace lsystem {

Alphabet::Alphabet()
{
  this->alphabet_.reset(new AlphabetType());
  this->commands_.reset(new CommandsType());

  if(alphabet_->size() == 0 || commands_->size() == 0) {
    alphabet_.get()->push_back(Axiom::Instance().GetAxiom());
    alphabet_.get()->push_back("FixedBrick");
    alphabet_.get()->push_back("ParametricBarJoint");
    alphabet_.get()->push_back("ActiveJoint");
    alphabet_.get()->push_back("PassiveJoint");

    commands_.get()->push_back("return");
    commands_.get()->push_back("right");
    commands_.get()->push_back("left");
    commands_.get()->push_back("front");
  }

  std::sort(alphabet_.get()->begin(), alphabet_.get()->end());
  std::sort(commands_.get()->begin(), commands_.get()->end());

  std::unique(alphabet_.get()->begin(), alphabet_.get()->end());
  std::unique(commands_.get()->begin(), commands_.get()->end());
}

Alphabet::Alphabet(AlphabetPtr &alphabet, CommandsPtr &commands)
{
  std::sort(alphabet.get()->begin(), alphabet.get()->end());
  std::sort(commands.get()->begin(), commands.get()->end());

  std::unique(alphabet.get()->begin(), alphabet.get()->end());
  std::unique(commands.get()->begin(), commands.get()->end());

  this->alphabet_ = std::move(alphabet);
  this->commands_ = std::move(commands);
}

std::string Alphabet::Command(const size_t index) const
{
  assert(this->commands_->size() > index);

  return this->commands_->at(index);
}

std::string Alphabet::RandomCommand() const
{
  assert(this->commands_->size() > 0);

  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<size_t> dist(0, this->commands_->size());

  return this->commands_->at(dist(mt));
}

std::string Alphabet::Letter(const size_t index) const
{
  assert(this->alphabet_->size() > index);

  return this->alphabet_->at(index);
}

std::string Alphabet::RandomLetter() const
{
  assert(this->alphabet_->size() > 0);

  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<size_t> dist(0, this->alphabet_->size());

  return this->alphabet_->at(dist(mt));
}

}