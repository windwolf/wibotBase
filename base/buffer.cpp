#include "buffer.hpp"

namespace wibot {
Buffer8Setter Buffer8::createSetter() {
    return Buffer8Setter(*this);
}
}  // namespace wibot
