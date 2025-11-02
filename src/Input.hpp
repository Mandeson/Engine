#pragma once

namespace Input {

enum class KeyState {
    kPress,
    kRelease
};

enum class PointerAction {
    kDown,
    kUp,
    kMove
};

}

namespace Input::Mouse {

enum class Button {
    kLeft,
    kRight,
    kMiddle
};

enum class ButtonState {
    kPress,
    kRelease
};

}
