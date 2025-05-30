# 📚 Library System

도서 정보를 기반으로 해시 테이블 및 이진 탐색 트리를 구성하여 다양한 방식의 도서 검색을 지원하는 시스템입니다.  
장르 ➝ 서브장르 ➝ 도서 구조로 저장되며, `CSV` 파일에서 데이터를 불러와 초기화됩니다.

---

## 🗂️ 폴더 구조

library_system/
├── datastructure/
│ ├── book_tree.h / book_tree.c
│ ├── genre_hash.h / genre_hash.c
│ └── …
├── books.csv
├── main.c
└── README.md

---

## 📈 주요 자료구조

### 📘 BookNode (도서 트리 노드)

```c
typedef struct BookNode {
    char title[100];
    char author[100];
    char genre[50];
    char sub_genre[50];
    struct BookNode* left;
    struct BookNode* right;
} BookNode;
```

### 🧱 GenreBucket (장르 해시 테이블)

```c
typedef struct GenreBucket {
    char genre[50];
    SubGenreNode* sub_genre_list;
    struct GenreBucket* next; // 체이닝
} GenreBucket;
```

### 📚 SubGenreNode (서브장르 연결 리스트)

```c
typedef struct SubGenreNode {
    char sub_genre[50];
    BookNode* book_bst; // 도서 BST
    struct SubGenreNode* next;
} SubGenreNode;
```

## 🔧 초기화 흐름 요약

### 1. 이진트리 탐색 방식

initialize_book_tree("books.csv");
├─ parse_line_with_quotes / without_quotes
│ ├─ create_and_insert_tree()
│ │ ├─ create_book_node()
│ │ └─ insert_book_node()

### 2. 장르 해시 테이블 방식

initialize_genre_hash("books.csv");
├─ parse_line_with_quotes / without_quotes
│ └─ insert_into_genre_hash_table()
│ │ ├─ hash_string()
│ │ ├─ create_genre_bucket()
│ │ ├─ insert_sub_genre_list()
│ │ ├─ create_book_node()
│ │ └─ insert_book_node()
