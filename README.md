## Work in progress (speech recognition and image processing works)

# Targets
- add cross-platform (for now Windows)

# Description
This project is an example of a game system using voice recognition modules and computer vision. In short, there is a character that receives voice commands from a person, recognizes them, creates a screenshot, analyzes it for enemies under specified conditions (for example, searching for players wearing red clothes), calculates the angle of turn and makes a shooting. The system provides for the addition of new modules and commands.

# Libraries
- Vosk 0.3.45 (speech recognition)
- PortAudio 19.7.0 (reading data from the microphone)
- OpenCV 4.7.0 (computer vision)
- Simpleson 1.1.0 (work with json, was chosen because of its small size)

# Architecture
- SmartCompanion - the set of classes responsible for character creation, shooting, and projectile generation. Inside SmartCompanionGameMode is a pointer to CommandHandler.
- CommandHandler is a class that implements the FRunnable interface and handles recognized commands. 
- UESpeechRecognition - reading data from the microphone, recognizing and parsing json.
- UEComputerVision - image preprocessing and postprocessing, rotation angle calculation.

# Models
- vosk-model-small-en-us-0.15
- YOLOv8s

# Screenshots
- Speech Recognition

![Image alt](https://github.com/ShadowrunnerLeon/SmartCompanion/raw/main/Screenshots/nlp.png)

- Computer Vision

![Image alt](https://github.com/ShadowrunnerLeon/SmartCompanion/raw/main/Screenshots/cv.png)

# P.S.
- The OpenCV library was built from source code rather than using an off-the-shelf library. The IPP flags were disabled during the build (see useful links) and ONNX was connected. In any case, if you are not satisfied with the library version in this project, build OpenCV from source.

- A separate dll was created for the computer vision module because memory leaks occurred when working with UE. If you need to change the code or rebuild the dll, see DllProjects.

- Relevant to Shipping. After building and running the application, there is a possibility that the project will not use the maximum resolution of your monitor, which will lead to incorrect operation of the computer vision module. This link will help you (https://forums.unrealengine.com/t/how-to-set-resolution-of-packaged-game/18950/16).

# Useful links

## General
- https://forums.unrealengine.com/t/how-to-set-resolution-of-packaged-game/18950/16

## UE5 - adding libs
- https://forums.unrealengine.com/t/unreal-engine-5-stuck-75-initializing/668571
- https://unrealcommunity.wiki/multithreading-with-frunnable-2a4xuf68
- https://forums.unrealengine.com/t/unresolved-external-symbol-for-custom-third-party-plugin-integrating-vosk/487496/2
- https://unrealcommunity.wiki/adding-custom-third-party-library-to-plugin-from-scratch-867b28

## Computer Vision
- https://github.com/thanujanVeracity/yolov_8_two_leaves_and_a_bud-/commit/4b866c97180842b546fe117610869d3c8d69d8ae#diff-8eb622789bc750d2aad29d5426d995d376cbd3847fdc62ec2e42c8bbf8f2aae8
- https://github.com/microsoft/onnxruntime/releases/tag/v1.15.0
- https://github.com/ultralytics/ultralytics

## Speech Recognition
- https://alphacephei.com/vosk/models