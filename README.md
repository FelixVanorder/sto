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

## `constexpr`

Almost (gcc version 5.2.1):
```
dsto.hpp:23:5: note: ‘constexpr typename std::enable_if<(std::numeric_limits<_Tp>::is_integer && std::numeric_limits<_Tp>::is_signed), NumType>::type vanorder::sto(const char*, const char*) [with NumType = int; typename std::enable_if<(std::numeric_limits<_Tp>::is_integer && std::numeric_limits<_Tp>::is_signed), NumType>::type = int]’ is not usable as a constexpr function because:
     sto( char const * begin, char const * const end )
     ^
dsto.hpp:23:5: sorry, unimplemented: unexpected AST of kind loop_expr
dsto.hpp:23: confused by earlier errors, bailing out

```

## Floating point?

Just wrappers over `std::sto(f|d|ld)`.

## Usage

```
auto number = vanorder::sto<int>( "-123" );
auto index = vanorder::sto<size_t>( std::to_string( 42 ) );
auto bignum = vanorder::sto<uint64_t>( "18446744073709551615" );
auto keepitreal = vanorder::sto<long double>( "3.14159265358979323846" );
```

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

Example for my system:
```
     std::sto<int8_t> for 10000000 cycles took: 339ms. 33.9695ns per cycle.
vanorder::sto<int8_t> for 10000000 cycles took: 120ms. 12.008ns per cycle.

     std::sto<int16_t> for 10000000 cycles took: 392ms. 39.2101ns per cycle.
vanorder::sto<int16_t> for 10000000 cycles took: 179ms. 17.9658ns per cycle.

     std::sto<int32_t> for 10000000 cycles took: 501ms. 50.1904ns per cycle.
vanorder::sto<int32_t> for 10000000 cycles took: 316ms. 31.6899ns per cycle.

     std::sto<int64_t> for 10000000 cycles took: 1142ms. 114.244ns per cycle.
vanorder::sto<int64_t> for 10000000 cycles took: 906ms. 90.6654ns per cycle.

     std::sto<uint8_t> for 10000000 cycles took: 344ms. 34.4799ns per cycle.
vanorder::sto<uint8_t> for 10000000 cycles took: 114ms. 11.4482ns per cycle.

     std::sto<uint16_t> for 10000000 cycles took: 380ms. 38.0922ns per cycle.
vanorder::sto<uint16_t> for 10000000 cycles took: 148ms. 14.8693ns per cycle.

     std::sto<uint32_t> for 10000000 cycles took: 539ms. 53.9712ns per cycle.
vanorder::sto<uint32_t> for 10000000 cycles took: 210ms. 21.0983ns per cycle.

     std::sto<uint64_t> for 10000000 cycles took: 1249ms. 124.953ns per cycle.
vanorder::sto<uint64_t> for 10000000 cycles took: 827ms. 82.7162ns per cycle.

```