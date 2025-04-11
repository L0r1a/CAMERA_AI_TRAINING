
from ultralytics import YOLO
import cv2
import math 
# start webcam
cap = cv2.VideoCapture(0)
cap.set(3, 640)
cap.set(4, 480)

# model
model = YOLO("YOLOv5_custom.pt")
model.cuda()

# object classes
classNames = ["Eye","Face","Mouth","Nose"]


while True:
    success, img = cap.read()
    img_YOLO = img.copy()
    results = model(img_YOLO, stream=True)

    # coordinates
    for r in results:
        boxes = r.boxes

        for box in boxes:
            # bounding box
            x1, y1, x2, y2 = box.xyxy[0]
            x1, y1, x2, y2 = int(x1), int(y1), int(x2), int(y2) # convert to int values

            # put box in cam
            cv2.rectangle(img_YOLO, (x1, y1), (x2, y2), (255, 0, 255), 3)

            # Độ tự tin - Confidence
            confidence = math.ceil((box.conf[0]*100))/100
            print("Confidence --->",confidence)

            # Tên lớp - Classname
            cls = int(box.cls[0])
            print("Class name -->", classNames[cls])

            # In thông tin vật thể / Chi tiết
            org = [x1, y1]
            font = cv2.FONT_HERSHEY_SIMPLEX
            fontScale = 1
            color = (255, 0, 0)
            thickness = 2

            cv2.putText(img_YOLO, classNames[cls], org, font, fontScale, color, thickness)

    cv2.imshow('YOLOv5_custom Output', img_YOLO)
    cv2.imshow('Webcam', img)
    if cv2.waitKey(1) == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()