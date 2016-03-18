tap.h
=====
Some simple macros for writing unit tests in C that use the Test Anything Protocol.

  * `plan(number_of_tests)`
  * `ok(got == expected, [test_name])`
  * `is(got, expected, fmt, [test_name])`
  * `isstr(got, expected, [test_name])`
  * `ismem(got, expected, size, [test_name])`
  * `isnt(got, expected, fmt, [test_name])`
  * `isntstr(got, expected, [test_name])`
  * `isntmem(got, expected, size, [test_name])`
  * `cmp_ok(got, op, expected, fmt, [test_name])`
  * `done_testing([number_of_tests])`
  * `BAIL_OUT(reason)`

## Examples

```c
    #include "tap.h"
    
    int main()
    {
        cmp_ok(0, <, 1, "%d");
        done_testing();
    }
```

```c
    #include "tap.h"
    
    int main(int argc, char *argv[])
    {
        plan(1);
        is(argc, 3, "%d", "There are two arguments");
        done_testing();
    }
```

```c
    #include "tap.h"
    
    int main(int argc, char *argv[])
    {
        isntstr("foo", "bar");
        done_testing(1);
    }
```
