#ifndef LSYSTEM_ALPHABET_H_
#define LSYSTEM_ALPHABET_H_

#include <assert.h>
#include <memory>
#include <string>
#include <vector>

#include "Axiom.h"

namespace lsystem {

class Alphabet {

    typedef std::vector<std::string> AlphabetType;
    typedef std::vector<std::string> CommandsType;
    typedef std::unique_ptr<AlphabetType> AlphabetPtr;
    typedef std::unique_ptr<CommandsType> CommandsPtr;

    /// \brief The method that assures there will be only one unique instance
    /// of the class.
    /// \return A reference to the class instance.
    public: static Alphabet &Instance() {
      static Alphabet * alphabetInstance = new Alphabet();
      return *alphabetInstance;
    }

    /// \brief The default getter for the command attribute.
    /// \return A string value containing alphabet.
    public: std::string Command(const size_t index) const;

    /// \brief A getter for a random command attribute.
    /// \return A string value containing alphabet.
    public: std::string RandomCommand() const;

    /// \brief The default getter for the letter attribute.
    /// \return A string value containing alphabet.
    public: std::string Letter(const size_t index) const;

    /// \brief A getter for a random letter attribute.
    /// \return A string value containing alphabet.
    public: std::string RandomLetter() const;

    /// \brief The constructor is private to assure uniqueness.
    private: Alphabet();

    /// \brief The constructor that is used to instantiate a unique instance
    /// of the class.
    private: Alphabet(AlphabetPtr & alphabet, CommandsPtr & commands);

    /// \brief The copy constructor is private to prevent accidental copy of
    /// the only instance.
    private: Alphabet(const Alphabet &alphabet);

    /// \brief The assignment operator is disallowed.
    private: const Alphabet &operator=(const Alphabet &alphabet);

    /// \brief The destructor is private to prevent unintended deletion.
    private: ~Alphabet()
    {}

    /// \brief List of possible elements from which the grammar is developed.
    private: AlphabetPtr alphabet_;

    /// \brief List of possible elements from which the grammar is developed.
    private: CommandsPtr commands_;
};

}

#endif // LSYSTEM_ALPHABET_H_
