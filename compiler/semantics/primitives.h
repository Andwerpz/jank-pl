#pragma once

struct Type;
struct BaseType;

namespace primitives {
    inline BaseType *_void;
    inline BaseType *i8, *i16, *i32, *i64;
    inline BaseType *u8, *u16, *u32, *u64;
    inline BaseType *f32;

    void init_primitives();
}
