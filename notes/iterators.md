# C++ Iterator Notes

## The Big Picture

An iterator is an object that lets you move through a container.

Containers include things like:

```cpp
std::string
std::vector<int>
std::array<int, 5>
std::map<std::string, int>
```

An iterator works a lot like a pointer. It points at one item inside a container, and you can use it to access that item or move to the next one.

For example:

```cpp
std::vector<int> numbers = {10, 20, 30};

auto it = numbers.begin();
```

Here, `it` points at the first element in `numbers`.

```cpp
std::cout << *it << '\n'; // prints 10
```

The `*` dereferences the iterator. That means "get the value this iterator currently points to."

## `begin()` and `end()`

Most containers have `.begin()` and `.end()`.

```cpp
auto first = numbers.begin();
auto last = numbers.end();
```

`begin()` returns an iterator pointing at the first element.

`end()` returns an iterator pointing one past the last element.

Important: `end()` does not point at a real element.

For a vector like this:

```cpp
std::vector<int> numbers = {10, 20, 30};
```

The iterators are conceptually like this:

```text
begin()
  |
  v
 10   20   30   one-past-the-end
                ^
                |
               end()
```

You can compare an iterator to `end()` to know when to stop.

## Looping with Iterators

A common iterator loop looks like this:

```cpp
for (auto it = numbers.begin(); it != numbers.end(); ++it) {
    std::cout << *it << '\n';
}
```

Line by line:

```cpp
auto it = numbers.begin()
```

Start at the first element.

```cpp
it != numbers.end()
```

Keep going while the iterator has not reached the end.

```cpp
++it
```

Move the iterator to the next element.

```cpp
*it
```

Access the value the iterator points at.

## Modifying Values with Iterators

If the container is not `const`, you can often modify values through the iterator.

```cpp
std::vector<int> numbers = {10, 20, 30};

for (auto it = numbers.begin(); it != numbers.end(); ++it) {
    *it = *it + 1;
}
```

After this loop, `numbers` contains:

```text
11, 21, 31
```

This works because `*it` gives you access to the actual element inside the vector, not a copy.

## Iterators with Strings

A `std::string` is also a container. Its elements are characters.

```cpp
std::string text = "abc";

for (auto it = text.begin(); it != text.end(); ++it) {
    std::cout << *it << '\n';
}
```

This prints:

```text
a
b
c
```

You can also modify the characters:

```cpp
std::string text = "abc";

for (auto it = text.begin(); it != text.end(); ++it) {
    *it = std::toupper(static_cast<unsigned char>(*it));
}
```

After the loop, `text` is:

```text
ABC
```

## Why Algorithms Use Iterators

Many standard library algorithms use iterators because iterators let the same algorithm work with many different containers.

For example:

```cpp
std::sort(numbers.begin(), numbers.end());
```

This means:

- start sorting at the first element
- stop at one past the last element

Another example:

```cpp
std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) {
    return std::tolower(c);
});
```

This means:

- read characters from `text.begin()` up to `text.end()`
- write the transformed characters starting at `text.begin()`
- use the lambda function to transform each character

## Iterator Ranges

In C++, a range is often represented by two iterators:

```cpp
[begin, end)
```

That means:

- include the element at `begin`
- keep going until `end`
- do not include `end` itself

This is called a half-open range.

For example:

```cpp
numbers.begin(), numbers.end()
```

means "all elements in `numbers`."

## `auto` with Iterators

Iterator types can be long and annoying to write.

Instead of this:

```cpp
std::vector<int>::iterator it = numbers.begin();
```

Most modern C++ code uses:

```cpp
auto it = numbers.begin();
```

`auto` tells the compiler to figure out the type.

This is especially useful with iterators.

## Const Iterators

A const iterator lets you read elements but not modify them.

```cpp
std::vector<int> numbers = {10, 20, 30};

for (auto it = numbers.cbegin(); it != numbers.cend(); ++it) {
    std::cout << *it << '\n';
}
```

`cbegin()` and `cend()` return const iterators.

This would not be allowed:

```cpp
*it = 99; // error
```

Use const iterators when you only need to read values.

## Range-Based For Loops

Most of the time, you do not need to write iterators directly.

This:

```cpp
for (auto it = numbers.begin(); it != numbers.end(); ++it) {
    std::cout << *it << '\n';
}
```

Can usually be written as:

```cpp
for (int value : numbers) {
    std::cout << value << '\n';
}
```

If you want to modify each value, use a reference:

```cpp
for (int& value : numbers) {
    value++;
}
```

The range-based for loop is easier to read, but iterators are still important because many algorithms and container operations use them.

## Common Mistakes

Do not dereference `end()`:

```cpp
auto it = numbers.end();
std::cout << *it << '\n'; // wrong
```

`end()` points one past the last element, so there is no real value there.

Do not use an iterator after the container changes in certain ways:

```cpp
std::vector<int> numbers = {10, 20, 30};
auto it = numbers.begin();

numbers.push_back(40);

std::cout << *it << '\n'; // may be wrong
```

For vectors, adding elements can move the vector's storage in memory. If that happens, old iterators no longer point to valid elements.

Do not confuse the iterator with the value:

```cpp
auto it = numbers.begin();
```

`it` is the iterator.

```cpp
*it
```

`*it` is the value the iterator points at.

## Quick Summary

- An iterator lets you move through a container.
- `begin()` points at the first element.
- `end()` points one past the last element.
- `*it` gets the value the iterator points at.
- `++it` moves to the next element.
- Algorithms use iterators to describe what part of a container to work on.
- Prefer range-based for loops when they are clear.
- Use iterators directly when an algorithm or container operation needs them.
