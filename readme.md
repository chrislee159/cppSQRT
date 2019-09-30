# CPUSQRT

Benchmarks the difference between five different implementations of square root functions

1. FPU fsqrt        (Our first attempt at trying to implement the fastest square root)
2. libc sqrt        (The standard library's implementation of the square root)
3. sqrtsd           (Our second attempt at trying to implement the fastest square root)
4. [Intrinsic sqrtsd](https://en.wikipedia.org/wiki/intrinsic_function) (GCC's intrinsic function)
5. inline sqrts      (sqrtsd but inline)

[Spreadsheet speed data for the five implementations of the squre root](https://docs.google.com/spreadsheets/d/1e0MTRu92CG3CFMIkh_iwIV1wPGlA4E1of4ieYeFI9zI/edit?usp=sharing)

Please hit us up with a faster implementation than inline sqrts; we would be exhilarated.
