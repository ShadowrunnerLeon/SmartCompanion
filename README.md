## Work in progress

# Description
This project is an example of a game system using voice recognition modules and computer vision. In short, there is a character that receives voice commands from a person, recognizes them, creates a screenshot, analyzes it for enemies under specified conditions (for example, searching for players wearing red clothes), calculates the angle of turn and makes a shooting. The system provides for the addition of new modules and commands.

# Libraries
- Vosk 0.3.45 (speech recognition)
- PortAudio 19.7.0 (reading data from the microphone)
- OpenCV 4.7.0 (computer vision)
- Simpleson 1.1.0 (work with json, was chosen because of its small size)

# Architecture
- Game Part - the set of classes responsible for character creation, shooting, and projectile generation. Inside SmartCompanionCharacter is a pointer to CommandHandler.
- CommandHandler is a class that implements the FRunnable interface and handles recognized commands. 
- SpeechRecognitionModule - reading data from the microphone, recognizing and parsing json.
- ComputerVisionModule - image preprocessing and postprocessing, rotation angle calculation.

# Models

- vosk-model-small-en-us-0.15
- YOLOv8s

# Useful links (add description for links)
- https://forums.unrealengine.com/t/unreal-engine-5-stuck-75-initializing/668571
- https://unrealcommunity.wiki/multithreading-with-frunnable-2a4xuf68
- https://forums.unrealengine.com/t/unresolved-external-symbol-for-custom-third-party-plugin-integrating-vosk/487496/2
- https://unrealcommunity.wiki/adding-custom-third-party-library-to-plugin-from-scratch-867b28