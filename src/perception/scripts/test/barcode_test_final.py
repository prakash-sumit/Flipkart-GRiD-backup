import cv2
import numpy as np

def has_large_white_sticker(image_path):
    # Load the image
    image = cv2.imread(image_path)
    
    # Convert the image to grayscale
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    
    # Apply Gaussian blur to reduce noise
    blurred = cv2.GaussianBlur(gray, (5, 5), 0)
    
    # Apply color thresholding to detect white regions
    _, thresh = cv2.threshold(blurred, 150, 230, cv2.THRESH_BINARY)
    cv2.imwrite('thresh.png', thresh)
    
    # Find contours in the thresholded image
    contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    
    # Check for large white sticker based on contour area
    for contour in contours:
        area = cv2.contourArea(contour)
        print(area)
        if area > 1000:  # Adjust this threshold based on your needs
            return "yes"
    
    return "no"

# Provide the path to the image
image_path = "/home/umic/sumit2_ws/barcode.jpg"
result = has_large_white_sticker(image_path)
print(result)