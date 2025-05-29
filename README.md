# library_system

---

# 1. datastructure

## Book Tree Initialization Flow 주요 흐름 요약

1. **책 트리의 루트 포인터를 전역으로 선언**

   - `BookNode* book_root = NULL;`

2. **CSV 파일 열기 및 헤더 스킵**

   - `fopen()`으로 파일을 열고 `fgets()`로 첫 줄을 건너뜀

3. **각 줄을 읽어 개행문자 제거 후, 따옴표 포함 여부 검사**

   - `strchr(line, '"')`로 따옴표 유무 확인
   - 조건에 따라 `parse_line_with_quotes()` 또는 `parse_line_without_quotes()` 호출

4. **줄 파싱 및 필드 추출**

### parse_line_without_quotes()

- 각 필드를 `strtok()`으로 단순하게 구분해 `fields[]`에 저장
- 필드 수가 부족한 경우 무시
- `create_book_node()`로 `BookNode` 생성 후 `insert_book_node()` 호출

### parse_line_with_quotes()

- 따옴표 안 쉼표를 무시하기 위한 커스텀 파서 구현
- `in_quotes` 플래그를 사용하여 쉼표 처리 방식 결정
- 필드를 `fields[]` 배열에 저장
- author가 비어 있을 경우 `"Anonymous"`로 대체
- `create_book_node()`로 `BookNode` 생성 후 `insert_book_node()` 호출

5. **BookNode 메모리 할당 및 삽입**

- `create_book_node(title, author, genre, sub_genre)` 호출
- 트리에 삽입: `insert_book_node(&book_root, new_book)`

---

이 문서는 향후 디버깅 및 유지 보수를 위한 개발자 레퍼런스로 활용될 수 있습니다.
