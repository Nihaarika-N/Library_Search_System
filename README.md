# Library_Search_System
Project submission for dsa hackathon
We implemented a dual-Trie indexing system consisting of a Prefix Trie and a Reversed Suffix Trie. 
Each node maintains insertion-order record IDs, allowing us to efficiently locate titles matching both a given prefix and suffix. 
By intersecting ID lists using a reverse two-pointer approach, we directly obtain the most recently inserted matching record. 
The solution satisfies all four deliverables: working search interface, reusable Trie-based index, deterministic reporting, and robust handling of duplicates and ambiguous matches without scanning the entire catalog for each query.
