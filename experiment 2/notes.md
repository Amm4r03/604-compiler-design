## nondeterministic finite automata machine

NFA has a defined set of states
The transitions between states are not necessarily well defined
transitions may occur also with no input - that leads to non determinism

tuples associated with NFAs
1. set of states
2. input set of characters 
3. set of transitions (initial state, character consumed, final state)
4. start state
5. set of accepting states

---

### flow of program
1. user specifies the number of states
2. user names the states
3. ask the user for the input set of characters
4. ask the user to specify the start state
5. ask the user to specify the set of accepting states
6. for each state and each character, ask the user to provide a set of next possible states
7. store all transitions
8. ask the user to give an input test string
9. simulate NFA on the input string
