this program is an implementation of the SpellChecker algorithm using Trie data strucure.

for every word not found in the dictionary, the closed word is calculated using Levenshtein Distance method

with the threads version, two threads are searching a vector of input file strings in parallel. computation time is reduced by half

if only alphanumeric characters are used, memory can be optimized by setting the search buffer to 26 bytes only

