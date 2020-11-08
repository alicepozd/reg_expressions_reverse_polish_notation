#include <iostream>
#include <stack>
#include <string>
#include <set>

class RegExprInfo {
public:
    RegExprInfo(char exp, char letter) {
        if (letter == exp) {
            possible_remainder.insert(1);
        } else {
            possible_remainder.insert(0);
        }
    }

    RegExprInfo(std::set<int> reminders_set) {
        possible_remainder = reminders_set;
    }

    bool has_reminder_0() const {
        return possible_remainder.end() != possible_remainder.find(0);
    }

    const std::set<int>& get_set() const {
        return possible_remainder;
    }

    ~RegExprInfo() = default;
private:
    std::set<int> possible_remainder;
};


int NOD(int first, int second) {
    while (second) {
        first = first % second;
        std::swap(first, second);
    }
    return first;
}


RegExprInfo operator_plus(const RegExprInfo& first, const RegExprInfo& second) {
    std::set<int> new_set(first.get_set());
    const std::set<int>& second_set(second.get_set());
    for (int elem : second_set) {
        new_set.insert(elem);
    }
    RegExprInfo new_exp_info(new_set);
    return new_exp_info;
}


RegExprInfo operator_multiplication(const RegExprInfo& first, const RegExprInfo& second, int k) {
    std::set<int> new_set;
    const std::set<int>& first_set(first.get_set());
    const std::set<int>& second_set(second.get_set());
    for (int first_elem : first_set) {
        for (int second_elem : second_set) {
            new_set.insert((first_elem + second_elem) % k);
        }
    }
    RegExprInfo new_exp_info(new_set);
    return new_exp_info;
}


RegExprInfo operator_star(const RegExprInfo& exp_info, int k) {
    std::set<int> new_set;
    const std::set<int>& exp_set(exp_info.get_set());
    int nod = -1;
    for (int elem : exp_set) {
        if (elem == 0) {
            continue;
        }
        if (nod == -1) {
            nod = elem;
        } else {
            nod = NOD(nod, elem);
        }
    }
    int new_elem = nod;
    bool begin = true;
    while (begin | (new_elem != nod)) {
        begin = false;
        new_set.insert(new_elem);
        new_elem = (new_elem + nod) % k;
    }
    RegExprInfo new_exp_info(new_set);
    return new_exp_info;
}


bool plus_process(std::stack<RegExprInfo>& st, std::string string_ans) {
    if (st.empty()) {
        string_ans = "ERROR: not correct regular expression";
        return false;
    }
    RegExprInfo first = st.top();
    st.pop();
    if (st.empty()) {
        string_ans = "ERROR: not correct regular expression";
        return false;
    }
    RegExprInfo second = st.top();
    st.pop();
    st.push(std::move(operator_plus(first, second)));
    return true;
}


bool multiplication_process(std::stack<RegExprInfo>& st, std::string string_ans, int k) {
    if (st.empty()) {
        string_ans = "ERROR: not correct regular expression";
        return false;
    }
    RegExprInfo first = st.top();
    st.pop();
    if (st.empty()) {
        string_ans = "ERROR: not correct regular expression";
        return false;
    }
    RegExprInfo second = st.top();
    st.pop();
    st.push(std::move(operator_multiplication(first, second, k)));
    return true;
}


bool star_process(std::stack<RegExprInfo>& st, std::string string_ans, int k) {
    if (st.empty()) {
        string_ans = "ERROR: not correct regular expression";
        return false;
    }
    RegExprInfo exp = st.top();
    st.pop();
    st.push(std::move(operator_star(exp, k)));
    return true;
}


void end_expr_process(std::stack<RegExprInfo>& st, std::string& string_ans) {
    if (st.empty()) {
        string_ans = "ERROR: not correct regular expression";
    }
    if (string_ans.empty()) {
        if (st.top().has_reminder_0()) {
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
    std::stack<RegExprInfo> st;
    std::string string_ans;
    for (int i = 0; i < reg_expr.size(); ++i) {
        if (reg_expr[i] == 'a' | reg_expr[i] == 'b' | reg_expr[i] == 'c' | reg_expr[i] == '1') {
            st.push(RegExprInfo(reg_expr[i], letter));
        } else if (reg_expr[i] == '+'){
            if (!plus_process(st, string_ans)) {
                break;
            }
        } else if (reg_expr[i] == '.') {
            if (!multiplication_process(st, string_ans, k)) {
                break;
            }
        } else if (reg_expr[i] == '*') {
            if (!star_process(st, string_ans, k)) {
                break;
            }
        } else {
            string_ans = "ERROR: not correct regular expression";
            break;
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
