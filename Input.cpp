/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley. All rights reserved.

Purpose: see header.
Date: Feb/14/2020
*/

#include "Input.h"
#include "Video.h"

ley::InputPressed::InputPressed(Uint16 sdlKeymod, auto delayTime, auto repeatTime) 
:
mDelayTimer{delayTime, {0, 0, 0, 0}},
mRepeatTimer{repeatTime, {0, 0, 0, 0}} {


}
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

ley::Command ley::Input::lookupPadCommand(const SDL_GameControllerButton scancode, PadBindingsType* bindings) {

    if(bindings->find(scancode) != bindings->end() ) {
        return bindings->at(scancode);
    }

    return ley::Command::none;
}

ley::Command ley::Input::lookupKeyCommand(const SDL_Scancode scancode, Uint16 modifiers, KeyBindingsType* bindings) {
    
    // Get range of entries for this scancode
    auto range = bindings->equal_range(scancode);
    
    // Iterate through all bindings for this key
    for (auto it = range.first; it != range.second; ++it) {
        
        //Check for commands that have no modifiers
        Uint16 unwantedMods = KMOD_SHIFT | KMOD_ALT | KMOD_CTRL;
        if(it->second.first == 0 && !(modifiers & unwantedMods)) {
            return it->second.second;
        }
        //Then check for commands with modifiers.
        else if(modifiers & it->second.first) {
            return it->second.second;
        }
    }

    return ley::Command::none;
}

/* Functions */
void ley::Input::pollEvents(
    bool& fullscreen, 
    PadBindingsType* buttonBindings2,
    KeyBindingsType* inKeyboardBindings,
    std::queue<ley::Command>* commandQueuePtr,
    ley::TextEntry* te,
    const std::function<void(ley::Command c)>& function,
    int inKeyDelay,
    int inKeyRepeat) {
    
    
    SDL_Event event;
    ley::Command command = ley::Command::none; //direction for this frame;

    // TODO it would be cool if we could come up with some token language for 
    // storing parens or and && so that we can do this key or that key or these 
    // two keys or those two keys e.g. shitching full screen. for now multiple 
    // or keys will have to suffice.



    // TODO clean this up as SDL_GetKeyboardState probably doesn't need to be called multiple times to get all the modifiers.
    auto ctrl_mod = [this]() -> bool  {

        const Uint8* keysPressed = SDL_GetKeyboardState( NULL );

        return keysPressed[SDL_SCANCODE_LCTRL] || keysPressed[SDL_SCANCODE_RCTRL];
    };

    auto alt_mod = [this]() -> bool  {

        const Uint8* keysPressed = SDL_GetKeyboardState( NULL );

        return keysPressed[SDL_SCANCODE_LALT] || keysPressed[SDL_SCANCODE_RALT];
    };

    auto shift_mod = [this]() -> bool {

        const Uint8* keysPressed = SDL_GetKeyboardState( NULL );

        return keysPressed[SDL_SCANCODE_LSHIFT] || keysPressed[SDL_SCANCODE_RSHIFT];
    };

    auto get_mod_bitmask = [this, alt_mod, shift_mod, ctrl_mod]() -> Uint16 {

        Uint16 modifiers = KMOD_NONE;

        if(alt_mod()) {
            modifiers |= KMOD_ALT;
        }

        if(ctrl_mod()) {
            modifiers |= KMOD_CTRL;
        }

        if(shift_mod()) {
            modifiers |= KMOD_SHIFT;
        }

        return modifiers;

    };

    auto check_timers = [this, commandQueuePtr, alt_mod, buttonBindings2, get_mod_bitmask, inKeyboardBindings]() {
     
        for(auto &[key, keyPress] : mKeysPressed) {

            keyPress->getDelayTimerPtr()->runFrame(false);
            keyPress->getRepeatTimerPtr()->runFrame(false);

            //if the delay timer has expired and the repeat timer has expired
            if(keyPress->getDelayTimerPtr()->hasExpired() && keyPress->getRepeatTimerPtr()->hasExpired()) {
                
                ley::Command command = lookupKeyCommand( (SDL_Scancode)key, get_mod_bitmask(), inKeyboardBindings);
                //don't repeat the enter command.
                if(command != ley::Command::enter) {
                    commandQueuePtr->push(command);
                }
                //reset the repeat timer
                keyPress->getRepeatTimerPtr()->reset();
            }
        }

        for(auto &[key, keyPress] : mButtonsPressed) {
            
            keyPress.first.runFrame(false); //run delay timer.
            keyPress.second.runFrame(false); //the repeat timer.

            if(keyPress.first.hasExpired() && keyPress.second.hasExpired()) {
                commandQueuePtr->push(lookupPadCommand((SDL_GameControllerButton)key, buttonBindings2));
                //reset the repeat timer
                keyPress.second.reset();
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
                if(te) {
                    te->onTextInput(event.text.text);
                }
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
                                        
                    SDL_Scancode pressedKey = event.key.keysym.scancode;
                    Uint16 pressedModifiers = event.key.keysym.mod;

                    //reset the timers if this key was previously not pressed 
                    if(mKeysPressed.find(pressedKey) == mKeysPressed.end()) {

                        //skip modifiers
                        if(pressedKey <= SDL_SCANCODE_LCTRL || pressedKey >= SDL_SCANCODE_RGUI)
                        { 
                            mKeysPressed.insert({pressedKey, std::make_unique<InputPressed>(pressedModifiers, inKeyDelay, inKeyRepeat)});
                            mKeysPressed[pressedKey]->getDelayTimerPtr()->reset();
                            mKeysPressed[pressedKey]->getRepeatTimerPtr()->reset();
                        }
                    }
                           
                    command = lookupKeyCommand(pressedKey, pressedModifiers, inKeyboardBindings);

                    if(command == ley::Command::fullscreen) {
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
                        mButtonsPressed.insert({buttonPressed, std::make_pair(ley::Timer(inKeyDelay, {0, 0, 0, 0}), ley::Timer(inKeyRepeat, {0, 0, 0, 0}))});

                        mButtonsPressed[buttonPressed].first.reset();
                        mButtonsPressed[buttonPressed].second.reset();
                    }

                    commandQueuePtr->push(lookupPadCommand((SDL_GameControllerButton)buttonPressed, buttonBindings2));
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
}