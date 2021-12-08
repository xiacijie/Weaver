#include "../automata/DFA.h"
namespace weaver {

    class Thread {
    public:
        Thread(uint16_t id) : _id(id) {}

        Alphabet& getAlphabet() { return _alphabet; }
        DFA& getCFG() { return _cfg; }

        uint16_t getID() const { return _id; }
    private:
        uint16_t _id;
        Alphabet _alphabet;
        DFA _cfg;
    };
}