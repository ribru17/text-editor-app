#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include "Undo.h"
#include <stack>
#include <string>

class StudentUndo : public Undo {
public:

	void submit(Action action, int row, int col, char ch = 0);
	Action get(int& row, int& col, int& count, std::string& text);
	void clear();

private:
	struct operations {
		operations(const Action actionpar, int row, int col, char ch) : row(row), col(col), count(1) { action = actionpar; str = ""; str.append(1, ch); }
		Action action;
		int row;
		int col;
		std::string str;
		int count;
	};
	std::stack<operations*> changes;
};

#endif // STUDENTUNDO_H_
