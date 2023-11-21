#include "validator.h"

#include <variant>
#include <vector>

#include "token.h"

namespace sql {

bool SqlValidator::is_valid() const {
    // TODO: Implement this

    return std::holds_alternative<state::Valid>(state_);
        //<T>(v) can query whether variant type v stores data of type T.
}

void SqlValidator::handle(const Token &token) {
    state_ = std::visit([this, &token](auto &&state){
        return transition(state, token);
    }, state_);
    // TODO: Implement this
}

struct TransitionFromStartVisitor {
    // Only the `Select` token advances the FSM from the `Start` state
    // TODO: Correct the behaviour
    State operator()(const token::Select &) const { return state::SelectStmt{}; }
    template <typename T>
    State operator()(const T &) const { return state::Invalid{}; }
};

State transition(state::Start, const Token &token) {
    return std::visit(TransitionFromStartVisitor{}, token.value());
}

struct TransitionFromSelectStmtVisitor {
    State operator()(const token::Asterisks &) const { return state::AllColumns{}; }
    State operator()(const token::Identifier &) const { return state::NamedColumn{}; }
    template <typename T>
    State operator()(const T &) const { return state::Invalid{}; }
};
State transition(state::SelectStmt, const Token &token) {
    return std::visit(TransitionFromSelectStmtVisitor{}, token.value());
}

struct TransitionFromAllColumnsVisitor {
    State operator()(const token::From &) const { return state::FromClause{}; }
    template <typename T>
    State operator()(const T &) const { return state::Invalid{}; }
};
State transition(state::AllColumns, const Token &token) {
    return std::visit(TransitionFromAllColumnsVisitor{}, token.value());
}

struct TransitionFromNamedColumnVisitor {
    State operator()(const token::Comma &) const { return state::MoreColumns{}; }
    State operator()(const token::From &) const { return state::FromClause{}; }
    template <typename T>
    State operator()(const T &) const { return state::Invalid{}; }
};
State transition(state::NamedColumn, const Token &token) {
    return std::visit(TransitionFromNamedColumnVisitor{}, token.value());
}

struct TransitionFromMoreColumnsVisitor {
    State operator()(const token::Identifier &) const { return state::NamedColumn{}; }
    template <typename T>
    State operator()(const T &) const { return state::Invalid{}; }
};
State transition(state::MoreColumns, const Token &token) {
    return std::visit(TransitionFromMoreColumnsVisitor{}, token.value());
}

struct TransitionFromFromClauseVisitor {
    State operator()(const token::Identifier &) const { return state::TableName{}; }
    template <typename T>
    State operator()(const T &) const { return state::Invalid{}; }
};
State transition(state::FromClause, const Token &token) {
    return std::visit(TransitionFromFromClauseVisitor{}, token.value());
}

struct TransitionFromTableNameVisitor {
    State operator()(const token::Semicolon &) const { return state::Valid{}; }
    State operator()(const token::Identifier &) const { return state::Invalid{}; }
    State operator()(const token::Comma &) const { return state::Invalid{}; }
    State operator()(const token::From &) const { return state::Invalid{};}
    State operator()(const token::Select &) const { return state::Invalid{};}
    State operator()(const token::Asterisks &) const { return state::Invalid{}; }
    //template <typename T>
    //State operator()(const T &) const { return state::Invalid{}; }
};
State transition(state::TableName, const Token &token) {
    return std::visit(TransitionFromTableNameVisitor{}, token.value());
}

State transition(state::Valid, const Token &token) {
    // TODO: Implement
    if (std::holds_alternative<token::Semicolon>(token.value())) {
        return state::Valid{};
    }
    return state::Invalid{};
}

State transition(state::Invalid, const Token &token) {
    // TODO: Implement

    return state::Invalid{};
}

bool is_valid_sql_query(const std::vector<Token> &tokens){
    SqlValidator validator;

    for (const auto &token : tokens) {
        validator.handle(token);

        if (std::holds_alternative<state::Invalid>(validator.get_state())) {
            return false;
        }
    }

    return std::holds_alternative<state::Valid>(validator.get_state());
};
} // namespace sql
