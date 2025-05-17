.section .text
.global malloc

# simple bump allocator
# just increases the heap size every time we want to allocate more memory.

malloc:
    