#include "StudentUndo.h"

Undo* createUndo()
{
	return new StudentUndo;
}

// add action to the record to be potentially undone later
void StudentUndo::submit(const Action action, int row, int col, char ch) {
	if (!changes.empty()) {
		operations* top = changes.top();
		if (top->action == action) {
			if (action == Action::DELETE) {
				if (row == top->row && (col == top->col || col == top->col - 1)) { // undo delete with previous deletes if at same spot or adjacent
					if (col == top->col) {
						top->row = row;
						top->col = col;
						top->str.append(1, ch);
					}
					else if (col == top->col - 1) {
						top->row = row;
						top->col = col;
						top->str.insert(0, 1, ch);
					}
					return;
				}
			}
			else if (action == Action::INSERT) {
				if (col == top->col + top->count && row == top->row) { // undo insertion with previous insertions if adjacent
					top->count++;
				}
				return;
			}
			else if (action == Action::ERROR) {
				return;
			}
		}
	}
	changes.push(new operations(action, row, col, ch));
}

// undo the last action on the stack and remove the action
StudentUndo::Action StudentUndo::get(int &row, int &col, int& count, std::string& text) {
	if (!changes.empty()) {
		operations* top = changes.top();
		row = top->row;
		col = top->col;
		count = 1;
		text = "";
		if (top->action == Action::INSERT) {
			count = top->count;
			col--;
			delete top;
			changes.pop();
			return Action::DELETE;
		}
		if (top->action == Action::DELETE) {
			text = top->str;
			delete top;
			changes.pop();
			return Action::INSERT;
		}
		if (top->action == Action::JOIN) {
			delete top;
			changes.pop();
			return Action::SPLIT;
		}
		if (top->action == Action::SPLIT) {
			delete top;
			changes.pop();
			return Action::JOIN;
		}
	}
	else {
		return Action::ERROR;
	}
}

// remove all undo actions from the stack
void StudentUndo::clear() {
	while (!changes.empty()) {
		delete changes.top();
		changes.pop();
	}
}
