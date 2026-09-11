# AI-Powered Smart Recycle Bin

🥇 **First Place -- SEA IMechE Design Competition 2026.**

This project forms part of an integrated smart waste management solution combining:

- AI-based waste classification
- Smart waste collection route optimization
- Recycling incentive mobile application
- Physical automated waste-sorting prototype

---

## Project Overview

The project proposes an integrated smart waste management system that addresses waste sorting, waste collection efficiency, and public recycling participation.

The complete solution consists of four major components:

1. **AI Waste Classification**  
   A YOLOv8s object-detection model classifies recyclable waste into Glass, Metal, and Plastic.

2. **Smart Collection Route Optimization**  
   A simulation evaluates more efficient waste-collection routes based on bin conditions instead of relying only on fixed collection schedules.

3. **EcoSphere Recycling Incentive Application**  
   A mobile application encourages recycling participation through EcoPoints, QR scanning, recycling guidance, and voucher redemption.

4. **Physical Automated Sorting Prototype**  
   A functional prototype uses an ESP32 and servo motors to open the corresponding waste compartment based on the AI prediction.

---

## My Role and Contribution

I was responsible primarily for the machine-learning pipeline and AI-to-hardware integration.

### Machine Learning
- Developed the object-detection pipeline
- Used YOLOv8n as a baseline model for model-development experiments
- Tuned epochs, batch size, image size, and augmentation strategy
- Compared candidate YOLO models using the finalized training configuration
- Selected YOLOv8s as the final deployment model
- Analyzed precision, recall, mAP, and confusion-matrix results
- Implemented real-time OpenCV inference

### Deployment
- Implemented confidence-based detection filtering
- Implemented continuous detection-time validation
- Integrated YOLO predictions with ESP32 over WiFi/HTTP
- Developed class-to-servo command mapping

### Embedded Integration
- Integrated four continuous-rotation SG90 servos
- Implemented individual servo calibration
- Developed actuator testing and calibration utilities
- Implemented WiFi communication between the AI system and ESP32

### System Recovery
- Migrated inference from Raspberry Pi to PC following Raspberry Pi instability
- Redesigned the architecture while maintaining ESP32-based actuator control
- Continued the prototype development without requiring changes to the trained YOLO model

---

## Overall System Architecture
The complete smart waste management system consists of multiple connected subsystems.
<p align="center">
  <img width="1152" height="934" alt="image" src="https://github.com/user-attachments/assets/33a8dfef-202c-4476-93e6-66281f70b959" />
</p>

---

## Machine Learning Pipeline
The model-development process was carried out in stages rather than immediately selecting YOLOv8s. 
YOLOv8n was first adopted as the baseline model based on prior research. It was used to determine a suitable training configuration before comparing other candidate models. 
This allowed the same optimized training conditions to be applied during model comparison, providing a more consistent basis for final model selection.
The pipeline is as shown:
<p align="center">
  <img width="1753" height="814" alt="image" src="https://github.com/user-attachments/assets/0304ccf2-88cb-4ad1-bb0a-ebd9b969ffe4" />
</p>

---

### Training Configuration
The final YOLOv8s model was trained using:
- **Model:** YOLOv8s
- **Epochs:** 75
- **Input image size:** 640 × 640
- **Batch size:** 16
- **Augmentation strength:** Light
- **Classes:** Glass, Metal, Plastic

---

### Model Performance
<div align="center">
  
  | Metric | Result |
| --- | --- |
| Precision | 79.7% |
| Recall | 72.2% |
| mAP@50 | 74.1% |
| mAP@50-95 | 65.8% |

</div>

- **Precision - 79.7%:** reliability of positive detections.
- **Recall - 72.2%:** proportion of relevant objects successfully detected.
- **mAP@50 - 74.1%:** mean average precision evaluated at IoU = 0.50.
- **mAP@50-95 - 65.8%:** stricter detection metric averaged across IoU threshold from 0.50 to 0.95.

---

### Trained Classes
The developed YOLOv8s model detects three recyclable waste classes:
1. Glass
2. Metal
3. Plastic
General waste is not currently a trained YOLO class and is implemented as a manual fallback path in the physical prototype.

---

### Detection Validation
The actuator is not triggered immediately after a single YOLO prediction.
A detection must satisfy:
1. Confidence ≥0.75
2. Same class continuously detected for ≥1.5 seconds
3. Cooldown period after actuation
This reduces accidental servo activation caused by short or unstable predictions.

---

### AI-to-ESP32 Integration
Once a valid recyclable class is confirmed, the PC sends an HTTP command to the ESP32 over WiFi.
The ESP32 provides separated endpoints:
/plastic
/glass
/metal
/general

The corresponding servo mapping is:
Glass   → Servo 1
Metal   → Servo 2
Plastic → Servo 3
General → Servo 4

This architecture separates AI inference from actuator control.

---

## Deployment Architecture Evolution
The initial deployment architecture used a Raspberry Pi for on-device inference.
During integration testing, the Raspberry Pi experienced system instability and unexpected reboot behaviour under the deployment workload.
To maintain development progress and improve inference performance, the system was redesigned as:
Webcam -> PC-based YOLOv8s inference -> Confidence + temporal validation -> WiFi / HTTP -> ESP32 -> Servo actuators
This separation allowed the PC to handle computationally intensive computer vision while the ESP32 remained responsible for deterministic hardware control.

## Demonstration
This demonstration video shows the working AI-Powered physical prototype and 30-day route optimization simulation for the garbage truck. It can be accessed via the link attached below:

**Video link on YouTube:** https://youtu.be/sIWVYZQ8Xho 

---

## Team and Acknowledgements
This project was developed collaboratively for the SEA IMechE Design Competition 2026.
The complete solution include:
- AI waste sorting
- Smart collection route optimization
- EcoSphere recycling incentive mobile application
- Physical automated sorting prototype
My individual contribution focused primarily on the AI pipeline, YOLO model development, real-time deployment, and ESP32 integration.
The route optimization, mobile application, physical prototype development, and other system components were developed collaboratively by the project team.

### Team Member
1. Yap Peng Kun
2. Ng Chuen Hoong
3. Lloyd Foo Voon Xuan
4. Sim Zhong Sen
5. Ong Xiang Bao
