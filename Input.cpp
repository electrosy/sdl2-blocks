/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley. All rights reserved.

Purpose: see header.
Date: Feb/14/2020
*/
#include "Input.h"
#include "Video.h"

const auto KEY_DELAY_TIME = 250;
const auto KEY_REPEAT_TIME = 35;
 
/* RAII */
ley::Input::Input() {
    SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

    if (SDL_IsGameController(0)) {
        mControllerPtr = SDL_GameControllerOpen(0);
        if (mControllerPtr == nullptr) {
            SDL_Log("Could not open gamecontroller %i: %s\n", 0, SDL_GetError());
        }
    }

    for(Uint8 i = 0; i < UINT8_MAX; ++i) {
        mKeysPressed.insert({i, std::make_tuple(false, ley::Timer(KEY_DELAY_TIME, {0, 0, 0, 0}), ley::Timer(KEY_REPEAT_TIME, {0, 0, 0, 0}))});
    }

    for(Uint8 i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i) {
        mButtonsPressed.insert({i, std::make_tuple(false, ley::Timer(KEY_DELAY_TIME, {0, 0, 0, 0}), ley::Timer(KEY_REPEAT_TIME, {0, 0, 0, 0}))});
    }

}

ley::Input::~Input() {
    
    if (mControllerPtr != nullptr) {
        SDL_GameControllerClose(mControllerPtr);
    }
    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
}
bool ley::Input::anyInputsMatch(const Uint8 scancode, std::vector<Uint8>* inputs) {

    for (auto it = inputs->begin(); it != inputs->end(); ++it) {
        if(scancode == (*it)) {
            return true;
        }
    }

    return false;
}

bool ley::Input::anyInputsMatch(const Uint8* state, std::vector<Uint8>* inputs) {

    for (auto it = inputs->begin(); it != inputs->end(); ++it) {
        if(state[(*it)]) {
            return true;
        }
    }

    return false;
}

/*
bool ley::Input::anyInputsMatch(std::vector<Uint8>* inputs) {

    for (auto it = inputs->begin(); it != inputs->end(); ++it) {
        if(std::get<0>(mKeysPressed[(*it)])) {
            return true;
        }
    }

    return false;
}
*/

/* Functions */
ley::Command ley::Input::pollEvents(bool& fullscreen, ley::KeyBindings* bindings, std::queue<ley::Command>* commandQueuePtr, ley::TextEntry* te, const std::function<void(ley::Command c)>& function) {
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
                    commandQueuePtr->push(command);
                }
                if (state[SDL_SCANCODE_C]) {
                    command = ley::Command::debugclear;
                    commandQueuePtr->push(command);
                }
                if (state[SDL_SCANCODE_F]) {
                    command = ley::Command::debugfill;
                    commandQueuePtr->push(command);
                }
                if (anyInputsMatch(state, &bindings->quit.second)) {
                    command = bindings->quit.first;
                    commandQueuePtr->push(command);
                }
                if (state[SDL_SCANCODE_BACKSPACE]) {
                    command = ley::Command::backspace;
                    commandQueuePtr->push(command);
                }
                if (state[SDL_SCANCODE_L]) {
                    command = ley::Command::debugonlyline;
                    commandQueuePtr->push(command);
                }
                //Full screen mode
                if ((state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_RETURN])
                    ||(state[SDL_SCANCODE_RALT] && state[SDL_SCANCODE_RETURN])) { 
                        fullscreen = !fullscreen; 
                }
                if (state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_D]) {
                    command = ley::Command::debugtexture;
                    commandQueuePtr->push(command);
                }
                //Output setstate layer on the board
                if (state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_S]) {
                    command = ley::Command::debugcolide;                    
                    commandQueuePtr->push(command);
                }
                if (state[SDL_SCANCODE_GRAVE]) {
                    command = ley::Command::console;
                    commandQueuePtr->push(command);
                }
                //Rotate Block counter clockwise
                if (anyInputsMatch(state, &bindings->cclockwise.second)) {
                    command = bindings->cclockwise.first; // TODO this needs some work, main menu has cclockwise overloaded
                    commandQueuePtr->push(command);
                }
                //Rotate Block clockwise
                if (anyInputsMatch(state, &bindings->clockwise.second)) {
                    command = bindings->clockwise.first;
                    commandQueuePtr->push(command);
                }

                //move block down
                if (anyInputsMatch(state, &bindings->down.second)) {
                    command = bindings->down.first;
                    commandQueuePtr->push(command);
                }
                //move block left
                if (anyInputsMatch(state, &bindings->left.second)) {
                    command = bindings->left.first;
                    commandQueuePtr->push(command);
                }
                //move block right
                if (anyInputsMatch(state, &bindings->right.second)) {
                    command = bindings->right.first;
                    commandQueuePtr->push(command);
                }
                //play next audio music track
                if (anyInputsMatch(state, &bindings->nextSong.second)) {
                    command = bindings->nextSong.first;
                    commandQueuePtr->push(command);
                }
                //play previous audio music track
                if (anyInputsMatch(state, &bindings->previousSong.second)) {
                    command = bindings->previousSong.first;
                    commandQueuePtr->push(command);
                }

                //debug next level
                if (state[SDL_SCANCODE_I]) {
                    command = ley::Command::debugnextlevel;
                    commandQueuePtr->push(command);
                }

                //debug prev level
                if (state[SDL_SCANCODE_U]) {
                    command = ley::Command::debugprevlevel;
                    commandQueuePtr->push(command);
                }

                //pause game
                if (anyInputsMatch(state, &bindings->pause.second)) {
                    command = bindings->pause.first;
                    commandQueuePtr->push(command);
                }

                if(state[SDL_SCANCODE_RETURN] && !(state[SDL_SCANCODE_LALT] || state[SDL_SCANCODE_RALT])) {
                    command = ley::Command::enter;
                    commandQueuePtr->push(command);
                }

                if (anyInputsMatch(state, &bindings->decreaseVolume.second)) {
                    command = bindings->decreaseVolume.first;
                    commandQueuePtr->push(command);
                }

                if (anyInputsMatch(state, &bindings->increaseVolume.second)) {
                    command = bindings->increaseVolume.first;
                    commandQueuePtr->push(command);
                }

                if (anyInputsMatch(state, &bindings->space.second)) {
                    command = bindings->space.first;
                    commandQueuePtr->push(command);
                }

                if(state[SDL_SCANCODE_TAB]) {
                    command = ley::Command::tab;
                    commandQueuePtr->push(command);
                }
                
                function(command);

                break;

            case SDL_KEYUP :

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

ley::Command ley::Input::pollEvents2(bool& fullscreen, ley::KeyBindings* bindings, std::queue<ley::Command>* commandQueuePtr, ley::TextEntry* te, const std::function<void(ley::Command c)>& function) {
    SDL_Event event;
    ley::Command command = ley::Command::none; //direction for this frame;
    //std::vector<ley::Character> characters;

    // TODO it would be cool if we could come up with some token language for 
    // storing parens or and && so that we can do this key or that key or these 
    // two keys or those two keys e.g. shitching full screen. for now multiple 
    // or keys will have to suffice.

    auto alt_mod = [this]() -> bool  {
        return std::get<0>(mKeysPressed[SDL_SCANCODE_LALT]) ||  std::get<0>(mKeysPressed[SDL_SCANCODE_RALT]);
    };
    
    auto find_command = [this, bindings](Uint8 scancode, bool altPressed) -> ley::Command {
                if (anyInputsMatch(scancode, &bindings->backspace.second)) {
                    return bindings->backspace.first;
                }
                if (anyInputsMatch(scancode, &bindings->quit.second)) {
                    return bindings->quit.first;
                }
                //Rotate Block counter clockwise
                if (anyInputsMatch(scancode, &bindings->cclockwise.second)) {
                    return bindings->cclockwise.first; // TODO this needs some work, main menu has cclockwise overloaded
                }
                //Rotate Block clockwise
                if (anyInputsMatch(scancode, &bindings->clockwise.second)) {
                    return bindings->clockwise.first;
                }

                //move block down
                if (anyInputsMatch(scancode, &bindings->down.second)) {
                    return bindings->down.first;
                }
                //move block left
                if (anyInputsMatch(scancode, &bindings->left.second)) {
                    return bindings->left.first;
                }
                //move block right
                if (anyInputsMatch(scancode, &bindings->right.second)) {
                    return bindings->right.first;
                }

                if(anyInputsMatch(scancode, &bindings->enter.second) && !altPressed) {
                    return bindings->enter.first;
                }

                if(anyInputsMatch(scancode, &bindings->tab.second)) {
                    return bindings->tab.first;
                }

                /////////////////////////////////////////////////////////
                //debug clear
                if (anyInputsMatch(scancode, &bindings->debugkeystoggle.second)) {
                    return bindings->debugkeystoggle.first;
                }
                if (scancode == SDL_SCANCODE_C) {
                    return ley::Command::debugclear;
                }
                if (scancode == SDL_SCANCODE_F) {
                    return ley::Command::debugfill;
                }
               
                if (scancode == SDL_SCANCODE_L) {
                    return ley::Command::debugonlyline;
                }
                if (altPressed && scancode == SDL_SCANCODE_D) {
                    return ley::Command::debugtexture;
                }
                //Output setstate layer on the board
                if (altPressed && scancode == SDL_SCANCODE_S) {
                    return ley::Command::debugcolide;                    
                }
                if (scancode == SDL_SCANCODE_GRAVE) {
                    return ley::Command::console;
                }
                //play next audio music track
                if (anyInputsMatch(scancode, &bindings->nextSong.second)) {
                    return bindings->nextSong.first;
                }
                //play previous audio music track
                if (anyInputsMatch(scancode, &bindings->previousSong.second)) {
                    return bindings->previousSong.first;
                }

                //debug next level
                if (scancode == SDL_SCANCODE_I) {
                    return ley::Command::debugnextlevel;
                }

                //debug prev level
                if (scancode == SDL_SCANCODE_U) {
                    return ley::Command::debugprevlevel;
                }

                //pause game
                if (anyInputsMatch(scancode, &bindings->pause.second)) {
                    return bindings->pause.first;
                }

                if (anyInputsMatch(scancode, &bindings->decreaseVolume.second)) {
                    return bindings->decreaseVolume.first;
                }

                if (anyInputsMatch(scancode, &bindings->increaseVolume.second)) {
                    return bindings->increaseVolume.first;
                }

                if (anyInputsMatch(scancode, &bindings->space.second)) {
                    return bindings->space.first;
                }
                /////////////////////////////////////////////////////////

        return ley::Command::none;                
    };

/*
    auto push_commands = [this, bindings, commandQueuePtr, &command]() {

                if (anyInputsMatch(&bindings->backspace.second)) {
                    command = bindings->backspace.first;
                    commandQueuePtr->push(command);
                }
                if (anyInputsMatch(&bindings->quit.second)) {
                    command = bindings->quit.first;
                    commandQueuePtr->push(command);
                }
                
                //Rotate Block counter clockwise
                if (anyInputsMatch(&bindings->cclockwise.second)) {
                    command = bindings->cclockwise.first; // TODO this needs some work, main menu has cclockwise overloaded
                    commandQueuePtr->push(command);
                }
                //Rotate Block clockwise
                if (anyInputsMatch(&bindings->clockwise.second)) {
                    command = bindings->clockwise.first;
                    commandQueuePtr->push(command);
                }

                //move block down
                if (anyInputsMatch(&bindings->down.second)) {
                    command = bindings->down.first;
                    commandQueuePtr->push(command);
                }
                //move block left
                if (anyInputsMatch(&bindings->left.second)) {
                    command = bindings->left.first;
                    commandQueuePtr->push(command);
                }
                //move block right
                if (anyInputsMatch(&bindings->right.second)) {
                    command = bindings->right.first;
                    commandQueuePtr->push(command);
                }

                if(anyInputsMatch(&bindings->enter.second) && !(anyInputsMatch(&bindings->alt.second))) {
                    command = bindings->enter.first;
                    commandQueuePtr->push(command);
                }

                if(anyInputsMatch(&bindings->tab.second)) {
                    command = bindings->tab.first;
                    commandQueuePtr->push(command);
                }
                
    };
*/

    auto check_timers = [this, commandQueuePtr, find_command, alt_mod]() {
        
        for(Uint8 i = 0; i < UINT8_MAX; ++i) {
            
            if (std::get<0>(mKeysPressed[i]) == true) {
                //Run all input timers
                std::get<1>(mKeysPressed[i]).runFrame(false); //run delay timer.
                std::get<2>(mKeysPressed[i]).runFrame(false); //the repeat timer.
            
                //if the delay timer has expired and the repeat timer has expired
                if(std::get<1>(mKeysPressed[i]).hasExpired() && std::get<2>(mKeysPressed[i]).hasExpired()) {
                    commandQueuePtr->push(find_command(i, alt_mod()));
                    
                    //reset the repeat timer
                    std::get<2>(mKeysPressed[i]).reset();
                }
            }
        }
    };

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
                
                if(event.key.repeat == 0) {
                                        
                    Uint8 pressedKey = event.key.keysym.scancode;
                    //reset the timers if this key was previously not pressed
                    if(std::get<0>(mKeysPressed[pressedKey]) == false) 
                    {
                        std::get<1>(mKeysPressed[pressedKey]).reset(); //reset the delay timer.
                        std::get<2>(mKeysPressed[pressedKey]).reset(); //reset the repeat timer.
                    }
                    std::get<0>(mKeysPressed[pressedKey]) = true;

                    command = find_command(pressedKey, alt_mod());
                    commandQueuePtr->push(command);

                    if ((state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_RETURN])
                        ||(state[SDL_SCANCODE_RALT] && state[SDL_SCANCODE_RETURN])) { 
                            fullscreen = !fullscreen;
                    }

                }

                if(te->hasFocus()) {
                    function(command);
                }

                break;

            case SDL_KEYUP :
                {
                    Uint8 releasedKey = event.key.keysym.scancode;
                    std::get<0>(mKeysPressed[releasedKey]) = false;
                    std::get<1>(mKeysPressed[releasedKey]).reset(); //reset the delay timer, just for brevity.
                    std::get<2>(mKeysPressed[releasedKey]).reset(); //reset the repeat timer, just for brevity.
                }
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
                    default:
                        command = ley::Command::none;
                        break;
                    // Add more cases for other buttons... 
                }

                commandQueuePtr->push(command);
                SDL_Log("Controller was pressed");
                break;

            case SDL_CONTROLLERBUTTONUP:
                break;

            default:
            break;
        }
    }

    check_timers();

    return command;
}