/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloSDLDevice.h"
#include "SoloDevice.h"

namespace solo
{
    namespace sdl
    {
        static umap<SDL_Keycode, KeyCode> keymap =
        {
            {SDLK_0, KeyCode::Digit0},
            {SDLK_1, KeyCode::Digit1},
            {SDLK_2, KeyCode::Digit2},
            {SDLK_3, KeyCode::Digit3},
            {SDLK_4, KeyCode::Digit4},
            {SDLK_5, KeyCode::Digit5},
            {SDLK_6, KeyCode::Digit6},
            {SDLK_7, KeyCode::Digit7},
            {SDLK_8, KeyCode::Digit8},
            {SDLK_9, KeyCode::Digit9},

            {SDLK_q, KeyCode::Q},
            {SDLK_w, KeyCode::W},
            {SDLK_e, KeyCode::E},
            {SDLK_r, KeyCode::R},
            {SDLK_t, KeyCode::T},
            {SDLK_y, KeyCode::Y},
            {SDLK_u, KeyCode::U},
            {SDLK_i, KeyCode::I},
            {SDLK_o, KeyCode::O},
            {SDLK_p, KeyCode::P},

            {SDLK_a, KeyCode::A},
            {SDLK_s, KeyCode::S},
            {SDLK_d, KeyCode::D},
            {SDLK_f, KeyCode::F},
            {SDLK_g, KeyCode::G},
            {SDLK_h, KeyCode::H},
            {SDLK_j, KeyCode::J},
            {SDLK_k, KeyCode::K},
            {SDLK_l, KeyCode::L},

            {SDLK_z, KeyCode::Z},
            {SDLK_x, KeyCode::X},
            {SDLK_c, KeyCode::C},
            {SDLK_v, KeyCode::V},
            {SDLK_b, KeyCode::B},
            {SDLK_n, KeyCode::N},
            {SDLK_m, KeyCode::M},

            {SDLK_LEFT, KeyCode::LeftArrow},
            {SDLK_RIGHT, KeyCode::RightArrow},
            {SDLK_UP, KeyCode::UpArrow},
            {SDLK_DOWN, KeyCode::DownArrow},
            {SDLK_ESCAPE, KeyCode::Escape},

            {SDLK_SPACE, KeyCode::Space},
            {SDLK_BACKSPACE, KeyCode::Backspace},
            {SDLK_RETURN, KeyCode::Return},

            {SDLK_LSHIFT, KeyCode::LeftShift},
            {SDLK_LALT, KeyCode::LeftAlt},
            {SDLK_LCTRL, KeyCode::LeftCtrl},
            {SDLK_RSHIFT, KeyCode::RightShift},
            {SDLK_RALT, KeyCode::RightAlt},
            {SDLK_RCTRL, KeyCode::RightCtrl}

            // ...
        };

        static umap<Uint8, MouseButton> mouseButtonsMap =
        {
            {SDL_BUTTON_LEFT, MouseButton::Left},
            {SDL_BUTTON_MIDDLE, MouseButton::Middle},
            {SDL_BUTTON_RIGHT, MouseButton::Right},
        };
    }
}