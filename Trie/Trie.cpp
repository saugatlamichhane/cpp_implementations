#include <iostream>
#include <string>
#include <unordered_map>
#include <memory> // for unique_ptr
#include <string_view> 

struct TrieNode {
  int ends{0};
  int startsWith{0};
  // Automatic lifetime management: replacing raw pointers
  std::unordered_map<char, std::unique_ptr<TrieNode>> children;
};

class Trie {
public:
  // std::make_unique is safer and cleaner than 'new'
  Trie() : root(std::make_unique<TrieNode>()) {}


  // Pass by string_view to avoid copying string data
  void insert(std::string_view word) {
    // Use get() to get a non-owning raw pointer for traversal
    auto* current = root.get();
    for (auto ch : word) {
      auto& child = current->children[ch];
      if(!child) {
        // actually modifies the map entry inside current->children
        child = std::make_unique<TrieNode>();
      }
      current = child.get();
      current->startsWith++;
    }
    current->ends++;
  }

  // const method because it doesn't modify the Trie
  int countWordsEqualTo(std::string_view word) const {
    const auto* current = root.get();
    for (auto ch : word) {
      // just one lookup
      // doesn't use subscript which can mutate
      auto it = current->children.find(ch);
      if (it == current->children.end()) {
        return 0;
      }
      current = it->second.get();
    }
    return current->ends;
  }

  int countWordsStartingWith(std::string_view word) const {
    const auto* current = root.get();
    for (auto ch : word) {
      auto it = current->children.find(ch);
      if (it == current->children.end()) {
        return 0;
      }
      current = it->second.get();
    }
    return current->startsWith;
  }

  void erase(std::string_view word) {
    auto* current = root.get();
    for (auto ch : word) {
      auto it = current->children.find(ch);
      if (it == current->children.end()) {
        return;
      }

      current = it->second.get();
      current->startsWith--;
    }
    current->ends--;
  }
  // No custom destructor needed! unique_ptr cleans up automatically
  // ~Trie() { deleteTrie(root); }
  ~Trie() = default;

private:
  // No need to delete manually
  // void deleteTrie(TrieNode *node) {
  //   for (auto &[ch, child] : node->children) {
  //     deleteTrie(child);
  //   }
  //   delete node;
  // }
  std::unique_ptr<TrieNode> root;
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