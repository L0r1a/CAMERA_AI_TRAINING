import torch
from ultralytics import YOLO
if __name__ == '__main__':
    
    torch.cuda.empty_cache()

    model = YOLO("yolov5s.pt")
    model.train(epochs=40, data="face_data.yaml",batch=4,imgsz=640,workers=1)