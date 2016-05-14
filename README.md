# sto library

"String TO" — C++ parsing library for numeric types.

## Rationale

It is written as replacement for standard library numeric parsers `std::sto(i|l|ll|ul|ull|f|d|ld)` and allow automatic selection of correct parsing function for types like `uint32_t`, where you have no portable guarantee of what fundamental type represent it.

Additionally standard parsers have complicated interface, behaviour and implying unavoidable overhead:
 * returning successful result when meet nondigit character, so caller need to check if this is end of string when format require strictly numbers
 * can depends on current C locale
 * unsigned parsers convert negative numbers to unsigned form without explicit error
 * allows to setup baseness of number representation, when most of the time decimal and hexadecimal (and rarely octal) is used and even autodetection of number base.

Need to mention that it's good to have these interfaces, but not as the only ones available.

So this library implement templates for parsing signed and unsinged integer numerics with very consistent error handling by exceptions.

## Floating point?

Just wrappers over `std::sto(f|d|ld)`.


## Tests

Decimal parser passes total testing:
 * All values for all types (except for `(u)int64_t` which is `(u)intmax_t` in my system).
 * Under/overflow tests for huge range out of the native domain.
 * Invalid characters in different places.
 * Empty strings.
 * Negative numbers in unsinged parsing.


## Performance

Because of absolute simplicity of interface the caculations is very simple and all unnecessary things is eliminated which gives good results especially for small integer types.
See `test/test_benchmark_sto.cpp`.

Example from my system:
```
     std::sto<int8_t> for 100000000 cycles took: 4600ms. 46.0002ns per cycle.
vanorder::sto<int8_t> for 100000000 cycles took: 1161ms. 11.614ns per cycle.
Test success.

     std::sto<int16_t> for 100000000 cycles took: 5093ms. 50.9361ns per cycle.
vanorder::sto<int16_t> for 100000000 cycles took: 1757ms. 17.5792ns per cycle.
Test success.

     std::sto<int32_t> for 100000000 cycles took: 6100ms. 61.0076ns per cycle.
vanorder::sto<int32_t> for 100000000 cycles took: 2841ms. 28.4145ns per cycle.
Test success.

     std::sto<int64_t> for 100000000 cycles took: 12601ms. 126.018ns per cycle.
vanorder::sto<int64_t> for 100000000 cycles took: 9080ms. 90.8021ns per cycle.
Test success.

     std::sto<uint8_t> for 100000000 cycles took: 4600ms. 46.0021ns per cycle.
vanorder::sto<uint8_t> for 100000000 cycles took: 1040ms. 10.401ns per cycle.
Test success.

     std::sto<uint16_t> for 100000000 cycles took: 4847ms. 48.4728ns per cycle.
vanorder::sto<uint16_t> for 100000000 cycles took: 1496ms. 14.96ns per cycle.
Test success.

     std::sto<uint32_t> for 100000000 cycles took: 6179ms. 61.795ns per cycle.
vanorder::sto<uint32_t> for 100000000 cycles took: 2617ms. 26.1742ns per cycle.
Test success.

     std::sto<uint64_t> for 100000000 cycles took: 15079ms. 150.792ns per cycle.
vanorder::sto<uint64_t> for 100000000 cycles took: 9261ms. 92.6154ns per cycle.
Test success.
```