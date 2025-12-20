# **<p align="center">MINON: A Compiler Behavior Simulator for Minimized Object Notation (JSON)**

<p align="center">Gerlie Campion, Joeniño Cainday, Hannah May Gentrolizo, Jhey Gulde, Junar Landicho, Kynehl Scott Misajon

<p align="center">Department of  Computer Science, University of Science and Technology of Southern Philippines, C.M. Recto Avenue, Lapasan, Cagayan de Oro City, 9000, Philippines

# 1. INTRODUCTION
  JSON (JavaScript Object Notation) is a lightweight, text-based data format used to represent structured data as key–value pairs and arrays. It’s language-independent, easy for humans to read/write, and widely used for APIs, configuration files, and data interchange. With that, the proposed compiler outputs a minimized (minified) form of JSON—meaning it removes all unnecessary whitespace and line breaks so the entire payload is on one line. This helps reduce file size and memory overhead, which can improve storage efficiency and transmission speed.

# 2. REGULAR EXPRESSION
  This regular expression is designed to identify and tokenize key elements commonly found in JSON-like data. It matches structural characters such as braces, brackets, colons, and commas, which define the overall structure of the data. It also detects string literals by capturing sequences of characters enclosed in double quotation marks, allowing textual values to be recognized accurately. In addition, the expression matches integer values, including optional negative numbers, enabling the identification of numeric data. Overall, the regex serves as a lightweight tokenizer for parsing and analyzing the basic components of JSON-formatted input.

<img width="853" height="94" alt="image" src="https://github.com/user-attachments/assets/315a81d1-1f88-4460-b710-77432b8b6c2b" />





<p align="center">Figure 1: Regular Expression of the Minimized JSON Compiler

# 3. SCANNING PHASE: Lexical Analysis
	
## 3.1 Regex to Epsilon Non-Deterministic Finite Automata
  In the proposed compiler, the regular expression is transformed into an ε-NFA (epsilon-Nondeterministic Finite Automaton) using Thompson’s Construction algorithm. This method systematically constructs the NFA by decomposing the regular expression into smaller components and linking them through ε-transitions (epsilon moves). The resulting automaton preserves the language of the original regular expression, providing a formal foundation for efficient pattern matching and lexical analysis.




<img width="1123" height="663" alt="image" src="https://github.com/user-attachments/assets/9f04c955-7c11-4a16-a370-4af8da4aef69" />










             
<p align="center">Figure 2: Epsilon Non-deterministic Finite Automata
	
## 3.2 Regex to Non-Deterministic Finite Automata
  In the compiler, an NFA (Nondeterministic Finite Automaton) is employed to define how tokens are recognized during lexical analysis. Rather than using ε-moves, the NFA relies on direct transitions: punctuation is accepted through simple loops, numbers follow a path that reads digits, and strings progress to a state that reads characters until a closing quote is encountered. Its nondeterministic nature allows the NFA to explore multiple paths simultaneously, providing a straightforward and flexible means of representing the token patterns used by the lexer.




<img width="1380" height="592" alt="image" src="https://github.com/user-attachments/assets/37c6948a-7f85-492d-91e7-f1c2e8df83ea" />












<p align="center">Figure 3: Non-deterministic Finite Automata

## 3.3 Regex to Deterministic Finite Automata
  After converting the regular expression into a Nondeterministic Finite Automaton (NFA), the subset construction algorithm is applied to transform it into a Deterministic Finite Automaton (DFA), effectively eliminating all epsilon and nondeterministic transitions. The resulting DFA serves as the deterministic recognition model for the lexical analyzer, preserving the language accepted by the original NFA while providing a clear and explicit representation of state transitions suitable for efficient compiler implementation.

	



<img width="1269" height="614" alt="image" src="https://github.com/user-attachments/assets/2eaec81c-61c6-4de3-81b1-3003599e726c" />
















<p align="center">Figure 4: Deterministic Finite Automata


## 3.4 Regex to Mealy Machine
  In addition to the recommended diagrams, a Lexical-to-Mealy machine diagram has been included to illustrate the resulting states of all lexical inputs relative to their current states. This diagram provides a high-level overview of how the Minimized Object Notation Compiler processes each input within a file and maps it to the appropriate token type (struct, string, or digit), offering a clear visualization of the lexical analysis workflow. By explicitly showing the state transitions for each input, the diagram highlights the systematic approach used to recognize tokens, resolve ambiguities, and ensure accurate classification. Furthermore, it serves as a valuable tool for understanding the compiler's internal mechanics, supporting debugging, optimization, and further development of the lexical analysis module.



	

<img width="1365" height="719" alt="image" src="https://github.com/user-attachments/assets/34d12f71-c37c-47e5-86d3-e3df3afcbe64" />












<p align="center">Figure 5: Mealy Machine for Tokenization

# 4. PARSING PHASE: Syntax Analysis
	
## 4.1 Non-Deterministic Pushdown Automata
  Lastly, the researchers have also added a Syntax parser to Non-deterministic Pushdown automata diagram as a more comprehensive overview of the previous Mealy machine diagram. The diagram visualizes the needed inputs for each transition, with the initial input being a struct, followed by all of the possible inputs that it can take, and ending in another struct resulting in an empty stack once the final struct has been added. 










<img width="1366" height="706" alt="image" src="https://github.com/user-attachments/assets/fe3c6f9e-ade0-4436-8d42-2b01f6aa1c8c" />








<p align="center">Figure 6: Non-Deterministic Pushdown Automata for Structural Analysis


## 4.2 Context-Free Grammar
  This context-free grammar describes a simplified version of the JSON data format by formally defining how valid JSON values are constructed. The grammar specifies that a JSON document consists of a single value, which may be an object, an array, or a literal. Objects are enclosed in curly braces and contain zero or more members, where each member is a key–value pair composed of a string key followed by a colon and a value. Arrays are enclosed in square brackets and contain zero or more elements, each of which is itself a value, allowing for nested structures. Literals are limited to string and number values. Through recursive rules for members and elements, the grammar supports comma-separated lists and arbitrary nesting of objects and arrays. Overall, this grammar generates all well-formed JSON structures within its defined scope, making it suitable for illustrating JSON syntax in formal language and compiler theory contexts.

## CFG OF THE MINIMIZED JSON COMPILER
1. JSON → Value

2. Value → Object
                   | Array
                   | Literal

3. Object → '{' Members '}'
4. Object → '{' '}'

5. Members → Pair
6. Members → Pair ',' Members

7. Pair → STRING ':' Value

8. Array → '[' Elements ']'
9. Array → '[' ']'

10. Elements → Value
11. Elements → Value ',' Elements

12. Literal → STRING
13. Literal → NUMBER

Table 1: Context-Free Grammar of the Minimized JSON Compiler



References

[1] Erickson, J. (2024, April 4). What is JSON? https://www.oracle.com/asean/database/what-is-json/ 
[2] JSON. (n.d.). https://www.json.org/json-en.html 



