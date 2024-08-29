/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley. All rights reserved.

Purpose: see header.
Date: Feb/14/2020
*/
#include "Input.h"
#include "Video.h"

/* RAII */
ley::Input::Input() {
    SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

    if (SDL_IsGameController(0)) {
        mControllerPtr = SDL_GameControllerOpen(0);
        if (mControllerPtr == nullptr) {
            SDL_Log("Could not open gamecontroller %i: %s\n", 0, SDL_GetError());
        }
    }
}

ley::Input::~Input() {
    
    if (mControllerPtr != nullptr) {
        SDL_GameControllerClose(mControllerPtr);
    }
    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);

}
bool ley::Input::anyInputsMatch(const Uint8* state, std::vector<Uint8>* inputs) {

    for (auto it = inputs->begin(); it != inputs->end(); ++it) {
        if(state[(*it)]) {
            return true;
        }
    }

    return false;
}

/* Functions */
ley::Command ley::Input::pollEvents(bool& fullscreen, ley::KeyBindings* bindings, ley::TextEntry* te, const std::function<void(ley::Command c)>& function) {
    SDL_Event event;
    ley::Command command = ley::Command::none; //direction for this frame;
    //std::vector<ley::Character> characters;

    // TODO it would be cool if we could come up with some token language for 
    // storing parens or and && so that we can do this key or that key or these 
    // two keys or those two keys e.g. shitching full screen. for now multiple 
    // or keys will have to suffice.

    while(SDL_PollEvent(&event))   {    //SDL_PollEvent calls pumpevents.
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        switch (event.type)     {       
            case SDL_QUIT:         
                command = ley::Command::quit;
                break;

            case SDL_TEXTINPUT:
                /* Add new text onto the end of our text */
                te->onTextInput(event.text.text);
                break;
            case SDL_TEXTEDITING:
                /*
                Update the composition text.
                Update the cursor position.
                Update the selection length (if any).
                */
                //(*ptr_s) = event.edit.text;
                //cursor = event.edit.start;
                //selection_len = event.edit.length;
                break;
            case SDL_KEYDOWN:
                //debug clear
                if (anyInputsMatch(state, &bindings->debugkeystoggle.second)) {
                    command = bindings->debugkeystoggle.first;
                    SDL_Log(SDL_GetScancodeName(SDL_SCANCODE_F12));
                }
                if (state[SDL_SCANCODE_C]) {
                    command = ley::Command::debugclear;
                }
                if (state[SDL_SCANCODE_F]) {
                    command = ley::Command::debugfill;
                }
                if (anyInputsMatch(state, &bindings->quit.second)) {
                    command = bindings->quit.first;
                }
                if (state[SDL_SCANCODE_BACKSPACE]) {
                    command = ley::Command::backspace;
                }
                if (state[SDL_SCANCODE_L]) {
                    command = ley::Command::debugonlyline;
                }
                //Full screen mode
                if ((state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_RETURN])
                    ||(state[SDL_SCANCODE_RALT] && state[SDL_SCANCODE_RETURN])) { 
                        fullscreen = !fullscreen; 
                }
                if (state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_D]) {
                    command = ley::Command::debugtexture;
                }
                //Output setstate layer on the board
                if (state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_S]) {
                    command = ley::Command::debugcolide;                    
                }
                if (state[SDL_SCANCODE_GRAVE]) {
                    command = ley::Command::console;
                }
                //Rotate Block counter clockwise
                if (anyInputsMatch(state, &bindings->cclockwise.second)) {
                    command = bindings->cclockwise.first; // TODO this needs some work, main menu has cclockwise overloaded
                }
                //Rotate Block clockwise
                if (anyInputsMatch(state, &bindings->clockwise.second)) {
                    command = bindings->clockwise.first;
                }

                //move block down
                if (anyInputsMatch(state, &bindings->down.second)) {
                    command = bindings->down.first;
                }
                //move block left
                if (anyInputsMatch(state, &bindings->left.second)) {
                    command = bindings->left.first;
                }
                //move block right
                if (anyInputsMatch(state, &bindings->right.second)) {
                    command = bindings->right.first;
                }
                //play next audio music track
                if (anyInputsMatch(state, &bindings->nextSong.second)) {
                    command = bindings->nextSong.first;
                }
                //play previous audio music track
                if (anyInputsMatch(state, &bindings->previousSong.second)) {
                    command = bindings->previousSong.first;
                }

                //debug next level
                if (state[SDL_SCANCODE_I]) {
                    command = ley::Command::debugnextlevel;
                }

                //debug prev level
                if (state[SDL_SCANCODE_U]) {
                    command = ley::Command::debugprevlevel;
                }

                //pause game
                if (anyInputsMatch(state, &bindings->pause.second)) {
                    command = bindings->pause.first;
                }

                if(state[SDL_SCANCODE_RETURN] && !(state[SDL_SCANCODE_LALT] || state[SDL_SCANCODE_RALT])) {
                    command = ley::Command::enter;
                }

                if (anyInputsMatch(state, &bindings->decreaseVolume.second)) {
                    command = bindings->decreaseVolume.first;
                }

                if (anyInputsMatch(state, &bindings->increaseVolume.second)) {
                    command = bindings->increaseVolume.first;
                }

                if (anyInputsMatch(state, &bindings->space.second)) {
                    command = bindings->space.first;
                }

                if(state[SDL_SCANCODE_TAB]) {
                    command = ley::Command::tab;
                }
                
                function(command);

                break;

            case SDL_CONTROLLERBUTTONDOWN :

                switch (event.cbutton.button) {
                    case SDL_CONTROLLER_BUTTON_A:
                        command = ley::Command::enter;
                        break;
                    case SDL_CONTROLLER_BUTTON_START:
                        command = ley::Command::pause;
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                        command = ley::Command::left;
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                        command = ley::Command::right;
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                        command = ley::Command::down;
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_UP:
                        command = ley::Command::cclockwise;
                        break;
                    case SDL_CONTROLLER_BUTTON_X:
                        command = ley::Command::clockwise;
                        break;
                    case SDL_CONTROLLER_BUTTON_Y:
                        command = ley::Command::cclockwise;
                        break;
                    case SDL_CONTROLLER_BUTTON_B:
                        command = ley::Command::space;
                        break;
                    case SDL_CONTROLLER_BUTTON_BACK:
                        command = ley::Command::quit;
                        break;
                    // Add more cases for other buttons...
                }
                SDL_Log("Controller was pressed");
                break;

            default:
            break;
        }
    }

    return command;
}