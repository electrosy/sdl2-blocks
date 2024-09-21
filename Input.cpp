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

ley::Command ley::Input::lookupCommand(const Uint8 scancode, std::map<Uint8, ley::Command>* bindings) {

    if(bindings->find(scancode) != bindings->end()) {
        return bindings->at(scancode);
    }

    return ley::Command::none;
}

ley::Command ley::Input::lookupButton(const Uint8 scancode, std::map<Uint8, ley::Command>* buttonBindings) {

    if(buttonBindings->find(scancode) != buttonBindings->end()) {
        return buttonBindings->at(scancode);
    }

    return ley::Command::none;
}

/* Functions */
ley::Command ley::Input::pollEvents(bool& fullscreen, std::map<Uint8, ley::Command>* buttonBindings2, std::map<Uint8, ley::Command>* bindings2, std::queue<ley::Command>* commandQueuePtr, ley::TextEntry* te, const std::function<void(ley::Command c)>& function) {
    SDL_Event event;
    ley::Command command = ley::Command::none; //direction for this frame;

    // TODO it would be cool if we could come up with some token language for 
    // storing parens or and && so that we can do this key or that key or these 
    // two keys or those two keys e.g. shitching full screen. for now multiple 
    // or keys will have to suffice.

    auto alt_mod = [this]() -> bool  {
        return std::get<0>(mKeysPressed[SDL_SCANCODE_LALT]) ||  std::get<0>(mKeysPressed[SDL_SCANCODE_RALT]);
    };

    auto find_button2 = [this, buttonBindings2](Uint8 button) -> ley::Command {
        return lookupCommand(button, buttonBindings2);
    };

    auto find_command2 = [this, bindings2](Uint8 scancode, bool altPressed) -> ley::Command {
        
        ley::Command command = lookupCommand(scancode, bindings2);

        if(command == ley::Command::enter && altPressed) {
            //we want to do the enter command only if alt is not pressed.
            command = ley::Command::none;
        }

        if(command == ley::Command::debugtexture && !altPressed) {
            //requires alt key
            command = ley::Command::none;
        }

        if(command == ley::Command::debugcolide && !altPressed) {
            //requires alt key
            command = ley::Command::none;
        }

        return command;
    };

    auto check_timers = [this, commandQueuePtr, find_command2, find_button2, alt_mod]() {
        
        for(Uint8 i = 0; i < UINT8_MAX; ++i) {
            if (std::get<0>(mKeysPressed[i]) == true) {
                //Run all input timers
                std::get<1>(mKeysPressed[i]).runFrame(false); //run delay timer.
                std::get<2>(mKeysPressed[i]).runFrame(false); //the repeat timer.
            
                //if the delay timer has expired and the repeat timer has expired
                if(std::get<1>(mKeysPressed[i]).hasExpired() && std::get<2>(mKeysPressed[i]).hasExpired()) {
                    commandQueuePtr->push(find_command2(i, alt_mod()));
                    
                    //reset the repeat timer
                    std::get<2>(mKeysPressed[i]).reset();
                }
            }
        }

        for(Uint8 i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i) {
            if (std::get<0>(mButtonsPressed[i]) == true) {
                //Run all button timers
                std::get<1>(mButtonsPressed[i]).runFrame(false); //run delay timer.
                std::get<2>(mButtonsPressed[i]).runFrame(false); //the repeat timer.
            
                //if the delay timer has expired and the repeat timer has expired
                if(std::get<1>(mButtonsPressed[i]).hasExpired() && std::get<2>(mButtonsPressed[i]).hasExpired()) {
                    commandQueuePtr->push(find_button2(i));
                    
                    //reset the repeat timer
                    std::get<2>(mButtonsPressed[i]).reset();
                }
            }
        }
    };

    while(SDL_PollEvent(&event))   {    //SDL_PollEvent calls pumpevents.
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

                    command = find_command2(pressedKey, alt_mod());
                    commandQueuePtr->push(command);

                    if ((std::get<0>(mKeysPressed[SDL_SCANCODE_LALT]) && std::get<0>(mKeysPressed[SDL_SCANCODE_RETURN]))
                        ||(std::get<0>(mKeysPressed[SDL_SCANCODE_RALT]) && std::get<0>(mKeysPressed[SDL_SCANCODE_RETURN]))) { 
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
                {
                    Uint8 buttonPressed = event.cbutton.button;

                    //reset the timers if this key was previously not pressed
                    if(std::get<0>(mButtonsPressed[buttonPressed]) == false) 
                    {
                        std::get<1>(mButtonsPressed[buttonPressed]).reset(); //reset the delay timer.
                        std::get<2>(mButtonsPressed[buttonPressed]).reset(); //reset the repeat timer.
                    }
                    std::get<0>(mButtonsPressed[buttonPressed]) = true;

                    commandQueuePtr->push(find_button2(buttonPressed));

                }
                SDL_Log("Controller was pressed");
                break;

            case SDL_CONTROLLERBUTTONUP:
                {
                    Uint8 buttonReleased = event.cbutton.button;
                    std::get<0>(mButtonsPressed[buttonReleased]) = false;
                    std::get<1>(mButtonsPressed[buttonReleased]).reset(); //reset the delay timer, just for brevity.
                    std::get<2>(mButtonsPressed[buttonReleased]).reset(); //reset the repeat timer, just for brevity.
                }

                break;

            default:
            break;
        }
    }

    check_timers();

    return command;
}