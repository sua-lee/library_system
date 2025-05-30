#include <stdlib.h>
#include "book_tree.h"
#include "genre_hash.h"
#include "author_hash.h"
#include "users.h"

void initialization (void);

void initialization (void) {
  User* head = NULL;
  load_users("users.csv", &head);

  initialize_book_tree("books.csv");
  initialize_genre_hash("books.csv");
  initialize_author_hash_from_genre_hash();
}