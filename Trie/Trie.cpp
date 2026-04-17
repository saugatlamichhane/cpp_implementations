#include <iostream>
#include <string>
#include <unordered_map>

struct TrieNode {
  int ends{0};
  int startsWith{0};
  std::unordered_map<char, TrieNode *> children;
};

class Trie {
public:
  Trie() : root(new TrieNode()) {}

  void insert(std::string word) {
    auto trie = root;
    for (auto ch : word) {
      if (trie->children.count(ch) == 0) {
        trie->children[ch] = new TrieNode();
      }
      trie = trie->children[ch];
      trie->startsWith++;
    }
    trie->ends++;
  }

  int countWordsEqualTo(std::string word) {
    auto trie = root;
    for (auto ch : word) {
      if (trie->children.count(ch) == 0) {
        return 0;
      }
      trie = trie->children[ch];
    }
    return trie->ends;
  }

  int countWordsStartingWith(std::string word) {
    auto trie = root;
    for (auto ch : word) {
      if (trie->children.count(ch) == 0) {
        return 0;
      }
      trie = trie->children[ch];
    }
    return trie->startsWith;
  }

  void erase(std::string word) {
    auto trie = root;
    for (auto ch : word) {
      if (trie->children.count(ch) == 0) {
        return;
      }

      trie = trie->children[ch];
      trie->startsWith--;
    }
    trie->ends--;
  }
  ~Trie() { deleteTrie(root); }

private:
  void deleteTrie(TrieNode *node) {
    for (auto &[ch, child] : node->children) {
      deleteTrie(child);
    }
    delete node;
  }
  TrieNode *root;
};

int main() {
  Trie trie;
  trie.insert("apple");
  std::cout << trie.countWordsEqualTo("apple") << "\n";
  trie.insert("app");
  std::cout << trie.countWordsStartingWith("app") << "\n";
  trie.erase("apple");
  std::cout << trie.countWordsStartingWith("app") << "\n";
  return 0;
}