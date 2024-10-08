# CNN-Based Steering Angle Estimation

In this university project, we designed and trained a CNN model capable of estimating the steering angle from an input image of a steering wheel. The project utilized an ESP32-CAM Dev board for image capture and an MPU6050 IMU sensor for measuring corresponding steering angles.

## Table of Contents
1. [Introduction](#introduction)
2. [Features](#features)
3. [Project Structure](#project-structure)
4. [Image Processing Layer](#image-processing-layer)
5. [Dataset](#dataset)
6. [Technologies & Tools Used](#technologies--tools-used)
7. [Video Demonstration](#video-demonstration)
8. [How to Run the Project](#how-to-run-the-project)
9. [Team Members](#team-members)
10. [Contributing](#contributing)
11. [License](#license)

## Introduction
This project revolves around using a **Convolutional Neural Network (CNN)** to estimate the steering angle of a vehicle from images of its steering wheel. The images were captured using an **ESP32-CAM** Dev board, and the corresponding angles were measured with an **MPU6050 IMU sensor**. We then trained the CNN to predict the angles from the images, achieving surprisingly accurate results even with low-quality images.

---

## Features
- Image capture using **ESP32-CAM** and corresponding angle measurement using **MPU6050**.
- CNN model for **steering angle estimation**.
- Comparison between CNN-predicted and **IMU-measured angles**.
- Real-time data collection and analysis.

---

## Project Structure
- **ESP32-CAM Setup**: The ESP32-CAM captures images of the steering wheel’s positions, while an **MPU6050 IMU** sensor measures the corresponding angles.
- **Data Collection**: The IMU readings are stored in a **CSV file**, and images are saved as **JPEG** files.
- **Model Training**: The **CNN** is trained using images as input and angles as labels.
  
![Camera setup image placeholder](image-link-here)
![IMU setup image placeholder](image-link-here)

---

## Image Processing Layer
We used the **Keras** library to design the CNN model, with an input size of **800x600 RGB images**. The output is a vector **[cosθ, sinθ]** where θ represents the steering angle. The model uses a **tanh** activation function in the final layer to ensure continuous outputs.

---

## Dataset
Our dataset includes:
- **Images**: Captured using **ESP32-CAM**.
- **Steering Angles**: Measured using the **MPU6050 IMU**.
  
Data was split into **90% for training** and **10% for validation**.

---

## Technologies & Tools Used
- **Convolutional Neural Networks (CNN)**
- **ESP32-CAM Microcontroller**
- **MPU6050 IMU Sensor**
- **Python (Programming Language)**
- **Keras (Deep Learning Library)**
- **C++** for microcontroller programming

---

## Video Demonstration
[![Video Demonstration](video-placeholder-link)](actual-video-link)

---

## How to Run the Project
1. Clone the repository.
2. Install dependencies using:
   ```bash
   pip install -r requirements.txt

## Team Members
- **Arman Javan Sekhavat**
- **Amin Elmi Ghiasi**
- **Mohammad Maleki Abyaneh(me)**

---

## Contributing
Contributions are always welcome! If you would like to collaborate, feel free to:
1. Fork this repository.
2. Create a branch for your feature (`git checkout -b feature-branch`).
3. Commit your changes (`git commit -m 'Add feature'`).
4. Push to the branch (`git push origin feature-branch`).
5. Open a pull request.

---

## License
This project is licensed under the **MIT License**. You are free to use, modify, and distribute this project. See the [LICENSE](LICENSE) file for more details.



