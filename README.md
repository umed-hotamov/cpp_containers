Implementation of the stl containers

## Running The Tests

```sh
make
```

## Usage

Example of using `list/list.h`

```cpp
#include <iostream>
#include "list/list.h"

int main() {
    s21::List<int> list;

    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    for (const auto& element : list) {
        std::cout << element << " ";
    }
    std::cout << std::endl;

    return 0;
}
```
