#include <iostream>
#include <stack>
#include <string>
#include <set>
#include <algorithm>


enum Symbols {
    PLUS,
    MUL,
    STAR,
    LETTER
};


Symbols symbol_by_char(char letter) {
    if (letter == '+') {
        return PLUS;
    } else if (letter == '.') {
        return MUL;
    } else if (letter == '*') {
        return STAR;
    }
    return LETTER;
}


std::set<int> operator_plus(const std::set<int>& first, const std::set<int>& second) {
    std::set<int> new_set;
    for (int elem : second) {
        new_set.insert(elem);
    }
    for (int elem : first) {
        new_set.insert(elem);
    }
    return new_set;
}


std::set<int> operator_multiplication(const std::set<int>& first, const std::set<int>& second, int k) {
    std::set<int> new_set;
    for (int first_elem : first) {
        for (int second_elem : second) {
            new_set.insert((first_elem + second_elem) % k);
        }
    }
    std::set<int> new_exp_info(new_set);
    return new_exp_info;
}


std::set<int> operator_star(const std::set<int>& expression, int k) {
    std::set<int> new_set;
    int nod = 0;
    for (int elem : expression) {
        nod = std::__gcd(nod, elem);
    }
    int new_elem = nod;
    bool begin = true;
    while (begin | (new_elem != nod)) {
        begin = false;
        new_set.insert(new_elem);
        new_elem = (new_elem + nod) % k;
    }
    std::set<int> new_exp_info(new_set);
    return new_exp_info;
}


bool process_letter(std::stack<std::set<int>>& st, char expression, char letter) {
    std::set<int> new_set;
    if (letter == expression) {
        new_set.insert(1);
    } else {
        new_set.insert(0);
    }
    st.push(new_set);
}


bool process_plus(std::stack<std::set<int>>& st, std::string string_ans) {
    if (st.size() < 2) {
        string_ans = "ERROR: not correct regular expression";
        return false;
    }
    std::set<int> first = st.top();
    st.pop();
    std::set<int> second = st.top();
    st.pop();
    st.push(operator_plus(first, second));
    return true;
}


bool process_multiplication(std::stack<std::set<int>>& st, std::string string_ans, int k) {
    if (st.size() < 2) {
        string_ans = "ERROR: not correct regular expression";
        return false;
    }
    std::set<int> first = st.top();
    st.pop();
    std::set<int> second = st.top();
    st.pop();
    st.push(operator_multiplication(first, second, k));
    return true;
}


bool process_star(std::stack<std::set<int>>& st, std::string string_ans, int k) {
    if (st.empty()) {
        string_ans = "ERROR: not correct regular expression";
        return false;
    }
    std::set<int> exp = st.top();
    st.pop();
    st.push(operator_star(exp, k));
    return true;
}


void end_expr_process(std::stack<std::set<int>>& st, std::string& string_ans) {
    if (st.empty()) {
        string_ans = "ERROR: not correct regular expression";
    }
    if (string_ans.empty()) {
        if (st.top().end() != st.top().find(0)) {
            string_ans = "YES";
        } else {
            string_ans = "NO";
        }
        st.pop();
    }
    if (!st.empty()) {
        string_ans = "ERROR: not correct regular expression";
    }
}


std::string has_div_k_words(std::string reg_expr, char letter, int k) {
    std::stack<std::set<int>> st;
    std::string string_ans;
    for (int i = 0; i < reg_expr.size(); ++i) {
        std::cout << symbol_by_char(reg_expr[i]);
        switch (symbol_by_char(reg_expr[i])) {
            case LETTER:
                process_letter(st, reg_expr[i], letter);
            case PLUS:
                if (!process_plus(st, string_ans)) {
                    break;
                }
            case MUL:
                if (!process_multiplication(st, string_ans, k)) {
                    break;
                }
            case STAR:
                std::cout << "!";
                if (!process_star(st, string_ans, k)) {
                    break;
                }
        }
    }
    end_expr_process(st, string_ans);
    return string_ans;
}


int main() {
    std::string str;
    int k;
    char letter;
    std::cin >> str >> letter >> k;
    std::cout << has_div_k_words(str, letter, k);
    return 0;
}
