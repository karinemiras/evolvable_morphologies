#ifndef LSYSTEM_INCLUDE_AXIOM_H_
#define LSYSTEM_INCLUDE_AXIOM_H_

#include <string>

namespace lsystem {

class Axiom {
    /// \brief The method that assures there will be only one unique instance
    /// of the class.
    /// \return A reference to the class instance.
    public: static Axiom &Instance() {
      static Axiom *axiomPtr = new Axiom("C");
      return *axiomPtr;
    }

    /// \brief The default getter for the only attribute.
    /// \return A string value containing axiom.
    public: std::string GetAxiom() const {
      return axiom_;
    }

    /// \brief The constructor is private to assure uniqueness.
    private: Axiom()
    {}

    /// \brief The constructor that is used to instantiate a unique instance
    /// of the class.
    private: Axiom(std::string axiom) {
      this->axiom_ = axiom;
    }

    /// \brief The copy constructor is private to prevent accidental copy of
    /// the only instance.
    private: Axiom(const Axiom &axiom);

    /// \brief The assignment operator is dissaloved.
    private: const Axiom &operator=(const Axiom &axiom);

    /// \brief The destructor is private to prevent unintended deletion.
    private: ~Axiom()
    {}

    /// \brief Initial string seed from which complex genome is developed.
    private: std::string axiom_;
};

}

#endif // LSYSTEM_INCLUDE_AXIOM_H_
