#include "test_runner.h"
#include <iterator>
#include <list>
#include <string>

using namespace std;

class Editor {
public:
  // Реализуйте конструктор по умолчанию и объявленные методы
  Editor() = default;
  void Left() {
    if (pos != text.begin()) {
      pos = prev(pos);
    }
  };
  void Right() {
    if (pos != text.end()) {
      pos = next(pos);
    }
  };
  void Insert(char token) { text.insert(pos, token); };
  void Cut(size_t tokens = 1) {
    buff.clear();
    auto start = pos;
    for (size_t i = 0; i < tokens; ++i) {
      if (pos == text.end()) {
        break;
      }
      pos = next(pos);
    }
    buff.splice(buff.begin(), text, start, pos);
  }
  void Copy(size_t tokens = 1) {
    buff.clear();
    auto end = pos;
    for (size_t i = 0; i < tokens; ++i) {
      if (end == text.end()) {
        break;
      }
      end = next(end);
    }
    buff.assign(pos, end);
  };
  void Paste() { text.insert(pos, buff.begin(), buff.end()); };
  string GetText() const { return string(text.begin(), text.end()); }

private:
  list<char> text = {};
  list<char>::iterator pos = text.begin();
  list<char> buff = {};
};

void TypeText(Editor &editor, const string &text) {
  for (char c : text) {
    editor.Insert(c);
  }
}
void TestEditing() {
  {
    Editor editor;

    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world");
    for (size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }

    editor.Cut(first_part_len);
    for (size_t i = 0; i < text_len - first_part_len; ++i) {
      editor.Right();
    }
    TypeText(editor, ", ");
    editor.Paste();

    editor.Left();
    editor.Left();
    editor.Cut(3);

    ASSERT_EQUAL(editor.GetText(), "world, hello");
  }
  {
    Editor editor;

    TypeText(editor, "misprnit");
    editor.Left();
    editor.Left();
    editor.Left();
    editor.Cut(1);
    editor.Right();
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "misprint");
  }
}

void TestReverse() {
  Editor editor;

  const string text = "esreveR";
  for (char c : text) {
    editor.Insert(c);
    editor.Left();
  }

  ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
  Editor editor;
  ASSERT_EQUAL(editor.GetText(), "");

  editor.Left();
  editor.Left();
  editor.Right();
  editor.Right();
  editor.Copy(0);
  editor.Cut(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
  Editor editor;

  editor.Paste();
  TypeText(editor, "example");
  editor.Left();
  editor.Left();
  editor.Paste();
  editor.Right();
  editor.Paste();
  editor.Copy(0);
  editor.Paste();
  editor.Left();
  editor.Cut(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestEditing);
  RUN_TEST(tr, TestReverse);
  RUN_TEST(tr, TestNoText);
  RUN_TEST(tr, TestEmptyBuffer);
  return 0;
}