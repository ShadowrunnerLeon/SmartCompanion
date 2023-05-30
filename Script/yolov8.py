import pyautogui
import cv2
import numpy as np

#myScreenshot = pyautogui.screenshot()
#myScreenshot.resize(640, 359)
#myScreenshot.save(r'D:\SmartCompanion\SmartCompanion\Screenshots\screen.png')

modelPath = 'C:\\Users\\sorok\\Downloads\\yolov8\\runs\\detect\\train\\weights\\best.onnx'

net = cv2.dnn.readNet(modelPath)
img = cv2.imread('C:\\Users\\sorok\\Downloads\\yolov8\\red\\test\\images\\7.png')

(height, width) = img.shape[:2]
xFactor = width / 640
yFactor = height / 640

# Define the neural network input
blob = cv2.dnn.blobFromImage(img, 1 / 255.0, (640, 640), swapRB=True, crop=False)
net.setInput(blob)

# Perform forward propagation
outputs = net.forward()

outputs = np.array([cv2.transpose(outputs[0])])
rows = outputs.shape[1]

boxes = []
scores = []
class_ids = []

for i in range(rows):
    classes_scores = outputs[0][i][4:]
    (minScore, maxScore, minClassLoc, (x, maxClassIndex)) = cv2.minMaxLoc(classes_scores)
    if maxScore >= 0.25:
        box = [
            int((outputs[0][i][0] - (0.5 * outputs[0][i][2])) * xFactor), 
            int((outputs[0][i][1] - (0.5 * outputs[0][i][3])) * yFactor),
            int(outputs[0][i][2] * xFactor),
            int(outputs[0][i][3] * yFactor)]
        boxes.append(box)
        scores.append(maxScore)

result_boxes = cv2.dnn.NMSBoxes(boxes, scores, 0.25, 0.45, 0.5)

for i in range(len(result_boxes)):
    index = result_boxes[i]
    box = boxes[index]
    cv2.rectangle(img, (int(box[0]), int(box[1])), (int(box[0] + box[2]), int(box[1] + box[3])), (0, 255, 0), 2)

    cv2.imshow("YOLOV8 Detection", img)
    while True:     
        if cv2.waitKey(1) & 0xFF == ord('q'):
            # center coords
            text = str(int((box[0] + box[2]) / 2)) + '\n' + str(int((box[1] + box[3]) / 2))
            with open('D:\\SmartCompanion\\SmartCompanion\\Script\\coords.txt', 'w') as f:
                f.write(text)

            exit(0)

     