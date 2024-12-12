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
}

ley::Input::~Input() {
    
    if (mControllerPtr != nullptr) {
        SDL_GameControllerClose(mControllerPtr);
    }
    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
}

ley::Command ley::Input::lookupCommand(const Uint8 scancode, std::map<Uint8, ley::Command>* bindings) {

    if(bindings->find(scancode) != bindings->end()) {
        return bindings->at(scancode);
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

        const Uint8* keysPressed = SDL_GetKeyboardState( NULL );

        return keysPressed[SDL_SCANCODE_LALT] || keysPressed[SDL_SCANCODE_RALT];
    };

    auto check_timers = [this, commandQueuePtr, alt_mod, bindings2, buttonBindings2]() {
     
        for(auto &key : mKeysPressed) {
            
            std::get<0>(key.second).runFrame(false);
            std::get<1>(key.second).runFrame(false);

            //if the delay timer has expired and the repeat timer has expired
            if(std::get<0>(key.second).hasExpired() && std::get<1>(key.second).hasExpired()) {
                
                ley::Command command = lookupCommand(key.first, bindings2);
                //don't repeat the enter button.
                if(command != ley::Command::enter) {
                    commandQueuePtr->push(command);
                }
                //reset the repeat timer
                std::get<1>(key.second).reset();
            }
        }

        for(auto &button : mButtonsPressed) {
            
            std::get<1>(button.second).runFrame(false); //run delay timer.
            std::get<2>(button.second).runFrame(false); //the repeat timer.

            if(std::get<1>(button.second).hasExpired() && std::get<2>(button.second).hasExpired()) {
                commandQueuePtr->push(lookupCommand(button.first, buttonBindings2));
                //reset the repeat timer
                std::get<2>(button.second).reset();
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
                
                if(!event.key.repeat) {
                                        
                    Uint8 pressedKey = event.key.keysym.scancode;
                    //reset the timers if this key was previously not pressed 

                    if(mKeysPressed.find(pressedKey) == mKeysPressed.end()) {

                        //skip modifiers
                        if(pressedKey <= SDL_SCANCODE_LCTRL || pressedKey >= SDL_SCANCODE_RGUI)
                        {
                            mKeysPressed.insert({pressedKey, std::make_tuple(ley::Timer(KEY_DELAY_TIME, {0, 0, 0, 0}), ley::Timer(KEY_REPEAT_TIME, {0, 0, 0, 0}))});

                            std::get<0>(mKeysPressed[pressedKey]).reset();
                            std::get<1>(mKeysPressed[pressedKey]).reset();
                        }
                    }
                    
                    command = lookupCommand(pressedKey, bindings2);

                    if(command == ley::Command::enter && alt_mod()) {
                         fullscreen = !fullscreen;
                    }
                    else {
                        // push on repeatable commands
                        commandQueuePtr->push(command);
                    }
                }

                // TODO how to add repeat key for delete for text edit?
                // If there is an active UI element then handle input.
                if(te && te->hasFocus()) {
                    function(command);
                }
                

                break;

            case SDL_KEYUP :
                {
                    Uint8 releasedKey = event.key.keysym.scancode;                    
                    mKeysPressed.erase(releasedKey);
                }
                break;

            case SDL_CONTROLLERBUTTONDOWN :
                {
                    Uint8 buttonPressed = event.cbutton.button;

                    if(mButtonsPressed.find(buttonPressed) == mButtonsPressed.end()) {
                        mButtonsPressed.insert({buttonPressed, std::make_tuple(true, ley::Timer(KEY_DELAY_TIME, {0, 0, 0, 0}), ley::Timer(KEY_REPEAT_TIME, {0, 0, 0, 0}))});

                        std::get<1>(mButtonsPressed[buttonPressed]).reset();
                        std::get<2>(mButtonsPressed[buttonPressed]).reset();
                    }

                    commandQueuePtr->push(lookupCommand(buttonPressed, buttonBindings2));

                }
                break;

            case SDL_CONTROLLERBUTTONUP:
                {
                    Uint8 buttonReleased = event.cbutton.button;
                    mButtonsPressed.erase(buttonReleased);
                }
                break;

            default:
            break;
        }
    }

    check_timers();

    return command;
}