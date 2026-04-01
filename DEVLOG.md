# DEVLOG

## 2026-03-31

### Session 1
- Set up project structure with `main.cpp` and `ArrayStack.h`
- Reviewed assignment requirements
- Planned implementation order: ArrayStack → tokenizer → postfix validation/eval → infix validation → infix-to-postfix conversion
- Implemented `ArrayStack<T>` with `push`, `pop`, `top`, `empty`, and `size` using a `std::vector` internally
- Added exception throwing on `pop` and `top` when stack is empty

### Session 2
- Implemented `tokenize`: scans input left to right, groups multi-digit integers, picks up operators (+, -, *, /) and parentheses, skips whitespace
- Implemented `precedence`: returns 2 for `*`/`/`, 1 for `+`/`-`
- Tested tokenizer manually with several inputs

### Session 3
- Implemented `isValidPostfix`: uses a depth counter — numbers increment it, operators require depth ≥ 2 and decrement it, parentheses immediately return false; valid if depth ends at exactly 1
- Implemented `evalPostfix`: uses `ArrayStack<double>` to push numbers, pop two operands per operator, push result
- Tested with `3 4 2 * +` → 11 (correct)

### Session 4
- Implemented `isValidInfix`: tracks a `prev` state machine to enforce valid token ordering (operators can't follow operators, numbers can't follow numbers, etc.)
- Added parenthesis balance checking with a depth counter
- Tested edge cases: `3 + * 4` correctly returns NEITHER, `(3 + 4) * 2` correctly returns INFIX

### Session 5
- Implemented `infixToPostfix` using the Shunting-Yard algorithm
- Handles operator precedence (left-associative) and parentheses correctly
- Tested `3 + 4 * 2` → postfix `3 4 2 * +` (correct), `(3 + 4) * 2` → postfix `3 4 + 2 *` (correct)

### Session 6
- Compiled and ran all test cases — all pass
- Fixed trailing space in POSTFIX output line to match exact required format
- Final review of output formatting
- Pushed completed project to GitHub
