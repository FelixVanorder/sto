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
     std::sto<int8_t> for 10000000 calls took: 339ms. 33.9357ns per call.
vanorder::sto<int8_t> for 10000000 calls took: 118ms. 11.8378ns per call.

     std::sto<int16_t> for 10000000 calls took: 393ms. 39.3204ns per call.
vanorder::sto<int16_t> for 10000000 calls took: 179ms. 17.952ns per call.

     std::sto<int32_t> for 10000000 calls took: 504ms. 50.4429ns per call.
vanorder::sto<int32_t> for 10000000 calls took: 316ms. 31.6767ns per call.

     std::sto<int64_t> for 10000000 calls took: 1142ms. 114.244ns per call.
vanorder::sto<int64_t> for 10000000 calls took: 905ms. 90.5819ns per call.

     std::sto<uint8_t> for 10000000 calls took: 342ms. 34.2943ns per call.
vanorder::sto<uint8_t> for 10000000 calls took: 115ms. 11.5372ns per call.

     std::sto<uint16_t> for 10000000 calls took: 380ms. 38.0403ns per call.
vanorder::sto<uint16_t> for 10000000 calls took: 149ms. 14.9052ns per call.

     std::sto<uint32_t> for 10000000 calls took: 539ms. 53.9324ns per call.
vanorder::sto<uint32_t> for 10000000 calls took: 210ms. 21.0826ns per call.

     std::sto<uint64_t> for 10000000 calls took: 1249ms. 124.965ns per call.
vanorder::sto<uint64_t> for 10000000 calls took: 829ms. 82.9425ns per call.

```