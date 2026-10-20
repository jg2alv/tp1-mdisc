# Closure Issues — Binary-relation properties and closures in C

Closure Issues translates definitions from discrete mathematics into an explainable command-line report. It examines six properties of a binary relation, identifies the ordered pairs behind failed conditions, and constructs reflexive, symmetric, and transitive closures.

## 1. Academic context and scope

- **Institution:** Universidade Federal de Minas Gerais (UFMG)
- **Degree:** Ciência da Computação
- **Course:** Matemática Discreta
- **Assignment:** Trabalho Prático 1
- **Semester:** 2024-01

The analyzer checks reflexivity, irreflexivity, symmetry, antisymmetry, asymmetry, and transitivity. It combines those checks to classify equivalence relations and partial orders. The implementation uses C structures and dynamically allocated arrays to represent the underlying set and relation.

The scope is a finite relation over integer-valued set elements. The output retains the original Portuguese mathematical labels and uses `V` and `F` to indicate true and false.

## 2. How to run

Prerequisite: a C compiler. Compile the actual tracked entrypoint, [main.c](main.c), from the repository root:

```sh
gcc main.c -o closure-issues
```

The executable consumes standard input: first the number of set elements, then those elements, followed by pairs of integers until end of file. Supply a complete input stream through file redirection or a pipe so that EOF terminates reading. No input fixture is tracked in this repository.

## 3. Implementation decisions

- **Explicit relation model:** `Set`, `Relation`, and `Pair` keep set membership and ordered-pair storage separate. `find_pair` provides the membership operation used throughout the property routines.
- **Explanations from missing pairs:** the checks construct collections that can also support the report, rather than reducing every property immediately to a Boolean value.
- **Iterative transitive closure:** composition can reveal new connections after earlier additions. The implementation merges missing pairs into the relation and repeats until no more are found.
- **Readable ordering:** pair sorting produces lexicographic reports; antisymmetry reporting normalizes paired counterexamples before presentation.

## 4. Learning and limitations

The central learning challenge was distinguishing closely related conditions while converting quantifiers into loops and membership checks. Symmetry and antisymmetry require different reasoning, and transitive closure demonstrates why a single scan need not reach a fixed point.

The current input loop tests `feof(stdin)` before checking whether a pair was successfully read. Closure merging also returns an uninitialized relation when no extra pairs are needed. These source-level limitations need attention before treating every input as reliable.
