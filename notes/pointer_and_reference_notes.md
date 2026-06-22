# C++ Pointer, Reference, and Passing Notes

## The Big Picture

C++ gives you several ways to work with data:

```cpp
int x = 10;
```

You can:

- use `x` directly
- copy `x` into another variable
- refer to `x` through a reference
- refer to `x` through a pointer

The main difference is whether you are working with a copy, an alias, or a memory address.

## Passing by Value

Passing by value means the function receives a copy.

```cpp
void addOne(int n) {
    n++;
}

int main() {
    int x = 10;
    addOne(x);

    // x is still 10
}
```

Inside `addOne`, `n` is a separate variable. Changing `n` does not change `x`.

Use pass-by-value when:

- the function should not modify the original value
- copying is cheap, like `int`, `char`, `double`, or small structs
- the function needs its own independent copy

Example:

```cpp
int square(int n) {
    return n * n;
}
```

## References

A reference is another name for an existing variable.

```cpp
int x = 10;
int& ref = x;

ref = 20;

// x is now 20
```

`ref` is not a separate integer. It is an alias for `x`.

The `&` means different things depending on context:

```cpp
int& ref = x;  // reference declaration
int* ptr = &x; // address-of operator
```

In `int& ref`, `&` means "reference to int."

In `&x`, `&` means "the address of x."

## Reference Rules

A reference must be initialized immediately:

```cpp
int x = 10;
int& ref = x; // okay
```

This is not allowed:

```cpp
int& ref; // error
```

A reference cannot be reseated to refer to another variable:

```cpp
int a = 1;
int b = 2;

int& ref = a;
ref = b;
```

This does not make `ref` refer to `b`. It assigns the value of `b` into `a`.

Afterward:

```cpp
// a is 2
// b is 2
// ref still refers to a
```

## Passing by Reference

Passing by reference lets a function modify the original argument.

```cpp
void addOne(int& n) {
    n++;
}

int main() {
    int x = 10;
    addOne(x);

    // x is now 11
}
```

Use pass-by-reference when:

- the function should modify the caller's object
- the argument must exist
- null is not a meaningful option

Example:

```cpp
void resetToZero(int& value) {
    value = 0;
}
```

## Const References

A `const` reference lets you avoid copying while preventing modification.

```cpp
void printName(const std::string& name) {
    std::cout << name << '\n';
}
```

This is useful for larger objects:

```cpp
void printVector(const std::vector<int>& values) {
    for (int value : values) {
        std::cout << value << '\n';
    }
}
```

The function receives access to the original object, but cannot modify it through that reference.

```cpp
void bad(const std::string& name) {
    name = "new name"; // error
}
```

Use `const T&` when:

- you want to read an object
- you do not want to copy it
- the function should not modify it

This is one of the most common parameter styles in C++.

## Pointers

A pointer stores a memory address.

```cpp
int x = 10;
int* ptr = &x;
```

Here:

- `x` is an `int`
- `&x` is the address of `x`
- `ptr` stores that address
- `ptr` has type `int*`, meaning "pointer to int"

You can use `*` to dereference the pointer.

```cpp
int x = 10;
int* ptr = &x;

std::cout << *ptr << '\n'; // prints 10

*ptr = 25;

// x is now 25
```

Dereferencing means "go to the thing this pointer points at."

The `*` also has different meanings depending on context:

```cpp
int* ptr = &x; // pointer declaration
*ptr = 20;     // dereference ptr
```

In `int* ptr`, `*` means "pointer to int."

In `*ptr`, `*` means "the object pointed to by ptr."

## Null Pointers

A pointer can point to nothing.

```cpp
int* ptr = nullptr;
```

Before dereferencing a pointer that might be null, check it:

```cpp
if (ptr != nullptr) {
    std::cout << *ptr << '\n';
}
```

Dereferencing a null pointer is invalid:

```cpp
int* ptr = nullptr;
std::cout << *ptr; // bad
```

Use pointers when:

- the object might be optional
- null is meaningful
- you need to reseat the pointer to refer to different objects
- you are working with APIs that require pointers
- you are working with dynamic allocation or ownership tools

## Reassigning Pointers

Pointers can be changed to point somewhere else.

```cpp
int a = 10;
int b = 20;

int* ptr = &a;
std::cout << *ptr << '\n'; // 10

ptr = &b;
std::cout << *ptr << '\n'; // 20
```

Changing the pointer changes what it points to.

Changing `*ptr` changes the pointed-to value.

```cpp
int a = 10;
int b = 20;

int* ptr = &a;
*ptr = 99;

// a is now 99
// b is still 20

ptr = &b;
*ptr = 77;

// a is still 99
// b is now 77
```

## Passing by Pointer

Passing by pointer gives a function access to an object through its address.

```cpp
void addOne(int* n) {
    if (n != nullptr) {
        (*n)++;
    }
}

int main() {
    int x = 10;
    addOne(&x);

    // x is now 11
}
```

The parentheses matter:

```cpp
(*n)++;
```

This means "increment the int pointed to by `n`."

Without parentheses:

```cpp
*n++;
```

That is parsed as:

```cpp
*(n++);
```

It increments the pointer, not the pointed-to value. That is usually not what you want.

Use pointer parameters when:

- the argument can be optional
- `nullptr` is a meaningful value
- the function may need to work with arrays or low-level memory
- an API expects pointer style

Example:

```cpp
void printIfPresent(const int* value) {
    if (value != nullptr) {
        std::cout << *value << '\n';
    }
}
```

## Pointer vs Reference Parameters

These two functions can both modify the original integer:

```cpp
void byReference(int& value) {
    value = 100;
}

void byPointer(int* value) {
    if (value != nullptr) {
        *value = 100;
    }
}
```

Called like this:

```cpp
int x = 10;

byReference(x);
byPointer(&x);
```

The reference version says:

> This function requires a valid integer.

The pointer version says:

> This function receives an address, and it might be null.

As a rule of thumb:

```cpp
void f(T value);        // copy
void f(T& value);       // modify required object
void f(const T& value); // read required object without copying
void f(T* value);       // optional or reseatable object
void f(const T* value); // optional read-only object
```

## Structs and Classes with References

References work naturally with objects.

```cpp
struct Player {
    std::string name;
    int score;
};

void addScore(Player& player, int amount) {
    player.score += amount;
}
```

Call it like this:

```cpp
Player p{"Ada", 10};
addScore(p, 5);

// p.score is now 15
```

Use `.` with references and normal objects:

```cpp
player.score = 20;
```

## Structs and Classes with Pointers

When you have a pointer to an object, you can access members with `->`.

```cpp
struct Player {
    std::string name;
    int score;
};

Player p{"Ada", 10};
Player* ptr = &p;

ptr->score = 20;
```

The `->` operator means:

```cpp
ptr->score
```

is shorthand for:

```cpp
(*ptr).score
```

The parentheses are required in the long form because `.` binds more tightly than `*`.

This is wrong:

```cpp
*ptr.score // error
```

That tries to access `score` on `ptr` first, but `ptr` is a pointer, not a `Player`.

Use:

```cpp
(*ptr).score
```

or, preferably:

```cpp
ptr->score
```

## The Arrow Operator

Use `->` when you have a pointer to a struct or class.

```cpp
class Account {
public:
    void deposit(int amount) {
        balance += amount;
    }

    int getBalance() const {
        return balance;
    }

private:
    int balance = 0;
};

int main() {
    Account account;
    Account* accountPtr = &account;

    accountPtr->deposit(50);

    std::cout << accountPtr->getBalance() << '\n';
}
```

Equivalent long form:

```cpp
(*accountPtr).deposit(50);
std::cout << (*accountPtr).getBalance() << '\n';
```

The arrow version is preferred because it is clearer.

## Pointer to Const

A pointer to const means you cannot modify the pointed-to value through that pointer.

```cpp
int x = 10;
const int* ptr = &x;

std::cout << *ptr << '\n'; // okay
*ptr = 20;                 // error
```

But the pointer itself can point somewhere else:

```cpp
int y = 30;
ptr = &y; // okay
```

Read this as:

```cpp
const int* ptr
```

"ptr is a pointer to const int."

This is also commonly written as:

```cpp
int const* ptr
```

Same meaning.

## Const Pointer

A const pointer means the pointer itself cannot be changed after initialization.

```cpp
int x = 10;
int y = 20;

int* const ptr = &x;

*ptr = 99; // okay, modifies x
ptr = &y;  // error
```

Read this as:

```cpp
int* const ptr
```

"ptr is a const pointer to int."

## Const Pointer to Const

You can make both the pointer and the pointed-to value const.

```cpp
int x = 10;
const int* const ptr = &x;

*ptr = 20; // error
ptr = nullptr; // error
```

This means:

- you cannot modify the value through `ptr`
- you cannot make `ptr` point somewhere else

## Dynamic Allocation

C++ lets you allocate objects dynamically with `new`.

```cpp
int* value = new int(10);

std::cout << *value << '\n';

delete value;
value = nullptr;
```

But in modern C++, avoid raw `new` and `delete` in normal code. Prefer smart pointers.

Use `std::unique_ptr` for single ownership:

```cpp
#include <memory>

std::unique_ptr<int> value = std::make_unique<int>(10);

std::cout << *value << '\n';
```

For objects:

```cpp
#include <memory>

struct Player {
    std::string name;
    int score;
};

std::unique_ptr<Player> player = std::make_unique<Player>("Ada", 10);

player->score = 20;
```

Smart pointers also use `*` and `->`.

## Arrays and Pointers

Arrays and pointers are related, but they are not exactly the same thing.

```cpp
int values[] = {10, 20, 30};

int* ptr = values;
```

For an array, the name often decays into a pointer to its first element.

These are equivalent:

```cpp
std::cout << values[0] << '\n';
std::cout << *ptr << '\n';
```

Pointer arithmetic can move through the array:

```cpp
std::cout << *(ptr + 1) << '\n'; // 20
std::cout << *(ptr + 2) << '\n'; // 30
```

But prefer standard containers when possible:

```cpp
std::vector<int> values = {10, 20, 30};
```

or fixed-size arrays:

```cpp
std::array<int, 3> values = {10, 20, 30};
```

## Returning References and Pointers

Never return a reference or pointer to a local variable.

Bad:

```cpp
int& badReference() {
    int x = 10;
    return x; // bad: x is destroyed when the function ends
}

int* badPointer() {
    int x = 10;
    return &x; // bad: x is destroyed when the function ends
}
```

The returned reference or pointer would refer to an object that no longer exists.

Returning a reference can be okay if the object outlives the function call:

```cpp
std::string& firstName(std::vector<std::string>& names) {
    return names[0];
}
```

But this requires care: the vector must not be empty, and the returned reference only remains valid while the vector element remains valid.

## Common Mistakes

Dereferencing null:

```cpp
int* ptr = nullptr;
*ptr = 10; // bad
```

Forgetting `&` when passing to a pointer parameter:

```cpp
void setToZero(int* value) {
    *value = 0;
}

int x = 5;
setToZero(x);  // error
setToZero(&x); // okay
```

Using `.` instead of `->` with a pointer:

```cpp
Player* player = &p;

player.score = 10;  // error
player->score = 10; // okay
```

Confusing pointer reassignment with value assignment:

```cpp
int a = 1;
int b = 2;
int* ptr = &a;

ptr = &b; // ptr now points to b
*ptr = 9; // b is now 9
```

Confusing reference assignment with rebinding:

```cpp
int a = 1;
int b = 2;
int& ref = a;

ref = b; // assigns b's value into a
```

## Practical Decision Guide

Use pass-by-value:

```cpp
void setCount(int count);
```

when the object is small or you need a copy.

Use pass-by-reference:

```cpp
void normalize(Vector3& v);
```

when the function modifies a required object.

Use pass-by-const-reference:

```cpp
void printUser(const User& user);
```

when the function reads a larger required object without copying it.

Use pass-by-pointer:

```cpp
void setTarget(Entity* target);
```

when the value may be absent, so `nullptr` is valid.

Use smart pointers:

```cpp
std::unique_ptr<Entity> entity;
```

when ownership matters.

Use raw pointers mostly for non-owning access:

```cpp
Entity* selectedEntity = nullptr;
```

This usually means:

> This points at something owned elsewhere, or it points at nothing.

## Quick Syntax Summary

```cpp
int x = 10;

int copy = x;       // copy of x
int& ref = x;       // reference to x
int* ptr = &x;      // pointer to x

copy = 20;          // changes copy only
ref = 30;           // changes x
*ptr = 40;          // changes x

int y = 50;
ptr = &y;           // ptr now points to y
*ptr = 60;          // changes y
```

For objects:

```cpp
Player p{"Ada", 10};

Player& ref = p;
Player* ptr = &p;

ref.score = 20;   // use . with object/reference
ptr->score = 30;  // use -> with pointer
```

Function parameters:

```cpp
void a(int value);        // copy
void b(int& value);       // mutable reference
void c(const int& value); // read-only reference
void d(int* value);       // pointer, possibly null
void e(const int* value); // pointer to read-only int, possibly null
```

