//
//  stacks.cpp
//  CF.STL_Containers_Stack
//
//  Created by Alan Sampson on 4/10/21.
//
//  MARK: - Reference.
//  @see: https://en.cppreference.com/w/cpp/container/stack
//  #see: https://en.wikipedia.org/wiki/Brainfuck
//

#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>
#include <algorithm>
#include <numeric>
#include <compare>
#include <memory>
#include <type_traits>
#include <stack>
#include <deque>
#include <array>
#include <map>
#include <vector>
#include <stdexcept>
#include <cassert>
#include <cstddef>

using namespace std::literals::string_literals;
using namespace std::literals::string_view_literals;

//  MARK: - Definitions

//  MARK: - Local Constants.
//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
//  MARK: namespace konst
namespace konst {

auto delimiter(char const dc = '-', size_t sl = 80) -> std::string const {
  auto const dlm = std::string(sl, dc);
  return dlm;
}

static
auto const dlm = delimiter();

static
auto const dot = delimiter('.');

} /* namespace konst */

#if (__cplusplus > 201707L)
#endif  /* (__cplusplus > 201707L) */

//  MARK: - Function Prototype.
auto C_stack(int argc, char const * argv[]) -> decltype(argc);
auto C_stack_deduction_guides(int argc, char const * argv[]) -> decltype(argc);

//  MARK: - Implementation.
//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
/*
 *  MARK: main()
 */
int main(int argc, char const * argv[]) {
  std::cout << "CF.STL_Containers_Stack\n"sv;
  std::cout << "C++ Version: "s << __cplusplus << std::endl;

  std::cout << '\n' << konst::dlm << std::endl;
  C_stack(argc, argv);
  std::cout << '\n' << konst::dlm << std::endl;
  C_stack_deduction_guides(argc, argv);

  return 0;
}

//  MARK: - C_stack
//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
//  ================================================================================
//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
/*
 *  MARK: C_stack()
 */
auto C_stack(int argc, char const * argv[]) -> decltype(argc) {
  std::cout << "In "s << __func__ << std::endl;

  /// Member functions
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::stack - constructor"s << '\n';
  {
    std::stack<int> c1;
    c1.push(5);
    std::cout << c1.size() << '\n';

    std::stack<int> c2(c1);
    std::cout << c2.size() << '\n';

    std::deque<int> deq { 3, 1, 4, 1, 5 };
    std::stack<int> c3(deq);
    std::cout << c3.size() << '\n';

    std::cout << '\n';
  };

  /// Element access
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::stack - top"s << '\n';
  {
    std::stack<int> stk;

    stk.push( 2 );
    stk.push( 6 );
    stk.push( 51 );

    std::cout << stk.size() << " elements on stack\n"sv;
    std::cout << "Top element: "sv
              << stk.top()    // Leaves element on stack
              << '\n';
    std::cout << stk.size() << " elements on stack\n"sv;
    stk.pop();
    std::cout << stk.size() << " elements on stack\n"sv;
    std::cout << "Top element: "sv << stk.top() << '\n';

    std::cout << '\n';
  };

  /// Capacity
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::stack - empty"s << '\n';
  {
    std::cout << std::boolalpha;

    std::stack<int> container;

    std::cout << "Initially, container.empty(): "sv
              << container.empty() << '\n';

    container.push(42);
    std::cout << "After adding elements, container.empty(): "sv
              << container.empty() << '\n';

    container.pop();

    std::cout << "Initially, container.size(): "sv
              << container.size() << '\n';

    for (auto i_ : { 0, 1, 2, 3, 4, 5, 6, }) {
      container.push(i_);
    }

    std::cout << "After adding elements, container.size(): "sv
              << container.size() << '\n';

    std::cout << std::noboolalpha;

    std::cout << '\n';
  };

  /// Modifiers
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::stack - push"s << '\n';
  {
    /*
     *  ....+....!....+....!....+....!....+....!....+....!....+.
     *  BrainFuckInterpreter
     *  #see: https://en.wikipedia.org/wiki/Brainfuck
     */
    class BrainFuckInterpreter {
      std::map<unsigned, unsigned> open_brackets, close_brackets;
      unsigned program_pos_ { 0 };
      std::array<std::uint8_t, 32'768> data_;
      int data_pos_ { 0 };

      void collect_brackets_positions(std::string_view const program) {
        std::stack<unsigned> brackets_stack;

        for (auto pos { 0U }; pos != program.length(); ++pos) {
          char const ch { program[pos] };
          if ('[' == ch) {
            brackets_stack.push(pos);
          }
          else if (']' == ch) {
            if (brackets_stack.empty()) {
              throw std::runtime_error("brackets [] do not match!"s);
            }
            else {
              open_brackets[brackets_stack.top()] = pos;
              close_brackets[pos] = brackets_stack.top();
              brackets_stack.pop();
            }
          }
        }
     
        if (!brackets_stack.empty()) {
          throw std::runtime_error("brackets [] do not match!"s);
        }
      }

      void check_data_pos(int pos) {
        if (pos < 0 or pos >= static_cast<int>(data_.size())) {
          throw std::out_of_range{"data pointer out of bound"s};
        }
      }

    public:
      BrainFuckInterpreter(const std::string_view program) {
        collect_brackets_positions(program);
        data_.fill(0);

        for (; program_pos_ < program.length(); ++program_pos_) {
          switch (program[program_pos_]) {
            case '<':
              check_data_pos(--data_pos_);
              break;

            case '>':
              check_data_pos(++data_pos_);
              break;

            case '-':
              --data_[data_pos_];
              break;

            case '+':
              ++data_[data_pos_];
              break;

            case '.':
              std::cout << data_[data_pos_];
              break;

            case ',':
              std::cin >> data_[data_pos_];
              break;

            case '[':
              if (data_[data_pos_] == 0) {
                program_pos_ = open_brackets[program_pos_];
              }
              break;

            case ']':
              if (data_[data_pos_] != 0) {
                program_pos_ = close_brackets[program_pos_];
              }
              break;
          }
        }
      }
    };

    try {
      BrainFuckInterpreter {
        "++++++++[>++>>++>++++>++++<<<<<-]>[<+++>>+++<-]>[<+"
        "+>>>+<<-]<[>+>+<<-]>>>--------.<<+++++++++.<<----.>"
        ">>>>.<<<------.>..++.<++.+.-.>.<.>----.<--.++.>>>+."
      };
    }
    catch (std::exception & ex) {
      std::cout << "BrainFuckInterpreter exception: "sv
                << ex.what()
                << '\n';
    }

    std::cout << '\n';
  };

  /// Modifiers
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::stack - swap"s << '\n';
  {
    auto print = [](auto stack /* pass by value */, int id) {
      std::cout << "s"sv << id << " ["sv << stack.size() << "]: "sv;
      for (; !stack.empty(); stack.pop()) {
        std::cout << stack.top() << ' ';
      }
      std::cout << (id > 1 ? "\n\n"sv : "\n"sv);
    };

    std::vector<std::string>
#define UTF16_
#if defined(UTF16_)
    v1 { "1"s, "2"s, "3"s, "4"s, },
    v2 { "\u2C6F"s, "B"s, "\u0186"s, "D"s, "\u018E"s, };
#elif defined(UTF8_)
    v1 { "1"s, "2"s, "3"s, "4"s, },
    v2 { "\xE2\xB1\xAF"s, "B"s, "\xC6\x86"s, "D"s, "\xC6\x8E"s, };
#else
      v1 { "1"s, "2"s, "3"s, "4"s, },
      v2 { "Ɐ"s, "B"s, "Ɔ"s, "D"s, "Ǝ"s, };
#endif

    std::stack s1{std::move(v1)};
    std::stack s2{std::move(v2)};

    print(s1, 1);
    print(s2, 2);

    s1.swap(s2);

    print(s1, 1);
    print(s2, 2);

    std::cout << '\n';
  };

  /// Non-member functions
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::stack - std::swap(std::stack)"s << '\n';
  {
    std::stack<int> alice;
    std::stack<int> bob;

    auto print = [](auto const & title, auto const & cont) {
      std::cout << title << " size="sv << cont.size();
      std::cout << " top="sv << cont.top() << '\n';
    };

    for (int i_ = 1; i_ < 4; ++i_) {
      alice.push(i_);
    }
    for (int i_ = 7; i_ < 11; ++i_) {
      bob.push(i_);
    }

    // Print state before swap
    print("alice:"sv, alice);
    print("bob  :"sv, bob);

    std::cout << "-- SWAP\n"sv;
    std::swap(alice, bob);

    // Print state after swap
    print("alice:"sv, alice);
    print("bob  :"sv, bob);

    std::cout << '\n';
  };

  std::cout << std::endl; //  make sure cout is flushed.

  return 0;
}

//  MARK: - C_stack_deduction_guides
//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
//  ================================================================================
//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
/*
 *  MARK: C_stack_deduction_guides()
 */
auto C_stack_deduction_guides(int argc, char const * argv[]) -> decltype(argc) {
  std::cout << "In "s << __func__ << std::endl;

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::stack = deduction guides"s << '\n';
  {
    std::vector<int> vi = { 1, 2, 3, 4, };
    std::stack si { vi };    // guide #1 deduces std::stack<int, vector<int>>

    std::cout << '\n';
  };

std::cout << std::endl; //  make sure cout is flushed.
  return 0;
}
