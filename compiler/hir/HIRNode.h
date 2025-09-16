#pragma once

namespace hir {
    struct HIRNode {
        virtual void emit_asm() = 0;
    };
}

