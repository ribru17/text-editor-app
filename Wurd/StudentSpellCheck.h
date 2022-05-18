#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>

class StudentSpellCheck : public SpellCheck {
public:
	StudentSpellCheck() { root = new Trie('.'); }
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
	struct Trie {
		Trie(char ch): children(27, nullptr) { val = ch; isEnd = false; }
		char val;
		std::vector<Trie*> children;
		bool isEnd;
	};
	Trie* root;
	void killTrie(Trie* root);
	void findSug(std::string word, int depth, bool oneoff, Trie* node, std::vector<std::string>& strvec, std::string curstr, bool& found);
};

#endif  // STUDENTSPELLCHECK_H_