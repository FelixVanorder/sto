# sto library

"String TO" — C++ parsing library for numeric types.

## Rationale

It is written as replacement for standard library numeric parsers `std::sto(i|l|ll|ul|ull|f|d|ld)` and allow automatic selection of correct parsing function for types like `uint32_t`, where you have no portable guarantee of what fundamental type represent it.

Additionally standard parsers have complicated interface, behaviour and implying unavoidable overhead:
 * returning successful result when meet nondigit character, so caller need to check if this is end of string when format require strictly numbers
 * can depends on current C locale
 * unsigned parsers convert negative numbers to unsigned form without explicit error
 * allows to setup baseness of number representation, when most of the time decimal and hexadecimal (and rarely octal) is used, and even autodetection of number base.

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

On my system:
```
     std::sto<int8_t> for 10000000 calls took: 328ms. 32.8565ns per call.
vanorder::sto<int8_t> for 10000000 calls took: 104ms. 10.4836ns per call.

     std::sto<int16_t> for 10000000 calls took: 382ms. 38.2171ns per call.
vanorder::sto<int16_t> for 10000000 calls took: 153ms. 15.3396ns per call.

     std::sto<int32_t> for 10000000 calls took: 486ms. 48.6525ns per call.
vanorder::sto<int32_t> for 10000000 calls took: 228ms. 22.8679ns per call.

     std::sto<int64_t> for 10000000 calls took: 676ms. 67.676ns per call.
vanorder::sto<int64_t> for 10000000 calls took: 390ms. 39.0338ns per call.

     std::sto<uint8_t> for 10000000 calls took: 325ms. 32.5209ns per call.
vanorder::sto<uint8_t> for 10000000 calls took: 94ms. 9.45724ns per call.

     std::sto<uint16_t> for 10000000 calls took: 364ms. 36.4621ns per call.
vanorder::sto<uint16_t> for 10000000 calls took: 132ms. 13.2965ns per call.

     std::sto<uint32_t> for 10000000 calls took: 518ms. 51.8971ns per call.
vanorder::sto<uint32_t> for 10000000 calls took: 192ms. 19.2719ns per call.

     std::sto<uint64_t> for 10000000 calls took: 786ms. 78.6062ns per call.
vanorder::sto<uint64_t> for 10000000 calls took: 359ms. 35.9652ns per call.


```