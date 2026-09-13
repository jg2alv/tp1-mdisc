#include <stdio.h>
#include <stdlib.h>

typedef struct Pair {
    int x;
    int y;
} Pair;

typedef struct Relation {
    Pair* pairs;
    int len;
} Relation;

typedef struct Set {
    int* elements;
    int len;
} Set;

int find_pair(int x, int y, Relation* rel, int upperbound);
Relation* reflexive_closure(Set* set, Relation* rel, int invert);
Relation* symmetric_closure(Set* set, Relation* rel, int invert);
Relation* antisymmetric_closure(Set* set, Relation* rel);
Relation* transitive_closure(Set* set, Relation* rel);
Relation* join_relation(Relation* rel1, Relation* rel2);
void sort_relation(Relation* rel, int sortswapped);
void print_report(Relation* rel, int i, char* name, int printexplanation, int printinreverse);
void print_closure(Relation* rel, char* name);

int main() {
    Set* set = (Set*)malloc(sizeof(Set));
    Relation* rel = (Relation*)malloc(sizeof(Relation));
    rel->pairs = (Pair*)malloc(sizeof(Pair));
    rel->len = 0;

    scanf("%d", &set->len);
    set->elements = (int*)malloc(set->len * sizeof(int));

    for(int i = 0; i < set->len; i++)
        scanf("%d", &set->elements[i]);

    for(int i = 0; !feof(stdin); i++) {
        rel->pairs = realloc(rel->pairs, sizeof(Pair) * (i + 1));
        Pair* curr = &rel->pairs[i];
        scanf("%d %d", &curr->x, &curr->y);
        rel->len++;
    }

    Relation* reflexive = reflexive_closure(set, rel, 0);
    Relation* irreflexive = reflexive_closure(set, rel, 1);
    Relation* symmetric = symmetric_closure(set, rel, 0);
    Relation* antisymmetric = antisymmetric_closure(set, rel);
    Relation* assymetric = symmetric_closure(set, rel, 1);
    Relation* transitive = transitive_closure(set, rel);

    sort_relation(reflexive, 0);
    sort_relation(irreflexive, 0);
    sort_relation(symmetric, 0);
    sort_relation(antisymmetric, 1);
    sort_relation(transitive, 0);

    printf("Propriedades\n");
    print_report(reflexive, 1, "Reflexiva", 1, 0);
    print_report(irreflexive, 2, "Irreflexiva", 1, 0);
    print_report(symmetric, 3, "Simetrica", 1, 0);
    print_report(antisymmetric, 4, "Anti-simetrica", 1, 1);
    print_report(assymetric, 5, "Assimetrica", 0, 0);
    print_report(transitive, 6, "Transitiva", 1, 0);
    
    int eq = !reflexive->len && !symmetric->len && !transitive->len;
    int parc = !reflexive->len && !antisymmetric->len && !transitive->len;
    printf("\nRelacao de equivalencia: %s", eq ? "V\n" : "F\n");
    printf("Relacao de ordem parcial: %s", parc ? "V\n\n" : "F\n\n");

    Relation* reflexive_joined = join_relation(rel, reflexive);
    Relation* symmetric_joined = join_relation(rel, symmetric);
    Relation* transitive_joined = join_relation(rel, transitive);

    if(transitive->len > 0) {
        Relation* newtransitive = transitive_closure(set, transitive_joined);
        while(newtransitive->len > 0) {
            transitive_joined = join_relation(transitive_joined, newtransitive);
            newtransitive = transitive_closure(set, transitive_joined);
        }
        free(newtransitive);
    }

    sort_relation(reflexive_joined, 0);
    sort_relation(symmetric_joined, 0);
    sort_relation(transitive_joined, 0);

    print_closure(reflexive_joined, "reflexivo");
    print_closure(symmetric_joined, "simetrico");
    print_closure(transitive_joined, "transitivo");

    free(set);
    free(rel);
    free(reflexive);
    free(irreflexive);
    free(symmetric);
    free(antisymmetric);
    free(assymetric);
    free(transitive);
    free(reflexive_joined);
    free(symmetric_joined);
    free(transitive_joined);

    return 0;
}

int find_pair(int x, int y, Relation* rel, int upperbound) {
    if(upperbound == -1) upperbound = rel->len;
    for(int i = 0; i < upperbound; i++) {
        Pair* currpair = &rel->pairs[i];
        if(currpair->x == x && currpair->y == y) return 1;
    }
    return 0;
}

Relation* reflexive_closure(Set* set, Relation* rel, int invert) {
    Relation* closure = (Relation*)malloc(sizeof(Relation));
    closure->pairs = (Pair*)malloc(sizeof(Pair));
    closure->len = 0;

    for(int i = 0; i < set->len; i++) {
        int currelem = set->elements[i];
        int found = find_pair(currelem, currelem, rel, -1);

        if(found == invert) {
            int len = closure->len;
            closure->pairs = (Pair*)realloc(closure->pairs, sizeof(Pair) * (len+1));
            Pair* currclospair = &closure->pairs[len];
            
            currclospair->x = currelem;
            currclospair->y = currelem;

            closure->len++;
        }
    }

    return closure;
}

Relation* symmetric_closure(Set* set, Relation* rel, int invert) {
    Relation* closure = (Relation*)malloc(sizeof(Relation));
    closure->pairs = (Pair*)malloc(sizeof(Pair));
    closure->len = 0;

    for(int i = 0; i < set->len; i++) {
        for(int j = 0; j < set->len; j++) {
            int x = set->elements[i];
            int y = set->elements[j];
            int relhasxy = find_pair(x, y, rel, -1);
            if(relhasxy) {
                int relhasyx = find_pair(y, x, rel, -1);
                if(relhasyx == invert) {
                    int len = closure->len;
                    closure->pairs = (Pair*)realloc(closure->pairs, sizeof(Pair) * (len+1));
                    Pair* currclospair = &closure->pairs[len];
            
                    currclospair->x = y;
                    currclospair->y = x;

                    closure->len++;
                }
            }
        }
    }

    return closure;
}

Relation* antisymmetric_closure(Set* set, Relation* rel) {
    Relation* closure = (Relation*)malloc(sizeof(Relation));
    closure->pairs = (Pair*)malloc(sizeof(Pair));
    closure->len = 0;

    for(int i = 0; i < set->len; i++) {
        for(int j = 0; j < set->len; j++) {
            int x = set->elements[i];
            int y = set->elements[j];
            int relhasxy = find_pair(x, y, rel, -1);
            if(relhasxy) {
                int relhasyx = find_pair(y, x, rel, -1);
                int closhasxy = find_pair(x, y, closure, -1);

                if(relhasyx && x != y && !closhasxy) {
                    int len = closure->len;
                    closure->pairs = (Pair*)realloc(closure->pairs, sizeof(Pair) * (len+1));
                    Pair* currclospair = &closure->pairs[len];
            
                    currclospair->x = y;
                    currclospair->y = x;

                    closure->len++;
                }
            }
        }
    }

    return closure;
}

Relation* transitive_closure(Set* set, Relation* rel) {
    Relation* closure = (Relation*)malloc(sizeof(Relation));
    closure->pairs = (Pair*)malloc(sizeof(Pair));
    closure->len = 0;

    for(int i = 0; i < set->len; i++) {
        for(int j = 0; j < set->len; j++) {
            for(int k = 0; k < set->len; k++) {
                int x = set->elements[i];
                int y = set->elements[j];
                int z = set->elements[k];
                int relhasxy = find_pair(x, y, rel, -1);
                int relhasyz = find_pair(y, z, rel, -1);
                if(relhasxy && relhasyz) {
                    int relhasxz = find_pair(x, z, rel, -1);
                    int closhasxz = find_pair(x, z, closure, -1);
                    if(!relhasxz && !closhasxz) {
                        int len = closure->len;
                        closure->pairs = (Pair*)realloc(closure->pairs, sizeof(Pair) * (len+1));
                        Pair* currclospair = &closure->pairs[len];
            
                        currclospair->x = x;
                        currclospair->y = z;

                        closure->len++;
                    }
                }
            }
        }
    }

    return closure;
}

Relation* join_relation(Relation* rel1, Relation* rel2) {
    Relation *join = (Relation*)malloc(sizeof(Relation));
    if(!rel2->len) return join;
    
    int cap = rel1->len + rel2->len;
    int sz = 0;
    join->pairs = (Pair*)malloc(cap * sizeof(Pair));
    join->len = cap;

    for(int i = 0; i < rel1->len; i++) {
        Pair* newpair = &join->pairs[sz];
        Pair* oldpair = &rel1->pairs[i];

        if(find_pair(oldpair->x, oldpair->y, join, sz)) continue;

        newpair->x = oldpair->x;
        newpair->y = oldpair->y;
        sz++;
    }

    for(int i = 0; i < rel2->len; i++) {
        Pair* newpair = &join->pairs[sz];
        Pair* oldpair = &rel2->pairs[i];

        if(find_pair(oldpair->x, oldpair->y, join, sz)) continue;

        newpair->x = oldpair->x;
        newpair->y = oldpair->y;
        sz++;
    }

    join->pairs = realloc(join->pairs, sz * sizeof(Pair));
    join->len = sz;
    return join;
}

void reduce_pair(Pair* p1) {
    if(p1->x > p1->y) {
        int x = p1->x;
        p1->x = p1->y;
        p1->y = x;
    }
}

void sort_relation(Relation* rel, int sortswapped) {
    for(int i = 0; i < rel->len - 1; i++) {
        for(int j = i + 1; j < rel->len; j++) {
            Pair* currpair = &rel->pairs[i], *nextpair = &rel->pairs[j];
            if(sortswapped) {
                reduce_pair(currpair);
                reduce_pair(nextpair);
            }

            int iscurrxbigger = currpair->x > nextpair->x;
            int iscurrxequal = currpair->x == nextpair->x;
            int iscurrybigger = currpair->y > nextpair->y;

            if(iscurrxbigger || (iscurrxequal && iscurrybigger)) {
                int x = currpair->x;
                currpair->x = nextpair->x;
                nextpair->x = x;

                int y = currpair->y;
                currpair->y = nextpair->y;
                nextpair->y = y;
            }
        }
    }
}

void print_report(Relation* rel, int i, char* name, int printexplanation, int printinreverse) {
    printf("%d. %s: ", i, name);
    if(!rel->len) printf("V\n");
    else {
        printf("F\n");
        if(!printexplanation) return;
        for(int i = 0; i < rel->len; i++) {
            Pair* curr = &rel->pairs[i];
            if(!printinreverse) printf("(%d, %d)", curr->x, curr->y);
            else printf("((%d, %d), (%d, %d))", curr->x, curr->y, curr->y, curr->x);

            if(i != rel->len - 1) printf(", ");
        }
        printf("\n");
    }
}

void print_closure(Relation* rel, char* name) {
    printf("Fecho %s da relacao:", name);
    if(!rel->len) printf(" R");
    else {
        printf("\n");
        for(int i = 0; i < rel->len; i++) {
            Pair* currpair = &rel->pairs[i];
            printf("(%d, %d)", currpair->x, currpair->y);
            if(i != rel->len - 1) printf(", ");
        }
    }
    printf("\n");
}