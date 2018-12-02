# Von Neumann machine emulator

### Building
```
$ cmake .
$ make
```

#### Building on Windows with MinGW
```
$ cmake -G "MinGW Makefiles"
$ mingw32-make
```

### Usage
```
$ ./von-neumann [OPTION...]
```
```
  -h, --help      Show help
  -f, --file arg  Path to the VNM program file
  -s, --save      Save output to file
  -r, --register  Print register values before every cycle
  -m, --memory    Print memory before every cycle
```
**Example:**
```
$ ./von-neumann -rmf example/array_sum.vnm
```
### Machine properties
* **Memory** of 512 16-bit words, addressed from 0 to 511.

* **Four 16-bit registers**
  - Accumulator (`AC`)
  - Operand register (`OR`)
  - Program counter (`PC`)
  - Instruction register (`IR`)

### Available instructions
* **`STOP `**, code: `0000`, stops the machine
* **`LOAD `**, code: `0001`, operation: `AC = OR`
* **`STORE`**, code: `0010`, operation: `MEM[ OR ] = AC`
* **`JUMP `**, code: `0011`, operation: `PC = OR`
* **`JNEG `**, code: `0100`, operation: `AC < 0 => PC = OR`
* **`JZERO`**, code: `0101`, operation: `AC == 0 => PC = OR`
* **`ADD  `**, code: `0110`, operation: `AC = AC + OR`
* **`SUB  `**, code: `0111`, operation: `AC = AC - OR`
* **`MULT`**, code: `1000`, operation: `AC = AC * OR`
* **`DIV  `**, code: `1001`, operation: `AC = AC / OR`
* **`AND  `**, code: `1010`, operation: `AC = AC & OR`
* **`OR   `**, code: `1011`, operation: `AC = AC | OR`
* **`NOT  `**, code: `1100`, operation: `AC = !OR`
* **`CMP  `**, code: `1101`, operation: `AC == OR => AC = -1`,`AC != OR => AC = 0`
* **`SHZ  `**, code: `1110`, operation: `OR < 0 => AC >> |OR|`, `OR > 0 => AC << |OR|`
* **`SHC  `**, code: `1111`, circular shift left or right, depending on the sign of the operand

### Available addressing modes
* **Instant**, code: `00`, `$`, operation: `OR = IR.arg`
* **Direct**, code: `01`, `@`, operation: `OR = MEM[ IR.arg ]`
* **Indirect**, code: `10`, `&`, operation: `OR = MEM[ MEM[ IR.arg ] ]`
* **Index**, code: `11`, `+`, operation: `OR = MEM[ AC + IR.arg ]`

### Word interpretation
* 16 bits
  - Most significant bit - farthest to the left left, unused
  - Next four bits - instruction code (`IR.code`)
  - Next two bits - addressing mode code (`IR.mode`)
  - Remaining nine bits - instruction argument (`IR.arg`)

### Examples

##### Doubling the sum of a pair of numbers
```asm
0. LOAD  @ 5 ; instructions begin
1. ADD   @ 6 ;
2. MULT  $ 2 ;
3. STORE $ 7 ;
4. STOP      ; instructions end
5. 21        ; first data
6. 34        ; second data
7. 0         ; result
```

##### Summation of the array with indirect addressing
```asm
0.  LOAD  $ 0
1.  STORE $ 19
2.  LOAD  $ 21
3.  STORE $ 18
4.  LOAD  $ 21
5.  ADD   @ 20
6.  SUB   @ 18
7.  JZERO $ 15
8.  LOAD  @ 19
9.  ADD   & 18
10. STORE $ 19
11. LOAD  @ 18
12. ADD   $ 1
13. STORE $ 18
14. JUMP  $ 4
15. STOP
16. 
17. 
18.     ; auxiliary memory
19.     ; result
20. 5   ; number of elements in sequence: n
21. 10  ; first element:  array[ 0 ]
22. 20  ; second element: array[ 1 ]
23. 30  ; ...
24. 40  ; ...
25. 50  ; last element:   array[ n - 1 ]

```

### Reference

Machine specification was based on `Przykładowa maszyna cyfrowa (PMC)` as defined in the book below.

Kawa, R., & Lembas, J. (2017). "Wykłady z informatyki: Wstęp do informatyki". Warszawa: Wydawnictwo Naukowe PWN.
